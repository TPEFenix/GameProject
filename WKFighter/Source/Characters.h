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

			virtual void AutoLoadBitmaps(CameraPosition,COLORREF);//依照各自角色讀檔
			virtual void OnUpdate(BattlePlayer *, CameraPosition, KeyBoardState, KeyBoardState, Audio_ID);//更新函式，且隨著視角移動
            virtual void OnRush(BattlePlayer *, CameraPosition, KeyBoardState, KeyBoardState, Audio_ID);//更新函式，且隨著視角移動
            virtual void Draw(int, int);//更新函式，且隨著視角移動

            //Effects
            BitmapAnimation *Effect_Rush;



	};
}
