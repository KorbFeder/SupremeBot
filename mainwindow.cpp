#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFile>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), Data(new Kundendaten),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Setup ComboBoxes
    QStringList Groessen, Zahlungsart, Kategorie, Monat, Jahr;
    Monat << "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8" << "9" << "10" << "11" << "12";
    Jahr << "2018" << "2019" << "2020" << "2021" << "2022" << "2023" << "2024" << "2025" << "2026" << "2027" << "2028";
    Groessen << "Small" << "Medium" << "Large" << "Xtra Large";
    Kategorie << "tops/sweaters" << "accessories" << "bags" << "hats" << "t-shirts" << "pants" << "sweatshirts" << "shirts" << "jackets" << "skate";
    Zahlungsart << "Visa" << "Master" << "American Express" << "Solo" << "Paypal";
    ui->comboGroesse->addItems(Groessen);
    ui->comboKategorie->addItems(Kategorie);
    ui->comboZahlung->addItems(Zahlungsart);
    ui->lineMonat->addItems(Monat);
    ui->lineJahr->addItems(Jahr);
    filepath = "test1.txt";
    loadall();
    motor = new BotMotor(Data);
}

void MainWindow::on_bStart_clicked()
{
    motor->start(ui->CheckinPPeingeloggt->isChecked(), ui->Profelauf->isChecked());
}

void MainWindow::on_bSpeichern_clicked()
{
    QFile file(filepath);
    if(!file.open(QFile::WriteOnly | QFile::Text)){
        QMessageBox msgBox(this);
        msgBox.setText("oeffnen bzw erstellen der Datei fehlgeschlagen!");
        msgBox.exec();
        return;
    }
    QTextStream out(&file);
    //Stringdaten:
    out << Data->Groesse << "\n";
    out << Data->Kategorie << "\n";
    out << Data->Name << "\n";
    out << Data->PPLogin << "\n";
    out << Data->PPPasswort << "\n";
    out << Data->Zahlungsart << "\n";

    //Koordinaten:
    out << Data->ArtDerZahlungPOS.x << "\n";
    out << Data->ArtDerZahlungPOS.y << "\n";
    out << Data->ChromeGeladenPOS.x << "\n";
    out << Data->ChromeGeladenPOS.y << "\n";
    out << Data->GroessePOS.x << "\n";
    out << Data->GroessePOS.y << "\n";
    out << Data->KassePOS.x << "\n";
    out << Data->KassePOS.y << "\n";
    out << Data->PPfeldPOS.x << "\n";
    out << Data->PPfeldPOS.y << "\n";
    out << Data->PPgeladenPOS.x << "\n";
    out << Data->PPgeladenPOS.y << "\n";
    out << Data->WarenkorbPOS.x << "\n";
    out << Data->WarenkorbPOS.y << "\n";
    out << Data->ZahlungGeladenPOS.x << "\n";
    out << Data->ZahlungGeladenPOS.y << "\n";
    out << Data->colorChromeR << "\n";
    out << Data->colorChromeG << "\n";
    out << Data->colorChromeB << "\n";
    out << Data->colorPPR << "\n";
    out << Data->colorPPG << "\n";
    out << Data->colorPPB << "\n";
    out << Data->colorZahlungR << "\n";
    out << Data->colorZahlungG << "\n";
    out << Data->colorZahlungB << "\n";
    out << Data->Monat << "\n";
    out << Data->Jahr << "\n";

    QString dummyChrome = Data->ChromePath;
    dummyChrome.replace(" ", "`");
    out << dummyChrome << "\n";

    file.flush();
    file.close();

    QMessageBox msgBox(this);
    msgBox.setText("Gespeichert");
    msgBox.exec();

}

