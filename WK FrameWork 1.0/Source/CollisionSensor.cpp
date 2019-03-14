#include "stdafx.h"
#include "stdafx.h"
#include "stdafx.h"
#include <ddraw.h>
#include <windows.h>
#include <stdio.h>
#include <sstream>
#include "gamelib.h"
#include "WKBitmap.h"
#include "BatllePlayer.h"
#include "CollisionSensor.h"

using namespace std;
using namespace game_framework;

namespace CollisionSensor_namespace
{
    bool BitmapPicture_HitRectangle(BitmapPicture Bitmap1, BitmapPicture Bitmap2)
    {
        int x1 = Bitmap1.Rect.X_int;
        int x2 = Bitmap2.Rect.X_int;
        int y1 = Bitmap1.Rect.Y_int;
        int y2 = Bitmap2.Rect.Y_int;
        int w1 = Bitmap1.Rect.Width;
        int w2 = Bitmap2.Rect.Width;
        int h1 = Bitmap1.Rect.Height;
        int h2 = Bitmap2.Rect.Height;

        if (x1 >= x2 && x1 >= x2 + w2) {
            return false;
        }
        else if (x1 <= x2 && x1 + w1 <= x2) {
            return false;
        }
        else if (y1 >= y2 && y1 >= y2 + h2) {
            return false;
        }
        else if (y1 <= y2 && y1 + h1 <= y2) {
            return false;
        }
        return true;
    }
    bool PixelCollision(BitmapPicture * Bitmap1, BitmapPicture * Bitmap2, int accuracy)
    {
        if (BitmapPicture_HitRectangle(*Bitmap1, *Bitmap2) == true)
        {
            if (Bitmap1->CanPixelCollision&&Bitmap2->CanPixelCollision&& Bitmap1->visable &&Bitmap2->visable &&accuracy > 0)
            {
                //決定碰撞四邊
                int HitRight = 0;
                int HitLeft = 0;
                int HitTop = 0;
                int Hitbottom = 0;
                if (Bitmap1->Rect.X_int + Bitmap1->Rect.Width > Bitmap2->Rect.X_int + Bitmap2->Rect.Width)
                {
                    HitRight = Bitmap2->Rect.X_int + Bitmap2->Rect.Width;
                }
                else
                {
                    HitRight = Bitmap1->Rect.X_int + Bitmap1->Rect.Width;
                }
                if (Bitmap1->Rect.X_int < Bitmap2->Rect.X_int)
                {
                    HitLeft = Bitmap2->Rect.X_int;
                }
                else
                {
                    HitLeft = Bitmap1->Rect.X_int;
                }
                if (Bitmap1->Rect.Y_int + Bitmap1->Rect.Height > Bitmap2->Rect.Y_int + Bitmap2->Rect.Height)
                {
                    Hitbottom = Bitmap2->Rect.Y_int + Bitmap2->Rect.Height;
                }
                else
                {
                    Hitbottom = Bitmap1->Rect.Y_int + Bitmap1->Rect.Height;
                }
                if (Bitmap1->Rect.Y_int < Bitmap2->Rect.Y_int)
                {
                    HitTop = Bitmap2->Rect.Y_int;
                }
                else
                {
                    HitTop = Bitmap1->Rect.Y_int;
                }
                for (int i = 0; i < Hitbottom - HitTop; i += (accuracy))
                {
                    for (int j = 0; j < HitRight - HitLeft; j += (accuracy))
                    {
                        if (Bitmap1->EffectRect[i + HitTop - Bitmap1->Rect.Y_int][j + HitLeft - Bitmap1->Rect.X_int] == true && Bitmap2->EffectRect[i + HitTop - Bitmap2->Rect.Y_int][j + HitLeft - Bitmap2->Rect.X_int] == true)
                        {
                            return true;
                        }
                    }
                }
            }
            return false;
        }
        else
        {
            return false;
        }
    }
    bool PixelCollision(BitmapAnimation Bitmap1, BitmapAnimation Bitmap2, int accuracy)
    {
        return PixelCollision(Bitmap1.DisplayBitmap, Bitmap2.DisplayBitmap, accuracy);
    }
}
