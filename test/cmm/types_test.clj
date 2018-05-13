(ns cmm.types-test
  (:require [clojure.test :refer :all]
            [cmm.types :refer :all]
            [cmm.errors :as err]))

(deftest parent-test
  (testing "Validate the type hierarchy of the numeric tower."
    ;; Checking subtypes of the Number type
    (are [p c] (parent? p c)
         NUMBER DOUBLE
         NUMBER FLOAT
         NUMBER INTEGER
         NUMBER CHARACTER
         DOUBLE FLOAT
         DOUBLE INTEGER
         DOUBLE CHARACTER
         FLOAT INTEGER
         FLOAT CHARACTER
         INTEGER CHARACTER)

    ;; Ensure that a backwards comparison returns an error
    (is (not (parent? CHARACTER NUMBER)) "Number should be the parent type of Character."))

  (testing "Validating that the root types are sole parents and not releated."
    ;; All non-void types are children of BOTTOM
    (are [t] (parent? BOTTOM t)
         BOTTOM
         NUMBER
         DOUBLE
         FLOAT
         INTEGER
         CHARACTER
         BOOLEAN)

    ;; No types are children of VOID
    (are [t] (not (parent? VOID t))
         BOTTOM
         NUMBER
         DOUBLE
         FLOAT
         INTEGER
         CHARACTER
         BOOLEAN)

    ;; VOID is not a child of BOTTOM, and vice-versa
    (is (not (parent? BOTTOM VOID)))
    (is (not (parent? VOID BOTTOM)))))

(deftest operator-test
  (testing "Testing that the binary comparison operators correctly return BOOLEAN."
    (are [op] (= (expression-result-type op [BOTTOM BOTTOM]) BOOLEAN)
         LESS-THAN
         LESS-THAN-OR-EQUAL
         GREATER-THAN
         GREATER-THAN-OR-EQUAL
         IS-EQUAL-TO
         NOT-EQUAL-TO))

  (testing "Testing that the binary boolean operators correctly return BOOLEAN."
    (are [op] (= (expression-result-type op [BOOLEAN BOOLEAN] BOOLEAN))
         AND
         OR))

  (testing "Checking that binary mathematical operators correctly return NUMBER."
    (are [op] (= (expression-result-type op [NUMBER NUMBER]) NUMBER)
         PLUS
         SUBTRACT
         MULTIPLY
         DIVIDE))

  (testing "Type checking correctly formed binary mathematical expressions."
    (are [operand1 op operand2] (check-operator op [operand1 operand2])
         NUMBER PLUS NUMBER
         NUMBER SUBTRACT NUMBER
         NUMBER MULTIPLY NUMBER))

  (testing "Type checking correctly formed binary boolean expressions."
    (are [operand1 op operand2] (check-operator op [operand1 operand2])
         BOOLEAN AND BOOLEAN
         BOOLEAN OR BOOLEAN)))
