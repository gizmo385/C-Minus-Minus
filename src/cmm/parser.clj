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

(defn ->ast
  "This function takes a parser node and builds an Abstract Syntax Tree out of that parser node. It
   will also use that parser node as the current context under which errors will be generated."
  ([parser-node]
   (->ast (sym/new-symbol-table) parser-node))

  ([symbol-table parser-node]
   (err/with-error-context parser-node
     (let [[updated-symbol-table ast] (build-ast symbol-table parser-node)]
       [(or updated-symbol-table symbol-table)
        (or ast {:node-type   :error
                 :parser-node parser-node})]))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Parsing top-level symbols
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defmethod build-ast :START
  [symbol-table [_ & program]]
  (loop [program program
         symbol-table symbol-table
         ast []]
    (if (first program)
      (let [[updated-symbol-table sub-ast] (->ast symbol-table (first program))]
        (recur (rest program)
               updated-symbol-table
               (conj ast sub-ast)))
      [symbol-table ast])))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Parsing functions and parameters
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defn- handle-statements
  [symbol-table statements]
  (loop [symbol-table symbol-table
         ast []
         statements statements]
    (if (first statements)
      (let [[updated-symbol-table node-ast] (->ast symbol-table (first statements))]
        (recur updated-symbol-table
               (conj ast node-ast)
               (rest statements)))
      [symbol-table ast])))

