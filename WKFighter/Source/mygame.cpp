//GameFramework介紹
#pragma region Game Framework Introduction
/*
* mygame.cpp: 本檔案儲遊戲本身的class的implementation
* Copyright (C) 2002-2008 Woei-Kae Chen <wkc@csie.ntut.edu.tw>
*
* This file is part of game, a free game development framework for windows.
*
* game is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* game is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
* History:
*   2002-03-04 V3.1
*          Add codes to demostrate the use of CMovingBitmap::ShowBitmap(CMovingBitmap &).
*	 2004-03-02 V4.0
*      1. Add CGameStateInit, CGameStateRun, and CGameStateOver to
*         demonstrate the use of states.
*      2. Demo the use of CInteger in CGameStateRun.
*   2005-09-13
*      Rewrite the codes for CBall and CEraser.
*   2005-09-20 V4.2Beta1.
*   2005-09-29 V4.2Beta2.
*      1. Add codes to display IDC_GAMECURSOR in GameStateRun.
*   2006-02-08 V4.2
*      1. Revise sample screens to display in English only.
*      2. Add code in CGameStateInit to demo the use of PostQuitMessage().
*      3. Rename OnInitialUpdate() -> OnInit().
*      4. Fix the bug that OnBeginState() of GameStateInit is not called.
*      5. Replace AUDIO_CANYON as AUDIO_NTUT.
*      6. Add help bitmap to CGameStateRun.
*   2006-09-09 V4.3
*      1. Rename Move() and Show() as OnMove and OnShow() to emphasize that they are
*         event driven.
*   2006-12-30
*      1. Bug fix: fix a memory leak problem by replacing PostQuitMessage(0) as
*         PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE,0,0).
*   2008-02-15 V4.4
*      1. Add namespace game_framework.
*      2. Replace the demonstration of animation as a new bouncing ball.
*      3. Use ShowInitProgress(percent) to display loading progress.
*   2010-03-23 V4.6
*      1. Demo MP3 support: use lake.mp3 to replace lake.wav.
*/
#pragma endregion 

//函式庫引入
#pragma region LibraryImport
#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include <windows.h>
#include <sstream>
#include <list>
#include <vector>
#include "audio.h"
#include "gamelib.h"
#include "mygame.h"
#include "Keycode.h"
#include "KeyBoardState.h"
#include "WKBitmap.h"
#include "BatllePlayer.h"
#include "CollisionSensor.h"
#include "TypeConverter.h"
#include "WKAudio.h"
#include "Bar.h"
#pragma endregion 

//命名空間引入
#pragma region NamespaceImport
using namespace std;
using namespace CollisionSensor_namespace;
using namespace TypeConverter_namespace;
using namespace WKAudio_namespace;
#pragma endregion 

namespace game_framework
{
    //本遊戲全域變數
#pragma region Global
    //邏輯
    int GameAction = 0;//遊戲場景
    const bool DebugMode = false;//是否啟用Debug模式
    const bool LoaddingBoost = true;

    //顯示
    CameraPosition Camera;//遊戲鏡頭
    const COLORREF TransparentColor = RGB(100, 120, 0);//透明色設定
	const int left = 1;
	const int right = 2;

    //聲音
    const Audio_ID Sounds;//音效資源編碼

    //鍵盤
    const Keycode Keys;//鍵盤字典物件
    KeyBoardState KeyState_now;//當前的鍵盤狀態
    KeyBoardState KeyState_last;//前一瞬間的鍵盤狀態
#pragma endregion 


 //開頭畫面變數
#pragma region GameAction_Title
    BitmapPicture BackGround_Title;
    BitmapPicture Title_Bitmap;
#pragma endregion 

    //主選單變數
#pragma region GameAction_Menu
    BitmapPicture BackGround_Menu;
    int TitleSelection = 0;
#pragma endregion 





    //這些函式拿來優化程式編寫※(效率並不會因此提升)
#pragma region RENEWAL Fuction And Objects
    void ExitGame()
    {
        PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);
    }
    //效率很差盡量不要使用
    void Showtext(char *mes, int X, int Y, int fontsize, COLORREF BK, COLORREF FC, int WorkingLayer, int TargetLayer)
    {
        if (WorkingLayer == TargetLayer)
        {
            CDC *pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC 
            CFont f, *fp;
            f.CreatePointFont(fontsize * 10, "Times New Roman");	// 產生 font f; 160表示16 point的字
            fp = pDC->SelectObject(&f);					// 選用 font f
            pDC->SetBkColor(BK);
            pDC->SetTextColor(FC);
            pDC->TextOut(X, Y, mes);
            pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
            CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
        }
    }
