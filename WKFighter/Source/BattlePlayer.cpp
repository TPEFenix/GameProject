#pragma once
#include "stdafx.h"
#include <ddraw.h>
#include <windows.h>
#include <stdio.h>
#include <sstream>
#include "gamelib.h"
#include "WKBitmap.h"
#include "Keycode.h"
#include "KeyBoardState.h"
#include "WKAudio.h"
#include "CollisionSensor.h"
#include "EffectSprite.h"
#include "BattlePlayer.h"
#include "FunctionUser.h"



using namespace std;
using namespace WKAudio_namespace;
using namespace CollisionSensor_namespace;
using namespace FunctionUser_namespace;

namespace game_framework
{
    BattlePlayer::BattlePlayer() :BitmapAnimation()
    {
        BodyPicture = BitmapPicture("Content\\Bitmaps\\BodyRect.bmp", 0, 0, true, true, true);
        AttributeState = vector<bool>(10, false);
        Effects = EffectSprite();
        Attacks = AttackManager();
        NeedCutIn = false;
    }
    BattlePlayer::~BattlePlayer()
    {

    }


    void BattlePlayer::AnimationUpdate(CameraPosition Camera)
    {
        #pragma region 確定圖檔名稱
        //確定圖檔名稱
        if (IsRight)
        {
            string Actionstring = "Content\\Bitmaps\\" + GetName() + "\\" + Action + "_" + IntToString(Step) + ".bmp";
            char *cc = new char[65535];
            strcpy(cc, Actionstring.c_str());
            DisplayBitmap = &BitmapPictures[cc];
            delete[] cc;
        }
        else
        {
            string Actionstring = "Content\\Bitmaps\\" + GetName() + "\\" + Action + "_" + IntToString(Step) + "_L.bmp";
            char *cc = new char[65535];
            strcpy(cc, Actionstring.c_str());
            DisplayBitmap = &BitmapPictures[cc];
            delete[] cc;
        }
        #pragma endregion 
        #pragma region 決定相對座標
        //決定相對座標
        Rect.Width = DisplayBitmap->Rect.Width;
        Rect.Height = DisplayBitmap->Rect.Height;
        if (InSideCamera)
        {
            Rect.X_int = (int)(Rect.X - Camera.X);
            Rect.Y_int = (int)(Rect.Y - Camera.Y);
        }
        else
        {
            Rect.X_int = (int)(Rect.X);
            Rect.Y_int = (int)(Rect.Y);
        }
        DisplayBitmap->Rect.X = Rect.X_int;
        DisplayBitmap->Rect.Y = Rect.Y_int;
        BodyRect.X = (Rect.X + 43);
        BodyRect.Y = (Rect.Y + 27);
        BodyRect.Width = 34;
        BodyRect.Height = 80;
        BodyPicture.Rect.X = BodyRect.X;
        BodyPicture.Rect.Y = BodyRect.Y;
        BodyPicture.OnUpdate(Camera);
        DisplayBitmap->OnUpdate();
        #pragma endregion 

    }
    void BattlePlayer::OnUpdate(GPH)
    {
        AnimationUpdate(Camera);
    }
    void BattlePlayer::PhysicalMovement(GPH)
    {
        #pragma region 基礎移動
        Rect.X += Velocity_X;
        Rect.Y += Velocity_Y;
        Velocity_X += Acceleration_X;
        Velocity_Y += Acceleration_Y + (Acceleration_gravity*0.9);
        if (Velocity_Y > MaxGravity)//如果大於最大正常下降速度，則不加重力加速度
        {
            Velocity_Y -= Acceleration_gravity;
        }
        if (Rect.Y >= GroundPosition)
        {
            Rect.Y = GroundPosition;
            if (this->Action == "受傷"&&OnGround == false && this->Velocity_Y > 14)
            {
                this->Velocity_Y *= -0.5;
                this->HP -= 15;
                this->HitFly = true;
                this->BeHitTimeMax += 200;
                PlaySounds(Sounds.HitWall, false);
                Sleep(100);
                OnGround = false;
                BreakPoint += 30;
                if (BreakPoint > 90)
                {
                    BreakPoint = 90;
                }
            }
            else
            {
                Velocity_Y = 0;
                OnGround = true;
            }

            Acceleration_Y = 0;
        }
        else if (Rect.Y < GroundPosition)
        {
            OnGround = false;
            if (Action == "待機")
            {
                Action = "跳躍";
                Step = 4;
            }
        }

        if (BitmapPicture_HitRectangle((this->BodyRect), (Enemy->BodyRect)) == true && this->Throughing == false && Enemy->Throughing == false)
        {

            if (Velocity_X == 0 && Enemy->Velocity_X == 0 && Enemy->BodyRect.X >= BodyRect.X)
            {
                while (BitmapPicture_HitRectangle((this->BodyRect), (Enemy->BodyRect)))
                {
                    Rect.X -= 1;
                    Rect.X_int -= 1;
                    BodyRect.X -= 1;
                    Enemy->Rect.X += 1;
                    Enemy->Rect.X_int += 1;
                    Enemy->BodyRect.X += 1;
                }
            }
            if (Velocity_X > 0 && Enemy->Velocity_X == 0 && Enemy->BodyRect.X >= BodyRect.X)
            {
                while (BitmapPicture_HitRectangle((this->BodyRect), (Enemy->BodyRect)))
                {
                    Rect.X -= 1;
                    Rect.X_int -= 1;
                    BodyRect.X -= 1;
                    Enemy->Rect.X += 1;
                    Enemy->Rect.X_int += 1;
                    Enemy->BodyRect.X += 1;
                }
            }
            if (Velocity_X < 0 && Enemy->Velocity_X == 0 && Enemy->BodyRect.X <= BodyRect.X)
            {
                while (BitmapPicture_HitRectangle((this->BodyRect), (Enemy->BodyRect)))
                {
                    Rect.X += 1;
                    Rect.X_int += 1;
                    BodyRect.X += 1;
                    Enemy->Rect.X -= 1;
                    Enemy->Rect.X_int -= 1;
                    Enemy->BodyRect.X -= 1;
                }
            }
            if (Velocity_X > 0 && Enemy->Velocity_X < 0 && Enemy->BodyRect.X >= BodyRect.X)
            {
                while (BitmapPicture_HitRectangle((this->BodyRect), (Enemy->BodyRect)))
                {
                    Rect.X -= 1;
                    Rect.X_int -= 1;
                    BodyRect.X -= 1;
                    Enemy->Rect.X += 1;
                    Enemy->Rect.X_int += 1;
                    Enemy->BodyRect.X += 1;
                }
            }
        }


        #pragma endregion

    }
    void BattlePlayer::Draw(int i, int j, CameraPosition Camera)
    {
        this->DisplayBitmap->Draw(i, j);
        Effects.DrawAllEffection(i);
        Attacks.DrawAllAttacks(i);
    }
    void BattlePlayer::AutoLoadBitmaps(GPH)
    {
    }
    void BattlePlayer::InsertBitmapPicture(string action, int step, COLORREF color)
    {
        string str;
        str = ("Content\\Bitmaps\\" + GetName() + "\\" + action + "_" + IntToString(step) + ".bmp");
        BitmapPictures.insert(std::pair<string, BitmapPicture>(str, BitmapPicture(visable)));
        char *cr = new char[65535];
        strcpy(cr, str.c_str());
        BitmapPictures[str].LoadTexture(cr, false, color);
        str = ("Content\\Bitmaps\\" + GetName() + "\\" + action + "_" + IntToString(step) + "_L.bmp");
        BitmapPictures.insert(std::pair<string, BitmapPicture>(str, BitmapPicture(visable)));
        char *cl = new char[65535];
        strcpy(cl, str.c_str());
        BitmapPictures[str].LoadTexture(cl, false, color);
        delete[] cr;
        delete[] cl;
    }
    void BattlePlayer::InsertAction(string actionname, int maxstep, COLORREF color)
    {
        for (int i = 0; i <= maxstep; i += 1)
        {
            InsertBitmapPicture(actionname, i, color);
        }
    }
    void BattlePlayer::InputJudge(KeyBoardState KeyState_now, KeyBoardState KeyState_last)
    {
        if (PlayerNumber == 1)
        {
            Button_now.button_Attack = KeyState_now.Player1_Attack;
            Button_now.button_Down = KeyState_now.Player1_Down;
            Button_now.button_Guard = KeyState_now.Player1_Guard;
            Button_now.button_Jump = KeyState_now.Player1_Jump;
            Button_now.button_Left = KeyState_now.Player1_Left;
            Button_now.button_Right = KeyState_now.Player1_Right;
            Button_now.button_Rush = KeyState_now.Player1_Rush;
            Button_now.button_Skill = KeyState_now.Player1_Skill;
            Button_now.button_Technique = KeyState_now.Player1_Technique;
            Button_now.button_Up = KeyState_now.Player1_Up;
            Button_last.button_Attack = KeyState_last.Player1_Attack;
            Button_last.button_Down = KeyState_last.Player1_Down;
            Button_last.button_Guard = KeyState_last.Player1_Guard;
            Button_last.button_Jump = KeyState_last.Player1_Jump;
            Button_last.button_Left = KeyState_last.Player1_Left;
            Button_last.button_Right = KeyState_last.Player1_Right;
            Button_last.button_Rush = KeyState_last.Player1_Rush;
            Button_last.button_Skill = KeyState_last.Player1_Skill;
            Button_last.button_Technique = KeyState_last.Player1_Technique;
            Button_last.button_Up = KeyState_last.Player1_Up;
        }
        else if (PlayerNumber == 2)
        {
            Button_now.button_Attack = KeyState_now.Player2_Attack;
            Button_now.button_Down = KeyState_now.Player2_Down;
            Button_now.button_Guard = KeyState_now.Player2_Guard;
            Button_now.button_Jump = KeyState_now.Player2_Jump;
            Button_now.button_Left = KeyState_now.Player2_Left;
            Button_now.button_Right = KeyState_now.Player2_Right;
            Button_now.button_Rush = KeyState_now.Player2_Rush;
            Button_now.button_Skill = KeyState_now.Player2_Skill;
            Button_now.button_Technique = KeyState_now.Player2_Technique;
            Button_now.button_Up = KeyState_now.Player2_Up;
            Button_last.button_Attack = KeyState_last.Player2_Attack;
            Button_last.button_Down = KeyState_last.Player2_Down;
            Button_last.button_Guard = KeyState_last.Player2_Guard;
            Button_last.button_Jump = KeyState_last.Player2_Jump;
            Button_last.button_Left = KeyState_last.Player2_Left;
            Button_last.button_Right = KeyState_last.Player2_Right;
            Button_last.button_Rush = KeyState_last.Player2_Rush;
            Button_last.button_Skill = KeyState_last.Player2_Skill;
            Button_last.button_Technique = KeyState_last.Player2_Technique;
            Button_last.button_Up = KeyState_last.Player2_Up;
        }
    }
    void BattlePlayer::CheckHit(GPH)
    {
        //20190413已將此程式碼移植至AttackObj所屬
        /*
        //可受傷狀態
        if (this->Invincible == false)
        {
            map<string, AttackObj>::iterator iter;
            for (iter = Enemy->Attacks.AttackObjects.begin(); iter != Enemy->Attacks.AttackObjects.end(); iter++)
            {
                if (iter->second.visable && (iter->second.IsHited == false || iter->second.CanCombo))
                {
                    if (PixelCollision(&(this->BodyPicture), iter->second.DisplayBitmap, 2))
                    {
                        #pragma region 防禦狀態

                        if ((Action == "防禦" || Action == "防禦受傷") && (iter->second.BitmapisRight != IsRight) && iter->second.HitBreak == false)
                        {
                            IsRight = !(iter->second.BitmapisRight);
                            iter->second.IsHited = true;
                            if (iter->second.HitNoon == true)
                            {
                                iter->second.visable = false;
                                iter->second.DisplayBitmap->visable = false;
                                iter->second.Drawable = false;
                            }

                            PlaySounds(iter->second.HitSound, false);
                            Effects.BootEffect(&(Effects.Content[iter->second.HitEffect]), Camera, BodyRect.X + 3, BodyRect.X - 6, Rect.Y + 30, 0, 0, false, iter->second.BitmapisRight);

                            GainHP(-(iter->second.Damage / 3));
                            GainSP(-(iter->second.Damage / 10));
                            GainSP(-(iter->second.SP_Damege / 2));

                            Velocity_X = iter->second.Ahead(iter->second.HitVelocity_X) / 3;
                            Velocity_Y -= 0;

                            BeHitTimer = 0;
                            BeHitTimeMax = (iter->second.HitTime / 2.5);

                            Sleep(25);
                            Action = "防禦受傷";
                            Step = 0;
                        }
                        #pragma endregion

                        #pragma region 非防禦狀態或無法防禦
                        if (!((Action == "防禦" || Action == "防禦受傷") && (iter->second.BitmapisRight != IsRight) && iter->second.HitBreak == false))
                        {
                            IsRight = !(iter->second.BitmapisRight);
                            iter->second.IsHited = true;
                            if (iter->second.HitNoon == true)
                            {
                                iter->second.visable = false;
                                iter->second.DisplayBitmap->visable = false;
                                iter->second.Drawable = false;
                            }

                            PlaySounds(iter->second.HitSound, false);
                            Effects.BootEffect(&(Effects.Content[iter->second.HitEffect]), Camera, BodyRect.X + 3, BodyRect.X - 6, Rect.Y + 30, 0, 0, false, iter->second.BitmapisRight);

                            GainHP(-(iter->second.Damage));
                            GainSP(+(iter->second.Damage / 15));
                            GainSP(-iter->second.SP_Damege);
                            if (((int)HP) > 0)
                                recovery = recovery + (iter->second.Damage / 1.5);
                            else
                                recovery = 0;

                            if (-iter->second.Attributes >= 0)
                                AttributeState[-iter->second.Attributes] = true;


                            Velocity_X = iter->second.Ahead(iter->second.HitVelocity_X);
                            Velocity_Y = -(iter->second.HitVelocity_Y);

                            BeHitTimer = 0;
                            BeHitTimeMax = iter->second.HitTime;

                            Sleep(25);
                            HitFly = iter->second.CanHitFly;
                            Step = 0;

                            Action = "受傷";
                        }
                        #pragma endregion
                    }
                }

            }
        }
        */
    }


