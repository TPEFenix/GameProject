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
#include <list>
#include <vector>
#include "audio.h"
#include "gamelib.h"
#include "mygame.h"
#include "Keycode.h"
#include "KeyBoardState.h"
#include "BitmapPicture.h"

#pragma endregion 

using namespace std;

namespace game_framework
{

    //本遊戲全域變數
    int GameAction = 0;
    bool DebugMode = true;
    const COLORREF TransparentColor = RGB(100, 120, 0);
    KeyBoardState KeyState_now;//當前的鍵盤狀態
    KeyBoardState KeyState_last;//前一瞬間的鍵盤狀態

    //偵錯模式測試用
#pragma region DebugValueable
    BitmapPicture DebugPicture1 = BitmapPicture(true);
    BitmapPicture DebugPicture2 = BitmapPicture(true);
	BitmapAnimation DebugPicture3 = BitmapAnimation(true);
#pragma endregion 
    //常數資源 如音訊編號等等
#pragma region Const Resources
// 定義各種音效的編號
    struct Audio_ID
    {
        const int Ding = 0;
    } Sound;
#pragma endregion 

    //這些函式拿來優化程式編寫※(效率並不會因此提升)
#pragma region RENEWAL Fuction And Objects
    Keycode Keys;
    void ExitGame()
    {
        PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);
    }
	//矩形碰撞判斷
    bool BitmapPicture_HitRectangle(BitmapPicture Bitmap1, BitmapPicture Bitmap2)
    {
        int x1 = Bitmap1.Rect.X_int;
        int x2 = Bitmap2.Rect.X_int;
        int y1 = Bitmap1.Rect.Y_int;
        int y2 = Bitmap2.Rect.Y_int;
        int w1 = Bitmap1.Rect.Width;
        int w2 = Bitmap2.Rect.Width;
        int h1 = Bitmap1.Rect.Height;
        int h2 = Bitmap2.Rect.Height;

        if (x1 >= x2 && x1 >= x2 + w2) {
            return false;
        }
        else if (x1 <= x2 && x1 + w1 <= x2) {
            return false;
        }
        else if (y1 >= y2 && y1 >= y2 + h2) {
            return false;
        }
        else if (y1 <= y2 && y1 + h1 <= y2) {
            return false;
        }
        return true;
    }
    //像素碰撞檢測
    bool PixelCollision(BitmapPicture *Bitmap1, BitmapPicture *Bitmap2, int accuracy)
    {
        if (BitmapPicture_HitRectangle(*Bitmap1, *Bitmap2) == true)
        {
			if (Bitmap1->CanCollision&&Bitmap2->CanCollision&& Bitmap1->visable &&Bitmap2->visable &&accuracy > 0)
			{
				//決定碰撞四邊
				int HitRight = 0;
				int HitLeft = 0;
				int HitTop = 0;
				int Hitbottom = 0;
				if (Bitmap1->Rect.X_int + Bitmap1->Rect.Width > Bitmap2->Rect.X_int + Bitmap2->Rect.Width)
				{
					HitRight = Bitmap2->Rect.X_int + Bitmap2->Rect.Width;
				}
				else
				{
					HitRight = Bitmap1->Rect.X_int + Bitmap1->Rect.Width;
				}
				if (Bitmap1->Rect.X_int < Bitmap2->Rect.X_int)
				{
					HitLeft = Bitmap2->Rect.X_int;
				}
				else
				{
					HitLeft = Bitmap1->Rect.X_int;
				}
				if (Bitmap1->Rect.Y_int + Bitmap1->Rect.Height > Bitmap2->Rect.Y_int + Bitmap2->Rect.Height)
				{
					Hitbottom = Bitmap2->Rect.Y_int + Bitmap2->Rect.Height;
				}
				else
				{
					Hitbottom = Bitmap1->Rect.Y_int + Bitmap1->Rect.Height;
				}
				if (Bitmap1->Rect.Y_int < Bitmap2->Rect.Y_int)
				{
					HitTop = Bitmap2->Rect.Y_int;
				}
				else
				{
					HitTop = Bitmap1->Rect.Y_int;
				}
				for (int i = 0; i < Hitbottom - HitTop; i += (accuracy))
				{
					for (int j = 0; j < HitRight - HitLeft; j += (accuracy))
					{
						if (Bitmap1->EffectRect[i + HitTop - Bitmap1->Rect.Y_int][j + HitLeft - Bitmap1->Rect.X_int] == true && Bitmap2->EffectRect[i + HitTop - Bitmap2->Rect.Y_int][j + HitLeft - Bitmap2->Rect.X_int] == true)
						{
							return true;
						}
					}
				}
			}
            return false;
        }
        else
        {
            return false;
        }
    }
    //載入音訊資源
    void LoadSounds(int ID, string path)
    {
        //範例
        //CAudio::Instance()->Load(0, "sounds\\ding.wav");
        CAudio::Instance()->Load(ID, &path[0]);	// 載入編號0的聲音ding.wav
    }
    //播放聲音(ID,是否重複撥放)
    void PlaySounds(int soundID, bool replay)
    {
        CAudio::Instance()->Play(soundID, replay);
    }
    //顯示文字(效率極差，盡量不要使用)
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
		DebugPicture3.AutoLoadBitmaps("ball",4, TransparentColor);
    }
