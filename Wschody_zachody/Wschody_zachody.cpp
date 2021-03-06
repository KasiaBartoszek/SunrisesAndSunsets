// Wschody_zachody.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <ctime>
#include <stdio.h>
#include <cmath>
#include <string>
#include <fstream>
#define M_PI 3.14159265358979323846
using namespace std;

struct date
{
	int day;
	int month;
	int year;
};

struct godziny
{
	int hour_sunrise;
	int minute_sunrise;
	int hour_sunset;
	int minute_sunset;
};

bool load_params(int argc, char* argv[], string &nazwa, double &lon, double &lat, double &t, struct date &data_poczatkowa, struct date &data_koncowa)
{

	const string params[7] = { "h", "p", "s", "e", "lon", "lat", "t" };
	int flaga[6] = { 0 };

	if (argc <= 1)
		cout << "Nie podano parametrow wejsciowych. Uruchom program z przelacznikiem h, aby uzyskac pomoc." << endl;
	
	else
		for (int i = 1; i < argc - 1; i++)
		{
			if (argv[i] == params[0]) //help
				cout << "Wpisz przy uruchomieniu programu:\nh - pomoc\np - nazwa pliku wyjsciowego\ns - data poczatkowa dd.mm.yyyy\ne - data koncowa dd.mm.yyyy\nlon - dlugosc (od -180 do 180)\nlat - szerokosc (od -90 do 90)\nt - strefa czasowa (od -12 do 12)\n\n" << endl;
			
			if (argv[i] == params[1]) //plik wyjsciowy
			{
				nazwa = argv[i + 1];
				flaga[0] = 1;
			}
			
			if (argv[i] == params[2]) //data poczatkowa
			{
				std::string s_date(argv[i + 1]);
				int pierwsza_kropka = s_date.find('.');
				std::string s_day = s_date.substr(0, pierwsza_kropka);
				data_poczatkowa.day = atoi(s_day.c_str()); // korekcja
				std::string temp1 = s_date.substr(pierwsza_kropka + 1, 9);
				int druga_kropka = temp1.find('.');
				std::string s_month = temp1.substr(0, druga_kropka);
				data_poczatkowa.month = atoi(s_month.c_str());
				std::string temp2 = temp1.substr(druga_kropka + 1, 9);
				data_poczatkowa.year = atoi(temp2.c_str());

				if ((pierwsza_kropka == -1) | (druga_kropka == -1))
				{
					cout << "Podano niewlasciwa date poczatkowa." << endl;
				}
				else flaga[1] = 1;
			}
			if (argv[i] == params[3]) //data koncowa
			{
				std::string e_date(argv[i + 1]);
				int pierwsza_kropka = e_date.find('.');
				std::string e_day = e_date.substr(0, pierwsza_kropka);
				data_koncowa.day = atoi(e_day.c_str());
				std::string temp1 = e_date.substr(pierwsza_kropka + 1, 9);
				int druga_kropka = temp1.find('.');
				std::string s_month = temp1.substr(0, druga_kropka);
				data_koncowa.month = atoi(s_month.c_str());
				std::string temp2 = temp1.substr(druga_kropka + 1, 9);
				data_koncowa.year = atoi(temp2.c_str());

				if ((pierwsza_kropka == -1) || (druga_kropka == -1))
					cout << "Podano niewlasciwa date koncowa." << endl;

				else flaga[2] = 1;
			}
			if (argv[i] == params[4]) //dlugosc
			{
				lon = atof(argv[i + 1]);

				if (lon < -180 || lon > 180)
					cout << "Podano niewlasciwa dlugosc." << endl;

				else flaga[3] = 1;
			}
			if (argv[i] == params[5]) //szerokosc
			{
				lat = atof(argv[i + 1]);

				if (lat < -90 || lat > 90)
					cout << "Podano niewlasciwa szerokosc." << endl;

				else flaga[4] = 1;
			}
			if (argv[i] == params[6]) //strefa czasowa
			{
				t = atof(argv[i + 1]);

				if (t < -12 || t > 12)
					cout << "Podano niewlasciwa strefe czasowa." << endl;

				else flaga[5] = 1;
			}
		}

	for (int i = 0; i < 6; i++)
	{
		if (flaga[i] == 0)
			return false;
	}

	return true;
}

/*void naglowek(string &nazwa)
{
	std::ofstream plik;
	plik.open(nazwa, std::ios::in | std::ios::out | std::ios::app);
	if (plik.good() == true)
	{
		plik << sunrise_avenue(lon, lat, t, temp);
		plik.close();
	}
}*/

tm correct(const int day, const int month, const int year)
{
	time_t sekundy;
	time(&sekundy);
	tm *dane_dnia = localtime(&sekundy);
	dane_dnia->tm_mday = day;
	dane_dnia->tm_mon = month - 1;
	dane_dnia->tm_year = year - 1900;
	sekundy = mktime(dane_dnia);
	dane_dnia->tm_mon += 1;
	dane_dnia->tm_year += 1900; //danie dnia->cuj = danie dnia->cuj + 1900

	/*
	tm danie;
	tm * dane_dnia = &danie;

	time_t czas = mktime(&danie);
	//dane_dnia =  localtime(&czas);
	*/
	return *(dane_dnia);
}

