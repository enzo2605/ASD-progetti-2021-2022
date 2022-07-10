#ifndef RBTREE_H
#define RBTREE_H


#include "RBNode.h"
#include <iostream>
#include <string>
#include <vector>

/**
 * Classe template che rappresenta un albero red black
 * @tparam T
 */
template <class T>
class RBTree {
private:
    RBNode<T> *root;
    RBNode<T> *nil = new RBNode<T>;
    std::vector<RBNode<T> *> *nodeList;
    static int numberOfNodes;

    void insertNodeFixup(RBNode<T> *z);
    void deleteNodeFixup(RBNode<T> *z);
    void transplant(RBNode<T> *u, RBNode<T> *v);
    void leftRotate(RBNode<T> *x);
    void rightRotate(RBNode<T> *x);
    void deleteNodeHelper(RBNode<T> *z);
    void deleteNodeFromNodeList(RBNode<T> *z);

protected:
    RBNode<T> *createNewNode(T key);
    void inorderVisit(RBNode<T> *x);
    void preorderVisit(RBNode<T> *x);
    void postorderVisit(RBNode<T> *x);

public:
    RBTree();
    void inorderVisit();
    void preorderVisit();
    void postorderVisit();
    RBNode<T> *treeMaximum(RBNode<T> *x);
    RBNode<T> *treeMinimum(RBNode<T> *x);
    RBNode<T> *searchNode(T key);
    bool findNode(T key);
    void insertNode(T key);
    bool deleteNode(T key);
    std::vector<RBNode<T> *> *getNodeList();

    // Utilities
    int size();
    bool isEmpty();
};

template <class T>
int RBTree<T>::numberOfNodes = 0;

template<class T>
RBTree<T>::RBTree() {
    nodeList = new std::vector<RBNode<T> *>;
    root = nil;
}

template<class T>
void RBTree<T>::insertNodeFixup(RBNode<T> *z) {
    while (z->getParent()->getColor() == RED) {
        if (z->getParent()->getParent()->getLeft() == z->getParent()) {
            RBNode<T> *y = z->getParent()->getParent()->getRight();
            if (y->getColor() == RED) {
                z->getParent()->setColor(BLACK);                                    /// Caso 1
                y->setColor(BLACK);                                                 /// Caso 1
                z->getParent()->getParent()->setColor(RED);                         /// Caso 1
                z = z->getParent()->getParent();                                    /// Caso 1
            }
            else {
                if (z == z->getParent()->getRight()) {
                    z = z->getParent();                                             /// Caso 2
                    leftRotate(z);                                               /// Caso 2
                }
                z->getParent()->setColor(BLACK);                                    /// Caso 3
                z->getParent()->getParent()->setColor(RED);                         /// Caso 3
                rightRotate(z->getParent()->getParent());                        /// Caso 3
            }
        }
        else {
            RBNode<T> *y = z->getParent()->getParent()->getLeft();
            if (y->getColor() == RED) {
                z->getParent()->setColor(BLACK);                                    /// Caso 4
                y->setColor(BLACK);                                                 /// Caso 4
                z->getParent()->getParent()->setColor(RED);                         /// Caso 4
                z = z->getParent()->getParent();                                    /// Caso 4
            }
            else {
                if (z == z->getParent()->getLeft()) {
                    z = z->getParent();                                             /// Caso 5
                    rightRotate(z);                                              /// Caso 5
                }
                z->getParent()->setColor(BLACK);                                    /// Caso 6
                z->getParent()->getParent()->setColor(RED);                         /// Caso 6
                leftRotate(z->getParent()->getParent());                         /// Caso 6
            }
        }
    }
    root->setColor(BLACK);
}

