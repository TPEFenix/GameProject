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
    BattlePlayer::BattlePlayer()
    {
    }



    BattlePlayer::~BattlePlayer()
    {
    }
    void BattlePlayer::AnimationUpdat(CameraPosition Camera)
    {
#pragma region 確定圖檔名稱
        //確定圖檔名稱
        string Actionstring = "Content\\Bitmaps\\" + GetName() + "\\" + GetName() + "_" + IntToString(Step) + ".bmp";
        static char cc[65535];
        strcpy(cc, Actionstring.c_str());
        DisplayBitmap = &BitmapPictures[cc];
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
        AnimationUpdat(Camera);
    }
    void BattlePlayer::Draw(int i,int j)
    {
        this->DisplayBitmap->Draw(i,j);
    }
    void BattlePlayer::AutoLoadBitmaps(string name, int MaxSteps, bool CanPixelCollisionin, COLORREF color,CameraPosition ca)
    {
    }
    void BattlePlayer::InsertBitmapPicture(string action,int step,COLORREF color)
    {
        string str;
        str = ("Content\\Bitmaps\\" + GetName() + "\\" + action + "_" + IntToString(step) + ".bmp");
        BitmapPictures.insert(std::pair<string, BitmapPicture>(str, BitmapPicture(visable)));
        static char cr[65535];
        strcpy(cr, str.c_str());
        BitmapPictures[str].LoadTexture(cr, true, color);
        str = ("Content\\Bitmaps\\" + GetName() + "\\" + action + "_" + IntToString(step) + "_L.bmp");
        BitmapPictures.insert(std::pair<string, BitmapPicture>(str, BitmapPicture(visable)));
        static char cl[65535];
        strcpy(cl, str.c_str());
        BitmapPictures[str].LoadTexture(cl, true, color);
    }
}

