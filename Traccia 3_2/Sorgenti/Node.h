#ifndef NODE_H
#define NODE_H

/**
 * Classe che rappresenta il singolo nodo dell'insieme disgiunto
 * @tparam T
 */
template <class T>
class Node {
    private:
        Node<T> *parent;
        int rank;
        T data;
    public:
        Node(T data) {
            this->data = data;
        }
        T getData() {
            return data;
        }
        void setParent(Node<T> *x) { parent = x; }
        Node<T> *getParent() { return parent; }
        void setRank(int rank) { this->rank = rank;}
        int getRank() { return rank; }
};

#endif // NODE_H