godziny sunrise_avenue(double lon, double lat, double t, tm &temp, godziny &godzina_wschodu, godziny &godzina_zachodu) //struct date &data
{
	//tm temp = correct(data.day, data.month, data.year);

	//godziny godzina_wschodu, godzina_zachodu;

	double req = -0.833;
	double J = 367 * temp.tm_year - int(7 * (temp.tm_year + int((temp.tm_mon + 9) / 12)) / 4) + int(275 * temp.tm_mon / 9) + temp.tm_mday - 730531.5;
	double Cent = J / 36525;
	double L = fmod((4.8949504201433 + 628.331969753199*Cent), 6.28318530718);
	double G = fmod((6.2400408 + 628.3019501*Cent), 6.28318530718);
	double O = 0.409093 - 0.0002269*Cent;
	double F = 0.033423*sin(G) + 0.00034907*sin(2 * G);
	double E = 0.0430398*sin(2 * (L + F)) - 0.00092502*sin(4 * (L + F)) - F;
	double A = asin(sin(O)*sin(L + F));
	double C = (sin(0.017453293*req) - sin(0.017453293*lat)*sin(A)) / (cos(0.017453293*lat)*cos(A));

	double sunrise = (M_PI - (E + 0.017453293*lon + 1 * acos(C)))*57.29577951 / 15;
	godzina_wschodu.hour_sunrise = sunrise + t;
	int calk = sunrise;
	int minuta = (sunrise - calk) * 60;
	godzina_wschodu.minute_sunrise = minuta;

	double sunset = (M_PI - (E + 0.017453293*lon + (-1)*acos(C)))*57.29577951 / 15;
	godzina_zachodu.hour_sunset = sunset + t;
	int calk1 = sunset;
	int minuta1 = (sunset - calk1) * 60;
	godzina_zachodu.minute_sunset = minuta1;

	return godzina_wschodu, godzina_zachodu;
}

void funkcja_nadrzedna(double lon, double lat, double t, struct date &data, struct date &data_koncowa, string &nazwa, godziny &godzina_wschodu, godziny &godzina_zachodu, tm &dane_dnia)
{
	tm correct_end_date = correct(data_koncowa.day, data_koncowa.month, data_koncowa.year);
	tm temp = correct(data.day, data.month, data.year);

	if (data_koncowa.year < data.year)
	{
		cout << "Data koncowa jest wczesniejsza niz data poczatkowa.";
		return;
	}
	else if (data_koncowa.year == data.year)
	{
		if (data_koncowa.month < data.month)
		{
			cout << "Data koncowa jest wczesniejsza niz data poczatkowa.";
			return;
		}
		else if (data_koncowa.month == data.month)
		{
			if (data_koncowa.day < data.day)
			{
				cout << "Data koncowa jest wczesniejsza niz data poczatkowa.";
				return;
			}
		}
	}

	std::fstream plik;
	plik.open(nazwa, std::ios::in | std::ios::out | std::ios::app); 
	//int k = (temp.tm_mday != correct_end_date.tm_mday) && (temp.tm_mon != correct_end_date.tm_mon);

	if (plik.good() == true)
	{
		do
		{
			sunrise_avenue(lon, lat, t, temp, godzina_wschodu, godzina_zachodu);
			plik << "Data: " << temp.tm_mday << "." << temp.tm_mon << "." << temp.tm_year << "\n";
			plik << "Godzina wschodu: " << godzina_wschodu.hour_sunrise << " h " << godzina_wschodu.minute_sunrise << " min\n";
			plik << "Godzina zachodu: " << godzina_zachodu.hour_sunset << " h " << godzina_zachodu.minute_sunset << " min\n";
			plik.close();
			data.day++;
			temp = correct(data.day, data.month, data.year);
		} 
		while (!((temp.tm_mday == correct_end_date.tm_mday) && (temp.tm_mon == correct_end_date.tm_mon) && (temp.tm_year == correct_end_date.tm_year)));
	}
}

int main(int argc, char* argv[])
{
	struct date data_poczatkowa, data_koncowa;
	double lon, lat, t;
	string nazwa;
	godziny godzina_wschodu, godzina_zachodu;
	tm dane_dnia;
	
	if (load_params(argc, argv, nazwa, lon, lat, t, data_poczatkowa, data_koncowa))
	{
		funkcja_nadrzedna(lon, lat, t, data_poczatkowa, data_koncowa, nazwa, godzina_wschodu, godzina_zachodu, dane_dnia);
		/*tm correct_end_date = correct(data_koncowa.day, data_koncowa.month, data_koncowa.year);
		sunrise_avenue(lon, lat, t, data_poczatkowa, correct_end_date);*/
	}
    return 0;
}