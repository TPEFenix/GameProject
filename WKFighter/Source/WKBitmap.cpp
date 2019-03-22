#pragma once
#include "stdafx.h"
#include <ddraw.h>
#include <windows.h>
#include <stdio.h>
#include <sstream>
#include "gamelib.h"
#include "WKBitmap.h"
#include "TypeConverter.h"
using namespace std;
using namespace TypeConverter_namespace;
namespace game_framework 
{
    /////////////////////////////////////////////////////////////////////////////
    // BitmapPicture: 提供一個以下功能具備的Bitmap圖片
    //1.透明色設定
    //2.執行像素碰撞
    //3.可以移動
    /////////////////////////////////////////////////////////////////////////////
    BitmapPicture::BitmapPicture()
    {
        Rect.X = 0;
        Rect.Y = 0;
        Rect.X_int = 0;
        Rect.Y_int = 0;
        visable = false;
        ResourcePath = "";
        CanPixelCollision = false;
        InSideCamera = false;
    }
    BitmapPicture::BitmapPicture(bool vis)
    {
        Rect.X = 0;
        Rect.Y = 0;
        Rect.X_int = 0;
        Rect.Y_int = 0;
        visable = vis;
        ResourcePath = "";
        CanPixelCollision = false;
        InSideCamera = false;
    }
    BitmapPicture::BitmapPicture(string respath, bool vis)
    {
        Rect.X = 0;
        Rect.Y = 0;
        Rect.X_int = 0;
        Rect.Y_int = 0;
        visable = vis;
        ResourcePath = respath;
        CanPixelCollision = false;
        InSideCamera = false;
    }
    BitmapPicture::BitmapPicture(string respath, bool vis,bool inside)
    {
        Rect.X = 0;
        Rect.Y = 0;
        Rect.X_int = 0;
        Rect.Y_int = 0;
        visable = vis;
        ResourcePath = respath;
        CanPixelCollision = false;
        InSideCamera = inside;
    }
    BitmapPicture::BitmapPicture(string respath, int X_in, int Y_in, bool vis, bool canhit, bool inside)
    {
        Rect.X = X_in;
        Rect.Y = Y_in;
        Rect.X_int = X_in;
        Rect.Y_int = Y_in;
        visable = vis;
        ResourcePath = respath;
        CanPixelCollision = canhit;
        InSideCamera = inside;
    }
    BitmapPicture::~BitmapPicture()
    {
    }
    void BitmapPicture::LoadTexture(COLORREF color)
    {
        char cc[3000], *filename;
        strcpy(cc, ResourcePath.c_str());
        filename = cc;
        if (this->isBitmapLoaded == false)
        {
            const int nx = 0;
            const int ny = 0;
            GAME_ASSERT(!isBitmapLoaded, "A bitmap has been loaded. You can not load another bitmap !!!");
            HBITMAP hbitmap = (HBITMAP)LoadImage(NULL, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
            if (hbitmap == NULL)
            {
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
            Rect.Width = Width();
            Rect.Height = Height();
            if (CanPixelCollision)//如果此圖片需要碰撞區才使用
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

                        if (RGB(GetRValue(dc.GetPixel(x, y)), GetGValue(dc.GetPixel(x, y)), GetBValue(dc.GetPixel(x, y))) != color)
                        {
                            EffectRect[y][x] = true;
                        }
                    }
                }
                CDDraw::ReleaseBackCDC();
            }
        }
    }
    void BitmapPicture::LoadTexture(bool CanHit, COLORREF color)
    {
        char cc[3000],*filename;
        strcpy(cc, ResourcePath.c_str());
        filename = cc;
        if (this->isBitmapLoaded == false)
        {
            CanPixelCollision = CanHit;
            const int nx = 0;
            const int ny = 0;
            GAME_ASSERT(!isBitmapLoaded, "A bitmap has been loaded. You can not load another bitmap !!!");
            HBITMAP hbitmap = (HBITMAP)LoadImage(NULL, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
            if (hbitmap == NULL) 
			{
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
                        
                        if (RGB(GetRValue(dc.GetPixel(x, y)), GetGValue(dc.GetPixel(x, y)), GetBValue(dc.GetPixel(x, y))) != color)
                        {
                            EffectRect[y][x] = true;
                        }
                    }
                }
                CDDraw::ReleaseBackCDC();
            }
        }
    }
    void BitmapPicture::LoadTexture(char *filename,bool CanHit, COLORREF color)
    {
        if (this->isBitmapLoaded == false)
        {
            CanPixelCollision = CanHit;
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
        Rect.X_int = (int)(Rect.X);
        Rect.Y_int = (int)(Rect.Y);
    }
    void BitmapPicture::OnUpdate(CameraPosition Camera)
    {
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
    }
    void BitmapPicture::Draw(int CurrentLayer,int TargetLayer)
    {
        if (CurrentLayer == TargetLayer&&this->visable == true)//直到他的圖片層級才可以顯示
        {
            SetTopLeft(Rect.X_int, Rect.Y_int);
            ShowBitmap();
        }
    }
    void BitmapPicture::SetResourcePath(string namein)
    {
        ResourcePath = namein;
    }
    string BitmapPicture::GetResourcePath()
    {
        return ResourcePath;
    }

    /////////////////////////////////////////////////////////////////////////////
    // BitmapAnimation: 提供一個以下功能具備的Bitmap圖片
    //1.透明色設定
    //2.執行像素碰撞(每張圖都有獨立的碰撞判斷)
    //3.可以移動
    //4.包含了一系列的BitmapPicture，可以此做成會動的圖片
    /////////////////////////////////////////////////////////////////////////////
    BitmapAnimation::BitmapAnimation()
    {
        Name = "";
        Step = 0;
        AutoMaxStep = 0;
        AutoPlayTimer = 0;
        CanPixelCollision = false;
        visable = false;
    }
    BitmapAnimation::BitmapAnimation(bool vis)
	{
        Name = "";
        Step = 0;
        AutoMaxStep = 0;
        AutoPlayTimer = 0;
        CanPixelCollision = false;
		visable = vis;
	}
    BitmapAnimation::BitmapAnimation(string namein, bool vis)
    {
        Step = 0;
        AutoMaxStep = 0;
        AutoPlayTimer = 0;
        CanPixelCollision = false;
        Name = namein;
        visable = vis;
    }
    BitmapAnimation::BitmapAnimation(string namein, bool vis,bool inside)
    {
        Step = 0;
        AutoMaxStep = 0;
        AutoPlayTimer = 0;
        CanPixelCollision = false;
        Name = namein;
        visable = vis;
        InSideCamera = inside;
    }
    BitmapAnimation::BitmapAnimation(string namein, int X, int Y, bool vis, bool CanHit, bool inside)
    {
        Step = 0;
        AutoMaxStep = 0;
        AutoPlayTimer = 0;
        CanPixelCollision = CanHit;
        Name = namein;
        visable = vis;
        InSideCamera = inside;
        Rect.X = X;
        Rect.Y = Y;
        Rect.X_int = X;
        Rect.Y_int = Y;
    }
    BitmapAnimation::~BitmapAnimation()
	{
	}
    void BitmapAnimation::AutoPlay(int frequence)
    {
        if (AutoPlayTimer >= frequence)
        {
            AutoPlayTimer = 0;
            if (Step < AutoMaxStep-1)
            {
                Step += 1;
            }
            else
            {
                Step = 0;
            }
        }
        else
        {
            AutoPlayTimer += TIMER_TICK_MILLIDECOND;
        }

    }
    void BitmapAnimation::AutoLoadBitmaps(string name,int MaxSteps,bool CanPixelCollisionin,COLORREF color)
	{
        AutoMaxStep = MaxSteps;
        CanPixelCollision = CanPixelCollisionin;
		string StepString = IntToString(MaxSteps);
		BitmapPictures = map<string, BitmapPicture>();
		for (int i = 0; i < MaxSteps; i += 1)
		{
            string str = ("Content\\Bitmaps\\" + name + "\\" + name + "_" + IntToString(i) + ".bmp");
			BitmapPictures.insert(std::pair<string, BitmapPicture>(str, BitmapPicture(visable)));
            static char cc[65535];
            strcpy(cc, str.c_str());
            BitmapPictures[str].LoadTexture(cc, CanPixelCollisionin,color);
		}
        OnUpdate();
	}
    void BitmapAnimation::OnUpdate()
    {
        string Actionstring = "Content\\Bitmaps\\" + Name + "\\" + Name + "_" + IntToString(Step) + ".bmp";
        static char cc[65535];
        strcpy(cc, Actionstring.c_str());
        DisplayBitmap = &BitmapPictures[cc];
        Rect.Width = DisplayBitmap->Rect.Width;
        Rect.Height = DisplayBitmap->Rect.Height;
        Rect.X_int = (int)(Rect.X);
        Rect.Y_int = (int)(Rect.Y);
        DisplayBitmap->Rect.X = Rect.X_int;
        DisplayBitmap->Rect.Y = Rect.Y_int;
        DisplayBitmap->OnUpdate();
    }
    void BitmapAnimation::OnUpdate(CameraPosition Camera)
    {

        string Actionstring = "Content\\Bitmaps\\" + Name + "\\" + Name + "_" + IntToString(Step) + ".bmp";
        static char cc[65535];
        strcpy(cc, Actionstring.c_str());
        DisplayBitmap = &BitmapPictures[cc];
        Rect.Width = DisplayBitmap->Rect.Width;
        Rect.Height = DisplayBitmap->Rect.Height;
        if (InSideCamera)
        {
            Rect.X_int = (int)(Rect.X - Camera.X);
            Rect.Y_int = (int)(Rect.Y - Camera.Y);
        }
        else
        {
            Rect.X_int = (int)(Rect.X );
            Rect.Y_int = (int)(Rect.Y );
        }
        DisplayBitmap->Rect.X = Rect.X_int;
        DisplayBitmap->Rect.Y = Rect.Y_int;
        DisplayBitmap->OnUpdate();
    }
    string BitmapAnimation::GetName()
    {
        return Name;
    }
    void BitmapAnimation::SetName(string namein)
    {
        Name = namein;

    }

}