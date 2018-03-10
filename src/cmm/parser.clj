(ns cmm.parser
  "This namespace contains functions related to parsing C files. This process lexical analysis,
  type checking, and error handling."
  (:require [instaparse.core :as insta]
            [clojure.edn :as edn]
            [clojure.string :refer [join]]
            [clojure.pprint :refer [pprint]]
            [cmm.errors :as err]
            [cmm.symbol-table :as sym]
            [cmm.types :as types]
            [cmm.debug :as dbg :refer [debug-msg]]))

;;; Loading the parser from the grammar file
(def whitespace-or-comments
  "This parser handles the whitespace and comment parsing. It is used as the whitespace parser in
   the primary C parser."
  (insta/parser (clojure.java.io/resource "whitespace.bnf") :auto-whitespace :standard))

(def c-parser
  "This is the primary C parser. It uses a custom whitespace parser which handles comments and
   whitespace characters. The results of this parser should be sent to the build-ast function."
  (insta/parser (clojure.java.io/resource "grammar.bnf") :auto-whitespace whitespace-or-comments))

;;; Building the ast
(defmulti build-ast
  "The build-ast function represents the primary parsing function for CMM. This uses the output
   from instaparse to generate a more easily understandable, parseable, and easy-to-use tree
   structure.

   The first argument represents the symbol table of the calling scope. The second argument
   represents a subsection of the parse result."
  (fn [symbol-table program]
    (first program)))

(defn ->ast
  "This function takes a parser node and builds an Abstract Syntax Tree out of that parser node. It
   will also use that parser node as the current context under which errors will be generated."
  ([parser-node]
   (->ast (sym/new-symbol-table) parser-node))

  ([symbol-table parser-node]
   (err/with-error-context parser-node
     (let [[updated-symbol-table ast] (build-ast symbol-table parser-node)]
       [(or updated-symbol-table symbol-table)
        (or ast {:node-type   :error
                 :parser-node parser-node})]))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Parsing top-level symbols
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defmethod build-ast :START
  [symbol-table [_ & program]]
  (debug-msg "Program: %s\n" (-> program pprint with-out-str))
  (loop [program program
         symbol-table symbol-table
         ast []]
    (if (first program)
      (let [[updated-symbol-table sub-ast] (->ast symbol-table (first program))]
        (recur (rest program)
               updated-symbol-table
               (conj ast sub-ast)))
      [symbol-table ast])))


;;; TODO: Handle function declarations without implementations

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Parsing functions and parameters
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defn- handle-statements
  "This handles parsing and building an AST for a sequence of statements"
  [symbol-table statements]
  (loop [symbol-table symbol-table
         ast []
         statements statements]
    (if (first statements)
      (let [[updated-symbol-table node-ast] (->ast symbol-table (first statements))]
        (recur updated-symbol-table
               (conj ast node-ast)
               (rest statements)))
      [symbol-table ast])))

;; TODO: Ensure that SOMETHING is returned from a function with a declared return type in every
;; situation
(defmethod build-ast :FUNCTION
  [symbol-table [_ type [_ name] arguments declarations & statements]]
  (let [arg-types                             (types/argument-types arguments)
        ;; Let's determine what the return type for this function is going to be
        return-type                           (types/keyword->type type)

        ;; Let's create our inner and outer symbol tables for this function
        updated-outer-scope                   (sym/add-function symbol-table return-type name arg-types)
        function-inner-scope                  (sym/new-symbol-table updated-outer-scope)]

    ;; Once we've defined our scope and return type, we'll parse the various elements of the
    ;; function within the context of this function's expected return type. This allows us to type
    ;; check return statements and ensure that they match up to the current function's expected
    ;; return type
    (types/with-return-type return-type
      (let [[argument-scope argument-ast]         (->ast function-inner-scope arguments)
            [declaration-scope declaration-ast]   (->ast argument-scope declarations)
            [statement-scope statement-ast]       (handle-statements declaration-scope statements)
            body-ast                              (concat declaration-ast statement-ast)]
        [statement-scope
         {:node-type    :function
          :name         name
          :return-type  return-type
          :arguments    argument-ast
          :body         body-ast
          :symbol-table statement-scope}]))))

