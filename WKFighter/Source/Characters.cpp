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
#include "AttackObj.h"
#include "Characters.h"
#include "EffectSprite.h"



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
    void Matchstick::AutoLoadBitmaps(GPH)
    {
        //���ħP�w��BitRect
        BodyPicture.LoadTexture(color);


        InsertAction("�ݾ�", 1, color);
        InsertAction("����", 4, color);
        InsertAction("�Ĩ�", 1, color);
        InsertAction("���D", 4, color);
        InsertAction("���m", 0, color);
        InsertAction("�m��", 3, color);
        InsertAction("����1", 4, color);
        InsertAction("����", 0, color);
        InsertAction("���m����", 0, color);
        //LoadEffects
        Effects.AutoLoadEffections(color);
        //LoadAttacks
        AutoLoadAttacks(GPP);
        AnimationUpdate(Camera);
    }
    void Matchstick::OnUpdate(GPH)
    {

        InputJudge(KeyState_now, KeyState_last);
        CheckHit(GPP);
        OnStandby(GPP);
        OnRunning(GPP);
        OnRush(GPP);
        OnJump(GPP);
        OnGuard(GPP);
        OnCharge(GPP);
        OnHit(GPP);
        OnHitGuard(GPP);
        OnNormalAttack1(GPP);

        //��s�Ҧ�Effect���ʧ@
        map<string, BitmapAnimation>::iterator Iter_Effect;
        for (Iter_Effect = Effects.Content.begin(); Iter_Effect != Effects.Content.end(); Iter_Effect++)
            Effects.EffectAutoUpdate(&(Iter_Effect->second), (int)(((Iter_Effect->second).PreAutoFrequence)), false, Camera);

        //��s�Ҧ�Attacks���ʧ@
        map<string, AttackObj>::iterator Iter_Attack;
        for (Iter_Attack = Attacks.AttackObjects.begin(); Iter_Attack != Attacks.AttackObjects.end(); Iter_Attack++)
            Attacks.AttackAutoUpdate(&(Iter_Attack->second), GetName(), (int)(((Iter_Attack->second).PreAutoFrequence)), (Iter_Attack->second).Replay, Camera);


        this->PhysicalMovement(Enemy, Camera, KeyState_now, KeyState_last);
        AnimationUpdate(Camera);
    }
    void Matchstick::OnRush(GPH)
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
                Effects.BootEffect(&Effects.Content["Airboost"], Camera, Rect.X, Rect.X + 30, Rect.Y - 30, 0, 0, false, IsRight);
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
                Effects.Content["AirBoost"].Step = 0;
                Effects.Content["AirBoost"].visable = false;
                Effects.Content["AirBoost"].OnUpdate("Effects", Camera);
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

    void Matchstick::GotoNormalAttack1(GPH)
    {
        if (SP >= 3)
        {
            SP -= 3;
            if (SP <= 0)
            {
                SP = 0;
            }
            Action = "����1";
            Step = 0;
            NormalAttack1Timer = 0;
        }
    }
    void Matchstick::OnNormalAttack1(GPH)
    {
        if (Action == "����1")
        {
            ProduceFriction(1, 1);
            NormalAttack1Timer += TIMER_TICK_MILLIDECOND;
            if (NormalAttack1Timer >= 16 && Step <= 2)
            {
                NormalAttack1Timer = 0;
                Step += 1;
                if (Step >= 3)
                {
                    Velocity_X += Ahead(3.5);
                    //�X��
                    Attacks.AttackReset
                        (
                            &(Attacks.AttackObjects["Normal1"]),GetName(),     //���������m,�o�X�̦W��
                            20,0,                                                                                        //�ˮ`,�d��SP
                            IsRight,2,2,Rect.X+72,Rect.X-2,Rect.Y+35,                   //���k,HitX,HitY,XR,XL,Y
                            0,0,                                                                                          //VX,VY
                            100,30,-1,false,false,false,true,false,                                 //�����ɶ�,�����̤j�s���ɶ�,���[�ݩ�,�h�q����,ø�s,���Ƽ���,���������,�}��
                            "PunchHit",Sounds.NormalHit,Camera                         //�����S�ĦW��,�������ĦW��,Camera
                        );
                }
            }
            else if (NormalAttack1Timer >= 80 && Step == 3)
            {
                NormalAttack1Timer = 0;
                Step = 4;
            }
            else if (NormalAttack1Timer < 100 && Step >= 4)
            {
                //��O���ʧ@
            }
            else if (NormalAttack1Timer >= 100 && Step >= 4)
            {
                //���`����
                GotoStandby(GPP);
            }

        }
    }

    void Matchstick::AutoLoadAttacks(GPH)
    {
        Attacks.AttackObjects = map<string, AttackObj>();
        Attacks.InsertAttacks(GetName(), "Normal1", 0,5, 16,0, color, Camera);
    }



}
