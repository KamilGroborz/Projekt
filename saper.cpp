#include <iostream>
#include <random>
#include <ctime>
#include <windows.h>
#include <conio.h>

using namespace std;

// definicja przycisków potrzebnych do sterowania
#define STRZALKA_GORA 72
#define STRZALKA_LEWO  75
#define STRZALKA_PRAWO 77
#define STRZALKA_DOL  80
#define SPACJA 32
#define FLAGA 102

// losowanie bomb po otrzymaniu pierwszego bezpiecznego pola oraz rozmiarów planszy
void losowanie(int** plansza, int x, int y, int rozmiar_x, int rozmiar_y, int liczba_min) {
    mt19937 generator(time(nullptr));
    uniform_int_distribution<int> losowanie_y(0, rozmiar_y-1);
    uniform_int_distribution<int> losowanie_x(0, rozmiar_x-1);
    int i = 0, k = 0;
    i = losowanie_x(generator);
    k = losowanie_y(generator);
    while (liczba_min != 0) {
// pętla losuje pole póki nie trafi na takie bez bomby
        do  {
            i = losowanie_x(generator);
            k = losowanie_y(generator);
        } while (plansza[i][k] == 9);
// warunki zapewniają aby pola dookoła pierwszego pola były bezpieczne
         if (i != x || k!=y) {
             if (i != x-1 || k != y-1) {
                 if (i != x || k != y-1) {
                     if (i != x+1 || k != y-1) {
                         if (i != x-1 || k != y) {
                             if (i != x+1 || k != y) {
                                 if (i != x-1 || k != y+1) {
                                     if (i != x || k != y+1) {
                                         if (i != x+1 || k != y+1) {
                                             plansza[i][k] = 9;
                                             liczba_min--;
                                         }
                                     }
                                 }
                             }
                         }
                     }
                 }
             }
        }
    }
}

// wypelnia odpowiednimi numerami pola dookola bomb, podobny algorytm do tego wykorzystanego do statkow podczas laboratoriow
void wypelnienie(int **plansza, int rozmiar_x, int rozmiar_y) {
    for (int i = 0; i < rozmiar_x; i++) {
        for (int k = 0; k < rozmiar_y; k++) {
            if (plansza[i][k] == 9) {
                if (i != 0 && plansza[i - 1][k] != 9) plansza[i - 1][k] += 1;
                if (i != 0 && plansza[i - 1][k - 1] != 9 && k != 0) plansza[i - 1][k - 1] += 1;
                if (i != 0 && plansza[i - 1][k + 1] != 9 && k != rozmiar_y - 1) plansza[i - 1][k + 1] += 1;
                if (plansza[i][k - 1] != 9 && k != 0) plansza[i][k - 1] += 1;
                if (plansza[i][k + 1] != 9 && k != rozmiar_y - 1) plansza[i][k + 1] += 1;
                if (i != rozmiar_x-1 && plansza[i + 1][k] != 9) plansza[i + 1][k] += 1;
                if (i != rozmiar_x-1 && plansza[i + 1][k - 1] != 9 && k != 0) plansza[i + 1][k - 1] += 1;
                if (i != rozmiar_x-1 && plansza[i + 1][k + 1] != 9 && k != rozmiar_y-1) plansza[i + 1][k + 1] += 1;
            }
        }
    }
}

// funkcja otrzymuje wspolzedne pola ktore uzytkownik chce odkryci i zmienia jego wartosc na przeciwna,
// jesli trafi na pole dookola ktorego nie ma zadnej bomby wywola ponownie funkcje dla wszystkich otaczajacych pol
void odslanianie(int x, int y, int **plansza, int rozmiar_x, int rozmiar_y) {
    if (x < 0 || x>rozmiar_x) return; 
    if (y < 0 || y>rozmiar_y) return;
    if (plansza[x][y] > 10) return;
    if (plansza[x][y] > 0 && plansza[x][y] != 9) plansza[x][y] = -plansza[x][y];
    else if (plansza[x][y] == 0) {
        plansza[x][y] = -10;
        if (x != 0) odslanianie(x - 1, y - 1, plansza, rozmiar_x, rozmiar_y);
        if (x != 0)odslanianie(x - 1, y, plansza, rozmiar_x, rozmiar_y);
        if (x != 0)odslanianie(x - 1, y + 1, plansza, rozmiar_x, rozmiar_y);
        odslanianie(x, y + 1, plansza, rozmiar_x, rozmiar_y);
        odslanianie(x, y - 1, plansza, rozmiar_x, rozmiar_y);
        if (x != rozmiar_x-1) odslanianie(x + 1, y - 1, plansza, rozmiar_x, rozmiar_y);
        if (x != rozmiar_x-1)odslanianie(x + 1, y + 1, plansza, rozmiar_x, rozmiar_y);
        if (x != rozmiar_x-1)odslanianie(x + 1, y, plansza, rozmiar_x, rozmiar_y);
    }
}

