(ns cmm.symbol-table
  "This namespace contains an implementation of a symbol table and relevant functions."
  (:require [instaparse.core :as insta]
            [clojure.edn :as edn]
            [clojure.string :refer [join]]
            [clojure.pprint :refer [pprint]]
            [cmm.errors :as err]
            [cmm.types :as types]
            [cmm.debug :as dbg :refer [debug-msg]]))

;; For symbol-keeping purposes, a variable is just a name and a type
(defrecord Variable [type name array? array-size])

;; For symbol-keeping purposes, a funtion is similar to a variable, but it also has argument types
(defrecord Function [return-type name argument-types])

;; A symbol table is just a collection of variables and functions, potentially nested within
;; a another symbol table (nested scope)
(defrecord SymbolTable [parent variables functions])

(defn new-symbol-table
  "Creates a new symbol table record. A parent symbol table can be optionally supplied to nest
   scopes within one-another."
  ([]
   (new-symbol-table nil))
  ([parent]
   (SymbolTable. parent {} {})))

(defn variable-defined?
  "Determines whether or not a variable with the same name has been defined in the supplied
   symbol table. If it has been defined, the definition will be returned. Otherwise nil is returned.

   You can optionally supply a 5th argument, the max depth in the symbol table tree that will be
   traversed searching for a definition. By default, this is nil meaning that there is not limit to
   how deep it will traverse."
  ([symbol-table name]
   (variable-defined? symbol-table name nil))
  ([symbol-table name max-depth]
   (loop [symbol-table symbol-table
         max-depth max-depth]
     (let [variable-with-name (get-in symbol-table [:variables name])]
       (cond
         ;; If the symbol table is nil, then we've bottomed out in the scope tree and nothing is
         ;; defined
         (nil? symbol-table) nil

         ;; If the max-depth is zero, we're not traversing any deeper in the scope tree
         (and (some? max-depth) (zero? max-depth))  nil

         ;; If the name is present in the table, we'll return true
         (some? variable-with-name) variable-with-name

         ;; In our default case, we'll go check the parent scope
         :default (recur (:parent symbol-table)
                         (if (nil? max-depth) max-depth (dec max-depth))))))))

(defn function-defined?
  "Determines whether or not a function with the same return type, name, and argument types has been
   defined in the supplied symbol table. If it has been defined, the definition will be returned.
   Otherwise nil is returned.

   You can optionally supply a 5th argument, the max depth in the symbol table tree that will be
   traversed searching for a definition. By default, this is nil meaning that there is not limit to
   how deep it will traverse."
  ([symbol-table name argument-types]
   ;; Our default value for max-depth is "nil", meaning we'll traverse all the way to the end of
   ;; the symbol-table
   (function-defined? symbol-table name argument-types nil))

  ([symbol-table name argument-types max-depth]
   (loop [symbol-table symbol-table
          max-depth max-depth]
     (let [functions-with-name (get-in symbol-table [:functions name])
           matching-function (->> functions-with-name
                                  (filter (fn [f] (= argument-types (:argument-types f))))
                                  (first))]
       (cond
          (some? matching-function) matching-function

         ;; If the symbol table is nil, then we've bottomed out in the scope tree and nothing is
         ;; defined
         (nil? symbol-table) nil

         ;; If the max-depth is zero, we're not traversing any deeper in the scope tree
         (and (some? max-depth) (zero? max-depth))  nil

         ;; In our default case, we'll go check the parent scope
         :default (recur (:parent symbol-table)
                         (if (nil? max-depth) max-depth (dec max-depth))))))))


(defn add-function
  "Adds a new function to the symbol table. If a function of the same name, return type, and with
   the same argument types has already been a defined, an error is raised."
  [symbol-table return-type name argument-types]
  (if (function-defined? symbol-table name argument-types)
    (err/raise-error! "Attempting to redeclare function %s with return type %s and arguments %s\n"
                      name
                      return-type
                      argument-types)
    (let [new-function (Function. return-type name argument-types)]
      (update-in symbol-table [:functions name] conj new-function))))

(defn add-variable
  "Adds a new variable to the symbol table. If a variable of the same name has been defined in the
   current scope, then an error is raised. If the variable was defined in a parent scope, no error
   is raised.

   Optionally, a 4th array size parameter can be supplied. This will mark the variable as an array
   and store its size."
  ([symbol-table type name]
   (add-variable symbol-table type name nil))

  ([symbol-table type name array-size]
   (if (variable-defined? symbol-table name 1)
     (err/raise-error! "Redeclaring variable %s of type %s\n" name type)
     (let [new-variable (Variable. type name (some? array-size) array-size)]
       (assoc-in symbol-table [:variables name] new-variable)))))
