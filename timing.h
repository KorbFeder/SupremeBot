#ifndef TIMING_H
#define TIMING_H

#include <QObject>
#include <QTimer>
#include <ctime>

class Timing : public QObject
{
    Q_OBJECT
public:
    explicit Timing(QObject *parent = nullptr);
    void autostart();
signals:

public slots:
};

#endif // TIMING_H
