#ifndef BOTMOTOR_H
#define BOTMOTOR_H

#include <QObject>
#include "mousekeyboardinput.h"
#include "kundendaten.h"
#include <memory>
#include "timing.h"

class BotMotor : public QObject
{
    Q_OBJECT
public:
    explicit BotMotor(std::shared_ptr<Kundendaten> d, QObject *parent = nullptr);
    void start(bool PPeingeloggt, bool testlauf);

private:
    Timing time;
    MouseKeyboardInput mki;
    std::shared_ptr<Kundendaten> Data;
    bool pploggedin;
    bool TestLauf;

    QString httpumwandeln(QString Kat);
    void groesseUndHinzu();
    void startChrome(QString urlOfCoice);
    void Colordetection(POINT p, int sr, int sg, int sb);
    int groesse();
    void Zahlungsmethode();
    void Paypal();
    void Karte();
signals:

public slots:
};



#endif // BOTMOTOR_H
