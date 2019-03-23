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
        PlayerNumber = number;//玩家編號
        if (number == 1)
        {
            IsRight = true;
        }
        else if (number == 2)
        {
            IsRight = false;
        }
        HP_Max = 1000;//最大生命值
        SP_Max = 100;//最大氣力
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
        Rush_cost = 15;
        StandbySPincrements = 5;

        //初始化特效
        Effect_Rush = new BitmapAnimation("Airboost", false);


    }
    Matchstick::~Matchstick()
    {
        delete Effect_Rush;
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
        if (Action == "衝刺")
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
