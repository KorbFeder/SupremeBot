#include "botmotor.h"
#include "colordetection.h"
#include <QProcess>

#define DIFF 30

BotMotor::BotMotor(std::shared_ptr<Kundendaten> d, QObject *parent) : QObject(parent)
{
    Data = d;
}

void BotMotor::start(bool PPeingeloggt, bool testlauf){
    bool quit = true;
    pploggedin = PPeingeloggt;
    TestLauf = testlauf;
    time.autostart();
    QStringList Kateg;
    Kateg << "tops/sweaters" << "accessories" << "bags" << "hats" << "t-shirts" << "pants" << "sweatshirts" << "shirts" << "jackets" << "skate";
    QString argument = mki.codeReady(Data->Name, httpumwandeln(Data->Kategorie));
    while(quit){
        if(argument != "error"){
            break;
        }
        for(const auto &i : Kateg){
            argument = mki.codeReady(Data->Name, httpumwandeln(i));
            if(argument != "error"){
                quit = false;
                break;
            }
            qDebug() << i;
            Sleep(300);
        }
    }
    startChrome(argument);
}

void BotMotor::startChrome(QString urlOfCoice){

    QStringList urlofcoice;
    urlofcoice << urlOfCoice;
    QProcess *Chrome = new QProcess(this);
    Chrome->start(Data->ChromePath, urlofcoice);

    Colordetection(Data->ChromeGeladenPOS, Data->colorChromeR, Data->colorChromeG, Data->colorChromeB);
    groesseUndHinzu();

    Colordetection(Data->ZahlungGeladenPOS, Data->colorZahlungR, Data->colorZahlungG, Data->colorZahlungB);

    mki.click(Data->ArtDerZahlungPOS);
    Zahlungsmethode();

    if(Data->Zahlungsart == "Paypal"){
        Paypal();
    }else if(Data->Zahlungsart == "Visa"){
        Karte();
    }else if(Data->Zahlungsart == "Master"){
        Karte();
    }else if(Data->Zahlungsart == "American Express"){
        Karte();
    }else if(Data->Zahlungsart == "Solo"){
        Karte();
    }else{
        Paypal();
    }
}

void BotMotor::Karte(){
    Sleep(200);
    mki.pressKey(VK_TAB);
    mki.copyToClipboard(Data->PPLogin);
    mki.paste();

    mki.pressKey(VK_TAB);
    if(Data->Monat == "1"){
        mki.pressKey(0x30);
        mki.pressKey(0x31);
    }else if(Data->Monat == "2"){
        mki.pressKey(0x30);
        mki.pressKey(0x32);
    }else if(Data->Monat == "3"){
        mki.pressKey(0x30);
        mki.pressKey(0x33);
    }else if(Data->Monat == "4"){
        mki.pressKey(0x30);
        mki.pressKey(0x34);
    }else if(Data->Monat == "5"){
        mki.pressKey(0x30);
        mki.pressKey(0x35);
    }else if(Data->Monat == "6"){
        mki.pressKey(0x30);
        mki.pressKey(0x36);
    }else if(Data->Monat == "7"){
        mki.pressKey(0x30);
        mki.pressKey(0x37);
    }else if(Data->Monat == "8"){
        mki.pressKey(0x30);
        mki.pressKey(0x38);
    }else if(Data->Monat == "9"){
        mki.pressKey(0x30);
        mki.pressKey(0x39);
    }else if(Data->Monat == "10"){
        mki.pressKey(0x31);
        mki.pressKey(0x30);
    }else if(Data->Monat == "11"){
        mki.pressKey(0x31);
        mki.pressKey(0x31);
    }else if(Data->Monat == "12"){
        mki.pressKey(0x31);
        mki.pressKey(0x32);
    }
    mki.pressKey(VK_TAB);
    int jahr = Data->Jahr.toInt();
    jahr = jahr - 2018;
    for(int i = 0; i < jahr; i++){
        mki.pressKey(VK_DOWN);
    }

    mki.pressKey(VK_TAB);
    mki.copyToClipboard(Data->PPPasswort);
    mki.paste();
    mki.pressKey(VK_TAB);
    mki.pressKey(VK_SPACE);
    if(!TestLauf){
        mki.pressKey(VK_RETURN);
    }
    //TODO Datum fuer karte
}

