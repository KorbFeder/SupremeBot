#include "colordetection.h"

ColorDetection::ColorDetection(POINT p, QObject *parent) : QObject(parent)
{
    inputpoint.x = p.x;
    inputpoint.y = p.y;
    int ScreenX = GetSystemMetrics(SM_CXSCREEN);
    int ScreenY = GetSystemMetrics(SM_CYSCREEN);
    int left_offset = ScreenX / 38;
    int right_offset = ScreenX / 19;
    int bottom_offset = ScreenY / 5;
    int top_offset = ScreenY / 10;
    rc.left = p.x - left_offset;
    rc.right = p.x + right_offset;
    rc.top = p.y - top_offset;
    rc.bottom = p.y + bottom_offset;

    width = rc.right - rc.left;
    hight = rc.bottom - rc.top;
}

POINT ColorDetection::StartDetection(int r, int g, int b){
    qDebug() << " startdddd";
    HDC hdc = GetDC(NULL);
    BITMAP bm;
    HDC memdc = CreateCompatibleDC(hdc);
    HBITMAP hbitmap = CreateCompatibleBitmap(hdc, width, hight);
    SelectObject(memdc, hbitmap);

    BitBlt(memdc, 0, 0, width, hight, hdc, rc.left, rc.top, SRCCOPY);
    if(!GetObject(hbitmap, sizeof(BITMAP), (LPSTR)&bm)){
        qDebug() << "failed GetObject";
    }

    BITMAPINFO bmi;
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = bm.bmWidth;
    bmi.bmiHeader.biHeight = bm.bmHeight;
    bmi.bmiHeader.biPlanes = bm.bmPlanes;
    bmi.bmiHeader.biBitCount = bm.bmBitsPixel;
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biSizeImage = 0;

    int maxPixels = bm.bmWidth * bm.bmHeight;
    RGBQUAD * pPixels = new RGBQUAD[maxPixels];

    if(!GetDIBits(memdc, hbitmap, 0, (UINT)bm.bmHeight, pPixels, &bmi, DIB_RGB_COLORS)){
        ReleaseDC(NULL, hdc);
        delete[] pPixels;
        qDebug() << "failed to GetDIBits";
    }

    int pixel = 0;
    int po;
    int posy = rc.bottom;
    qDebug() << "checkpoint 1";
    for(int y = 0; y < hight; y++){
        for(int x = width; x > 0; x--){
           po = x + pixel;
           if(ColorCompare(r, g, b, pPixels[po].rgbRed, pPixels[po].rgbGreen, pPixels[po].rgbBlue)){
               POINT point;
               point.x = x+rc.left;
               point.y = posy;
               return point;
           }
        }
        pixel += width;
        posy--;
    }


    delete[] pPixels;
    DeleteDC(memdc);
    DeleteObject(hbitmap);
    ReleaseDC(NULL, hdc);
    return inputpoint;
    qDebug() << "finished";
}

bool ColorDetection::ColorCompare(int r, int g, int b, int rnew, int gnew, int bnew){
    if(r + 30 >= rnew && r - 30 <= rnew){
        if(g + 30 >= gnew && g - 30 <= gnew){
            if(b + 30 >= bnew && b - 30 <= bnew){
                qDebug() << r;
                qDebug() << g;
                qDebug() << b;
                return true;
            }
        }
    }
    return false;
}

void ColorDetection::setPos(POINT p){
    inputpoint.x = p.x;
    inputpoint.y = p.y;
}

