#include "TS.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <math.h>
#include <Windows.h>
#include <random>

// Zamienia elementy o indeksach i oraz j w tablicy.
int* TS::Swap(int* series, int i, int j)
{
	int temp;
	temp = series[i];
	series[i] = series[j];
	series[j] = temp;

	return series;
}

// Wstawia elementy pomi�dzy indeksami i i j, przemieszczaj�c elementy w odpowiedni� stron�.
int* TS::Insert(int* series, int i, int j)
{
	if (i > j)
	{
		for (int a = i; a > j; a--)
			Swap(series, a, a - 1);
	}
	else if (i < j)
	{
		for (int a = i; a < j - 1; a++)
			Swap(series, a, a + 1);
	}
	return series;
}

// Odwraca podci�g element�w mi�dzy indeksami i oraz j w tablicy.
int* TS::Invert(int* series, int i, int j)
{
	int substring_length, b, c;
	if (i < j)
	{
		substring_length = j - (i - 1);
		b = i;
		c = j;

		for (int a = 0; a < floor(substring_length / 2); a++)
		{
			Swap(series, b, c);
			b++;
			c--;
		}
	}
	if (i > j)
	{
		substring_length = i - (j - 1);
		b = j;
		c = i;

		for (int a = 0; a < floor(substring_length / 2); a++)
		{
			Swap(series, b, c);
			b++;
			c--;
		}
	}
	return series;
}

// Oblicza d�ugo�� trasy na podstawie przekazanej tablicy indeks�w wierzcho�k�w.
int TS::RouteLength(int* route)
{
	int length = 0;

	for (int i = 0; i < wierzcholki; i++)
		length += matrix[route[i]][route[i + 1]];

	return length;
}

// Funkcja generuj�ca now� tras�
void TS::GenerateNewRoute()
{
	random_device rd;		// Tworzy niezale�ny generator liczb losowych
	mt19937 gen(rd());		// Silnik losowy zainicjowany warto�ci� z generatora rd
	uniform_int_distribution<> dist(1, wierzcholki - 1); // Rozk�ad wynik�w mi�dzy warto�ciami

	int random;
	bool* generated;		// Tablica przechowuj�ca informacje o wygenerowanych wierzcho�kach
	generated = new bool[wierzcholki];
	generated[0] = true;
	route[0] = 0;			// Pierwszy wierzcho�ek trasy
	route[wierzcholki] = 0;	// Ostatni wierzcho�ek trasy

	// Ustawienie wszystkich wierzcho�k�w poza pierwszym jako niewygenerowanych
	for (int i = 1; i < wierzcholki; i++)
		generated[i] = false;

	// Dla ka�dego wierzcho�ka poza pierwszym nast�puje wygenerowanie losowo nast�pnego wierzcho�ka. 
	// Je�li wierzcho�ek zosta� ju� wygenerowany losowanie nast�puje ponownie,
	// je�li nie wylosowany wierzcho�ek zostaje dodany do trasy i oznaczony jako wygenerowany.
	for (int i = 1; i < wierzcholki; i++)
	{
		random = dist(gen);

		while (generated[random])
			random = dist(gen);

		route[i] = random;
		generated[random] = true;
	}
}


// Funkcja przegl�du zach�annego
void TS::Greedy()
{
	boolean* visited = new boolean[wierzcholki];

	for (int i = 0; i < wierzcholki; i++)
		visited[i] = false;

	route[wierzcholki] = 0;				// Ostatni wierzcho�ek trasy
	int min = 0, next = 0, current = 0; // Zmienne przechowuj�ce informacje o minimalnym, nast�pnym i bie��cym wierzcho�ku
	for (int i = 0; i < wierzcholki; i++)
	{
		visited[current] = true;
		min = 0;

		for (int j = 0; j < wierzcholki; j++)
		{
			if (!visited[j] && (matrix[current][j] < min || min == 0)) // Je�li wierzcho�ek nie jest odwiedzony i odleg�o�� jest mniejsza od minimalnej lub minimalna to pierwsza warto��
			{
				min = matrix[current][j];		// Zaktualizuj minimaln� warto��
				next = j;						// Ustaw nast�pny wierzcho�ek
			}
		}
		route[i] = current;			// Dodaj bie��cy wierzcho�ek do trasy
		current = next;				// Ustaw bie��cy wierzcho�ek na nast�pny
	}

	// Wy�wietlenie wyniku dla algorytmu zach�annego
	cout << endl << endl << "Trasa (Greedy): " << route[0];
	for (int i = 1; i < wierzcholki + 1; i++)
		cout << "-> " << route[i];

	cout << endl << "Dlugosc(Greedy): " << RouteLength(route);

	cout << endl << endl;
}

