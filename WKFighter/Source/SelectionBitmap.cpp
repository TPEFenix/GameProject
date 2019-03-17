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
    }


    SelectionBitmap::~SelectionBitmap()
    {
    }
    void SelectionBitmap::AutoLoadBitmaps(string name, int MaxSteps, bool CanPixelCollisionin, COLORREF color)
    {
        AutoMaxStep = MaxSteps;
        CanPixelCollision = CanPixelCollisionin;
        string StepString = IntToString(MaxSteps);
        BitmapPictures = map<string, BitmapPicture>();
        for (int i = 0; i < MaxSteps; i += 1)
        {
            string str = ("Content\\Bitmaps\\" + name + "\\" + name + "_" + IntToString(i) + ".bmp");
            BitmapPictures.insert(std::pair<string, BitmapPicture>(str, BitmapPicture(visable)));
            static char cc[65535];
            strcpy(cc, str.c_str());
            BitmapPictures[str].LoadTexture(cc, CanPixelCollisionin, color);
        }
        OnUpdate();
    }
}
