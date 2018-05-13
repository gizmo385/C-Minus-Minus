(ns cmm.three
  (:require [cmm.errors :as err]))

(defmulti ->tac
  "Converts a node in an abstract syntax tree to a series of three address codes."
  (fn [symbol-table {:keys [node-type] :as ast}]
    node-type))

(defmethod ->tac :default
  [symbol-table {:keys [node-type] :as ast}]
  (err/raise-error! "Unexpected AST node when generating TAC: %s" node-type))

(defn ast->tac [symbol-table ast-nodes]
  ([symbol-table (flatten (map) ->tac ast-nodes)]))
