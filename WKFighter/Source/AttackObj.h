#pragma once
#include "Keycode.h"
#include "KeyBoardState.h"
#include "CollisionSensor.h"
#include "TypeConverter.h"
#include "WKAudio.h"

using namespace std;
using namespace WKAudio_namespace;
using namespace CollisionSensor_namespace;

namespace game_framework
{
    class AttackObj:public BitmapAnimation
    {
    public:
        AttackObj();
        ~AttackObj();


		//
		double Damage = 0;//傷害
		double HitVelocity_X = 0;
		double HitVelocity_Y = 0;
		bool IsHited;//已經擊中
		bool CanCombo;//可持續擊中

		//方法函式




    };
}