    void BattlePlayer::GotoStandby(GPH)
    {
        Action = "待機";
        Step = 0;
        RunningTimer = 0;
        StandbyTimer = 0;
        RushTimer = 0;
    }
    void BattlePlayer::OnStandby(GPH)
    {
        if (Action == "待機")
        {
            StandbyTimer += TIMER_TICK_MILLIDECOND;
            AddSP(StandbySPincrements);

            #pragma region 處理摩擦力
            if (Button_now.button_Left == false && Button_now.button_Right == false)
                ProduceFriction(1, 1);
            #pragma endregion

            #pragma region 待機擺頭動作
            if (StandbyTimer >= 200)
            {
                StandbyTimer = 0;
                if (Step == 0)
                    Step = 1;
                else if (Step == 1)
                    Step = 0;
            }
            #pragma endregion

            #pragma region 到別的動作

            #pragma region 左右移動
            if (CanControl&&Button_now.button_Right&&OnGround)
            {
                IsRight = true;
                GotoRunning(GPP);
            }
            else if (CanControl&&Button_now.button_Left&&OnGround)
            {
                IsRight = false;
                GotoRunning(GPP);
            }
            #pragma endregion


            CanToCharge;
            CanToGuard;
            CanToRush;
            CanToJump;
            CanToNormalAttack1;
            CanToSkill1;
            CanToUpAttack;
            CanToDownAttack;
            CanToUpSkill;
            CanToUltimateSkill;
            #pragma endregion

        }
    }

