#pragma once
#include "Keycode.h"
#include "KeyBoardState.h"
#include "CollisionSensor.h"
#include "TypeConverter.h"
#include "WKAudio.h"
#include "AttackObj.h"
#include "EffectSprite.h"


using namespace std;
using namespace WKAudio_namespace;
using namespace CollisionSensor_namespace;
using namespace TypeConverter_namespace;


namespace game_framework
{
	class Matchstick :public  BattlePlayer
	{
        #define Matchstick_HP_Max 1000;
        #define Matchstick_SP_Max 100;
        #define Matchstick_Rush_Cost 15;
        #define Matchstick_RunSpeed 6;
        #define Matchstick_StandbySPincrements 0.2;
        #define Matchstick_RunningSPincrements 0.1;
        #define Matchstick_ChargeSPincrements 14;


		public:
			Matchstick(int);
			~Matchstick();
			virtual void AutoLoadBitmaps(GPH);//依照各自角色讀檔
			virtual void OnUpdate(GPH);//更新函式，且隨著視角移動
            virtual void OnRush(GPH);
            virtual void GotoNormalAttack1(GPH);
            virtual void OnNormalAttack1(GPH);
            void AutoLoadAttacks(GPH);

            //Timer及次數控制參數------------------------------------------------------------------------------------------------------
            double NormalAttack1Timer = 0;



	};
}
