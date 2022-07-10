#ifndef RBGRAPH_H
#define RBGRAPH_H


#include <vector>
#include <queue>
#include "graphNode.h"

/**
 * La classe RBGraph incapsula la struttura dati grafo. Mantiene un vector di
 * graphNode che rappresentano i vertici del grafo e consente di effettuare
 * varie operazioni, tra le quali:
 * - l'aggiunta di archi al grafo
 * - la rimozione di archi
 * - la ricerca di archi
 * - lanciare la BFS sul grafo
 * - Stampare la lista di adiacenza di ogni nodo del grafo
 * - altre funzioni utili
 * @tparam T
 */
template <class T>
class RBGraph {
private:
    std::vector<graphNode<T> *> *vertices;
    int maxEdges;
    static int actualEdges;

protected:
    graphNode<T> *createNewGraphNode(T data);

public:
    RBGraph(int vertices, int edges);
    bool addEdge(T src, T dest);
    bool removeEdge(T src, T dest);
    bool findEdge(T src, T dest);
    void printGraph();
    void BFS(T src);
    // utilities
    int numOfVertices();
    int numOfEdges();
    bool isEmpty();
};

template<class T>
int RBGraph<T>::actualEdges = 0;

template<class T>
RBGraph<T>::RBGraph(int vertices, int edges) {
    this->vertices = new std::vector<graphNode<T> *>(vertices + 1);
    maxEdges = edges;
}

template<class T>
int RBGraph<T>::numOfVertices() {
    return vertices->size();
}

template<class T>
bool RBGraph<T>::isEmpty() {
    if (vertices->size() == 0) {
        return false;
    }
    else {
        return true;
    }
}

template<class T>
graphNode<T> *RBGraph<T>::createNewGraphNode(T data) {
    graphNode<T> *node;
    // Il nodo non può essere creato, eccederebbe il numero di vertici
    if (data >= vertices->size()) {
        return nullptr;
    }
        // Il nodo non è ancora stato creato
    else if (vertices->at(data) == nullptr) {
        node = new graphNode<T>;
        node->setData(data);
        // Si aggiunge il nodo all'insieme dei vertici
        vertices->at(data) = node;
    }
        // Il nodo è già stato creato
    else {
        node = vertices->at(data);
    }
    return node;
}

template<class T>
bool RBGraph<T>::addEdge(T src, T dest) {
    graphNode<T> *srcNode = createNewGraphNode(src);
    graphNode<T> *destNode = createNewGraphNode(dest);
    // Se uno dei due è nullptr, significa che non è stato possibile
    // creare l'arco perché uno dei due nodi non può essere contemplato
    // nel grafo (eccederebbe le dimensioni del grafo)
    if (srcNode == nullptr || destNode == nullptr) {
        std::cout << "\nNon e' stato possibile creare l'arco " << src << " " << dest << std::endl;
        std::cout << "Il numero di vertici del grafo e' " << numOfVertices() << " (da 0 a " << numOfVertices() - 1 << ")" << std::endl;
        return false;
    }
    // L'arco già è stato creato in quanto dest si trova nella lista di adiacenza di src
    if (srcNode->findNodeInAdjList(destNode)) {
        std::cout << "\nNon e' stato possibile creare l'arco " << src << " " << dest << std::endl;
        std::cout << "L'arco gia' esiste.' " << std::endl;
        return false;
    }
    srcNode->addEdge(destNode);
    actualEdges++;
    return true;
}

template<class T>
bool RBGraph<T>::removeEdge(T src, T dest) {
    if (findEdge(src, dest)) {
        graphNode<T> *srcNode = vertices->at(src);
        graphNode<T> *destNode = vertices->at(dest);
        srcNode->removeEdge(destNode);
        actualEdges--;
        return true;
    }
    else {
        std::cout << "\nImpossibile rimuovere arco " << src << " " << dest << ": non esiste." << std::endl;
        return false;
    }
}

template<class T>
bool RBGraph<T>::findEdge(T src, T dest) {
    // I nodi superano il numero di vertici massimo del grafo
    if (src >= vertices->size() || dest >= vertices->size()) {
        return false;
    }
    // I nodi non sono stati ancora caricati
    if (vertices->at(src) == nullptr || vertices->at(dest) == nullptr) {
        return false;
    }
    // Il nodo dest non si trova nella lista di adiacenza del nodo src
    if (!vertices->at(src)->findNodeInAdjList(vertices->at(dest))) {
        return false;
    }
    return true;
}

template<class T>
void RBGraph<T>::BFS(T s) {
    // Sorgente di dimensione maggiore del numero di vertici del grafo
    if (s >= this->numOfVertices()) {
        std::cout << "\nLa sorgente supera la dimensione del grafo" << std::endl;
        std::cout << "Dimensione del grafo: " << this->numOfVertices() << std::endl;
        return;
    }
    graphNode<T> *src = vertices->at(s);
    // Sorgente non ancora creata
    if (src == nullptr) {
        std::cout << "\nSorgente inesistente" << std::endl;
        return;
    }
    // Inizializzazione di tutti i vertici
    for (auto vertex : *vertices) {
        if (vertex != nullptr) {
            vertex->setColor(GraphColors::WHITE);
            vertex->setDistance(INT_INFINITY);
            vertex->setParentNode(nullptr);
        }
    }
    // Inizializzazione della sorgente
    src->setColor(GraphColors::GRAY);
    src->setDistance(0);
    src->setParentNode(nullptr);
    std::queue<graphNode<T> *> q;
    q.push(src);
    while (!q.empty()) {
        graphNode<T> *u = q.front();
        q.pop();
        std::cout << u->getData() << " ";
        RBTree<graphNode<T> *> *rbTree = u->getAdj();
        std::vector<RBNode<graphNode<T> *> *> *adjU = rbTree->getNodeList();
        for (auto v : *adjU) {
            if (v->getKey()->getColor() == GraphColors::WHITE) {
                v->getKey()->setColor(GraphColors::GRAY);
                v->getKey()->setDistance(u->getDistance() + 1);
                v->getKey()->setParentNode(u);
                q.push(v->getKey());
            }
        }
        u->setColor(GraphColors::BLACK);
    }
    std::cout << std::endl;
}

template<class T>
void RBGraph<T>::printGraph() {
    for (auto vertex : *vertices) {
        if (vertex != nullptr) {
            std::cout << vertex->getPrintableAdj();
        }
    }
}

template<class T>
int RBGraph<T>::numOfEdges() {
    return actualEdges;
}

#endif // RBGRAPH_H
