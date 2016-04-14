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
(defn new-label [label]
  (new-tac :label label))

(defn new-random-label []
  (let [label (gensym "_label")]
    (debug-msg "Creating new random label: %s\n" label)
    (new-label (str label))))

;;; Generating three address codes
(defn- boolean-jumping-tac
  "Generates three address codes that represent a boolean computation followed by a jump based on
   the result of the boolean computation."
  [condition true-destination false-destination]
  (case (:operator condition)
    ;; Boolean operations
    :and
    (let [separator (new-random-label)]
      (concat
        (boolean-jumping-tac (:left-operand condition) separator false-destination)
        (list separator)
        (boolean-jumping-tac (:right-operand condition) true-destination false-destination)))
    :or
    (let [separator (new-random-label)]
      (concat
        (boolean-jumping-tac (:left-operand condition) true-destination separator)
        (list separator)
        (boolean-jumping-tac (:right-operand condition) true-destination false-destination)))
    :not
    (boolean-jumping-tac (:operand condition) false-destination true-destination)

    ;; Comparison operations
    :gte nil
    :lte nil
    :gt nil
    :lt nil
    :eq nil
    :neq nil))

(defmulti generate-tac-helper
  "Generates a list of three address codes based on the AST supplied."
  (fn [ast] (or (:statement-type ast) (:node-type ast))))

(defmethod generate-tac-helper :function [function]
  (debug-msg "Generating TAC for function: %s\n" (:function-name function))
  (doall (map generate-tac-helper (:body function))))

(defmethod generate-tac-helper :function-call
  [func]
  (debug-msg "Generating TAC for call to function: %s\n" (:function-name func)))

(defmethod generate-tac-helper :while-loop [while-loop]
  (debug-msg "Generating TAC for while loop\n")
  (comment
    (let [eval-label (new-random-label)
          top-label (new-random-label)
          after-label (new-random-label)]
      (concat
        (list (new-tac :goto eval-label))
        (map generate-tac-helper (:body while-loop))
        (boolean-jumping-tac (:condition while-loop) top-label after-label)))))

(defmethod generate-tac-helper :default [ast]
  (err/raise-error! "Cannot parse AST node of type %s: %s\n" (:node-type ast) ast))

;;; User facing functions
(defn generate-tac [program]
  (mapcat generate-tac-helper program))

