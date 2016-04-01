(ns cmm.parser
  (:require [instaparse.core :as insta]
            [clojure.edn :as edn]
            [clojure.set :as s]))

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

(defmethod build-ast :FUNCTION [program]
  ;; We can destructure the most of the important data out of this list
  (let [[[return-type] [_ function-name] params & body] (rest program)]
    {:name function-name
     :type return-type
     :params (build-ast params)
     :body (map build-ast body)}))

(defmethod build-ast :FUNCTION_CALL [call]
  (let [[[_ function-name] & args] (rest call)]
    {:node-type :function-call
     :function-name function-name
     :arguments (map build-ast args)}))

(defmethod build-ast :PARAM_LIST [params]
  ;; Destructure unnecssary information out of structure
  (for [[_ _ param-type _ param-name] (partition 5 (flatten params))]
    {:name param-name
     :type param-type}))

(defmethod build-ast :VARIABLE_DECLARATION [declaration]
  (let [[[_ variable-type] & ids] (rest declaration)
        variable-type (keyword variable-type)]
    {:node-type :declaration
     :type variable-type
     :vars (map build-ast ids)}))

(defmethod build-ast :VARIABLE_ID [structure]
  (if-let [[_ id size-expr] structure]
    {:name (second id)
     :array-size (build-ast size-expr)}
    {:name (second (second structure))
     :array-size nil}))

(defmethod build-ast :EXPRESSION [[_ expression]]
  (build-ast expression))

(defmethod build-ast :BINARY_EXPRESSION [expression]
  (let [[_ left-operand [operator _] right-operand] expression
        left-operand (build-ast left-operand)
        right-operand (build-ast right-operand)]
    {:node-type     :expression
     :type          (compute-type operator (left-operand :type) (right-operand :type))
     :operator      operator
     :left-operand  left-operand
     :right-operand right-operand}))

(defmethod build-ast :UNARY_EXPRESSION [expression]
  (let [[_ [operator _] operand] expression
        operand (build-ast operand)]
    {:node-type     :expression
     :type          (compute-type operator (operand :type))
     :operator      operator
     :operand operand}))

(defmethod build-ast :INTEGER [n]
  {:node-type :expression
   :type      :integer
   :value     (edn/read-string (second n))})

(defmethod build-ast :FLOAT [n]
  {:node-type :expression
   :type      :float
   :value     (edn/read-string (second n))})

(defmethod build-ast :FLOAT [n]
  {:node-type :expression
   :type      :float
   :value     (edn/read-string (second n))})

(defmethod build-ast :CHAR_CONST [c]
  {:node-type :expression
   :type      :char
   :value     (char (second (second c)))})

(defmethod build-ast :STRING [s]
  {:node-type :expression
   :type      :string
   :value     (second s)})

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
   }
   void x(int g) { return 6; }
   ")

(comment
  (parse "int x; char a, b[5];")
  (parse "void printf(char x[]) { return; } int number_5(void) { return 5; }")
  (parse "void x(void);")
  (parse "extern void x(void);")
  (parse "void x(void) { printf(!(((1 + 4) * 0.5) == 4)); }")
  (parse "void x(void) { printf('c'); }")
  (parse "void x(void) { printf(\"test\"); }")
  (insta/parse c-parser "void x(void) { printf(!(1 == 1)); }")
  (parse example)
  (build-ast (insta/parse c-parser "void x(int g, char x, float y) { }"))
  )
