#pragma once
#include "stdafx.h"
#include "Resource.h"
#include <ddraw.h>
#include <windows.h>
#include "audio.h"
#include "gamelib.h"
#include "WKBitmap.h"
#include "BattlePlayer.h"
#include "EffectSprite.h"

using namespace std;

namespace game_framework
{
    EffectSprite::EffectSprite()
    {
    }
    EffectSprite::~EffectSprite()
    {
    }
    //所有特效都寫這
    void EffectSprite::AutoLoadEffections(COLORREF color)
    {

        InsertEffection("Airboost", 5, 4, 10, color);
        InsertEffection("Airboost2", 5, 4, 10, color);
        InsertEffection("Airboost3", 5, 4, 10, color);
        InsertEffection("SPCharge", 12, 4, 10, color);
        InsertEffection("PunchHit", 3, 4, 16, color);
        InsertEffection("ResetBody", 4, 4, 16, color);
        InsertEffection("Disable", 4, 4, 16, color);
        InsertEffection("stun_star", 3, 4, 16, color);
        InsertEffection("HitWall", 3, 4, 16, color);
        InsertEffection("Matchstick_US", 0, 6, 16, color);
        InsertEffection("Rina_US", 0, 6, 16, color);
        InsertEffection("OraOraFire", 4, 4, 30, color);
    }

    void EffectSprite::EffectAutoUpdate(BitmapAnimation * Effection, int tick, bool replay, CameraPosition Camera)
    {
        if (Effection->visable)
        {
            Effection->InSideCamera = true;
            Effection->AutoPlay(tick, replay);
            Effection->Rect.X += Effection->Velocity_X;
            Effection->Rect.Y += Effection->Velocity_Y;
            Effection->OnUpdate("Effects", Camera);
        }
    }
    void EffectSprite::BootEffect(BitmapAnimation *Effection, CameraPosition Camera, double XR, double XL, double Y, double VX, double VY, bool Track, bool IsRight)
    {
        Effection->AutoPlayTimer = 0;
        Effection->Step = 0;
        Effection->BitmapisRight = IsRight;
        if (Effection->BitmapisRight)
            Effection->Rect.X = XR;
        else
            Effection->Rect.X = XL;
        Effection->Rect.Y = Y;
        Effection->visable = true;
        Effection->Velocity_X = VX;
        Effection->Velocity_X = VY;
        Effection->TrackPoint = Track;
        EffectAutoUpdate(Effection, (int)(Effection->PreAutoFrequence), false, Camera);
    }
    void EffectSprite::DrawAllEffection(int i)
    {
        map<string, BitmapAnimation>::iterator iter;
        for (iter = Content.begin(); iter != Content.end(); iter++)
            if (iter->second.visable)
                iter->second.DisplayBitmap->Draw(i, iter->second.drawlayer);
    }
    void EffectSprite::InsertEffection(string name, int maxstep, int drawlayer, double pre, COLORREF color)
    {
        Content.insert(std::pair<string, BitmapAnimation>(name, BitmapAnimation(false)));
        Content[name].SetName(name);
        Content[name].AutoLoadBitmaps("Effects", name, maxstep + 1, pre, false, color);
        Content[name].drawlayer = drawlayer;
        Content[name].OnUpdate();
    }

}
