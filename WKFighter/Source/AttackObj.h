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
		double Damage = 0;//�ˮ`
		double HitVelocity_X = 0;
		double HitVelocity_Y = 0;
		bool IsHited;//�w�g����
		bool CanCombo;//�i��������

		//��k�禡




    };
}
