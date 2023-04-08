/*****************************************************************//**
 * \file   Naglowek.h
 * \brief  
 * 
 * \author michal sikora
 * \date   February 2023
 *********************************************************************/
#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>
#include <limits>

using namespace std;
/**
 * @brief struktura ktora tworzy date.
 */
struct DATA //struktura tworzaca date
{
	int rok;
	int ms;
	int dzien;
};
/**
 * @brief struktura ktora tworzy pozycje sklada sie z nazwy daty i wartosci.
 */
 struct pozycja  
 {
	 string nazwa;
	 DATA data;
	 double wartosc;
 };
 /**
  * @brief struktura Category ktora tworzy kategorie .
  */
struct Category
{
	Category* rodzic = NULL; //null bo nie ma jeszcze nic przypisanego moze byc null jak jest glowna 
	string name;
	vector<pozycja> pozycje;
	vector<Category*> podcategory; // wektor wskaznikow
};

/// <summary>
/// @brief funkcja zmieniajaca date na napis w postaci "DD.MM.YYYY"
/// </summary>
/// <param name="data"></param>
/// <returns></returns> napis w postaci "DD.MM.YYYY"
string funkcjadata(DATA& data);
/// <summary>
/// @brief Funkcja zczytujaca plik o naziwe "budzet.txt".
/// </summary>
/// <param name="nazwapliku"></param>
/// <returns></returns> zwraca wskaznik na korzen kategorii
Category* czytajplik(string& nazwapliku);
/// <summary>
/// @brief Funkcja tworzaca napisy z kategorii i ich pozycji.
/// </summary>
/// <param name="category"></param>
/// <param name="poziom"></param>
/// <returns></returns>zwraca napis
string categorynastring(Category* category, int poziom);
/// <summary>
/// @brief Funkcja zapisujaca do pliku.
/// </summary>
/// <param name="nazwapliku"></param>
/// <param name="content"></param>
void zapiszdopliku(string nazwapliku, string content);
/// <summary>
/// @brief Funkcja szukajaca danej kategorii.
/// </summary>
/// <param name="category"></param>
/// <param name="ans"></param>
/// <returns></returns> wskaznik na wybrana kategorie
Category* znajdzkategorie(Category* category, string& ans);
/// <summary>
/// @brief Funkcja rekurencyjnie usuwa wszystkie kategorie i ich podkategorie zaczynajac od danej kategorii.
/// </summary>
/// <param name="category"></param>
void wyczysckategorie(Category* category);
/// <summary>
/// @brief Funkcja umozliwiajaca dodawanie wybranej kategorii.
/// </summary>
/// <param name="root_category"></param>
void dodajkategorie(Category* root_category);
/// <summary>
/// @brief Funkcja umozliwiajaca usuwanie wybranej kategorii.
/// </summary>
/// <param name="root_category"></param>
void usuniekategorii(Category* root_category);
/// <summary>
/// @brief Funkcja umozliwiajaca edycje wybranych kategorii.
/// </summary>
/// <param name="root_category"></param>
void edytujkategorie(Category* root_category);
/// <summary>
/// @brief Funkcja umozliwiajaca przenoszenie wybranych kategorii w drzewie.
/// </summary>
/// <param name="root_category"></param>
void przenieskategorie(Category* root_category);
/// <summary>
/// @brief Funkcja sumujaca wszystkie kwoty w budzecie.
/// </summary>
/// <param name="root_category"></param>
/// <returns></returns>aktualna kwote calego budzetu
double zsumujcalybudzet(Category* root_category);
/// <summary>
/// @brief Funkcja sumujaca wartosc wybranej kategorii i jej podkategorii.
/// </summary>
/// <param name="root_category"></param>
/// <returns></returns>wartosc dla danej kategorii i jej podkategorii
double sumawartosci(Category* root_category);
/// <summary>
/// @brief Funkcja korzystajaca z funkcji sumawartosci wyswietla ile wynosi kwota dla danej kategorii i jej podkategorii wprowadzonej przez uzytkownika.
/// </summary>
/// <param name="root_category"></param>
void wartoscdlakategorii(Category* root_category);
/// <summary>
/// @brief Funkcja umozliwiajaca dodawanie nowych pozycji do odpowiedniej kategorii.
/// </summary>
/// <param name="root_category"></param>
void dodajpozycje(Category* root_category);
/// <summary>
/// @brief Funkcja szukajaca wybranej pozycji.
/// </summary>
/// <param name="category"></param>
/// <param name="pozycjanazwa"></param>
/// <returns></returns> wskaznik szukanej pozycji
pozycja* znajdzpozycje(Category* category, string& pozycjanazwa);
/// <summary>
/// @brief funkcja umozliwiajaca edytowanie wybranej pozycji
/// </summary>
/// <param name="root_category"></param>
void edytujpozycje(Category* root_category);
/// <summary>
/// @brief Funkcja umozliwiajaca usuwanie danej pozycji.
/// </summary>
/// <param name="root_category"></param>
void usunpozycje(Category* root_category);
/// <summary>
/// @brief Funkcja, ktora umozliwa wyswietlanie pozycji ktore maja wieksza kwote od wpisanej przez uzytkownika.
/// </summary>
/// <param name="root_category"></param>
/// <param name="kwota"></param>
void filtrujodkwoty(Category* root_category, double kwota);
/// <summary>
/// @brief Funkcja umozliwiajaca wyswietlanie pozycji z przedzialu dat wpisanych przez uzytkownika.
/// </summary>
/// <param name="root_category"></param>
/// <param name="dziendo"></param>
/// <param name="msdo"></param>
/// <param name="rokdo"></param>
/// <param name="rokod"></param>
/// <param name="msod"></param>
/// <param name="dzienod"></param>
void filtrujoddaty(Category* root_category,  int dziendo, int msdo, int rokdo, int rokod, int msod, int dzienod);
/// <summary>
/// @brief Funkcja, ktora liczy odpowiednia kwote w miesiaciu wpisanym przez uzytkownika.
/// </summary>
/// <param name="root_category"></param>
/// <param name="ms"></param>
/// <param name="rok"></param>
/// <returns></returns> wartosc w danym miesiacu
double sumawartosciwmiesiacu(Category* root_category, int ms, int rok);
/// <summary>
/// @brief Funkcja sprawdzajaca czy dany rok jest przestepny.
/// </summary>
/// <param name="rok"></param>
/// <returns></returns> czy rok jest przestepny czy nie 
bool czyjestprzestepnyrok(int rok);
/// <summary>
/// @brief Funkcja, ktora zmienia dana date na konkretna liczbe dzieki czemu mozna je porownywac arytmetycznie.
/// </summary>
/// <param name="dzien"></param>
/// <param name="ms"></param>
/// <param name="rok"></param>
/// <returns></returns> liczbednii
int iledniwroku(int dzien, int ms, int rok);

