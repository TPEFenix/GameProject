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
#include <thread>
#include "audio.h"
#include "gamelib.h"
#include "mygame.h"
#include "Keycode.h"
#include "KeyBoardState.h"
#include "WKBitmap.h"
#include "BattlePlayer.h"
#include "CollisionSensor.h"
#include "TypeConverter.h"
#include "WKAudio.h"
#include "Bar.h"
#include "Characters.h"
#include "FunctionUser.h"
#include "SelectionBitmap.h"
#pragma endregion 

//命名空間引入
#pragma region NamespaceImport
using namespace std;
using namespace CollisionSensor_namespace;
using namespace TypeConverter_namespace;
using namespace WKAudio_namespace;
using namespace FunctionUser_namespace;
#pragma endregion 

namespace game_framework
{
	#pragma region 變數
	//本遊戲全域變數
	#pragma region Global
	//邏輯
	int GameAction = 0;//遊戲場景
	const bool DebugMode = false;//是否啟用Debug模式
	const bool LoaddingBoost = false;//使否啟用讀取加速
	bool CloseingDebug = false;

	BitmapPicture LoadingPicture;//讀取畫面圖示
	thread LoadingThread;//讀取執行序
	bool LoadingStart = false;//開始讀取布林值
	bool LoadingDone = false;//讀取完成布林值
	bool LoadingTemp = false;//放大縮小
	double Loadingfactor = 1;//放大縮小Double

	//顯示
	CameraPosition Camera;//遊戲鏡頭
	const COLORREF TransparentColor = RGB(100, 120, 0);//透明色設定
	const int left = 1;
	const int right = 2;


	//戰鬥
	#pragma region 戰鬥畫面變數
	double ReadyTimer;
	BattlePlayer *Player1;//1P戰鬥者k
	BattlePlayer *Player2;//2P戰鬥者
	int Player1Character = 1;//Player1選擇的角色ID
	int Player2Character = 0;//Player1選擇的角色ID
	BitmapPicture BK;//戰鬥背景
	BitmapPicture BlackCover;//戰鬥黑幕
	BitmapPicture WhiteCover;//戰鬥白幕
	BitmapPicture ReadyBmp;//ReadyBitmap
	BitmapPicture KoBmp;//ReadyBitmap

	double BlackCoverfactor;//戰鬥黑幕大小
	Bar Bar_HP1;//玩家1血量
	Bar Bar_HP2;//玩家2血量
	Bar Bar_SP1;//玩家1體力
	Bar Bar_SP2;//玩家2體力
	Bar Bar_RE1;//玩家1殘餘回復量
	Bar Bar_RE2;//玩家2殘餘回復量
	Bar Bar_Player1Break;//玩家1失衡值
	Bar Bar_Player2Break;//玩家2失衡值
	BitmapPicture Bar_HP1_MaskTop;
	BitmapPicture Bar_HP1_MaskBottom;
	BitmapPicture Bar_SP1_MaskTop;
	BitmapPicture Bar_SP1_MaskBottom;
	BitmapPicture Bar_HP2_MaskTop;
	BitmapPicture Bar_HP2_MaskBottom;
	BitmapPicture Bar_SP2_MaskTop;
	BitmapPicture Bar_SP2_MaskBottom;
	BitmapPicture Player1_Name;
	BitmapPicture Player2_Name;
	BitmapPicture CutInMask;
	double WinnerTimer;
	double WinnerTimer2;
	bool SomeBodyDown;
	int WinnerID;
	#pragma endregion

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
	BitmapPicture Title_Start;
	BitmapPicture Title_SkillTable;
	BitmapPicture Title_Exit;
	BitmapPicture Title_Cursor;
	BitmapPicture BackGround_Select;
	#pragma endregion 
	//主選單變數
	#pragma region GameAction_Menu
	BitmapPicture BackGround_Menu;
	vector <SelectionBitmap> TitleSelects;
	BitmapPicture P1Select_0;
	BitmapPicture P1Select_1;
	BitmapPicture P1Select_2;
	BitmapPicture P1Select_3;
	BitmapPicture P1Select_4;
	BitmapPicture P1Select_5;
	BitmapPicture P2Select_0;
	BitmapPicture P2Select_1;
	BitmapPicture P2Select_2;
	BitmapPicture P2Select_3;
	BitmapPicture P2Select_4;
	BitmapPicture P2Select_5;
	BitmapPicture P1P2Select_0;
	BitmapPicture P1P2Select_1;
	BitmapPicture P1P2Select_2;
	BitmapPicture P1P2Select_3;
	BitmapPicture P1P2Select_4;
	BitmapPicture P1P2Select_5;
	BitmapPicture Characters_Menu;

	int TitleSelection = 0;
	int P1Selection = 0;
	int P2Selection = 0;
	bool SelectedP1 = false;
	bool SelectedP2 = false;



	#pragma endregion 
	#pragma endregion

	#pragma region 套裝函式內容
	//這些函式拿來作套裝程式編寫※
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
	//決定使用角色
	BattlePlayer *DecideCharacter(int PlayerIndex, int Decide)
	{
		BattlePlayer *Player;
		if (Decide == 0)//選擇火柴人
		{
			Player = new Matchstick(PlayerIndex);
		}
		if (Decide == 1)//選擇火柴人
		{
			Player = new Rina(PlayerIndex);
		}
		return Player;
	}

