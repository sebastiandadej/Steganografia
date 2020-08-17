#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

#include <stdio.h>
#include <stdint.h>
#include <stdio.h>
#include "jpeg/jpeglib.h"
#include <setjmp.h>


#include <math.h>
#include <stdio.h>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //RESECIKI
    plikDoOsadzenia = NULL;
    player = NULL;
    obrazBMP = NULL;
    obrazBMPZmody = NULL;

    checkBoxRedIlosc=0; //Wartości początkowe, domyslne
    checkBoxGreenIlosc=0;
    checkBoxBlueIlosc=0;

    checkBoxy = new QCheckBox*[ 24 ];

    infOAutorze = "Sebastian Dadej\nStudent UMCS\nProgram jest częścią pracy licencjackiej\n";
    ui->label_InfOAutorze->setText( infOAutorze );

    //MAPOWANIE QCheckBox'ow z GUI na QChecBoxy z mojego programu

    checkBoxy[16] = ui->checkBoxR1;
    checkBoxy[17] = ui->checkBoxR2;
    checkBoxy[18] = ui->checkBoxR3;
    checkBoxy[19] = ui->checkBoxR4;
    checkBoxy[20] = ui->checkBoxR5;
    checkBoxy[21] = ui->checkBoxR6;
    checkBoxy[22] = ui->checkBoxR7;
    checkBoxy[23] = ui->checkBoxR8;

    checkBoxy[8] = ui->checkBoxG1;
    checkBoxy[9] = ui->checkBoxG2;
    checkBoxy[10] = ui->checkBoxG3;
    checkBoxy[11] = ui->checkBoxG4;
    checkBoxy[12] = ui->checkBoxG5;
    checkBoxy[13] = ui->checkBoxG6;
    checkBoxy[14] = ui->checkBoxG7;
    checkBoxy[15] = ui->checkBoxG8;

    checkBoxy[0] = ui->checkBoxB1;
    checkBoxy[1] = ui->checkBoxB2;
    checkBoxy[2] = ui->checkBoxB3;
    checkBoxy[3] = ui->checkBoxB4;
    checkBoxy[4] = ui->checkBoxB5;
    checkBoxy[5] = ui->checkBoxB6;
    checkBoxy[6] = ui->checkBoxB7;
    checkBoxy[7] = ui->checkBoxB8;
}

void MainWindow::cialo() //Głowna funkcja
{
    qDebug() << "[SEBA] 1";

    //BMP
    connect( ui->guzikWczytajBMP, SIGNAL( clicked() ), this, SLOT( slotWyborBazowegoBMP()) );
    connect( ui->guzikWczytajPlik, SIGNAL( clicked() ), this, SLOT( slotWyborPlikuDoOsadzenia()) );
    connect( ui->guzik_KonwertujDoJPEG, SIGNAL( clicked() ), this, SLOT( slotKonwertujDoJPEG() ));

    connect( ui->guzikOsadzajWiadonosc, SIGNAL( clicked() ), this, SLOT( slotOsadzajWiadomoscBMP()) );
    connect( ui->guzikOdczytajWiadomosc, SIGNAL( clicked() ), this, SLOT( slotOdczytajWiadomoscBMP()) );
    connect( ui->guzikZapisz, SIGNAL( clicked() ), this, SLOT( slotZapiszBMP() ));
    connect( ui->guzik_zapiszJakoJPEG, SIGNAL( clicked() ), this, SLOT( slotZapiszJakoJPEG() ));

    //WAV
    connect( ui->guzikOdtworzOryginal, SIGNAL( clicked() ), this, SLOT( slotOdtworzAudio()) );
    connect( ui->suwak_Glosnosc, SIGNAL( sliderMoved(int) ), this, SLOT( slotZmienGlosnoscWAV()) );
    connect( ui->guzik_stop, SIGNAL( clicked()), this, SLOT( slotStopAudio()) );
    connect( ui->guzikWczytajWav, SIGNAL( clicked() ), this, SLOT( slotWyborPlikuWAV()) );
    connect( ui->guzikOsadzWiadomoscWav, SIGNAL( clicked() ), this, SLOT( slotOsadzajWAV()) );
    connect( ui->guzik_WAV_OdczytajWiadomosc, SIGNAL( clicked() ), this, SLOT( slotOdczytajWiadomoscWAV()) );
    connect( ui->guzik_zapisz_WAV, SIGNAL( clicked() ), this, SLOT( slotZapiszWAV() ) );
    connect( ui->guzik_WAV_WczytajPlikDoOsadzenia, SIGNAL( clicked() ), this, SLOT( slotWyborPlikuDoOsadzenia() ));

    this->show();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete[] checkBoxy;

    if( player!=NULL ) delete player;
    if( plikDoOsadzenia != NULL) delete plikDoOsadzenia;
    if( obrazBMP != NULL ) delete obrazBMP;
    if( obrazBMPZmody != NULL ) delete  obrazBMPZmody;
}