    void BattlePlayer::GotoRunning(GPH)
    {
        Action = "移動";
        Step = 0;
        RunningTimer = 0;
    }
    void BattlePlayer::OnRunning(GPH)
    {
        if (Action == "移動")
        {

            AddSP(RunningSPincrements);
            RunningTimer += TIMER_TICK_MILLIDECOND;
            #pragma region 左右移動
            if (CanControl&&IsRight&&OnGround && (Button_now.button_Right == true || Button_now.button_Left == true))
            {
                if (Button_now.button_Left == true)
                    IsRight = false;
                if (RunningTimer >= 25)
                {
                    RunningTimer = 0;
                    LoopStep(4);
                }

                RunAhead(0.75, RunSpeed);
            }
            else if (CanControl&&IsRight == false && OnGround && (Button_now.button_Right == true || Button_now.button_Left == true))
            {
                if (Button_now.button_Right == true)
                    IsRight = true;
                if (RunningTimer >= 25)
                {
                    RunningTimer = 0;
                    LoopStep(4);
                }
                RunAhead(0.75, RunSpeed);
            }
            #pragma endregion

            #pragma region 到別的動作
            CanToCharge;
            CanToGuard;
            CanToJump;
            CanToNormalAttack1;
            CanToRush;
            CanToSkill1;
            CanToUpAttack;
            CanToUpSkill;

            //正常結束(左右放開)
            if (Button_now.button_Right == false && Button_now.button_Left == false)
                CanToStandby;
            #pragma endregion

        }
    }

