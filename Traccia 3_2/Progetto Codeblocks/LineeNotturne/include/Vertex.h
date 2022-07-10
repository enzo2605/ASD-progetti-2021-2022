#ifndef VERTEX_H
#define VERTEX_H


#include <vector>
#include <sstream>
#include <limits>
#include <list>

enum class GraphColors { WHITE, GRAY, BLACK };

const int INT_INFINITY = std::numeric_limits<int>::max();

/**
 * La classe Vertex rappresenta i singoli vertici del grafo. In
 * essa troviamo tutti gli attributi e i relativi metodi get e set
 * per la loro manipolazione
 * @tparam T
 */
template <class T>
class Vertex {
    private:
        T data;
        GraphColors color;
        int distance;
        Vertex<T>* parent;
        std::list<Vertex<T> *> *adj;

    public:
        Vertex() {
            adj = new std::list<Vertex<T> *>;
            this->color = GraphColors::WHITE;
            this->distance = INT_INFINITY;
            this->parent = nullptr;
        }

        Vertex(T data) {
            adj = new std::list<Vertex<T> *>;
            this->color = GraphColors::WHITE;
            this->distance = INT_INFINITY;
            this->parent = nullptr;
            this->data = data;
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

        Vertex<T> *getParentNode() {
            return parent;
        }

        void setParentNode(Vertex<T> *parent) {
            this->parent = parent;
        }

        std::list<Vertex<T> *> *getAdj() {
            return adj;
        }

        std::string getPrintableAdj() {
            std::stringstream adjList("", std::ios::app | std::ios::out);
            for (auto element : *adj) {
                adjList << this->getData() << " -> " << element->getData() << "\n";
            }
            return adjList.str();
        }

        void addEdge(Vertex<T> *node) {
            adj->push_back(node);
        }

        void removeEdge(Vertex<T> *node) {
            adj->remove(node);
        }

        bool findNodeInAdjList(Vertex<T> *node) {
            for (auto vertex : *adj) {
                if (vertex->getData() == node->getData()) {
                    return true;
                }
            }
            return false;
        }
};

#endif // VERTEX_H