//FUNKCJE
//Tekst, Wiadomosc
void MainWindow::slotWyborPlikuDoOsadzenia()
{
    QString TNazwaPliku = QFileDialog::getOpenFileName( );
    if( TNazwaPliku == "" )
    {
        ui->label_BMP_Komunikat->setText( "Tym razem nie wybrano pliku." );
        ui->label_WAV_Komunikat->setText( "Tym razem nie wybrano pliku." );
        return;
    }

    if( plikDoOsadzenia!=NULL ) { plikDoOsadzenia->close(); delete plikDoOsadzenia; }
    plikDoOsadzenia = new QFile();
    plikDoOsadzenia->setFileName(TNazwaPliku);
    plikDoOsadzenia->open(QIODevice::ReadWrite);
    //plikDoOsadzenia->size();

    wiadomosc = plikDoOsadzenia->readAll();

    QString komunikat;
    komunikat = QString("Rozmiar pliku do osadzenia: %1 \n").arg(wiadomosc.size());
    ui->label_WAV_Inf_Osadzane->setText( komunikat );
}

//BMP
void MainWindow::slotWyborBazowegoBMP()
{
        nazwaPliku = QFileDialog::getOpenFileName(this, tr("Open file"), "c:\\" , tr("Images (*.png *.jpg *bmp)"));
        if( nazwaPliku == "" ) return;

        if( obrazBMP != NULL ) delete obrazBMP;
        obrazBMP = new QImage();
        obrazBMP->load(nazwaPliku); //jaki plik

        ui->labelObrazekPrzed->setPixmap( QPixmap::fromImage( *obrazBMP) );
        ui->labelObrazekPo->setPixmap( QPixmap::fromImage( *obrazBMP ) );

        //Zakładka informacje o nośniku
        QString temp;
        temp = "Nazwa pliku: " + nazwaPliku + "\n" + QString( "Wysokość: %1 \nSzerokość: %2\n" ).arg( obrazBMP->height() ).arg( obrazBMP->width() );

        ui->labelInfONosniku->setText(temp);
}


