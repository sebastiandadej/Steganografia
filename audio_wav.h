#include <QString>
#include <QDebug>
#include <QFile>

#ifndef AUDIO_WAV_H
#define AUDIO_WAV_H

struct str_short_int
{
    int a : 1;
    int b : 1;
    int c : 1;
    int d : 1;
    int e : 1;
    int f : 1;
    int g : 1;
    int h : 1; // 8 bitów
    int i : 1;
    int j : 1;
    int k : 1;
    int l : 1;
    int m : 1;
    int n : 1;
    int o : 1;
    int p : 1; // 8 bitów

};

union unio_short_int
{
    short int liczba;
    str_short_int pola_short_int;
    char liczba_bajty[2];
};

struct str_int
{
    int a : 1;
    int b : 1;
    int c : 1;
    int d : 1;
    int e : 1;
    int f : 1;
    int g : 1;
    int h : 1; // 8 bitów
    int i : 1;
    int j : 1;
    int k : 1;
    int l : 1;
    int m : 1;
    int n : 1;
    int o : 1;
    int p : 1; //8 bitów

    int a2 : 1;
    int b2 : 1;
    int c2 : 1;
    int d2 : 1;
    int e2 : 1;
    int f2 : 1;
    int g2 : 1;
    int h2 : 1; // 8 bitów
    int i2 : 1;
    int j2 : 1;
    int k2 : 1;
    int l2 : 1;
    int m2 : 1;
    int n2 : 1;
    int o2 : 1;
    int p2 : 1; //8 bitów
};

union unio_int
{
    int liczba;
    char bajty[4];
    str_int bity;
};


inline void zapisz( unio_int &x, int y, bool wartosc )
{
    switch( y )
    {
    case 0:
        x.bity.a = wartosc; return;
    case 1:
        x.bity.b = wartosc; return;
    case 2:
        x.bity.c = wartosc; return;
    case 3:
        x.bity.d = wartosc; return;
    case 4:
        x.bity.e = wartosc; return;
    case 5:
        x.bity.f = wartosc; return;
    case 6:
        x.bity.g = wartosc; return;
    case 7:
        x.bity.h = wartosc; return; //8

    case 8:
        x.bity.i = wartosc; return;
    case 9:
        x.bity.j = wartosc; return;
    case 10:
        x.bity.k = wartosc; return;
    case 11:
        x.bity.l = wartosc; return;
    case 12:
        x.bity.m = wartosc; return;
    case 13:
        x.bity.n = wartosc; return;
    case 14:
        x.bity.o = wartosc; return;
    case 15:
        x.bity.p = wartosc; return; //8


    case 16:
        x.bity.a2 = wartosc; return;
    case 17:
        x.bity.b2 = wartosc; return;
    case 18:
        x.bity.c2 = wartosc; return;
    case 19:
        x.bity.d2 = wartosc; return;
    case 20:
        x.bity.e2 = wartosc; return;
    case 21:
        x.bity.f2 = wartosc; return;
    case 22:
        x.bity.g2 = wartosc; return;
    case 23:
        x.bity.h2 = wartosc; return; //8

    case 24:
        x.bity.i2 = wartosc; return;
    case 25:
        x.bity.j2 = wartosc; return;
    case 26:
        x.bity.k2 = wartosc; return;
    case 27:
        x.bity.l2 = wartosc; return;
    case 28:
        x.bity.m2 = wartosc; return;
    case 29:
        x.bity.n2 = wartosc; return;
    case 30:
        x.bity.o2 = wartosc; return;
    case 31:
        x.bity.p2 = wartosc; return; //8
    }
}

inline bool pobierz( unio_int x, int y )
{
    switch( y )
    {
    case 0:
        return x.bity.a;
    case 1:
        return x.bity.b;
    case 2:
        return x.bity.c;
    case 3:
        return x.bity.d;
    case 4:
        return x.bity.e;
    case 5:
        return x.bity.f;
    case 6:
        return x.bity.g;
    case 7:
        return x.bity.h; //8

    case 8:
        return x.bity.i;
    case 9:
        return x.bity.j;
    case 10:
        return x.bity.k;
    case 11:
        return x.bity.l;
    case 12:
        return x.bity.m;
    case 13:
        return x.bity.n;
    case 14:
        return x.bity.o;
    case 15:
        return x.bity.p; //8


    case 16:
        return x.bity.a2;
    case 17:
        return x.bity.b2;
    case 18:
        return x.bity.c2;
    case 19:
        return x.bity.d2;
    case 20:
        return x.bity.e2;
    case 21:
        return x.bity.f2;
    case 22:
        return x.bity.g2;
    case 23:
        return x.bity.h2; //8

    case 24:
        return x.bity.i2;
    case 25:
        return x.bity.j2;
    case 26:
        return x.bity.k2;
    case 27:
        return x.bity.l2;
    case 28:
        return x.bity.m2;
    case 29:
        return x.bity.n2;
    case 30:
        return x.bity.o2;
    case 31:
        return x.bity.p2; //8
    }
}

struct str_fmt
{
    int size;
    short int kod_kompresji;
    short int liczba_kanalow;
    int czestotliwosc_probkowania;
    int przecietna_ilosc_bitow_probka;
    short int wyrownanie_bloku;
    short int znaczace_bity_probka;
    short int dodatkowe_bajty;
};

struct str_chunk
{
    char ID[5];
    int dlugosc;
    QByteArray bufor;
};

struct str_data
{
    int size;
    QByteArray dane;
};
struct str_list
{
    int size;
    QByteArray dane;
};

struct str_id3
{
    int size;
    QByteArray dane;
};

class Audio_WAV
{
public:
    QString nazwa_plik;
    QString nowa_nazwa_pliku;
    int rozmiarDanych;
    char typPliku[5] = {0,0,0,0,0};
    char RIFFType[5] = {0,0,0,0,0};

    str_fmt format;
    str_data dane;
    str_list tekst;
    str_id3 id3;
    str_chunk chunks[10];
    int iloscKawalkow;

    //FUNKCJE
    Audio_WAV();

    bool wczytajPlik();
    bool zapiszPlik();

    void wyswietl_fmt();
    void wyswietl_naglowek();
    void wyswietl_opis();
    void wyswietl_id3();
};

#endif // AUDIO_WAV_H