#pragma endregion 

    //遊戲開頭讀取畫面
#pragma region Game Loading Progress
    void CGameStateInit::OnInit()
    {

        //讀取開始
        ShowInitProgress(0);	// 一開始的loading進度為0%
        logo.LoadBitmap("Content\\Bitmaps\\BG.bmp");
        // 進入CGameStaterRun::OnInit()

    }
    void CGameStateRun::OnInit()  								// 遊戲的初值及圖形設定
    {
        ShowInitProgress(15);	// 接個前一個狀態的進度，此處進度視為33%
        if (DebugMode)
        {
            DebugPicture1.LoadTexture("Content\\Bitmaps\\BG.bmp", true, TransparentColor);
            DebugPicture1.Rect.X = 300;
            DebugPicture1.Rect.Y = 300;
            DebugPicture2.LoadTexture("Content\\Bitmaps\\火柴人_地面輕攻2_4.bmp", true, TransparentColor);

        }
        ShowInitProgress(33);	// 接個前一個狀態的進度，此處進度視為33%
        if (DebugMode == false)
        {
            int i;
            for (i = 0; i < NUMBALLS; i++)
                ball[i].LoadBitmap();								// 載入第i個球的圖形
            eraser.LoadBitmap();
            background.LoadBitmap(IDB_BACKGROUND);					// 載入背景的圖形
                                                                    //
                                                                    // 完成部分Loading動作，提高進度
                                                                    //
            ShowInitProgress(50);
            Sleep(300); // 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
                        //
                        // 繼續載入其他資料
                        //
            help.LoadBitmap(IDB_HELP, RGB(255, 255, 255));				// 載入說明的圖形
            corner.LoadBitmap(IDB_CORNER);							// 載入角落圖形
            corner.ShowBitmap(background);							// 將corner貼到background
            bball.LoadBitmap();										// 載入圖形
            hits_left.LoadBitmap();
        }

        LoadSounds(Sound.Ding, "sounds\\ding.wav");
        LoadSounds(1, "sounds\\lake.mp3");
        CAudio::Instance()->Load(2, "sounds\\ntut.mid");	// 載入編號2的聲音ntut.mid
        CAudio::Instance()->Load(3, "sounds\\KEMURIKUSA.mp3");	// 載入編號3的聲音ntut.mid
                                                                    //
                                                                    // 此OnInit動作會接到CGameStaterOver::OnInit()，所以進度還沒到100%
                                                                    //
    }
    void CGameStateOver::OnInit()
    {
        //
        // 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
        //     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
        //
        ShowInitProgress(50);	// 接個前一個狀態的進度，此處進度視為66%
                                //
                                // 開始載入資料
                                //
        Sleep(50);				// 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
        for (int i = 75; i <= 100; i += 1)
        {
            ShowInitProgress(i);
            Sleep(5);
        }
        // 最終進度為100%
        //
        ShowInitProgress(100);
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
        if (DebugMode)
        {
            DebugPicture1.Rect.X += 0;
            DebugPicture1.Rect.Y += 0;
            DebugPicture2.Rect.X += 0;
            DebugPicture2.Rect.Y += 0;
            for (int i = 0; i < 10; i += 1)
            {
                DebugPicture1.Draw(i, 3);
                DebugPicture2.Draw(i, 5);

                Showtext("DebugTesting...", 50, 200, 20, RGB(100, 120, 0), RGB(255, 255, 255), i, 2);
                if (PixelCollision(&DebugPicture1, &DebugPicture2, 3))
                {
                    Showtext("LAYERTesting...True", 50, 200, 40, RGB(0, 50, 0), RGB(255, 255, 255), i, 1);
                }
                else
                {
                    Showtext("LAYERTesting...false", 50, 200, 40, RGB(0, 50, 0), RGB(255, 255, 255), i, 1);
                }
            }

        }
        else
        {
            //
            // 貼上logo
            //
            logo.SetTopLeft((SIZE_X - logo.Width()) / 2, SIZE_Y / 8);
            logo.ShowBitmap();
            //
            // Demo螢幕字型的使用，不過開發時請盡量避免直接使用字型，改用CMovingBitmap比較好
            //
            CDC *pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC 
            CFont f, *fp;
            f.CreatePointFont(160, "Times New Roman");	// 產生 font f; 160表示16 point的字
            fp = pDC->SelectObject(&f);					// 選用 font f
            pDC->SetBkColor(RGB(0, 0, 0));
            pDC->SetTextColor(RGB(255, 255, 0));
            pDC->TextOut(120, 220, "Please click mouse or press SPACE to begin.");
            pDC->TextOut(5, 395, "Press Ctrl-F to switch in between window mode and full screen mode.");
            if (ENABLE_GAME_PAUSE)
                pDC->TextOut(5, 425, "Press Ctrl-Q to pause the Game.");
            pDC->TextOut(5, 455, "Press Alt-F4 or ESC to Quit.");
            pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
            CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
        }
    }
    void CGameStateInit::OnMove()
    {
        if (KeyState_now.Space == true && KeyState_now.A == true && KeyState_last.A == false)
        {
            GotoGameState(GAME_STATE_RUN);
        }
        if (DebugMode)
        {
            bool fuck = false;
            if (KeyState_now.Right == true)
            {
                DebugPicture1.Rect.X += 3;
            }
            if (KeyState_now.Left == true)
            {
                DebugPicture1.Rect.X -= 3;
            }
            if (KeyState_now.Up == true)
            {
                DebugPicture1.Rect.Y -= 3;
            }
            if (KeyState_now.Down == true)
            {
                DebugPicture1.Rect.Y += 3;
            }


        }
        KeyState_last = KeyState_now;
    }
