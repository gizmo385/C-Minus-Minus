(ns cmm.parser
  (:require [instaparse.core :as insta]
            [clojure.edn :as edn]))

(def c-parser
  (insta/parser (clojure.java.io/resource "grammar.bnf") :auto-whitespace :standard))

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

(defmethod build-ast :EXPRESSION [expression] expression)

(defmethod build-ast :default [structure]
  (printf "Can't parse a %s\n" (first structure)))

;; Transformation map
(def transform-map
  {:INTEGER edn/read-string
   :FLOAT   edn/read-string})

;; User facing functions
(defn parse [source]
  (as-> source <>
    (insta/parse c-parser <>)
    (insta/transform transform-map <>)
    (do (println <>) <>)
    (build-ast <>)))

(def example
  "
  void testing(void) {
   int x, y;
   printf(5);
   printf(5);
   }
   void x(int g) { return 6; }
   ")

(comment
  (parse "int x; char a, b[5];")
  (parse "void printf(char x[]) { return; } int number_5(void) { return 5; }")
  (parse "void x(void);")
  (parse "extern void x(void);")
  (insta/parse c-parser example)
  (parse example)
  (build-ast (insta/parse c-parser "void x(int g, char x, float y) { }"))
  )