	#pragma endregion 
	//讀取總檔案
	#pragma region GameLoading
	void CGameStateInit::GameLoading()
	{
		//讀取所有圖檔--Begin
		ShowInitProgress(10);
		LoadingPicture = BitmapPicture("Content\\Bitmaps\\Loading.bmp", 150, 200, true, false, false);
		LoadingPicture.LoadTexture(TransparentColor);
		BackGround_Title = BitmapPicture("Content\\Bitmaps\\BackGround_Title.bmp", 0, 0, true, false, false);
		BackGround_Title.LoadTexture(TransparentColor);
		Title_Bitmap = BitmapPicture("Content\\Bitmaps\\Title.bmp", 100, 0, true, false, false);
		Title_Bitmap.LoadTexture(TransparentColor);
		BackGround_Menu = BitmapPicture("Content\\Bitmaps\\BackGround_Menu.bmp", 0, 0, true, false, false);
		BackGround_Menu.LoadTexture(TransparentColor);
		Title_Start = BitmapPicture("Content\\Bitmaps\\介面文字1.bmp", 260, 300, true, false, false);
		Title_Start.LoadTexture(TransparentColor);
		Title_SkillTable = BitmapPicture("Content\\Bitmaps\\介面文字2.bmp", 260, 400, true, false, false);
		Title_SkillTable.LoadTexture(TransparentColor);
		Title_Exit = BitmapPicture("Content\\Bitmaps\\介面文字3.bmp", 285, 500, true, false, false);
		Title_Exit.LoadTexture(TransparentColor);
		Title_Cursor = BitmapPicture("Content\\Bitmaps\\游標.bmp", 225, 300, true, false, false);
		Title_Cursor.LoadTexture(TransparentColor);
		P1Select_0 = BitmapPicture("Content\\Bitmaps\\Select\\P1Select_0.bmp", 210, 270, true, false, false);
		P1Select_0.LoadTexture(TransparentColor);
		P1Select_1 = BitmapPicture("Content\\Bitmaps\\Select\\P1Select_1.bmp", 210, 270, true, false, false);
		P1Select_1.LoadTexture(TransparentColor);
		P1Select_2 = BitmapPicture("Content\\Bitmaps\\Select\\P1Select_2.bmp", 210, 270, true, false, false);
		P1Select_2.LoadTexture(TransparentColor);
		P1Select_3 = BitmapPicture("Content\\Bitmaps\\Select\\P1Select_3.bmp", 210, 270, true, false, false);
		P1Select_3.LoadTexture(TransparentColor);
		P1Select_4 = BitmapPicture("Content\\Bitmaps\\Select\\P1Select_4.bmp", 210, 270, true, false, false);
		P1Select_4.LoadTexture(TransparentColor);
		P1Select_5 = BitmapPicture("Content\\Bitmaps\\Select\\P1Select_5.bmp", 210, 270, true, false, false);
		P1Select_5.LoadTexture(TransparentColor);
		P2Select_0 = BitmapPicture("Content\\Bitmaps\\Select\\P2Select_0.bmp", 210, 270, true, false, false);
		P2Select_0.LoadTexture(TransparentColor);
		P2Select_1 = BitmapPicture("Content\\Bitmaps\\Select\\P2Select_1.bmp", 210, 270, true, false, false);
		P2Select_1.LoadTexture(TransparentColor);
		P2Select_2 = BitmapPicture("Content\\Bitmaps\\Select\\P2Select_2.bmp", 210, 270, true, false, false);
		P2Select_2.LoadTexture(TransparentColor);
		P2Select_3 = BitmapPicture("Content\\Bitmaps\\Select\\P2Select_3.bmp", 210, 270, true, false, false);
		P2Select_3.LoadTexture(TransparentColor);
		P2Select_4 = BitmapPicture("Content\\Bitmaps\\Select\\P2Select_4.bmp", 210, 270, true, false, false);
		P2Select_4.LoadTexture(TransparentColor);
		P2Select_5 = BitmapPicture("Content\\Bitmaps\\Select\\P2Select_5.bmp", 210, 270, true, false, false);
		P2Select_5.LoadTexture(TransparentColor);
		P1P2Select_0 = BitmapPicture("Content\\Bitmaps\\Select\\P1P2Select_0.bmp", 210, 270, true, false, false);
		P1P2Select_0.LoadTexture(TransparentColor);
		P1P2Select_1 = BitmapPicture("Content\\Bitmaps\\Select\\P1P2Select_1.bmp", 210, 270, true, false, false);
		P1P2Select_1.LoadTexture(TransparentColor);
		P1P2Select_2 = BitmapPicture("Content\\Bitmaps\\Select\\P1P2Select_2.bmp", 210, 270, true, false, false);
		P1P2Select_2.LoadTexture(TransparentColor);
		P1P2Select_3 = BitmapPicture("Content\\Bitmaps\\Select\\P1P2Select_3.bmp", 210, 270, true, false, false);
		P1P2Select_3.LoadTexture(TransparentColor);
		P1P2Select_4 = BitmapPicture("Content\\Bitmaps\\Select\\P1P2Select_4.bmp", 210, 270, true, false, false);
		P1P2Select_4.LoadTexture(TransparentColor);
		P1P2Select_5 = BitmapPicture("Content\\Bitmaps\\Select\\P1P2Select_5.bmp", 210, 270, true, false, false);
		P1P2Select_5.LoadTexture(TransparentColor);
		BackGround_Select = BitmapPicture("Content\\Bitmaps\\whitecover.bmp", 0, 0, true, false, false);
		BackGround_Select.LoadTexture(TransparentColor);
		Characters_Menu = BitmapPicture("Content\\Bitmaps\\Select\\Characters_Menu.bmp", 210, 270, true, false, false);
		Characters_Menu.LoadTexture(TransparentColor);
		ShowInitProgress(50);
		//讀取所有音效--Begin
		LoadSounds(Sounds.Ding, "Content\\Sounds\\ding.wav");
		LoadSounds(Sounds.Rush, "Content\\Sounds\\rush.wav");
		LoadSounds(Sounds.Jump, "Content\\Sounds\\jump.wav");
		LoadSounds(Sounds.SPCharge, "Content\\Sounds\\SPCharge.wav");
		LoadSounds(Sounds.NormalHit, "Content\\Sounds\\NormalHit.wav");
		LoadSounds(Sounds.HitWall, "Content\\Sounds\\HitWall.wav");
		LoadSounds(Sounds.Disable, "Content\\Sounds\\Disable.wav");
		LoadSounds(Sounds.Stoned, "Content\\Sounds\\Stoned.wav");
		LoadSounds(Sounds.Fire1, "Content\\Sounds\\Fire1.wav");
		LoadSounds(Sounds.CutIn, "Content\\Sounds\\CutIn.wav");
		LoadSounds(Sounds.NormalHit2, "Content\\Sounds\\NormalHit2.wav");
		LoadSounds(Sounds.SbDown, "Content\\Sounds\\SbDown.wav");
		ShowInitProgress(70);
		LoadSounds(Sounds.DoubleHelixXi, "Content\\Sounds\\DoubleHelix-Xi.mp3");
		ShowInitProgress(80);
	}
	#pragma endregion
	#pragma endregion

