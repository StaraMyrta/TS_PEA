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

	// Wstawia elementy pomiêdzy indeksami i i j, przemieszczaj¹c elementy w odpowiedni¹ stronê.
	int* Insert(int* series, int i, int j);

	// Odwraca podci¹g elementów miêdzy indeksami i oraz j w tablicy.
	int* Invert(int* series, int i, int j); 

	// Oblicza d³ugoœæ trasy na podstawie przekazanej tablicy indeksów wierzcho³ków.
	int RouteLength(int* route);

	// Funkcja generuj¹ca now¹ losow¹ trasê
	void GenerateNewRoute();

	// Funkcja przegl¹du zach³annego
	void Greedy();

	// Metoda przeprowadzaj¹ca ca³y Tabu Search
	void TabuSearch(DistanceMatrix distanceMatrix, bool diversification, double stop_cryterium);

	// Metoda przeprowadzaj¹ca testy dla 100 instancji o zadanym rozmiarze
	void TabuSearchTest(int size);

};
