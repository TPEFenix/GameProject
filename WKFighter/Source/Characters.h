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
		public:
			Matchstick(int);
			~Matchstick();

			virtual void AutoLoadBitmaps(CameraPosition,COLORREF);//�̷ӦU�ۨ���Ū��
			virtual void OnUpdate(BattlePlayer *, CameraPosition, KeyBoardState, KeyBoardState, Audio_ID);//��s�禡�A�B�H�۵�������
            virtual void OnRush(BattlePlayer *, CameraPosition, KeyBoardState, KeyBoardState, Audio_ID);//��s�禡�A�B�H�۵�������
            virtual void Draw(int, int);//��s�禡�A�B�H�۵�������

            //Effects
            BitmapAnimation *Effect_Rush;



	};
}
