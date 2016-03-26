(ns cmm.lexer-test
  (:require [clojure.test :refer :all]
            [cmm.lexer :refer [tokenize tokens]]))

(deftest test-character-literals
  (testing "Tokenizing alphanumeric characters"
    (doseq [ch (map char (concat (range (int \A) (int \Z))
                                 (range (int \a) (int \z))
                                 (range (int \0) (int \9))))]
      (let [parsed (as-> ch token
                     (str token)
                     (format "'%s'" token)
                     (tokenize token "test")
                     (:accepted token)
                     (first token))]
        (is (= :char (:token parsed))
            (format "Token from parsing '%s' should be :char, not %s" ch (:token parsed)))
        (is (= ch (:lexeme parsed))
            (format "Lexeme from parsing '%s' should be %s, not %s" ch ch (:lexeme parsed))))))

  (testing "Parsing character escape sequences"
    (doseq [[input correct] [["'\n'" \newline] ["'\t'" \tab]]
            :let [printable-in (char-escape-string input)]]
      (let [parse-out (tokenize input "test")
            first-token (first (:accepted parse-out))]
        (println parse-out)
        (is (= :char (:token first-token))
            (format "Token from parsing '%s' should be :char, not %s"
                    printable-in
                    (:token first-token)))
        (is (= correct (:lexeme first-token))
            (format "Lexeme parsing %s should be %s, not %s"
                    printable-in
                    correct
                    (:lexeme first-token)))))))

(deftest test-integer-literals
  (testing "Integer literals"
    (dotimes [i 1000]
      (let [parsed (first (:accepted (tokenize (str i) "test")))]
        (is (= i (:lexeme parsed))
            (format "The lexeme from parsing %d should be %d, not %s" i i (:lexeme parsed)))
        (is (= :integer (:token parsed))
            (format "The token from parsing %d should be :integer, not %s" i (:token parsed)))))))

(deftest test-string-literals
  (testing "Plain string literals"
    (let [test-string "Hello world!"
          parsed (first (:accepted (tokenize (format "\"%s\"" test-string) "test")))]
      (is (= :string (:token parsed))
          (format "Token for \"%s\" should be :string not %s" test-string (:token parsed)))
      (is (= test-string (:lexeme parsed))
          (format "Parsed string \"%s\" should be equal to \"%s.\""
                  (:lexeme parsed)
                  test-string))))

  ;; Ensure that escape sequences in strings work
  ;; TODO Escaped quotes in strings
  (testing "String literals with escape sequences"
    (doseq [test-string ["Hello world!\n" "\tHello"]]
      (let [parsed (first (:accepted (tokenize (format "\"%s\"" test-string) "test")))]
        (is (= :string (:token parsed))
            (format "Token for \"%s\" should be :string not %s" test-string (:token parsed)))
        (is (= test-string (:lexeme parsed))
            (format "Parsed string \"%s\" should be equal to \"%s.\""
                    (:lexeme parsed)
                    test-string))))))

(deftest test-ids-keywords
  (testing "Keywords"
    (doseq [[kw token] tokens]
      (let [parsed (first (:accepted (tokenize kw "test")))]
        (is (= token (:token parsed))
            (format "Token for %s should be %s, is %s" kw token (:token parsed)))
        (is (= kw (:lexeme parsed))
            (format "Lexeme for %s should be %s, is %s" kw kw (:lexeme parsed))))))
  (testing "Identifiers"))
