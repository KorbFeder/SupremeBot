#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <windows.h>
//#include "kundendaten.h"
#include <memory>
#include "botmotor.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void getKoord(QLabel *label, POINT *point);
    void loadall();
    QString CoordinateString(POINT);
    QString ColorString(int, int, int);

    QString filepath;
    BotMotor *motor;
    std::shared_ptr<Kundendaten> Data;
    Ui::MainWindow *ui;

private slots:
    void on_bChromePOS_clicked();
    void on_bGroePOS_clicked();
    void on_bWarenkorbPOS_clicked();
    void on_bKassePOS_clicked();
    void on_bZahlungGelPOS_clicked();
    void on_bArtDerZPOS_clicked();
    void on_bPPgelPOS_clicked();
    void on_bPPFeldPOS_clicked();
    void on_bSpeichern_clicked();
    void on_bStart_clicked();
    void on_bName_clicked();
    void on_bKategorie_clicked();
    void on_bZahlungsart_clicked();
    void on_bGroesse_clicked();
    void on_bPaypallogin_clicked();
    void on_bPaypalPasswort_clicked();
    void on_bChromepath_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
};

#endif // MAINWINDOW_H