(defmethod build-ast :PARAMS
  [symbol-table [_ & parameters]]
  (loop [symbol-table symbol-table
         parameters parameters
         asts []]
    (if (first parameters)
      ;; We'll destructure the parameter to retrieve type and name information. We'll also create a
     ;; new AST node
      (let [[_ type [_ name]] (first parameters)
            parameter-type (types/keyword->type type)
            param-ast {:node-type :parameter
                       :name      parameter-type
                       :type      type}]
        ;; We'll add this new variable to the symbol table and proceed onto the next argument
        (recur (sym/add-variable symbol-table parameter-type name)
               (rest parameters)
               (conj asts param-ast)))
      [symbol-table asts])))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Parsing variable declarations
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defn- handle-declarations
  "Handles the parsing out the comma-separated names in a variable declaration."
  [symbol-table type declarations]
  ;; We want to loop over the various individual declarations in the comma-separated list.
  (loop [declarations declarations
         ast-nodes []
         symbol-table symbol-table]
    ;; For each, we'll extract the name and array size (if presnet) of each declaration and create
    ;; a new AST node.
    (if-let [[_ [_ name] [_ array-size]] (first declarations)]
      (let [new-ast-node {:node-type  :variable-declaration
                          :name       name
                          :array?     (some? array-size)
                          :array-size array-size
                          :type       type}]
        ;; We'll add a new entry to the symbol table and go back around the loop to tackle the next
        ;; element in the declaration.
        (recur
          (rest declarations)
          (conj ast-nodes new-ast-node)
          (sym/add-variable symbol-table type name array-size)))
      [symbol-table ast-nodes])))

(defmethod build-ast :DECLARATIONS
  [symbol-table [_ & declarations]]
  (loop [symbol-table symbol-table
         declarations declarations
         asts []]
    (if (first declarations)
      ;; We'll destructure the parameter to retrieve type and name information. We'll also create a
      ;; new AST node
      (let [[_ type & names] (first declarations)
            declared-type (types/keyword->type type)
            [updated-table decl-ast]  (handle-declarations symbol-table declared-type names) ]
        ;; We'll add this new variable to the symbol table and proceed onto the next argument
        (recur updated-table
               (rest declarations)
               (concat asts decl-ast)))
      [symbol-table asts])))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Expression Parsing
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defn literal->ast
  "This wraps a simple value type's parser node into an AST node. For example, [:INTEGER 5] would
   get returned as:
      {:node-type  :literal
      :type        :INTEGER
      :value       5}"
  [[type value]]
  {:node-type :literal
   :type      (types/keyword->type type)
   :value     value})

(defn expression->ast
  "Handles parsing and building out the AST for a nested expression tree."
  [symbol-table expression]
  (let [expression-type (first expression)]
    (case expression-type
      ;; For any sort of nested expression, we'll dispatch out to the build-ast function and
      ;; explicitly return only the generated AST
      :BINARY_EXPRESSION  (second (->ast symbol-table expression))
      :UNARY_EXPRESSION   (second (->ast symbol-table expression))
      :FUNCTION_CALL      (second (->ast symbol-table expression))
      :ARRAY_ACCESS       (second (->ast symbol-table expression))

      ;; For an ID, we'll just create a simple variable node after checking that the name is defined
      ;; in the supplied symbol table
      :ID (let [variable-name (second expression)
                variable (sym/variable-defined? symbol-table variable-name)]
            (if (nil? variable)
              (err/raise-error! "Could not find variable %s in expression\n" variable-name)
              {:node-type     :variable
               :type          (:type variable)
               :variable-name variable-name}))

      ;; By default, we'll assume the expression is just a value
      (literal->ast expression))))

(defmethod build-ast :BINARY_EXPRESSION
  [symbol-table [_ left [operator] right]]
  ;; First we want to create ASTs for the left and right hand sides of the expression.
  (let [left-ast      (expression->ast symbol-table left)
        right-ast     (expression->ast symbol-table right)
        operator      (types/keyword->operator operator)
        result-type   (types/expression-result-type operator left-ast right-ast)]
    ;; We'll ensure that the left and right operands match the expected types for the operator
    (types/check-operator operator [(:type left-ast) (:type right-ast)])

    ;; Once we do that, we'll return an AST for the entire expression
    [symbol-table
     {:node-type      :binary-expression
      :left-operand   left-ast
      :right-operand  right-ast
      :type           result-type
      :operator       operator}]))

(defmethod build-ast :UNARY_EXPRESSION
  [symbol-table [_ [operator] operand]]
  ;; We need to create an AST for the operand.
  (let [operand-ast (expression->ast symbol-table operand)
        operator    (types/keyword->operator operator)
        result-type (types/expression-result-type operator operand-ast)]
    (types/check-operator operator [(:type operand-ast)])
    [symbol-table
     {:node-type  :unary-expression
      :operator   operator
      :type       result-type
      :operand    operand-ast}]))