#pragma endregion 

    //遊戲進行畫面
#pragma region GameRunning
//除了開頭以外的遊戲主體(將以GameAction切換遊戲視窗)
    CGameStateRun::CGameStateRun(CGame *g) : CGameState(g), NUMBALLS(28)
    {
        ball = new CBall[NUMBALLS];
    }
    CGameStateRun::~CGameStateRun()
    {
        delete[] ball;
    }
    void CGameStateRun::OnBeginState()
    {
        const int BALL_GAP = 90;
        const int BALL_XY_OFFSET = 45;
        const int BALL_PER_ROW = 7;
        const int HITS_LEFT = 10;
        const int HITS_LEFT_X = 590;
        const int HITS_LEFT_Y = 0;
        const int BACKGROUND_X = 60;
        const int ANIMATION_SPEED = 15;
        for (int i = 0; i < NUMBALLS; i++) {				// 設定球的起始座標
            int x_pos = i % BALL_PER_ROW;
            int y_pos = i / BALL_PER_ROW;
            ball[i].SetXY(x_pos * BALL_GAP + BALL_XY_OFFSET, y_pos * BALL_GAP + BALL_XY_OFFSET);
            ball[i].SetDelay(x_pos);
            ball[i].SetIsAlive(true);
        }
        eraser.Initialize();
        background.SetTopLeft(BACKGROUND_X, 0);				// 設定背景的起始座標
        help.SetTopLeft(0, SIZE_Y - help.Height());			// 設定說明圖的起始座標
        hits_left.SetInteger(HITS_LEFT);					// 指定剩下的撞擊數
        hits_left.SetTopLeft(HITS_LEFT_X, HITS_LEFT_Y);		// 指定剩下撞擊數的座標
        CAudio::Instance()->Play(1, true);			// 撥放 WAVE
        CAudio::Instance()->Play(Sound.Ding, false);		// 撥放 WAVE
        CAudio::Instance()->Play(3, true);			// 撥放 MIDI
    }


    //GameState LogicUpdate
    //這裡處理遊戲邏輯判斷
    void CGameStateRun::OnMove()							// 移動遊戲元素
    {
        //
        // 如果希望修改cursor的樣式，則將下面程式的commment取消即可
        //
        // SetCursor(AfxGetApp()->LoadCursor(IDC_GAMECURSOR));
        //
        // 移動背景圖的座標
        //
        if (background.Top() > SIZE_Y)
            background.SetTopLeft(60, -background.Height());
        background.SetTopLeft(background.Left(), background.Top() + 1);
        //
        // 移動球
        //
        int i;
        for (i = 0; i < NUMBALLS; i++)
            ball[i].OnMove();
        //
        // 移動擦子
        //
        eraser.OnMove();
        //
        // 判斷擦子是否碰到球
        //
        for (i = 0; i < NUMBALLS; i++)
            if (ball[i].IsAlive() && ball[i].HitEraser(&eraser)) {
                ball[i].SetIsAlive(false);
                CAudio::Instance()->Play(0);
                hits_left.Add(-1);
                //
                // 若剩餘碰撞次數為0，則跳到Game Over狀態
                //
                if (hits_left.GetInteger() <= 0) {
                    CAudio::Instance()->Stop(2);	// 停止 WAVE
                    CAudio::Instance()->Stop(3);	// 停止 MIDI
                    GotoGameState(GAME_STATE_OVER);
                }
            }
        //
        // 移動彈跳的球
        //
        bball.OnMove();
    }



    void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
    {
        const char KEY_LEFT = 0x25; // keyboard左箭頭
        const char KEY_UP = 0x26; // keyboard上箭頭
        const char KEY_RIGHT = 0x27; // keyboard右箭頭
        const char KEY_DOWN = 0x28; // keyboard下箭頭
        if (nChar == KEY_LEFT)
            eraser.SetMovingLeft(true);
        if (nChar == KEY_RIGHT)
            eraser.SetMovingRight(true);
        if (nChar == KEY_UP)
            eraser.SetMovingUp(true);
        if (nChar == KEY_DOWN)
            eraser.SetMovingDown(true);
    }
    void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
    {
        const char KEY_LEFT = 0x25; // keyboard左箭頭
        const char KEY_UP = 0x26; // keyboard上箭頭
        const char KEY_RIGHT = 0x27; // keyboard右箭頭
        const char KEY_DOWN = 0x28; // keyboard下箭頭
        if (nChar == KEY_LEFT)
            eraser.SetMovingLeft(false);
        if (nChar == KEY_RIGHT)
            eraser.SetMovingRight(false);
        if (nChar == KEY_UP)
            eraser.SetMovingUp(false);
        if (nChar == KEY_DOWN)
            eraser.SetMovingDown(false);
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


    //GameState LogicUpdate
    //這裡處理圖片貼圖
    void CGameStateRun::OnShow()
    {
        //
        //  注意：Show裡面千萬不要移動任何物件的座標，移動座標的工作應由Move做才對，
        //        否則當視窗重新繪圖時(OnDraw)，物件就會移動，看起來會很怪。換個術語
        //        說，Move負責MVC中的Model，Show負責View，而View不應更動Model。
        //
        //
        //  貼上背景圖、撞擊數、球、擦子、彈跳的球
        //
        background.ShowBitmap();			// 貼上背景圖
        help.ShowBitmap();					// 貼上說明圖
        hits_left.ShowBitmap();
        for (int i = 0; i < NUMBALLS; i++)
            ball[i].OnShow();				// 貼上第i號球
        bball.OnShow();						// 貼上彈跳的球
        eraser.OnShow();					// 貼上擦子
        //
        //  貼上左上及右下角落的圖
        //
        corner.SetTopLeft(0, 0);
        corner.ShowBitmap();
        corner.SetTopLeft(SIZE_X - corner.Width(), SIZE_Y - corner.Height());
        corner.ShowBitmap();
    }
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



}