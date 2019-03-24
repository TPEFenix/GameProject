#pragma once
#include "Keycode.h"
#include "KeyBoardState.h"
#include "CollisionSensor.h"
#include "TypeConverter.h"
#include "WKAudio.h"


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
        #define Matchstick_RunSpeed 7.5;
        #define Matchstick_StandbySPincrements 0.2;
        #define Matchstick_RunningSPincrements 0.1;
        #define Matchstick_ChargeSPincrements 14;


		public:
			Matchstick(int);
			~Matchstick();
			virtual void AutoLoadBitmaps(CameraPosition,COLORREF);//�̷ӦU�ۨ���Ū��
			virtual void OnUpdate(BattlePlayer *, CameraPosition, KeyBoardState, KeyBoardState, Audio_ID);//��s�禡�A�B�H�۵�������
            virtual void OnRush(BattlePlayer *, CameraPosition, KeyBoardState, KeyBoardState, Audio_ID);//��s�禡�A�B�H�۵�������


            



	};
}
