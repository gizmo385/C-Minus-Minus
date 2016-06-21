(ns cmm.types
  "Handles the various type checking responsibilities for the parser."
  (:require [cmm.errors :as err]
            [clojure.string :refer [join]]))

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
  [:float :int :char])

(def type-size
  "Size of bytes of the default types"
  {:float 4
   :int 4
   :char 2})


(defn match-type [actual-type valid-types operator]
  (if-let [t (some #{actual-type} valid-types)]
    t
    (err/raise-error! "Expected one of [%s] for operator %s, found %s\n"
                      (join ", " valid-types)
                      operator
                      actual-type)))

(defn assert-of-type
  "Asserts that all of types to check are of one of the types to assert"
  [types-to-assert types-to-check operator]
  (not-any? nil? (for [t types-to-check] (match-type t types-to-assert operator))))

(defn operand-types-for-operator
  "These are the types of operands that can be supplied to the various expression operators. Those
   which can have multiple operand types are collections, whereas those which have a constant
   operand type are expressed as static values."
  [operator]
  (cond
    (some #{operator} arithmetic-operations) number-types
    (some #{operator} comparison-operations) (conj number-types :boolean)
    (some #{operator} boolean-operations) [:boolean]))

(defn result-types-for-operator "These are the types that can result from various expression operators. Those which can have
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
    (assert-of-type (operand-types-for-operator operator) operand-types operator)
    (if (coll? (result-types-for-operator operator))
      ;; Collections of result types are expressions which can have multiple result types, like
      ;; those found in arithmetic expressions
      (first (filter-types (result-types-for-operator operator) operand-types))

      ;; Singular return types are always the same, such as booleans from comparison operations
      (result-types-for-operator operator))

    :default :error))

(defn types-compatible?
  "Determines whether or not two types are compatible. They're compatible under 3 circumstances:

   1: They are equal
   2: One of them is an error type. This prevents a single error from triggering multiple type
      checking errors
   3: They belong to the same class of types (number types for example)"
  [t1 t2]
  (cond
    (= t1 t2) true
    (or (= t1 :error) (= t2 :error)) true
    (some #{t2} number-types) (some? (some #{t1} number-types))))

(defn type-check-args
  "Type checks a function's arguments and raises an error if there is a type mismatch."
  [argument-types expected-types function-name]
  (doall
    (for [[ord t expected] (map vector (map inc (range)) argument-types expected-types)]
      (if (not (types-compatible? t expected))
        (err/raise-error! "Argument %d to %s, expected one of [%s], got %s"
                          ord function-name expected t)))))
