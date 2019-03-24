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



    }
    Matchstick::~Matchstick()
    {

    }
    void Matchstick::AutoLoadBitmaps(CameraPosition Camera, COLORREF color)
    {
        //有效判定區BitRect
        BodyPicture.LoadTexture(color);


        InsertAction("待機", 1, color);
        InsertAction("移動", 4, color);
        InsertAction("衝刺", 1, color);
        InsertAction("跳躍", 4, color);
        InsertAction("防禦", 0, color);
        InsertAction("練氣", 3, color);



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

        //更新所有Effect的動作
        map<string, BitmapAnimation>::iterator iter;
        for (iter = Effections.begin(); iter != Effections.end(); iter++)
            EffectAutoUpdate(&(iter->second), (int)(((iter->second).PreAutoFrequence)), false, Camera);


        this->PhysicalMovement(Enemy,Camera, KeyState_now, KeyState_last);
        AnimationUpdate(Camera);
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
                Effections["AirBoost"].Step = 0;
                Effections["AirBoost"].visable = false;
                Effections["AirBoost"].OnUpdate("Effects", Camera);
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
