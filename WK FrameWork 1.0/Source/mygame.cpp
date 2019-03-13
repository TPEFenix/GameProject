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

    //���C�������ܼ�
    int GameAction = 0;
    bool DebugMode = true;
    const COLORREF TransparentColor = RGB(100, 120, 0);
    KeyBoardState KeyState_now;//��e����L���A
    KeyBoardState KeyState_last;//�e�@��������L���A

    //�����Ҧ����ե�
#pragma region DebugValueable
    BitmapPicture DebugPicture1 = BitmapPicture(true);
    BitmapPicture DebugPicture2 = BitmapPicture(true);
	BitmapAnimation DebugPicture3 = BitmapAnimation(true);
#pragma endregion 
    //�`�Ƹ귽 �p���T�s������
#pragma region Const Resources
// �w�q�U�ح��Ī��s��
    struct Audio_ID
    {
        const int Ding = 0;
    } Sound;
#pragma endregion 

    //�o�Ǩ禡�����u�Ƶ{���s�g��(�Ĳv�ä��|�]������)
#pragma region RENEWAL Fuction And Objects
    Keycode Keys;
    void ExitGame()
    {
        PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);
    }
	//�x�θI���P�_
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
    //�����I���˴�
    bool PixelCollision(BitmapPicture *Bitmap1, BitmapPicture *Bitmap2, int accuracy)
    {
        if (BitmapPicture_HitRectangle(*Bitmap1, *Bitmap2) == true)
        {
			if (Bitmap1->CanCollision&&Bitmap2->CanCollision&& Bitmap1->visable &&Bitmap2->visable &&accuracy > 0)
			{
				//�M�w�I���|��
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
    //���J���T�귽
    void LoadSounds(int ID, string path)
    {
        //�d��
        //CAudio::Instance()->Load(0, "sounds\\ding.wav");
        CAudio::Instance()->Load(ID, &path[0]);	// ���J�s��0���n��ding.wav
    }
    //�����n��(ID,�O�_���Ƽ���)
    void PlaySounds(int soundID, bool replay)
    {
        CAudio::Instance()->Play(soundID, replay);
    }
    //��ܤ�r(�Ĳv���t�A�ɶq���n�ϥ�)
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
#pragma endregion 

    //�{���}�l
#pragma region Program Initialize
    CGameStateInit::CGameStateInit(CGame *g) : CGameState(g)
    {
    }
    void CGameStateInit::OnBeginState()
    {
		DebugPicture3.AutoLoadBitmaps("ball",4, TransparentColor);
    }
#pragma endregion 

    //�C���}�YŪ���e��
#pragma region Game Loading Progress
    void CGameStateInit::OnInit()
    {

        //Ū���}�l
        ShowInitProgress(0);	// �@�}�l��loading�i�׬�0%
        logo.LoadBitmap("Content\\Bitmaps\\BG.bmp");
        // �i�JCGameStaterRun::OnInit()

    }
    void CGameStateRun::OnInit()  								// �C������Ȥιϧγ]�w
    {
        ShowInitProgress(15);	// ���ӫe�@�Ӫ��A���i�סA���B�i�׵���33%
        if (DebugMode)
        {
            DebugPicture1.LoadTexture("Content\\Bitmaps\\BG.bmp", true, TransparentColor);
            DebugPicture1.Rect.X = 300;
            DebugPicture1.Rect.Y = 300;
            DebugPicture2.LoadTexture("Content\\Bitmaps\\����H_�a������2_4.bmp", true, TransparentColor);

        }
        ShowInitProgress(33);	// ���ӫe�@�Ӫ��A���i�סA���B�i�׵���33%
        if (DebugMode == false)
        {
            int i;
            for (i = 0; i < NUMBALLS; i++)
                ball[i].LoadBitmap();								// ���J��i�Ӳy���ϧ�
            eraser.LoadBitmap();
            background.LoadBitmap(IDB_BACKGROUND);					// ���J�I�����ϧ�
                                                                    //
                                                                    // ��������Loading�ʧ@�A�����i��
                                                                    //
            ShowInitProgress(50);
            Sleep(300); // ��C�A�H�K�ݲM���i�סA��ڹC���ЧR����Sleep
                        //
                        // �~����J��L���
                        //
            help.LoadBitmap(IDB_HELP, RGB(255, 255, 255));				// ���J�������ϧ�
            corner.LoadBitmap(IDB_CORNER);							// ���J�����ϧ�
            corner.ShowBitmap(background);							// �Ncorner�K��background
            bball.LoadBitmap();										// ���J�ϧ�
            hits_left.LoadBitmap();
        }

        LoadSounds(Sound.Ding, "sounds\\ding.wav");
        LoadSounds(1, "sounds\\lake.mp3");
        CAudio::Instance()->Load(2, "sounds\\ntut.mid");	// ���J�s��2���n��ntut.mid
        CAudio::Instance()->Load(3, "sounds\\KEMURIKUSA.mp3");	// ���J�s��3���n��ntut.mid
                                                                    //
                                                                    // ��OnInit�ʧ@�|����CGameStaterOver::OnInit()�A�ҥH�i���٨S��100%
                                                                    //
    }
    void CGameStateOver::OnInit()
    {
        //
        // ��ϫܦh�ɡAOnInit���J�Ҧ����ϭn��ܦh�ɶ��C���קK���C�����H
        //     �������@�СA�C���|�X�{�uLoading ...�v�A���Loading���i�סC
        //
        ShowInitProgress(50);	// ���ӫe�@�Ӫ��A���i�סA���B�i�׵���66%
                                //
                                // �}�l���J���
                                //
        Sleep(50);				// ��C�A�H�K�ݲM���i�סA��ڹC���ЧR����Sleep
        for (int i = 75; i <= 100; i += 1)
        {
            ShowInitProgress(i);
            Sleep(5);
        }
        // �̲׶i�׬�100%
        //
        ShowInitProgress(100);
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
            // �K�Wlogo
            //
            logo.SetTopLeft((SIZE_X - logo.Width()) / 2, SIZE_Y / 8);
            logo.ShowBitmap();
            //
            // Demo�ù��r�����ϥΡA���L�}�o�ɽкɶq�קK�����ϥΦr���A���CMovingBitmap����n
            //
            CDC *pDC = CDDraw::GetBackCDC();			// ���o Back Plain �� CDC 
            CFont f, *fp;
            f.CreatePointFont(160, "Times New Roman");	// ���� font f; 160���16 point���r
            fp = pDC->SelectObject(&f);					// ��� font f
            pDC->SetBkColor(RGB(0, 0, 0));
            pDC->SetTextColor(RGB(255, 255, 0));
            pDC->TextOut(120, 220, "Please click mouse or press SPACE to begin.");
            pDC->TextOut(5, 395, "Press Ctrl-F to switch in between window mode and full screen mode.");
            if (ENABLE_GAME_PAUSE)
                pDC->TextOut(5, 425, "Press Ctrl-Q to pause the Game.");
            pDC->TextOut(5, 455, "Press Alt-F4 or ESC to Quit.");
            pDC->SelectObject(fp);						// �� font f (�d�U���n�|�F��)
            CDDraw::ReleaseBackCDC();					// �� Back Plain �� CDC
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

    //�C���i��e��
#pragma region GameRunning
//���F�}�Y�H�~���C���D��(�N�HGameAction�����C������)
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
        for (int i = 0; i < NUMBALLS; i++) {				// �]�w�y���_�l�y��
            int x_pos = i % BALL_PER_ROW;
            int y_pos = i / BALL_PER_ROW;
            ball[i].SetXY(x_pos * BALL_GAP + BALL_XY_OFFSET, y_pos * BALL_GAP + BALL_XY_OFFSET);
            ball[i].SetDelay(x_pos);
            ball[i].SetIsAlive(true);
        }
        eraser.Initialize();
        background.SetTopLeft(BACKGROUND_X, 0);				// �]�w�I�����_�l�y��
        help.SetTopLeft(0, SIZE_Y - help.Height());			// �]�w�����Ϫ��_�l�y��
        hits_left.SetInteger(HITS_LEFT);					// ���w�ѤU��������
        hits_left.SetTopLeft(HITS_LEFT_X, HITS_LEFT_Y);		// ���w�ѤU�����ƪ��y��
        CAudio::Instance()->Play(1, true);			// ���� WAVE
        CAudio::Instance()->Play(Sound.Ding, false);		// ���� WAVE
        CAudio::Instance()->Play(3, true);			// ���� MIDI
    }


    //GameState LogicUpdate
    //�o�̳B�z�C���޿�P�_
    void CGameStateRun::OnMove()							// ���ʹC������
    {
        //
        // �p�G�Ʊ�ק�cursor���˦��A�h�N�U���{����commment�����Y�i
        //
        // SetCursor(AfxGetApp()->LoadCursor(IDC_GAMECURSOR));
        //
        // ���ʭI���Ϫ��y��
        //
        if (background.Top() > SIZE_Y)
            background.SetTopLeft(60, -background.Height());
        background.SetTopLeft(background.Left(), background.Top() + 1);
        //
        // ���ʲy
        //
        int i;
        for (i = 0; i < NUMBALLS; i++)
            ball[i].OnMove();
        //
        // �������l
        //
        eraser.OnMove();
        //
        // �P�_���l�O�_�I��y
        //
        for (i = 0; i < NUMBALLS; i++)
            if (ball[i].IsAlive() && ball[i].HitEraser(&eraser)) {
                ball[i].SetIsAlive(false);
                CAudio::Instance()->Play(0);
                hits_left.Add(-1);
                //
                // �Y�Ѿl�I�����Ƭ�0�A�h����Game Over���A
                //
                if (hits_left.GetInteger() <= 0) {
                    CAudio::Instance()->Stop(2);	// ���� WAVE
                    CAudio::Instance()->Stop(3);	// ���� MIDI
                    GotoGameState(GAME_STATE_OVER);
                }
            }
        //
        // ���ʼu�����y
        //
        bball.OnMove();
    }



    void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
    {
        const char KEY_LEFT = 0x25; // keyboard���b�Y
        const char KEY_UP = 0x26; // keyboard�W�b�Y
        const char KEY_RIGHT = 0x27; // keyboard�k�b�Y
        const char KEY_DOWN = 0x28; // keyboard�U�b�Y
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
        const char KEY_LEFT = 0x25; // keyboard���b�Y
        const char KEY_UP = 0x26; // keyboard�W�b�Y
        const char KEY_RIGHT = 0x27; // keyboard�k�b�Y
        const char KEY_DOWN = 0x28; // keyboard�U�b�Y
        if (nChar == KEY_LEFT)
            eraser.SetMovingLeft(false);
        if (nChar == KEY_RIGHT)
            eraser.SetMovingRight(false);
        if (nChar == KEY_UP)
            eraser.SetMovingUp(false);
        if (nChar == KEY_DOWN)
            eraser.SetMovingDown(false);
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


    //GameState LogicUpdate
    //�o�̳B�z�Ϥ��K��
    void CGameStateRun::OnShow()
    {
        //
        //  �`�N�GShow�̭��d�U���n���ʥ��󪫥󪺮y�СA���ʮy�Ъ��u�@����Move���~��A
        //        �_�h��������sø�Ϯ�(OnDraw)�A����N�|���ʡA�ݰ_�ӷ|�ܩǡC���ӳN�y
        //        ���AMove�t�dMVC����Model�AShow�t�dView�A��View�������Model�C
        //
        //
        //  �K�W�I���ϡB�����ơB�y�B���l�B�u�����y
        //
        background.ShowBitmap();			// �K�W�I����
        help.ShowBitmap();					// �K�W������
        hits_left.ShowBitmap();
        for (int i = 0; i < NUMBALLS; i++)
            ball[i].OnShow();				// �K�W��i���y
        bball.OnShow();						// �K�W�u�����y
        eraser.OnShow();					// �K�W���l
        //
        //  �K�W���W�Υk�U��������
        //
        corner.SetTopLeft(0, 0);
        corner.ShowBitmap();
        corner.SetTopLeft(SIZE_X - corner.Width(), SIZE_Y - corner.Height());
        corner.ShowBitmap();
    }
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



}