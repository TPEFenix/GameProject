#pragma once
#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include <windows.h>
#include <vector>
#include "audio.h"
#include "gamelib.h"
#include "WKBitmap.h"
#include "BattlePlayer.h"
#include "CollisionSensor.h"
#include "WKAudio.h"
#include "Characters.h"
#include "AttackObj.h"

using namespace std;
using namespace WKAudio_namespace;
using namespace CollisionSensor_namespace;

namespace game_framework
{
    AttackObj::AttackObj():BitmapAnimation()
    {
    }
    AttackObj::~AttackObj()
    {
    }
}
