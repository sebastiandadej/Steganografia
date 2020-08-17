#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QPixmap>
#include <QFileDialog>
#include <QRect>
#include <QRgb>
#include <QBitArray>
#include <QByteArray>
#include <QDataStream>
#include <QBuffer>
#include <QCryptographicHash> //SHA-1

//BMP
#include <QCheckBox>

//WAV
#include <QMediaPlayer>
#include <QAudioDecoder>
#include <QAudioFormat>
#include <QAudioDeviceInfo>
#include <QAudioOutput>

#include "Bity.h"
#include "audio_wav.h"

#include <stdio.h>



//Wideo
#include <QVideoEncoderSettingsControl>


namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Ui::MainWindow *ui;


    //Ogólne
    QString infOAutorze;

    //Głowne
    QString nazwaPliku;
    QImage *obrazBMP;
    QImage *obrazBMPZmody;
    QImage *obrazJPEG;
    QFile *plikDoOsadzenia;
    QFile obrazJPEG_plik;
    QByteArray wiadomosc;
    QString komunikat;

    //BMP
    int startX, startY;
    int iloscPreambul;
    int checkBoxRedIlosc;
    int checkBoxGreenIlosc;
    int checkBoxBlueIlosc;
    QCheckBox **checkBoxy;
    QBitArray qBitArrayJeden, qBitArrayDwa;


    //WAV
    QMediaPlayer *player;
    Audio_WAV plik_WAV;


public:
    void cialo();
    int osadzaj();
    void odczytWav();
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:   
    void slotWyborPlikuDoOsadzenia();


    //BMP
    void slotWyborBazowegoBMP();
    void slotOsadzajWiadomoscBMP();
    void slotZapiszBMP();
    void slotZapiszJakoJPEG();
    void slotOdczytajWiadomoscBMP();

    //WAV
    void slotWyborPlikuWAV();
    void slotOdtworzAudio();
    void slotOdtworzOsadzoneAudio();
    void slotStopAudio();
    void slotOsadzajWAV();  
    void slotZmienGlosnoscWAV();
    void slotZapiszWAV();
    void slotOdczytajWiadomoscWAV();
};

#endif // MAINWINDOW_H
