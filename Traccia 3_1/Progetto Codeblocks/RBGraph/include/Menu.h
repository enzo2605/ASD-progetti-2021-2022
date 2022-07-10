#ifndef MENU_H
#define MENU_H


#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include "RBGraph.h"

/**
 * La classe Menu offre le operazioni necessarie per caricare il grafo e
 * gestire le possibili operazioni su di esso. Per fare ciò, tale classe
 * è dotata di un'istanza di RBGraph.
 */
class Menu {
    private:
        RBGraph<int> *graph;

        bool saveInGraph(std::ifstream &inFile);
        bool convertIntoPair(std::vector<int> &pair, std::string edge);
        void printMainMenu();
        bool loadGraph(std::string file);
        void startMenu();

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
        // Se ci sono più di due valori separati dallo spazio
        if (pair.size() >= 2) {
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
    if ((n < 0 || n > 1000) || (m < 0 || m > 1000)) {
        std::cout << "\nI dati alla riga 1 non sono corretti. I valori iniziali devono essere 0 <= N <= 1000 e 0 <= M <= 1000.\n";
        return false;
    }
    // Istanziamo il grafo, con i valori di N e M
    graph = new RBGraph<int>(n, m);
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
        bool res = graph->addEdge(edge.at(0), edge.at(1));
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
        std::cout << "\nErrore apertura file. Il file " << file << " potrebbe essere inesistente o trovarsi in una cartella" << std::endl;
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
 * Banale procedura il cui scopo è stampare a video il menù
 */
void Menu::printMainMenu() {
    std::cout << "\nMain menu" << std::endl;
    std::cout << "0. Uscita dal programma" << std::endl;
    std::cout << "1. AddEdge(i, j)" << std::endl;
    std::cout << "2. RemoveEdge(i, j)" << std::endl;
    std::cout << "3. FindEdge(i, j)" << std::endl;
    std::cout << "4. BFS(s)" << std::endl;
}

/**
 * Si occupa della gestione delle operazioni svolte dall'utente attraverso il menù
 */
void Menu::startMenu() {
    int choice = 0;
    int src, dest;
    do {
        printMainMenu();
        std::cout << "Scelta: ";
        std::cin >> choice;

        if (choice >= 1 && choice <= 3) {
            std::cout << "\nScelta " << choice << std::endl;
            std::cout << "Nodo sorgente: ";
            std::cin >> src;
            std::cout << "Nodo destinazione: ";
            std::cin >> dest;
            if (choice == 1) {
                if (graph->addEdge(src, dest)) {
                    graph->printGraph();
                    std::cout << "Arco " << src << " " << dest << " aggiunto con successo" << std::endl;
                }
            }
            else if (choice == 2) {
                if (graph->removeEdge(src, dest)) {
                    graph->printGraph();
                    std::cout << "Arco " << src << " " << dest << " rimosso con successo" << std::endl;
                }
            }
            else if (choice == 3) {
                if (graph->findEdge(src, dest)) {
                    std::cout << "L'arco " << src << " " << dest << " e' presente nel grafo" << std::endl;
                }
                else {
                    std::cout << "L'arco " << src << " " << dest << " NON e' presente nel grafo" << std::endl;
                }
            }
        }
        else if (choice == 4) {
            std::cout << "\nNodo sorgente per la BFS: ";
            std::cin >> src;
            std::cout << "BFS: ";
            graph->BFS(src);
        }
        else {
            if (choice != 0) {
				std::cout << "\nNon esiste alcuna voce nel menu relativa alla scelta " << choice << ": riprovare" << std::endl;
            }
        }
    } while (choice != 0);
    std::cout << "Arrivederci!" << std::endl;
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
        startMenu();
    }
    else {
        std::cout << "Non e' stato possibile caricare il file o parti di esso." << std::endl;
    }
}

#endif // MENU_H
