(ns cmm.three
  (:require [cmm.errors :as err]
            [cmm.symbol-table :as sym]
            [cmm.types :as types]))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Functions to help while generating Three Address Codes
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defn- random-variable-name
  "Generates a temporary variable name with an optional prefix."
  ([]
   (random-variable-name "_temp_"))
  ([prefix]
   (let [random-suffix (int (rand 10000000))]
     (format "%s%d" prefix random-suffix))))

(defn new-temporary-variable
  "Creates a new 'temporary' variable in the supplied symbol table and returns the symbol table
   and the new variable name."
  [symbol-table variable-type value]
  (loop [variable-name (random-variable-name)]
    (if (sym/variable-defined? symbol-table variable-name)
      (recur (random-variable-name))
      [(sym/add-variable symbol-table variable-type variable-name value)
       variable-name])))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Setting up functions to generate Three Address Codes based on AST nodes
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defmulti ->tac
  "Converts a node in an abstract syntax tree to a series of three address codes."
  (fn [symbol-table codes {:keys [node-type] :as ast}]
    node-type))

(defmethod ->tac :default
  [symbol-table codes {:keys [node-type] :as ast}]
  (err/raise-error! "Unexpected AST node when generating TAC: %s" node-type))

(defn ast->tac [symbol-table ast-nodes]
  ([symbol-table (flatten (map) ->tac ast-nodes)]))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Generating Three Address Codes for different kinds of expressions
;
; In order, these are:
;   - Literal/constant expressions
;   - Variable expressions
;   - Array Access TODO
;   - Function call expressions
;   - Unary expressions TODO
;   - Binary expressions TODO
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defmethod ->tac :literal
  [symbol-table codes {:keys [type value]}]
  (let [temporary-variable (new-temporary-variable symbol-table type value)
        assignment-code {}]
    [symbol-table (conj codes {:code-type :literal-expression
                               :target temporary-variable})]))

(defmethod ->tac :variable
  [symbol-table codes {:keys [variable-name]}]
  (if-let [variable (sym/variable-defined? symbol-table variable-name)]
    [symbol-table (conj codes {:code-type :variable-expression
                               :source variable})]
    (err/raise-error! "Could not find variable when generating three address code: %s"
                      variable-name)) )

(defn- arguments->tac
  "Helper function to generate three address code for the arguments in a function call. Since each
   argument is an expression, we'll just generate TACs separately for each of the arguments and
   then combine those TACs afterwards"
  [symbol-table arguments]
  (loop [symbol-table symbol-table
         code         []
         argument     (first arguments)
         arguments    (rest arguments)]
    (if argument
      (let [[symbol-table argument-code] (->tac symbol-table code argument)]
        (recur symbol-table (conj code argument-code) (first arguments) (rest arguments)))
      [symbol-table code])))

(defmethod ->tac :function-call-expression
  [symbol-table codes {:keys [function-type function-name arguments]}]
  (if-let [function (sym/function-defined? symbol-table)]
    ;; We need to first handle TAC creation for the arguments of the function. This is done with the
    ;; arguments->tac helper defined above. Afterwards, we just need to combine it with the current
    ;; code list.
    (let [[symbol-table argument-code]  (arguments->tac symbol-table arguments)
          code                          (concat codes argument-code)
          [symbol-table result]         (new-temporary-variable symbol-table function-type nil)
          function-call-codes           [{:code-type   :function-call
                                          :function    function}
                                         {:code-type   :retrieve-result
                                          :destination result}]]
      [symbol-table (concat code function-call-codes)])
    (err/raise-error! "Could not find function '%s' when generating Three Address Code"
                      function-name)))

;;; TODO: Find a better way to handle expression results


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Generating Three Address Codes for various kinds of statements
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