void MainWindow::slotOsadzajWiadomoscBMP()
{
    //BMP
    ui->label_BMP_Komunikat->setText( "Osadzam wiadomość" );
    QImage *obrazTemp = new QImage;
    if( obrazBMP!=NULL ) *obrazTemp = *obrazBMP;
     else { ui->label_BMP_Komunikat->setText("Brak OBRAZKA!"); return; }


    //Które bity w CIELE/PIKSELACH modyfkować
    int tabModBit[24];
    int rozTab=0;
    for( int i=0; i<24; i++)
    {
       if( checkBoxy[i]->isChecked() == true )  tabModBit[rozTab++] = i;

    }

    for( int i=0; i<rozTab; i++)
    {
        qDebug() << "Tablica zapis: " << tabModBit[i];
    }

    if( rozTab == 0 )
    {
        ui->label_BMP_Komunikat->setText( "Nie wybrano bitów przezanczonych na miejsce wiadomosci." );
        return;
    }

    unio_int liczba;
    QBitArray bufor;
    int nrBitu=0;
    bufor.resize( 32 + 160 + wiadomosc.size() * 8); //rozmair, hash, wiadomość

    //BUFOROWANIE DANYCH
    // -- Rozmiar
    liczba.liczba = wiadomosc.size();
    for (int i=0; i<32; i++) bufor[ nrBitu++ ] = pobierz( liczba, i );
    qDebug() << "Rozmiar wiadomości: " << liczba.liczba;

    // -- Hash - wyliczanie
    QCryptographicHash hash( QCryptographicHash::Sha1 );
    hash.addData( wiadomosc );
    QByteArray hashTEMP;
    hashTEMP = hash.result();
    qDebug() << "Rozmair hasu: " << hashTEMP.size();
    qDebug() << "Hash: " << hash.result();

    // -- Hash - bity
    for( int i=0; i<hashTEMP.size(); i++)
    {
        liczba.liczba = hashTEMP[i];
        for( int k=0; k<8; k++ ) bufor[ nrBitu++ ] = pobierz(liczba, k); //k-ty bit z liczby
    }

    qDebug() << "Po rozmairze i hasu: " << nrBitu;

    // -- WIADOMOSCI - bity
    qDebug() << "Wiadomość - zapis: " << wiadomosc;

    for(int i=0; i<wiadomosc.size(); i++)
    {
        liczba.liczba = wiadomosc[i]; //8 bitów
        for(int k=0; k<8; k++) bufor[ nrBitu++ ]=pobierz( liczba, k);
    }

    qDebug() << "Po rozmairze i hasu i wiadomosci: " << nrBitu;

    //ZAPIS DANYCH

    //METADANE //rozmiar, hash
    //DANE //Wiaodmość
    nrBitu=0;
    qDebug() << "Rozmair bufora: " << bufor.size();
    for( int sze=0, wy=0; nrBitu<bufor.size() && sze<obrazTemp->width() && wy < obrazTemp->height(); )
    {
        liczba.liczba = obrazTemp->pixel(sze, wy);

        for( int i=0; i<rozTab && nrBitu<bufor.size(); i++ )
        {
            zapisz( liczba, tabModBit[i], bufor[ nrBitu ] );
            nrBitu++;
            //qDebug() << QString("Osadzony bit [%1]: ").arg(nrBitu) << bufforWiadomosci[ i++ ];
        }

            qDebug() << "\n Piksel: " << sze << ", " << wy;
            qDebug() << QString("Wartość piksela przed:  %1\n Wartośc po:             %2").arg( liczba.liczba,0,2 ).arg( obrazTemp->pixel( sze, 0 ),0,2 );

        obrazTemp->setPixel( sze, wy, liczba.liczba );
        this->show(); //update()

        //Post produkcja
        sze++;
        if( sze == obrazTemp->width() ) { sze=0; wy++; }
    }

    komunikat = QString( "W pliku BMP zmiesciło się %1/%2 bitów danych. ").arg( nrBitu ).arg( bufor.size() );
    ui->label_BMP_Komunikat->setText( komunikat );

    if( obrazBMPZmody == NULL ) obrazBMPZmody = new QImage();

    *obrazBMPZmody = *obrazTemp; //Zmodyfikowany obraz będzie dostępny po za funkcją
    ui->labelObrazekPo->setPixmap( QPixmap::fromImage(*obrazBMPZmody) );
}