// Metoda przeprowadzaj�ca ca�y Tabu Search
void TS::TabuSearch(DistanceMatrix distanceMatrix, bool diversification, double stop_cryterium)
{
	time_t timer1;
	time(&timer1);
	time_t  timer2;
	double second = 0;

	// Inicjalizacja zmiennych i kopii macierzy odleg�o�ci
	// Ustawienie pocz�tkowych warto�ci i alokacja pami�ci
	matrix = distanceMatrix.tab;
	wierzcholki = distanceMatrix.size;

	route = new int[wierzcholki + 1];

	// U�ycie heurystyki Greedy do utworzenia pocz�tkowej trasy
	Greedy();

	// Inicjalizacja zmiennych i struktur danych dla algorytmu
	int GlobalMin = RouteLength(route), MovedLength, temp = 0;
	int moved[2], counter = 0, divCounter = counter + 1000;
	TabuList = new int* [wierzcholki];
	int Cadency = (int)sqrt(wierzcholki);
	int* routeCopy, * GlobalMinRoute;
	routeCopy = new int[wierzcholki + 1];
	GlobalMinRoute = new int[wierzcholki + 1];
	boolean** TabuMove = new boolean * [wierzcholki];

	// Inicjalizacja i czyszczenie listy tabu
	for (int i = 0; i < wierzcholki; i++)
	{
		TabuList[i] = new int[wierzcholki];
		TabuMove[i] = new boolean[wierzcholki];
	}

	for (int i = 0; i < wierzcholki; i++)
	{
		for (int j = 0; j < wierzcholki; j++)
			TabuList[i][j] = 0;
	}

	// Rozpocz�cie g��wnej p�tli algorytmu
	while (second <= stop_cryterium)
	{
		// Przygotowanie do wyznaczenia ruch�w zakazanych i stworzenia kopii trasy
		temp = 0;
		for (int i = 0; i < wierzcholki; i++)
		{
			for (int j = 0; j < wierzcholki; j++)
				TabuMove[i][j] = false;
		}

		for (int i = 0; i < wierzcholki + 1; i++)
			routeCopy[i] = route[i];

		// Sprawdzenie wszystkich mo�liwych ruch�w i wyb�r najlepszego
		for (int i = 1; i < wierzcholki; i++)
		{
			for (int j = 1; j < wierzcholki; j++)
			{
				// Sprawdzenie warunk�w dla s�siad�w, czy s� ruchem dozwolonym w li�cie tabu
				if (i != j && (!TabuMove[i][j] && !TabuMove[j][i]))
				{
					// Tworzenie kopii aktualnej trasy
					for (int k = 0; k < wierzcholki + 1; k++)
						routeCopy[k] = route[k];

					// Wyb�r rodzaju s�siedztwa do sprawdzenia i obliczenie d�ugo�ci nowej trasy
					switch (sasiedztwo)
					{
					case 1:
						MovedLength = RouteLength(Swap(routeCopy, i, j));
						TabuMove[i][j] = true;
						break;

					case 2:
						MovedLength = RouteLength(Insert(routeCopy, i, j));
						TabuMove[i][j] = true;
						break;

					case 3:
						MovedLength = RouteLength(Invert(routeCopy, i, j));
						TabuMove[i][j] = true;
						break;

					default:
						MovedLength = RouteLength(Swap(routeCopy, i, j));
						TabuMove[i][j] = true;
						break;
					}

					// Aktualizacja najlepszego ruchu, je�li spe�nione s� warunki
					if (MovedLength < temp || temp == 0)
					{
						temp = MovedLength;
						moved[0] = i;
						moved[1] = j;
					}
				}
			}
		}

		// Sprawdzanie listy ruch�w zakazanych z uwzgl�dnieniem kryterium aspiracji
		if (diversification)
		{

			if ((TabuList[moved[0]][moved[1]] == 0 && TabuList[moved[1]][moved[0]] == 0) || temp < GlobalMin)
			{
				// Wykonanie ruchu zakazanego zgodnie z wybranym s�siedztwem
				switch (sasiedztwo)
				{
				case 1:
					Swap(route, moved[0], moved[1]);
					break;

				case 2:
					Insert(route, moved[0], moved[1]);
					break;

				case 3:
					Invert(route, moved[0], moved[1]);
					break;

				default:
					Swap(route, moved[0], moved[1]);
					break;
				}

				// Aktualizacja listy tabu oraz najlepszego rozwi�zania
				TabuList[moved[0]][moved[1]] += Cadency;
				TabuList[moved[1]][moved[0]] += Cadency;

				if (temp < GlobalMin)
				{
					GlobalMin = temp;

					// Zapisanie najlepszej trasy
					for (int i = 0; i < wierzcholki + 1; i++)
						GlobalMinRoute[i] = route[i];
				}
			}

			// Sprawdzenie warunku dywersyfikacji - generowanie losowej trasy
			if (counter == divCounter)
			{
				if (GlobalMin <= temp)
				{
					GenerateNewRoute();
				}

				divCounter += 1000;

			}
		}

		// Sprawdzenie warunk�w, gdy nie ma dywersyfikacji
		else if (!diversification)
		{
			// Sprawdzenie kryterium aspiracji lub mo�liwo�ci wykonania ruchu zakazanego
			if ((TabuList[moved[0]][moved[1]] == 0 && TabuList[moved[1]][moved[0]] == 0) || temp < GlobalMin)
			{
				// Wykonanie ruchu zakazanego w zale�no�ci od wybranego s�siedztwa
				switch (sasiedztwo)
				{
				case 1:
					Swap(route, moved[0], moved[1]);
					break;

				case 2:
					Insert(route, moved[0], moved[1]);
					break;

				case 3:
					Invert(route, moved[0], moved[1]);
					break;

				default:
					Swap(route, moved[0], moved[1]);
					break;
				}

				// Zmniejszenie warto�ci listy tabu dla wszystkich ruch�w
				for (int i = 0; i < wierzcholki; i++)
				{
					for (int j = 0; j < wierzcholki; j++)
					{
						if (TabuList[i][j] != 0)
							TabuList[i][j] -= 1;
					}
				}
				// Dodanie ruchu do listy tabu
				TabuList[moved[0]][moved[1]] += Cadency;

				// Aktualizacja najlepszego rozwi�zania
				if (temp < GlobalMin)
				{
					GlobalMin = temp;

					// Zapisanie najlepszej trasy
					for (int i = 0; i < wierzcholki + 1; i++)
						GlobalMinRoute[i] = route[i];
				}
			}
		}

		// Aktualizacja listy tabu, licznika iteracji i pomiaru czasu
		for (int i = 0; i < wierzcholki; i++)
		{
			for (int j = 0; j < wierzcholki; j++)
			{
				if (TabuList[i][j] != 0)
					TabuList[i][j] = TabuList[i][j] - 1;
			}
		}

		counter++;

		time(&timer2);
		second = difftime(timer2, timer1);

	}

	// Wy�wietlenie informacji o trybie pracy algorytmu oraz najlepszym rozwi�zaniu
	if (diversification)
		cout << endl << endl << "Algorytm wykonano z dywersyfikacja," << endl;
	else 
		cout << endl << endl << "Algorytm wykonano bez dywersyfikacji," << endl;
	switch (sasiedztwo)
	{
	case 1:
		cout << "wykorzystujac sasiedztwo typu swap" << endl;
		break;

	case 2:
		cout << "wykorzystujac sasiedztwo typu insert" << endl;
		break;

	case 3:
		cout << "wykorzystujac sasiedztwo typu invert" << endl;
		break;

	default:
		cout << "wykorzystujac sasiedztwo typu swap" << endl;
		break;
	}
	cout << "oraz z kryterium stopu " << stop_cryterium << "s" << endl;
	cout << endl << endl << "Najlepsza ze wszystkich iteracji trasa (Tabu Search): " << GlobalMinRoute[0];
	for (int i = 1; i < wierzcholki + 1; i++)
		cout << "-> " << GlobalMinRoute[i];

	cout << endl << "Jej dlugosc wynosi(Tabu Search): " << GlobalMin << endl;
	cout << "Ilosc wykonanych iteracji: " << counter << endl << endl;
}

void TS::TabuSearchTest(int size)
{
	DistanceMatrix matrix;
	matrix.createRandom(size);
	int stop_cryterium = size / 10;
	for (int i = 0; i < 100; i++)
	{
		matrix.createRandom(size);
		TabuSearch(matrix, true, stop_cryterium * 3);
		TabuSearch(matrix, true, stop_cryterium * 6);
		TabuSearch(matrix, true, stop_cryterium * 9);
		TabuSearch(matrix, true, stop_cryterium * 12);
		TabuSearch(matrix, true, stop_cryterium * 15);
		cout << endl;
		
	}
}