#pragma endregion 

    //程式開始
#pragma region Program Initialize
    CGameStateInit::CGameStateInit(CGame *g) : CGameState(g)
    {
    }
    void CGameStateInit::OnBeginState()
    {

    }
#pragma endregion 

    //遊戲開頭讀取畫面
#pragma region Game Loading Progress
    void CGameStateInit::OnInit()
    {
        //讀取開始
        ShowInitProgress(0);	// 一開始的loading進度為0%
#pragma region Loadding Effect
        // 進入CGameStaterRun::OnInit()
        for (int i = 0; i <= 40; i += 1)
        {
            ShowInitProgress(i);
            if (!LoaddingBoost)
                Sleep(10);
            else
                i = 100;
        }
#pragma endregion 
        //讀取所有圖檔--Debug模式讀取檔案Begin
        DebugmodeLoading();
        //讀取所有圖檔--Debug模式讀取檔案End
    }
    void CGameStateRun::OnInit()// 讀取檔案
    {
        //讀取所有圖檔--Begin
        BackGround_Title = BitmapPicture("Content\\Bitmaps\\BackGround_Title.bmp", 0, 0, true, false, false);
        BackGround_Title.LoadTexture(TransparentColor);
        Title_Bitmap = BitmapPicture("Content\\Bitmaps\\Title.bmp", 100, 0, true, false, false);
        Title_Bitmap.LoadTexture(TransparentColor);
        BackGround_Menu = BitmapPicture("Content\\Bitmaps\\BackGround_Menu.bmp", 0, 0, true, false, false);
        BackGround_Menu.LoadTexture(TransparentColor);
        //讀取所有圖檔--End
#pragma region Loadding Effect
        ShowInitProgress(40);
        for (int i = 40; i <= 50; i += 1)
        {
            ShowInitProgress(i);
            if (!LoaddingBoost)
                Sleep(4);
            else
                i = 100;
        }
        ShowInitProgress(50);	// 接個前一個狀態的進度，此處進度視為33%
        for (int i = 50; i <= 75; i += 1)
        {
            ShowInitProgress(i);
            if (!LoaddingBoost)
                Sleep(1);
            else
                i = 100;
        }
#pragma endregion 
        //讀取所有音效--Begin
        LoadSounds(Sounds.Ding, "Content\\Sounds\\ding.wav");
        //讀取所有音效--End
        ShowInitProgress(75);
        // 此OnInit動作會接到CGameStaterOver::OnInit()，所以進度還沒到100%
        //
    }
    void CGameStateOver::OnInit()
    {
#pragma region Loadding Effect
        for (int i = 75; i <= 100; i += 1)
        {
            ShowInitProgress(i);
            if (!LoaddingBoost)
                Sleep(4);
            else
                i = 100;
        }
        ShowInitProgress(0);	// 接個前一個狀態的進度，此處進度視為66%
        for (int i = 0; i <= 100; i += 1)
        {
            ShowInitProgress(i);
            if (!LoaddingBoost)
                Sleep(2);
            else
                i = 100;
        }
        ShowInitProgress(100);
#pragma endregion 
        PlaySounds(Sounds.Ding, false);
        if (!LoaddingBoost)
            Sleep(500);				// 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
    }
#pragma endregion 

    //遊戲開頭畫面
#pragma region Game start screen
    void CGameStateInit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
    {
        KeyState_now.UpdateState_Down(nChar);
    }
    void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
    {
        KeyState_now.UpdateState_Up(nChar);
    }
    void CGameStateInit::OnShow()
    {
        DebugmodeOnShow();
        for (int i = 0; i < 5; i++)
        {
            BackGround_Title.Draw(i, 1);
            Title_Bitmap.Draw(i, 3);
            Showtext("Press [SPACE] to start the game", 100, 450, 35, RGB(0, 0, 0), RGB(255, 255, 255), i, 3);
        }
    }
    void CGameStateInit::OnMove()
    {
        BackGround_Title.OnUpdate();
        Title_Bitmap.OnUpdate();
        if (KeyState_now.Space == true && KeyState_last.Space == false)
        {
            GameAction = 0;
            GotoGameState(GAME_STATE_RUN);
        }
        DebugmodeOnMove();
        KeyState_last = KeyState_now;
    }