void MainWindow::slotOdczytajWiadomoscBMP()
{
    //BMP
    if( obrazBMP!=NULL ) ui->label_BMP_Komunikat->setText( "Odczytuje wiadomość" );
      else ui->label_BMP_Komunikat->setText("Brak OBRAZKA!");

    // BUFOROWANIE DANYCH
    unio_int liczba, liczba2;
    QString komunikat;
    QBitArray bufor;
    wiadomosc.resize(0);
    int rozmiarWiadomosci=0;

    // -- BITY ZAPISU DANYCH
    int tabModBity[24];
    int rozTab=0;
    for( int i=0; i<24; i++) if( checkBoxy[i]->isChecked() == true ) tabModBity[ rozTab++ ] = i;

    for( int i=0; i<rozTab; i++)
    {
        qDebug() << "Tablica odczyt: " << tabModBity[i];
    }

    if( rozTab == 0 )
    {
        ui->label_BMP_Komunikat->setText( "Nie wybrano bitów!" );
        return;
    }

    // -- Rozmair i HASH - odczyt z pliku
    int sze=0, wy=0;
    int nrBitu=0;
    int granica = 32+160;
    bufor.resize( granica );
    liczba2.liczba = 0;

    for( ; nrBitu < granica ; )
    {
        liczba.liczba = obrazBMP->pixel(sze, wy);
        for( int j=0; j<rozTab && nrBitu < granica; j++ )
        {
            bufor[ nrBitu++ ] = pobierz( liczba, tabModBity[j] ); //Jeden piksel
            if( nrBitu == 32 ) //odczytujemy rozmair i rozszeżamy granicę
            {
                for( int k=0; k<32; k++ ) zapisz( liczba2, k, bufor[ k ] );
                rozmiarWiadomosci = liczba2.liczba;

                //ROZMIAR -- czy odczytano właściwy rozmiar
                if( rozmiarWiadomosci < 0 || rozmiarWiadomosci > ( obrazBMP->width() * obrazBMP->height() ) )
                {
                    ui->label_BMP_Komunikat->setText( "Obrazek nie zawiera wiadomości. Zły rozmiar." );
                    return;
                }
                granica += rozmiarWiadomosci * 8; // Bo rozmair był w bajtach, a bufor jest bitowy;
                bufor.resize( granica );
            }
        }

        sze++;
        if( sze == obrazBMP->width()  ) { sze=0; wy++; }
    }

        komunikat = QString("Wiadomość ma rozmiar: %1\n").arg(rozmiarWiadomosci);
        ui->label_BMP_Komunikat->setText( komunikat );

        qDebug() << "Odczytano bitów: " << nrBitu;

    // -- HASH - z bitów
    nrBitu = 32;
    char hashTEMP[21]; hashTEMP[20] = '\0'; //Znak końca stringa musi być.
    for( int i=0; i<20; i++)
    {
        for( int j=0; j<8; j++, nrBitu++ ) zapisz( liczba, j, bufor[ nrBitu ] );
        hashTEMP[i] = liczba.bajty[0];
    }

    qDebug() << "Hash z pliku: " << hashTEMP;

    // -- ODCZYTYWANIE WIADOMOSCI
    // WIADOMOŚĆ - z bitów
    char wiadomoscTEMP[ rozmiarWiadomosci ];
    //nrBitu = 32+160;
    for( int i=0; i<rozmiarWiadomosci; i++ )
    {
        for( int k=0; k<8; k++, nrBitu++ ) zapisz(liczba, k, bufor[ nrBitu ]);
        wiadomoscTEMP[i] = liczba.bajty[0];
    }

    qDebug() << "Wiadomość: " << wiadomoscTEMP;

    //HASH
    QCryptographicHash hash( QCryptographicHash::Sha1);
    hash.addData( wiadomoscTEMP, rozmiarWiadomosci );

    QString jakis ( hash.result() );
    komunikat += QString("Hash wiaodmosci: %1 \n").arg( jakis );
    QString jakis2( hashTEMP );
    komunikat += QString("Hash odnaleźiony w pliku: %1 \n").arg( jakis2 );

    if( hash.result() == hashTEMP )
    {
        komunikat += QString( "Hashe są zgodne" );
        //ui->label_InfOOsadzanychDanych->setText( komunikat );
    } else
    {
        komunikat += QString( "Hashe są niezgodne" );
        //ui->label_InfOOsadzanychDanych->setText( komunikat );
    }


    QFile plik;
    plik.setFileName("BMP_wyjsciowy.txt");
    plik.open( QIODevice::WriteOnly );
    plik.write( wiadomoscTEMP, rozmiarWiadomosci );
    plik.close();

    komunikat += " Odczytano wiaodmość. ";
    ui->label_BMP_Komunikat->setText(komunikat);
}


void MainWindow::slotZapiszBMP()
{
    QString nowaNazwa = QFileDialog::getSaveFileName(this, tr("Pod jaką nazwą zapisać plik?"), "c:\\" , tr("Images (*.png *.jpg *bmp)"));

    if( obrazBMPZmody!=NULL && nowaNazwa != "" )
    {
        obrazBMPZmody->save( nowaNazwa);
        QString temp;
        temp = "Zapisano plik pod ścieszką: " + nowaNazwa;
        ui->label_BMP_Komunikat->setText( temp );
    }
    //this->update();
    //ui->graphicsView->show();

}



