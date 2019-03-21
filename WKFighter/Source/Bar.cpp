#include "stdafx.h"
#include <ddraw.h>
#include <windows.h>
#include <stdio.h>
#include <sstream>
#include "gamelib.h"
#include "WKBitmap.h"
#include "TypeConverter.h"
#include "Bar.h"

namespace game_framework
{


    Bar::Bar(string respath, int X_in, int Y_in, bool vis, bool canhit, bool inside)
    {
        Rect.X = X_in;
        Rect.Y = Y_in;
        Rect.X_int = X_in;
        Rect.Y_int = Y_in;
        visable = vis;
        this->SetResourcePath(respath);
        CanPixelCollision = canhit;
        InSideCamera = inside;
    }

    Bar::~Bar()
	{
	}
 
	void Bar::Draw(int CurrentLayer, int TargetLayer)
	{
		if (CurrentLayer == TargetLayer&&this->visable == true)//直到他的圖片層級才可以顯示
		{
			SetTopLeft(Rect.X_int, Rect.Y_int);
			ShowBitmap(this->Rect.X_int, this->Rect.Y_int, this->Rect.X_int+this->Rect.Width, this->Rect.Y_int + this->Rect.Height);
		}
	}

}