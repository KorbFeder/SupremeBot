#include "timing.h"
#include <windows.h>

Timing::Timing(QObject *parent) : QObject(parent)
{

}

void Timing::autostart(){
    while(1){
        std::time_t t = std::time(0);
        std::tm *now = std::localtime(&t);
        if(now->tm_hour >= 12 && (now->tm_sec >= 2 || now->tm_min >= 1)){
            break;
        }
        Sleep(1000);
    }
}