template<class T>
void RBTree<T>::deleteNodeFixup(RBNode<T> *x) {
    while (x->getColor() == BLACK && x != root) {
        if (x == x->getParent()->getLeft()) {
            RBNode<T> *w = x->getParent()->getRight();
            // Caso 1
            if (w->getColor() == RED) {
                w->setColor(BLACK);
                x->getParent()->setColor(RED);
                leftRotate(x->getParent());
                w = x->getParent()->getRight();
            }
            // Caso 2
            if (w->getLeft()->getColor() == BLACK && w->getRight()->getColor() == BLACK) {
                w->setColor(RED);
                x = x->getParent();
            }
            else {
                // Caso 3
                if (w->getRight()->getColor() == BLACK) {
                    w->getLeft()->setColor(BLACK);
                    w->setColor(RED);
                    rightRotate(w);
                    w = x->getParent()->getRight();
                }
                // Caso 4
                w->setColor(x->getParent()->getColor());
                x->getParent()->setColor(BLACK);
                w->getRight()->setColor(BLACK);
                leftRotate(x->getParent());
                x = root;
            }
        }
        else {
            RBNode<T> *w = x->getParent()->getLeft();
            // Caso 1
            if (w->getColor() == RED) {
                w->setColor(BLACK);
                x->getParent()->setColor(RED);
                rightRotate(x->getParent());
                w = x->getParent()->getLeft();
            }
            // Caso 2
            if (w->getRight()->getColor() == BLACK && w->getLeft()->getColor() == BLACK) {
                w->setColor(RED);
                x = x->getParent();
            }
            else {
                // Caso 3
                if (w->getLeft()->getColor() == BLACK) {
                    w->getRight()->setColor(BLACK);
                    w->setColor(RED);
                    leftRotate(w);
                    w = x->getParent()->getLeft();
                }
                // Caso 4
                w->setColor(x->getParent()->getColor());
                x->getParent()->setColor(BLACK);
                w->getLeft()->setColor(BLACK);
                rightRotate(x->getParent());
                x = root;
            }
        }
    }
    x->setColor(BLACK);
}

template<class T>
void RBTree<T>::transplant(RBNode<T> *u, RBNode<T> *v) {
    if (u->getParent() == nil) {
        root = v;
    }
    else if (u == u->getParent()->getLeft()) {
        u->getParent()->setLeft(v);
    }
    else {
        u->getParent()->setRight(v);
    }
    v->setParent(u->getParent());
}

template<class T>
void RBTree<T>::leftRotate(RBNode<T> *x) {
    RBNode<T> *y = x->getRight();
    x->setRight(y->getLeft());
    if (y->getLeft() != nil) {
        y->getLeft()->setParent(y);
    }
    y->setParent(x->getParent());
    if (x->getParent() == nil) {
        root = y;
    }
    else if (x->getParent()->getLeft() == x) {
        x->getParent()->setLeft(y);
    }
    else {
        x->getParent()->setRight(y);
    }
    y->setLeft(x);
    x->setParent(y);
}

template<class T>
void RBTree<T>::rightRotate(RBNode<T> *x) {
    RBNode<T> *y = x->getLeft();
    x->setLeft(y->getRight());
    if (y->getRight() != nil) {
        y->getRight()->setParent(y);
    }
    y->setParent(x->getParent());
    if (x->getParent() == nil) {
        root = y;
    }
    else if (x->getParent()->getRight() == x) {
        x->getParent()->setRight(y);
    }
    else {
        x->getParent()->setLeft(y);
    }
    y->setRight(x);
    x->setParent(y);
}

template<class T>
void RBTree<T>::deleteNodeHelper(RBNode<T> *z) {
    RBNode<T> *x;
    RBNode<T> *y = z;
    enum colors y_original_color = y->getColor();
    if (z->getLeft() == nil) {
        x = z->getRight();
        transplant(z, z->getRight());
    }
    else if (z->getRight() == nil) {
        x = z->getLeft();
        transplant(z, z->getLeft());
    }
    else {
        y = treeMinimum(z->getRight());
        y_original_color = y->getColor();
        x = y->getRight();
        if (y->getParent() == z) {
            x->setParent(y);
        }
        else {
            transplant(y, y->getRight());
            y->setRight(z->getRight());
            y->getRight()->setParent(y);
        }
        transplant(z, y);
        y->setLeft(z->getLeft());
        y->getLeft()->setParent(y);
        y->setColor(z->getColor());
    }
    deleteNodeFromNodeList(z);
    delete z;
    numberOfNodes--;
    if (y_original_color == BLACK) {
        deleteNodeFixup(x);
    }
}

/**
 * Metodo che effettua la creazione e l'inizializzazione di un nodo
 * @tparam T
 * @param key
 * @return
 */
template<class T>
RBNode<T> *RBTree<T>::createNewNode(T key) {
    RBNode<T> *z = new RBNode<T>;
    z->setLeft(nil);
    z->setRight(nil);
    z->setKey(key);
    return z;
}

template <class T>
void RBTree<T>::inorderVisit(RBNode<T> *x) {
    if (x != nil) {
        inorderVisit(x->getLeft());
        std::cout << x->getKey() << " ";
        inorderVisit(x->getRight());
    }
}