    void BattlePlayer::GotoRush(GPH)
    {
        if (this->SP >= Rush_cost)
        {
            this->SP -= Rush_cost;
            Action = "衝刺";
            Step = 0;
            RushTimer = 0;
        }
    }
    void BattlePlayer::OnRush(GPH)
    {
    }

    void BattlePlayer::FastDrop(GPH)
    {
        if (SP >= 4)
        {
            GainSP(-4);
            PlaySounds(Sounds.Rush, false);
            PlayEffect(this, "Airboost3", Camera, Rect.X - 20, Rect.X - 20, Rect.Y);
            Velocity_Y = 12;
        }

    }

    void BattlePlayer::GotoJump(GPH)
    {
        if (OnGround)
        {
            Action = "跳躍";
            Step = 0;
            RushTimer = 0;
            JumpTimer = 0;
        }
        else
        {
            if (SP >= 10)
            {
                GainSP(-10);
                Action = "跳躍";
                RushTimer = 0;
                Step = 3;
                JumpTimer = 0;
                Velocity_Y = -10;
                OnGround = false;
                PlayEffect(this, "Airboost2", Camera, Rect.X - 30, Rect.X - 35, Rect.Y + 80);
                PlaySounds(Sounds.Jump, false);
            }
        }
    }
    void BattlePlayer::OnJump(GPH)
    {
        if (Action == "跳躍")
        {
            JumpTimer += TIMER_TICK_MILLIDECOND;
            #pragma region 跳躍主程序
            if (JumpTimer >= 10 && Step < 2)
            {
                ProduceFriction(0.15, 1);
                Step += 1;
                JumpTimer = 0;
            }
            else if (JumpTimer >= 20 && Step == 2)
            {
                Step = 3;
                JumpTimer = 0;
                Velocity_Y = -10;
                OnGround = false;
                PlayEffect(this, "Airboost2", Camera, Rect.X - 30, Rect.X - 35, Rect.Y + 80);
                PlaySounds(Sounds.Jump, false);
            }
            else if (Step == 3 && Velocity_Y < 0)
            {
                if (CanControl&&Button_now.button_Jump == true && Velocity_Y < 2 && JumpTimer < 120)
                    Velocity_Y -= 0.25;
            }
            else if (Velocity_Y >= 0 && Step == 3)
            {
                JumpTimer = 0;
                Step = 4;
            }
            #pragma endregion

            #pragma region 起跳後
            if (Step >= 3)
            {
                #pragma region 空中移動
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

                #pragma region 到別的動作

                CanToAirAttack1;
                CanToRush;
                CanToSkill1;
                CanToAirDownAttack;
                CanToAirUpAttack;
                CanToJump;
                CanToFastDrop;
                //正常落地
                if (OnGround)
                    CanToStandby;

                #pragma endregion
            }
            #pragma endregion       
        }
    }

