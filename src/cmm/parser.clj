(ns cmm.parser
  (:require [instaparse.core :as insta]
            [clojure.edn :as edn]
            [clojure.pprint :refer [pprint]]
            [cmm.symbol-table :as sym]
            [cmm.types :as types]))

;; Loading the parser from the grammar file
(def whitespace-or-comments
  (insta/parser (clojure.java.io/resource "whitespace.bnf") :auto-whitespace :standard))

(def c-parser
  (insta/parser (clojure.java.io/resource "grammar.bnf") :auto-whitespace whitespace-or-comments))

;; Building the ast
(defmulti build-ast (fn [symbol-table program] (first program)))

(defmethod build-ast :START [symbol-table program]
  (loop [program (next program)
         ast []
         symbol-table symbol-table]
    (if program
      (let [element (first program)]
        ;; Handle proper symbol table entries
        (condp = (first element)
          :VARIABLE_DECLARATION
          (let [element-ast (build-ast symbol-table element)]
            (recur (next program)
                   (conj ast element-ast)
                   (sym/add-declarations symbol-table [element-ast])))
          :FUNCTION_DECL
          (let [element-ast (build-ast (sym/new-symbol-table symbol-table) element)]
            (recur (next program)
                   (conj ast element-ast)
                   symbol-table))
          :FUNCTION
          (let [element-ast (build-ast (sym/new-symbol-table symbol-table) element)]
            (recur (next program)
                   (conj ast element-ast)
                   symbol-table))))
      ast)))

(defmethod build-ast :FUNCTION_DECL [symbol-table decl] nil) ;; TODO

(defmethod build-ast :FUNCTION
  [symbol-table [_ [return-type] [_ function-name] params declarations & body]]
  (let  [params (build-ast symbol-table params)
         symbol-table (sym/add-parameters symbol-table params)
         declarations (build-ast symbol-table declarations)
         symbol-table (sym/add-declarations symbol-table declarations)]
    {:name function-name
     :type return-type
     :params params
     :declarations declarations
     :body (map (partial build-ast symbol-table) body)}))

(defmethod build-ast :FUNCTION_CALL [symbol-table [_ [_ function-name] & args]]
  {:node-type :function-call
   :function-name function-name
   :arguments (map (partial build-ast symbol-table) args)})

(defmethod build-ast :PARAMS [symbol-table [_ & params]]
  (map (partial build-ast symbol-table) params))

(defmethod build-ast :PARAM [symbol-table [_ param-type param-id array-brackets?]]
  {:name (second param-id)
   :type (second param-type)
   :array (some? array-brackets?)})

(defmethod build-ast :DECLARATIONS [symbol-table [_ & declarations]]
  (map (partial build-ast symbol-table) declarations))

(defmethod build-ast :VARIABLE_DECLARATION [symbol-table [_ [_ variable-type] & ids]]
  {:node-type :declaration
   :type (keyword variable-type)
   :vars (map (partial build-ast symbol-table) ids)})

(defmethod build-ast :VARIABLE_ID [symbol-table [_ id size?]]
  {:name (second id)
   :array-size (if size? (build-ast symbol-table size?))})

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

(defmethod build-ast :ID [symbol-table [_ id]]
  (if-let [entries (sym/find-variable-entry symbol-table id)]
    (if (= 1 (count entries))
      {:node-type :expression
       :type (:symbol-type (first entries))
       :id id}
      (printf "Error: Multiple symbol table entries found for id \"%s\": %s" id entries))
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

(defmethod build-ast :default [symbol-table structure]
  (printf "Can't parse a %s\n" (first structure)))

;; User facing functions
(defn parse [source]
  (->> source
    (insta/parse c-parser)
    (build-ast (sym/new-symbol-table))))

(comment
  (pprint (parse "int z; void g(int x) { char z; printf(z); }"))
  (pprint (parse (slurp (clojure.java.io/resource "test_code/type_checking/test19.c")))))
