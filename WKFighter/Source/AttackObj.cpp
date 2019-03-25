#pragma once
#include "stdafx.h"
#include "Resource.h"
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "WKBitmap.h"
#include "CollisionSensor.h"
#include "WKAudio.h"
#include "AttackObj.h"
#include "EffectSprite.h"


using namespace std;
using namespace WKAudio_namespace;
using namespace CollisionSensor_namespace;

namespace game_framework
{
    AttackManager::AttackManager()
    {
    }


    AttackManager::~AttackManager()
    {
    }
    void AttackManager::AttackAutoUpdate(AttackObj * Attack, string BeloneName, int tick, bool replay, CameraPosition Camera)
    {
        Attack->AutoPlay(tick, replay);
        Attack->Rect.X += Attack->Velocity_X;
        Attack->Rect.Y += Attack->Velocity_Y;
        Attack->OnUpdate(BeloneName + "\\Attacks", Camera);
    }
    void AttackManager::AttackReset(AttackObjPH)
    {
        Attack->BitmapisRight = IsRight;
        if (Attack->BitmapisRight)
            Attack->Rect.X = XR;
        else
            Attack->Rect.X = XL;

        Attack->Rect.Y = Y;
        Attack->visable = true;
        Attack->AutoPlayTimer = 0;
        Attack->Step = 0;
        Attack->IsHited = false;
        Attack->Damage = Damage;
        Attack->Velocity_X = VX;
        Attack->Velocity_Y = VY;
        Attack->HitVelocity_X = HitVX;
        Attack->HitVelocity_Y = HitVY;
        Attack->HitEffect = HitEffect;
        Attack->CanCombo = CanCombo;
        Attack->Drawable = drawable;
        Attack->HitSound = HitSound;
        Attack->HitTime = HitTime;
        Attack->HitBreak = HitBreak;
        Attack->OnUpdate(BeloneName + "\\Attacks", Camera);
    }
    void AttackManager::DrawAllAttacks(int i)
    {
        map<string, AttackObj>::iterator iter;
        for (iter = AttackObjects.begin(); iter != AttackObjects.end(); iter++)
        {
            if (iter->second.Drawable)
            {
                iter->second.DisplayBitmap->Draw(i, iter->second.drawlayer);
            }
        }
    }
    void AttackManager::InsertAttacks(string BeloneName, string name, int maxstep, int drawlayer, double pre, COLORREF color, CameraPosition Camera)
    {
        AttackObjects.insert(std::pair<string, AttackObj>(name, AttackObj(name, 0, 0, false, true, true)));
        AttackObjects[name].SetName(name);
        AttackObjects[name].AutoLoadBitmaps(BeloneName + "\\Attacks", name, maxstep + 1, pre, true, color);
        AttackObjects[name].drawlayer = drawlayer;
        AttackObjects[name].OnUpdate(BeloneName + "\\Attacks", Camera);
    }


    AttackObj::AttackObj() :BitmapAnimation()
    {
    }
    AttackObj::AttackObj(string namein, int X, int Y, bool vis, bool CanHit, bool inside)
    {
        Step = 0;
        AutoMaxStep = 0;
        AutoPlayTimer = 0;
        CanPixelCollision = CanHit;
        SetName(namein);
        visable = vis;
        InSideCamera = inside;
        Rect.X = X;
        Rect.Y = Y;
        Rect.X_int = X;
        Rect.Y_int = Y;
        BitmapisRight = true;
        drawlayer = 0;
        TrackPoint = false;
    }
    AttackObj::~AttackObj()
    {
    }
    void AttackObj::OnUpdate(string unsingfolder, CameraPosition Camera)
    {
        string Actionstring;
        if (BitmapisRight)
            Actionstring = "Content\\Bitmaps\\" + unsingfolder + "\\" + GetName() + "_" + IntToString(Step) + ".bmp";
        else
            Actionstring = "Content\\Bitmaps\\" + unsingfolder + "\\" + GetName() + "_" + IntToString(Step) + "_L.bmp";
        char *cc = new char[65535];
        strcpy(cc, Actionstring.c_str());
        DisplayBitmap = &BitmapPictures[cc];
        Rect.Width = DisplayBitmap->Rect.Width;
        Rect.Height = DisplayBitmap->Rect.Height;
        Rect.X += Velocity_X;
        Rect.Y += Velocity_Y;
        if (InSideCamera)
        {
            Rect.X_int = (int)(Rect.X - Camera.X);
            Rect.Y_int = (int)(Rect.Y - Camera.Y);
        }
        else
        {
            Rect.X_int = (int)(Rect.X);
            Rect.Y_int = (int)(Rect.Y);
        }
        DisplayBitmap->Rect.X = Rect.X_int;
        DisplayBitmap->Rect.Y = Rect.Y_int;
        DisplayBitmap->OnUpdate();
        DisplayBitmap->visable = visable;
        delete[] cc;
    }

    double AttackObj::Ahead(double move)
    {
        {
            double returner = 0;
            if (BitmapisRight)
            {
                returner = move;
            }
            else
            {
                returner = -move;
            }
            return returner;
        }
    }


}