	#pragma region 遊戲內容
	//各大GameAction的Show跟Move
	#pragma region GameActions

	void GameAction0_initialization()
	{
		GameAction = 0;
		TitleSelection = 0;
	}
	void GameAction1_initialization()
	{
		GameAction = 1;
	}
	void GameAction2_initialization()
	{
		GameAction = 2;
		P1Selection = 0;
		P2Selection = 2;
		SelectedP1 = false;
		SelectedP2 = false;
	}
	void GameAction3_initialization()
	{
		GameAction = 3;
	}
	void GameAction4_initialization()
	{
		GameAction = 4;
	}
	void GameAction5_initialization()
	{
		GameAction = 5;
		LoadingStart = false;//開始讀取布林值
		LoadingDone = false;//讀取完成布林值
	}
	void GameAction6_initialization()
	{
		GameAction = 6;
	}
	void GameAction7_initialization()
	{
		GameAction = 7;
	}
	void GameAction0_OnMove()
	{
		if (GameAction == 0)
		{

			if (KeyState_now.Enter == true && KeyState_last.Enter == false)
			{
				if (TitleSelection == 0)
				{
					GameAction2_initialization();
				}
				else if (TitleSelection == 2)
				{
					ExitGame();
				}
			}
			else if ((KeyState_now.Up == true && KeyState_last.Up == false) || (KeyState_now.W == true && KeyState_last.W == false))
			{
				if (TitleSelection == 0)
				{
					Title_Cursor.Rect.Y = 500;
					TitleSelection = 2;
				}
				else if (TitleSelection == 1)
				{
					Title_Cursor.Rect.Y = 300;
					TitleSelection = 0;
				}
				else if (TitleSelection == 2)
				{
					Title_Cursor.Rect.Y = 400;
					TitleSelection = 1;
				}
			}
			else if ((KeyState_now.Down == true && KeyState_last.Down == false) || (KeyState_now.S == true && KeyState_last.S == false))
			{
				if (TitleSelection == 0)
				{
					Title_Cursor.Rect.Y = 400;
					TitleSelection = 1;
				}
				else if (TitleSelection == 1)
				{
					Title_Cursor.Rect.Y = 500;
					TitleSelection = 2;
				}
				else if (TitleSelection == 2)
				{
					Title_Cursor.Rect.Y = 300;
					TitleSelection = 0;
				}
			}
			BackGround_Title.OnUpdate();
			Title_Bitmap.OnUpdate();
			Title_Start.OnUpdate();
			Title_SkillTable.OnUpdate();
			Title_Exit.OnUpdate();
			Title_Cursor.OnUpdate();
		}
	}
	void GameAction0_OnShow(int i)
	{
		if (GameAction == 0)
		{
			BackGround_Title.Draw(i, 1);
			Title_Bitmap.Draw(i, 3);
			Title_Start.Draw(i, 3);
			Title_SkillTable.Draw(i, 3);
			Title_Exit.Draw(i, 3);
			Title_Cursor.Draw(i, 3);
		}
	}
	void GameAction1_OnMove()
	{
		if (GameAction == 1)
		{

		}
	}
	void GameAction1_OnShow(int i)
	{
		if (GameAction == 1)
		{

		}
	}
	void GameAction2_OnMove()
	{
		if (GameAction == 2)
		{
			if (SelectedP1 == true && SelectedP2 == true)
			{
				Player1Character = P1Selection;
				Player2Character = P2Selection;
				GameAction5_initialization();
			}

			if (KeyState_now.G == true && KeyState_last.G == false)
				SelectedP1 = false;
			if (KeyState_now.K == true && KeyState_last.K == false)
				SelectedP2 = false;

			if (SelectedP1 == false)
			{
				if (KeyState_now.F == true && KeyState_last.F == false)
				{
					SelectedP1 = true;
				}
				else if (KeyState_now.D == true && KeyState_last.D == false)
				{
					P1Selection++;
					if (P1Selection > 5)
					{
						P1Selection = 0;
					}
				}
				else if (KeyState_now.A == true && KeyState_last.A == false)
				{
					P1Selection--;
					if (P1Selection < 0)
					{
						P1Selection = 5;
					}
				}
				else if (KeyState_now.W == true && KeyState_last.W == false)
				{
					P1Selection += 3;
					if (P1Selection > 5)
					{
						P1Selection -= 6;
					}
				}
				else if (KeyState_now.S == true && KeyState_last.S == false)
				{
					P1Selection -= 3;
					if (P1Selection < 0)
					{
						P1Selection += 6;
					}
				}
			}
			if (SelectedP2 == false)
			{
				if (KeyState_now.J == true && KeyState_last.J == false)
				{
					SelectedP2 = true;
				}
				else if (KeyState_now.Right == true && KeyState_last.Right == false)
				{
					P2Selection++;
					if (P2Selection > 5)
					{
						P2Selection = 0;
					}
				}
				else if (KeyState_now.Left == true && KeyState_last.Left == false)
				{
					P2Selection--;
					if (P2Selection < 0)
					{
						P2Selection = 5;
					}
				}
				else if (KeyState_now.Up == true && KeyState_last.Up == false)
				{
					P2Selection += 3;
					if (P2Selection > 5)
					{
						P2Selection -= 6;
					}
				}
				else if (KeyState_now.Down == true && KeyState_last.Down == false)
				{
					P2Selection -= 3;
					if (P2Selection < 0)
					{
						P2Selection += 6;
					}
				}
			}
		}
	}
	void GameAction2_OnShow(int i)
	{
		if (GameAction == 2)
		{
			BackGround_Select.Draw(i, 0);
			Characters_Menu.Draw(i, 1);
			if (P1Selection == P2Selection)
			{
				switch (P1Selection)
				{
				case 0:
					P1P2Select_0.Draw(i, 3);
					break;
				case 1:
					P1P2Select_1.Draw(i, 3);
					break;
				case 2:
					P1P2Select_2.Draw(i, 3);
					break;
				case 3:
					P1P2Select_3.Draw(i, 3);
					break;
				case 4:
					P1P2Select_4.Draw(i, 3);
					break;
				case 5:
					P1P2Select_5.Draw(i, 3);
					break;
				default:
					break;
				}
			}
			else
			{
				switch (P1Selection)
				{
				case 0:
					P1Select_0.Draw(i, 3);
					break;
				case 1:
					P1Select_1.Draw(i, 3);
					break;
				case 2:
					P1Select_2.Draw(i, 3);
					break;
				case 3:
					P1Select_3.Draw(i, 3);
					break;
				case 4:
					P1Select_4.Draw(i, 3);
					break;
				case 5:
					P1Select_5.Draw(i, 3);
					break;
				default:
					break;
				}
				switch (P2Selection)
				{
				case 0:
					P2Select_0.Draw(i, 3);
					break;
				case 1:
					P2Select_1.Draw(i, 3);
					break;
				case 2:
					P2Select_2.Draw(i, 3);
					break;
				case 3:
					P2Select_3.Draw(i, 3);
					break;
				case 4:
					P2Select_4.Draw(i, 3);
					break;
				case 5:
					P2Select_5.Draw(i, 3);
					break;
				default:
					break;
				}
			}
		}
	}
	void GameAction3_OnMove()
	{
		if (GameAction == 3)
		{

		}
	}
	void GameAction3_OnShow(int i)
	{
		if (GameAction == 3)
		{

		}
	}
	void GameAction4_OnMove()
	{
		if (GameAction == 4)
		{

		}
	}
	void GameAction4_OnShow(int i)
	{
		if (GameAction == 4)
		{

		}
	}
	//戰鬥畫面
	#pragma region 戰鬥環境
	//大絕招Cover
	void CutInFunction(BitmapPicture *Cover, BattlePlayer *Player)
	{
		Cover->visable = true;
		string EffectName = Player->GetName() + "_US";
		Player->Effects.Content[Player->GetName() + "_US"].visable = true;
		if (Player->Effects.Content[Player->GetName() + "_US"].Rect.X > 100 || Player->Effects.Content[Player->GetName() + "_US"].Rect.X < -100)
		{
			Player->Effects.Content[Player->GetName() + "_US"].Rect.X -= 65;
		}
		else
		{
			Player->Effects.Content[Player->GetName() + "_US"].Rect.X -= 5;
		}
		if (Player->Effects.Content[Player->GetName() + "_US"].Rect.X < -800)
		{
			Cover->visable = false;
			Player->Effects.Content[Player->GetName() + "_US"].visable = false;
			Player->NeedCutIn = false;
		}
		CameraPosition C;
		Player->Effects.Content[Player->GetName() + "_US"].OnUpdate("Effects", C);
	}
	//撞擊牆壁
	void GotTerrainHit(CameraPosition *C, BattlePlayer *Player, BitmapPicture BK)
	{
		Player->Velocity_X *= -0.75;
		Player->HP -= 15;
		Player->HitFly = true;
		Player->BeHitTimeMax += 200;
		PlaySounds(Sounds.HitWall, false);
		Player->BreakPoint += 30;
		if (Player->BreakPoint > 90)
			Player->BreakPoint = 90;
		Player->Effects.BootEffect(&(Player->Effects.Content["HitWall"]), Camera, Player->BodyRect.X, Player->BodyRect.X - 60, Player->Rect.Y + 10, 0, 0, false, Player->IsRight);
		Sleep(100);
	}
	//地形環境
	void ProduceTerrain(CameraPosition *C, BattlePlayer *P1, BattlePlayer *P2, BitmapPicture BK)
	{
		int CameraMax_right = GAME_SIZE_X + ((BK.Rect.Width - GAME_SIZE_X) / 2) - GAME_SIZE_X;//鏡頭右邊界
		int CameraMax_Left = -(((BK.Rect.Width - GAME_SIZE_X) / 2) - GAME_SIZE_X) - GAME_SIZE_X;//鏡頭左邊界
		#pragma region 地形傷害_X
		if ((P1->Rect.X < CameraMax_Left))
		{
			if (P1->Action == "受傷"&&P1->Velocity_X < -9)
			{
				GotTerrainHit(C, P1, BK);
			}

		}
		if ((P1->Rect.X + P1->Rect.Width > CameraMax_right + GAME_SIZE_X))
		{
			if (P1->Action == "受傷"&&P1->Velocity_X > 9)
			{
				GotTerrainHit(C, P1, BK);
			}

		}
		if ((P2->Rect.X < CameraMax_Left))
		{
			if (P2->Action == "受傷"&&P2->Velocity_X < -9)
			{
				GotTerrainHit(C, P2, BK);
			}

		}
		if ((P2->Rect.X + P2->Rect.Width > CameraMax_right + GAME_SIZE_X))
		{
			if (P2->Action == "受傷"&&P2->Velocity_X > 9)
			{
				GotTerrainHit(C, P2, BK);
			}
		}
		#pragma endregion
		#pragma region 鏡頭控制



		//
		if (P1->Rect.X_int + P1->Rect.Width >= GAME_SIZE_X&&P1->Velocity_X > 0)
		{
			if (C->X_double < CameraMax_right)
				C->X_double += P1->Velocity_X;
			if (P2->Rect.X < C->X_double)
			{
				P2->Rect.X += P1->Velocity_X;
			}
		}
		//

		if (P1->Rect.X_int <= 0 && P1->Velocity_X < 0)
		{
			if (C->X_double > CameraMax_Left)
				C->X_double += P1->Velocity_X;
			if (P2->Rect.X + P2->Rect.Width > C->X_double + GAME_SIZE_X)
			{
				P2->Rect.X += P1->Velocity_X;
			}
		}


		if (P2->Rect.X_int + P2->Rect.Width >= GAME_SIZE_X&&P2->Velocity_X > 0)
		{
			if (C->X_double < CameraMax_right)
				C->X_double += P2->Velocity_X;
			if (P1->Rect.X < C->X_double)
			{
				P1->Rect.X += P2->Velocity_X;
			}
		}
		if (P2->Rect.X_int <= 0 && P2->Velocity_X < 0)
		{
			if (C->X_double > CameraMax_Left)
				C->X_double += P2->Velocity_X;
			if (P1->Rect.X + P1->Rect.Width > C->X_double + GAME_SIZE_X)
			{
				P1->Rect.X += P2->Velocity_X;
			}
		}

		//
		C->X = (int)C->X_double;
		C->Y = (int)C->Y_double;
		if (C->X > CameraMax_right)
			C->X = CameraMax_right;
		if (C->X < CameraMax_Left)
			C->X = CameraMax_Left;
		//

		if (P1->Rect.X_int < 0)
		{
			P1->Rect.X_int = 0;
			P1->Rect.X = P1->Rect.X_int + C->X;
		}

		//
		if (P1->Rect.X_int > GAME_SIZE_X - P1->Rect.Width)
		{
			P1->Rect.X_int = GAME_SIZE_X - P1->Rect.Width;
			P1->Rect.X = P1->Rect.X_int + C->X;
		}
		//

		if (P2->Rect.X_int < 0)
		{
			P2->Rect.X_int = 0;
			P2->Rect.X = P2->Rect.X_int + C->X;
		}
		if (P2->Rect.X_int > GAME_SIZE_X - P2->Rect.Width)
		{
			P2->Rect.X_int = GAME_SIZE_X - P2->Rect.Width;
			P2->Rect.X = P2->Rect.X_int + C->X;
		}
		#pragma endregion
	}
	//戰鬥讀取
	void BattleLoading()
	{
		if (LoadingDone == false)
		{
			//重製相機鏡頭
			Camera = CameraPosition();


			#pragma region 戰鬥背景
			BK = BitmapPicture("Content\\Bitmaps\\BackGround_Fight1.bmp", -400, 0, true, false, true);
			BK.LoadTexture(TransparentColor);
			BlackCover = BitmapPicture("Content\\Bitmaps\\Blackcover.bmp", 0, 0, false, false, false);
			BlackCover.LoadTexture(TransparentColor);
			WhiteCover = BitmapPicture("Content\\Bitmaps\\whitecover.bmp", 0, 0, false, false, false);
			WhiteCover.LoadTexture(TransparentColor);
			ReadyBmp = BitmapPicture("Content\\Bitmaps\\ReadyItem.bmp", 280, 600, false, false, false);
			ReadyBmp.LoadTexture(TransparentColor);
			KoBmp = BitmapPicture("Content\\Bitmaps\\KO.bmp", 0, 200, false, false, false);
			KoBmp.LoadTexture(TransparentColor);
			#pragma endregion

			#pragma region 建置玩家變數
			delete Player1;
			delete Player2;
			Player1 = DecideCharacter(1, Player1Character);
			Player2 = DecideCharacter(2, Player2Character);
			#pragma endregion

			#pragma region 讀取玩家圖檔與設定初始參數
			Player1->AutoLoadBitmaps(Player2, Camera, KeyState_now, KeyState_last, Sounds, TransparentColor);
			Player2->AutoLoadBitmaps(Player1, Camera, KeyState_now, KeyState_last, Sounds, TransparentColor);
			Player1->Rect.X = 250;
			Player1->Rect.Y = GroundPosition - 200;
			Player2->Rect.X = 430;
			Player2->Rect.Y = GroundPosition - 200;
			#pragma endregion

			#pragma region 讀取血量條等等
			Bar_HP1 = Bar("Content\\Bitmaps\\red_bar.bmp", 1, 25, 25, true);
			Bar_HP1.LoadTexture(TransparentColor);
			Bar_HP2 = Bar("Content\\Bitmaps\\red_bar.bmp", 2, 525, 25, true);
			Bar_HP2.LoadTexture(TransparentColor);
			Bar_SP1 = Bar("Content\\Bitmaps\\orange_bar.bmp", 1, 25, 50, true);
			Bar_SP1.LoadTexture(TransparentColor);
			Bar_SP2 = Bar("Content\\Bitmaps\\orange_bar.bmp", 2, 625, 50, true);
			Bar_SP2.LoadTexture(TransparentColor);
			Bar_RE1 = Bar("Content\\Bitmaps\\gray_bar.bmp", 1, 25, 25, true);
			Bar_RE1.LoadTexture(TransparentColor);
			Bar_RE2 = Bar("Content\\Bitmaps\\gray_bar.bmp", 2, 525, 25, true);
			Bar_RE2.LoadTexture(TransparentColor);
			Bar_Player1Break = Bar("Content\\Bitmaps\\BreakBar.bmp", 1, 0, 0, true);
			Bar_Player1Break.LoadTexture(TransparentColor);
			Bar_Player2Break = Bar("Content\\Bitmaps\\BreakBar.bmp", 1, 0, 0, true);
			Bar_Player2Break.LoadTexture(TransparentColor);
			Bar_HP1_MaskTop = BitmapPicture("Content\\Bitmaps\\Red_BarMaskTop.bmp", 20, 25, true, false, false);
			Bar_HP1_MaskTop.LoadTexture(TransparentColor);
			Bar_HP1_MaskBottom = BitmapPicture("Content\\Bitmaps\\Red_BarMaskBottom.bmp", 20, 25, true, false, false);
			Bar_HP1_MaskBottom.LoadTexture(TransparentColor);
			Bar_SP1_MaskTop = BitmapPicture("Content\\Bitmaps\\Orange_BarMaskTop.bmp", 20, 50, true, false, false);
			Bar_SP1_MaskTop.LoadTexture(TransparentColor);
			Bar_SP1_MaskBottom = BitmapPicture("Content\\Bitmaps\\Orange_BarMaskBottom.bmp", 20, 50, true, false, false);
			Bar_SP1_MaskBottom.LoadTexture(TransparentColor);
			Bar_HP2_MaskTop = BitmapPicture("Content\\Bitmaps\\Red_BarMaskTop.bmp", 520, 25, true, false, false);
			Bar_HP2_MaskTop.LoadTexture(TransparentColor);
			Bar_HP2_MaskBottom = BitmapPicture("Content\\Bitmaps\\Red_BarMaskBottom.bmp", 520, 25, true, false, false);
			Bar_HP2_MaskBottom.LoadTexture(TransparentColor);
			Bar_SP2_MaskTop = BitmapPicture("Content\\Bitmaps\\Orange_BarMaskTop.bmp", 620, 50, true, false, false);
			Bar_SP2_MaskTop.LoadTexture(TransparentColor);
			Bar_SP2_MaskBottom = BitmapPicture("Content\\Bitmaps\\Orange_BarMaskBottom.bmp", 620, 50, true, false, false);
			Bar_SP2_MaskBottom.LoadTexture(TransparentColor);
			#pragma endregion

			#pragma region 雜圖讀取
			Player1_Name = BitmapPicture("Content\\Bitmaps\\1P.bmp", 40, GroundPosition - 220, true, false, true);
			Player1_Name.LoadTexture(TransparentColor);
			Player2_Name = BitmapPicture("Content\\Bitmaps\\2P.bmp", 620, GroundPosition - 220, true, false, true);
			Player2_Name.LoadTexture(TransparentColor);
			CutInMask = BitmapPicture("Content\\Bitmaps\\UltimateSkill.bmp", 0, 0, false, false, false);
			CutInMask.LoadTexture(TransparentColor);
			#pragma endregion

			Player1->CanControl = true;
			Player2->CanControl = true;
			SomeBodyDown = false;
			BlackCoverfactor = 1;
			ReadyTimer = 0;
			LoadingDone = true;
		}
	}
	//戰鬥OnMove
	void BattleOnMove()
	{
		BK.OnUpdate(Camera);
		#pragma region 正常更新
		if (Player1->NeedCutIn == false && Player2->NeedCutIn == false)
		{
			Player1->OnUpdate(Player2, Camera, KeyState_now, KeyState_last, Sounds, TransparentColor);
			Player2->OnUpdate(Player1, Camera, KeyState_now, KeyState_last, Sounds, TransparentColor);
			ProduceTerrain(&Camera, Player1, Player2, BK);
		}
		if (Player1->NeedCutIn)
			CutInFunction(&CutInMask, Player1);
		if (Player2->NeedCutIn)
			CutInFunction(&CutInMask, Player2);
		if (ReadyTimer < 1500)
		{
			ReadyTimer += TIMER_TICK_MILLIDECOND;
			Player1->CanControl = false;
			Player2->CanControl = false;
			ReadyBmp.visable = true;
			if (ReadyBmp.Rect.Y > 270)
			{
				ReadyBmp.Rect.Y -= 20;
				if (ReadyBmp.Rect.Y < 270)
				{
					ReadyBmp.Rect.Y = 270;
				}
			}
			if (ReadyTimer >= 1500)
			{
				ReadyTimer = 1500;
				Player1->CanControl = true;
				Player2->CanControl = true;
				ReadyBmp.visable = false;
				PlaySounds(Sounds.SbDown, false);
				PlaySounds(Sounds.DoubleHelixXi, true);
			}
			ReadyBmp.OnUpdate();
		}

		#pragma endregion


		#pragma region 有人被打敗
		if (Player1->HP <= 0 && SomeBodyDown == false)
		{
			SomeBodyDown = true;
			Player1->CanControl = false;
			PlaySounds(Sounds.SbDown, false);
			WinnerTimer = 0;
			WinnerTimer2 = 0;
			CAudio::Instance()->Stop(Sounds.DoubleHelixXi);
		}
		if (Player2->HP <= 0 && SomeBodyDown == false)
		{
			SomeBodyDown = true;
			Player2->CanControl = false;
			PlaySounds(Sounds.SbDown, false);
			WinnerTimer = 0;
			WinnerTimer2 = 0;
			CAudio::Instance()->Stop(Sounds.DoubleHelixXi);
		}

		if (SomeBodyDown == true)
		{
			WinnerTimer += TIMER_TICK_MILLIDECOND;
			WinnerTimer2 += TIMER_TICK_MILLIDECOND;
			if (WinnerTimer < 500 && WinnerTimer2>40)
			{
				WinnerTimer2 = 0;
				if (WhiteCover.visable)
				{
					WhiteCover.visable = false;
				}
				else
				{
					WhiteCover.visable = true;
				}
			}
			if (WinnerTimer >= 1000)
			{
				if (KoBmp.Rect.X < 250)
				{
					KoBmp.Rect.X += 25;
					if (KoBmp.Rect.X > 250)
					{
						KoBmp.Rect.X = 250;
					}
				}
				KoBmp.visable = true;
				KoBmp.OnUpdate();
			}
			Player1->Invincible = true;
			Player2->Invincible = true;
			if (Player1->HP <= 0)
			{
				Player1->CanControl = false;
				Player1->Action = "受傷";
				if (Player1->Rect.Y < GroundPosition)
					Player1->Step = 1;
				else
					Player1->Step = 2;
				Player1->AnimationUpdate(Camera);
				Player1->Throughing = true;
				Player1->Invincible = true;
			}
			if (Player2->HP <= 0)
			{
				Player2->CanControl = false;
				Player2->Action = "受傷";
				if (Player2->Rect.Y < GroundPosition)
					Player2->Step = 1;
				else
					Player2->Step = 2;
				Player2->AnimationUpdate(Camera);
				Player2->Throughing = true;
				Player2->Invincible = true;
			}
			if (WinnerTimer > 2000)
			{
				if (Player1->HP > 0)
				{
					WinnerID = 1;
				}
				else if (Player2->HP > 0)
				{
					WinnerID = 2;
				}
				else
				{
					WinnerID = 0;
				}
				BlackCoverfactor = ((501 - (2500 - WinnerTimer)) / 500);
				BlackCover.visable = true;
				if (BlackCoverfactor > 1)
				{
					BlackCoverfactor = 1;
					GameAction2_initialization();
				}
				BlackCover.OnUpdate();
			}


		}

		#pragma endregion




		Bar_HP1_MaskTop.OnUpdate();
		Bar_HP1_MaskBottom.OnUpdate();
		Bar_SP1_MaskTop.OnUpdate();
		Bar_SP1_MaskBottom.OnUpdate();
		Bar_HP2_MaskTop.OnUpdate();
		Bar_HP2_MaskBottom.OnUpdate();
		Bar_SP2_MaskTop.OnUpdate();
		Bar_SP2_MaskBottom.OnUpdate();
		Bar_Player1Break.Rect.X = Player1->Rect.X + 30;
		Bar_Player1Break.Rect.Y = Player1->Rect.Y + 125;
		Bar_Player2Break.Rect.X = Player2->Rect.X + 30;
		Bar_Player2Break.Rect.Y = Player2->Rect.Y + 125;
		Bar_Player1Break.OnUpdate(Camera);
		Bar_Player2Break.OnUpdate(Camera);
		Player1_Name.Rect.X = Player1->Rect.X + 35;
		Player1_Name.Rect.Y = Player1->Rect.Y - 20;
		Player2_Name.Rect.X = Player2->Rect.X + 35;
		Player2_Name.Rect.Y = Player2->Rect.Y - 20;
		Player1_Name.OnUpdate(Camera);
		Player2_Name.OnUpdate(Camera);

	}
	//戰鬥OnShow
	void BattleOnShow(int i)
	{
		BK.Draw(i, 1);
		BlackCover.Draw(i, 8, BlackCoverfactor);
		WhiteCover.Draw(i, 8);
		KoBmp.Draw(i, 7);
		ReadyBmp.Draw(i, 7);
		Player1->Draw(i, 3, Camera);
		Player2->Draw(i, 3, Camera);
		Bar_HP1.Draw(i, 6, Player1->HP, Player1->HP_Max);
		Bar_HP2.Draw(i, 6, Player2->HP, Player2->HP_Max);
		Bar_SP1.Draw(i, 6, Player1->SP, Player1->SP_Max);
		Bar_SP2.Draw(i, 6, Player2->SP, Player2->SP_Max);

		Bar_Player1Break.Draw(i, 3, Player1->BreakPoint, 90, Camera);
		Bar_Player2Break.Draw(i, 3, Player2->BreakPoint, 90, Camera);

		Bar_HP1_MaskTop.Draw(i, 7);
		Bar_HP1_MaskBottom.Draw(i, 5);
		Bar_SP1_MaskTop.Draw(i, 7);
		Bar_SP1_MaskBottom.Draw(i, 5);
		Bar_HP2_MaskTop.Draw(i, 7);
		Bar_HP2_MaskBottom.Draw(i, 5);
		Bar_SP2_MaskTop.Draw(i, 7);
		Bar_SP2_MaskBottom.Draw(i, 5);
		Bar_RE1.Draw(i, 5, Player1->HP + Player1->recovery, Player1->HP_Max);
		Bar_RE2.Draw(i, 5, Player2->HP + Player2->recovery, Player2->HP_Max);
		Player1_Name.Draw(i, 3);
		Player2_Name.Draw(i, 3);
		CutInMask.Draw(i, 4);
	}
	#pragma endregion 
	void GameAction5_OnMove()
	{
		if (GameAction == 5)
		{
			LoadingResource(BattleLoading, &LoadingThread, &LoadingStart, &LoadingDone);
			LoadingPicture.OnUpdate();
			if (LoadingStart == false && LoadingDone == true)
			{
				GameAction6_initialization();
			}
		}
	}
	void GameAction5_OnShow(int i)
	{
		if (GameAction == 5)
		{
			if (LoadingTemp == false)
			{
				Loadingfactor += 0.0025;
				if (Loadingfactor > 1.1)
				{
					LoadingTemp = true;
				}
			}
			else
			{
				Loadingfactor -= 0.0025;
				if (Loadingfactor < 0.9)
				{
					LoadingTemp = false;
				}
			}
			LoadingPicture.Draw(i, 2, Loadingfactor);
		}
	}
	void GameAction6_OnMove()
	{
		if (GameAction == 6)
		{
			BattleOnMove();
		}
	}
	void GameAction6_OnShow(int i)
	{
		if (GameAction == 6)
		{
			BattleOnShow(i);
		}
	}
	void GameAction7_OnMove()
	{
		if (GameAction == 7)
		{
			GameAction5_initialization();
		}
	}
	void GameAction7_OnShow(int i)
	{
		if (GameAction == 7)
		{
		}
	}

