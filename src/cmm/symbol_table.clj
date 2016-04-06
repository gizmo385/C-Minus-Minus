(ns cmm.symbol-table)

;;; Symbol table structure
(defrecord SymbolTable [parent entries])
(defrecord Variable [symbol-name symbol-type array])
(defrecord Function [function-name function-type defined?]) ;; TODO: Add parameter types

;; Constructors
(defn new-symbol-table
  ([] (SymbolTable. nil []))
  ([parent] (SymbolTable. parent []))
  ([parent entries] (SymbolTable. parent entries)))

;;; Creating and finding entries
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
           (filter #(= (:function-name %) function-name))
           (filter #(= (type %) Function))
           (first)))))

(defn- add-variable-entry
  "Makes a new symbol table entry for a variable and places it into the table"
  [symbol-table symbol-name symbol-type array]
  (if (find-variable-entry symbol-table symbol-name 1)
    (printf "Error: Cannot add %s to the symbol table, already declared" symbol-name)
    (let [entry (Variable. symbol-name symbol-type array)]
      (assoc symbol-table :entries (conj (:entries symbol-table) entry)))))

;;; Symbol table insertion functions, for paramters and declarations
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

(defn add-function [symbol-table {:keys [function-name return-type]} defined?]
  (if (find-function-entry symbol-table function-name)
    (printf "Error: Attempting to redeclare function with name %s\n" function-name)
    (let [function (Function. function-name return-type defined?)]
      (assoc symbol-table :entries (conj (:entries symbol-table) function)))))