#pragma endregion 

    //遊戲進行畫面
#pragma region GameRunning
//除了開頭以外的遊戲主體(將以GameAction切換遊戲視窗)
    CGameStateRun::CGameStateRun(CGame *g) : CGameState(g)
    {

    }
    CGameStateRun::~CGameStateRun()
    {

    }
    void CGameStateRun::OnBeginState()
    {
        PlaySounds(Sounds.Ding, false);
    }

    //GameState LogicUpdate
    void CGameStateRun::OnMove()
    {
        // 如果希望修改cursor的樣式，則將下面程式的commment取消即可
        // SetCursor(AfxGetApp()->LoadCursor(IDC_GAMECURSOR));
        if (GameAction == 0)
        {
            BackGround_Menu.OnUpdate();
        }

    }
    //GameState ShowBitmaps
    void CGameStateRun::OnShow()
    {
        //  注意：Show裡面千萬不要移動任何物件的座標，移動座標的工作應由Move做才對，
        //        否則當視窗重新繪圖時(OnDraw)，物件就會移動，看起來會很怪。換個術語
        //        說，Move負責MVC中的Model，Show負責View，而View不應更動Model。
        for (int i = 0; i < 5; i++)
        {
            if (GameAction == 0)
            {
                BackGround_Menu.Draw(i, 1);
            }
        }
    }


    void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
    {
        KeyState_now.UpdateState_Down(nChar);
    }
    void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
    {
        KeyState_now.UpdateState_Up(nChar);
    }

    //滑鼠處理事件
#pragma region MouseState
    void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
    {

    }
    void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
    {

    }
    void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// 處理滑鼠的動作
    {
        // 沒事。如果需要處理滑鼠移動的話，寫code在這裡
    }
    void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
    {

    }
    void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
    {

    }
#pragma endregion 

#pragma endregion 

    //Game End-遊戲結束畫面==退回開頭畫面
#pragma region GameOverState
    CGameStateOver::CGameStateOver(CGame *g) : CGameState(g)
    {
    }
    void CGameStateOver::OnMove()
    {
        GotoGameState(GAME_STATE_INIT);
    }
    void CGameStateOver::OnBeginState()
    {

    }
    void CGameStateOver::OnShow()
    {

    }
#pragma endregion 

    //偵錯模式測試用
