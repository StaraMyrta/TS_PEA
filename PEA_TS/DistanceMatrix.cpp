#include "DistanceMatrix.h"
#include "Time.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;


// Sta�a oznaczaj�ca brak kraw�dzi
const int maxValue = INT_MAX;

// Struktura w�z�a
struct Node {
    int node_nr;
    int node_bound;
};

// Domy�lny konstruktor
DistanceMatrix::DistanceMatrix() {}

// Konstruktor przyjmuj�cy liczb� wierzcho�k�w grafu
DistanceMatrix::DistanceMatrix(int nodes) {
    size = nodes;
    generate(nodes);
}

// Ustawienie atrybut�w pod nowy graf
void DistanceMatrix::generate(int nodes) {
    tab = new int* [nodes];
    for (int i = 0; i < nodes; i++)
        tab[i] = new int[nodes];
    for (int i = 0; i < nodes; i++)
        for (int j = 0; j < nodes; j++)
            tab[i][j] = maxValue;
}

// Wy�wietlanie grafu w postaci macierzy s�siedztwa
void DistanceMatrix::show() {
    cout << "Reprezentacja macierzowa grafu:\n     ";
    for (int i = 0; i < size; i++)
        printf("%4d ", i);
    cout << endl << endl;
    for (int i = 0; i < size; i++) {
        printf("%3d  ", i);
        for (int j = 0; j < size; j++) {
            printf("%4d ", tab[i][j]);

        }
        cout << endl;
    }
}

// Dodanie kraw�dzi mi�dzy wierzcho�kami
void DistanceMatrix::addEdge(int x, int y, int dist) {
    tab[x][y] = dist;
}

// Tworzenie grafu z losowymi warto�ciami kraw�dzi
void DistanceMatrix::createRandom(int testSize) {
    size = testSize;
    generate(size);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            int d;
            if (i == j) d = -1;
            else d = rand() % 50 + 1; //kraw�d� przyjmuje losow� warto�� od 1 do 50.
            addEdge(i, j, d);
        }
    }
}


// Wczytanie grafu z pliku txt
void DistanceMatrix::loadIt(string fileName) {
    fstream f;
    f.open(fileName, ios::in);
    if (!f.good()) {
        cout << "nie mozna otworzyc pliku!" << endl;
        exit(1);
    }

    f >> size;

    generate(size);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            int d;
            f >> d;
            addEdge(i, j, d);
        }
    }
    f.close();
}