void MainWindow::loadall(){
    QFile file(filepath);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        QMessageBox msgBox(this);
        msgBox.setText("laden der Datei fehlgeschlagen!");
        msgBox.exec();
        return;
    }
    QTextStream in(&file);
    //Stringdaten:
    in >> Data->Groesse;
    in >> Data->Kategorie;
    in >> Data->Name;
    in >> Data->PPLogin;
    in >> Data->PPPasswort;
    in >> Data->Zahlungsart;

    //Koordinaten:
    in >> Data->ArtDerZahlungPOS.x;
    in >> Data->ArtDerZahlungPOS.y;
    in >> Data->ChromeGeladenPOS.x;
    in >> Data->ChromeGeladenPOS.y;
    in >> Data->GroessePOS.x;
    in >> Data->GroessePOS.y;
    in >> Data->KassePOS.x;
    in >> Data->KassePOS.y;
    in >> Data->PPfeldPOS.x;
    in >> Data->PPfeldPOS.y;
    in >> Data->PPgeladenPOS.x;
    in >> Data->PPgeladenPOS.y;
    in >> Data->WarenkorbPOS.x;
    in >> Data->WarenkorbPOS.y;
    in >> Data->ZahlungGeladenPOS.x;
    in >> Data->ZahlungGeladenPOS.y;
    in >> Data->colorChromeR;
    in >> Data->colorChromeG;
    in >> Data->colorChromeB;
    in >> Data->colorPPR;
    in >> Data->colorPPG;
    in >> Data->colorPPB;
    in >> Data->colorZahlungR;
    in >> Data->colorZahlungG;
    in >> Data->colorZahlungB;
    in >> Data->Monat;
    in >> Data->Jahr;
    in >> Data->ChromePath;
    Data->ChromePath.replace("`", " ");

    ui->lArtZahlPOS->setText(CoordinateString(Data->ArtDerZahlungPOS));
    ui->lChromePOS->setText(CoordinateString(Data->ChromeGeladenPOS));
    ui->lGroePOS->setText(CoordinateString(Data->GroessePOS));
    ui->lKassePOS->setText(CoordinateString(Data->KassePOS));
    ui->lPPfeldPOS->setText(CoordinateString(Data->PPfeldPOS));
    ui->lPPgelPOS->setText(CoordinateString(Data->PPgeladenPOS));
    ui->lWarenkPOS->setText(CoordinateString(Data->WarenkorbPOS));
    ui->lZahlungGelPOS->setText(CoordinateString(Data->ZahlungGeladenPOS));
    ui->lcolorChrome->setText(ColorString(Data->colorChromeR, Data->colorChromeG, Data->colorChromeB));
    ui->lcolorPP->setText(ColorString(Data->colorPPR, Data->colorPPG, Data->colorPPB));
    ui->lcolorZahl->setText(ColorString(Data->colorZahlungR, Data->colorZahlungG, Data->colorZahlungB));

    ui->lineName->setText(Data->Name);
    ui->linePPlogin->setText(Data->PPLogin);
    ui->linePPPasswort->setText(Data->PPPasswort);
    ui->lineChromepath->setText(Data->ChromePath);

    //Comboboxes
    int Index;
    Index = ui->comboGroesse->findText(Data->Groesse);
    ui->comboGroesse->setCurrentIndex(Index);
    Index = ui->comboKategorie->findText(Data->Kategorie);
    ui->comboKategorie->setCurrentIndex(Index);
    Index = ui->comboZahlung->findText(Data->Zahlungsart);
    ui->comboZahlung->setCurrentIndex(Index);
    Index = ui->lineMonat->findText(Data->Monat);
    ui->lineMonat->setCurrentIndex(Index);
    Index = ui->lineJahr->findText(Data->Jahr);
    ui->lineJahr->setCurrentIndex(Index);

    file.close();
}

QString MainWindow::CoordinateString(POINT p){
    QString str1, str2;
    str1 = QString::number((int)p.x);
    str2 = QString::number((int)p.y);
    str1 = "x: " + str1 + " y: " + str2;
    return str1;
}

MainWindow::~MainWindow()
{
    delete motor;
    delete ui;
}