// funkcja wyswietla ksztalt planszy korzystajac z charow oraz kogowania 852, jesli wartosc pola jest mniejsza od zera
// czyli zostalo ono odkryte wyswietla dana wartosc, wyswietla rowniez aktualna pozycje "kursora" oraz liczbe pol nie oznaczonych flaga 
void wyswietlanie(int rozmiar_x, int rozmiar_y, int x, int y, int** plansza, int liczba_min) {
    char lewy_grog = 218;
    char prawy_grog = 191;
    char lewy_drog = 192;
    char prawy_drog = 217;
    char poziom = 196;
    char pion = 179;
    char gora = 194;
    char dol = 193;
    char lewa = 195;
    char prawa = 180;
    char srodek = 197;
    rozmiar_x *= 2;
    rozmiar_y *= 2;
    system("cls");
    for (int i = 0; i <= rozmiar_y; i++) {
        for (int k = 0; k <= rozmiar_x; k++) {
            if (i == 0 && k == 0) cout << lewy_grog;
            else if (i == 0 && k == rozmiar_x) cout << prawy_grog;
            else if (i == rozmiar_y && k == 0) cout << lewy_drog;
            else if (i == rozmiar_y && k == rozmiar_x) cout << prawy_drog;
            else if (i == 0 && k % 2 == 0) cout << gora;
            else if (i == rozmiar_y && k % 2 == 0) cout << dol;
            else if (i % 2 == 0 && k == 0) cout << lewa;
            else if (i % 2 == 0 && k == rozmiar_x) cout << prawa;
            else if (i % 2 == 1 && k % 2 == 0) cout << pion;
            else if (i % 2 == 0 && k % 2 == 1) cout << poziom;
            else if (i % 2 == 1 && k % 2 == 1 && i == y && k == x) {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 128);
                cout << " ";
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
                if (plansza[k / 2][i / 2] > 10) liczba_min--;
            }
            else if (i % 2 == 1 && k % 2 == 1 && plansza[k / 2][i / 2] == -10) {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 113);
                cout << " ";
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
            }
            else if (i % 2 == 1 && k % 2 == 1 && plansza[k / 2][i / 2] < 0) {
                if (plansza[k / 2][i / 2] == -1) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 113);
                else if (plansza[k / 2][i / 2] == -2) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 114);
                else if (plansza[k / 2][i / 2] == -3) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 124);
                else if (plansza[k / 2][i / 2] == -4) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 117);
                else if (plansza[k / 2][i / 2] == -5) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 116);
                else if (plansza[k / 2][i / 2] == -6) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 115);
                else if (plansza[k / 2][i / 2] == -7) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 112);
                else if (plansza[k / 2][i / 2] == -8) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 120);
                cout << plansza[k / 2][i / 2] * -1;
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
            }
            else if (i % 2 == 1 && k % 2 == 1 && plansza[k / 2][i / 2] > 10) {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
                cout << "F";
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
                liczba_min--;
            }
            else if (i % 2 == 1 && k % 2 == 1) cout << " ";
            else cout << srodek;

        }cout << endl;

    }cout << "\n\nPozostalo min: " << liczba_min << endl;
}

//funkcja sprawdza czy wszystkie pola nie bedace bombami zostaly odkryte, jesli tak gra konczy sie wygrana
bool koniec(int** plansza,int iloscmin, int rozmiar_x, int rozmiar_y) {
    int pola = 0;
    for (int i = 0; i < rozmiar_y; i++) {
        for (int k = 0; k < rozmiar_y; k++) {
            if (plansza[i][k] > 0) pola++;
        }
    }
    if (iloscmin == pola) return false;
    else return true;
}

