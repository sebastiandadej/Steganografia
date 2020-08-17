#include "plik_jpeg.h"

Plik_JPEG::Plik_JPEG()
{
    qDebug() << "KONSTRUKTOR Plik_JPEG";

    for( int i=0; i<100; i++ ) komentarz[i] = 0;
}
