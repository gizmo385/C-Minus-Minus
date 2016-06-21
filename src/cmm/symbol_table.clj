(ns cmm.symbol-table
  (:require [cmm.errors :as err]
            [cmm.debug :refer [debug-msg]]
            [cmm.types :refer [type-size]]
            [clojure.string :as s]))

;;; Symbol table structure
(defrecord SymbolTable [parent entries offset])
(defrecord Variable [symbol-name symbol-type array offset])
(defrecord Function [function-name function-type params defined?]) ;; TODO: Add parameter types

;;; Constructors
(defn new-symbol-table
  ([] (SymbolTable. nil [] 0))
  ([parent] (SymbolTable. parent [] 0))
  ([parent entries] (SymbolTable. parent entries 0)))


;;; Finding entries in the symbol table
(defn find-variable-entry
  "Searches through nested symbol tables (up to an optional max depth) to find an occurrence of a
   particular symbol name."
  ;; Unlimited search depth
  ([symbol-table symbol-name]
   (find-variable-entry symbol-table symbol-name -1))

  ;; Limited search depth
  ([{:keys [entries parent]} symbol-name search-depth]
   (if (or (empty? entries) (zero? search-depth))
     nil
     (if-let [results (->> entries
                           (filter #(= (:symbol-name %) symbol-name))
                           (filter #(= (type %) Variable))
                           (first))]
       results
       (find-variable-entry parent symbol-name (dec search-depth))))))

(defn find-function-entry [symbol-table function-name]
  (loop [symbol-table symbol-table]
    (if (:parent symbol-table)
      (recur (:parent symbol-table))
      (->> (:entries symbol-table)
           (filter (partial instance? Function))
           (filter #(= (:function-name %) function-name))
           (first)))))

;;; Creating entries in the symbol table
(defn- add-variable-entry
  "Makes a new symbol table entry for a variable and places it into the table"
  [symbol-table symbol-name symbol-type array]
  (if (find-variable-entry symbol-table symbol-name 1)
    (err/raise-error! "Cannot add %s to the symbol table, already declared" symbol-name)
    (let [entry (Variable. symbol-name symbol-type array (:offset symbol-table))]
      (assoc symbol-table
             :entries (conj (:entries symbol-table) entry)
             :offset (+ (:offset symbol-table) (type-size symbol-type))))))

(defn add-parameters
  "Traverses the parameter structures and add thoses variables to the symbol table. Returns the
   symbol table"
  [symbol-table params]
  (loop [symbol-table symbol-table
         params params]
    (if-let [param (first params)]
      (recur (add-variable-entry symbol-table (:name param) (:type param) (:array param))
             (next params))
      symbol-table)))

(defn- add-declaration-vars
  "Helper function when declaring a declaration. This traverses the listed vars in a declaration
   and properly declares them. Returns the symbol table after insertions"
  [symbol-table symbol-types vars]
  (loop [vars vars
         symbol-table symbol-table]
    (if-let [v (first vars)]
      (recur (next vars)
             (add-variable-entry symbol-table (:name v) symbol-types (not= nil (:array-size v))))
      symbol-table)))

(defn add-declarations
  "Given a series of declarations, properly inserts those declarations into the symbol table."
  [symbol-table declarations]
  (loop [declarations declarations
         symbol-table symbol-table]
    (if-let [declaration (first declarations)]
      (recur (next declarations)
             (add-declaration-vars symbol-table (:type declaration) (:vars declaration)))
      symbol-table)))

(defn add-function
  "Adds or modifies a function in the symbol table based on what is currently declared/defined."
  [symbol-table {:keys [function-name params return-type]} defined?]
  (if-let [entry (find-function-entry symbol-table function-name)]
    (or
      (cond
        ;; Attempting to define a function that already has a definition
        (:defined? entry)
        (err/raise-error! "The function %s has already been defined!\n" function-name)

        ;; Function definition for a function with a declared prototype
        (and defined? (not (:defined? entry)))
        (if (= (map :type params) (:params entry))
          (assoc symbol-table :entries
                 (replace {entry (assoc entry :defined? true)}
                          (:entries symbol-table)))
          (err/raise-error! "Attempting to change declared arguments of %s from (%s) to (%s)!\n"
                            function-name
                            (s/join ", " (:params entry))
                            (s/join ", " (map :type params))))

        ;; Duplicate function prototypes
        (not-any? true? [defined? (:defined? entry)])
        (err/raise-error! "Attempting to redeclare declared function %s\n" function-name))
      symbol-table)

    ;; Function hasn't been previously defined
    (let [param-types (map :type params)
          function (Function. function-name return-type param-types defined?)]
      (debug-msg "New fn: %s :: (%s) -> %s\n" function-name (s/join ", " param-types) return-type)
      (assoc symbol-table :entries (conj (:entries symbol-table) function)))))

;;; Symbol table predicates
(defn function-defined? [symbol-table function]
  (if-let [function (find-function-entry symbol-table (:function-name function))]
    (:defined? function)
    false))

(defn function-declared? [symbol-table function]
  (some? (find-function-entry symbol-table (:function-name function))))