void BotMotor::Paypal(){
    Sleep(1000);
    mki.pressKey(VK_TAB);
    mki.pressKey(VK_SPACE);
    mki.pressKey(VK_RETURN);

    if(pploggedin){
        Colordetection(Data->PPgeladenPOS, Data->colorPPR, Data->colorPPG, Data->colorPPB);
        if(!TestLauf){
            mki.pressKey(VK_RETURN);
            mki.click(Data->PPfeldPOS);
        }
    }else{
        mki.pressKey(VK_TAB);
        mki.copyToClipboard(Data->PPLogin);
        mki.paste();
        mki.pressKey(VK_TAB);
        mki.copyToClipboard(Data->PPPasswort);
        mki.paste();
        mki.pressKey(VK_RETURN);
        Colordetection(Data->PPgeladenPOS, Data->colorPPR, Data->colorPPG, Data->colorPPB);
        if(!TestLauf){
            mki.pressKey(VK_RETURN);
            mki.click(Data->PPfeldPOS);
        }
    }
}

void BotMotor::groesseUndHinzu(){

    ColorDetection color_detect(Data->WarenkorbPOS);
    POINT warenkPos = color_detect.StartDetection(255, 0, 0);
    warenkPos.x -= 10;
    warenkPos.y -= 10;
    POINT Groesse = warenkPos;
    Groesse.y -= Data->WarenkorbPOS.y - Data->GroessePOS.y;

    mki.click(Groesse);
    int gr = groesse();
    for(int i = 0; i < gr; i++){
        mki.pressKey(VK_DOWN);
    }
    mki.pressKey(VK_RETURN);

    mki.click(warenkPos);
    Colordetection(Data->KassePOS, Data->colorChromeR, Data->colorChromeG, Data->colorChromeB);
    mki.click(Data->KassePOS);
}

void BotMotor::Zahlungsmethode(){
    if(Data->Zahlungsart == "Visa"){
        mki.pressKey(0x56);
    }else if(Data->Zahlungsart == "Master"){
        mki.pressKey(0x4D);
    }else if(Data->Zahlungsart == "American Express"){
        mki.pressKey(0x41);
    }else if(Data->Zahlungsart == "Paypal"){
        mki.pressKey(0x50);
    }else if(Data->Zahlungsart == "Solo"){
        mki.pressKey(0x53);
    }else{
        mki.pressKey(0x50);
    }
    mki.pressKey(VK_RETURN);
}

int BotMotor::groesse(){
    int gr;
    if(Data->Groesse == "Small"){
        gr = 0;
    }else if(Data->Groesse == "Medium"){
        gr = 1;
    }else if(Data->Groesse == "Large"){
        gr = 2;
    }else if(Data->Groesse == "Xtra Large"){
        gr = 3;
    }
    return gr;
}

void BotMotor::Colordetection(POINT p, int sa, int sg, int sb){
    int r, g, b;
    while(true){
        Sleep(400);
        Pixelcolor(p, r, g, b);
            if(r + DIFF >= sa && r - DIFF  < sa){
                if((g + DIFF > sg && g - DIFF < sg)){
                    if(b + DIFF > sb && b - DIFF < sb){
                        break;
                    }
                }
            }
    }
}

QString BotMotor::httpumwandeln(QString urll){
    QString Url;
    if(urll == "tops/sweaters"){
        Url = "https://www.supremenewyork.com/shop/all/tops_sweaters";
    }else if(urll == "accessories"){
        Url = "https://www.supremenewyork.com/shop/all/accessories";
    }else if(urll == "bags"){
        Url = "https://www.supremenewyork.com/shop/all/bags";
    }else if (urll == "hats"){
        Url = "https://www.supremenewyork.com/shop/all/hats";
    }else if (urll == "t-shirts"){
        Url = "https://www.supremenewyork.com/shop/all/t-shirts";
    }else if (urll == "pants"){
        Url = "https://www.supremenewyork.com/shop/all/pants";
    }else if (urll == "sweatshirts"){
        Url = "https://www.supremenewyork.com/shop/all/sweatshirts";
    }else if(urll == "shirts"){
        Url = "https://www.supremenewyork.com/shop/all/shirts";
    }else if(urll == "jackets"){
        Url = "https://www.supremenewyork.com/shop/all/jackets";
    }else if(urll == "skate"){
        Url = "https://www.supremenewyork.com/shop/all/skate";
    }
    return Url;
}


void Pixelcolor(POINT p, int &r, int &g, int &b){
    HDC dc = GetDC(NULL);
    COLORREF color = GetPixel(dc, p.x, p.y);
    ReleaseDC(NULL, dc);
    r = GetRValue(color);
    g = GetGValue(color);
    b = GetBValue(color);
}