(defmethod build-ast :FUNCTION_CALL
  [symbol-table [_ [_ function-name] & arguments]]
  (let [argument-ast    (into [] (map (partial expression->ast symbol-table) arguments))
        argument-types  (map :type argument-ast)
        function        (sym/function-defined? symbol-table function-name argument-types)]
    (cond
      ;; Ensure that the function is actually defined
      (nil? function) (err/raise-error! "Cannot find function '%s' with argument types %s\n"
                                        function-name (into [] argument-types))

      :default [symbol-table
                {:node-type     :function-call-expression
                 :type          (:return-type function)
                 :function-name function-name
                 :arguments     argument-ast}])))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Assignment Parsing
;
; TODO: Ensure that everything related to assignments to array indices is functioning correctly
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defn- parse-expression-assignment
  [symbol-table destination-variable array-index expression]
  (let [[_ expression-ast] (->ast symbol-table expression)]
    (types/check-assignment destination-variable (:type expression-ast))
    [symbol-table
     {:node-type        :expression-assignment
      :destination      (:name destination-variable)
      :destination-type (:type destination-variable)
      :array-index      array-index
      :expression       expression-ast}]))

(defn- parse-variable-assignment
  [symbol-table destination-variable array-index [_ source]]
  (if-let [source-variable (sym/variable-defined? symbol-table source)]
    (do
      (types/check-assignment destination-variable (:type source-variable))
      [symbol-table
       {:node-type        :variable-assignment
        :destination      (:name destination-variable)
        :destination-type (:type destination-variable)
        :array-index      array-index
        :source           source
        :source-type      (:type source-variable)}])
    (err/raise-error! "Cannot assign '%s' to '%s', cannot find variable '%s'\n"
                      source (:name destination-variable) source)))

(defn- parse-literal-assignment
  [symbol-table destination-variable array-index value value-type]
  (let [value-type (types/keyword->type value-type)]
    (types/check-assignment destination-variable value-type)
    [symbol-table
     {:node-type        :literal-assignment
      :destination      destination-variable
      :destination-type (:type destination-variable)
      :array-index      array-index
      :source           value
      :source-type      value-type}]))

