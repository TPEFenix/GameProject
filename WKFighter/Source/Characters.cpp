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


        //��l�ƯS��
        Effect_Rush = new BitmapAnimation("Airboost", false);
        Effect_Jump = new BitmapAnimation("Airboost2", false);

    }
    Matchstick::~Matchstick()
    {
        delete Effect_Rush;
        delete Effect_Jump;
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
        InsertBitmapPicture("���D", 0, color);
        InsertBitmapPicture("���D", 1, color);
        InsertBitmapPicture("���D", 2, color);
        InsertBitmapPicture("���D", 3, color);
        InsertBitmapPicture("���D", 4, color);



        //LoadEffects
        Effect_Rush->AutoLoadBitmaps("Effects", "Airboost", 6, false, color);
        Effect_Jump->AutoLoadBitmaps("Effects", "Airboost2", 6, false, color);

        AnimationUpdate(Camera);
    }
    void Matchstick::OnUpdate(BattlePlayer *Enemy, CameraPosition Camera, KeyBoardState KeyState_now, KeyBoardState KeyState_last, Audio_ID Sounds)
    {
        InputJudge(KeyState_now, KeyState_last);

        OnStandby(Enemy, Camera, KeyState_now, KeyState_last, Sounds);
        OnRunning(Enemy, Camera, KeyState_now, KeyState_last, Sounds);
        OnRush(Enemy, Camera, KeyState_now, KeyState_last, Sounds);
        OnJump(Enemy, Camera, KeyState_now, KeyState_last, Sounds);


        EffectAutoUpdate(Effect_Rush, 8, false, Camera);
        EffectAutoUpdate(Effect_Jump, 8, false, Camera);
        AnimationUpdate(Camera);
        this->PhysicalMovement(Camera, KeyState_now, KeyState_last);
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
                EffectReset(Effect_Rush, Camera, Rect.X, Rect.X + 30, Rect.Y - 30);
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
                Invincible = false;
                Velocity_X = 0;
                Acceleration_X = 0;
                Effect_Rush->Step = 0;
                Effect_Rush->visable = false;
                Effect_Rush->OnUpdate("Effects", Camera);
                if (OnGround)
                {
                    GotoStandby(Enemy, Camera, KeyState_now, KeyState_last, Sounds);
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
    void Matchstick::OnJump(BattlePlayer *Enemy, CameraPosition Camera, KeyBoardState KeyState_now, KeyBoardState KeyState_last, Audio_ID Sounds)
    {
        if (Action == "���D")
        {
            JumpTimer += TIMER_TICK_MILLIDECOND;
#pragma region ���D�D�{��
            if (JumpTimer >= 10 && Step < 2)
            {
                Step += 1;
                JumpTimer = 0;
            }
            else if (JumpTimer >= 30 && Step == 2)
            {
                Step = 3;
                JumpTimer = 0;
                Velocity_Y = -9;
                OnGround = false;
                EffectReset(Effect_Jump,Camera, Rect.X-30, Rect.X-35, Rect.Y + 80);
                PlaySounds(Sounds.Rush, false);
            }
            else if (Step == 3 && Velocity_Y < 0)
            {
                JumpTimer += TIMER_TICK_MILLIDECOND;
                if (CanControl&&Button_now.button_Jump == true && Button_last.button_Jump == true && Velocity_Y < 2 && JumpTimer < 120)
                    Velocity_Y -= 0.5;
            }
            else if (Velocity_Y >= 0 && Step == 3)
            {
                JumpTimer = 0;
                Step = 4;
            }
#pragma endregion
#pragma region �_����
            if (Step >= 3)
            {  
#pragma region �Ť�����
                if (CanControl&&Button_now.button_Right == false && CanControl&&Button_now.button_Left == false)
                {
                    ProduceFriction(0.15, 1);
                }
                else if (CanControl&&Button_now.button_Right == true)
                {
                    IsRight = true;
                    RunAhead(0.5, RunSpeed/2);
                }
                else if (CanControl&&Button_now.button_Left == true)
                {
                    IsRight = false;
                    RunAhead(0.5, RunSpeed/2);
                }
#pragma endregion

#pragma region ��O���ʧ@
                //���`���a
                if (OnGround)
                {
                    GotoStandby(Enemy, Camera, KeyState_now, KeyState_last, Sounds);
                }
                else if (CanControl&&Button_now.button_Rush&& Button_last.button_Rush == false)
                {
                    GotoRush(Enemy, Camera, KeyState_now, KeyState_last, Sounds);
                }
#pragma endregion
            }
#pragma endregion       
        }
    }
    void Matchstick::Draw(int i, int j)
    {
        this->DisplayBitmap->Draw(i, j);
        this->Effect_Rush->DisplayBitmap->Draw(i, j);
        this->Effect_Jump->DisplayBitmap->Draw(i, j);
    }

}
