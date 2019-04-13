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
    #pragma region �s�W�ʧ@�о�
    /*
    �p��s�W�@�Ө���ʧ@
    1.�ǳƦn�Ϥ��A�Ϥ��R�W�W�h(�ʧ@�V�k):����W��\�ʧ@�W��_Step.bmp
    �������ǳƦn��A�ǳƥ��k�ۤϪ��Ϥ��A�åB�ɦW��:����W��\�ʧ@�W��_Step_L.bmp

    2.����void Matchstick::AutoLoadBitmaps(GPH)�̭��s�W�ʧ@��InsertAction("�ʧ@�W��", �ʧ@�̤j��, color);

    3.�p�G����������A��void Matchstick::AutoLoadAttacks(GPH)�s�W
    Attacks.InsertAttacks(GetName(), "�ʧ@�W��", ��������̤jStep��, ø�s�ϼh, �Ϥ��ܤƳt��, ��������, color, Camera);

    4.�bvoid Matchstick::OnUpdate(GPH)���s�W�۹������禡

    5.�s�gGoto�禡�A�D�n�O�Ψӱ���i�J�Ӱʧ@�ɪ���l���ܼơC

    6.��BattlePlayer�s�gCanTo�C

    7.�s�gOn�禡�A�ʧ@������A�H�@��IF�P�_�O�_�b�o�Ӱʧ@�̭��A�HSTEP�MTIMER����ʧ@�y�{�A�̫ᵹ���@�q�����ɶ�������i�H���L�i�檺�ʧ@(CanTo)�C
    */
    #pragma endregion

    Matchstick::Matchstick(int number) :BattlePlayer()
    {

        //��O���ܼ�
        HP_Max = Matchstick_HP_Max;//�̤j�ͩR��
        SP_Max = Matchstick_SP_Max;//�̤j��O
        recovery = 0;
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
        HitFly = false;

        Acceleration_gravity = 0.5;


    }
    Matchstick::~Matchstick()
    {

    }

    //Ū�ɦb��
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
        InsertAction("����2", 4, color);
        InsertAction("����3", 4, color);
        InsertAction("����", 2, color);
        InsertAction("���m����", 0, color);
        InsertAction("�S��1", 5, color);
        InsertAction("�Ŵ�1", 4, color);
        InsertAction("�Ŵ�2", 4, color);
        InsertAction("�ŤU��", 7, color);
        InsertAction("�ŤW��", 7, color);
        InsertAction("�W��", 4, color);

        //LoadEffects
        Effects.AutoLoadEffections(color);
        //LoadAttacks
        AutoLoadAttacks(GPP);
        AnimationUpdate(Camera);
    }

    //���U��������
    void Matchstick::AutoLoadAttacks(GPH)
    {
        Attacks.AttackObjects = map<string, AttackObj>();
        Attacks.InsertAttacks(GetName(), "Normal1", 0, 5, 16, 0, color, Camera);
        Attacks.InsertAttacks(GetName(), "Normal2", 0, 5, 16, 0, color, Camera);
        Attacks.InsertAttacks(GetName(), "Normal3", 0, 5, 16, 0, color, Camera);
        Attacks.InsertAttacks(GetName(), "Normal4", 0, 5, 16, 0, color, Camera);
        Attacks.InsertAttacks(GetName(), "Normal5", 0, 5, 16, 0, color, Camera);
        Attacks.InsertAttacks(GetName(), "Skill1", 2, 5, 20, 0, 5, color, Camera);//�h�@�ӰѼƬO�㦳�s����
    }

    //���U�ʧ@�b��
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
        OnNormalAttack2(GPP);
        OnNormalAttack3(GPP);
        OnSkill1(GPP);
        OnAirAttack1(GPP);
        OnAirAttack2(GPP);
        OnAirDownAttack(GPP);
        OnAirUpAttack(GPP);
        OnUpAttack(GPP);

        //��s�Ҧ�Effect���ʧ@
        map<string, BitmapAnimation>::iterator Iter_Effect;
        for (Iter_Effect = Effects.Content.begin(); Iter_Effect != Effects.Content.end(); Iter_Effect++)
            Effects.EffectAutoUpdate(&(Iter_Effect->second), (int)(((Iter_Effect->second).PreAutoFrequence)), false, Camera);

        //��s�Ҧ�Attacks���ʧ@
        map<string, AttackObj>::iterator Iter_Attack;
        for (Iter_Attack = Attacks.AttackObjects.begin(); Iter_Attack != Attacks.AttackObjects.end(); Iter_Attack++)
            Attacks.AttackAutoUpdate(&(Iter_Attack->second), GetName(), (int)(((Iter_Attack->second).PreAutoFrequence)), (Iter_Attack->second).Replay, Camera);


        this->PhysicalMovement(GPP);
        AnimationUpdate(Camera);

        if (recovery > 0)
        {
            GainHP(0.1);
            recovery -= 0.1;
        }
    }


    void Matchstick::GotoRush(GPH)
    {
        if (SP >= Rush_cost)
        {
            GainSP(-Rush_cost);
            Action = "�Ĩ�";
            Step = 0;
            RushTimer = 0;
        }
    }
    void Matchstick::OnRush(GPH)
    {
        if (Action == "�Ĩ�")
        {
            #pragma region �Ĩ�D�{��
            RushTimer += TIMER_TICK_MILLIDECOND;
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

                #pragma region �^���S��
                Effects.Content["AirBoost"].Step = 0;
                Effects.Content["AirBoost"].visable = false;
                Effects.Content["AirBoost"].OnUpdate("Effects", Camera);
                #pragma endregion

                #pragma region �P�_���Ӧ^����Ӱʧ@
                if (OnGround)
                    GotoStandby(GPP);
                else
                    GotoDrop(GPP);
                #pragma endregion

            }

        }
    }

    void Matchstick::GotoNormalAttack1(GPH)
    {
        if (SP >= 2.5)
        {
            GainSP(-2.5);
            Action = "����1";
            Step = 0;
            NormalAttack1Timer = 0;
        }
    }
    void Matchstick::OnNormalAttack1(GPH)
    {
        if (Action == "����1")
        {
            NormalAttack1Timer += TIMER_TICK_MILLIDECOND;

            #pragma region �ʧ@�D��
            //�B�z�����O
            ProduceFriction(1, 1);
            if (NormalAttack1Timer >= 16 && Step <= 2)
            {
                NormalAttack1Timer = 0;
                Step += 1;

                #pragma region ���ͧ�������
                //�X��
                if (Step >= 3)
                {
                    Velocity_X += Ahead(3.5);
                    Attacks.AttackReset
                    (
                        &(Attacks.AttackObjects["Normal1"]), GetName(), this, Enemy,     //���������m,�o�X�̦W��
                        20, 0, 10,                                                                                        //�ˮ`,�d��SP
                        IsRight, 2, 2, Rect.X + 72, Rect.X - 2, Rect.Y + 35,                   //���k,HitX,HitY,XR,XL,Y
                        0, 0,                                                                                          //VX,VY
                        120, 30, -1, false, false, false, true, false, false,                      //�����ɶ�,�����̤j�s���ɶ�,���[�ݩ�,�h�q����,ø�s,���Ƽ���,���������,�i�}��,�i����
                        "PunchHit", Sounds.NormalHit, Camera                         //�����S�ĦW��,�������ĦW��,Camera
                    );
                }
                #pragma endregion


            }
            else if (NormalAttack1Timer >= 40 && Step == 3)
            {
                NormalAttack1Timer = 0;
                Step = 4;
            }
            #pragma endregion

            #pragma region ��O���ʧ@
            if (NormalAttack1Timer < 100 && Step >= 4)
            {
                //��O���ʧ@
                CanToNormalAttack2;
                CanToSkill1;
                CanToJump;
                CanToRush;
                CanToUpAttack;
            }
            else if (NormalAttack1Timer >= 100 && Step >= 4)
            {
                //���`����
                GotoStandby(GPP);
            }
            #pragma endregion

        }
    }

    void Matchstick::GotoNormalAttack2(GPH)
    {
        if (SP >= 2.5)
        {
            GainSP(-2.5);
            Action = "����2";
            Step = 0;
            NormalAttack1Timer = 0;
        }
    }
    void Matchstick::OnNormalAttack2(GPH)
    {
        if (Action == "����2")
        {
            NormalAttack1Timer += TIMER_TICK_MILLIDECOND;
            #pragma region �ʧ@�D��
            //�B�z�����O
            ProduceFriction(1, 1);
            if (NormalAttack1Timer >= 16 && Step <= 2)
            {
                NormalAttack1Timer = 0;
                Step += 1;
                if (Step >= 3)
                {
                    Velocity_X += Ahead(3.5);
                    #pragma region ���ͧ�������
                    //�X��
                    Attacks.AttackReset
                    (
                        &(Attacks.AttackObjects["Normal1"]), GetName(), this, Enemy,     //���������m,�o�X�̦W��
                        20, 0, 10,                                                                        //�ˮ`,�d��SP
                        IsRight, 3, 2, Rect.X + 72, Rect.X - 2, Rect.Y + 35,                   //���k,HitX,HitY,XR,XL,Y
                        0, 0,                                                                                          //VX,VY
                        120, 30, -1, false, false, false, true, false, false,                      //�����ɶ�,�����̤j�s���ɶ�,���[�ݩ�,�h�q����,ø�s,���Ƽ���,���������,�i�}��,�i����
                        "PunchHit", Sounds.NormalHit, Camera                         //�����S�ĦW��,�������ĦW��,Camera
                    );
                    #pragma endregion
                }
            }
            else if (NormalAttack1Timer >= 50 && Step == 3)
            {
                NormalAttack1Timer = 0;
                Step = 4;
            }
            #pragma endregion

            #pragma region ��O���ʧ@
            if (NormalAttack1Timer < 100 && Step >= 4)
            {
                //��O���ʧ@
                CanToNormalAttack3;
                CanToSkill1;
                CanToJump;
                CanToRush;
                CanToUpAttack;

            }
            else if (NormalAttack1Timer >= 100 && Step >= 4)
            {
                //���`����
                GotoStandby(GPP);
            }
            #pragma endregion

        }
    }

    void Matchstick::GotoNormalAttack3(GPH)
    {
        if (SP >= 5)
        {
            GainSP(-5);
            Velocity_X += Ahead(5);
            Velocity_Y -= 5;
            Action = "����3";
            Step = 0;
            NormalAttack1Timer = 0;
        }
    }
    void Matchstick::OnNormalAttack3(GPH)
    {
        if (Action == "����3")
        {
            NormalAttack1Timer += TIMER_TICK_MILLIDECOND;
            #pragma region �ʧ@�D��
            ProduceFriction(0.2, 0.25);
            if (NormalAttack1Timer >= 84 && Step <= 2)
            {
                Step += 1;
                if (Step >= 3)
                {
                    #pragma region ���ͧ�������
                    //�X��
                    Attacks.AttackReset
                    (
                        &(Attacks.AttackObjects["Normal2"]), GetName(), this, Enemy,     //���������m,�o�X�̦W��
                        35, 0, 10,                                                                                  //�ˮ`,�d��SP
                        IsRight, 12, 7.5, Rect.X + 98, Rect.X - 7, Rect.Y + 17,                   //���k,HitX,HitY,XR,XL,Y
                        0, 0,                                                                                          //VX,VY
                        200, 30, -1, false, false, false, true, false, true,                      //�����ɶ�,�����̤j�s���ɶ�,���[�ݩ�,�h�q����,ø�s,���Ƽ���,���������,�i�}��,�i����
                        "PunchHit", Sounds.NormalHit, Camera                         //�����S�ĦW��,�������ĦW��,Camera
                    );
                    #pragma endregion
                }
            }
            else if (NormalAttack1Timer >= 150 && Step == 3)
            {
                NormalAttack1Timer = 0;
                Step = 4;
            }
            #pragma endregion

            #pragma region ��O���ʧ@
            if (NormalAttack1Timer < 100 && Step >= 4)
            {
                CanToJump;
                CanToRush;
                CanToUpAttack;
            }
            else if (NormalAttack1Timer >= 100 && Step >= 4)
            {
                //���`����
                GotoStandby(GPP);
            }
            #pragma endregion

        }
    }

    void Matchstick::GotoSkill1(GPH)
    {
        if (SP >= 3)
        {
            GainSP(-3);
            if (Velocity_Y > 0 && OnGround == false)
                Velocity_Y = 0;
            Action = "�S��1";
            Step = 0;
            Shot1Timer = 0;
        }
    }
    void Matchstick::OnSkill1(GPH)
    {
        if (Action == "�S��1")
        {

            Shot1Timer += TIMER_TICK_MILLIDECOND;

            #pragma region �ʧ@�D��
            if (OnGround)
                ProduceFriction(1, 1);
            else
                ProduceFriction(0.15, 0.75);
            if (Shot1Timer >= 70 && Step == 0)
            {
                Step = 1;
                if (Velocity_Y > 0 && OnGround == false)
                    Velocity_Y = 0;
            }
            if (Shot1Timer >= 16 && Step >= 1 && Step <= 3)
            {
                Shot1Timer = 0;
                Step += 1;
                if (Step == 3)
                {
                    #pragma region ���ͧ�������
                    //�X��
                    Attacks.AttackReset
                    (
                        &(Attacks.AttackObjects["Skill1_" + IntToString(Shot1Current)]), GetName(), this, Enemy,     //���������m,�o�X�̦W��
                        15, 0, 2,                                                                            //�ˮ`,�d��SP
                        IsRight, 2, 3.5, Rect.X + 50, Rect.X + 10, Rect.Y + 35,                   //���k,HitX,HitY,XR,XL,Y
                        Ahead(3), 0,                                                                                          //VX,VY
                        120, 1000, -1, false, true, true, true, false, false,                      //�����ɶ�,�����̤j�s���ɶ�,���[�ݩ�,�h�q����,ø�s,���Ƽ���,���������,�i�}��,�i����
                        "PunchHit", Sounds.NormalHit, Camera                         //�����S�ĦW��,�������ĦW��,Camera
                    );
                    Shot1Current += 1;
                    if (Shot1Current == 5)
                    {
                        Shot1Current = 0;
                    }
                    #pragma endregion
                }
            }
            else if (Shot1Timer >= 50 && Step == 4)
            {
                Shot1Timer = 0;
                Step = 5;
            }
            #pragma endregion

            #pragma region ��O���ʧ@
            if (Shot1Timer < 100 && Step >= 5)
            {
                //��O���i��ʧ@
                if (OnGround)
                {
                    CanToNormalAttack1;
                    CanToUpAttack;
                }
                else
                {
                    CanToAirUpAttack;
                    CanToAirDownAttack;
                    CanToAirAttack1;
                    CanToFastDrop;
                }

                CanToRush;
                CanToJump;
            }
            else if (Shot1Timer >= 100 && Step >= 5)
            {
                //���`����
                if (OnGround)
                    GotoStandby(GPP);
                else
                    GotoDrop(GPP);
            }
            #pragma endregion

        }
    }

    void Matchstick::GotoAirAttack1(GPH)
    {
        if (SP >= 2.5)
        {
            SP -= 2.5;
            if (SP <= 0)
            {
                SP = 0;
            }
            Action = "�Ŵ�1";
            Step = 0;
            if (Velocity_Y > 0)
            {
                Velocity_Y = 0;
            }
            NormalAttack1Timer = 0;
        }
    }
    void Matchstick::OnAirAttack1(GPH)
    {
        if (Action == "�Ŵ�1")
        {

            NormalAttack1Timer += TIMER_TICK_MILLIDECOND;

            #pragma region ���k����
            if (CanControl&&Button_now.button_Right == false && CanControl&&Button_now.button_Left == false)
            {
                ProduceFriction(0.15, 1);
            }
            else if (CanControl&&Button_now.button_Right == true)
            {
                IsRight = true;
                RunAhead(0.5, RunSpeed / 2);
            }
            else if (CanControl&&Button_now.button_Left == true)
            {
                IsRight = false;
                RunAhead(0.5, RunSpeed / 2);
            }
            #pragma endregion

            #pragma region �ʧ@�D��
            if (NormalAttack1Timer >= 50 && Step == 0)
            {
                Step = 1;
            }
            if (NormalAttack1Timer >= 16 && Step >= 1 && Step <= 3)
            {
                NormalAttack1Timer = 0;
                Step += 1;
                #pragma region ���ͧ�������
                if (Step == 3)
                {
                    //�X��
                    Attacks.AttackReset
                    (
                        &(Attacks.AttackObjects["Normal3"]), GetName(), this, Enemy,     //���������m,�o�X�̦W��
                        15, 0, 10,                                                                                 //�ˮ`,�d��SP
                        IsRight, 2, 5, Rect.X + 72, Rect.X - 2, Rect.Y + 60,         //���k,HitX,HitY,XR,XL,Y
                        0, 0,                                                                                          //VX,VY
                        150, 30, -1, false, false, false, true, false, false,              //�����ɶ�,�����̤j�s���ɶ�,���[�ݩ�,�h�q����,ø�s,���Ƽ���,���������,�i�}��,�i����
                        "PunchHit", Sounds.NormalHit, Camera                       //�����S�ĦW��,�������ĦW��,Camera
                    );
                }
                #pragma endregion
            }
            else if (NormalAttack1Timer >= 125 && Step == 3)
            {
                NormalAttack1Timer = 0;
                Step = 4;
            }
            #pragma endregion

            #pragma region ��O���ʧ@
            if (NormalAttack1Timer < 125 && Step >= 4)
            {
                //��O���i��ʧ@
                CanToAirAttack2;
                CanToSkill1;
                CanToRush;
                CanToAirDownAttack;
                CanToAirUpAttack;
                CanToJump;
                CanToFastDrop;
            }
            else if (NormalAttack1Timer >= 125 && Step >= 4)
            {
                //���`����
                if (OnGround)
                    GotoStandby(GPP);
                else
                    GotoDrop(GPP);
            }
            #pragma endregion

        }
    }

    void Matchstick::GotoAirAttack2(GPH)
    {
        if (SP >= 5)
        {
            SP -= 5;
            if (SP <= 0)
            {
                SP = 0;
            }
            Action = "�Ŵ�2";
            Step = 0;
            Velocity_X += Ahead(4);
            Velocity_Y = -3.5;
            NormalAttack1Timer = 0;
        }
    }
    void Matchstick::OnAirAttack2(GPH)
    {
        if (Action == "�Ŵ�2")
        {
            ProduceFriction(0.1, 0.15);
            NormalAttack1Timer += TIMER_TICK_MILLIDECOND;
            if (NormalAttack1Timer >= 84 && Step <= 2)
            {
                Step += 1;
                if (Step >= 3)
                {
                    //�X��
                    Attacks.AttackReset
                    (
                        &(Attacks.AttackObjects["Normal2"]), GetName(), this, Enemy,     //���������m,�o�X�̦W��
                        35, 0, 10,                                                                                //�ˮ`,�d��SP
                        IsRight, 12, 7.5, Rect.X + 95, Rect.X, Rect.Y + 40,                   //���k,HitX,HitY,XR,XL,Y
                        Velocity_X / 3, 0,                                                                                          //VX,VY
                        250, 50, -1, false, false, false, true, false, true,                      //�����ɶ�,�����̤j�s���ɶ�,���[�ݩ�,�h�q����,ø�s,���Ƽ���,���������,�i�}��,�i����
                        "PunchHit", Sounds.NormalHit, Camera                         //�����S�ĦW��,�������ĦW��,Camera
                    );
                }
            }
            else if (NormalAttack1Timer >= 150 && Step == 3)
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
                if (OnGround)
                    GotoStandby(GPP);
                else
                    GotoDrop(GPP);
            }

        }
    }

    void Matchstick::GotoUpAttack(GPH)
    {
        if (SP >= 5)
        {
            GainSP(-5);
            Action = "�W��";
            Step = 0;
            NormalAttack1Timer = 0;
        }
    }
    void Matchstick::OnUpAttack(GPH)
    {
        if (Action == "�W��")
        {
            NormalAttack1Timer += TIMER_TICK_MILLIDECOND;

            #pragma region �ʧ@�D��
            //�B�z�����O
            ProduceFriction(1, 1);
            if (NormalAttack1Timer >= 80 && Step == 0)
            {
                NormalAttack1Timer = 0;
                Step = 1;
            }
            else if (NormalAttack1Timer >= 16 && Step >= 1 && Step <= 2)
            {
                NormalAttack1Timer = 0;
                Step += 1;
                #pragma region ���ͧ�������
                //�X��
                if (Step == 3)
                {
                    Velocity_X += Ahead(7);
                    Attacks.AttackReset
                    (
                        &(Attacks.AttackObjects["Normal2"]), GetName(), this, Enemy,     //���������m,�o�X�̦W��
                        35, 0, 10,                                                                            //�ˮ`,�d��SP
                        IsRight, 2, 12, Rect.X + 98, Rect.X - 7, Rect.Y + 17,                   //���k,HitX,HitY,XR,XL,Y
                        0, 0,                                                                                          //VX,VY
                        225, 30, -1, false, false, false, true, false, false,                      //�����ɶ�,�����̤j�s���ɶ�,���[�ݩ�,�h�q����,ø�s,���Ƽ���,���������,�i�}��,�i����
                        "PunchHit", Sounds.NormalHit, Camera                         //�����S�ĦW��,�������ĦW��,Camera
                    );
                }
                #pragma endregion
            }
            else if (NormalAttack1Timer >= 40 && Step == 3)
            {
                NormalAttack1Timer = 0;
                Step = 4;
            }
            #pragma endregion

            #pragma region ��O���ʧ@
            if (NormalAttack1Timer < 100 && Step >= 4)
            {
                //��O���ʧ@
                CanToNormalAttack1;
                CanToSkill1;
                CanToJump;
                CanToRush;
            }
            else if (NormalAttack1Timer >= 100 && Step >= 4)
            {
                //���`����
                GotoStandby(GPP);
            }
            #pragma endregion

        }
    }

    void Matchstick::GotoDownAttack(GPH)
    {
    }
    void Matchstick::OnDownAttack(GPH)
    {
    }

    void Matchstick::GotoRushAttack(GPH)
    {
    }
    void Matchstick::OnRushAttack(GPH)
    {
    }

    void Matchstick::GotoAirUpAttack(GPH)
    {
        if (SP >= 5)
        {
            GainSP(-5);
            Action = "�ŤW��";
            Step = 0;
            if (Velocity_Y > 0)
                Velocity_Y = 0;
            NormalAttack1Timer = 0;
        }
    }
    void Matchstick::OnAirUpAttack(GPH)
    {
        if (Action == "�ŤW��")
        {
            #pragma region ���k����
            if (CanControl&&Button_now.button_Right == false && CanControl&&Button_now.button_Left == false)
            {
                ProduceFriction(0.15, 1);
            }
            else if (CanControl&&Button_now.button_Right == true)
            {
                IsRight = true;
                RunAhead(0.5, RunSpeed / 2);
            }
            else if (CanControl&&Button_now.button_Left == true)
            {
                IsRight = false;
                RunAhead(0.5, RunSpeed / 2);
            }
            #pragma endregion

            #pragma region �ʧ@�D��
            NormalAttack1Timer += TIMER_TICK_MILLIDECOND;
            if (NormalAttack1Timer >= 100 && Step == 0)
            {
                Step = 1;
                if (Velocity_Y > 1)
                    Velocity_Y = 1;
            }
            if (NormalAttack1Timer >= 16 && Step >= 1 && Step <= 3)
            {
                NormalAttack1Timer = 0;
                Step += 1;
                Velocity_Y = -6;

                #pragma region ���ͧ�������
                if (Step == 2)
                {
                    Attacks.AttackReset
                    (
                        &(Attacks.AttackObjects["Normal5"]), GetName(), this, Enemy,     //���������m,�o�X�̦W��
                        20, 0, 10,                                                                   //�ˮ`,�d��SP�Z���O��
                        IsRight, 2.5, 12, Rect.X + 20, Rect.X, Rect.Y,         //���k,HitX,HitY,XR,XL,Y
                        Velocity_X / 4, 0,                                                                   //VX,VY
                        250, 30, -1, false, false, false, true, false, false,            //�����ɶ�,�����̤j�s���ɶ�,���[�ݩ�,�h�q����,ø�s,���Ƽ���,���������,�i�}��,�i����
                        "PunchHit", Sounds.NormalHit, Camera                       //�����S�ĦW��,�������ĦW��,Camera
                    );
                }
                #pragma endregion

            }
            else if (NormalAttack1Timer >= 16 && Step >= 4 && Step <= 6)
            {
                if (Velocity_Y > 2)
                    Velocity_Y = 2;
                NormalAttack1Timer = 0;
                Step += 1;
            }
            #pragma endregion

            #pragma region ��O���ʧ@
            if (NormalAttack1Timer < 100 && Step >= 7)
            {
                if (Velocity_Y > 1)
                    Velocity_Y = 1;
                //��O���i��ʧ@
                CanToSkill1;
                CanToRush;
                CanToJump;
                CanToAirAttack1;
                CanToAirDownAttack;
                CanToFastDrop;
            }
            else if (NormalAttack1Timer >= 100 && Step >= 7)
            {
                //���`����
                if (OnGround)
                    GotoStandby(GPP);
                else
                    GotoDrop(GPP);
            }
            #pragma endregion
        }
    }

    void Matchstick::GotoAirDownAttack(GPH)
    {
        if (SP >= 6)
        {
            GainSP(-6);
            Action = "�ŤU��";
            Step = 0;
            if (Velocity_Y > 0)
            {
                Velocity_Y = 0;
            }
            NormalAttack1Timer = 0;
        }
    }
    void Matchstick::OnAirDownAttack(GPH)
    {
        if (Action == "�ŤU��")
        {
            NormalAttack1Timer += TIMER_TICK_MILLIDECOND;

            #pragma region ���k����
            if (CanControl&&Button_now.button_Right == false && CanControl&&Button_now.button_Left == false)
            {
                ProduceFriction(0.15, 1);
            }
            else if (CanControl&&Button_now.button_Right == true)
            {
                IsRight = true;
                RunAhead(0.5, RunSpeed / 2);
            }
            else if (CanControl&&Button_now.button_Left == true)
            {
                IsRight = false;
                RunAhead(0.5, RunSpeed / 2);
            }
            #pragma endregion

            #pragma region �ʧ@�D��
            if (NormalAttack1Timer >= 75 && Step == 0)
            {
                if (Velocity_Y > 1)
                    Velocity_Y = 1;
                Step = 1;
            }
            if (NormalAttack1Timer >= 8 && Step >= 1 && Step <= 3)
            {
                NormalAttack1Timer = 0;
                Step += 1;
            }
            else if (NormalAttack1Timer >= 20 && Step >= 4 && Step <= 6)
            {
                NormalAttack1Timer = 0;
                Step += 1;
                #pragma region ���ͧ�������
                if (Step == 5)
                {
                    Attacks.AttackReset
                    (
                        &(Attacks.AttackObjects["Normal4"]), GetName(), this, Enemy,    //���������m,�o�X�̦W��
                        20, 0, 10,                                                                                        //�ˮ`,�d��SP
                        IsRight, 2.5, -16, Rect.X + 60, Rect.X, Rect.Y + 10,         //���k,HitX,HitY,XR,XL,Y
                        Velocity_X / 2, Velocity_Y / 3,                                                                   //VX,VY
                        200, 30, -1, false, false, false, true, false, false,            //�����ɶ�,�����̤j�s���ɶ�,���[�ݩ�,�h�q����,ø�s,���Ƽ���,���������,�i�}��,�i����
                        "PunchHit", Sounds.NormalHit, Camera                       //�����S�ĦW��,�������ĦW��,Camera
                    );
                }
                #pragma endregion
            }
            #pragma endregion

            #pragma region ��O���ʧ@
            if (NormalAttack1Timer > 50 && NormalAttack1Timer < 120 && Step >= 7)
            {
                if (Velocity_Y > 1)
                    Velocity_Y = 1;
                //��O���i��ʧ@
                CanToSkill1;
                CanToRush;
                CanToJump;
                CanToFastDrop;
            }
            else if (NormalAttack1Timer >= 120 && Step >= 7)
            {
                //���`����
                if (OnGround)
                    GotoStandby(GPP);
                else
                    GotoDrop(GPP);
            }
            #pragma endregion
        }
    }

    void Matchstick::GotoUpSkill(GPH)
    {
    }
    void Matchstick::OnUpSkill(GPH)
    {
    }

    void Matchstick::GotoDownSkill(GPH)
    {
    }
    void Matchstick::OnDownSkill(GPH)
    {
    }

    void Matchstick::GotoRushSkill(GPH)
    {
    }
    void Matchstick::OnRushSkill(GPH)
    {
    }

    void Matchstick::GotoAirUpSkill(GPH)
    {
    }
    void Matchstick::OnAirUpSkill(GPH)
    {
    }

    void Matchstick::GotoAirDownSkill(GPH)
    {
    }
    void Matchstick::OnAirDownSkill(GPH)
    {
    }



}
