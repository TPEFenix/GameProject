#pragma once
#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include <windows.h>
#include <vector>
#include "audio.h"
#include "gamelib.h"
#include "Keycode.h"
#include "KeyBoardState.h"
#include "WKBitmap.h"
#include "BattlePlayer.h"
#include "CollisionSensor.h"
#include "TypeConverter.h"
#include "WKAudio.h"
#include "Characters.h"

using namespace std;
using namespace WKAudio_namespace;
using namespace CollisionSensor_namespace;
using namespace TypeConverter_namespace;

namespace game_framework
{
	Matchstick::Matchstick(int number):BattlePlayer()
	{
		SetName("Matchstick");
		PlayerNumber = number;//玩家編號
		if (number == 1)
		{
			IsRight = true;
		}
		else if (number == 2)
		{
			IsRight = false;
		}
		HP_Max = 1000;//最大生命值
		SP_Max = 100;//最大氣力
		HP = HP_Max;//當前生命
		SP = SP_Max;//當前氣力
		CanControl = false;//可以控制
		Invincible = false;//無敵狀態
		Action = "待機";//動作狀態
		Step = 0;//當前步驟數
		visable =true;//是否可見
		CanPixelCollision = true;//是否使用像素碰撞，所有動作分割都會套用
		InSideCamera = true;//是否受到鏡頭影響
		Velocity_X = 0;//X速度
		Velocity_Y = 0;//Y速度
        Acceleration_X = 0;//X加速度
		Acceleration_Y = 0;//Y加速度
		
	}
	Matchstick::~Matchstick()
	{
		
	}
	void Matchstick::AutoLoadBitmaps(CameraPosition Camera, COLORREF color)
	{
        
		InsertBitmapPicture("待機",0, color);
		InsertBitmapPicture("待機",1, color);
        InsertBitmapPicture("移動", 0, color);
        InsertBitmapPicture("移動", 1, color);
        InsertBitmapPicture("移動", 2, color);
        InsertBitmapPicture("移動", 3, color);
        InsertBitmapPicture("移動", 4, color);
        


		AnimationUpdate(Camera);
	}
	void Matchstick::OnUpdate(BattlePlayer *Enemy, CameraPosition Camera, KeyBoardState KeyState_now, KeyBoardState KeyState_last, Audio_ID Sounds)
	{
        InputJudge(KeyState_now, KeyState_last);
        OnStandby(Enemy,Camera, KeyState_now,KeyState_last,Sounds);
        OnRunning(Enemy, Camera, KeyState_now, KeyState_last, Sounds);





        AnimationUpdate(Camera);
		this->PhysicalMovement(Camera, KeyState_now, KeyState_last);
	}
}
