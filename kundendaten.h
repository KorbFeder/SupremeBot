#ifndef KUNDENDATEN_H
#define KUNDENDATEN_H

#include <windows.h>
#include <QString>

struct Kundendaten{
    POINT ChromeGeladenPOS;
    POINT GroessePOS;
    POINT WarenkorbPOS;
    POINT KassePOS;
    POINT ZahlungGeladenPOS;
    POINT ArtDerZahlungPOS;
    POINT PPgeladenPOS;
    POINT PPfeldPOS;

    QString Name;
    QString Kategorie;
    QString Zahlungsart;
    QString Groesse;
    QString PPLogin;
    QString PPPasswort;
    QString ChromePath;
    QString Monat;
    QString Jahr;

    int colorChromeR;
    int colorChromeG;
    int colorChromeB;
    int colorZahlungR;
    int colorZahlungG;
    int colorZahlungB;
    int colorPPR;
    int colorPPG;
    int colorPPB;
};

//body in botmotor.cpp
void Pixelcolor(POINT p, int &r, int &g, int &b);

#endif // KUNDENDATEN_H
