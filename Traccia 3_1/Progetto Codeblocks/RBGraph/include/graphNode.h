#ifndef GRAPHNODE_H
#define GRAPHNODE_H


#include <vector>
#include <sstream>
#include <limits>
#include "RBTree.h"

enum class GraphColors { WHITE, GRAY, BLACK };

const int INT_INFINITY = std::numeric_limits<int>::max();

/**
 * La classe graphNode rappresenta i singoli vertici del grafo. In
 * essa troviamo tutti gli attributi e i relativi metodi get e set
 * per la loro manipolazione
 * @tparam T
 */
template <class T>
class graphNode {
private:
    T data;
    GraphColors color;
    int distance;
    graphNode<T>* parent;
    RBTree<graphNode<T> *> *adj;

public:
    graphNode() {
        adj = new RBTree<graphNode<T> *>;
        this->color = GraphColors::WHITE;
        this->distance = INT_INFINITY;
        this->parent = nullptr;
    }

    T getData() {
        return data;
    }

    void setData(T data) {
        this->data = data;
    }

    GraphColors getColor() {
        return color;
    }

    void setColor(GraphColors color) {
        this->color = color;
    }

    int getDistance() {
        return distance;
    }

    void setDistance(int distance) {
        this->distance = distance;
    }

    graphNode<T> *getParentNode() {
        return parent;
    }

    void setParentNode(graphNode<T> *parent) {
        this->parent = parent;
    }

    RBTree<graphNode<T> *> *getAdj() {
        return adj;
    }

    std::string getPrintableAdj() {
        std::stringstream adjList("", std::ios::app | std::ios::out);
        for (auto element : *adj->getNodeList()) {
            adjList << this->getData() << " -> " << element->getKey()->getData() << "\n";
        }
        return adjList.str();
    }

    void addEdge(graphNode<T> *node) {
        adj->insertNode(node);
    }

    void removeEdge(graphNode<T> *node) {
        adj->deleteNode(node);
    }

    bool findNodeInAdjList(graphNode<T> *node) {
        if(adj->findNode(node)) {
            return true;
        }
        return false;
    }
};

#endif // GRAPHNODE_H