//Koordinaten Eingeben:
void MainWindow::getKoord(QLabel *label, POINT *point){
    QString str1, str2;
    while(!(GetAsyncKeyState(VK_RETURN) & 0x8000)){
        Sleep(50);
        qDebug() << " d";
    }
    GetCursorPos(point);
    str1 = QString::number((int)point->x);
    str2 = QString::number((int)point->y);
    str1 = "x: " + str1 + " y: " + str2;

    label->setText(str1);
}

//Koordinaten Knoepfe
void MainWindow::on_bChromePOS_clicked()
{
    getKoord(ui->lChromePOS, &Data->ChromeGeladenPOS);
    Pixelcolor(Data->ChromeGeladenPOS, Data->colorChromeR, Data->colorChromeG, Data->colorChromeB);
    ui->lcolorChrome->setText(ColorString(Data->colorChromeR, Data->colorChromeG, Data->colorChromeB));
}

void MainWindow::on_bGroePOS_clicked()
{
    getKoord(ui->lGroePOS, &Data->GroessePOS);
}

void MainWindow::on_bWarenkorbPOS_clicked()
{
    getKoord(ui->lWarenkPOS, &Data->WarenkorbPOS);
}

void MainWindow::on_bKassePOS_clicked()
{
    getKoord(ui->lKassePOS, &Data->KassePOS);
}

void MainWindow::on_bZahlungGelPOS_clicked()
{
    getKoord(ui->lZahlungGelPOS, &Data->ZahlungGeladenPOS);
    Pixelcolor(Data->ZahlungGeladenPOS, Data->colorZahlungR, Data->colorZahlungG, Data->colorZahlungB);
    ui->lcolorZahl->setText(ColorString(Data->colorZahlungR, Data->colorZahlungG, Data->colorZahlungB));
}

void MainWindow::on_bArtDerZPOS_clicked()
{
    getKoord(ui->lArtZahlPOS, &Data->ArtDerZahlungPOS);
}

void MainWindow::on_bPPgelPOS_clicked()
{
    getKoord(ui->lPPgelPOS, &Data->PPgeladenPOS);
    Pixelcolor(Data->PPgeladenPOS, Data->colorPPR, Data->colorPPG, Data->colorPPB);
    ui->lcolorPP->setText(ColorString(Data->colorPPR, Data->colorPPG, Data->colorPPB));
}

void MainWindow::on_bPPFeldPOS_clicked()
{
    getKoord(ui->lPPfeldPOS, &Data->PPfeldPOS);
}

void MainWindow::on_bName_clicked()
{
    Data->Name = ui->lineName->text();
}

void MainWindow::on_bKategorie_clicked()
{
    Data->Kategorie = ui->comboKategorie->currentText();
}

void MainWindow::on_bZahlungsart_clicked()
{
    Data->Zahlungsart = ui->comboZahlung->currentText();
}

void MainWindow::on_bGroesse_clicked()
{
    Data->Groesse = ui->comboGroesse->currentText();
}

void MainWindow::on_bPaypallogin_clicked()
{
    Data->PPLogin = ui->linePPlogin->text();
}

void MainWindow::on_bPaypalPasswort_clicked()
{
    Data->PPPasswort = ui->linePPPasswort->text();
}

void MainWindow::on_bChromepath_clicked()
{
    Data->ChromePath = ui->lineChromepath->text().replace("\\", "/");
}

QString MainWindow::ColorString(int r, int g, int b){
    QString str1, str2, str3;
    str1 = QString::number(r);
    str2 = QString::number(g);
    str3 = QString::number(b);
    return str1 = "r: " + str1 + " g: " + str2 + " b: " + str3;
}

//Monat
void MainWindow::on_pushButton_2_clicked()
{
    Data->Monat = ui->lineMonat->currentText();
}

//Jahr
void MainWindow::on_pushButton_clicked()
{
    Data->Jahr = ui->lineJahr->currentText();
}
