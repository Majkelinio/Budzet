/*****************************************************************//**
 * \file   Projekt.cpp
 * \brief  
 * 
 * \author michal sikora
 * \date   February 2023
 *********************************************************************/
#include "Naglowek.h"
#include <iostream>

int main(int argc, char* argv[])
{
    string nazwapliku ;
    cout << "podaj nazwe pliku" << endl;
    cin >> nazwapliku;
    if (nazwapliku == "budzet.txt")
    {
        for (; ;)
        {

            Category* root_category = czytajplik(nazwapliku);
            cout << "WITAJ W SWOIM BUDZECIE" << endl;
            cout << "___________________________" << endl;
            cout << "Aktualny stan twojego budzetu" << endl;
            cout << categorynastring(root_category, 0);
            cout << "Nacisnij w zaleznosci od tego co chcesz zrobic" << endl;
            cout << "a.dodaj kategorie" << endl;
            cout << "b.edytuj kategorie" << endl;
            cout << "c.usun kategorie" << endl;
            cout << "d.dodaj pozycje" << endl;
            cout << "e.edytuj pozycje" << endl;
            cout << "f.usun pozycje" << endl;
            cout << "g.zsumuj dana kategorie" << endl;
            cout << "h.zsumuj caly budzet" << endl;
            cout << "i.filtruj od kwoty" << endl;
            cout << "j.filtruj od daty" << endl;
            cout << "k.policz ile jestes w danym miesiacu na +/-" << endl;
            cout << "l. przenies kategorie" << endl;
            cout << "w.wyjscie" << endl;
            char wybor;
            cin >> wybor;
            cout << ' ' << endl;
            double suma = 0;
            switch (wybor)
            {
            case 'w':
                wyczysckategorie(root_category);
                return 0;

            default: cout << "nie ma tego na liscie sprobuj jeszcze raz" << endl;
                break;

            case 'a':
                dodajkategorie(root_category);
                break;
            case 'b':
                edytujkategorie(root_category);
                break;
            case 'c':
                usuniekategorii(root_category);
                break;
            case 'd':
                dodajpozycje(root_category);
                break;
            case 'e':
                edytujpozycje(root_category);
                break;
            case 'f':
                usunpozycje(root_category);
                break;
            case 'g':
                wartoscdlakategorii(root_category);
                break;
            case 'h':
                suma = zsumujcalybudzet(root_category);
                cout << "aktualny stan twojego budzetu wynosi: " << suma << endl;
                break;
            case 'i':
                double kwota;
                cout << "podaj od jakiej kwoty chcesz wyswietlic pozycje" << endl;
                cin >> kwota;
                if (cin.fail())
                {
                    cout << "Podano niepoprawna wartosc\n";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                else
                {
                    filtrujodkwoty(root_category, kwota);
                }

                break;
            case 'j':
                int dziendo, msdo, rokdo, dzienod, msod, rokod;
                cout << "podaj date od (rok)" << endl;
                cin >> rokod;
                if (cin.fail())
                {
                    cout << "zle podany rok\n";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                else if (rokod >= 0 || rokod <= 9999)
                {
                    cout << "podaj date od (miesiac)" << endl;
                    cin >> msod;

                    if (cin.fail())
                    {
                        cout << "zle podany miesiac\n";
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    }
                    else if (msod >= 1 || msod <= 12)
                    {
                        cout << "podaj date od (dzien)" << endl;
                        cin >> dzienod;
                        if (cin.fail())
                        {
                            cout << "zle podany miesiac\n";
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        }
                        else if (dzienod >= 1 || dzienod <= 31)
                        {
                            cout << "podaj date do (rok)" << endl;
                            cin >> rokdo;
                            if (cin.fail())
                            {
                                cout << "zle podany rok\n";
                                cin.clear();
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            }
                            else if (rokdo >= 0 || rokdo <= 9999)
                            {
                                cout << "podaj date do (miesiac)" << endl;
                                cin >> msdo;

                                if (cin.fail())
                                {
                                    cout << "zle podany miesiac\n";
                                    cin.clear();
                                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                }
                                else if (msdo >= 1 || msdo <= 12)
                                {
                                    cout << "podaj date do (dzien)" << endl;
                                    cin >> dziendo;
                                    if (cin.fail())
                                    {
                                        cout << "zle podany dzien\n";
                                        cin.clear();
                                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                    }
                                    else if (dziendo >= 1 || dziendo <= 31)
                                    {

                                        filtrujoddaty(root_category, dziendo, msdo, rokdo, rokod, msod, dzienod);
                                    }
                                }
                            }
                        }
                    }
                }

                break;
            case 'k':
                int ms, rok;

                cout << "podaj rok " << endl;
                cin >> rok;
                if (cin.fail())
                {
                    cout << "zle podany rok\n";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                else if (rok >= 0 || rok <= 9999)
                {
                    cout << "podaj miesiac" << endl;
                    cin >> ms;
                    if (cin.fail())
                    {
                        cout << "zle podany miesiac\n";
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    }
                    else if (ms >= 1 || ms <= 12)
                    {
                        suma = sumawartosciwmiesiacu(root_category, ms, rok);
                        if (suma != 0)
                        {
                            cout << "Suma wartosci w podanym miesiacu i roku wynosi: " << suma << endl;
                        }
                        else
                        {
                            cout << "brak pozycji w tym miesiacu" << endl;
                        }
                    }
                    break;
            case 'l':
                przenieskategorie(root_category);
                break;
                }
                getchar();
                getchar();
                system("cls");

            }
        }
 
    }
    else
    {
    cout << "zly plik" << endl;
    }
    return 0;
}