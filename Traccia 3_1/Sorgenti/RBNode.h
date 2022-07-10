#ifndef RBNODE_H
#define RBNODE_H


enum colors { BLACK, RED };

/**
 * Rappresenta il nodo dell'albero red black, con
 * opportuni attributi. Sostanzialmente la classe
 * si compone di metodi get e set per poter ritornare
 * e modificare gli attributi
 * @tparam T
 */
template <class T>
class RBNode {
    private:
        T key;
        RBNode<T> *parent;
        RBNode<T> *left;
        RBNode<T> *right;
        enum colors color;

    public:
        RBNode() : parent(nullptr), left(nullptr), right(nullptr) {
            color = BLACK;
        }

        void setKey(T key) {
            this->key = key;
        }

        T getKey() {
            return key;
        }

        RBNode<T> *getParent() const {
            return parent;
        }

        void setParent(RBNode<T> *parent) {
            this->parent = parent;
        }

        RBNode<T> *getLeft() const {
            return left;
        }

        void setLeft(RBNode<T> *left) {
            this->left = left;
        }

        RBNode<T> *getRight() const {
            return right;
        }

        void setRight(RBNode<T> *right) {
            this->right = right;
        }

        colors getColor() const {
            return color;
        }

        void setColor(enum colors color) {
            this->color = color;
        }
};

#endif // RBNODE_H
