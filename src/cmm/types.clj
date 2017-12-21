(ns cmm.types
  "Handles the various type checking responsibilities for the parser."
  (:require [cmm.errors :as err]
            [clojure.string :refer [join lower-case upper-case]]))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Defining the structure for our type system in the compiler
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defrecord Type [name parent])

;;; Bottom is the base type in the type system and is the eventual parent type for ALL types (except Void)
(defonce BOTTOM (Type. "Bottom" nil))
(defonce VOID (Type. "Void" nil))

;;; Numeric types
(defonce NUMBER   (Type. "Number" BOTTOM))
(defonce INTEGER  (Type. "Integer" NUMBER))
(defonce FLOAT    (Type. "Float" NUMBER))
(defonce DOUBLE   (Type. "Double" NUMBER))

;;; A character is under the Numeric tree, but is not a conventional Number Type
(defonce CHARACTER (Type. "Character" INTEGER))

;;; Additional Types
(defonce BOOLEAN (Type. "Boolean" BOTTOM))

(defonce keyword-type-map
  {:INTEGER   INTEGER
   :FLOAT     FLOAT
   :DOUBLE    DOUBLE
   :CHAR      CHARACTER
   :BOOLEAN   BOOLEAN
   :VOID      VOID})

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Some type-checking helper functions
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defn parent?
  "Determines if the supplied 'parent' type is actually in the direct type-hierarchy of the 'child'
   type"
  [parent child]
  (loop [child child]
    (cond
      (nil? child) false
      (= child parent) true
      :default (recur (:parent child)))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Defining some different type-checking predicates for our operators
;
; These predicates will return functions which take a variable number of arguments and check them
; against various constraints.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defn descendants-of?
  "Returns a predicate that enforces the following constraints:

    1: There are exactly 'expected-arg-count' arguments supplied.
    2: Each of those arguments is a descendant of the supplied 'base-type'."
  [base-type expected-arg-count]
  (fn [args]
    (printf "Checking against args: %s\n" args)
    (let [arg-count (count args)]
      (cond
        ;; We want to verify that the right number of arguments were supplied
        (not= arg-count expected-arg-count)
        (err/raise-error! "Operator expects %d arguments, found %d\n" expected-arg-count arg-count)

        ;; We also need to verify that every argument in the list is a descendant of the supplied
        ;; base type
        (not-every? true? (map (partial parent? base-type) args))
        (err/raise-error! "Mismatch: Expected all arguments to be children of %s, found: [%s]\n"
                          (:name base-type) (join ", " (map :name args)))

        :default true))))

(defn identical-descendants-of?
  "Returns a predicate that enforces the following constraints:

    1: There are exactly 'expected-arg-count' arguments supplied.
    2: Each of those arguments is a descendant of the supplied 'base-type'
    3: All of those arguments have the same type."
  [base-type expected-arg-count]
  (let [descendants-of?* (descendants-of? base-type expected-arg-count)]
    (fn [args]
      (cond
        ;; We want to apply the same descendants check as the above predicate
        (not (descendants-of?* args)) false

        ;; We also want to check that all supplied arguments are of the SAME type
        (not (apply = args))
        (err/raise-error! "Found varying children of %s, expected same types: [%s]\n"
                          (:name base-type) (join ", " (map :name args)))

        :default true))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Defining our supported operators
;
; Below we define the supported operators in the language, as well as the contracts define what
; types of arguments are expected.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defrecord Operator [name symbol result-type predicate])

;;; Binary mathematical operators
(defonce PLUS     (Operator. "Plus"     "+" NUMBER (descendants-of? NUMBER 2)))
(defonce MULTIPLY (Operator. "Multiply" "*" NUMBER (descendants-of? NUMBER 2)))
(defonce SUBTRACT (Operator. "Subtract" "-" NUMBER (descendants-of? NUMBER 2)))
(defonce DIVIDE   (Operator. "Divide"   "/" NUMBER (descendants-of? NUMBER 2)))

;;; Unary mathematical operators
(defonce NEGATE   (Operator. "Negate"   "-" NUMBER (descendants-of? NUMBER 1)))

;;; Binary Comparison Operators
(defonce LESS-THAN              (Operator. "Less Than"              "<"   BOOLEAN (descendants-of? NUMBER 2)))
(defonce LESS-THAN-OR-EQUAL     (Operator. "Less Than Or Equal"     "<="  BOOLEAN (descendants-of? NUMBER 2)))
(defonce GREATER-THAN           (Operator. "Greater Than"           ">"   BOOLEAN (descendants-of? NUMBER 2)))
(defonce GREATER-THAN-OR-EQUAL  (Operator. "Greater Than Or Equal"  ">="  BOOLEAN (descendants-of? NUMBER 2)))
(defonce IS-EQUAL-TO            (Operator. "Is Equal To"            "=="  BOOLEAN (identical-descendants-of? BOTTOM 2)))
(defonce NOT-EQUAL-TO           (Operator. "Not Equal To"           "!="  BOOLEAN (identical-descendants-of? BOTTOM 2)))

(defonce keyword-operator-map
  {:plus                  PLUS
   :multiply              MULTIPLY
   :minus                 SUBTRACT
   :divide                DIVIDE
   :negate                NEGATE
   :lt                    LESS-THAN
   :lte                   LESS-THAN-OR-EQUAL
   :gt                    GREATER-THAN
   :gte                   GREATER-THAN-OR-EQUAL
   :eq                    IS-EQUAL-TO
   :neq                   NOT-EQUAL-TO})

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; User-facing functions for type-checking
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defn check-operator
  "Checks that the argument types supplied are valid for the supplied operator"
  [operator argument-types]
  (let [type-check-predicate (:predicate operator)]
    (type-check-predicate argument-types)))

(defn keyword->operator
  "Retrieves the proper Operator based on the keyword returned by the parser."
  [kw]
  (if-let [operator (get keyword-operator-map kw)]
    operator
    (err/raise-error! "Unrecognized operator: %s\n" kw)))

(defn keyword->type
  "Retrieves the proper Type based on the keyword returned by the parser"
  [kw]
  (let [kw (-> kw name upper-case keyword)]
    (if-let [type (get keyword-type-map kw)]
      type
      (err/raise-error! "Unrecognized type: %s\n" kw))))
