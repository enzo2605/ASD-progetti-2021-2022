#ifndef EDGE_H
#define EDGE_H


/**
 * La classe Edge rappresenta gli archi del grafo. In essa troviamo
 * il nodo sorgente, il nodo destinazione e il costo dell'arco.
 * @tparam T
 * @tparam S
 */
template <class T>
class Edge {
    private:
        Vertex<T> *src;
        Vertex<T> *dest;
        T cost;
    public:
        Edge() {
            src = nullptr;
            dest = nullptr;
            cost = 0;
        }

        Edge(T cost, Vertex<T> *src, Vertex<T> *dest) {
            this->src = src;
            this->dest = dest;
            this->cost = cost;
        }

        Vertex<T> *getSrc() const {
            return src;
        }

        Vertex<T> *getDest() const {
            return dest;
        }

        T getCost() const {
            return cost;
        }

        friend bool operator< (const Edge<T> &e1, const Edge<T> &e2) {
            return  (e1.getCost() < e2.getCost());
        }

        friend bool operator== (const Edge<T> &e1, const Edge<T> &e2) {
            return ((e1.getSrc() == e2.getSrc()) &&
                    (e1.getDest() == e2.getDest()) &&
                    (e1.getCost() == e2.getCost())
            );
        }
};

#endif // EDGE_H
