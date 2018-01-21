(ns cmm.core
  (:require [clojure.string :refer [join]]
            [clojure.tools.cli :refer [parse-opts]]
            [clojure.pprint :refer [pprint]]
            [clansi :refer [style]]
            [cmm.parser :as parser]
            [cmm.errors :as err]
            [cmm.debug :refer [*debugging-enabled*]])
  (:gen-class))

;;; Command line arguments
(def ^:private cli-options
  [["-d" "--debug" "Debug mode"]
   ["-h" "--help" "Display usage information"]])

(defn- usage [options-summary]
  (->>  ["This is a small C compiler implemented in Clojure."
         ""
         "Usage: lein run -- [options] filenames ..."

         ""
         "Options:"
         options-summary
         ""
         "Please refer to the manual page for more information."]
       (join \newline)))

;;; Convinience functions for printing/exiting
(defn- printf-err
  "Calls printf on the arguments with *out* bound to stderr."
  [& msg]
  (binding [*out* *err*]
    (apply printf msg)
    (flush)))

(defn- exit
  "Prints a message to stdout and then exits with the supplied status code."
  [status message]
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
    (parser/parse)
    (pprint))

  ;; Determine if compilation failed
  (if (err/error?)
    (printf-err "Could not successfully compile %s\n" (style filename :underline))
    (printf "Successfully compiled %s\n" (style filename :underline))))

;;; Main entry point for the compiler.
(defn -main [& args]
  (let [{:keys [options arguments summary errors]} (parse-opts args cli-options)]
    ;; Check for help options
    (cond
      (:help options) (exit 0 (usage summary))
      errors (exit 1 (usage (join errors))))

    ;; Begin parsing files
    (binding [*debugging-enabled* (:debug options)]
      (doseq [filename arguments]
        (handle-file filename))
      (if (err/error?)
        (exit 1 (style "Compilation failed." :red))
        (exit 0 (style "Compilation succeeded." :green))))))
