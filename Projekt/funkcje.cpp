/*****************************************************************//**
 * \file   funkcje.cpp
 * \brief  
 * 
 * \author michal sikora
 * \date   February 2023
 *********************************************************************/


#include "Naglowek.h"
#include <iostream>
#include <fstream>



string funkcjadata(DATA& data) 
{
    string datastring = ""; 
    if (data.dzien < 10) 
    {
        datastring += "0";
    }
    datastring += to_string(data.dzien) + ".";
    if (data.ms < 10) 
    {
        datastring += "0";
    }
    datastring += to_string(data.ms) + ".";
    datastring += to_string(data.rok);

    return datastring;
}
Category* czytajplik(string& nazwapliku)
{
    Category* root_category = new Category; 
    Category* aktualnacategory = root_category; 

    ifstream plik(nazwapliku);

    string line;
    int wczesniejszaliczbaminusow = -1;
    while (getline(plik, line))
    {
        // Ile jest - w linii
        if (line.empty()) break;
        int liczbaminusow = 0;
        while (line[0] == '-')
        {
            liczbaminusow += 1;
            line = line.substr(1);
        }
        
        if (liczbaminusow <= wczesniejszaliczbaminusow)
        {
            for (int i = 0; i < (wczesniejszaliczbaminusow - liczbaminusow + 1); i++) 
            {
                aktualnacategory = aktualnacategory->rodzic;
            }
        }
        
        Category* new_category = new Category;
        new_category->rodzic = aktualnacategory;
        aktualnacategory->podcategory.push_back(new_category);
       
        aktualnacategory = new_category;
        wczesniejszaliczbaminusow = liczbaminusow; 

        
        int pozycjakursora;
        pozycjakursora = line.find("(");
        aktualnacategory->name = line.substr(0, pozycjakursora - 1);
        line = line.substr(pozycjakursora + 1);
        // Pozycje
        if (line.length() > 1) 
        {
            bool spacja = true; 
            do { 
                pozycja nowapozycja;
                pozycjakursora = line.find(" ");
                nowapozycja.nazwa = line.substr(0, pozycjakursora);
                line = line.substr(pozycjakursora + 1); 
                pozycjakursora = line.find(" ");
                string datastring = line.substr(0, pozycjakursora);
                line = line.substr(pozycjakursora + 1);
    
                // Data
                DATA nowadata;
                pozycjakursora = datastring.find(".");
                nowadata.dzien = stoi(datastring.substr(0, pozycjakursora));
                datastring = datastring.substr(pozycjakursora + 1);
                pozycjakursora = datastring.find(".");
                nowadata.ms = stoi(datastring.substr(0, pozycjakursora));
                datastring = datastring.substr(pozycjakursora + 1);
                nowadata.rok = stod(datastring);
                nowapozycja.data = nowadata;
                pozycjakursora= line.find(" ");
                if (pozycjakursora == -1) 
                {
                    spacja = false;
                    pozycjakursora = line.find(")");
                }

                nowapozycja.wartosc = stod(line.substr(0, pozycjakursora));
                if (spacja) 
                {
                    line = line.substr(pozycjakursora + 3); 
                }
                aktualnacategory->pozycje.push_back(nowapozycja);
            } while (spacja);
        }
    }

    plik.close();
    return root_category;
}
string categorynastring(Category* category, int poziom) 
{
    string categoriesstring = "";

    if (poziom > 0) {
        for (int i = 0; i < poziom; i++) 
        {
            categoriesstring += "-";
        }
        categoriesstring += category->name + " (";
        for (int i = 0; i < category->pozycje.size(); i++) 
        {
            categoriesstring += category->pozycje[i].nazwa + " " + funkcjadata(category->pozycje[i].data) + " " + to_string(category->pozycje[i].wartosc);
            if (i < category->pozycje.size() - 1) 
            {
                categoriesstring += " ; ";
            }
        }
        categoriesstring += ")\n";
    }

    for (int i = 0; i < category->podcategory.size(); i++)
    {
        categoriesstring += categorynastring(category->podcategory[i], poziom + 1);
    }

    return categoriesstring;
}
void zapiszdopliku(string nazwapliku, string content) 
{
    ofstream plik(nazwapliku);
    plik << content;
    plik.close();
}
Category* znajdzkategorie(Category* category, string& name) 
{
    if (category->name == name)
        return category;
    else
    {
        Category* categorywskaznik = NULL;
        for (int i = 0; i < category->podcategory.size(); i++)
        {
            categorywskaznik = znajdzkategorie(category->podcategory[i], name);
            if (categorywskaznik != NULL)
                return categorywskaznik;
        }
        return NULL;
    }
}
void wyczysckategorie(Category* category) 
{
    int n_clears = 0;
    for (int i = 0; i < category->podcategory.size(); i++)
        wyczysckategorie(category->podcategory[i]);
    n_clears++;
    delete category; 
}
void dodajkategorie(Category* root_category)
{
    string ans;
    cout << "Podaj nazwe rodzica (jeœli chcesz dodac kategorie glowna, wpisz: root): ";
    cin >> ans;
    Category* rodzicwskaznik = znajdzkategorie(root_category, ans);
    if (rodzicwskaznik != NULL || ans == "root")
    {
        cout << "Podaj nazwe kategorii: ";
        cin >> ans;
        Category* nowakategoria = new Category; 
        nowakategoria->name = ans;
        if (rodzicwskaznik != NULL)
        {
            rodzicwskaznik->podcategory.push_back(nowakategoria);
            nowakategoria->rodzic = rodzicwskaznik;
        }
        else
        {
            root_category->podcategory.push_back(nowakategoria);
            nowakategoria->rodzic = root_category;
        }
        zapiszdopliku("budzet.txt", categorynastring(root_category, 0));
        cout << "dodano kategorie pomyslnie" << endl;
    }
    else
    {
        cout << "Nie znaleziono takiej kategorii." << endl;
    }
}
void usuniekategorii(Category* root_category)
{
    cout << "Podaj nazwe kategorii do usuniecia: ";
    string ans;
    cin >> ans;
    Category* categorywskaznik = znajdzkategorie(root_category, ans);
    if (categorywskaznik != NULL)
    {
        Category* rodzic = categorywskaznik->rodzic;
        for (int i = 0; i < rodzic->podcategory.size(); i++)
        {
            if (rodzic->podcategory[i]->name == ans)
            {
                wyczysckategorie(rodzic->podcategory[i]); 
                rodzic->podcategory.erase(rodzic->podcategory.begin() + i); 
                break;
            }
        }
        zapiszdopliku("budzet.txt", categorynastring(root_category, 0));
        cout << "usunieto kategorie pomyœlnie" << endl;
    }
    else
    {
        cout << "Nie ma takiej kategorii." << endl;
    }
}
void edytujkategorie(Category* root_category)
{
    string ans;
    cout << "podaj nazwe kategorii jaka chcesz edytowac " << endl;
    cin >> ans;
    Category* categorywskaznik = znajdzkategorie(root_category, ans);
    if (categorywskaznik != NULL)
    {
        cout << "podaj na jaka nazwe chcesz zmienic ta kategorie" << endl;
        cin >> ans;
        categorywskaznik->name = ans;
        zapiszdopliku("budzet.txt", categorynastring(root_category, 0));
        cout << "edycja kategorii przebiegla bez problemu" << endl;
    }
    else
    {
        cout << "nie ma takiej kategorii" << endl;
    }

}
void przenieskategorie(Category* root_category)
{
    string kategoriadoprzeniesienia;
    cout << "podaj kategorie ktora chcesz przeniesc" << endl;
    cin >>kategoriadoprzeniesienia;
    Category* categorydoprzeniesienia = znajdzkategorie(root_category, kategoriadoprzeniesienia);
    if (categorydoprzeniesienia != NULL)
    {
        string nazwarodzica;
        cout << "podaj nazwe nowego rodzica (jesli chcesz zrobic zeby to byla glowna kategoria napisz 'root')" << endl;
        cin >> nazwarodzica;
        Category* rodzicnowy = znajdzkategorie(root_category, nazwarodzica);
        if (rodzicnowy != NULL || nazwarodzica == "root")
        {
            Category* rodzicstary = categorydoprzeniesienia->rodzic;
            for (int i = 0; i < rodzicstary->podcategory.size(); i++)
            {
                if (rodzicstary->podcategory[i]->name == kategoriadoprzeniesienia)
                {
                    rodzicstary->podcategory.erase(rodzicstary->podcategory.begin() + i); 
                    break;
                }


            }
            if (rodzicnowy != NULL)
            { 
                rodzicnowy->podcategory.push_back(categorydoprzeniesienia);
                categorydoprzeniesienia->rodzic = rodzicnowy;
            }
            else
            {
                root_category->podcategory.push_back(categorydoprzeniesienia);
                categorydoprzeniesienia->rodzic = root_category; 
            }
            zapiszdopliku("budzet.txt", categorynastring(root_category, 0));
            cout << "przeniesiono kategorie pomyslnie" << endl;
        }
        else
        {
            cout << "nie ma takiej kategorii" << endl;
        }
    }
    else
    {
        cout << "nie ma takiej kategorii" << endl;
    }

}