void MainWindow::slotZapiszJakoJPEG()
{
    QString nowaNazwa = QFileDialog::getSaveFileName(this, tr("Pod jaką nazwą zapisać plik?"), "c:\\" , tr("Images (*.png *.jpg *bmp)"));

    if( obrazBMPZmody!=NULL && nowaNazwa != "" )
    {
        obrazBMPZmody->save( nowaNazwa, ".jpeg", ui->suwak_jakoscJPEG->value() );
        QString temp;
        temp = "Zapisano plik jako JPEG pod ścieszką: " + nowaNazwa;
        ui->label_BMP_Komunikat->setText( temp );
    }
    else ui->label_BMP_Komunikat->setText( "Brak pliku przetworzonego, lub zła ścieszka zapisu" );
}


//WAV
void MainWindow::slotWyborPlikuWAV()
{
        plik_WAV.nazwa_plik = QFileDialog::getOpenFileName(this, tr("Wybierz plik audio"), "c:\\" , tr("Audio (*.wav)"));
        plik_WAV.wczytajPlik();

        QString komunikat;
        komunikat = QString("Plik oryginalny: %1\n").arg( plik_WAV.nazwa_plik );
        ui->label_Inf_WAV->setText( komunikat );


        //plikWAV->load(nazwaPliku); //jaki plik

        //this->update();
        //ui->graphicsView->show();
        //lqDebug() << plik.size();
}


void MainWindow::slotOdtworzAudio()
{
    if( player == NULL ) player = new QMediaPlayer;

    if( player->state() != QMediaPlayer::PlayingState )
    {
        player->setMedia( QUrl::fromLocalFile(plik_WAV.nazwa_plik) );
        player->setVolume( ui->suwak_Glosnosc->value() );
        player->play();
        ui->label_WAV_Komunikat->setText("Odtwarzam Oryginalne audio.");
    }
    else
    {
        ui->label_WAV_Komunikat->setText("Już odtwarzam oryginalne audio.");
    }
}

void MainWindow::slotStopAudio()
{
    player->stop();
}

void MainWindow::slotOdtworzOsadzoneAudio()
{
    if( player == NULL ) player = new QMediaPlayer;

    player->stop();
    player->setMedia( QUrl::fromLocalFile( plik_WAV.nazwa_plik ) );
    player->setVolume( ui->suwak_Glosnosc->value() );
    player->play();
    ui->label_WAV_Komunikat->setText("Odtwarzam Zmodyfikowane audio.");
}


void MainWindow::slotZmienGlosnoscWAV()
{
    if( player != NULL )
    {
        player->setVolume( ui->suwak_Glosnosc->value() );
        ui->lcd_glosnosc->display( ui->suwak_Glosnosc->value() );
    }
}


