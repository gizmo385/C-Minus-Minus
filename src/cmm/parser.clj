(ns cmm.parser
  "This namespace contains functions related to parsing C files. This process lexical analysis,
   type checking, and error handling."
  (:require [instaparse.core :as insta]
            [clojure.edn :as edn]
            [clojure.pprint :refer [pprint]]
            [cmm.symbol-table :as sym]
            [cmm.types :as types]))

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
  (fn [symbol-table program] (first program)))

(defmethod build-ast :START [symbol-table program]
  (loop [program (next program)
         ast []
         symbol-table symbol-table]
    (if program
      (let [element (first program)]
        ;; Handle proper symbol table entries
        (condp = (first element)
          :VARIABLE_DECLARATION
          (let [variable (build-ast symbol-table element)]
            (recur (next program)
                   (conj ast variable)
                   (sym/add-declarations symbol-table [variable])))
          :FUNCTION_DECL
          (let [function (build-ast (sym/new-symbol-table symbol-table) element)]
              ;; Don't print an error since sym/add-function will handle errors
              (recur (next program)
                     ast ; Don't add declarations to AST, not useful
                     (sym/add-function symbol-table function false)))
          :FUNCTION
          (let [function (build-ast (sym/new-symbol-table symbol-table) element)]
            ;; Don't print an error since sym/add-function will handle errors
            (recur (next program)
                   (if (sym/function-defined? symbol-table function) ast (conj ast function))
                   (sym/add-function symbol-table function true)))))
      ast)))


;;; Parsing functions and function declarations
(defmethod build-ast :FUNCTION_DECL [symbol-table [_ & fields]]
  (if (= (first (first fields)) :extern)
    (let  [[_ [return-type] [_ function-name] params] fields]
      {:function-name function-name
       :return-type (keyword return-type)
       :params (map (partial build-ast symbol-table) (next params))
       :extern true
       :defined false})
    (let  [[[return-type] [_ function-name] params] fields]
      {:function-name function-name
       :return-type (keyword return-type)
       :params (map (partial build-ast symbol-table) (next params))
       :extern false
       :defined false})))

(defmethod build-ast :FUNCTION
  [symbol-table [_ [return-type] [_ function-name] params declarations & body]]
  (let  [params (build-ast symbol-table params)
         symbol-table (sym/add-parameters symbol-table params)
         declarations (build-ast symbol-table declarations)
         symbol-table (sym/add-declarations symbol-table declarations)]
    {:function-name function-name
     :return-type return-type
     :params params
     :declarations declarations
     :body (map (partial build-ast symbol-table) body)}))


(defmethod build-ast :PARAMS [symbol-table [_ & params]]
  (map (partial build-ast symbol-table) params))

(defmethod build-ast :PARAM [symbol-table [_ param-type param-id array-brackets?]]
  {:name (second param-id)
   :type (keyword (second param-type))
   :array (some? array-brackets?)})

;;; Variable declarations
(defmethod build-ast :DECLARATIONS [symbol-table [_ & declarations]]
  (map (partial build-ast symbol-table) declarations))

(defmethod build-ast :VARIABLE_DECLARATION [symbol-table [_ [_ variable-type] & ids]]
  {:node-type :declaration
   :type (keyword variable-type)
   :vars (map (partial build-ast symbol-table) ids)})

;;; Parsing expressions
(defmethod build-ast :OPT_EXPR [symbol-table [_ expression]]
  (if expression (build-ast symbol-table expression)))

(defmethod build-ast :EXPRESSION [symbol-table [_ expression]]
  (build-ast symbol-table expression))

(defmethod build-ast :BINARY_EXPRESSION [symbol-table [_ left-operand [operator] right-operand]]
  (let [left-operand (build-ast symbol-table left-operand)
        right-operand (build-ast symbol-table right-operand)]
    {:node-type :expression
     :type (types/compute-type operator (:type left-operand) (:type right-operand))
     :operator operator
     :left-operand left-operand
     :right-operand right-operand}))

