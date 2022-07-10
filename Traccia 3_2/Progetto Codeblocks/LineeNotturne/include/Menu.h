#ifndef MENU_H
#define MENU_H


#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include "Graph.h"

/**
 * La classe Menu offre le operazioni necessarie per caricare il grafo e
 * gestire le possibili operazioni su di esso. Per fare ciò, tale classe
 * è dotata di un'istanza di Graph.
 */
class Menu {
    private:
        Graph<int> *graph;

        bool saveInGraph(std::ifstream &inFile);
        bool convertIntoPair(std::vector<int> &pair, std::string edge);
        bool loadGraph(std::string file);

    public:
        Menu() { graph = nullptr; };
        void start(std::string file_name);
};

/**
 * Salviamo i due valori interi che rappresentano gli archi in un vector
 * pair, che è un paremetro di output. Per farlo si divide la stringa
 * in base al delimitatore, che nel nostro caso sarà lo spazio. Ogni valore
 * ottenuto viene salvato nel vector pair. Se la dimensione del vector supera
 * 2, significa che i dati nel file non sono riportati in un formato corretto.
 * @param pair
 * @param edge
 * @return vector pair, contenente due valori che rappresentano rispettivamente
 * nodo sorgente e nodo destinazione
 */
bool Menu::convertIntoPair(std::vector<int> &pair, std::string edge) {
    std::string spaceDelimiter = " ";
    std::size_t pos = 0;
    int value;
    while ((pos = edge.find(spaceDelimiter)) != std::string::npos) {
        value = std::stoi(edge.substr(0, pos), nullptr, 10); // conversione da stringa a intero
        edge.erase(0, pos + spaceDelimiter.length());
        pair.push_back(value); // inserimento valore nell'array
        // Se ci sono più di tre valori separati dallo spazio
        if (pair.size() >= 3) {
            return false;
        }
    }
    // Se c'è solo un valore oppure si è usato un delimitatore diverso dallo spazio
    if (!pair.size()) {
        return false;
    }
    // Se si giunge a questo punto, significa che i dati sono riportati in formato corretto
    value = std::stoi(edge, nullptr, 10); // conversione da stringa a intero
    pair.push_back(value); // inserimento valore nell'array
    return true;
}

/**
 * Funzione helper che salva i dati nel grafo, occupandosi della loro estrazione e della
 * verifica della loro correttezza
 * @param inFile
 * @return true se il caricamento va a buon fine, false altrimenti
 */
bool Menu::saveInGraph(std::ifstream &inFile) {
    std::string temp;
    int n = 0, m = 0;
    int row = 1; // contatore righe del file, in modo da localizzare dove avviene l'errore
    // Lettura dei primi due valori, rispettivamente N e M
    if (std::getline(inFile, temp)) {
        std::vector<int> edge;
        if (!convertIntoPair(edge, temp)) {
            std::cout << "\nI dati nel file alla riga " << row;
            std::cout << " non sono riportati nel formato corretto." << std::endl;
            return false;
        }
        n = edge.at(0);
        m = edge.at(1);
    }
    // Controllo sui valori assunti da N e M
    if ((n < 1 || n > MAX_NODES) || (m < 0 || m > MAX_EDGES)) {
        std::cout << "\nI dati alla riga 1 non sono corretti. I valori iniziali devono essere 0 <= N <= " << MAX_NODES << " e 0 <= M <= " << MAX_EDGES << std::endl;
        std::cout << "Valori inseriti: " << n << " " << m << std::endl;
        return false;
    }
    // Istanziamo il grafo, con i valori di N e M
    graph = new Graph<int>(n, m);
    // Lettura dati dal file
    while (std::getline(inFile, temp)) {
        std::vector<int> edge;
        row++;
        // Estrazione dati dal file
        if (!convertIntoPair(edge, temp)) {
            std::cout << "\nI dati nel file alla riga " << row;
            std::cout << " non sono riportati nel formato corretto." << std::endl;
            return false;
        }
        // Aggiunta arco al grafo
        bool res = graph->addEdge(edge.at(0), edge.at(1), edge.at(2));
        if (!res) {
            std::cout << "Errore alla riga " << row << std::endl;
            return false;
        }
    }
    return true;
}

/**
 * Effettua il caricamento dei dati del file (se corretti) nel grafo
 * @param file
 * @return true se il caricamento va a buon fine, false altrimenti
 */
bool Menu::loadGraph(std::string file) {
    std::ifstream inFile;
    // Apertura file
    inFile.open(file, std::ios::in);
    if (inFile.fail()) {
        std::cout << "\nErrore apertura file. Il file potrebbe essere inesistente o trovarsi in una cartella" << std::endl;
        std::cout << "non visibile dal programma.\n" << std::endl;
        return false;
    }
    else {
        if (!saveInGraph(inFile)) {
            return false;
        }
    }
    inFile.close();
    return true;
}

/**
 * Funzione principale, la quale richiama le operazioni di caricamento del
 * grafo nell'apposita struttura dati e la funzione responsabile della
 * gestione del menù
 * @param file_name
 */
void Menu::start(std::string file_name) {
    if (loadGraph(file_name)) {
        std::cout << "File " << file_name << " caricato con successo." << std::endl;
        graph->findFirstAndSecondMST();
    }
    else {
        std::cout << "Non e' stato possibile caricare il file o parti di esso." << std::endl;
    }
}

#endif // MENU_H
