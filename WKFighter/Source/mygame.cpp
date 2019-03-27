//GameFramework����
#pragma region Game Framework Introduction
/*
* mygame.cpp: ���ɮ��x�C��������class��implementation
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

//�禡�w�ޤJ
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
#include "BattlePlayer.h"
#include "CollisionSensor.h"
#include "TypeConverter.h"
#include "WKAudio.h"
#include "Bar.h"
#include "Characters.h"

#pragma endregion 

//�R�W�Ŷ��ޤJ
#pragma region NamespaceImport
using namespace std;
using namespace CollisionSensor_namespace;
using namespace TypeConverter_namespace;
using namespace WKAudio_namespace;
#pragma endregion 

namespace game_framework
{
    //���C�������ܼ�
#pragma region Global
    //�޿�
    int GameAction = 0;//�C������
    const bool DebugMode = true;//�O�_�ҥ�Debug�Ҧ�
    const bool LoaddingBoost = true;//�ϧ_�ҥ�Ū���[�t
    bool CloseingDebug = false;
    //����
    BitmapPicture BK;
    Bar Bar_HP1;
    Bar Bar_HP2;
    Bar Bar_SP1;
    Bar Bar_SP2;
    Bar Bar_RE1;
    Bar Bar_RE2;
    BitmapPicture Bar_HP1_MaskTop;
    BitmapPicture Bar_HP1_MaskBottom;
    BitmapPicture Bar_SP1_MaskTop;
    BitmapPicture Bar_SP1_MaskBottom;
    BitmapPicture Bar_HP2_MaskTop;
    BitmapPicture Bar_HP2_MaskBottom;
    BitmapPicture Bar_SP2_MaskTop;
    BitmapPicture Bar_SP2_MaskBottom;
    bool played = false;

    //���
    CameraPosition Camera;//�C�����Y
    const COLORREF TransparentColor = RGB(100, 120, 0);//�z����]�w
    const int left = 1;
    const int right = 2;

    //�n��
    const Audio_ID Sounds;//���ĸ귽�s�X

    //��L
    const Keycode Keys;//��L�r�媫��
    KeyBoardState KeyState_now;//��e����L���A
    KeyBoardState KeyState_last;//�e�@��������L���A
#pragma endregion 


 //�}�Y�e���ܼ�
#pragma region GameAction_Title
    BitmapPicture BackGround_Title;
    BitmapPicture Title_Bitmap;
#pragma endregion 

    //�D����ܼ�
#pragma region GameAction_Menu
    BitmapPicture BackGround_Menu;
    int TitleSelection = 0;
#pragma endregion 

    BattlePlayer *Player1;
    BattlePlayer *Player2;




    //�o�Ǩ禡�����u�Ƶ{���s�g��(�Ĳv�ä��|�]������)
#pragma region RENEWAL Fuction And Objects
    void ExitGame()
    {
        PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);
    }
    //�Ĳv�ܮt�ɶq���n�ϥ�
    void Showtext(char *mes, int X, int Y, int fontsize, COLORREF BK, COLORREF FC, int WorkingLayer, int TargetLayer)
    {
        if (WorkingLayer == TargetLayer)
        {
            CDC *pDC = CDDraw::GetBackCDC();			// ���o Back Plain �� CDC 
            CFont f, *fp;
            f.CreatePointFont(fontsize * 10, "Times New Roman");	// ���� font f; 160���16 point���r
            fp = pDC->SelectObject(&f);					// ��� font f
            pDC->SetBkColor(BK);
            pDC->SetTextColor(FC);
            pDC->TextOut(X, Y, mes);
            pDC->SelectObject(fp);						// �� font f (�d�U���n�|�F��)
            CDDraw::ReleaseBackCDC();					// �� Back Plain �� CDC
        }
    }
    //�a������
    void ProduceTerrain(CameraPosition *C, BattlePlayer *P1, BattlePlayer *P2, BitmapPicture BK)
    {
        int CameraMax_right = GAME_SIZE_X + ((BK.Rect.Width - GAME_SIZE_X) / 2) - GAME_SIZE_X;//���Y�k���
        int CameraMax_Left = -(((BK.Rect.Width - GAME_SIZE_X) / 2) - GAME_SIZE_X) - GAME_SIZE_X;//���Y�����

        if (P1->Rect.X_int + P1->Rect.Width >= GAME_SIZE_X&&P1->Velocity_X > 0)
        {
            if (C->X_double < CameraMax_right)
                C->X_double += P1->Velocity_X;
            if (P2->Rect.X < C->X_double)
            {
                P2->Rect.X += P1->Velocity_X;
            }
        }
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



        if ((P1->Rect.X < CameraMax_Left))
        {
            if (P1->Action == "����"&&P1->Velocity_X < -12)
            {
                P1->Velocity_X *= -1;
                P1->HP -= 20;
                P1->HitFly = true;
                P1->BeHitTimeMax += 200;
                PlaySounds(Sounds.HitWall, false);
                Sleep(100);
            }

        }
        if ((P1->Rect.X + P1->Rect.Width > CameraMax_right + GAME_SIZE_X))
        {
            if (P1->Action == "����"&&P1->Velocity_X > 12)
            {
                P1->Velocity_X *= -1;
                P1->HP -= 20;
                P1->HitFly = true;
                P1->BeHitTimeMax += 200;
                PlaySounds(Sounds.HitWall, false);
                Sleep(100);
            }

        }
        if ((P2->Rect.X < CameraMax_Left))
        {
            if (P2->Action == "����"&&P2->Velocity_X < -12)
            {
                P2->Velocity_X *= -1;
                P2->HP -= 20;
                P2->HitFly = true;
                P2->BeHitTimeMax += 200;
                PlaySounds(Sounds.HitWall, false);
                Sleep(100);
            }

        }
        if ((P2->Rect.X + P2->Rect.Width > CameraMax_right + GAME_SIZE_X))
        {
            if (P2->Action == "����"&&P2->Velocity_X > 12)
            {
                P2->Velocity_X *= -1;
                P2->HP -= 20;
                P2->HitFly = true;
                P2->BeHitTimeMax += 200;
                PlaySounds(Sounds.HitWall, false);
                Sleep(100);
            }

        }

        C->X = (int)C->X_double;
        C->Y = (int)C->Y_double;
        if (C->X > CameraMax_right)
            C->X = CameraMax_right;
        if (C->X < CameraMax_Left)
            C->X = CameraMax_Left;

        if (P1->Rect.X_int < 0)
        {
            P1->Rect.X_int = 0;
            P1->Rect.X = P1->Rect.X_int + C->X;
        }
        if (P1->Rect.X_int > GAME_SIZE_X - P1->Rect.Width)
        {
            P1->Rect.X_int = GAME_SIZE_X - P1->Rect.Width;
            P1->Rect.X = P1->Rect.X_int + C->X;
        }
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
    }

#pragma endregion 

    //�{���}�l
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

    //�C���}�YŪ���e��
#pragma region Game Loading Progress
    void CGameStateInit::OnInit()
    {
        //Ū���}�l
        ShowInitProgress(0);	// �@�}�l��loading�i�׬�0%
#pragma region Loadding Effect
        // �i�JCGameStaterRun::OnInit()
        for (int i = 0; i <= 40; i += 1)
        {
            ShowInitProgress(i);
            if (!LoaddingBoost)
                Sleep(10);
            else
                i = 100;
        }
#pragma endregion 
        //Ū���Ҧ�����--Debug�Ҧ�Ū���ɮ�Begin
        DebugmodeLoading();
        //Ū���Ҧ�����--Debug�Ҧ�Ū���ɮ�End
    }
    void CGameStateRun::OnInit()// Ū���ɮ�
    {
        //Ū���Ҧ�����--Begin
        BackGround_Title = BitmapPicture("Content\\Bitmaps\\BackGround_Title.bmp", 0, 0, true, false, false);
        BackGround_Title.LoadTexture(TransparentColor);
        Title_Bitmap = BitmapPicture("Content\\Bitmaps\\Title.bmp", 100, 0, true, false, false);
        Title_Bitmap.LoadTexture(TransparentColor);
        BackGround_Menu = BitmapPicture("Content\\Bitmaps\\BackGround_Menu.bmp", 0, 0, true, false, false);
        BackGround_Menu.LoadTexture(TransparentColor);

        //Ū���Ҧ�����--End
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
        ShowInitProgress(50);	// ���ӫe�@�Ӫ��A���i�סA���B�i�׵���33%
        for (int i = 50; i <= 75; i += 1)
        {
            ShowInitProgress(i);
            if (!LoaddingBoost)
                Sleep(1);
            else
                i = 100;
        }
#pragma endregion 
        //Ū���Ҧ�����--Begin
        LoadSounds(Sounds.Ding, "Content\\Sounds\\ding.wav");
        LoadSounds(Sounds.Rush, "Content\\Sounds\\rush.wav");
        LoadSounds(Sounds.Jump, "Content\\Sounds\\jump.wav");
        LoadSounds(Sounds.SPCharge, "Content\\Sounds\\SPCharge.wav");
        LoadSounds(Sounds.BKMusic, "Content\\Sounds\\Lemegeton.mp3");
        LoadSounds(Sounds.NormalHit, "Content\\Sounds\\NormalHit.wav");
        LoadSounds(Sounds.HitWall, "Content\\Sounds\\HitWall.wav");
        //Ū���Ҧ�����--End
        ShowInitProgress(75);
        // ��OnInit�ʧ@�|����CGameStaterOver::OnInit()�A�ҥH�i���٨S��100%
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
        ShowInitProgress(0);	// ���ӫe�@�Ӫ��A���i�סA���B�i�׵���66%
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
            Sleep(500);				// ��C�A�H�K�ݲM���i�סA��ڹC���ЧR����Sleep
    }
#pragma endregion 

    //�C���}�Y�e��
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
            if (played == false)
            {
                played = true;
                //PlaySounds(Sounds.BKMusic, true);
            }
            DebugmodeOnShow();
        }
        else
        {
            for (int i = 0; i < 5; i++)
            {
                BackGround_Title.Draw(i, 1);
                Title_Bitmap.Draw(i, 3);
                Showtext("Press [SPACE] to start the game", 100, 450, 35, RGB(0, 0, 0), RGB(255, 255, 255), i, 3);
            }
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

    //�C���i��e��
#pragma region GameRunning
//���F�}�Y�H�~���C���D��(�N�HGameAction�����C������)
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
        // �p�G�Ʊ�ק�cursor���˦��A�h�N�U���{����commment�����Y�i
        // SetCursor(AfxGetApp()->LoadCursor(IDC_GAMECURSOR));
        if (GameAction == 0)
        {
            BackGround_Menu.OnUpdate();
        }

    }
    //GameState ShowBitmaps
    void CGameStateRun::OnShow()
    {
        //  �`�N�GShow�̭��d�U���n���ʥ��󪫥󪺮y�СA���ʮy�Ъ��u�@����Move���~��A
        //        �_�h��������sø�Ϯ�(OnDraw)�A����N�|���ʡA�ݰ_�ӷ|�ܩǡC���ӳN�y
        //        ���AMove�t�dMVC����Model�AShow�t�dView�A��View�������Model�C
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

    //�ƹ��B�z�ƥ�
#pragma region MouseState
    void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // �B�z�ƹ����ʧ@
    {

    }
    void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
    {

    }
    void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
    {
        // �S�ơC�p�G�ݭn�B�z�ƹ����ʪ��ܡA�gcode�b�o��
    }
    void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  // �B�z�ƹ����ʧ@
    {

    }
    void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
    {

    }
#pragma endregion 

#pragma endregion 

    //Game End-�C�������e��==�h�^�}�Y�e��
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

    //�����Ҧ����ե�
#pragma region DebugValueable

    void CGameStateInit::DebugmodeLoading()
    {
        if (DebugMode)
        {
            Player1 = new Matchstick(1);
            Player2 = new Matchstick(2);
            BK = BitmapPicture("Content\\Bitmaps\\BackGround_Fight1.bmp", -400, 0, true, false, true);
            BK.LoadTexture(TransparentColor);
            Player1->AutoLoadBitmaps(Player2, Camera, KeyState_now, KeyState_last, Sounds, TransparentColor);
            Player2->AutoLoadBitmaps(Player1, Camera, KeyState_now, KeyState_last, Sounds, TransparentColor);
            Player1->Rect.X = 50;
            Player1->Rect.Y = GroundPosition - 200;
            Player2->Rect.X = 630;
            Player2->Rect.Y = GroundPosition - 200;
            Bar_HP1 = Bar("Content\\Bitmaps\\red_bar.bmp", 1, 25, 25, true);
            Bar_HP1.LoadTexture(TransparentColor);
            Bar_HP2 = Bar("Content\\Bitmaps\\red_bar.bmp", 2, 525, 25, true);
            Bar_HP2.LoadTexture(TransparentColor);
            Bar_SP1 = Bar("Content\\Bitmaps\\orange_bar.bmp", 1, 125, 50, true);
            Bar_SP1.LoadTexture(TransparentColor);
            Bar_SP2 = Bar("Content\\Bitmaps\\orange_bar.bmp", 2, 525, 50, true);
            Bar_SP2.LoadTexture(TransparentColor);
            Bar_RE1 = Bar("Content\\Bitmaps\\gray_bar.bmp", 1, 25, 25, true);
            Bar_RE1.LoadTexture(TransparentColor);
            Bar_RE2 = Bar("Content\\Bitmaps\\gray_bar.bmp", 2, 525, 25, true);
            Bar_RE2.LoadTexture(TransparentColor);
            Player1->CanControl = true;
            Player2->CanControl = true;
            Bar_HP1_MaskTop = BitmapPicture("Content\\Bitmaps\\Red_BarMaskTop.bmp", 20, 25, true, false, false);
            Bar_HP1_MaskTop.LoadTexture(TransparentColor);
            Bar_HP1_MaskBottom = BitmapPicture("Content\\Bitmaps\\Red_BarMaskBottom.bmp", 20, 25, true, false, false);
            Bar_HP1_MaskBottom.LoadTexture(TransparentColor);
            Bar_SP1_MaskTop = BitmapPicture("Content\\Bitmaps\\Orange_BarMaskTop.bmp", 120, 50, true, false, false);
            Bar_SP1_MaskTop.LoadTexture(TransparentColor);
            Bar_SP1_MaskBottom = BitmapPicture("Content\\Bitmaps\\Orange_BarMaskBottom.bmp", 120, 50, true, false, false);
            Bar_SP1_MaskBottom.LoadTexture(TransparentColor);
            Bar_HP2_MaskTop = BitmapPicture("Content\\Bitmaps\\Red_BarMaskTop.bmp", 520, 25, true, false, false);
            Bar_HP2_MaskTop.LoadTexture(TransparentColor);
            Bar_HP2_MaskBottom = BitmapPicture("Content\\Bitmaps\\Red_BarMaskBottom.bmp", 520, 25, true, false, false);
            Bar_HP2_MaskBottom.LoadTexture(TransparentColor);
            Bar_SP2_MaskTop = BitmapPicture("Content\\Bitmaps\\Orange_BarMaskTop.bmp", 520, 50, true, false, false);
            Bar_SP2_MaskTop.LoadTexture(TransparentColor);
            Bar_SP2_MaskBottom = BitmapPicture("Content\\Bitmaps\\Orange_BarMaskBottom.bmp", 520, 50, true, false, false);
            Bar_SP2_MaskBottom.LoadTexture(TransparentColor);
        }
    }
    void CGameStateInit::DebugmodeOnShow()
    {
        if (DebugMode&&CloseingDebug == false)
        {
            for (int i = 0; i <= 6; i++)
            {
                BK.Draw(i, 1);
                Player1->Draw(i, 3, Camera);
                Player2->Draw(i, 3, Camera);
                Bar_HP1.Draw(i, 5, Player1->HP, Player1->HP_Max);
                Bar_HP2.Draw(i, 5, Player2->HP, Player2->HP_Max);
                Bar_SP1.Draw(i, 4, Player1->SP, Player1->SP_Max);
                Bar_SP2.Draw(i, 4, Player2->SP, Player2->SP_Max);

                Bar_HP1_MaskTop.Draw(i, 5);
                Bar_HP1_MaskBottom.Draw(i, 3);
                Bar_SP1_MaskTop.Draw(i, 5);
                Bar_SP1_MaskBottom.Draw(i, 3);
                Bar_HP2_MaskTop.Draw(i, 5);
                Bar_HP2_MaskBottom.Draw(i, 3);
                Bar_SP2_MaskTop.Draw(i, 5);
                Bar_SP2_MaskBottom.Draw(i, 3);
                //Player1->BodyPicture.Draw(i,5);
                //Player2->BodyPicture.Draw(i, 5);
                Bar_RE1.Draw(i, 4, Player1->HP + Player1->recovery, Player1->HP_Max);
                Bar_RE2.Draw(i, 4, Player2->HP + Player2->recovery, Player2->HP_Max);
            }
        }
    }
    void CGameStateInit::DebugmodeOnMove()
    {
        if (DebugMode)
        {
            BK.OnUpdate(Camera);
            Player1->OnUpdate(Player2, Camera, KeyState_now, KeyState_last, Sounds, TransparentColor);
            Player2->OnUpdate(Player1, Camera, KeyState_now, KeyState_last, Sounds, TransparentColor);
            ProduceTerrain(&Camera, Player1, Player2, BK);
            Bar_HP1_MaskTop.OnUpdate();
            Bar_HP1_MaskBottom.OnUpdate();
            Bar_SP1_MaskTop.OnUpdate();
            Bar_SP1_MaskBottom.OnUpdate();
            Bar_HP2_MaskTop.OnUpdate();
            Bar_HP2_MaskBottom.OnUpdate();
            Bar_SP2_MaskTop.OnUpdate();
            Bar_SP2_MaskBottom.OnUpdate();
            if (KeyState_now.Space == true)
            {
                //CloseingDebug = true;
                //ExitGame();
            }
        }
    }



#pragma endregion 

}