#include "mousekeyboardinput.h"
#include <QMessageBox>
#include <QEventLoop>
#include <QTimer>
#include <QNetworkReply>

MouseKeyboardInput::MouseKeyboardInput(QObject *parent) : QObject(parent)
{
    Key.type = INPUT_KEYBOARD;
    Key.ki.dwExtraInfo = 0;
    Key.ki.dwFlags = 0;
    Key.ki.time = 0;

    time_between_up_and_down = 80;
    time_after_key = 50;
    time_after_mousemove = 80;
    time_between_klick = 100;
    time_after_klick = 300;

    Mouse.type = INPUT_MOUSE;
    Mouse.mi.dwExtraInfo = 0;
    Mouse.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    Mouse.mi.dx = 0;
    Mouse.mi.dy = 0;
    Mouse.mi.mouseData = 0;
    Mouse.mi.time = 0;

    Mouseup = Mouse;
    Mouseup.mi.dwFlags = MOUSEEVENTF_LEFTUP;

    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    connect(manager, SIGNAL(sslErrors(QNetworkReply *, QList<QSslError>)), this, SLOT(errorhandling(QNetworkReply *, QList<QSslError>)));
}

//give this function a Virtual Key to have it pressed
void MouseKeyboardInput::pressKey(WORD key){
    Key.ki.wScan = MapVirtualKey(key, MAPVK_VK_TO_VSC);
    Key.ki.wVk = key;

    SendInput(1, &Key, sizeof(INPUT));

    Sleep(time_between_up_and_down);

    Key.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &Key, sizeof(INPUT));

    Sleep(time_after_key);
    Key.ki.dwFlags = 0;
}

//this function simply pastes your clipboard
void MouseKeyboardInput::paste(){
    INPUT helper = Key;
    //Ctrl
    Key.ki.wScan = MapVirtualKey(VK_CONTROL, MAPVK_VK_TO_VSC);
    Key.ki.wVk = VK_CONTROL;
    //V
    helper.ki.wScan = MapVirtualKey(0x56, MAPVK_VK_TO_VSC);
    helper.ki.wVk = 0x56;

    SendInput(1, &Key, sizeof(INPUT));
    Sleep(10); //making sure ctrl comes first
    SendInput(1, &helper, sizeof(INPUT));

    Sleep(time_between_up_and_down);

    //move Key up again
    Key.ki.dwFlags = KEYEVENTF_KEYUP;
    helper.ki.dwFlags = KEYEVENTF_KEYUP;

    SendInput(1, &Key, sizeof(INPUT));
    SendInput(1, &helper, sizeof(INPUT));

    Sleep(time_after_key);

    Key.ki.dwFlags = 0;
    helper.ki.dwFlags = 0;
}

//give this function a Point and it will click on the coordinates
void MouseKeyboardInput::click(POINT p){
    SetCursorPos(p.x, p.y);
    Sleep(time_after_mousemove);
    SendInput(1, &Mouse, sizeof(INPUT));
    Sleep(time_between_klick);
    SendInput(1, &Mouseup, sizeof(INPUT));
    Sleep(time_after_klick);
}

//this sends a clip to your clipboard to be pasted with paste() afterwards
void MouseKeyboardInput::copyToClipboard(QString name){
    QByteArray ba = name.toLatin1();
    char* output = ba.data();

    size_t len = strlen(output) + 1;
    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);
    memcpy(GlobalLock(hMem), output, len);
    GlobalUnlock(hMem);
    OpenClipboard(0);
    EmptyClipboard();
    SetClipboardData(CF_TEXT, hMem);
    CloseClipboard();
}

void MouseKeyboardInput::errorhandling(QNetworkReply *reply, QList<QSslError> errorlist){
    qDebug() << "handeling";
    reply->ignoreSslErrors(errorlist);
}

void MouseKeyboardInput::replyFinished(QNetworkReply *reply){

    if(reply->error()){
        QMessageBox msgBox;
        msgBox.setText(reply->errorString());
        msgBox.exec();
        //reply->ignoreSslErrors(reply->errorString());
    }
    htmlCode = reply->readAll();
    emit sigHtmlCode(htmlCode);
    reply->deleteLater();
}

void MouseKeyboardInput::getHTMLCode(QString Url){
    QEventLoop loop;
    QTimer time;
    QTimer::connect(&time, SIGNAL(timeout()), &loop, SLOT(quit()));
    connect(this, SIGNAL(sigHtmlCode(QString)), &loop, SLOT(quit()));

    QNetworkReply *resp = manager->get(QNetworkRequest(QUrl(Url)));
    time.start(30000); //zeit bis timeout

    loop.exec();

    if(resp == NULL){
        qDebug() << "timeout";
    }else if(QNetworkReply::NoError != resp->error()){
        qDebug() << "error";
    }else{
        //when it worked
    }

    delete resp;
}

QString MouseKeyboardInput::codeReady(QString Produktname, QString Url){
    getHTMLCode(Url);
    int pos = htmlCode.indexOf(Produktname);
    int httppos, httpend, backwards = 70;
    pos -= backwards;
    QString url;
    QString UrlSupr = "https://www.supremenewyork.com";
    if(pos > -1){
        httppos = htmlCode.indexOf("href=", pos);
        httppos += 6;
        httpend = htmlCode.indexOf("\"", httppos);
        url = htmlCode.mid(httppos, httpend);
        url = url.left(httpend-httppos);
        url = UrlSupr + url;
    }else{
        //if string (Supreme) wasnt found!
        return "error";
    }
    return url;
}