#pragma region DebugValueable
    BitmapPicture DebugPicture1 = BitmapPicture("Content\\Bitmaps\\BG.bmp", true);
    BitmapPicture DebugPicture2 = BitmapPicture(true);
    BitmapAnimation DebugPicture3 = BitmapAnimation("ball", true);
    BitmapPicture DebugPicture4 = BitmapPicture("RES\\level1.bmp", true);
    BitmapPicture* DebugPicture5[20];
	Bar DebugPicture6 = Bar(200, left);

    void CGameStateInit::DebugmodeLoading()
    {
        if (DebugMode)
        {
            int xy[20][2] = { {100, 100}, {100, 200}, {100, 300}, {100, 400}, {100, 500}, {100, 600}, {100, 700}, {100, 800}, {200, 850}, {300, 850},
            {400, 850}, {500, 850}, {600, 850}, {700, 850}, {800, 850}, {900, 850}, {1000, 850}, {1100, 850}, {1150, 800}, {1150, 700} };
            DebugPicture1.LoadTexture(true, TransparentColor);
            DebugPicture1.Rect.X = 300;
            DebugPicture1.Rect.Y = 300;
            DebugPicture2.LoadTexture("Content\\Bitmaps\\火柴人_地面輕攻2_4.bmp", true, TransparentColor);
            DebugPicture3.AutoLoadBitmaps("ball", 4, true, TransparentColor);
            DebugPicture4.LoadTexture(true, TransparentColor);
            DebugPicture3.Rect.X = 200;
            DebugPicture3.Rect.Y = 100;
            for (int i = 0; i < 20; i++)
            {
                DebugPicture5[i] = new BitmapPicture("RES\\ball.bmp", true);
                DebugPicture5[i]->LoadTexture(true, TransparentColor);
                (*DebugPicture5[i]).Rect.X = xy[i][0];
                (*DebugPicture5[i]).Rect.Y = xy[i][1];
            }

        }
    }
    void CGameStateInit::DebugmodeOnShow()
    {
        if (DebugMode)
        {
            for (int i = 0; i < 10; i += 1)
            {
                DebugPicture1.visable = false;
                DebugPicture2.visable = false;
                DebugPicture3.DisplayBitmap->Draw(i, 4);
                DebugPicture3.AutoPlay(1000);
                DebugPicture1.Draw(i, 3);
                DebugPicture2.Draw(i, 5);
                DebugPicture4.Draw(i, 0);
                for (int j = 0; j < 20; j++)
                    (*DebugPicture5[j]).Draw(i, 5);

                //Showtext("DebugTesting...", 50, 200, 20, RGB(100, 120, 0), RGB(255, 255, 255), i, 2);

                //if (PixelCollision(DebugPicture3.DisplayBitmap, &DebugPicture4, 3))
                //{
                //    Showtext("LAYERTesting...True", 50, 200, 40, RGB(0, 50, 0), RGB(255, 255, 255), i, 1);
               // }
                //else
               //{
                //    Showtext("LAYERTesting...false", 50, 200, 40, RGB(0, 50, 0), RGB(255, 255, 255), i, 1);
                //}

            }
        }
    }
    void CGameStateInit::DebugmodeOnMove()
    {
        if (DebugMode)
        {

            DebugPicture1.OnUpdate(Camera);
            DebugPicture3.OnUpdate(Camera);
            DebugPicture2.OnUpdate(Camera);
            DebugPicture4.OnUpdate(Camera);
            for (int i = 0; i < 20; i++)
                (*DebugPicture5[i]).OnUpdate(Camera);

            bool fuck = false;
            for (int i = 0; i < 20; i++)
                if (BitmapPicture_HitRectangle(*DebugPicture3.DisplayBitmap, (*DebugPicture5[i])))
                {
                    (*DebugPicture5[i]).visable = false;

                }
            if (KeyState_now.Right == true)
            {
                DebugPicture3.DisplayBitmap->Rect.X_int += 5;
                if (PixelCollision(DebugPicture3.DisplayBitmap, &DebugPicture4, 1))
                {

                }
                else
                {
                    if (DebugPicture3.Rect.X_int > (SIZE_X - 100 - DebugPicture3.Rect.Width))
                    {
                        Camera.X += 5;
                        DebugPicture3.DisplayBitmap->Rect.X_int -= 5;
                    }
                    else
                    {
                        DebugPicture3.Rect.X += 5;
                    }
                }
                DebugPicture3.DisplayBitmap->Rect.X_int -= 5;
            }
            if (KeyState_now.Left == true)
            {
                DebugPicture3.DisplayBitmap->Rect.X_int -= 5;
                if (PixelCollision(DebugPicture3.DisplayBitmap, &DebugPicture4, 1))
                {

                }
                else
                {
                    if (DebugPicture3.Rect.X_int < 100)
                    {
                        Camera.X -= 5;
                        DebugPicture3.DisplayBitmap->Rect.X_int += 5;
                    }
                    else
                    {
                        DebugPicture3.Rect.X -= 5;
                    }
                }
                DebugPicture3.DisplayBitmap->Rect.X_int += 5;
            }
            if (KeyState_now.Up == true)
            {
                DebugPicture3.DisplayBitmap->Rect.Y_int -= 5;
                if (PixelCollision(DebugPicture3.DisplayBitmap, &DebugPicture4, 1))
                {

                }
                else
                {
                    if (DebugPicture3.Rect.Y_int < 100)
                    {
                        Camera.Y -= 5;
                        DebugPicture3.DisplayBitmap->Rect.Y_int += 5;
                    }
                    else
                    {
                        DebugPicture3.Rect.Y -= 5;
                    }
                }
                DebugPicture3.DisplayBitmap->Rect.Y_int += 5;
            }
            if (KeyState_now.Down == true)
            {
                DebugPicture3.DisplayBitmap->Rect.Y_int += 5;
                if (PixelCollision(DebugPicture3.DisplayBitmap, &DebugPicture4, 1))
                {

                }
                else
                {
                    if (DebugPicture3.Rect.Y_int > SIZE_Y - 100 - DebugPicture3.Rect.Height)
                    {
                        Camera.Y += 5;
                        DebugPicture3.DisplayBitmap->Rect.Y_int -= 5;
                    }
                    else
                    {
                        DebugPicture3.Rect.Y += 5;
                    }
                }
                DebugPicture3.DisplayBitmap->Rect.Y_int -= 5;
            }
        }
    }
#pragma endregion 

}