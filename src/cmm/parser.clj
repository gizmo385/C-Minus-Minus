(ns cmm.parser
  "This namespace contains functions related to parsing C files. This process lexical analysis,
  type checking, and error handling."
  (:require [instaparse.core :as insta]
            [clojure.edn :as edn]
            [clojure.string :refer [join]]
            [clojure.pprint :refer [pprint]]
            [cmm.errors :as err]
            [cmm.symbol-table :as sym]
            [cmm.types :as types]
            [cmm.debug :as dbg :refer [debug-msg]]))

;;; Loading the parser from the grammar file
(def whitespace-or-comments
  "This parser handles the whitespace and comment parsing. It is used as the whitespace parser in
   the primary C parser."
  (insta/parser (clojure.java.io/resource "whitespace.bnf") :auto-whitespace :standard))

(def c-parser
  "This is the primary C parser. It uses a custom whitespace parser which handles comments and
   whitespace characters. The results of this parser should be sent to the build-ast function."
  (insta/parser (clojure.java.io/resource "grammar.bnf") :auto-whitespace whitespace-or-comments))

;;; Building the ast
(defmulti build-ast
  "The build-ast function represents the primary parsing function for CMM. This uses the output
   from instaparse to generate a more easily understandable, parseable, and easy-to-use tree
   structure.

   The first argument represents the symbol table of the calling scope. The second argument
   represents a subsection of the parse result."
  (fn [symbol-table program]
    (first program)))

(defmethod build-ast :START
  [symbol-table [_ & program]]
  (debug-msg "Program: %s\n" (with-out-str (pprint program)))
  (loop [program program
         symbol-table symbol-table
         ast []]
    (if (first program)
      (let [[updated-symbol-table sub-ast] (build-ast symbol-table (first program))]
        (recur (rest program) updated-symbol-table (conj ast sub-ast))))))

(defmethod build-ast :FUNCTION
  [symbol-table [_ [type] [_ name] arguments declarations & statements]]
  (let [arg-types []
        ;; First we want to add the function to the current symbol table
        updated-outer-scope                   (sym/add-function symbol-table type name arg-types)

        ;; Then we'll create a nested symbol table for the inside of the function
        function-inner-scope                  (sym/new-symbol-table updated-outer-scope)

        ;; Now we'll build ASTs and update symbol table based on the arguments, declarations, and
        ;; the statements inside of the body of the function
        [argument-scope argument-ast]         (build-ast function-inner-scope arguments)
        [declaration-scope declaration-ast]   (build-ast argument-scope declarations)
        [statement-scope statement-ast]       (build-ast declaration-scope statements)

        ;; Lastly we'll join the ASTs for the declarations and the statements, as they make up the
        ;; AST for the entire body of the function
        body-ast                              (concat declaration-ast statement-ast)]
    (debug-msg "AST for body of %s: \n%s\n" name (with-out-str (pprint body-ast)))
    (debug-msg "Symbol table for %s: \n%s\n" name (with-out-str (pprint declaration-scope)))
    [statement-scope
     {:node-type    :function
      :name         name
      :return-type  type
      :arguments    argument-ast
      :body         body-ast
      :symbol-table declaration-scope}]))

(defmethod build-ast :PARAMS
  [symbol-table [_ & parameters]]
  (loop [symbol-table symbol-table
         parameters parameters
         asts []]
    (if (first parameters)
      ;; We'll destructure the parameter to retrieve type and name information. We'll also create a
      ;; new AST node
      (let [[_ [_ type] [_ name]] (first parameters)
            param-ast {:node-type :parameter
                       :name      name
                       :type      type}]
        ;; We'll add this new variable to the symbol table and proceed onto the next argument
        (recur (sym/add-variable symbol-table type name)
               (rest parameters)
               (conj asts param-ast)))
      [symbol-table asts])))

(defn- handle-declarations
  "Handles the parsing out the comma-separated names in a variable declaration."
  [symbol-table type declarations]
  (debug-msg "Names: %s\n" declarations)
  (loop [declarations declarations
         ast-nodes []
         symbol-table symbol-table]
    (if-let [[_ [_ name] array-size] (first declarations)]
      (let [new-ast-node {:node-type  :variable-declaration
                          :name       name
                          :array?     (some? array-size)
                          :array-size array-size
                          :type       type}]
        (recur
          (rest declarations)
          (conj ast-nodes new-ast-node)
          (sym/add-variable symbol-table type name array-size)))
      [symbol-table ast-nodes])))

(defmethod build-ast :DECLARATIONS
  [symbol-table [_ & declarations]]
  (loop [symbol-table symbol-table
         declarations declarations
         asts []]
    (if (first declarations)
      ;; We'll destructure the parameter to retrieve type and name information. We'll also create a
      ;; new AST node
      (let [[_ [_ type] & names] (first declarations)
            [updated-table decl-ast] (handle-declarations symbol-table type names) ]
        ;; We'll add this new variable to the symbol table and proceed onto the next argument
        (recur updated-table
               (rest declarations)
               (concat asts decl-ast)))
      [symbol-table asts])))

(defmethod build-ast :default [symbol-table [unrecognized & _]]
  (err/raise-error! "Unrecognized symbol of type %s\n" unrecognized))

;;; These are functions to help us simplify aspects of our parsing (such as converting
;;; integers and floats)
(defonce transformation-map
  {:INTEGER read-string
   :FLOAT   read-string})

;;; User facing functions
(defn parse [source]
  (->> source
       (insta/parse c-parser)
       (insta/transform transformation-map)
       (insta/add-line-and-column-info-to-metadata source)
       (build-ast (sym/new-symbol-table))))
