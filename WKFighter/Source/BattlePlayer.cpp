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
#include "BattlePlayer.h"

using namespace std;
using namespace WKAudio_namespace;

namespace game_framework
{
    BattlePlayer::BattlePlayer() :BitmapAnimation()
    {
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
        DisplayBitmap->OnUpdate();
#pragma endregion 

    }
    void BattlePlayer::OnUpdate(BattlePlayer *Enemy, CameraPosition Camera, KeyBoardState KeyState_now, KeyBoardState KeyState_last, Audio_ID Sounds)
    {
        AnimationUpdate(Camera);
    }
    void BattlePlayer::PhysicalMovement(CameraPosition Camera, KeyBoardState KeyState_now, KeyBoardState KeyState_last)
    {
#pragma region 基礎移動
        Rect.X += Velocity_X;
        Rect.Y += Velocity_Y;
        Velocity_X += Acceleration_X;
        Velocity_Y += Acceleration_Y + Acceleration_gravity;
        if (Rect.Y >= GroundPosition)
        {
            Rect.Y = GroundPosition;
            OnGround = true;
        }
        else if (Rect.Y < GroundPosition)
        {
            OnGround = false;
        }


#pragma endregion

    }
    void BattlePlayer::Draw(int i, int j)
    {
        this->DisplayBitmap->Draw(i, j);
    }
    void BattlePlayer::AutoLoadBitmaps(CameraPosition Camera, COLORREF color)
    {
    }
    void BattlePlayer::InsertBitmapPicture(string action, int step, COLORREF color)
    {
        string str;
        str = ("Content\\Bitmaps\\" + GetName() + "\\" + action + "_" + IntToString(step) + ".bmp");
        BitmapPictures.insert(std::pair<string, BitmapPicture>(str, BitmapPicture(visable)));
        char *cr = new char[65535];
        strcpy(cr, str.c_str());
        BitmapPictures[str].LoadTexture(cr, true, color);
        str = ("Content\\Bitmaps\\" + GetName() + "\\" + action + "_" + IntToString(step) + "_L.bmp");
        BitmapPictures.insert(std::pair<string, BitmapPicture>(str, BitmapPicture(visable)));
        char *cl = new char[65535];
        strcpy(cl, str.c_str());
        BitmapPictures[str].LoadTexture(cl, true, color);
        delete[] cr;
        delete[] cl;
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
    void BattlePlayer::GotoStandby(BattlePlayer *Enemy, CameraPosition Camera, KeyBoardState KeyState_now, KeyBoardState KeyState_last, Audio_ID Sounds)
    {
        Action = "待機";
        Step = 0;
        RunningTimer = 0;
        StandbyTimer = 0;
        RushTimer = 0;
    }
    void BattlePlayer::GotoRunning(BattlePlayer *Enemy, CameraPosition Camera, KeyBoardState KeyState_now, KeyBoardState KeyState_last, Audio_ID Sounds)
    {
        Action = "移動";
        Step = 0;
        RunningTimer = 0;
    }
    void BattlePlayer::GotoRush(BattlePlayer *Enemy, CameraPosition Camera, KeyBoardState KeyState_now, KeyBoardState KeyState_last, Audio_ID Sounds)
    {
        if (this->SP >= Rush_cost)
        {
            this->SP -= Rush_cost;
            Action = "衝刺";
            Step = 0;
            RushTimer = 0;
        }
    }
    void BattlePlayer::GotoJump(BattlePlayer *, CameraPosition, KeyBoardState, KeyBoardState, Audio_ID)
    {
        Action = "跳躍";
        Step = 0;
        RushTimer = 0;
        JumpTimer = 0;
    }
    void BattlePlayer::OnStandby(BattlePlayer *Enemy, CameraPosition Camera, KeyBoardState KeyState_now, KeyBoardState KeyState_last, Audio_ID Sounds)
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
            if (StandbyTimer >= 250)
            {
                StandbyTimer = 0;
                if (Step == 0)
                    Step = 1;
                else if (Step == 1)
                    Step = 0;
            }
#pragma endregion

#pragma region 到別的動作

            if (CanControl&&Button_now.button_Right&&OnGround)
            {
                IsRight = true;
                GotoRunning(Enemy, Camera, KeyState_now, KeyState_last, Sounds);
            }
            else if (CanControl&&Button_now.button_Left&&OnGround)
            {
                IsRight = false;
                GotoRunning(Enemy, Camera, KeyState_now, KeyState_last, Sounds);
            }
            else if (CanControl&&Button_now.button_Rush&&Button_last.button_Rush == false)
            {
                GotoRush(Enemy, Camera, KeyState_now, KeyState_last, Sounds);
            }
            else if (CanControl&&Button_now.button_Jump&&Button_last.button_Jump == false)
            {
                GotoJump(Enemy, Camera, KeyState_now, KeyState_last, Sounds);
            }
#pragma endregion

        }
    }
    void BattlePlayer::OnRunning(BattlePlayer *Enemy, CameraPosition Camera, KeyBoardState KeyState_now, KeyBoardState KeyState_last, Audio_ID Sounds)
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

                RunAhead(0.5, RunSpeed);
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
                RunAhead(0.5, RunSpeed);
            }
