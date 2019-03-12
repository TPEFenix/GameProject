#include "stdafx.h"
#include "game.h"
#include "MainFrm.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include <direct.h>
#include <string.h>
#include "audio.h"
#include "gamelib.h"
#include "mygame.h"
#include <windows.h>
#include "BitmapPicture.h"


using namespace std;

namespace game_framework 
{
    BitmapPicture::BitmapPicture()
    {
        visable = false;
    }
    BitmapPicture::~BitmapPicture()
    {
    }
    void BitmapPicture::LoadTexture(char *filename,bool CanHit, COLORREF color)
    {
        if (this->isBitmapLoaded == false)
        {
            CanCollision = CanHit;
            const int nx = 0;
            const int ny = 0;
            GAME_ASSERT(!isBitmapLoaded, "A bitmap has been loaded. You can not load another bitmap !!!");
            HBITMAP hbitmap = (HBITMAP)LoadImage(NULL, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
            if (hbitmap == NULL) {
                char error_msg[300];
                sprintf(error_msg, "Loading bitmap from file \"%s\" failed !!!", filename);
                GAME_ASSERT(false, error_msg);
            }
            CBitmap *bmp = CBitmap::FromHandle(hbitmap); // memory will be deleted automatically
            BITMAP bitmapSize;
            bmp->GetBitmap(&bitmapSize);
            this->location.left = nx; this->location.top = ny;
            this->location.right = nx + bitmapSize.bmWidth;
            this->location.bottom = ny + bitmapSize.bmHeight;
            this->SurfaceID = CDDraw::RegisterBitmap(filename, color);
            this->isBitmapLoaded = true;
            Rect.X = 0;
            Rect.Y = 0;
            Rect.X_int = 0;
            Rect.Y_int = 0;
            Rect.Width = Width();
            Rect.Height = Height();
            if (CanHit)//如果此圖片需要碰撞區才使用
            {
                CDC dc;
                CDC* pDC = CDDraw::GetBackCDC();
                dc.CreateCompatibleDC(pDC);
                CBitmap* pOld = dc.SelectObject(bmp);
                for (int y = 0; y <bitmapSize.bmHeight; y++)
                {
                    EffectRect.push_back(vector<bool>());
                    for (int x = 0; x <bitmapSize.bmWidth; x++)
                    {
                        EffectRect[y].push_back(false);
                        COLORREF rgb = dc.GetPixel(x, y);
                        BYTE r = GetRValue(rgb);
                        BYTE g = GetGValue(rgb);
                        BYTE b = GetBValue(rgb);
                        if (RGB(r, g, b) != color)
                        {
                            EffectRect[y][x] = true;
                        }
                    }
                }
                CDDraw::ReleaseBackCDC();
            }
        }
    }
    void BitmapPicture::OnUpdate()
    {

    }
    void BitmapPicture::Draw(int CurrentLayer,int TargetLayer)
    {
        if (CurrentLayer == TargetLayer&&visable)//直到他的圖片層級才可以顯示
        {
            Rect.X_int = (int)Rect.X;
            Rect.Y_int = (int)Rect.Y;
            SetTopLeft(Rect.X_int, Rect.Y_int);
            ShowBitmap();
        }
    }
    void BitmapPicture::Draw(int CurrentLayer, int TargetLayer, CMovingBitmap & Mask)
    {
        if (CurrentLayer == TargetLayer&&visable)//直到他的圖片層級才可以顯示
        {
            Rect.X_int = (int)Rect.X;
            Rect.Y_int = (int)Rect.Y;
            SetTopLeft(Rect.X_int, Rect.Y_int);
            location.right = 10;
            ShowBitmap(Mask);
        }
    }
}