double zsumujcalybudzet(Category* root_category)
{
    double suma = 0.0;
    for ( int i = 0; i < root_category->podcategory.size(); i++) 
    {
        Category* kategoria = root_category->podcategory[i];
        for (int j = 0; j < kategoria->pozycje.size(); j++) 
        {
            pozycja* pozycja = &(kategoria->pozycje[j]); 
            suma += pozycja->wartosc;
        }
        suma += zsumujcalybudzet(kategoria); 
    }
    return suma;
}
double sumawartosci(Category* root_category)
{
    double suma = 0.0;
    for (int i = 0; i < root_category->pozycje.size(); i++)
        suma += root_category->pozycje[i].wartosc;

    for (int i = 0; i < root_category->podcategory.size(); i++)
        suma += sumawartosci(root_category->podcategory[i]);

    return suma;
}
void wartoscdlakategorii(Category* root_category)
{
   double suma = 0.0;
    string nazwakategorii;
    cout << "podaj nazwe kategorii" << endl;
    cin >> nazwakategorii;
   Category* kategoria= znajdzkategorie(root_category, nazwakategorii);
   if (kategoria != NULL)
   {
       cout << "wartosc dla twojej kategorii: " << sumawartosci(kategoria) << endl;
   }
   else
   {
       cout << "nie ma takiej kategorii" << endl;
   }      
}
void dodajpozycje(Category* root_category)
{
   string nazwakategorii, nazwapozycji;
   double wartosc;
    int dzien, ms, rok;
    cout << "podaj nazwe kategorii, do ktorej chcesz dodac pozycje: " << endl;
    cin >> nazwakategorii;

    Category* kategoria = znajdzkategorie(root_category, nazwakategorii);
    if (kategoria != NULL)
    {
        cout << "podaj nazwe nowej pozycji " << endl;
        cin >> nazwapozycji;
        cout << "podaj dzien" << endl;
        cin >> dzien;
        if (cin.fail())
        {
            cout << "niepoprawny dzien";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else if (dzien <= 31 || dzien >= 1)
        {
            cout << "podaj miesiac";
            cin >> ms;
            if (cin.fail())
            {
                cout << "niepoprawny miesiac";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            else if(ms>=1 || ms<=12)
            {
                cout << "podaj rok ";
                cin >> rok;
                if (cin.fail())
                {
                    cout << "niepoprawny rok ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                else if (rok >= 0 || rok <= 9999)
                {
                    cout << "podaj wartosc";
                    cin >> wartosc;
                    if (cin.fail())
                    {
                        cout << "niepoprawna wartosc ";
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    }
                    else 
                    {
                        DATA data = { rok, ms, dzien };
                        pozycja nowa_pozycja = { nazwapozycji, data, wartosc };
                        kategoria->pozycje.push_back(nowa_pozycja);
                        cout << "Pozycja o nazwie " << nazwapozycji << " zostala dodana do kategorii o nazwie  " << nazwakategorii << endl;
                    }
                    zapiszdopliku("budzet.txt", categorynastring(root_category, 0));
                    
                }
            }
        }
        

    }
    else
    {
        cout << "nie ma takiej kategorii" << endl;
    }
    
}
pozycja* znajdzpozycje(Category* category, string& pozycjanazwa) 
{
    for (int i = 0; i < category->pozycje.size(); i++) 
    {
        if (category->pozycje[i].nazwa == pozycjanazwa)
            return &category->pozycje[i];
    }

    pozycja* pozycjawskaznik = NULL;
    for (int i = 0; i < category->podcategory.size(); i++)
    {
        pozycjawskaznik = znajdzpozycje(category->podcategory[i], pozycjanazwa);
        if (pozycjawskaznik != NULL)
            return pozycjawskaznik;
    }
    return NULL;
}
void edytujpozycje(Category* root_category)
{
    string nazwapozycji;
    int odp;
    cout << "podaj nazwe pozycji do edycji" << endl;
    cin >> nazwapozycji;
    pozycja* pozycja = znajdzpozycje(root_category, nazwapozycji);
    if (pozycja != NULL)
    {
        cout << "jezeli chcesz zmienic nazwe wcisnij 1 " << endl;
        cout << "jezeli chcesz zmienic date wcisnij 2 " << endl;
        cout << "jezeli chcesz zmienic wartosc wcisnij 3 " << endl;
        cin >> odp;
        if (cin.fail())
        {
            cout << "ERROR";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else if (odp == 1)
        {
            string nowanazwa;
            cout << "podaj nowa nazwe:" << endl;
            cin >> nowanazwa;
            pozycja->nazwa = nowanazwa;
            cout << "zmieniono nazwe pomyslnie" << endl;
        }
        else if (odp == 2)
        {
            int dzien, ms, rok;
            cout << "podaj dzien" << endl;
            cin >> dzien;
            if (cin.fail())
            {
                cout << "niepoprawny dzien";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
              
            }
            else if(dzien <=31 || dzien >=1)
            {
                cout << "podaj miesiac";
                cin >> ms;
                if (cin.fail())
                {
                    cout << "niepoprawny miesiac";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                else if(ms<=12 || ms>=1)
                {
                    cout << "podaj rok ";
                    cin >> rok;
                    if (cin.fail())
                    {
                        cout << "niepoprawny rok\n";
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    }
                    else if(rok<=9999 || rok>=0 )
                    {
                        DATA data = { rok, ms, dzien };
                        pozycja->data = data;
                        cout << "zaktualizowano date pomyslnie" << endl;
                    }
                }
            }
        }
        else if (odp == 3)
        {
            double wartosc;
            cout << "podaj wartosc" << endl;
            cin >> wartosc;  
            if (cin.fail())
            {
                cout << "niepoprawna wartosc" << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
               
            }
            else 
            {
                pozycja->wartosc = wartosc;
                cout << "zmieniono wartosc pomyslnie" << endl;
            }
            
        }
        zapiszdopliku("budzet.txt", categorynastring(root_category, 0));
    }
    else
    {
        cout << "nie ma takiej pozycji" << endl;
    }
}
Category* znajdzkategorieszukanejpozycji(Category* category, string& pozycjanazwa) 
{
    for (int i = 0; i < category->pozycje.size(); i++)
    {
        if (category->pozycje[i].nazwa == pozycjanazwa)
            return category;
    }

    Category* kategoria_w_podcategory = NULL;
    for (int i = 0; i < category->podcategory.size(); i++)
    {
        kategoria_w_podcategory = znajdzkategorieszukanejpozycji(category->podcategory[i], pozycjanazwa);
        if (kategoria_w_podcategory != NULL)
            return kategoria_w_podcategory;
    }
    return NULL;
}
void usunpozycje(Category* root_category)
{
    string nazwapozycji;
    cout << "podaj nazwe pozycji do usuniecia" << endl;
    cin >> nazwapozycji;
     Category* kategoria_wskaznik = znajdzkategorieszukanejpozycji(root_category, nazwapozycji); 
     if (kategoria_wskaznik != NULL)
     {
         for (int i = 0; i < kategoria_wskaznik->pozycje.size(); i++)
         {
             if (kategoria_wskaznik->pozycje[i].nazwa == nazwapozycji)
             {
                 kategoria_wskaznik->pozycje.erase(kategoria_wskaznik->pozycje.begin() + i);
                 break;
             }
             
         }
         zapiszdopliku("budzet.txt", categorynastring(root_category, 0));
         cout << "pozycja o nazwie " << nazwapozycji << " zostala usunieta z budzetu" << endl;
     }
     else
     {
         cout << "nie ma takiej pozycji" << endl;
     }
}
void filtrujodkwoty(Category* root_category,double kwota)
{
  

        for (int i = 0; i < root_category->pozycje.size(); i++)
        {


            if (root_category->pozycje[i].wartosc >= kwota)
            {
                cout << root_category->pozycje[i].nazwa << " kwota:" << root_category->pozycje[i].wartosc << endl;
            }
            
        }
        for (int i = 0; i < root_category->podcategory.size(); i++)
        {
            filtrujodkwoty(root_category->podcategory[i], kwota);
        }
    
}
bool czyjestprzestepnyrok(int rok)
{
    if (rok % 400 == 0)
        return true;
    if (rok % 100 == 0)
        return false;
    if (rok % 4 == 0)
        return true;
    return false;
}
int iledniwroku(int dzien, int ms, int rok) 
{
    int daysinmonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if (czyjestprzestepnyrok(rok)) 
    {
        daysinmonth[1] = 29; //luty 29 lub 28
    }

    int dayinyear = dzien;
    for (int i = 0; i < ms - 1; i++)
        dayinyear += daysinmonth[i];

    return dayinyear;
}
void filtrujoddaty(Category* root_category,  int dziendo ,int msdo,int rokdo, int rokod, int msod , int dzienod)
{
   for (int i = 0; i < root_category->pozycje.size(); i++)
   {
       pozycja pozycja = root_category->pozycje[i]; 
       if ((pozycja.data.rok >= rokod && iledniwroku(pozycja.data.dzien, pozycja.data.ms, pozycja.data.rok) >= iledniwroku(dzienod, msod, rokod))
           && (pozycja.data.rok <= rokdo && iledniwroku(pozycja.data.dzien, pozycja.data.ms, pozycja.data.rok) <= iledniwroku(dziendo, msdo, rokdo)))
       {
            cout << "pozycja z tego przedzialu to:" << endl;
            cout << root_category->pozycje[i].nazwa << " kwota: " << root_category->pozycje[i].wartosc << endl;
       }
    }

    for (int i = 0; i < root_category->podcategory.size(); i++)
    {
        filtrujoddaty(root_category->podcategory[i],  dziendo, msdo, rokdo, rokod,msod,dzienod);
    }

}
double sumawartosciwmiesiacu(Category* root_category, int ms, int rok)
{
    double suma = 0;
    for (int i = 0; i < root_category->pozycje.size(); i++)
    {
        pozycja pozycja = root_category->pozycje[i];
        if (pozycja.data.rok == rok && pozycja.data.ms == ms)
        {
            suma += pozycja.wartosc;
        }

    }

    for (int i = 0; i < root_category->podcategory.size(); i++)
    {
        suma += sumawartosciwmiesiacu(root_category->podcategory[i], ms, rok);
    }

    return suma;
}