(defmethod build-ast :UNARY_EXPRESSION [symbol-table [_ [operator] operand]]
  (let [operand (build-ast symbol-table operand)]
    {:node-type :expression
     :type (types/compute-type operator (:type operand))
     :operator operator
     :operand operand}))

(defmethod build-ast :FUNCTION_CALL [symbol-table [_ [_ function-name] & args]]
  (if-let [entry (sym/find-function-entry symbol-table function-name)]
    {:node-type :function-call
     :function-name function-name
     :type (:function-type entry)
     :arguments (map (partial build-ast symbol-table) args)}
    (printf "Error: Could not find function with name: %s\n" function-name)))
 
(defmethod build-ast :VARIABLE_ID [symbol-table [_ id size?]]
  {:name (second id)
   :array-size (if size? (build-ast symbol-table size?))})

(defmethod build-ast :ID [symbol-table [_ id]]
  (if-let [entry (sym/find-variable-entry symbol-table id)]
    {:node-type :expression
     :type (:symbol-type entry)
     :id id}
    (printf "Error: Could not locate symbol %s\n" id)))

(defmethod build-ast :INTEGER [symbol-table n]
  {:node-type :expression
   :type :integer
   :value (edn/read-string (second n))})

(defmethod build-ast :FLOAT [symbol-table n]
  {:node-type :expression
   :type :float
   :value (edn/read-string (second n))})

(defmethod build-ast :CHAR_CONST [symbol-table c]
  {:node-type :expression
   :type :char
   :value (char (second (second c)))})

(defmethod build-ast :STRING [symbol-table s]
  {:node-type :expression
   :type      :string
   :value     (second s)})

;;; Parsing different statement types
(defmethod build-ast :STATEMENT_LIST [symbol-table [_ & statements]]
  (map (partial build-ast symbol-table) statements))

(defmethod build-ast :IF [symbol-table [_ condition body]]
  {:node-type :statement
   :statement-type :conditional
   :condition (build-ast symbol-table condition)
   :then (build-ast symbol-table body)
   :else nil})

(defmethod build-ast :IF_ELSE [symbol-table [_ condition then else]]
  {:node-type :statement
   :statement-type :conditional
   :condition (build-ast symbol-table condition)
   :then (build-ast symbol-table then)
   :else (build-ast symbol-table else)})

(defmethod build-ast :OPT_ASSG [symbol-table [_ assignment]]
  (if assignment (build-ast symbol-table assignment)))

(defmethod build-ast :ASSIGNMENT [symbol-table [_ id first-expression second-expression]]
  (if (and first-expression second-expression)
    ;; Assignment to an array index
    {:node-type :statement
     :statement-type :assignment
     :id (second id)
     :index (build-ast symbol-table first-expression)
     :value (build-ast symbol-table second-expression)}

    ;; Assignment to a plain variable
    {:node-type :statement
     :statement-type :assignment
     :id (second id)
     :index nil
     :value (build-ast symbol-table first-expression)}))

(defmethod build-ast :RETURN [symbol-table [_ return-value?]]
  {:node-type :statement
   :statement-type :return
   :value (if return-value? (build-ast symbol-table return-value?))})

(defmethod build-ast :WHILE_LOOP [symbol-table [_ condition body]]
  {:node-type :statement
   :statement-type :while-loop
   :condition (build-ast symbol-table condition)
   :body (build-ast symbol-table body)})

(defmethod build-ast :FOR_LOOP [symbol-table [_ init? condition? update? body]]
  {:node-type :statement
   :statement-type :for-loop
   :init (build-ast symbol-table init?)
   :condition (build-ast symbol-table condition?)
   :update (build-ast symbol-table update?)
   :body (build-ast symbol-table body)})

;;; Handling invalid parse elements
(defmethod build-ast :default [symbol-table structure]
  (printf "Can't parse a %s\n" (first structure)))

;;; User facing functions
(defn parse [source]
  (let [parse-result (insta/parse c-parser source)]
    (if-let [failure (insta/get-failure parse-result)]
      failure
      (build-ast (sym/new-symbol-table) parse-result))))