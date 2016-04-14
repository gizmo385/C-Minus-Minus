(ns cmm.debug
  "This namespace contains convinience macros and functions for handling debugging. Print messages
   meant for debugging can be replaced with calls to debug-msg. These calls will only execute when
   the dynamic debugging-enabled var has been set to true. This can be easily accomplished with the
   with-debugging macro."
  (:require [clansi :refer [style]]))

(def ^:dynamic *debugging-enabled*
  "Determines whether or not debugging messages should print."
  false)

(defmacro with-debugging
  "Rebinds the debugging-enabled var to true, allowing debugging messages within this block to
   print."
  [& body]
  `(binding [*debugging-enabled* true]
    ~@body))

(defn debug-msg
  "Calls printf with the arguments if debugging has been enabled."
  [& args]
  (if *debugging-enabled*
    (do
      (print (style "DEBUG:" :green) (apply format args))
      (flush))))