(defmethod build-ast :ASSIGNMENT
  [symbol-table [_ [_ destination] & assignment-body]]
  (let [array-assignment? (= 2 (count assignment-body))
        array-index (if array-assignment?
                      (expression->ast symbol-table (first assignment-body)))
        source (if array-assignment?
                 (second assignment-body)
                 (first assignment-body))]
    ;; Since we're assigning to a variable, we need to check that the variable we're attempting to
    ;; assign to exists
    (if-let [destination-variable (sym/variable-defined? symbol-table destination)]
      ;; We want to determine what kind of assignment is taking place (literal, variable, expression)
      (let [[value-type value] source]
        (case value-type
          ;; These are cases where we'll need more complicate parsing to generate the AST for the
          ;; assignment
          :BINARY_EXPRESSION  (parse-expression-assignment symbol-table destination-variable
                                                           array-index source)
          :UNARY_EXPRESSION   (parse-expression-assignment symbol-table destination-variable
                                                           array-index source)
          :FUNCTION_CALL      (parse-expression-assignment symbol-table destination-variable
                                                           array-index source)
          :ID                 (parse-variable-assignment symbol-table destination-variable
                                                         array-index source)

          ;; In the default case (literal assignment), we'll just create a simple AST node
          (parse-literal-assignment
            symbol-table destination-variable array-index value value-type)))
      (err/raise-error! "Cannot assign to '%s', variable does not exist!\n" destination))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Statement Parsing
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defmethod build-ast :STATEMENT_LIST
  [symbol-table [_ & body]]
  (handle-statements symbol-table body))

(defmethod build-ast :FOR_LOOP
  [symbol-table [_ [_ initialization] [_ condition] [_ afterthought] body]]
  (let [[_ initialization-ast]  (->ast symbol-table initialization)
        condition-ast           (expression->ast symbol-table condition)
        [_ afterthought-ast]    (->ast symbol-table afterthought)
        [_ body-ast]            (->ast symbol-table body)]
    ;; We want to ensure that the return type of the expression in the condition AST is a boolean
    ;; expression, otherwise the condition doesn't make sense.
    (if (not (types/parent? types/BOOLEAN (:type condition-ast)))
      (err/raise-error! "Expected Boolean expression in for loop condition, found %s\n"
                        (-> condition-ast :type :name)))
    [symbol-table
     {:node-type       :for-loop
      :initialization  initialization-ast
      :condition       condition-ast
      :afterthought    afterthought-ast
      :body            body-ast}]))

(defmethod build-ast :WHILE_LOOP
  [symbol-table [_ condition body]]
  (let [condition-ast     (expression->ast symbol-table condition)
        [_ body-ast]      (->ast symbol-table body)]
    ;; We want to ensure that the return type of the expression in the condition AST is a boolean
    ;; expression, otherwise the condition doesn't make sense.
    (if (not (types/parent? types/BOOLEAN (:type condition-ast)))
      (err/raise-error! "Expected Boolean expression in while loop condition, found %s\n"
                        (-> condition-ast :type :name)))
    [symbol-table
     {:node-type  :while-loop
      :condition  condition-ast
      :body       body-ast}]))

(defmethod build-ast :IF
  [symbol-table [_ condition then]]
  (let [condition-ast     (expression->ast symbol-table condition)
        [_ then-ast]      (->ast symbol-table then)]
    ;; We want to ensure that the return type of the expression in the condition AST is a boolean
    ;; expression, otherwise the condition doesn't make sense.
    (if (not (types/parent? types/BOOLEAN (:type condition-ast)))
      (err/raise-error! "Expected Boolean expression in if condition, found %s\n"
                        (-> condition-ast :type :name)))
    [symbol-table
     {:node-type :if-statement
      :condition condition-ast
      :then      then-ast}]))

(defmethod build-ast :IF_ELSE
  [symbol-table [_ condition then else]]
  (let [condition-ast     (expression->ast symbol-table condition)
        [_ then-ast]      (->ast symbol-table then)
        [_ else-ast]      (->ast symbol-table else)]
    ;; We want to ensure that the return type of the expression in the condition AST is a boolean
    ;; expression, otherwise the condition doesn't make sense.
    (if (not (types/parent? types/BOOLEAN (:type condition-ast)))
      (err/raise-error! "Expected Boolean expression in if/else condition, found %s\n"
                        (-> condition-ast :type :name)))
    [symbol-table
     {:node-type  :if-else-statement
      :condition  condition-ast
      :then       then-ast
      :else       else-ast}]))

(defmethod build-ast :RETURN
  [symbol-table [_ return]]
  (let [return-ast  (if (some? return) (expression->ast symbol-table return))
        return-type (:type return-ast)]
    ;; We need to type check the return statement and ensure that the type of the expression matches
    ;; the expected return type of the current function.
    (if (some? return-type)
      ;; Check to see if the type of the expression is not a child of the declared return type.
      (if (not (types/parent? types/*current-return-type* return-type))
        (err/raise-error! "Cannot return %s from function declared to return %s.\n"
                          (:name return-type) (:name types/*current-return-type*)))
      ;; If there is no expression associated with this return statement, then the function must
      ;; have been declared as VOID, otherwise this is a type error.
      (if (not= types/VOID types/*current-return-type*)
        (err/raise-error! "Cannot return nothing from a function declared to return %s\n"
                          (:name types/*current-return-type*))))
    [symbol-table
     {:node-type     :return-statement
      :return-value  return-ast}]))

(defmethod build-ast :FUNCTION_CALL_STATEMENT
  [symbol-table [_ function-call-statement]]
  (let [call-ast (expression->ast symbol-table function-call-statement)]
    [symbol-table
     {:node-type      :function-call-statement
      :function-call  call-ast}]))

(defmethod build-ast :default [symbol-table unrecognized-node]
  (let [unrecognized-tag (first unrecognized-node)]
    (err/raise-error! "Unrecognized symbol of with tag %s: %s\n"
                      unrecognized-tag unrecognized-node)))

;;; These are functions to help us simplify aspects of our parsing (such as converting
;;; integers and floats) or handling some quick type assignment
(defonce c-names->keyword
  {"int"      :INTEGER
   "float"    :FLOAT
   "double"   :DOUBLE
   "boolean"  :BOOLEAN
   "char"     :CHAR_CONST
   "void"     :VOID})

(defonce transformation-map
  {:INTEGER (fn [v] [:INTEGER (read-string v)])
   :FLOAT   (fn [v] [:FLOAT (read-string v)])
   :void    (fn [& _] :VOID)
   :TYPE    (fn [v] (c-names->keyword v))})

;;; User facing functions
(defn parse [source]
  (->> source
       (insta/parse c-parser)
       (insta/transform transformation-map)
       (insta/add-line-and-column-info-to-metadata source)
       (->ast)))
