#ifndef DISJOINTSET_H
#define DISJOINTSET_H


#include <iostream>
#include <vector>
#include "Node.h"

/**
 * Classe che implementa un insieme disgiunto. Fa uso di una classe node
 * per simulare i nodi dell'insieme disgiunto. Implementa i metodi necessari
 * quali makeSet, findSet, Union e Link
 * @tparam T
 */
template <class T>
class DisjointSet {
    private:
        std::vector<Node<T> *> nodes;
        Node<T> *findSetHelper(Node<T> *xNode) {
            if (xNode != xNode->getParent()) {
                xNode->setParent(findSetHelper(xNode->getParent()));
            }
            return xNode->getParent();
        }
    public:
        void makeSet(T x) {
            Node<T> *xNode = new Node<T>(x);
            nodes.push_back(xNode);
            xNode->setParent(xNode);
            xNode->setRank(0);
        }

        Node<T> *findSet(T x) {
            Node<T> *xNode;
            for (auto node : nodes) {
                if (node->getData() == x) {
                    xNode = node;
                    break;
                }
            }
            return findSetHelper(xNode);
        }

        void Union(T u, T v) {
            Link(findSet(u), findSet(v));
        }

        void Link(Node<T> *x, Node<T> *y) {
            if (x->getRank() > y->getRank()) {
                y->setParent(x);
            }
            else {
                x->setParent(y);
                if (x->getRank() == y->getRank()) {
                    y->setRank(y->getRank() + 1);
                }
            }
        }
};

#endif // DISJOINTSET_H
