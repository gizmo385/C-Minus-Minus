(ns cmm.core
  (:require [cmm.parser :refer [parse]]
            [cmm.errors :as err])
  (:gen-class))

(defn -main [& args]
  (doseq [arg args]
    (printf "Compiling %s...\n" arg)
    (let [parse-result (parse (slurp arg))]
      (if (err/error?)
        (binding [*out* *err*]
          (printf "Was unable to successfully compile %s\n" arg)
          (System/exit 1))
        (printf "Successfully parsed %s: %s\n" arg parse-result)))))
