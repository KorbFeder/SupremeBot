#ifndef MOUSEKEYBOARDINPUT_H
#define MOUSEKEYBOARDINPUT_H

#include <QObject>
#include <windows.h>
#include <QtNetwork/QNetworkAccessManager>

class MouseKeyboardInput : public QObject
{
    Q_OBJECT
public:
    explicit MouseKeyboardInput(QObject *parent = nullptr);
    void paste();
    void click(POINT p);
    void copyToClipboard(QString name);
    void pressKey(WORD);
    QString codeReady(QString Produktname, QString Url);

private:
    INPUT Key;
    INPUT Mouse;
    INPUT Mouseup;

    int time_between_up_and_down;
    int time_after_key;
    int time_after_mousemove;
    int time_between_klick;
    int time_after_klick;

    QNetworkAccessManager *manager;
    QString htmlCode;
    void getHTMLCode(QString Url);

signals:
    void sigHtmlCode(QString content);

public slots:
    void replyFinished(QNetworkReply*);
    void errorhandling(QNetworkReply *, QList<QSslError>);
};

#endif // MOUSEKEYBOARDINPUT_H
