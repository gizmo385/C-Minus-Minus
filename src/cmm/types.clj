(ns cmm.types
  "Handles the various type checking responsibilities for the parser."
  (:require [cmm.errors :as err]
            [cmm.debug :refer [debug-msg]]
            [clojure.pprint :as pp]
            [clojure.string :refer [join lower-case upper-case]]))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Defining the structure for our type system in the compiler
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defrecord Type [name parent])

(defmethod print-method Type
  [v ^java.io.Writer w]
  (.write w (format "<Type: %s>" (:name v))))

(defmethod pp/simple-dispatch Type [t] (pr t))

;;; Bottom is the base type in the type system and is the eventual parent type for ALL types (except Void)
(defonce BOTTOM (Type. "Bottom" nil))
(defonce VOID (Type. "Void" nil))

;;; Building our numeric tower
(defonce NUMBER   (Type. "Number" BOTTOM))
(defonce DOUBLE   (Type. "Double" NUMBER))
(defonce FLOAT    (Type. "Float" DOUBLE))
(defonce INTEGER  (Type. "Integer" FLOAT))

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

(defn widest-type
  "Given a series of types, returns the type that is the highest up the type hierarchy. This is used
   when determining the type of an expression where the types of the operands supplied to an
   operator conflict (ex: Integer + Float)."
  [types]
  (loop [widest         (first types)
         types          (rest types)]
    (if-let [current-type  (first types)]
      (if (parent? current-type widest)
        (recur current-type (rest types))
        (recur widest (rest types)))
      widest)))

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

(defmethod print-method Operator
  [v ^java.io.Writer w]
  (.write w (format "<Operator: %s (%s)>" (:name v) (:symbol v))))

(defmethod pp/simple-dispatch Operator [o] (pr o))

;;; Binary mathematical operators
(defonce PLUS     (Operator. "Plus"     "+" NUMBER (descendants-of? NUMBER 2)))
(defonce MULTIPLY (Operator. "Multiply" "*" NUMBER (descendants-of? NUMBER 2)))
(defonce SUBTRACT (Operator. "Subtract" "-" NUMBER (descendants-of? NUMBER 2)))
(defonce DIVIDE   (Operator. "Divide"   "/" NUMBER (descendants-of? NUMBER 2)))

;;; Unary Mathematical Operators
(defonce NEGATE   (Operator. "Negate"   "-" NUMBER (descendants-of? NUMBER 1)))

;;; Unary Boolean Operators
(defonce NOT      (Operator. "Not"      "!" BOOLEAN (descendants-of? BOOLEAN 1)))

;;; Binary Comparison Operators
(defonce LESS-THAN              (Operator. "Less Than"              "<"   BOOLEAN (descendants-of? NUMBER 2)))
(defonce LESS-THAN-OR-EQUAL     (Operator. "Less Than Or Equal"     "<="  BOOLEAN (descendants-of? NUMBER 2)))
(defonce GREATER-THAN           (Operator. "Greater Than"           ">"   BOOLEAN (descendants-of? NUMBER 2)))
(defonce GREATER-THAN-OR-EQUAL  (Operator. "Greater Than Or Equal"  ">="  BOOLEAN (descendants-of? NUMBER 2)))
(defonce IS-EQUAL-TO            (Operator. "Is Equal To"            "=="  BOOLEAN (identical-descendants-of? BOTTOM 2)))
(defonce NOT-EQUAL-TO           (Operator. "Not Equal To"           "!="  BOOLEAN (identical-descendants-of? BOTTOM 2)))

;;; Binary Boolean Operators
(defonce AND                    (Operator. "And"                    "&&" BOOLEAN (descendants-of? BOOLEAN 2)))
(defonce OR                     (Operator. "Or"                     "||" BOOLEAN (descendants-of? BOOLEAN 2)))

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

(defn expression-result-type
  "Given an operator and a series of operands, determines the return-type that the expression is
   going to have."
  [operator & operands]
  (let [widest-operand-type   (widest-type (map :type operands))
        operator-result-type  (:result-type operator)]
    (if (parent? operator-result-type widest-operand-type)
      widest-operand-type
      (err/raise-error!
        "Error while determining operator result type of %s expression: %s is not a child of %s\n"
        (:name operator) (:name widest-operand-type) (:name operator-result-type)))))

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

(defn argument-types
  "Given the parameter declarations from a function definition, determine what the argument types
   should be"
  [[_ & parameters]]
  (for [[_ arg-type _] parameters] (keyword->type arg-type)))
