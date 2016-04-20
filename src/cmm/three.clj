(ns cmm.three
  (:require [cmm.errors :as err]
            [cmm.debug :refer [debug-msg]]))

;;; Creating three address codes
(defrecord ThreeAddressCode [operation destination src1 src2])

(defn- new-tac
  "Variadic function to quickly create new three address codes without gratuitous nil's"
  ([operation] (ThreeAddressCode. operation nil nil nil))
  ([operation destination] (ThreeAddressCode. operation destination nil nil))
  ([operation destination src1] (ThreeAddressCode. operation destination src1 nil))
  ([operation destination src1 src2] (ThreeAddressCode. operation destination src1 src2)))

;;; Creating labels
(defn- new-random-label []
  (let [label (gensym "_label")]
    (str label)))

;;; Creating temporary variables
(defn- new-temporary-variable []
  (str (gensym "_tmp")))

;;; Convinience functions
(defn- tac-list
  "This takes a sequence of Three Address Codes (or lists of Three Address Codes) and puts them
   all into a single, flattened list"
  [& tacs]
  (filter some? (flatten (mapcat (fn [l] (if (list? l) l (list l))) tacs))))

;;; Define TAC generation multimethod
(defmulti generate-tac-helper
  "Generates a list of three address codes based on the AST supplied."
  (fn [ast] (or (:statement-type ast) (:node-type ast))))

;;; Helper functions for booleans and conditionals
(defn- relational-operator-tac
  "Generates three address code to handle branching statements"
  [operator left right true-destination false-destination]
  (let [[left-place left-code] (generate-tac-helper left)
        [right-place right-code] (generate-tac-helper right)]
    (tac-list
      left-code
      right-code
      (new-tac operator true-destination left-place right-place)
      (new-tac :goto false-destination))))

(defn- boolean-jumping-tac
  "Generates three address codes that represent a boolean computation followed by a jump based on
   the result of the boolean computation."
  [condition true-destination false-destination]
  (case (:operator condition)
    ;; Boolean operations
    :and
    (let [separator (new-random-label)]
      (tac-list
        (boolean-jumping-tac (:left-operand condition) separator false-destination)
        (new-tac :label separator)
        (boolean-jumping-tac (:right-operand condition) true-destination false-destination)))
    :or
    (let [separator (new-random-label)]
      (tac-list
        (boolean-jumping-tac (:left-operand condition) true-destination separator)
        (new-tac :label separator)
        (boolean-jumping-tac (:right-operand condition) true-destination false-destination)))
    :not
    (boolean-jumping-tac (:operand condition) false-destination true-destination)

    ;; Comparison operations form the remainder of the use cases
    (relational-operator-tac (:operator condition) (:left-operand condition)
                             (:right-operand condition) true-destination
                             false-destination)))

;;; TAC generation for functions
(defmethod generate-tac-helper :function [function]
  (debug-msg "Generating TAC for function: %s\n" (:function-name function))
  (tac-list
    (new-tac :label (:function-name function))
    (map generate-tac-helper (:body function))))

;;; TAC generation for statements
(defmethod generate-tac-helper :return [return]
  (if (:value return)
    (let [[place code] (generate-tac-helper (:value return))]
      (tac-list
        code
        (new-tac :retval place)
        (new-tac :return-from-func))))
  (tac-list (new-tac :return-from-func)))

(defmethod generate-tac-helper :while-loop [while-loop]
  (debug-msg "Generating TAC for while-loop\n")
  (let [eval-label (new-random-label)
        top-label (new-random-label)
        after-label (new-random-label)]
    (tac-list
      ;; Jump to the condition
      (new-tac :goto eval-label)

      ;; The loop body
      (new-tac :label top-label)
      (doall (mapcat generate-tac-helper (:body while-loop)))

      ;; The condition check
      (new-tac :label eval-label)
      (boolean-jumping-tac (:condition while-loop) top-label after-label)
      (new-tac :label after-label))))

