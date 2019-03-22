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
		PlayerNumber = number;//���a�s��
		if (number == 1)
		{
			IsRight = true;
		}
		else if (number == 2)
		{
			IsRight = false;
		}
		HP_Max = 1000;//�̤j�ͩR��
		SP_Max = 100;//�̤j��O
		HP = HP_Max;//��e�ͩR
		SP = SP_Max;//��e��O
		CanControl = false;//�i�H����
		Invincible = false;//�L�Ī��A
		Action = "�ݾ�";//�ʧ@���A
		Step = 0;//��e�B�J��
		visable =true;//�O�_�i��
		CanPixelCollision = true;//�O�_�ϥι����I���A�Ҧ��ʧ@���γ��|�M��
		InSideCamera = true;//�O�_�������Y�v�T
		Velocity_X = 0;//X�t��
		Velocity_Y = 0;//Y�t��
        Acceleration_X = 0;//X�[�t��
		Acceleration_Y = 0;//Y�[�t��
		
	}
	Matchstick::~Matchstick()
	{
		
	}
	void Matchstick::AutoLoadBitmaps(CameraPosition Camera, COLORREF color)
	{
        
		InsertBitmapPicture("�ݾ�",0, color);
		InsertBitmapPicture("�ݾ�",1, color);
        InsertBitmapPicture("����", 0, color);
        InsertBitmapPicture("����", 1, color);
        InsertBitmapPicture("����", 2, color);
        InsertBitmapPicture("����", 3, color);
        InsertBitmapPicture("����", 4, color);
        


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
