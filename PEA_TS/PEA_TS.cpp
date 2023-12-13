#include <iostream>
#include <stdio.h>
#include "DistanceMatrix.h"
#include "TS.h"


using namespace std;
string fileName;

// Funkcja wyświetlająca menu opcji
void showMenu(void) {
    cout << "\n\n----PROBLEM KOMIWOJAZERA----\n";
    cout << "   Wybierz odpowiednia opcje:\n";
    cout << "1. Wczytaj graf z pliku\n";
    cout << "2. Wyswietl macierz sasiedztwa\n";
    cout << "3. Przeprowadz algorytm Tabu Search\n";
    cout << "4. Przeprowadz testy Tabu Search\n";
    cout << "5. Wprowadz kryterium stopu\n";
    cout << "6. Wlacz / Wylacz dywersyfikacje\n";
    cout << "7. Wybierz sasiedztwo\n";
    cout << "8. Stworz graf\n";
    cout << "0. Zamknij program\n";
}

// Główna funkcja programu
int main(void)
{
    int sasiedztwo = 1;// Zmienna przechowująca wybór sasiedztwa
    char div; // Zmienna przechowująca wybór dywersyfikacji
    int size; // Zmienna przechowująca rozmiar
    bool diversification = true; // Flaga dywersyfikacji ustawiona na true
    double stop_cryterium = 20; // Kryterium stopu dla algorytmu w sekundach

    DistanceMatrix matrix;// Obiekt macierzy odległości
    TS ts; // Obiekt algorytmu Tabu Search

    while (true) {
        showMenu();
        int wybor;
        cin >> wybor;
        switch (wybor) {
        case 1:
            // Wczytanie pliku z macierzą
            cout << "Podaj sciezke do pliku\n";
            cin >> fileName;
            matrix.loadIt(fileName);
            cout << "Wczytano pomyslnie !\n";
            break;
        case 2:
            // Wyświetlenie macierzy sąsiedztwa
            matrix.show();
            break;
        case 3:
            // Uruchomienie algorytmu Tabu Search z odpowiednimi parametrami
            ts.sasiedztwo = sasiedztwo;
            ts.TabuSearch(matrix, diversification, stop_cryterium);
            break;
        case 4:
            //cout << endl << "35 wierzcholkow:";
            //ts.TabuSearchTest(35);
            //cout << endl << "45 wierzcholkow:";
            //ts.TabuSearchTest(45);
            //cout << endl << "55 wierzcholkow:";
            //ts.TabuSearchTest(55);
            break;
        case 5:
            // Ustawienie kryterium stopu
            cout << endl << "Podaj maksymalny czas (w sekundach) szukania rozwiazania: ";
            cin >> stop_cryterium;
            break;
        case 6:
            // Wybór dywersyfikacji
            cout << endl << "Dywersyfikacja?(wybierz t/n): ";
            cin >> div;
            if (div == 'n')
                diversification = false;
            else if (div == 't')
                diversification = true;
            break;
        case 7:
            // Wybór typu sąsiedztwa
            cout << endl << "Sasiedztwo (1.Swap, 2.Insert, 3.Invert): ";
            cin >> sasiedztwo;
            break;
        case 8:
            // Tworzenie losowej macierzy
            cout << "Podaj ilosc wierzcholkow" << endl;
            cin >> size;
            matrix.createRandom(size);
            matrix.show();
            break;
        case 0:
            // Zamknięcie programu
            exit(0);
        }
    }
    return 0;
}