// funkcja oflagowuje dane pole wedlug otrzymanych wspolrzednych oraz jesli pole jest oflagowane ta flage zdejmuje
void flaga(int x, int y, int** plansza) {
    if (plansza[x][y] > 10) plansza[x][y] -= 100;
    else if(plansza[x][y]>0) plansza[x][y] += 100;
}

// wyswietla w pelni odkryta plansze jesli gra sie zakonczy
void wyswietlenie_koncowe(int rozmiar_x, int rozmiar_y, int** plansza) {
    char lewy_grog = 218;
    char prawy_grog = 191;
    char lewy_drog = 192;
    char prawy_drog = 217;
    char poziom = 196;
    char pion = 179;
    char gora = 194;
    char dol = 193;
    char lewa = 195;
    char prawa = 180;
    char srodek = 197;
    rozmiar_x *= 2;
    rozmiar_y *= 2;
    system("cls");
    for (int i = 0; i <= rozmiar_y; i++) {
        for (int k = 0; k <= rozmiar_x; k++) {
            if (i == 0 && k == 0) cout << lewy_grog;
            else if (i == 0 && k == rozmiar_x) cout << prawy_grog;
            else if (i == rozmiar_y && k == 0) cout << lewy_drog;
            else if (i == rozmiar_y && k == rozmiar_x) cout << prawy_drog;
            else if (i == 0 && k % 2 == 0) cout << gora;
            else if (i == rozmiar_y && k % 2 == 0) cout << dol;
            else if (i % 2 == 0 && k == 0) cout << lewa;
            else if (i % 2 == 0 && k == rozmiar_x) cout << prawa;
            else if (i % 2 == 1 && k % 2 == 0) cout << pion;
            else if (i % 2 == 0 && k % 2 == 1) cout << poziom;
            else if (i % 2 == 1 && k % 2 == 1 && (plansza[k / 2][i / 2] == -10 || plansza[k / 2][i / 2] == 0)) {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 120);
                cout << " ";
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
            }
            else if (i % 2 == 1 && k % 2 == 1 && plansza[k / 2][i / 2] < 0) {
                if (plansza[k / 2][i / 2] == -1) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 113);
                else if (plansza[k / 2][i / 2] == -2) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 114);
                else if (plansza[k / 2][i / 2] == -3) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 124);
                else if (plansza[k / 2][i / 2] == -4) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 117);
                else if (plansza[k / 2][i / 2] == -5) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 116);
                else if (plansza[k / 2][i / 2] == -6) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 115);
                else if (plansza[k / 2][i / 2] == -7) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 112);
                else if (plansza[k / 2][i / 2] == -8) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 120);
                cout << plansza[k / 2][i / 2] * -1;
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
            }
            else if (i % 2 == 1 && k % 2 == 1 && plansza[k / 2][i / 2] > 0 && plansza[k / 2][i / 2] < 9) {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 120);
                cout << plansza[k / 2][i / 2];
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
            }
            else if (i % 2 == 1 && k % 2 == 1 && plansza[k / 2][i / 2] == 9) {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 71);
                cout << "X";
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
            }
            else if (i % 2 == 1 && k % 2 == 1 && plansza[k / 2][i / 2] > 10) {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
                cout << "F";
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
            }
            else if (i % 2 == 1 && k % 2 == 1) cout << " ";
            else cout << srodek;

        }cout << endl;
    }
}

