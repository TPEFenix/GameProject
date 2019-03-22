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
    BattlePlayer::BattlePlayer():BitmapAnimation()
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
    void BattlePlayer::OnUpdate(BattlePlayer *Enemy,CameraPosition Camera,KeyBoardState KeyState_now, KeyBoardState KeyState_last, Audio_ID Sounds)
    {
        AnimationUpdate(Camera);
    }
	void BattlePlayer::PhysicalMovement(CameraPosition Camera, KeyBoardState KeyState_now, KeyBoardState KeyState_last)
	{
		Rect.X += Velocity_X;
		Rect.Y += Velocity_Y;
		Velocity_X += Acceleration_X;
		Velocity_Y += Acceleration_Y + Acceleration_gravity;
		if (Rect.Y > GroundPosition)
		{
			Rect.Y = GroundPosition;
            OnGround = true;
		}
	}
    void BattlePlayer::Draw(int i,int j)
    {
        this->DisplayBitmap->Draw(i,j);
    }
    void BattlePlayer::AutoLoadBitmaps(CameraPosition Camera,COLORREF color)
    {
    }
    void BattlePlayer::InsertBitmapPicture(string action,int step,COLORREF color)
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
    void BattlePlayer::OnStandby(BattlePlayer *Enemy, CameraPosition Camera, KeyBoardState KeyState_now, KeyBoardState KeyState_last, Audio_ID Sounds)
    {
        if (Action == "待機")
        {
            if (Button_now.button_Left == false && Button_now.button_Right == false)
            {
                if (Velocity_X <= 1&& Velocity_X>=-1)
                {
                    Velocity_X = 0;
                }
                else if (Velocity_X > 1)
                {
                    Velocity_X -= 1;
                }
                else if (Velocity_X < -1)
                {
                    Velocity_X += 1;
                }
                
            }

            StandbyTimer += TIMER_TICK_MILLIDECOND;
            if (StandbyTimer >= 250)
            {
                StandbyTimer = 0;
                if (Step == 0)
                {
                    Step = 1;
                }
                else if(Step == 1)
                {
                    Step = 0;
                }
            }
            
            if (CanControl&&Button_now.button_Right&&OnGround)
            {
                IsRight = true;
                Action = "移動";
                Step = 0;
                RunningTimer = 0;
            }
            else if (CanControl&&Button_now.button_Left&&OnGround)
            {
                IsRight = false;
                Action = "移動";
                Step = 0;
                RunningTimer = 0;
            }


        }
    }
    void BattlePlayer::OnRunning(BattlePlayer *Enemy, CameraPosition Camera, KeyBoardState KeyState_now, KeyBoardState KeyState_last, Audio_ID Sounds)
    {
        if (Action == "移動")
        {
            //向右走
            if (CanControl&&IsRight&&OnGround&&(Button_now.button_Right == true || Button_now.button_Left == true))
            {
                if (Button_now.button_Left == true)
                {
                    IsRight = false;
                }
                RunningTimer += TIMER_TICK_MILLIDECOND;
                if (RunningTimer >= 25)
                {
                    Step += 1;
                    if (Step > 4)
                    {
                        Step = 0;
                    }
                    RunningTimer = 0;
                }
                if (Velocity_X < 6)
                {
                    Velocity_X += 0.5;
                }
                else
                {
                    Velocity_X = 6;
                }
            }
            //向左走
            else if(CanControl&&IsRight == false&&OnGround && (Button_now.button_Right == true || Button_now.button_Left == true))
            {
                if (Button_now.button_Right == true)
                {
                    IsRight = true;
                }
                RunningTimer += TIMER_TICK_MILLIDECOND;
                if (RunningTimer >= 25)
                {
                    Step += 1;
                    if (Step > 4)
                    {
                        Step = 0;
                    }
                    RunningTimer = 0;
                }
                if (Velocity_X > -6)
                {
                    Velocity_X -= 0.5;
                }
                else
                {
                    Velocity_X = -6;
                }
            }
            else if (Button_now.button_Right == false &&Button_now.button_Left == false)
            {
                Action = "待機";
                Step = 0;
                RunningTimer = 0;
                StandbyTimer = 0;
            }
        }
    }
}