    void BattlePlayer::GotoGuard(GPH)
    {
        if (this->SP > 5)
        {
            Action = "防禦";
            Step = 0;
        }
    }
    void BattlePlayer::OnGuard(GPH)
    {
        if (Action == "防禦")
        {
            if (SP <= 0 || Button_now.button_Guard == false)
            {
                GotoStandby(GPP);
            }
            else
            {
                ProduceFriction(1, 1);
                SP -= GuardSPCost;
            }

            #pragma region 到別的動作
            if (CanControl&&Button_now.button_Down == true && Button_last.button_Down == false)
            {
                GotoCharge(GPP);
            }
            #pragma endregion


        }
    }

    void BattlePlayer::GotoCharge(GPH)
    {
        Action = "練氣";
        Step = 0;
        ChargeTimer = 0;
        //啟動特效
        PlayEffect(this, "SPCharge", Camera, Rect.X - 35, Rect.X - 30, Rect.Y - 45);
    }
    void BattlePlayer::OnCharge(GPH)
    {
        if (Action == "練氣")
        {
            ProduceFriction(1, 1);
            ChargeTimer2 += TIMER_TICK_MILLIDECOND;
            ChargeTimer += TIMER_TICK_MILLIDECOND;

            if (ChargeTimer >= 50 && Step == 0)
            {
                Step = 1;
                ChargeTimer = 0;
            }
            else if (ChargeTimer >= 16 && Step >= 1 && Step < 3)
            {
                Step += 1;
                ChargeTimer = 0;
                if (Step == 3)
                {
                    ChargeTimer2 = 0;
                    Chargecount = 0;
                    PlaySounds(Sounds.SPCharge, false);
                }
            }
            else if (ChargeTimer2 >= 10 && Step == 3)
            {
                Chargecount += 1;
                if (Chargecount < 10)
                {
                    SP += ChargeSPincrements / 10;
                    if (SP > SP_Max)
                        SP = SP_Max;
                }
                ChargeTimer2 = 0;
            }
            else if (ChargeTimer >= 90 && Step == 3)//正常結束
            {
                ChargeTimer = 0;
                if (Button_now.button_Guard == true)
                {
                    GotoGuard(GPP);
                }
                else
                {
                    GotoStandby(GPP);
                }
            }


        }
    }

