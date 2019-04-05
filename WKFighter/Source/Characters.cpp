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

        //能力值變數
        HP_Max = Matchstick_HP_Max;//最大生命值
        SP_Max = Matchstick_SP_Max;//最大氣力
        recovery = 0;
        Rush_cost = Matchstick_Rush_Cost;//衝刺消耗量
        StandbySPincrements = Matchstick_StandbySPincrements;
        RunningSPincrements = Matchstick_RunningSPincrements;
        RunSpeed = Matchstick_RunSpeed;
        ChargeSPincrements = Matchstick_ChargeSPincrements;

        //現狀變數
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
        HP = HP_Max;//當前生命
        SP = SP_Max;//當前氣力
        CanControl = false;//可以控制
        Invincible = false;//無敵狀態
        Action = "待機";//動作狀態
        Step = 0;//當前步驟數
        visable = true;//是否可見
        CanPixelCollision = true;//是否使用像素碰撞，所有動作分割都會套用
        InSideCamera = true;//是否受到鏡頭影響
        Velocity_X = 0;//X速度
        Velocity_Y = 0;//Y速度
        Acceleration_X = 0;//X加速度
        Acceleration_Y = 0;//Y加速度
        Throughing = false;
        HitFly = false;

        Acceleration_gravity = 0.5;


    }
    Matchstick::~Matchstick()
    {

    }

    //讀檔在此
    void Matchstick::AutoLoadBitmaps(GPH)
    {
        //有效判定區BitRect
        BodyPicture.LoadTexture(color);


        InsertAction("待機", 1, color);
        InsertAction("移動", 4, color);
        InsertAction("衝刺", 1, color);
        InsertAction("跳躍", 4, color);
        InsertAction("防禦", 0, color);
        InsertAction("練氣", 3, color);
        InsertAction("普攻1", 4, color);
        InsertAction("普攻2", 4, color);
        InsertAction("普攻3", 4, color);
        InsertAction("受傷", 2, color);
        InsertAction("防禦受傷", 0, color);
        InsertAction("特技1", 5, color);
        InsertAction("空普1", 4, color);
        InsertAction("空普2", 4, color);

        //LoadEffects
        Effects.AutoLoadEffections(color);
        //LoadAttacks
        AutoLoadAttacks(GPP);
        AnimationUpdate(Camera);
    }

    //註冊攻擊物件
    void Matchstick::AutoLoadAttacks(GPH)
    {
        Attacks.AttackObjects = map<string, AttackObj>();
        Attacks.InsertAttacks(GetName(), "Normal1", 0, 5, 16, 0, color, Camera);
        Attacks.InsertAttacks(GetName(), "Normal2", 0, 5, 16, 0, color, Camera);
        Attacks.InsertAttacks(GetName(), "Normal3", 0, 5, 16, 0, color, Camera);
        Attacks.InsertAttacks(GetName(), "Skill1", 2, 5, 20, 0, 5, color, Camera);//多一個參數是具有編號的
    }

    //註冊動作在此
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

        //更新所有Effect的動作
        map<string, BitmapAnimation>::iterator Iter_Effect;
        for (Iter_Effect = Effects.Content.begin(); Iter_Effect != Effects.Content.end(); Iter_Effect++)
            Effects.EffectAutoUpdate(&(Iter_Effect->second), (int)(((Iter_Effect->second).PreAutoFrequence)), false, Camera);

        //更新所有Attacks的動作
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





    void Matchstick::OnRush(GPH)
    {
        if (Action == "衝刺")
        {
            RushTimer += TIMER_TICK_MILLIDECOND;
#pragma region 衝刺主程序
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
            else if (RushTimer <= 80 && Step == 1)//煞車
            {
                if (IsRight&&Velocity_X < 0)
                    Velocity_X = 0;
                else if (IsRight == false && Velocity_X > 0)
                    Velocity_X = 0;
            }
#pragma endregion

            //正常結束
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
                    Action = "跳躍";
                    Step = 4;
                    JumpTimer = 0;
                }
            }

        }
    }

    void Matchstick::GotoNormalAttack1(GPH)
    {
        if (SP >= 4)
        {
            SP -= 4;
            if (SP <= 0)
            {
                SP = 0;
            }
            Action = "普攻1";
            Step = 0;
            NormalAttack1Timer = 0;
        }
    }
    void Matchstick::OnNormalAttack1(GPH)
    {
        if (Action == "普攻1")
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
                    //出拳
                    Attacks.AttackReset
                    (
                        &(Attacks.AttackObjects["Normal1"]), GetName(),     //攻擊物件位置,發出者名稱
                        20, 0,                                                                                        //傷害,削減SP
                        IsRight, 2, 2, Rect.X + 72, Rect.X - 2, Rect.Y + 35,                   //左右,HitX,HitY,XR,XL,Y
                        0, 0,                                                                                          //VX,VY
                        180, 30, -1, false, false, false, true, false, false,                      //僵直時間,攻擊最大存活時間,附加屬性,多段攻擊,繪製,重複播放,擊中後消失,可破防,可擊飛
                        "PunchHit", Sounds.NormalHit, Camera                         //擊中特效名稱,擊中音效名稱,Camera
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
                //到別的動作
                CanToNormalAttack2;
                CanToSkill1;
                CanToJump;
                CanToRush;
            }
            else if (NormalAttack1Timer >= 100 && Step >= 4)
            {
                //正常結束
                GotoStandby(GPP);
            }

        }
    }
    void Matchstick::GotoNormalAttack2(GPH)
    {
        if (SP >= 4)
        {
            SP -= 4;
            if (SP <= 0)
            {
                SP = 0;
            }
            Action = "普攻2";
            Step = 0;
            NormalAttack1Timer = 0;
        }
    }
    void Matchstick::OnNormalAttack2(GPH)
    {
        if (Action == "普攻2")
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
                    //出拳
                    Attacks.AttackReset
                    (
                        &(Attacks.AttackObjects["Normal1"]), GetName(),     //攻擊物件位置,發出者名稱
                        20, 0,                                                                                        //傷害,削減SP
                        IsRight, 3, 2, Rect.X + 72, Rect.X - 2, Rect.Y + 35,                   //左右,HitX,HitY,XR,XL,Y
                        0, 0,                                                                                          //VX,VY
                        180, 30, -1, false, false, false, true, false, false,                      //僵直時間,攻擊最大存活時間,附加屬性,多段攻擊,繪製,重複播放,擊中後消失,可破防,可擊飛
                        "PunchHit", Sounds.NormalHit, Camera                         //擊中特效名稱,擊中音效名稱,Camera
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
                //到別的動作
                CanToNormalAttack3;
                CanToSkill1;
                CanToJump;
                CanToRush;
            }
            else if (NormalAttack1Timer >= 100 && Step >= 4)
            {
                //正常結束
                GotoStandby(GPP);
            }

        }
    }
    void Matchstick::GotoNormalAttack3(GPH)
    {
        if (SP >= 5)
        {
            SP -= 5;
            if (SP <= 0)
            {
                SP = 0;
            }
            Velocity_X += Ahead(5);
            Velocity_Y -= 5;
            Action = "普攻3";
            Step = 0;
            NormalAttack1Timer = 0;
        }
    }
    void Matchstick::OnNormalAttack3(GPH)
    {
        if (Action == "普攻3")
        {
            ProduceFriction(0.2, 0.25);
            NormalAttack1Timer += TIMER_TICK_MILLIDECOND;
            if (NormalAttack1Timer >= 84 && Step <= 2)
            {
                Step += 1;
                if (Step >= 3)
                {
                    //出拳
                    Attacks.AttackReset
                    (
                        &(Attacks.AttackObjects["Normal2"]), GetName(),     //攻擊物件位置,發出者名稱
                        35, 0,                                                                                        //傷害,削減SP
                        IsRight, 12, 7.5, Rect.X + 98, Rect.X - 7, Rect.Y + 17,                   //左右,HitX,HitY,XR,XL,Y
                        0, 0,                                                                                          //VX,VY
                        200, 30, -1, false, false, false, true, false, true,                      //僵直時間,攻擊最大存活時間,附加屬性,多段攻擊,繪製,重複播放,擊中後消失,可破防,可擊飛
                        "PunchHit", Sounds.NormalHit, Camera                         //擊中特效名稱,擊中音效名稱,Camera
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
                CanToJump;
                CanToRush;
            }
            else if (NormalAttack1Timer >= 100 && Step >= 4)
            {
                //正常結束
                GotoStandby(GPP);
            }

        }
    }

    void Matchstick::GotoSkill1(GPH)
    {
        if (SP >= 4)
        {
            SP -= 4;
            if (SP <= 0)
            {
                SP = 0;
            }
            if (Velocity_Y > 0 && OnGround == false)
            {
                Velocity_Y = 0;
            }
            Action = "特技1";
            Step = 0;
            Shot1Timer = 0;
        }
    }
    void Matchstick::OnSkill1(GPH)
    {
        if (Action == "特技1")
        {
            if (OnGround)
                ProduceFriction(1, 1);
            else
                ProduceFriction(0.15, 0.75);
            Shot1Timer += TIMER_TICK_MILLIDECOND;
            if (Shot1Timer >= 50 && Step == 0)
            {
                Step = 1;
                if (Velocity_Y > 0 && OnGround == false)
                    Velocity_Y = 0;

            }
            if (Shot1Timer >= 16 && Step >= 1 && Step <= 4)
            {
                Shot1Timer = 0;
                Step += 1;
                if (Step == 4)
                {
                    //出拳
                    Attacks.AttackReset
                    (
                        &(Attacks.AttackObjects["Skill1_" + IntToString(Shot1Current)]), GetName(),     //攻擊物件位置,發出者名稱
                        12, 0,                                                                                        //傷害,削減SP
                        IsRight, 2, 3.5, Rect.X + 50, Rect.X + 10, Rect.Y + 35,                   //左右,HitX,HitY,XR,XL,Y
                        Ahead(3), 0,                                                                                          //VX,VY
                        150, 1000, -1, false, true, true, true, false, false,                      //僵直時間,攻擊最大存活時間,附加屬性,多段攻擊,繪製,重複播放,擊中後消失,可破防,可擊飛
                        "PunchHit", Sounds.NormalHit, Camera                         //擊中特效名稱,擊中音效名稱,Camera
                    );
                    Shot1Current += 1;
                    if (Shot1Current == 5)
                    {
                        Shot1Current = 0;
                    }
                }
            }
            else if (Shot1Timer >= 50 && Step == 4)
            {
                Shot1Timer = 0;
                Step = 5;
            }
            else if (Shot1Timer < 100 && Step >= 5)
            {
                //到別的可能動作
                CanToNormalAttack1;
                CanToRush;
                if (OnGround)
                {
                    CanToJump;
                }
            }
            else if (Shot1Timer >= 100 && Step >= 5)
            {
                //正常結束
                if (OnGround)
                {
                    GotoStandby(GPP);
                }
                else
                {
                    Action = "跳躍";
                    Step = 4;
                    JumpTimer = 0;
                }
            }

        }
    }

    void Matchstick::GotoAirAttack1(GPH)
    {
        if (SP >= 4)
        {
            SP -= 4;
            if (SP <= 0)
            {
                SP = 0;
            }
            Action = "空普1";
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
        if (Action == "空普1")
        {
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


            NormalAttack1Timer += TIMER_TICK_MILLIDECOND;
            if (NormalAttack1Timer >= 50 && Step == 0)
            {
                Step = 1;
            }
            if (NormalAttack1Timer >= 16 && Step >= 1 && Step <= 3)
            {
                NormalAttack1Timer = 0;
                Step += 1;
                if (Step == 3)
                {
                    //出拳
                    Attacks.AttackReset
                    (
                        &(Attacks.AttackObjects["Normal3"]), GetName(),     //攻擊物件位置,發出者名稱
                        15, 0,                                                                                        //傷害,削減SP
                        IsRight, 2, 5, Rect.X + 72, Rect.X - 2, Rect.Y + 70,         //左右,HitX,HitY,XR,XL,Y
                        0, 0,                                                                                          //VX,VY
                        150, 30, -1, false, false, false, true, false, false,              //僵直時間,攻擊最大存活時間,附加屬性,多段攻擊,繪製,重複播放,擊中後消失,可破防,可擊飛
                        "PunchHit", Sounds.NormalHit, Camera                       //擊中特效名稱,擊中音效名稱,Camera
                    );
                }
            }
            else if (NormalAttack1Timer >= 125 && Step == 3)
            {
                NormalAttack1Timer = 0;
                Step = 4;
            }
            else if (NormalAttack1Timer < 125 && Step >= 4)
            {
                //到別的可能動作
                CanToAirAttack2;
                CanToSkill1;
                CanToRush;
            }
            else if (NormalAttack1Timer >= 125 && Step >= 4)
            {
                //正常結束
                if (OnGround)
                {
                    GotoStandby(GPP);
                }
                else
                {
                    Action = "跳躍";
                    Step = 4;
                    JumpTimer = 0;
                }
            }

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
            Action = "空普2";
            Step = 0;
            Velocity_X += Ahead(4);
            Velocity_Y = -3.5;
            NormalAttack1Timer = 0;
        }
    }
    void Matchstick::OnAirAttack2(GPH)
    {
        if (Action == "空普2")
        {



            ProduceFriction(0.1, 0.15);
            NormalAttack1Timer += TIMER_TICK_MILLIDECOND;
            if (NormalAttack1Timer >= 84 && Step <= 2)
            {
                Step += 1;
                if (Step >= 3)
                {
                    //出拳
                    Attacks.AttackReset
                    (
                        &(Attacks.AttackObjects["Normal2"]), GetName(),     //攻擊物件位置,發出者名稱
                        35, 0,                                                                                        //傷害,削減SP
                        IsRight, 12, 7.5, Rect.X + 95, Rect.X, Rect.Y + 40,                   //左右,HitX,HitY,XR,XL,Y
                        Velocity_X / 3, 0,                                                                                          //VX,VY
                        200, 50, -1, false, false, false, true, false, true,                      //僵直時間,攻擊最大存活時間,附加屬性,多段攻擊,繪製,重複播放,擊中後消失,可破防,可擊飛
                        "PunchHit", Sounds.NormalHit, Camera                         //擊中特效名稱,擊中音效名稱,Camera
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
                //到別的動作
            }
            else if (NormalAttack1Timer >= 100 && Step >= 4)
            {
                //正常結束
                if (OnGround)
                {
                    GotoStandby(GPP);
                }
                else
                {
                    Action = "跳躍";
                    Step = 4;
                    JumpTimer = 0;
                }
            }

        }
    }







}
