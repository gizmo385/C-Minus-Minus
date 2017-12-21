(ns cmm.symbol-table-test
  (:require [clojure.test :refer :all]
            [cmm.symbol-table :refer :all]
            [cmm.errors :as err]
            [instaparse.core :as insta]))

(deftest variable-test
  (testing "Adding variables to empty symbol table works"
    (err/reset-error!)
    (is (some? (variable-defined?  (add-variable (new-symbol-table) :int "testing") "testing")))
    (is (not (err/error?)) "The error flag has been set"))

  (testing "Adding variables to a non-empty symbol table works"
    (err/reset-error!)
    (is (some? (add-variable (add-variable (new-symbol-table) :int "testing1") :int "testing2")))
    (is (not (err/error?)) "The error flag has been set"))

  (testing "Redeclaring variable in nested scope works"
    (err/reset-error!)
    (let [parent-scope (add-variable (new-symbol-table) :int "testing")
          test-scope (new-symbol-table parent-scope)
          redeclaration-result (add-variable test-scope :int "testing")]
      (is (some? redeclaration-result) "Redeclaration result is nil")
      (is (variable-defined? parent-scope "testing") "'testing' NOT defined in parent scope")
      (is (variable-defined? test-scope "testing") "'testing' NOT defined in test scope")
      (is (not (err/error?)) "The error flag has been set")))

  (testing "Cannot redeclare variable already declared in same scope"
    (err/reset-error!)
    (let [table (add-variable (new-symbol-table) :int "testing1")
          redeclaration-result(add-variable table :int "testing1")]
      (is (nil? redeclaration-result) "Redeclaration result is 'nil'.")
      (is (true? (err/error?)) "The error flag has NOT been set to true."))))

(deftest functions-test
  (testing "Declaring functions in an empty symbol-table"
    (err/reset-error!)
    (is (some? (add-function (new-symbol-table) :int "testing" []))
        "Couldn't declare 0-arg function")
    (is (some? (add-function (new-symbol-table) :int "testing" [:int]))
        "Couldn't declare 1-arg function")
    (is (some? (add-function (new-symbol-table) :int "testing" [:int :char]))
        "Couldn't declare 2+ arg function")
    (is (not (err/error?)) "The error flag is set."))

  (testing "Function with different name"
    (err/reset-error!)
    (let [table (add-function (new-symbol-table) :int "testing1" [])
          declaration-result (add-function table :char "testing2" [])]
      (is (some? declaration-result) "Declaration result is nil")
      (is (not (err/error?)) "The error flag is set.")))

  (testing "Overloading function type"
    (err/reset-error!)
    (let [table (add-function (new-symbol-table) :int "testing" [])
          declaration-result (add-function table :char "testing" [])]
      (is (nil? declaration-result) "Declaration result is not nil")
      (is (err/error?) "The error flag is not set.")))

  (testing "Overloading function argument types"
    (err/reset-error!)
    (let [table1 (add-function (new-symbol-table) :int "testing1" [])
          declaration-result1 (add-function table1 :int "testing1" [:int])
          declaration-result2 (add-function table1 :int "testing1" [:int :int])

          table2 (add-function (new-symbol-table) :int "testing2" [:int :char])
          declaration-result3 (add-function table2 :int "testing2" [:char :int])
          declaration-result4 (add-function table2 :int "testing2" [:int :int])]
      (is (some? declaration-result1) "Couldn't overload 0-arg function with 1 arg.")
      (is (some? declaration-result2) "Couldn't overload 0-arg function with 2 args.")
      (is (some? declaration-result3) "Couldn't change argument order of 2-arg function.")
      (is (some? declaration-result4) "Couldn't change argument types of 2-arg function.")
      (is (not (err/error?)) "The error flag is set.")))

  (testing "Redeclaring an already declared function"
    (err/reset-error!)
    (let [table (add-function (new-symbol-table) :int "testing" [:int])
          redeclaration-result (add-function table :int "testing" [:int])]
      (is (nil? redeclaration-result) "Was able to redeclare a function.")
      (is (err/error?) "Error flag is not set."))))
