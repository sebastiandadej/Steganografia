#include <QDebug>

#ifndef PLIK_JPEG_H
#define PLIK_JPEG_H

class Plik_JPEG
{
public:
    Plik_JPEG();

    //
    char typKontenera[5]; //Always equals "JFIF" (with zero following) (0x4A46494600)
    short int wersja;
    char densityUnits[2]; //I tak wykozystujemy tylko jeden znak
    short int densityX, densityY;
    char rozmairMiniatury[3];


    //Komentarz
    short int rozKomen;
    char komentarz[100];

    //Macierz kwantyzacji
    char macierzKwan1[64]; // 8 x 8
    char macierzKwan2[64]; // 8 x 8

    //Bazeline czy Progresywny DCT
    char infoDCT[20];

    //Tabela Huffmana
    char tabHuffman1[100];
    char tabHuffman2[100];
};

#endif // PLIK_JPEG_H
