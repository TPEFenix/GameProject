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

        //��O���ܼ�
        HP_Max = Matchstick_HP_Max;//�̤j�ͩR��
        SP_Max = Matchstick_SP_Max;//�̤j��O
        Rush_cost = Matchstick_Rush_Cost;//�Ĩ���Ӷq
        StandbySPincrements = Matchstick_StandbySPincrements;
        RunningSPincrements = Matchstick_RunningSPincrements;
        RunSpeed = Matchstick_RunSpeed;
        ChargeSPincrements = Matchstick_ChargeSPincrements;

        //�{���ܼ�
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
        Throughing = false;



    }
    Matchstick::~Matchstick()
    {

    }
    void Matchstick::AutoLoadBitmaps(CameraPosition Camera, COLORREF color)
    {
        //���ħP�w��BitRect
        BodyPicture.LoadTexture(color);


        InsertAction("�ݾ�", 1, color);
        InsertAction("����", 4, color);
        InsertAction("�Ĩ�", 1, color);
        InsertAction("���D", 4, color);
        InsertAction("���m", 0, color);
        InsertAction("�m��", 3, color);



        //LoadEffects
        AutoLoadEffections(Camera,color);
        AnimationUpdate(Camera);
    }
    void Matchstick::OnUpdate(BattlePlayer *Enemy, CameraPosition Camera, KeyBoardState KeyState_now, KeyBoardState KeyState_last, Audio_ID Sounds)
    {

        InputJudge(KeyState_now, KeyState_last);

        OnStandby(GPP);
        OnRunning(GPP);
        OnRush(GPP);
        OnJump(GPP);
        OnGuard(GPP);
        OnCharge(GPP);

        //��s�Ҧ�Effect���ʧ@
        map<string, BitmapAnimation>::iterator iter;
        for (iter = Effections.begin(); iter != Effections.end(); iter++)
            EffectAutoUpdate(&(iter->second), (int)(((iter->second).PreAutoFrequence)), false, Camera);


        this->PhysicalMovement(Enemy,Camera, KeyState_now, KeyState_last);
        AnimationUpdate(Camera);
    }
    void Matchstick::OnRush(BattlePlayer *Enemy, CameraPosition Camera, KeyBoardState KeyState_now, KeyBoardState KeyState_last, Audio_ID Sounds)
    {
        if (Action == "�Ĩ�")
        {
            RushTimer += TIMER_TICK_MILLIDECOND;
#pragma region �Ĩ�D�{��
            if (Step == 1)
                Velocity_Y = 0;
            if (RushTimer < 40 && Step == 0)
                Velocity_X = 0;
            if (RushTimer >= 40 && Step == 0)
            {
                RushTimer = 0;
                Step = 1;
                EffectReset(&Effections["Airboost"], Camera, Rect.X, Rect.X + 30, Rect.Y - 30);
                Throughing = true;
                PlaySounds(Sounds.Rush, false);
                if (IsRight)
                {
                    Velocity_X = 35;
                    Acceleration_X = -3.5;
                    Invincible = true;
                }
                else
                {
                    Velocity_X = -35;
                    Acceleration_X = +3.5;
                    Invincible = true;
                }
            }
            else if (RushTimer <= 80 && Step == 1)//�٨�
            {
                if (IsRight&&Velocity_X < 0)
                    Velocity_X = 0;
                else if (IsRight == false && Velocity_X > 0)
                    Velocity_X = 0;
            }
#pragma endregion

            //���`����
            if (RushTimer > 80 && Step == 1)
            {
                RushTimer = 0;
                Throughing = false;
                Invincible = false;
                Velocity_X = 0;
                Acceleration_X = 0;
                Effections["AirBoost"].Step = 0;
                Effections["AirBoost"].visable = false;
                Effections["AirBoost"].OnUpdate("Effects", Camera);
                if (OnGround)
                {
                    GotoStandby(GPP);
                }
                else
                {
                    Action = "���D";
                    Step = 4;
                    JumpTimer = 0;
                }
            }

        }
    }

    

}
