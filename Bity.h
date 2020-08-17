#ifndef BITY_H
#define BITY_H

#include <QString>

class Bity
{
public:
    int liczba; // 32 bity, znak (16 bitów UNICOD)

    int getZPoz(int x)
    {
        return ( (liczba & (1 << x) ) >> x);
    }

    void setNaPoz (int x, bool y)
    {
        liczba = (liczba & ~(1 << x)) | ( y!=false) << x; //I true, i false.
    }

    void reset()
    {
        liczba = 0;
    }

    Bity()
    {
        liczba = 0;
    }
};

#endif // BITY_H
