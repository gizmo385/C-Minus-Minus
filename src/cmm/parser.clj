(ns cmm.parser
  (:require [instaparse.core :as insta]))

(def c-parser
  (insta/parser (clojure.java.io/resource "grammar.bnf") :auto-whitespace :standard))

(comment
  (insta/parse c-parser "-5-4")
  (insta/parse c-parser "5.4")
  (insta/parse c-parser "5.4 <= -3.7 + 4")
  (insta/parse c-parser "testing[get_number(5, 3)]")
  (insta/parse c-parser "testing()")
  (insta/parse c-parser "testing('c')")
  (insta/parse c-parser "\"this is a string\\n\"")
  (insta/parse c-parser "testing(1)")
  (insta/parse c-parser "testing(1, 2)")
  (insta/parse c-parser "while(1 == 1) return 5.4;")
  (insta/parse c-parser "while(1 == 1) if((1 - 4) > 5) return; else return 5;")
  (insta/parse c-parser "for(x = 5; x < 5; x = x + 1) return 5;")
  (insta/parse c-parser "int x; char a, b;")
  (insta/parse c-parser "void printf(char x[]) { return; } int number_5(void) { return 5; }")
  (insta/parse c-parser "void x(void);")
  )
