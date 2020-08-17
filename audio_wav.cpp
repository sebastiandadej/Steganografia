#include "audio_wav.h"

Audio_WAV::Audio_WAV()
{
    typPliku[4]=0; //Typ pliku potrzebuje 4 bajtów, 5 jest dla /0
    RIFFType[4]=0; //4 potrzebne, jw.
    iloscKawalkow = 0;

    /*Łańcuchy znaków musza być zakończone znakiem /0, tj. zerem
     *Nazwy poszczegółnych "kawałków" są róznej długości, dla tego zerują cały łańcuch,
     * przecietnie są one długości 4 znaków, a jedynie ID3 jest "nazwą" zajmujacą jedynie trzy znaki
     * Robię wiec tak dla uproszczenia i lepszej automatyzacji tego zadania.
     * Na kazwę każdego kawałak zarezerwowane są 5 znaki, wraz ze znakiem końca łańcucha.
     */
    for( int i=0; i<10; i++ )
        for( int j=0; j<5; j++) chunks[i].ID[j]=0;  //4 potrzebne
}

bool Audio_WAV::wczytajPlik()
{
    /** Read a wav file to play audio into a buffer and return the size of the data read
     * after stripping the header.
     *
     * The header for a WAV file looks like this:
     * Positions	Sample Value	Description
     * 1 - 4	"RIFF"	Marks the file as a riff file. Characters are each 1 byte long.
     * 5 - 8	File size (integer)	Size of the overall file - 8 bytes, in bytes (32-bit integer).
     * 9 -12	"WAVE"	File Type Header. For our purposes, it always equals "WAVE".

     * 13-16	"fmt "	Format chunk marker. Includes trailing null
     * 17-20	16	Length of format data as listed above
     * 21-22	1	Type of format (1 is PCM) - 2 byte integer
     * 23-24	2	Number of Channels - 2 byte integer
     * 25-28	44100	Sample Rate - 32 byte integer. CSample Rate = Number of Samples per second, or Hertz.
     * 29-32	176400	(Sample Rate * BitsPerSample * Channels) / 8.
     * 33-34	4	(BitsPerSample * Channels) / 8.1 - 8 bit mono2 - 8 bit stereo/16 bit mono4 - 16 bit stereo
     * 35-36	16	Bits per sample
     * 37-40	"data"	"data" chunk header. Marks the beginning of the data section.
     * 41-44	File size (data)	Size of the data section.
     * Sample values are given above for a 16-bit stereo source.
     *
     * @param fileName
     * A QString representing the file location to open with QFile
     *
     * @param ramBuffer
     * A pointer to a Pre-Allocated signed short buffer
     *
     */

    //QString fileName, signed short *ramBuffer)
    // Open wave file

    qDebug() << "Otwieram plik WAV z: " << nazwa_plik;

    QFile plikWAV( nazwa_plik );
    if ( !plikWAV.open(QFile::ReadOnly) )
    {
         /*qDebug() << "Nie mozna otworzyc pliku WAV";
         if( ui->checkBox_WAV_PustyPlik->isChecked() == true )
         {
             ui->label_WAV_Komunikat->setText( "Osadzam dane w pustym pliku (!!!)" );

             // Now pop off the appropriate data into each header field defined above
             strcpy( plik_WAV.typPliku, "RIFF" );
             plik_WAV.rozmiarDanych = 2000; // Rozmiar pliku
             strcpy( plik_WAV.RIFFType, "WAVE");

             strcpy(plik_WAV.chunks[0].ID, "fmt " ); // Chunk ID -> "LIST", "fmt ", " "
             plik_WAV.chunks[0].dlugosc = 16;        // Długość fragmentu: 16 bitów standardowo

             if( QString( plik_WAV.chunks[0].ID) == "fmt " )
             {
                  plik_WAV.format.kod_kompresji = 1; //
                  plik_WAV.format.liczba_kanalow = 2; //
                  plik_WAV.format.czestotliwosc_probkowania = 44100; //
                  plik_WAV.format.przecietna_ilosc_bitow_probka = 176400; //
                  plik_WAV.format.wyrownanie_bloku = 4; //
                  plik_WAV.format.znaczace_bity_probka = 16; //
                  plik_WAV.format.dodatkowe_bajty = 0;

                  plik_WAV.format.size = plik_WAV.chunks[0].dlugosc; //
             }

             strcpy( plik_WAV.chunks[1].ID, "data");
             plik_WAV.chunks[1].dlugosc = 0; // !!!!

             qDebug() << "Drugi kawałek: " << plik_WAV.chunks[1].ID;
             qDebug() << "Rozmiar: " << plik_WAV.chunks[1].dlugosc;

             if( QString(plik_WAV.chunks[1].ID) == "data" )
             {
                 plik_WAV.dane.dane.resize( plik_WAV.chunks[1].dlugosc );
                 for( int i=0; i< plik_WAV.dane.dane.size(); i++)
                 {
                     plik_WAV.dane.dane[i]=0;
                 }
             }

             strcpy( plik_WAV.chunks[2].ID, "");
             plik_WAV.chunks[2].dlugosc = 0;

             qDebug() << "Trzeci kawałek: " << plik_WAV.chunks[2].ID;

             for( int i=0; i<300; i++ )
             {
                 char znak[10];
                 znak[1] = 0;
                 //strumienDanychWAV.readRawData( znak, 1  );
                 //qDebug() << "Znak: " << znak;
             }
       }


             plik_WAV.wyswietl_naglowek();
             plik_WAV.wyswietl_fmt();
             plik_WAV.wyswietl_opis();
         }  */
          return false; // Nie ma pliku

    }

    // Odczytanie wszystkich bitów z pliku.
    QByteArray wavFileContent = plikWAV.readAll();

    qDebug() << "Rozmiar pliku WAV: " << wavFileContent.size();

    // Tworzenie strumienia danych w celu analizy danych
    QDataStream strumienDanychWAV(&wavFileContent,QIODevice::ReadOnly);
    strumienDanychWAV.setByteOrder(QDataStream::LittleEndian);

    // Zczytanie nagłówka
    strumienDanychWAV.readRawData( typPliku,4); // "RIFF"
    strumienDanychWAV >> rozmiarDanych; // File Size
    strumienDanychWAV.readRawData( RIFFType,4); // "WAVE"

    char kawalek[5];
    int rozmiarKawalka;

    for(int nrKawalka = 0; strumienDanychWAV.atEnd() != true; nrKawalka++, iloscKawalkow++ )
    {
        strumienDanychWAV.readRawData( kawalek, 4); // Chunk ID -> "LIST", "fmt ", "data", itd.
        strumienDanychWAV >> rozmiarKawalka;        // Długość kawałaka

        if( QString(kawalek) == "fmt " ) // 0x020746d66
        {
            strcpy( chunks[ nrKawalka ].ID, "fmt ");
            chunks[ nrKawalka ].dlugosc = rozmiarKawalka;
            format.size = rozmiarKawalka; //

            strumienDanychWAV >> format.kod_kompresji; //
            strumienDanychWAV >> format.liczba_kanalow; //
            strumienDanychWAV >> format.czestotliwosc_probkowania; //
            strumienDanychWAV >> format.przecietna_ilosc_bitow_probka; //
            strumienDanychWAV >> format.wyrownanie_bloku; //
            strumienDanychWAV >> format.znaczace_bity_probka; //
            if(  chunks[ nrKawalka ].dlugosc != 16 )
                strumienDanychWAV >> format.dodatkowe_bajty; //
              else format.dodatkowe_bajty = 0;

            continue;
        }

        if( QString(kawalek) == "data" ) // 0x61746164
        {
            strcpy( chunks[ nrKawalka ].ID, kawalek );
            chunks[ nrKawalka ].dlugosc = rozmiarKawalka ;

            dane.size = rozmiarKawalka;
            dane.dane.resize( chunks[ nrKawalka ].dlugosc );
            strumienDanychWAV.readRawData( dane.dane.begin(), rozmiarKawalka );

            continue;
        }

        if( QString(kawalek) == "LIST" ) //
        {
            strcpy( chunks[ nrKawalka ].ID, kawalek );
            chunks[ nrKawalka ].dlugosc = rozmiarKawalka ;

            tekst.size = rozmiarKawalka;
            tekst.dane.resize( rozmiarKawalka );
            strumienDanychWAV.readRawData( tekst.dane.begin(), tekst.size );

            continue;
        }

        if( QString(kawalek) == "id3 " )
        {
            strcpy( chunks[ nrKawalka ].ID, kawalek );
            chunks[ nrKawalka ].dlugosc = rozmiarKawalka ;

            id3.size = rozmiarKawalka;
            id3.dane.resize( rozmiarKawalka );
            strumienDanychWAV.readRawData( id3.dane.begin(), id3.size );

            continue;
        }

        //else jakieś inne kawałki pliku WAV
        //strcpy( chunks[ nrKawalka ].ID, kawalek );
        //chunks[ nrKawalka ].dlugosc = rozmiarKawalka ;

        //chunks[ nrKawalka ].bufor.resize( rozmiarKawalka );
        //strumienDanychWAV.readRawData( chunks[ nrKawalka ].bufor.begin(), rozmiarKawalka );
    }


    qDebug() << "W pliku było: " << iloscKawalkow << "  kawałków.";

    for( int i=0; i <iloscKawalkow; i++ )
    {
        qDebug() << "Kawałek: " << chunks[ i ].ID;
        qDebug() << "Rozmiar: " << chunks[ i ].dlugosc << endl;
    }

    wyswietl_naglowek();
    wyswietl_fmt();
    wyswietl_opis();
    wyswietl_id3();

    plikWAV.close();
}



