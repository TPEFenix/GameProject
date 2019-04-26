#pragma once
#include "stdafx.h"
#include <ddraw.h>
#include <windows.h>
#include <stdio.h>
#include <sstream>
#include <thread>
#include "gamelib.h"
#include "WKBitmap.h"
#include "BattlePlayer.h"
#include "CollisionSensor.h"
#include "AttackObj.h"
#include "EffectSprite.h"
#include "FunctionUser.h"


using namespace std;
using namespace game_framework;

namespace FunctionUser_namespace
{



    void LoadingResource(void (LoadingFunction)(), thread *mThread, bool *started, bool *finished)
    {
        if (*started == false && *finished == false)
        {
            *started = true;
            *mThread = thread(LoadingFunction);
        }
        if (*finished == true && *started == true)
        {
            *started = false;
            mThread->join();
        }
    }


    void PlayEffect(BattlePlayer *Master, string EffectName, CameraPosition Camera, double XR, double XL, double Y)
    {
        Master->Effects.BootEffect(&Master->Effects.Content[EffectName], Camera, XR, XL, Y, 0, 0, false, Master->IsRight);
    }
    void PlayEffect(EffectSprite *Effects, string EffectName, CameraPosition Camera, double XR, double XL, double Y, bool IsRight)
    {
        Effects->BootEffect(&Effects->Content[EffectName], Camera, XR, XL, Y, 0, 0, false, IsRight);
    }
    void DisableEffect(BattlePlayer * Master, string EffectName)
    {
        Master->Effects.Content[EffectName].loop = false;
        Master->Effects.Content[EffectName].visable = false;
    }
    void DisableEffect(EffectSprite * Effects, string EffectName)
    {
        Effects->Content[EffectName].loop = false;
        Effects->Content[EffectName].visable = false;
    }
}
