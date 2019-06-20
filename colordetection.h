#ifndef COLORDETECTION_H
#define COLORDETECTION_H

#include <QObject>
#include <windows.h>
#include <QDebug>

class ColorDetection : public QObject
{
    Q_OBJECT
public:
    explicit ColorDetection(POINT p, QObject *parent = nullptr);
    POINT StartDetection(int r, int g, int b);
    bool ColorCompare(int r, int g, int b, int rnew, int gnew, int bnew);
    void setPos(POINT p);

private:
    POINT inputpoint;
    RECT rc;
    int width;
    int hight;

signals:

public slots:
};

#endif // COLORDETECTION_H
