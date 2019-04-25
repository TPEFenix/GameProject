#pragma once

using namespace std;
using namespace game_framework;

namespace CollisionSensor_namespace
{
    bool BitmapPicture_HitRectangle(BitmapPicture, BitmapPicture);
    bool BitmapPicture_HitRectangle(BitMapRectangle, BitMapRectangle);
    bool PixelCollision(BitmapPicture *, BitmapPicture *, int);
    bool PixelCollision(BitmapAnimation *, BitmapAnimation *, int);
}

