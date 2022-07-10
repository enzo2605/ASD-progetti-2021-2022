#ifndef GRAPH_H
#define GRAPH_H


#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include "Vertex.h"
#include "Edge.h"
#include "DisjointSet.h"

const int MAX_NODES = 1000;
const int MAX_EDGES = 10000;
const int MAX_COST = 10000;

/**
 * La classe Graph incapsula la struttura dati grafo. Mantiene un vector di
 * Vertex che rappresentano i vertici del grafo e consente di effettuare
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
class Graph {
    private:
        std::vector<Vertex<T> *> *vertices;
        std::vector<Edge<T>> edges;
        int maxEdges;
        static int actualEdges;

        Vertex<T> *createNewGraphNode(T data);
        void addEdgeToTheList(T cost, Vertex<T> *src, Vertex<T> *dest);
        void removeEdgeToTheList(Vertex<T> *src, Vertex<T> *dest);

    public:
        Graph(int vertices, int edges);
        bool addEdge(T src, T dest, T cost);
        bool removeEdge(T src, T dest);
        bool findEdge(T src, T dest);
        void printGraph();
        void BFS(T src);
        std::vector<Edge<T>> MSTKruskal(T &cost);
        void findFirstAndSecondMST();
        void printMST(std::vector<Edge<T>> MST, T mstCost);
        // utilities
        int numOfVertices();
        int numOfEdges();
        bool isEmpty();
};

template<class T>
int Graph<T>::actualEdges = 0;

template<class T>
Graph<T>::Graph(int vertices, int edges) {
    this->vertices = new std::vector<Vertex<T> *>(vertices + 1);
    maxEdges = edges;
}

template<class T>
int Graph<T>::numOfVertices() {
    return vertices->size();
}

template<class T>
bool Graph<T>::isEmpty() {
    if (vertices->size() == 0) {
        return true;
    }
    else {
        return false;
    }
}

template<class T>
Vertex<T> *Graph<T>::createNewGraphNode(T data) {
    Vertex<T> *node;
    // Il nodo non può essere creato, eccederebbe il numero di vertici
    if (data > this->numOfVertices()) { // modificato
        return nullptr;
    }
    // Il nodo non è ancora stato creato
    else if (vertices->at(data) == nullptr) {
        node = new Vertex<T>;
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
bool Graph<T>::addEdge(T src, T dest, T cost) {
    Vertex<T> *srcNode = createNewGraphNode(src);
    Vertex<T> *destNode = createNewGraphNode(dest);
    // Se uno dei due è nullptr, significa che non è stato possibile
    // creare l'arco perché uno dei due nodi non può essere contemplato
    // nel grafo (eccederebbe le dimensioni del grafo)
    if (srcNode == nullptr || destNode == nullptr) {
        std::cout << "\nNon e' stato possibile creare l'arco " << src << " " << dest << std::endl;
        std::cout << "Il numero di vertici del grafo e' " << this->numOfVertices() << std::endl;
        return false;
    }
    // L'arco già è stato creato in quanto dest si trova nella lista di adiacenza di src
    if (srcNode->findNodeInAdjList(destNode)) {
        std::cout << "\nNon e' stato possibile creare l'arco " << src << " " << dest << std::endl;
        std::cout << "L'arco gia' esiste.' " << std::endl;
        return false;
    }
    // Controlla se il costo dell'arco supera il costo massimo
    if (cost < 1 || cost > MAX_COST) {
		std::cout << "\nNon e' stato possibile creare l'arco " << src << " " << dest << std::endl;
        std::cout << "Il suo costo (" << cost << ") non rientra nell'intervallo consentito." << std::endl;
        std::cout << "Intervallo di costo consentito: [1-" << MAX_COST << "]" << std::endl;
        return false;
    }
    srcNode->addEdge(destNode);
    destNode->addEdge(srcNode);
    addEdgeToTheList(cost, srcNode, destNode);
    actualEdges++;
    return true;
}

template<class T>
bool Graph<T>::removeEdge(T src, T dest) {
    if (findEdge(src, dest)) {
        Vertex<T> *srcNode = vertices->at(src);
        Vertex<T> *destNode = vertices->at(dest);
        srcNode->removeEdge(destNode);
        destNode->removeEdge(srcNode);
        removeEdgeToTheList(srcNode, destNode);
        std::cout << "number of edges1: " << edges.size() << std::endl;
        actualEdges--;
        return true;
    }
    else {
        std::cout << "\nImpossibile rimuovere arco " << src << " " << dest << ": non esiste." << std::endl;
        return false;
    }
}

template<class T>
bool Graph<T>::findEdge(T src, T dest) {
    // I nodi superano il numero di vertici massimo del grafo
    if (src > vertices->size() || dest > vertices->size()) {
        return false;
    }
    // I nodi non sono stati ancora caricati
    if (vertices->at(src) == nullptr || vertices->at(dest) == nullptr) {
        return false;
    }
    // Il nodo dest non si trova nella lista di adiacenza del nodo src
    if ((!vertices->at(src)->findNodeInAdjList(vertices->at(dest))) || (!vertices->at(dest)->findNodeInAdjList(vertices->at(src)))) {
        return false;
    }
    return true;
}

template<class T>
void Graph<T>::BFS(T s) {
    // Sorgente di dimensione maggiore del numero di vertici del grafo
    if (s >= this->numOfVertices()) {
        std::cout << "\nLa sorgente supera la dimensione del grafo" << std::endl;
        std::cout << "Dimensione del grafo: " << this->numOfVertices() << std::endl;
        return;
    }
    Vertex<T> *src = vertices->at(s);
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
    std::queue<Vertex<T> *> q;
    q.push(src);
    while (!q.empty()) {
        Vertex<T> *u = q.front();
        q.pop();
        std::cout << u->getData() << " ";
        std::list<Vertex<T> *> *adjU = u->getAdj();
        for (auto v : *adjU) {
            if (v->getColor() == GraphColors::WHITE) {
                v->setColor(GraphColors::GRAY);
                v->setDistance(u->getDistance() + 1);
                v->setParentNode(u);
                q.push(v);
            }
        }
        u->setColor(GraphColors::BLACK);
    }
    std::cout << std::endl;
}

template<class T>
void Graph<T>::printGraph() {
    for (auto vertex : *vertices) {
        if (vertex != nullptr) {
            std::cout << vertex->getPrintableAdj();
        }
    }
}

template<class T>
int Graph<T>::numOfEdges() {
    return actualEdges;
}

template<class T>
std::vector<Edge<T>> Graph<T>::MSTKruskal(T &cost) {
    DisjointSet<Vertex<T> *> disjointSet;
    std::vector<Edge<T>> MST;
    cost = 0;
    for (auto vertex : *vertices) {
        disjointSet.makeSet(vertex);
    }
    std::stable_sort(edges.begin(), edges.end());
    for (auto edge : edges) {
        if (disjointSet.findSet(edge.getSrc()) != disjointSet.findSet(edge.getDest())) {
            MST.push_back(edge);
            cost += edge.getCost();
            disjointSet.Union(edge.getSrc(), edge.getDest());
        }
    }
    return MST;
}

template<class T>
void Graph<T>::printMST(std::vector<Edge<T>> MST, T mstCost) {
    std::cout << "\ncost = " << mstCost << std::endl;
    for (auto edge : MST) {
        std::cout << edge.getSrc()->getData() << "-" << edge.getDest()->getData() << "->";
        std::cout << edge.getCost() << "   ";
    }
    std::cout << std::endl;
}

template<class T>
void Graph<T>::findFirstAndSecondMST() {
    std::vector<Edge<T>> MST, secondMST, MST_temp;
    T firstMSTcost = 0, secondMSTCost = INT_INFINITY, tempMSTcost = 0;
    MST = MSTKruskal(firstMSTcost);
    printMST(MST, firstMSTcost);
    for (auto edge : MST) {
        auto temp = edge;
        edges.erase(std::remove(edges.begin(), edges.end(), edge), edges.end());
        MST_temp = MSTKruskal(tempMSTcost);
        if (tempMSTcost < secondMSTCost) {
            secondMSTCost = tempMSTcost;
            secondMST = MST_temp;
        }
        edges.push_back(temp);
    }
    printMST(secondMST, secondMSTCost);
}

template<class T>
void Graph<T>::removeEdgeToTheList(Vertex<T> *src, Vertex<T> *dest) {
    for (auto edge : edges) {
        if ((edge.getSrc()->getData() == src->getData() && edge.getDest()->getData() == dest->getData()) ||
                (edge.getSrc()->getData() == dest->getData() && edge.getDest()->getData() == src->getData())) {
            edges.erase(std::remove(edges.begin(), edges.end(), edge), edges.end());
            return;
        }
    }
}

template<class T>
void Graph<T>::addEdgeToTheList(T cost, Vertex<T> *src, Vertex<T> *dest) {
    Edge<T> edge(cost, src, dest);
    edges.push_back(edge);
}

#endif // GRAPH_H