template<class T>
void RBTree<T>::preorderVisit(RBNode<T> *x) {
    if (x != nil) {
        preorderVisit(x->getLeft());
        std::cout << x->getKey() << " ";
        preorderVisit(x->getRight());
    }
}

template<class T>
void RBTree<T>::postorderVisit(RBNode<T> *x) {
    if (x != nil) {
        postorderVisit(x->getLeft());
        std::cout << x->getKey() << " ";
        postorderVisit(x->getRight());
    }
}

template <class T>
void RBTree<T>::inorderVisit() {
    inorderVisit(root);
}

template<class T>
void RBTree<T>::preorderVisit() {
    preorderVisit(root);
}

template<class T>
void RBTree<T>::postorderVisit() {
    postorderVisit(root);
}

/**
 * Massimo valore dell'albero red black
 * @tparam T
 * @param x
 * @return il nodo massimo
 */
template<class T>
RBNode<T> *RBTree<T>::treeMaximum(RBNode<T> *x) {
    while (x->getRight() != nil) {
        x = x->getRight();
    }
    return x;
}

/**
 * Minimo valore dell'albero red black
 * @tparam T
 * @param x
 * @return il nodo minimo
 */
template<class T>
RBNode<T> *RBTree<T>::treeMinimum(RBNode<T> *x) {
    while (x->getLeft() != nil) {
        x = x->getLeft();
    }
    return x;
}

/**
 * Effettua la ricerca di un nodo nell'albero red black
 * @tparam T
 * @param key
 * @return il nodo cercato se questo è presente, nil altrimenti
 */
template<class T>
RBNode<T> *RBTree<T>::searchNode(T key) {
    RBNode<T> *x = root;
    while (x != nil && x->getKey() != key) {
        if (key < x->getKey()) {
            x = x->getLeft();
        }
        else {
            x = x->getRight();
        }
    }
    return x;
}

/**
 * Effettua l'inserimento del nodo nell'albero red black
 * @tparam T
 * @param key
 */
template<class T>
void RBTree<T>::insertNode(T key) {
    RBNode<T> *z = createNewNode(key);
    RBNode<T> *y = nil;
    RBNode<T> *x = root;
    while (x != nil) {
        y = x;
        if (z->getKey() < x->getKey()) {
            x = x->getLeft();
        }
        else {
            x = x->getRight();
        }
    }
    z->setParent(y);
    if (y == nil) {
        root = z;
    }
    else if (z->getKey() < y->getKey()) {
        y->setLeft(z);
    }
    else {
        y->setRight(z);
    }
    z->setColor(RED);
    nodeList->push_back(z);
    numberOfNodes++;
    insertNodeFixup(z);
}

/**
 * Funzione che effettua l'eliminazione del nodo. Controlla
 * prima se il nodo esiste per poi effettuare l'eliminazione
 * @tparam T
 * @param key
 * @return
 */
template<class T>
bool RBTree<T>::deleteNode(T key) {
    RBNode<T> *z = searchNode(key);
    // Nodo non trovato
    if (z == nil) {
        std::cout << "\nIl nodo " << key << " non puo' essere eliminato perche' non e' presente nell'albero red black. Riprovare" << std::endl;
        return false;
    }
    else {
        deleteNodeHelper(z);
    }
    return true;
}

/**
 * Restituisce il numero di elementi di cui
 * è costituito l'albero red black
 * @tparam T
 * @return un intero che rappresenta il size dell'albero red black
 */
template<class T>
int RBTree<T>::size() {
    return numberOfNodes;
}

template<class T>
bool RBTree<T>::isEmpty() {
    if (root == nil) {
        return true;
    }
    else {
        return false;
    }
}

template<class T>
void RBTree<T>::deleteNodeFromNodeList(RBNode<T> *z) {
    for (int i = 0; i < nodeList->size(); i++) {
        RBNode<T> *node = nodeList->at(i);
        if (node->getKey() == z->getKey()) {
            nodeList->erase(nodeList->begin() + i);
            break;
        }
    }
}

/**
 * Restituisce una lista contenente tutti i nodi
 * che si trovano nell'albero red black
 * @tparam T
 * @return
 */
template<class T>
std::vector<RBNode<T> *> *RBTree<T>::getNodeList() {
    return nodeList;
}

/**
 *
 * @tparam T
 * @param key
 * @return true se il nodo si trova nell'albero
 * false altrimenti
 */
template<class T>
bool RBTree<T>::findNode(T key) {
    if (searchNode(key) == nil) {
        return false;
    }
    else {
        return true;
    }
}


#endif // RBTREE_H
