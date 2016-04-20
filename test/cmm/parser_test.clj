(ns cmm.parser-test
  (:require [clojure.test :refer :all]
            [cmm.parser :refer :all]
            [cmm.errors :as err]
            [instaparse.core :as insta]))

(deftest whitespace-parser-test
  (testing "Whitespace and comments are parsed without failing"
    (are [input] (->> input
                      (insta/parse whitespace-or-comments)
                      (insta/failure?)
                      (false?))
         ;; Some whitespace
         "    "
         "\n\n\t   \t\n"
         ;; Comments
         "/* */"
         "/* jfdklafjkdlajlfd\n\n\n */"
         "/********************************/"))
  (testing "Fails on invalid comment structures"
    (are [input] (->> input
                      (insta/parse whitespace-or-comments)
                      (insta/failure?))
         "/* "
         "/ jfdklafjkdlajlfd\n\n\n */"
         "/* *")))

(deftest c-parser-test
  (testing "Correctly parses valid C expressions"
    (are [input] (->> input
                      (insta/parse c-parser)
                      (insta/failure?)
                      (false?))
         "int x(void) { g = 1 + 2 + 4 * 5 / -(4 * 7); }"
         "int x(void) { g = (1 == 1) && (2 != 3) || ((4 > 3) && !(3 <= 7)); }"
         "int x(void) { g = ------------2; }"
         "int x(void) { g = x + (7 * y); }"))
  (testing "Correctly parses various function declarations"
    (are [input] (->> input
                      (insta/parse c-parser)
                      (insta/failure?)
                      (false?))
         "void x(void);"
         "int x_(void);"
         "char x_x12144231531(void);"
         "extern char x_x12144231531(void);"
         "float x(void);"
         "void x(int a);"
         "void x(char a);"
         "void x(float a);"
         "void x(int a, char b);"
         "void x(int a, char b, float h184814949313_);"))
  (testing "Correctly various statement types")
  )