(defmethod generate-tac-helper :for-loop [for-loop]
  (debug-msg "Generating TAC for for-loop\n")
  (let [eval-label (new-random-label)
        top-label (new-random-label)
        after-label (new-random-label)]
    (tac-list
      ;; Jump to the predicate evaluation
      (new-tac :goto eval-label)

      ;; For loop body
      (new-tac :label top-label)
      (doall (mapcat generate-tac-helper (:body for-loop)))
      (generate-tac-helper (:update for-loop))

      ;; Predicate evaluation
      (new-tac :label eval-label)
      (boolean-jumping-tac (:condition for-loop) top-label after-label)
      (new-tac :label after-label))))

(defmethod generate-tac-helper :conditional [conditional]
  (let [then-label (new-random-label)
        else-label (new-random-label)
        after-label (new-random-label)]
    (tac-list
      ;; Check the condition
      (boolean-jumping-tac (:condition conditional)
                           then-label
                           (if (:else conditional) else-label after-label))

      ;; Then body
      (new-tac :label then-label)
      (doall (mapcat generate-tac-helper (:then conditional)))

      ;; Else body, if present
      (if (:else conditional)
        (tac-list
          (new-tac :label else-label)
          (doall (mapcat generate-tac-helper (:else conditional)))))

      ;; After loop
      (new-tac :label after-label))))

(defmethod generate-tac-helper :assignment [assg]
  (if (:index assg)
    (let [[index-place index-code] (generate-tac-helper (:index assg))
          [value-place value-code] (generate-tac-helper (:value assg))]
      (debug-msg "TAC: %s[%s] = %s\n" (:id assg) index-place value-place)
      (tac-list
        value-code
        index-code
        (new-tac :index-assignment (:id assg) index-place value-place)))
    (let [[value-place value-code] (generate-tac-helper (:value assg))]
      (debug-msg "TAC: %s = %s\n" (:id assg) value-place)
      (tac-list
        value-code
        (new-tac :assignment (:id assg) value-place)))))

;;; TAC generation for expressions
(defmethod generate-tac-helper :unary-expression [expr]
  (case (:operator expr)
    :minus
    (let [result (new-temporary-variable)
          [place code] (generate-tac-helper (:operand expr))]
      (debug-msg "TAC: %s = %s %s\n" result :unary-minus place)
      [result
       (tac-list
         code
         (new-tac :unary-minus result place))])
    (err/raise-error! "Invalid unary operator: %s\n" (:operator expr))))

(defmethod generate-tac-helper :binary-expression [expr]
  (let [result (new-temporary-variable)
        [left-place left-code] (generate-tac-helper (:left-operand expr))
        [right-place right-code] (generate-tac-helper (:right-operand expr))]
    (debug-msg "TAC: %s %s %s\n" left-place (:operator expr) right-place)
    [result
     (tac-list
       left-code
       right-code
       (new-tac (:operator expr) result left-place right-place))]))

(defn- function-call-helper
  "Functions can be present in an expression or statment. The generated TAC code is very similar
   except for the value of result. This is the area that the result of the function call is stored
   in. For statements this is nil, and for expressions is the name of a variable."
  [func result]
  (tac-list
    ;; Handle arguments
    (for [param (:arguments func)
          :let [[place code] (generate-tac-helper param)]]
      (tac-list
        code
        (new-tac :param place)))

    ;; The actual function call
    (new-tac :call result (:function-name func))))

(defmethod generate-tac-helper :function-call-statement
  [func]
  (function-call-helper func nil))

(defmethod generate-tac-helper :function-call
  [func]
  (debug-msg "Generating TAC for call to function: %s\n" (:function-name func))
  (let [result (new-temporary-variable)]
    ;; For function calls in expressions, the result is expected by our caller
    [result (function-call-helper func result)]))

;;; TAC generation for various literal expressions
(defmethod generate-tac-helper :id-expression [expr]
  ;; Only need to return the place, no code necessary
  [(:id expr) nil])

(defmethod generate-tac-helper :literal-expression [expr]
  (let [result (new-temporary-variable)]
    (debug-msg "%s = %s\n" result (:value expr))
    [result
     (tac-list (new-tac :assg-literal result (:value expr)))]))

;;; Handling invalid node-types
(defmethod generate-tac-helper :default [ast]
  (err/raise-error! "Cannot parse AST node of type %s: %s\n" (:node-type ast) ast))

;;; User facing functions
(defn generate-tac [program]
  (mapcat generate-tac-helper program))