	#pragma endregion
	#pragma endregion

	#pragma region 底層mygame.cpp的運作程序(基本上不用更改)
	//程式開始
	#pragma region Program Initialize
	CGameStateInit::CGameStateInit(CGame *g) : CGameState(g)
	{
	}
	CGameStateInit::~CGameStateInit()
	{
		delete Player1;
		delete Player2;
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
		ShowInitProgress(0);
		GameLoading();
	}
	void CGameStateRun::OnInit()// 讀取檔案
	{
	}
	void CGameStateOver::OnInit()
	{
		ShowInitProgress(100);
		PlaySounds(Sounds.Ding, false);
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
			GameAction0_initialization();
			GotoGameState(GAME_STATE_RUN);

		}
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
		GameAction0_OnMove();
		GameAction1_OnMove();
		GameAction2_OnMove();
		GameAction3_OnMove();
		GameAction4_OnMove();
		GameAction5_OnMove();
		GameAction6_OnMove();
		GameAction7_OnMove();
		KeyState_last = KeyState_now;
	}
	//GameState ShowBitmaps
	void CGameStateRun::OnShow()
	{
		for (int i = 0; i < 10; i++)
		{
			GameAction0_OnShow(i);
			GameAction1_OnShow(i);
			GameAction2_OnShow(i);
			GameAction3_OnShow(i);
			GameAction4_OnShow(i);
			GameAction5_OnShow(i);
			GameAction6_OnShow(i);
			GameAction7_OnShow(i);
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
	#pragma endregion
}