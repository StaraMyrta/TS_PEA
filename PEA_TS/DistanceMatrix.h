#pragma once
#include <string.h>
#include <string>
using namespace std;

class DistanceMatrix
{
public:
	int** tab;
	int size;
	string name;

	// Domy�lny konstruktor
	DistanceMatrix();

	// Konstruktor przyjmuj�cy liczb� wierzcho�k�w grafu
	DistanceMatrix(int nodes);

	// Ustawienie atrybut�w pod nowy graf
	void generate(int nodes);

	// Wy�wietlanie grafu w postaci macierzy s�siedztwa
	void show();

	// Dodanie kraw�dzi mi�dzy wierzcho�kami
	void addEdge(int x, int y, int dist);

	// Tworzenie grafu z losowymi warto�ciami kraw�dzi
	void createRandom(int testSize);

	// Wczytanie grafu z pliku txt
	void loadIt(string fileName);
};