(defmethod build-ast :FUNCTION
  [symbol-table [_ [type] [_ name] arguments declarations & statements]]
  (let [arg-types []
        ;; First we want to add the function to the current symbol table
        updated-outer-scope                   (sym/add-function symbol-table type name arg-types)

        ;; Then we'll create a nested symbol table for the inside of the function
        function-inner-scope                  (sym/new-symbol-table updated-outer-scope)

        ;; Now we'll build ASTs and update symbol table based on the arguments, declarations, and
        ;; the statements inside of the body of the function
        [argument-scope argument-ast]         (->ast function-inner-scope arguments)
        [declaration-scope declaration-ast]   (->ast argument-scope declarations)
        [statement-scope statement-ast]       (handle-statements declaration-scope statements)

        ;; Lastly we'll join the ASTs for the declarations and the statements, as they make up the
        ;; AST for the entire body of the function
        body-ast                              (concat declaration-ast statement-ast)]
    [statement-scope
     {:node-type    :function
      :name         name
      :return-type  type
      :arguments    argument-ast
      :body         body-ast
      :symbol-table statement-scope}]))

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

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Parsing variable declarations
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defn- handle-declarations
  "Handles the parsing out the comma-separated names in a variable declaration."
  [symbol-table type declarations]
  ;; We want to loop over the various individual declarations in the comma-separated list.
  (loop [declarations declarations
         ast-nodes []
         symbol-table symbol-table]
    ;; For each, we'll extract the name and array size (if presnet) of each declaration and create
    ;; a new AST node.
    (if-let [[_ [_ name] [_ array-size]] (first declarations)]
      (let [new-ast-node {:node-type  :variable-declaration
                          :name       name
                          :array?     (some? array-size)
                          :array-size array-size
                          :type       type}]
        ;; We'll add a new entry to the symbol table and go back around the loop to tackle the next
        ;; element in the declaration.
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

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Expression Parsing
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defn- literal->ast
  "This wraps a simple value type's parser node into an AST node. For example, [:INTEGER 5] would
   get returned as:
      {:node-type  :literal
      :type        :INTEGER
      :value       5}"
  [[type value]]
  {:node-type :literal
   :type      type
   :value     value})

(defn- expression->ast
  [symbol-table expression]
  (let [expression-type (first expression)]
    (case expression-type
      ;; For any sort of nested expression, we'll dispatch out to the build-ast function and
      ;; explicitly return only the generated AST
      :BINARY_EXPRESSION  (second (->ast symbol-table expression))
      :UNARY_EXPRESSION   (second (->ast symbol-table expression))
      :FUNCTION_CALL      (second (->ast symbol-table expression))
      :ARRAY_ACCESS       (second (->ast symbol-table expression))

      ;; For an ID, we'll just create a simple variable node after checking that the name is defined
      ;; in the supplied symbol table
      :ID (let [variable-name (second expression)
                variable (sym/variable-defined? symbol-table variable-name)]
            (if (nil? variable)
              (err/raise-error! "Could not find variable %s in expression\n" variable-name)
              {:node-type     :variable
               :type          (:type variable)
               :variable-name variable-name}))

      ;; By default, we'll assume the expression is just a value
      (literal->ast expression))))

(defmethod build-ast :BINARY_EXPRESSION
  [symbol-table [_ left operator right]]
  ;; First we want to create ASTs for the left and right hand sides of the expression.
  (let [left-ast  (expression->ast symbol-table left)
        right-ast (expression->ast symbol-table right)]
    ;; Once we do that, we'll return an AST for the entire expression
    [symbol-table
     {:node-type      :binary-expression
      :left-operand   left-ast
      :right-operand  right-ast
      :operator       operator}]))

(defmethod build-ast :UNARY_EXPRESSION
  [symbol-table [_ operator operand]]
  ;; We need to create an AST for the operand.
  (let [operand-ast (expression->ast symbol-table operand)]
    [symbol-table
     {:node-type :unary-expression
      :operator operator
      :operand operand-ast}]))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Assignment Parsing
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defn- parse-expression-assignment
  [symbol-table destination-variable expression]
  (let [[_ expression-ast] (->ast symbol-table expression)]
    [symbol-table
     {:node-type        :expression-assignment
      :destination      (:name destination-variable)
      :destination-type (:type destination-variable)
      :expression       expression-ast}]))

(defn- parse-variable-assignment
  [symbol-table destination-variable [_ source]]
  (if-let [source-variable (sym/variable-defined? symbol-table source)]
    [symbol-table
     {:node-type        :variable-assignment
      :destination       (:name destination-variable)
      :destination-type  (:type destination-variable)
      :source            source
      :source-type       (:type source-variable)}]
    (err/raise-error! "Cannot assign '%s' to '%s', cannot find variable '%s'\n"
                      source (:name destination-variable) source)))

(defmethod build-ast :ASSIGNMENT
  [symbol-table [_ [_ destination] source]]
  (debug-msg "Symbols @ assignment of %s: %s\n" destination (with-out-str (pprint symbol-table)))
  ;; Since we're assigning to a variable, we need to check that the variable we're attempting to
  ;; assign to exists
  (if-let [destination-variable (sym/variable-defined? symbol-table destination)]
    ;; We want to determine what kind of assignment is taking place (literal, variable, expression)
    (let [[value-type value] source]
      (case value-type
        ;; These are cases where we'll need more complicate parsing to generate the AST for the
        ;; assignment
        :BINARY_EXPRESSION  (parse-expression-assignment symbol-table destination-variable source)
        :UNARY_EXPRESSION   (parse-expression-assignment symbol-table destination-variable source)
        :ID                 (parse-variable-assignment symbol-table destination-variable source)

        ;; In the default case (literal assignment), we'll just create a simple AST node
        ;; TODO: Type check the literal/destination
        ;; TODO: Handle assigning to an array index
        [symbol-table
         {:node-type        :literal-assignment
          :destination      destination
          :destination-type (:type destination-variable)
          :source           value
          :source-type      value-type}]))
    (err/raise-error! "Cannot assign to '%s', variable does not exist!\n" destination)))

(defmethod build-ast :default [symbol-table unrecognized-node]
  (let [unrecognized-tag (first unrecognized-node)]
    (err/raise-error! "Unrecognized symbol of with tag %s: %s\n" unrecognized-tag unrecognized-node)))

;;; These are functions to help us simplify aspects of our parsing (such as converting
;;; integers and floats)
(defonce transformation-map
  {:INTEGER (fn [v] [:INTEGER (read-string v)])
   :FLOAT   (fn [v] [:FLOAT (read-string v)])})

;;; User facing functions

(defn parse [source]
  (let [ast (->> source
                 (insta/parse c-parser)
                 (insta/transform transformation-map)
                 (insta/add-line-and-column-info-to-metadata source)
                 (->ast))]
    (debug-msg "Final AST: %s\n" (with-out-str (pprint ast)))
    ast))
