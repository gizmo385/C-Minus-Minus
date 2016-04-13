(ns cmm.errors
  "This namespace encapsulates access to the global error flag in the parser system. If an error
   occurs during parsing, then the raise-error! function should be called, along with appropriate
   printf arguments.")

(def error-occured?
  "This represents the singular piece of global state within the parser. For the sake of
   simplifying return schematics for the parser functions, this piece of state was extracted and
   moved here. This boolean represents whether or not an unrecoverable error was encountered during
   the parsing process.

   It can be modified solely through raise-error!, which sets it to true, and
   reset-error! which sets it to false. The reset-error! MUST be called inbetween independent file
   parses to maintain the contract assumed in this sytem."
  (atom false))

(defn error?
  "Wrapper function around access to the global error flag. This will return true if raise-error!
   has been called since the last call to reset-error!, and will return false otherwise."
  []
  (deref error-occured?))

(defn raise-error!
  "Sets the global error flag to true and then sends the supplied arguments to printf,
   destined for stderr."
  [& args]
  (reset! error-occured? true)
  (binding [*out* *err*] (print "ERROR:" (apply format args))
    (flush)))

(defn reset-error!
  "Resets the global error flag to false. This function should be called in-between independent
   parses. This will return true if resetting the flag was successful, false otherwise."
  []
  (not (reset! error-occured? false)))