bool Audio_WAV::zapiszPlik()
{
    QFile plik(nowa_nazwa_pliku);
    plik.open( QFile::ReadWrite );

    QByteArray buffor;
    QDataStream strumienDanych( &buffor, QIODevice::WriteOnly );
    strumienDanych.setByteOrder( QDataStream::LittleEndian );

    strumienDanych.writeRawData( typPliku,4); // "RIFF"
    strumienDanych << rozmiarDanych; // File Size
    strumienDanych.writeRawData( RIFFType,4); // "WAVE"

    for( int i=0; i<iloscKawalkow; i++ )
    {
        strumienDanych.writeRawData( chunks[i].ID,4 ); // Chunk ID -> "LIST", "fmt ", " "
        strumienDanych << chunks[i].dlugosc;        // Chunk length

        if( QString( chunks[i].ID) == "fmt " )
        {
            strumienDanych << format.kod_kompresji; //
            strumienDanych << format.liczba_kanalow; //
            strumienDanych << format.czestotliwosc_probkowania; //
            strumienDanych << format.przecietna_ilosc_bitow_probka; //
            strumienDanych << format.wyrownanie_bloku; //
            strumienDanych << format.znaczace_bity_probka; //
            if(  chunks[i].dlugosc != 16 )
                strumienDanych << format.dodatkowe_bajty; //
            else {} //plik_WAV.format.dodatkowe_bajty = 0;

            continue;
        }


       if( QString( chunks[i].ID) == "data" )
       {
           strumienDanych.writeRawData( dane.dane.begin(), dane.size);

           continue;
       }

       if( QString(chunks[i].ID) == "LIST" ) //
       {
           strumienDanych.writeRawData( tekst.dane.begin(), tekst.size );

           continue;
       }

       if( QString(chunks[i].ID) == "id3 " )
       {
           strumienDanych.writeRawData( id3.dane.begin(), id3.size );

           continue;
       }

       //else inne kawałki
        strumienDanych.writeRawData( chunks[i].bufor.data(), chunks[i].dlugosc) ;
    }

    plik.write(buffor);
    plik.close();
}