#pragma endregion

#pragma region 到別的動作
            if (CanControl&&Button_now.button_Rush&&Button_last.button_Rush == false)
            {
                GotoRush(Enemy, Camera, KeyState_now, KeyState_last, Sounds);
            }
            else if (CanControl&&Button_now.button_Jump&&Button_last.button_Jump == false)
            {
                GotoJump(Enemy, Camera, KeyState_now, KeyState_last, Sounds);
            }
            else if (Button_now.button_Right == false && Button_now.button_Left == false)
            {
                GotoStandby(Enemy, Camera, KeyState_now, KeyState_last, Sounds);
            }
#pragma endregion

        }
    }
    void BattlePlayer::OnJump(BattlePlayer *Enemy, CameraPosition Camera, KeyBoardState KeyState_now, KeyBoardState KeyState_last, Audio_ID Sounds)
    {
        if (Action == "跳躍")
        {
            JumpTimer += TIMER_TICK_MILLIDECOND;
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
            }
            else if (Step == 3 && Velocity_Y < 0)
            {
                if (CanControl&&Button_now.button_Jump == true && Button_last.button_Jump == true && Velocity_Y < 2 && JumpTimer < 120)
                    Velocity_Y -= 0.5;
            }
            else if (Velocity_Y >= 0 && Step == 3)
            {
                JumpTimer = 0;
                Step = 4;
            }

            if (Step >= 3)
            {
                if (CanControl&&Button_now.button_Right == false && CanControl&&Button_now.button_Left == false)
                {
                    ProduceFriction(0.15,1);
                }
                else if (CanControl&&Button_now.button_Right == true)
                {
                    IsRight = true;
                    RunAhead(0.5,3);
                }
                else if (CanControl&&Button_now.button_Left == true)
                {
                    IsRight = false;
                    RunAhead(0.5, 3);
                }
            }
            if (OnGround&& Step >= 3)
            {
                GotoStandby(Enemy, Camera, KeyState_now, KeyState_last, Sounds);
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
            else
            {
                Velocity_X = Maxspeed;
            }
        }
        else
        {
            if (Velocity_X > -Maxspeed)
            {
                Velocity_X -= Addspeed;
            }
            else
            {
                Velocity_X = -Maxspeed;
            }
        }
    }

    void BattlePlayer::EffectAutoUpdate(BitmapAnimation * Effection,int tick,bool replay, CameraPosition Camera)
    {
        Effection->AutoPlay(tick, replay);
        Effection->OnUpdate("Effects", Camera);
    }

    void BattlePlayer::EffectReset(BitmapAnimation *Effection,CameraPosition Camera,double XR,double XL,double Y)
    {
        Effection->BitmapisRight = IsRight;
        if (Effection->BitmapisRight)
        {
            Effection->Rect.X = XR;
        }
        else
        {
            Effection->Rect.X = XL;
        }
        Effection->Rect.Y = Y;
        Effection->visable = true;
        Effection->AutoPlayTimer = 0;
        Effection->Step = 0;
        Effection->OnUpdate("Effects", Camera);
    }

}

