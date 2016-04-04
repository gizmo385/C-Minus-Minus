(ns cmm.parser
  (:require [instaparse.core :as insta]
            [clojure.edn :as edn]
            [clojure.pprint :refer [pprint]]))

;; Loading the parser from the grammar file
(def c-parser
  (insta/parser (clojure.java.io/resource "grammar.bnf") :auto-whitespace :standard))

;; Defining operator classes
(def arithmetic-operations
  "Operations which involve arithmetic"
  [:plus :minus :multiply :divide])

(def comparison-operations
  "Operations which involve comparisons between values"
  [:gte :lte :gt :lt :eq :neq])

(def boolean-operations
  "Operations which operate on booleans"
  [:and :or :not])

;; Defining type relationships
(def number-types
  "The types of numbers available, ordered based precision."
  [:float :integer :char])

(defn assert-of-type
  "Asserts that all of types to check are of one of the types to assert"
  [types-to-assert types-to-check]
  (not-any? nil? (for [t types-to-check]
    (some #{t} types-to-assert))))

(defn operand-types-for-operator
  "These are the types of operands that can be supplied to the various expression operators. Those
   which can have multiple operand types are collections, whereas those which have a constant
   operand type are expressed as static values."
  [operator]
  (cond
    (some #{operator} arithmetic-operations) number-types
    (some #{operator} comparison-operations) (conj number-types :boolean)
    (some #{operator} boolean-operations) [:boolean]))

(defn result-types-for-operator
  "These are the types that can result from various expression operators. Those which can have
   multiple resultant types are collections, whereas those which have a constant result type are
   expressed as static values."
  [operator]
  (cond
    (some #{operator} arithmetic-operations)  number-types
    (some #{operator} comparison-operations)  :boolean
    (some #{operator} boolean-operations)     :boolean))

(defn filter-types
  "Filters a list of actual types based on types present."
  [possible-types actual-types]
  (filter (fn [t] (some #{t} actual-types)) possible-types))

(defn one-of
  "Determines if any of the actual-values are one of the possible-values."
  [possible-values actual-values]
  (not-every? false?
    (for [value possible-values
          actual-value actual-values]
      (= value actual-value))))

(defn compute-type
  "Computes the type of an expression based upon its operator and operands"
  [operator & operand-types]
  (cond
    ;; Propogate errors upward
    (one-of [nil :error] (list operand-types)) :error

    ;; Check operand types
    (assert-of-type (operand-types-for-operator operator) operand-types)
    (if (coll? (result-types-for-operator operator))
      ;; Collections of result types are expressions which can have multiple result types, like
      ;; those found in arithmetic expressions
      (first (filter-types (result-types-for-operator operator) operand-types))

      ;; Singular return types are always the same, such as booleans from comparison operations
      (result-types-for-operator operator))

    :default :error))

;; Building the ast
(defmulti build-ast first)

(defmethod build-ast :START [program]
  (map build-ast (rest program)))

(defmethod build-ast :FUNCTION [[_ [return-type] [_ function-name] params & body]]
  {:name function-name
   :type return-type
   :params (build-ast params)
   :body (map build-ast body)})

(defmethod build-ast :FUNCTION_CALL [[_ [_ function-name] & args]]
  {:node-type :function-call
   :function-name function-name
   :arguments (map build-ast args)})

(defmethod build-ast :PARAMS [[_ & params]]
  (map build-ast params))

(defmethod build-ast :PARAM [[_ param-type param-id array-brackets?]]
  {:name (second param-id)
   :type (second param-type)
   :array (some? array-brackets?)})

(defmethod build-ast :VARIABLE_DECLARATION [[_ [_ variable-type] & ids]]
  {:node-type :declaration
   :type (keyword variable-type)
   :vars (map build-ast ids)})

(defmethod build-ast :VARIABLE_ID [[_ id size?]]
  {:name (second id)
   :array-size (if size? (build-ast size?))})

(defmethod build-ast :OPT_EXPR [[_ expression]]
  (if expression (build-ast expression)))

(defmethod build-ast :EXPRESSION [[_ expression]]
  (build-ast expression))

(defmethod build-ast :BINARY_EXPRESSION [[_ left-operand [operator] right-operand]]
  (let [left-operand (build-ast left-operand)
        right-operand (build-ast right-operand)]
    {:node-type :expression
     :type (compute-type operator (left-operand :type) (right-operand :type))
     :operator operator
     :left-operand left-operand
     :right-operand right-operand}))

(defmethod build-ast :UNARY_EXPRESSION [[_ [operator] operand]]
  (let [operand (build-ast operand)]
    {:node-type :expression
     :type (compute-type operator (operand :type))
     :operator operator
     :operand operand}))

(defmethod build-ast :ID [[_ id]]
  {:node-type :expression
   :type :id ;; TODO Figure out symbol table
   :id id})

(defmethod build-ast :INTEGER [n]
  {:node-type :expression
   :type :integer
   :value (edn/read-string (second n))})

(defmethod build-ast :FLOAT [n]
  {:node-type :expression
   :type :float
   :value (edn/read-string (second n))})

(defmethod build-ast :CHAR_CONST [c]
  {:node-type :expression
   :type :char
   :value (char (second (second c)))})

(defmethod build-ast :STRING [s]
  {:node-type :expression
   :type      :string
   :value     (second s)})

(defmethod build-ast :STATEMENT_LIST [[_ & statements]]
  (map build-ast statements))

(defmethod build-ast :IF [[_ condition body]]
  {:node-type :statement
   :statement-type :conditional
   :condition (build-ast condition)
   :then (build-ast body)
   :else nil})

(defmethod build-ast :IF_ELSE [[_ condition then else]]
  {:node-type :statement
   :statement-type :conditional
   :condition (build-ast condition)
   :then (build-ast then)
   :else (build-ast else)})

(defmethod build-ast :OPT_ASSG [[_ assignment]]
  (if assignment (build-ast assignment)))

(defmethod build-ast :ASSIGNMENT [[_ id first-expression second-expression]]
  (if (and first-expression second-expression)
    ;; Assignment to an array index
    {:node-type :statement
     :statement-type :assignment
     :id (second id)
     :index (build-ast first-expression)
     :value (build-ast second-expression)}

    ;; Assignment to a plain variable
    {:node-type :statement
     :statement-type :assignment
     :id (second id)
     :index nil
     :value (build-ast first-expression)}))

(defmethod build-ast :RETURN [[_ return-value?]]
  {:node-type :statement
   :statement-type :return
   :value (if return-value? (build-ast return-value?))})

(defmethod build-ast :WHILE_LOOP [[_ condition body]]
  {:node-type :statement
   :statement-type :while-loop
   :condition (build-ast condition)
   :body (build-ast body)})

(defmethod build-ast :FOR_LOOP [[_ init? condition? update? body]]
  {:node-type :statement
   :statement-type :for-loop
   :init (build-ast init?)
   :condition (build-ast condition?)
   :update (build-ast update?)
   :body (build-ast body)})

(defmethod build-ast :default [structure]
  (printf "Can't parse a %s\n" (first structure)))

;; User facing functions
(defn parse [source]
  (as-> source <>
    (insta/parse c-parser <>)
    (do (println <>) <>)
    (build-ast <>)))

(def example
  "
  void testing(void) {
   int x, y;
   printf(5);
   printf(5.7);
   printf(1 + 2 - (4 * 7.2));

   for (;;) printf(y);
   printf(g);
   }
   void x(int g) { return 6; }
   ")

(comment
  (parse "int x; char a, b[5];")
  (parse "void g(void) { if(x) { printf(y); }}")
  (parse "void g(void) { while(x) { printf(x); printf(y); }}")
  (parse "void g(void) { for(;;) { printf(y); }}")
  (parse "void g(void) { for(x = 5;;) { printf(y); }}")
  (parse "void g(void) { for(x = 5; x < 5;) { printf(y); }}")
  (parse "void g(void) { for(x = 5; x < 5; x = x + 1) { printf(y); }}")
  (parse "void g(void) { x = 5; }")
  (parse "void g(void) { return 5; }")
  (parse "void g(void) { return; }")
  (parse "void g(void) { x[7] = 5; }")
  (parse "void g(void) { if(x) printf(y); else printf(z); }")
  (parse "void printf(char x[], int g) { return; }")
  (parse "void x(void) { printf(testing, 1 + 1, 'c'); }")
  (parse example))
