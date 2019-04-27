#pragma once
#include "stdafx.h"
#include <ddraw.h>
#include <windows.h>
#include <stdio.h>
#include <sstream>
#include "gamelib.h"
#include "WKBitmap.h"
#include "TypeConverter.h"
#include "SelectionBitmap.h"

using namespace std;
using namespace TypeConverter_namespace;

namespace game_framework
{
    SelectionBitmap::SelectionBitmap()
    {
        Step = 0;
        AutoMaxStep = 0;
        AutoPlayTimer = 0;
        CanPixelCollision = false;
        visable = false;
    }


    SelectionBitmap::~SelectionBitmap()
    {
    }
    void SelectionBitmap::OnUpdate(int Nowselect, int Actselect)
    {
        if (Nowselect == Actselect)
            this->Step = 1;
        else
            this->Step = 0;
        string Actionstring = "Content\\Bitmaps\\Selection\\" + this->GetName() + "_" + IntToString(Step) + ".bmp";
        char *cd = new char[65535];
        strcpy(cd, Actionstring.c_str());
        DisplayBitmap = &BitmapPictures[cd];
        Rect.Width = DisplayBitmap->Rect.Width;
        Rect.Height = DisplayBitmap->Rect.Height;
        Rect.X_int = (int)(Rect.X);
        Rect.Y_int = (int)(Rect.Y);
        DisplayBitmap->Rect.X = Rect.X_int;
        DisplayBitmap->Rect.Y = Rect.Y_int;
        DisplayBitmap->OnUpdate();
        delete[] cd;
    }
    void SelectionBitmap::AutoLoadBitmaps(string name, COLORREF color)
    {
        this->SetName(name);
        CanPixelCollision = false;
        BitmapPictures = map<string, BitmapPicture>();
        string str = ("Content\\Bitmaps\\Selection\\" + name + "_0.bmp");
        BitmapPictures.insert(std::pair<string, BitmapPicture>(str, BitmapPicture(visable)));
        char *cc = new char[65535];
        strcpy(cc, str.c_str());
        BitmapPictures[str].LoadTexture(cc, false, color);
        DisplayBitmap = &BitmapPictures[cc];
        Rect.Width = DisplayBitmap->Rect.Width;
        Rect.Height = DisplayBitmap->Rect.Height;
        Rect.X_int = (int)(Rect.X);
        Rect.Y_int = (int)(Rect.Y);
        DisplayBitmap->Rect.X = Rect.X_int;
        DisplayBitmap->Rect.Y = Rect.Y_int;
        DisplayBitmap->OnUpdate();
        delete[] cc;
        str = ("Content\\Bitmaps\\Selection\\" + name + "_1.bmp");
        BitmapPictures.insert(std::pair<string, BitmapPicture>(str, BitmapPicture(visable)));
        char *ca = new char[65535];
        strcpy(ca, str.c_str());
        BitmapPictures[str].LoadTexture(ca, false, color);
        DisplayBitmap = &BitmapPictures[ca];
        Rect.Width = DisplayBitmap->Rect.Width;
        Rect.Height = DisplayBitmap->Rect.Height;
        Rect.X_int = (int)(Rect.X);
        Rect.Y_int = (int)(Rect.Y);
        DisplayBitmap->Rect.X = Rect.X_int;
        DisplayBitmap->Rect.Y = Rect.Y_int;
        DisplayBitmap->OnUpdate();
        delete[] ca;
    }
}
