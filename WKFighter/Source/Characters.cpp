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
    Matchstick::Matchstick(int number) :BattlePlayer()
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
        visable = true;//�O�_�i��
        CanPixelCollision = true;//�O�_�ϥι����I���A�Ҧ��ʧ@���γ��|�M��
        InSideCamera = true;//�O�_�������Y�v�T
        Velocity_X = 0;//X�t��
        Velocity_Y = 0;//Y�t��
        Acceleration_X = 0;//X�[�t��
        Acceleration_Y = 0;//Y�[�t��
        Rush_cost = 15;
        StandbySPincrements = 5;

        //��l�ƯS��
        Effect_Rush = new BitmapAnimation("Airboost", false);


    }
    Matchstick::~Matchstick()
    {
        delete Effect_Rush;
    }
    void Matchstick::AutoLoadBitmaps(CameraPosition Camera, COLORREF color)
    {

        InsertBitmapPicture("�ݾ�", 0, color);
        InsertBitmapPicture("�ݾ�", 1, color);
        InsertBitmapPicture("����", 0, color);
        InsertBitmapPicture("����", 1, color);
        InsertBitmapPicture("����", 2, color);
        InsertBitmapPicture("����", 3, color);
        InsertBitmapPicture("����", 4, color);
        InsertBitmapPicture("�Ĩ�", 0, color);
        InsertBitmapPicture("�Ĩ�", 1, color);


        //LoadEffects
        Effect_Rush->AutoLoadBitmaps("Effects", "Airboost", 5, false, color);


        AnimationUpdate(Camera);
    }
    void Matchstick::OnUpdate(BattlePlayer *Enemy, CameraPosition Camera, KeyBoardState KeyState_now, KeyBoardState KeyState_last, Audio_ID Sounds)
    {
        InputJudge(KeyState_now, KeyState_last);
        OnStandby(Enemy, Camera, KeyState_now, KeyState_last, Sounds);
        OnRunning(Enemy, Camera, KeyState_now, KeyState_last, Sounds);
        OnRush(Enemy, Camera, KeyState_now, KeyState_last, Sounds);




        AnimationUpdate(Camera);
        this->PhysicalMovement(Camera, KeyState_now, KeyState_last);
    }
    void Matchstick::OnRush(BattlePlayer *Enemy, CameraPosition Camera, KeyBoardState KeyState_now, KeyBoardState KeyState_last, Audio_ID Sounds)
    {
        if (Action == "�Ĩ�")
        {
            RushTimer += TIMER_TICK_MILLIDECOND;
            if (Step == 1)
            {
                Velocity_Y = 0;
                Effect_Rush->AutoPlay(20);
                Effect_Rush->OnUpdate("Effects", Camera);
            }
            if (RushTimer < 50 && Step == 0)
            {
                Velocity_X = 0;
            }
            else if (RushTimer >= 50 && Step == 0)
            {
                RushTimer = 0;
                Step = 1;
                Effect_Rush->BitmapisRight = IsRight;
                Effect_Rush->Rect.X = Rect.X;
                if (!IsRight)
                {
                    Effect_Rush->Rect.X += 30;
                }
                Effect_Rush->Rect.Y = Rect.Y-30;
                Effect_Rush->visable = true;
                Effect_Rush->AutoPlayTimer = 0;
                Effect_Rush->OnUpdate("Effects", Camera);
                PlaySounds(Sounds.Rush,false);
            }
            else if (RushTimer <= 80 && Step == 1)
            {
                if (IsRight)
                {
                    Velocity_X = 12;
                }
                else
                {
                    Velocity_X = -12;
                }
            }
            else if (RushTimer > 100 && Step == 1)
            {
                GotoStandby(Enemy, Camera, KeyState_now, KeyState_last, Sounds);
                Velocity_X = 0;
                Effect_Rush->Step = 0;
                Effect_Rush->visable = false;
                Effect_Rush->OnUpdate("Effects", Camera);
            }
        }
    }
    void Matchstick::Draw(int i, int j)
    {
        this->DisplayBitmap->Draw(i, j);
        this->Effect_Rush->DisplayBitmap->Draw(i, j);
    }
}