// https://ccrma.stanford.edu/courses/422/projects/WaveFormat/
void MainWindow::slotOsadzajWAV()
{
    ui->label_WAV_Komunikat->setText("Osadzam wiadomosc.");

    QBitArray bufor;
    unio_int liczba;
    int nrBitu;
    QString komunikat;
    bufor.resize( 32 + 160 + wiadomosc.size() * 8 ); //Bajty razy rozmair bajta

    //unio_int bufor[ 4 + wiadomosc.size() + 20 ]; //Wiadomość + HASH


    //BUFOROWANIE DANYCH

    // -- Rozmair
    liczba.liczba = wiadomosc.size(); //Rozmiar wiadomości
    nrBitu = 0;
    for( int j=0; j<32; j++ ) bufor[ nrBitu++ ] = pobierz( liczba, j);
    komunikat = QString("Rozmair wiadomości: %1\n").arg( liczba.liczba );

    // -- Hash
    QCryptographicHash hash( QCryptographicHash::Sha1);
    hash.addData( wiadomosc, wiadomosc.size() );
    qDebug() << "Hash wiaodmosci: " << hash.result();

    for( int i=0, j=0; i<20; i++, j++ )
    {
        liczba.liczba = hash.result()[i];
        for( int j=0; j<8; j++) bufor[ nrBitu++] = pobierz(liczba, j);
    }

    // -- Wiadomość
    for( int i=0; i<wiadomosc.size(); i++ )
    {
        liczba.liczba = wiadomosc[i];
        for( int j=0; j<8; j++) bufor[ nrBitu++ ] = pobierz(liczba, j);
    }

    //qDebug() << "Wartość z buf - Liczba: " << bufor[1].liczba;
    /*qDebug() << "Wartosc z buf - bit: " << bufor[1].bity.a;
    qDebug() << "Wartosc z buf - bit: " << bufor[1].bity.b;
    qDebug() << "Wartosc z buf - bit: " << bufor[1].bity.c;
    qDebug() << "Wartosc z buf - bit: " << bufor[1].bity.d;
    qDebug() << "Wartosc z buf - bit: " << bufor[1].bity.e;
    qDebug() << "Wartosc z buf - bit: " << bufor[1].bity.f;
    qDebug() << "Wartosc z buf - bit: " << bufor[1].bity.g;
    qDebug() << "Wartosc z buf 8 bit: " << bufor[1].bity.h;
    qDebug() << "Wartosc z buf - bit: " << bufor[1].bity.i;
    qDebug() << "Wartosc z buf - bit: " << bufor[1].bity.j;
    qDebug() << "Wartosc z buf - bit: " << bufor[1].bity.k;
    qDebug() << "Wartosc z buf - bit: " << bufor[1].bity.l;
    qDebug() << "Wartosc z buf - bit: " << bufor[1].bity.m;
    qDebug() << "Wartosc z buf - bit: " << bufor[1].bity.n;
    qDebug() << "Wartosc z buf - bit: " << bufor[1].bity.o;
    qDebug() << "Wartosc z buf 8 bit: " << bufor[1].bity.p;

    qDebug() << "Wartosc z buf - bit: " << bufor[1].bity.a2;
    qDebug() << "Wartosc z buf - bit: " << bufor[1].bity.b2;
    qDebug() << "Wartosc z buf - bit: " << bufor[1].bity.c2;
    qDebug() << "Wartosc z buf - bit: " << bufor[1].bity.d2;
    qDebug() << "Wartosc z buf - bit: " << bufor[1].bity.e2;
    qDebug() << "Wartosc z buf - bit: " << bufor[1].bity.f2;
    qDebug() << "Wartosc z buf - bit: " << bufor[1].bity.g2;
    qDebug() << "Wartosc z buf 8 bit: " << bufor[1].bity.h2;
    qDebug() << "Wartosc z buf - bit: " << bufor[1].bity.i2;
    qDebug() << "Wartosc z buf - bit: " << bufor[1].bity.j2;
    qDebug() << "Wartosc z buf - bit: " << bufor[1].bity.k2;
    qDebug() << "Wartosc z buf - bit: " << bufor[1].bity.l2;
    qDebug() << "Wartosc z buf - bit: " << bufor[1].bity.m2;
    qDebug() << "Wartosc z buf - bit: " << bufor[1].bity.n2;
    qDebug() << "Wartosc z buf - bit: " << bufor[1].bity.o2;
    qDebug() << "Wartosc z buf 8 bit: " << bufor[1].bity.p2; */

    //swap powoduje, że QByteArray z argumentu staje się QByteArray do którego przypisujemy
    QByteArray kopia = plik_WAV.dane.dane; //Aby porówanac zmiany

    //Dla wszsytkich short int - 16 bit - próbka
    for( int i=0, j=0; i<plik_WAV.dane.dane.size() && j<bufor.size(); i=i+2 )
    {
        liczba.bajty[0] = plik_WAV.dane.dane[i];
        liczba.bajty[1] = plik_WAV.dane.dane[i+1];

        liczba.bity.a = bufor[j++]; //Oszadzenie bitu wiadomości
        plik_WAV.dane.dane.replace(i, 2, liczba.bajty, 2);
    }

        komunikat += QString( "Zakończono oszadzanie danych.\n");
        ui->label_WAV_Komunikat->setText(komunikat);

    //Weryfiakcja danych
    for( int i=0; i<32; i++) qDebug() << "Wartosc bitu: " << bufor[i];
    for( int i=0; i<100; i++ )
    {
        qDebug() << "Bit: " << i << " : " << (int)plik_WAV.dane.dane[i];
        qDebug() << "Bit: " << i << " : " << (int)kopia[i]<<endl;
    }


}