    void BattlePlayer::OnHit(GPH)
    {
        NotHitTimer += TIMER_TICK_MILLIDECOND;
        if (Action == "受傷")
        {
            OnHitTimer += TIMER_TICK_MILLIDECOND;
            ProduceFriction(0.25, 0.35);
            BeHitTimer += TIMER_TICK_MILLIDECOND;
            if (Effects.Content["stun_star"].visable == true)
            {
                Effects.Content["stun_star"].Rect.X = Rect.X;
                Effects.Content["stun_star"].Rect.Y = Rect.Y;
                Effects.Content["stun_star"].loop = true;
            }
            if (HitFly)
            {
                Acceleration_Y = -0.075;
            }
            if (BeHitTimer >= BeHitTimeMax&&HitFly == false)
            {
                BeHitTimer = 0;
                BeHitTimeMax = 0;
                OnGround = false;
                HitFly = false;
                GotoStandby(GPP);
                Effects.Content["stun_star"].loop = false;
            }
            if (HitFly == true)
            {
                if (Step == 0)
                    Step = 1;
                if (BeHitTimer >= 80 && BeHitTimer <= 180 && Button_now.button_Jump&&Button_last.button_Jump == false && OnGround == false)
                {
                    if (SP >= 10)
                    {
                        GainSP(-10);
                        Action = "跳躍";
                        RushTimer = 0;
                        Step = 3;
                        JumpTimer = 0;
                        Velocity_Y = -10;
                        OnGround = false;
                        Effects.Content["stun_star"].loop = false;
                        PlayEffect(this, "Airboost2", Camera, Rect.X - 30, Rect.X - 35, Rect.Y + 80);
                        PlaySounds(Sounds.Jump, false);
                    }
                }
                else if (BeHitTimer > 300 || (BeHitTimer > 100 && OnGround))
                {
                    if (OnGround&&Velocity_Y <= 14)
                    {
                        if (Step < 2)
                        {
                            BeHitTimer = 0;
                            Invincible = true;
                            Throughing = true;
                            Step = 2;
                            Acceleration_Y = 0;
                        }
                    }
                }
            }
            if (Step >= 2 && BeHitTimer < 500 && BeHitTimer >= 200 && OnGround&&Button_now.button_Jump&&Button_last.button_Jump == false)
            {
                GainSP(8);
                GotoJump(GPP);
                Effects.Content["stun_star"].loop = false;
                Step = 3;
                JumpTimer = 0;
                Velocity_Y = -1;
                Velocity_X /= 2;
                OnGround = false;
                HitFly = false;
                BeHitTimer = 0;
                Invincible = false;
                Throughing = false;
                HitFly = false;
                PlayEffect(this, "Airboost2", Camera, Rect.X - 30, Rect.X - 35, Rect.Y + 80);
                PlaySounds(Sounds.Jump, false);
                Acceleration_Y = 0;
            }
            if (Step >= 2 && BeHitTimer >= 500)
            {
                BeHitTimer = 0;
                Invincible = false;
                Throughing = false;
                HitFly = false;
                GotoStandby(GPP);
                Effects.Content["stun_star"].loop = false;
                Acceleration_Y = 0;
            }
            if (NotHitTimer - OnHitTimer > 1500)
            {
                recovery = 0;
                OnHitTimer = 0;
                NotHitTimer = 0;
            }
        }
    }
    void BattlePlayer::OnHitGuard(GPH)
    {
        if (Action == "防禦受傷")
        {
            ProduceFriction(0.5, 0.5);
            BeHitTimer += TIMER_TICK_MILLIDECOND;
            if (BeHitTimer >= BeHitTimeMax)
            {
                BeHitTimer = 0;
                BeHitTimeMax = 0;
                if (Button_now.button_Guard)
                {
                    if (SP > 0)
                    {
                        GotoGuard(GPP);
                    }
                    else
                    {
                        GotoStandby(GPP);
                    }
                }
                else
                {
                    GotoStandby(GPP);
                }
            }
        }
    }

