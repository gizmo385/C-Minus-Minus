(ns cmm.core
  (:require [cmm.parser :refer [parse]]
            [cmm.errors :as err])
  (:gen-class))

(defn -main [& args]
  (doseq [filename args]
    (printf "Compiling %s...\n" filename)
    (flush)
    (err/without-error->> filename
      (slurp)
      (parse))
    (if (err/error?)
      (binding [*out* *err*] (printf "Could not successfully compile %s\n" filename))
      (printf "Successfully compiled %s\n" filename))))
