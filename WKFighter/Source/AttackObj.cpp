#pragma once
#include "stdafx.h"
#include "Resource.h"
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "WKBitmap.h"
#include "CollisionSensor.h"
#include "WKAudio.h"
#include "AttackObj.h"
#include "EffectSprite.h"
#include "TypeConverter.h"
#include "BattlePlayer.h"



using namespace std;
using namespace WKAudio_namespace;
using namespace CollisionSensor_namespace;
using namespace TypeConverter_namespace;

namespace game_framework
{
	AttackManager::AttackManager()
	{
	}


	AttackManager::~AttackManager()
	{
	}
	void AttackManager::AttackAutoUpdate(AttackObj * Attack, string BeloneName, int tick, bool replay, CameraPosition Camera)
	{
		if (Attack->IsHited&&Attack->CanCombo)
		{
			Attack->ComboTimer += TIMER_TICK_MILLIDECOND;
			if (Attack->ComboTimer > TIMER_TICK_MILLIDECOND * 4)
			{
				Attack->IsHited = false;
			}
		}

		if (Attack->visable = true)
			Attack->AliveTimer += TIMER_TICK_MILLIDECOND;
		if (Attack->AliveTimer >= Attack->MaxAliveTime)
		{
			Attack->visable = false;
			Attack->DisplayBitmap->visable = false;
		}
		Attack->AutoPlay(tick, replay);
		Attack->Rect.X += Attack->Velocity_X;
		Attack->Rect.Y += Attack->Velocity_Y;
		Attack->OnUpdate(BeloneName + "\\Attacks", Camera);

	}
	void AttackManager::AttackReset(AttackObjPH)
	{
		//屬性設定
		Attack->Damage = Damage;
		Attack->SP_Damege = SP_Damege;
		Attack->HitVelocity_X = HitVelocity_X;
		Attack->HitVelocity_Y = HitVelocity_Y;
		Attack->BitmapisRight = IsRight;
		if (Attack->BitmapisRight)
			Attack->Rect.X = XR;
		else
			Attack->Rect.X = XL;
		Attack->Rect.Y = Y;
		Attack->Velocity_X = VX;
		Attack->Velocity_Y = VY;
		Attack->HitTime = HitTime;
		Attack->MaxAliveTime = MaxAliveTime;
		Attack->Attributes = Attributes;
		Attack->CanCombo = CanCombo;
		Attack->Drawable = Drawable;
		Attack->Replay = Replay;
		Attack->HitNoon = HitNoon;
		Attack->HitBreak = HitBreak;
		Attack->CanHitFly = CanHitFly;
		Attack->HitEffect = HitEffect;
		Attack->HitSound = HitSound;
		Attack->visable = true;
		Attack->IsHited = false;
		Attack->AutoPlayTimer = 0;
		Attack->Step = 0;
		Attack->AliveTimer = 0;
		Attack->Timer1 = 0;
		Attack->Timer2 = 0;
		Attack->ComboTimer = 0;
		Attack->Belone = Belone;
		Attack->Target = Target;


		//初次更新
		Attack->OnUpdate(BeloneName + "\\Attacks", Camera);
	}
	void AttackManager::DrawAllAttacks(int i)
	{
		map<string, AttackObj>::iterator iter;
		for (iter = AttackObjects.begin(); iter != AttackObjects.end(); iter++)
		{
			if (iter->second.Drawable&&iter->second.visable)
			{
				iter->second.DisplayBitmap->Draw(i, iter->second.drawlayer);
			}
		}
	}
	void AttackManager::InsertAttacks(string BeloneName, string name, int maxstep, int drawlayer, double pre, int category, COLORREF color, CameraPosition Camera)
	{
		if (category == 0) { AttackObjects.insert(std::pair<string, AttackObj>(name, AttackObj(name, 0, 0, false, true, true))); }
		AttackObjects[name].SetName(name);
		AttackObjects[name].AutoLoadBitmaps(BeloneName + "\\Attacks", name, maxstep + 1, pre, true, color);
		AttackObjects[name].drawlayer = drawlayer;
		AttackObjects[name].OnUpdate(BeloneName + "\\Attacks", Camera);
	}

	void AttackManager::InsertAttacks(string BeloneName, string name, int maxstep, int drawlayer, double pre, int category, int current, COLORREF color, CameraPosition Camera)
	{
		for (int i = 0; i < current; i++)
		{
			if (category == 0) { AttackObjects.insert(std::pair<string, AttackObj>(name + "_" + IntToString(i), AttackObj(name, 0, 0, false, true, true))); }
			AttackObjects[name + "_" + IntToString(i)].SetName(name);
			AttackObjects[name + "_" + IntToString(i)].AutoLoadBitmaps(BeloneName + "\\Attacks", name, maxstep + 1, pre, true, color);
			AttackObjects[name + "_" + IntToString(i)].drawlayer = drawlayer;
			AttackObjects[name + "_" + IntToString(i)].OnUpdate(BeloneName + "\\Attacks", Camera);
		}
	}


	AttackObj::AttackObj() :BitmapAnimation()
	{
	}
	AttackObj::AttackObj(string namein, int X, int Y, bool vis, bool CanHit, bool inside)
	{
		Step = 0;
		AutoMaxStep = 0;
		AutoPlayTimer = 0;
		CanPixelCollision = CanHit;
		SetName(namein);
		visable = vis;
		InSideCamera = inside;
		Rect.X = X;
		Rect.Y = Y;
		Rect.X_int = X;
		Rect.Y_int = Y;
		BitmapisRight = true;
		drawlayer = 0;
		TrackPoint = false;
	}
	AttackObj::~AttackObj()
	{
	}
	void AttackObj::OnUpdate(string unsingfolder, CameraPosition Camera)
	{
		string Actionstring;
		if (BitmapisRight)
			Actionstring = "Content\\Bitmaps\\" + unsingfolder + "\\" + GetName() + "_" + IntToString(Step) + ".bmp";
		else
			Actionstring = "Content\\Bitmaps\\" + unsingfolder + "\\" + GetName() + "_" + IntToString(Step) + "_L.bmp";
		char *cc = new char[65535];
		strcpy(cc, Actionstring.c_str());
		DisplayBitmap = &BitmapPictures[cc];
		Rect.Width = DisplayBitmap->Rect.Width;
		Rect.Height = DisplayBitmap->Rect.Height;
		Rect.X += Velocity_X;
		Rect.Y += Velocity_Y;
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
		DisplayBitmap->visable = visable;
		delete[] cc;
	}

	double AttackObj::Ahead(double move)
	{
		{
			double returner = 0;
			if (BitmapisRight)
			{
				returner = move;
			}
			else
			{
				returner = -move;
			}
			return returner;
		}
	}


}