    void BattlePlayer::AddSP(double mathin)
    {
        if (SP < SP_Max)
        {
            SPincrementsTimer += TIMER_TICK_MILLIDECOND;
            if (SPincrementsTimer >= 25)
            {
                SPincrementsTimer = 0;
                SP += mathin;
                if (SP > SP_Max)
                {
                    SP = SP_Max;
                }
            }
        }
    }
    void BattlePlayer::GainHP(double mathin)
    {
        HP += mathin;
        if (HP > HP_Max)
            HP = HP_Max;
        else if (HP <= 0)
            HP = 0;
    }
    void BattlePlayer::GainSP(double mathin)
    {
        SP += mathin;
        if (SP > SP_Max)
            SP = SP_Max;
        else if (SP <= 0)
            SP = 0;
    }
    void BattlePlayer::ProduceFriction(double power, double range)
    {
        if (Velocity_X <= range && Velocity_X >= -range)
        {
            Velocity_X = 0;
        }
        else if (Velocity_X > range)
        {
            Velocity_X -= power;
        }
        else if (Velocity_X < -range)
        {
            Velocity_X += power;
        }
    }
    void BattlePlayer::GotoDrop(GPH)
    {
        Action = "跳躍";
        Step = 4;
        JumpTimer = 0;
    }
    void BattlePlayer::LoopStep(int maxstep)
    {
        if (Step <= maxstep)
        {
            Step += 1;
            if (Step == maxstep)
            {
                Step = 0;
            }
        }
    }
    void BattlePlayer::RunAhead(double Addspeed, double Maxspeed)
    {
        if (IsRight)
        {
            if (Velocity_X < Maxspeed)
            {
                Velocity_X += Addspeed;
            }

        }
        else
        {
            if (Velocity_X > -Maxspeed)
            {
                Velocity_X -= Addspeed;
            }

        }
    }
    double BattlePlayer::Ahead(double move)
    {
        double returner = 0;
        if (IsRight)
        {
            returner = move;
        }
        else
        {
            returner = -move;
        }
        return returner;
    }


}

