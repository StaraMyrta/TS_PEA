#pragma once
#include "DistanceMatrix.h"
using namespace std;
class TS
{
public:
	int wierzcholki, sasiedztwo;
	int** matrix;	
	int* route, ** TabuList;

	// Zamienia elementy o indeksach i oraz j w tablicy.
	int* Swap(int* series, int i, int j);

	// Wstawia elementy pomi�dzy indeksami i i j, przemieszczaj�c elementy w odpowiedni� stron�.
	int* Insert(int* series, int i, int j);

	// Odwraca podci�g element�w mi�dzy indeksami i oraz j w tablicy.
	int* Invert(int* series, int i, int j); 

	// Oblicza d�ugo�� trasy na podstawie przekazanej tablicy indeks�w wierzcho�k�w.
	int RouteLength(int* route);

	// Funkcja generuj�ca now� losow� tras�
	void GenerateNewRoute();

	// Funkcja przegl�du zach�annego
	void Greedy();

	// Metoda przeprowadzaj�ca ca�y Tabu Search
	void TabuSearch(DistanceMatrix distanceMatrix, bool diversification, double stop_cryterium);

	// Metoda przeprowadzaj�ca testy dla 100 instancji o zadanym rozmiarze
	void TabuSearchTest(int size);

};
