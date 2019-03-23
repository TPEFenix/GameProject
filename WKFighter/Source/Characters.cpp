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

        //能力值變數
        HP_Max = Matchstick_HP_Max;//最大生命值
        SP_Max = Matchstick_SP_Max;//最大氣力
        Rush_cost = Matchstick_Rush_Cost;//衝刺消耗量
        StandbySPincrements = Matchstick_StandbySPincrements;
        RunningSPincrements = Matchstick_RunningSPincrements;
        RunSpeed = Matchstick_RunSpeed;


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


        //初始化特效
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

        InsertBitmapPicture("待機", 0, color);
        InsertBitmapPicture("待機", 1, color);
        InsertBitmapPicture("移動", 0, color);
        InsertBitmapPicture("移動", 1, color);
        InsertBitmapPicture("移動", 2, color);
        InsertBitmapPicture("移動", 3, color);
        InsertBitmapPicture("移動", 4, color);
        InsertBitmapPicture("衝刺", 0, color);
        InsertBitmapPicture("衝刺", 1, color);
        InsertBitmapPicture("跳躍", 0, color);
        InsertBitmapPicture("跳躍", 1, color);
        InsertBitmapPicture("跳躍", 2, color);
        InsertBitmapPicture("跳躍", 3, color);
        InsertBitmapPicture("跳躍", 4, color);



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
                    Action = "跳躍";
                    Step = 4;
                    JumpTimer = 0;
                }
            }

        }
    }
    void Matchstick::OnJump(BattlePlayer *Enemy, CameraPosition Camera, KeyBoardState KeyState_now, KeyBoardState KeyState_last, Audio_ID Sounds)
    {
        if (Action == "跳躍")
        {
            JumpTimer += TIMER_TICK_MILLIDECOND;
#pragma region 跳躍主程序
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
                    RunAhead(0.5, RunSpeed/2);
                }
                else if (CanControl&&Button_now.button_Left == true)
                {
                    IsRight = false;
                    RunAhead(0.5, RunSpeed/2);
                }
#pragma endregion

#pragma region 到別的動作
                //正常落地
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
