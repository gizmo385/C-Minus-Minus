(ns user
  "Contains functions that will be automatically loaded into the REPL"
  (:use [cmm core parser]
        [clojure.pprint]
        [clojure.repl]
        [clojure.tools.namespace.repl]
        [clojure.stacktrace :only  [print-stack-trace]]))

(defn trace
  "Prints the first num-lines (or 10) lines of the stack trace for the root cause of the last stack
   trace"
  ([]  (trace 10))
  ([num-lines]
   (print-stack-trace  (root-cause *e) num-lines)))