void MainWindow::slotZapiszWAV()
{
    plik_WAV.nowa_nazwa_pliku = QFileDialog::getSaveFileName(this, tr("Pod jaką nazwą zapisać plik?"), "c:\\" , tr("Music (*.wav *.mp3 *ogg)"));
    qDebug() << plik_WAV.nowa_nazwa_pliku;

    plik_WAV.zapiszPlik();
}


void MainWindow::slotOdczytajWiadomoscWAV()
{
    ui->label_WAV_Komunikat->setText("Odczytuje wiadomosc.");

    unio_int liczba;
    QBitArray bufor;
    QString komunikat;
    bufor.resize( 32 + 160 ); //32 Bity int'a + bity hashu

    // -- Rozmair oraz hash, po 1 bicie na każde 16 bitów próbki
    int nrOdczy=0;
    for( int j=0; nrOdczy<plik_WAV.dane.dane.size() && j<32+160; )
    {
        liczba.bajty[0] = plik_WAV.dane.dane[nrOdczy++];
        liczba.bajty[1] = plik_WAV.dane.dane[nrOdczy++];
        bufor[j++] = liczba.bity.a; //Odczyt bitu z próbki
    }

    // -- Rozmair
    int nrBitu = 0;
    for( ; nrBitu<32; nrBitu++ )  zapisz( liczba, nrBitu, bufor[ nrBitu ]);
    int rozmiar = liczba.liczba;

        komunikat = QString( "Wiadomość ma rozmiar: %1\n" ).arg(rozmiar);
        ui->label_WAV_Komunikat->setText( komunikat );

    // -- HASH
    char hashTEMP[21]; hashTEMP[20] = 0; // Koniec Stringa
    for( int i=0; i<20; i++)
    {
        for( int j=0; j<8; j++, nrBitu++ ) zapisz( liczba, j, bufor[ nrBitu ] );
        hashTEMP[i] = liczba.bajty[0];
    }

    // -- DANE
    int ilBitow = rozmiar * 8; //Ile bitów wiaodmości ?
    bufor.resize( 32+160+ rozmiar );

    //Odczytujemy bity określajace wiadomość, po 1 bicie na 16 bitów próbki,
    for( int j=0; nrOdczy<plik_WAV.dane.dane.size() && j<ilBitow; )
    {
        liczba.bajty[0] = plik_WAV.dane.dane[ nrOdczy++ ];
        liczba.bajty[1] = plik_WAV.dane.dane[ nrOdczy++ ];
        bufor[j++] = liczba.bity.a; //Oszadzenie bitu wiadomości
    }

    // -- WIADOMOŚĆ
    char wiadomoscTEMP[ rozmiar ];

    for( int i=0, j=0; i<rozmiar && j<ilBitow; i++ )
    {
        for( int k=0; k<8; k++) zapisz( liczba, k, bufor[j] ); j++;
        wiadomoscTEMP[i] = liczba.bajty[0];
    }

    //HASH z Wiaodmości
    QCryptographicHash hash( QCryptographicHash::Sha1);
    hash.addData( wiadomoscTEMP, rozmiar );

    QString jakis ( hash.result() );
    komunikat = QString("Hash wiaodmosci: %1 \n").arg( jakis );
    QString jakis2( hashTEMP );
    komunikat += QString("Hash odnaleźiony w pliku: %1 \n").arg( jakis2 );

    if( hash.result() == hashTEMP )
    {
        komunikat += QString( "Hashe są zgodne" );
        //ui->label_InfOOsadzanychDanych->setText( komunikat );
    } else
    {
        komunikat += QString( "Hashe są niezgodne" );
        //ui->label_InfOOsadzanychDanych->setText( komunikat );
    }

    ui->label_WAV_Komunikat->setText( komunikat );

    QString NowyPlik = "WAV_wyjściowy.txt";

    QFile plik( NowyPlik);
    if( !plik.open( QIODevice::WriteOnly ) )
    {
        ui->label_WAV_Komunikat->setText("Jakiś błąd przy zapisie pliku odczytanego!");
    }

    plik.write( wiadomoscTEMP, rozmiar );
    plik.close();
}

