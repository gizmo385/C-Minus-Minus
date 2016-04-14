(ns cmm.core
  (:require [clojure.string :refer [join]]
            [clojure.tools.cli :refer [parse-opts]]
            [clansi :refer [style]]
            [cmm.parser :refer [parse]]
            [cmm.errors :as err]
            [cmm.debug :refer [*debugging-enabled*]])
  (:gen-class))

(def ^:private cli-options
  [["-d" "--debug" "Debug mode"]
   ["-h" "--help" "Display usage information"]])

(defn- usage [options-summary]
  (->>  ["This is a small C compiler implemented in Clojure."
         ""
         "Usage: program-name [options] filenames ..."
         ""
         "Options:"
         options-summary
         ""
         "Please refer to the manual page for more information."]
       (join \newline)))

(defn- printf-err
  "Calls printf on the arguments with *out* bound to stderr."
  [& msg]
  (binding [*out* *err*]
    (apply printf msg)
    (flush)))

(defn- exit [status message]
  (println message)
  (System/exit status))

(defn- handle-file
  "Convinience function handle all the involved in compiling a single file."
  [filename]
  (printf "Compiling %s...\n" (style filename :underline))
  (flush)
  ;; The actual parsing of the file
  (err/without-error->> filename
    (slurp)
    (parse))
  ;; Determine if compilation failed
  (if (err/error?)
    (printf-err "Could not successfully compile %s\n" (style filename :underline))
    (printf "Successfully compiled %s\n" (style filename :underline))))

(defn -main [& args]
  (let [{:keys [options arguments summary errors]} (parse-opts args cli-options)]
    ;; Check for help options
    (cond
      (:help options) (exit 0 (usage summary))
      errors (exit 1 (usage (join errors))))

    ;; Begin parsing files
    (binding [*debugging-enabled* (:debug options)]
      (doseq [filename arguments]
        (handle-file filename)))))