int main()
{   
    int rozmiar_x = 9, rozmiar_y = 9, wybor=0, ilosc_min=10, menu=0;
// wybor poziomu trudnosci
    gra:
    system("cls");
    do {
        cout << "Wybierz poziom trudnosci:\n\n1. Poczatkujacy\n\n2. Sredniozaawansowany\n\n3. Zaawansowany\n\n4. Niestandardowy" << endl;
        cin >> wybor;
        menu = 1;
        switch (wybor) {
        case 1:
            rozmiar_x = 9;
            rozmiar_y = 9;
            ilosc_min = 10;
            break;
        case 2:
            rozmiar_x = 15;
            rozmiar_y = 15;
            ilosc_min = 40;
            break;
        case 3:
            rozmiar_x = 30;
            rozmiar_y = 16;
            ilosc_min = 99;
            break;
        case 4:
            system("cls");
            dlugosc:
            cout << "Podaj dlugosc planszy (w przedziale <5,44>)" << endl;
            cin >> rozmiar_x;
            cout << "Podaj wysokosc planszy (w przedziale <5,24>)" << endl;
            cin >> rozmiar_y;
            cout << "Podaj ilosc min (nie moze byc wieksza niz 60% ilosci pol)" << endl;
            cin >> ilosc_min;
// sprawdzenie czy liczba odpowiada kryterią, jeśli nie powrót do ponownego wyboru
            if ((ilosc_min > rozmiar_x * rozmiar_y * 0.6) || rozmiar_x < 5 || rozmiar_y < 5 || rozmiar_y > 24 || rozmiar_x > 44)
            {
                system("cls");
                cout << "WPISANO BLEDNE DANE" << endl;
                goto dlugosc;
            }
            break;
        default:
            system("cls");
            cout << "Zla wartosc wybierz ponownie!\n" << endl;
            menu = 0;
            break;
        }
    } while (menu == 0);
// tworzenie tablicy dynamicznej aby przekazywac ja do funkcji
    int** plansza = new int* [rozmiar_x];
    for (int i = 0; i < rozmiar_x; i++) {
        plansza[i] = new int[rozmiar_y];
        for (int j = 0; j < rozmiar_y; j++) {
            plansza[i][j] = 0;
        }
    }
    int x=1, y=1, ex, a=0, gra=0, liczba_ruchow=0;
    wyswietlanie(rozmiar_x, rozmiar_y, x, y,plansza, ilosc_min);
// algorytm sterowania, switch reaguje na przyciski zdefiniowane wczesniej poki gra sie nie skonczy
    while (gra==0)
    {
        Sleep(50);
        switch (ex = _getch())
                {
                case STRZALKA_GORA:
                    y -= 2;
                    break;
                case STRZALKA_DOL:
                    y += 2;
                    break;
                case STRZALKA_LEWO:
                    x -= 2;
                    break;
                case STRZALKA_PRAWO:
                    x += 2;
                    break;
                case SPACJA:
                    if (liczba_ruchow == 0) {
// bezpiczny start, losowanie po wyborze pierwszego pola
                        losowanie(plansza, x / 2, y / 2, rozmiar_x, rozmiar_y, ilosc_min);
                        wypelnienie(plansza, rozmiar_x, rozmiar_y);
                        liczba_ruchow++;
                    }
                    odslanianie(x / 2, y / 2, plansza, rozmiar_x, rozmiar_y);
// sprawdzenie czy nie odkrylismy bomby jesli tak to gra jest przegrana
                    if (plansza[x / 2][y / 2] == 9) gra = 2;
                    break;
                case FLAGA:
                    flaga(x / 2, y / 2, plansza);
                    break;
                default:                  
                    break;
                }
 // poruszanie przez granice na druga strone           
            if (x < 0) x = rozmiar_x*2 - 1;
            if (x > rozmiar_x*2) x = 1;
            if (y < 0) y = rozmiar_y*2 - 1;
            if (y > rozmiar_y*2) y = 1;
            wyswietlanie(rozmiar_x, rozmiar_y, x, y, plansza, ilosc_min);
            if (koniec(plansza, ilosc_min, rozmiar_x, rozmiar_y) == false) gra = 1;
    }
    wyswietlenie_koncowe(rozmiar_x, rozmiar_y, plansza);
    for (int i = 0; i < rozmiar_y; i++) {
        delete[] plansza[i];
    }
    delete[] plansza;
    cout << "KONIEC GRY!!\n\n";
    if (gra == 1) cout<<"Gratulacje wygrales!!!\n\n" << endl;
    if (gra == 2) cout<<"Niestety przegrales :(\n\n" << endl;
    Sleep(1000);
    cout << "Czy chesz zagrac ponownie?\n\nJesli tak wpisz: 1\n\nJesli nie wpisz: 2" << endl;
    int kontynuacja = 0;
    cin >> kontynuacja;
 // skok do wyboru trudnosci
    if (kontynuacja == 1) goto gra;
    return 0;
}