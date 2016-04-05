(ns cmm.symbol-table)

;;; Symbol table structure
(defrecord SymbolTable [parent entries])
(defrecord TableEntry [symbol-name symbol-type array])

;; Constructors
(defn new-symbol-table [parent entries]
  (SymbolTable. parent entries))

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
     (if-let [results (not-empty (filter #(= (:symbol-name %) symbol-name) entries))]
       results
       (find-variable-entry parent symbol-name (dec search-depth))))))

(defn- add-entry
  "Makes a new symbol table entry and places it into the table"
  [symbol-table symbol-name symbol-type array]
  (if (find-variable-entry symbol-table symbol-name 1)
    (printf "Error: Cannot add %s to the symbol table, already declared" symbol-name)
    (let [entry (TableEntry. symbol-name symbol-type array)]
      (assoc symbol-table :entries (conj (:entries symbol-table) entry)))))

;;; Symbol table insertion functions, for paramters and declarations
(defn add-parameters
  "Traverses the parameter structures and add thoses variables to the symbol table. Returns the
   symbol table"
  [symbol-table params]
  (loop [symbol-table symbol-table
         params params]
    (if-let [param (first params)]
      (recur (add-entry symbol-table (:name param) (:type param) (:array param))
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
             (add-entry symbol-table (:name v) symbol-types (not= nil (:array-size v))))
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