void Audio_WAV::wyswietl_naglowek()
{
    qDebug() << "Nagłówek pliku WAV: ";
    qDebug() << "Typ pliku: " << typPliku;
    qDebug() << "Rozmiar Danych w Pliku (Rozmair pliku - 8 bitów): " << rozmiarDanych;
    qDebug() << "RIFF Type/WAV Marker: " << RIFFType << endl;
}

void Audio_WAV::wyswietl_fmt()
{
     qDebug() << "Kawałek - FORMAT - fmt: ";
     qDebug() << "Rozmiar tego kawałka: " << format.size;
     qDebug() << "Kod kompresji: " << format.kod_kompresji;
     qDebug() << "Liczba kanałów: " << format.liczba_kanalow;
     qDebug() << "Częstotliwość próbkowania: " << format.czestotliwosc_probkowania << " Hz";
     qDebug() << "Przeciętna ilośc bitów na próbkę: " << format.przecietna_ilosc_bitow_probka;
     qDebug() << "Wyrównainie bloku: " << format.wyrownanie_bloku;
     qDebug() << "Znaczące bity na próbkę: " << format.znaczace_bity_probka;
     qDebug() << "Dodatkowe bajty: " << format.dodatkowe_bajty << endl;
}

void Audio_WAV::wyswietl_opis()
{
    qDebug() << "Kawałek - LIST - :";
    qDebug() << "Rozmiar: " << tekst.size;

    str_list temp = tekst;
    for( int i=0; i<temp.size; i++ )
     if( temp.dane[i] == 0 ) temp.dane[i] = 60;
    qDebug() << "Zawarty tekst to: " << temp.dane;
}

void Audio_WAV::wyswietl_id3()
{
    qDebug() << "Kawałek - id3 - :";
    qDebug() << "Rozmiar: " << id3.size;

    str_id3 temp = id3;
    for( int i=0; i<temp.size; i++ )
     if( temp.dane[i] == 0 ) temp.dane[i] = 60;
    qDebug() << "Tekst: " << temp.dane << endl;
}
