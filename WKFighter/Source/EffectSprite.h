#pragma once
#include "stdafx.h"

namespace game_framework
{
    class EffectSprite
    {
    public:
        EffectSprite();
        ~EffectSprite();

        map<string, BitmapAnimation>  Content;//儲存所有特效
        void EffectAutoUpdate(BitmapAnimation*, int, bool, CameraPosition);//(讓特效自動更新狀態，放在Update裡)
        //執行一個Effect(BitmapAnimation,Camera,XR,XL,Y,VX,VY,Track,左右)
        void BootEffect(BitmapAnimation*, CameraPosition, double, double, double,double,double,bool,bool);
        void DrawAllEffection(int);//更新函式，且隨著視角移動
        void AutoLoadEffections(COLORREF);//讀取遊戲中全部特效
        void InsertEffection(string, int, int, double, COLORREF);//讀取遊戲中全部特效


    };
}

