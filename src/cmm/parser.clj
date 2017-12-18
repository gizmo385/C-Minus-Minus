(ns cmm.parser
  "This namespace contains functions related to parsing C files. This process lexical analysis,
  type checking, and error handling."
  (:require [instaparse.core :as insta]
            [clojure.edn :as edn]
            [clojure.string :refer [join]]
            [clojure.pprint :refer [pprint]]
            [cmm.errors :as err]
            [cmm.types :as types]
            [cmm.debug :as dbg :refer [debug-msg]]))

;;; Loading the parser from the grammar file
(def whitespace-or-comments
  "This parser handles the whitespace and comment parsing. It is used as the whitespace parser in
   the primary C parser."
  (insta/parser (clojure.java.io/resource "whitespace.bnf") :auto-whitespace :standard))

(def c-parser
  "This is the primary C parser. It uses a custom whitespace parser which handles comments and
   whitespace characters. The results of this parser should be sent to the build-ast function."
  (insta/parser (clojure.java.io/resource "grammar.bnf") :auto-whitespace whitespace-or-comments))

;;; Building the ast
(defmulti build-ast
  "The build-ast function represents the primary parsing function for CMM. This uses the output
   from instaparse to generate a more easily understandable, parseable, and easy-to-use tree
   structure.

   The first argument represents the symbol table of the calling scope. The second argument
   represents a subsection of the parse result."
  (fn [program] (first program)))

;;; User facing functions
(defn parse [source]
  (let [parse-result (->> source
                          (insta/parse c-parser)
                          (insta/add-line-and-column-info-to-metadata source))]
    (pprint parse-result)))
