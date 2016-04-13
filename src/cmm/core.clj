(ns cmm.core
  (:require [cmm.parser :refer [parse]])
  (:gen-class))

(defn -main [& args]
  (doseq [arg args]
    (printf "Compiling %s...\n" arg)
    (println (parse (slurp arg)))))
