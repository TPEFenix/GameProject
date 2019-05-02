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
#include "FunctionUser.h"


using namespace std;
using namespace WKAudio_namespace;
using namespace CollisionSensor_namespace;
using namespace TypeConverter_namespace;
using namespace FunctionUser_namespace;

namespace game_framework
{
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
		#pragma region 尋找敵人
		if (this->visable && this->IsHited == false && this->Target->Invincible == false)
		{
			if (PixelCollision(&(this->Target->BodyPicture), this->DisplayBitmap, 2))
			{
				#pragma region 防禦狀態

				if ((Target->Action == "防禦" || Target->Action == "防禦受傷") && (this->BitmapisRight != Target->IsRight) && this->HitBreak == false)
				{
					bool IsPerfectBlock = false;


					#pragma region 完美格檔
					if (Target->ClickDefendTimer < PerfectBlockTime)
					{
						Audio_ID Sounds;
						this->Target->Attacks.AttackReset_Normal(
							&(this->Target->Attacks.AttackObjects["Counterattact"]), this->Target, this->Belone,
							30,
							10, 8, this->Target->Rect.X + 0, this->Target->Rect.X - 0, this->Target->Rect.Y, 0, 0,
							120, 130, "PunchHit", Sounds.NormalHit, Camera);
						this->Target->Attacks.AttackObjects["Counterattact"].Drawable = true;
						this->Target->Attacks.AttackObjects["Counterattact"].HitNoon = false;
						this->Target->Attacks.AttackObjects["Counterattact"].CanHitFly = true;
						IsPerfectBlock = true;
					}
					#pragma endregion


					#pragma region 確保穿越狀態跟無敵狀態
					Target->Throughing = false;
					Target->Invincible = false;
					#pragma endregion
					#pragma region 控制方向
					Target->IsRight = !(this->BitmapisRight);
					this->IsHited = true;
					#pragma endregion
					#pragma region 消失物件
					if (this->HitNoon == true)
					{
						this->visable = false;
						this->DisplayBitmap->visable = false;
						this->Drawable = false;
					}
					#pragma endregion
					#pragma region 特效與音效
					PlaySounds(this->HitSound, false);
					Target->Effects.BootEffect(&(Target->Effects.Content[this->HitEffect]), Camera, Target->BodyRect.X + 3, Target->BodyRect.X - 6, Target->Rect.Y + 30, 0, 0, false, this->BitmapisRight);
					#pragma endregion
					#pragma region 生命值與體力值的增減
					if (IsPerfectBlock == false)
					{
						Target->GainHP(-(this->Damage / 4));
						Target->GainSP(-(this->Damage / 30));
						Target->GainSP(-this->SP_Damege);
					}
					#pragma endregion
					#pragma region 物理狀態控制
					Target->Acceleration_X = 0;
					Target->Acceleration_Y = 0;
					Target->Velocity_X = (this->Ahead(this->HitVelocity_X)) / 2;
					Target->Velocity_Y = 0;
					Target->BeHitTimer = 0;
					Target->BeHitTimeMax = 0;
					Target->Step = 0;
					Target->Action = "防禦受傷";
					#pragma endregion

					//延遲
					Sleep(10);
				}
				#pragma endregion


				#pragma region 非防禦狀態或無法防禦
				if (!((Target->Action == "防禦" || Target->Action == "防禦受傷") && (this->BitmapisRight != Target->IsRight) && this->HitBreak == false))
				{

					#pragma region 確保穿越狀態跟無敵狀態
					Target->Throughing = false;
					Target->Invincible = false;
					#pragma endregion
					#pragma region 控制方向
					Target->IsRight = !(this->BitmapisRight);
					this->IsHited = true;
					#pragma endregion
					#pragma region 消失物件
					if (this->HitNoon == true)
					{
						this->visable = false;
						this->DisplayBitmap->visable = false;
						this->Drawable = false;
					}
					#pragma endregion
					#pragma region 特效與音效
					PlaySounds(this->HitSound, false);
					Target->Effects.BootEffect(&(Target->Effects.Content[this->HitEffect]), Camera, Target->BodyRect.X + 3, Target->BodyRect.X - 6, Target->Rect.Y + 30, 0, 0, false, this->BitmapisRight);
					#pragma endregion
					#pragma region 生命值與體力值的增減
					Target->GainHP(-(this->Damage));
					Target->GainSP(+(this->Damage / 15));
					Target->GainSP(-this->SP_Damege);
					if (((int)Target->HP) > 0)
						Target->recovery = Target->recovery + (this->Damage / 1.5);
					else
						Target->recovery = 0;
					#pragma endregion
					#pragma region 屬性
					if (this->Attributes >= 0)
						Target->AttributeState[this->Attributes] = true;
					#pragma endregion
					#pragma region 物理狀態控制
					Target->Acceleration_X = 0;
					Target->Acceleration_Y = 0;
					Target->Velocity_X = this->Ahead(this->HitVelocity_X);
					Target->Velocity_Y = -(this->HitVelocity_Y);
					Target->BeHitTimer = 0;
					Target->BeHitTimeMax = this->HitTime;
					Target->HitFly = this->CanHitFly;
					Target->Step = 0;
					Target->Action = "受傷";
					#pragma endregion
					#pragma region 失衡崩解
					if (Target->HitFly)
					{
						Target->BreakPoint += 30;
						if (Target->BreakPoint > 90)
						{
							Target->BreakPoint = 90;
							Target->BreakPointTimer = 0;
						}
					}
					else
					{
						Target->BreakPoint += 5+(Damage / 10);
						if (Target->BreakPoint > 90)
						{
							Target->BreakPoint = 90;
							Target->BreakPointTimer = 0;
						}
					}
					if (HitBreak&&Target->BreakPoint >= 90)
					{
						Audio_ID Sounds;
						PlaySounds(Sounds.Stoned, false);
						Target->BeHitTimer = 0;
						Target->BeHitTimeMax = 1250;
						Target->BreakPoint = 0;
						Target->BreakPointTimer = 0;
						PlayEffect(Target, "stun_star", Camera, Rect.X - 15, Rect.X - 15, Rect.Y);

					}

					#pragma endregion

					//延遲
					Sleep(10);
				}
				#pragma endregion
			}
		}
		#pragma endregion


		#pragma region 尋找敵人的攻擊物件

		if (this->visable&&this->CanBeDisappear)
		{
			map<string, AttackObj>::iterator iter;
			for (iter = Target->Attacks.AttackObjects.begin(); iter != Target->Attacks.AttackObjects.end(); iter++)
			{
				if (iter->second.visable&&iter->second.CanCrackOther)
				{
					if (PixelCollision(iter->second.DisplayBitmap, this->DisplayBitmap, 2))
					{

						if (Mass < iter->second.Mass)
						{
							double HX, HY = 0;
							HX = this->Rect.X + (this->Rect.Width / 2) - (Effects.Content[this->HitEffect].Rect.Width / 2);
							HY = this->Rect.Y + (this->Rect.Height / 2) - (Effects.Content[this->HitEffect].Rect.Height / 2);
							PlayEffect(&Effects, "Disable", Camera, HX, HX, HY - 30, this->BitmapisRight);
							this->visable = false;
							this->DisplayBitmap->visable = false;
							this->Drawable = false;
							this->IsHited = true;
							Audio_ID sounds;
							PlaySounds(sounds.Disable, false);
						}
						else if (Mass == iter->second.Mass)
						{
							double HX, HY = 0;
							HX = this->Rect.X + (this->Rect.Width / 2) - (Effects.Content[this->HitEffect].Rect.Width / 2);
							HY = this->Rect.Y + (this->Rect.Height / 2) - (Effects.Content[this->HitEffect].Rect.Height / 2);
							PlayEffect(&Effects, "Disable", Camera, HX, HX, HY - 30, this->BitmapisRight);
							HX = iter->second.Rect.X + (iter->second.Rect.Width / 2) - (Effects.Content[iter->second.HitEffect].Rect.Width / 2);
							HY = iter->second.Rect.Y + (iter->second.Rect.Height / 2) - (Effects.Content[iter->second.HitEffect].Rect.Height / 2);
							PlayEffect(&iter->second.Effects, "Disable", Camera, HX, HX, HY - 30, iter->second.BitmapisRight);

							this->visable = false;
							this->DisplayBitmap->visable = false;
							this->Drawable = false;
							this->IsHited = true;
							if (iter->second.CanBeDisappear)
							{
								iter->second.visable = false;
								iter->second.DisplayBitmap->visable = false;
								iter->second.Drawable = false;
								iter->second.IsHited = true;
							}
							Audio_ID sounds;
							PlaySounds(sounds.Disable, false);
						}

					}

				}

			}
		}

		#pragma endregion


		#pragma region 圖片更新
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



		#pragma endregion
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




	AttackManager::AttackManager()
	{
	}
	AttackManager::~AttackManager()
	{
	}
	void AttackManager::AttackAutoUpdate(AttackObj * Attack, string BeloneName, int tick, bool replay, CameraPosition Camera)
	{
		map<string, BitmapAnimation>::iterator Iter_Effect;
		for (Iter_Effect = Attack->Effects.Content.begin(); Iter_Effect != Attack->Effects.Content.end(); Iter_Effect++)
			Attack->Effects.EffectAutoUpdate(&(Iter_Effect->second), (int)(((Iter_Effect->second).PreAutoFrequence)), false, Camera);
		if (Attack->visable)
		{
			if (Attack->IsHited&&Attack->CanCombo)
			{
				Attack->ComboTimer += TIMER_TICK_MILLIDECOND;
				if (Attack->ComboTimer > TIMER_TICK_MILLIDECOND * 5)
				{
					Attack->ComboTimer = 0;
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
		Attack->Mass = Mass;
		Attack->CanBeDisappear = CanBeDisappear;
		Attack->CanCrackOther = CanCrackOther;

		//初次更新
		Attack->OnUpdate(BeloneName + "\\Attacks", Camera);
	}
	void AttackManager::AttackReset_Normal(AttackObjPH_Normal)
	{
		//屬性設定
		Attack->Belone = Belone;
		Attack->Target = Target;
		Attack->Damage = Damage;
		Attack->SP_Damege = 0;
		Attack->HitVelocity_X = HitVelocity_X;
		Attack->HitVelocity_Y = HitVelocity_Y;
		Attack->BitmapisRight = Belone->IsRight;
		if (Attack->BitmapisRight)
			Attack->Rect.X = XR;
		else
			Attack->Rect.X = XL;
		Attack->Rect.Y = Y;
		Attack->Velocity_X = VX;
		Attack->Velocity_Y = VY;
		Attack->HitTime = HitTime;
		Attack->MaxAliveTime = MaxAliveTime;
		Attack->Attributes = -1;
		Attack->CanCombo = false;
		Attack->Drawable = false;
		Attack->Replay = true;
		Attack->HitNoon = true;
		Attack->HitBreak = false;
		Attack->CanHitFly = false;
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
		Attack->Mass = 10;
		Attack->CanBeDisappear = false;
		Attack->CanCrackOther = false;
		//初次更新
		Attack->OnUpdate(Belone->GetName() + "\\Attacks", Camera);
	}
	void AttackManager::AttackReset_Shot(AttackObjPH_Shot)
	{
		//屬性設定
		Attack->Belone = Belone;
		Attack->Target = Target;
		Attack->Damage = Damage;
		Attack->SP_Damege = 0;
		Attack->HitVelocity_X = HitVelocity_X;
		Attack->HitVelocity_Y = HitVelocity_Y;
		Attack->BitmapisRight = Belone->IsRight;
		if (Attack->BitmapisRight)
			Attack->Rect.X = XR;
		else
			Attack->Rect.X = XL;
		Attack->Rect.Y = Y;
		Attack->Velocity_X = VX;
		Attack->Velocity_Y = VY;
		Attack->HitTime = HitTime;
		Attack->MaxAliveTime = MaxAliveTime;
		Attack->Attributes = -1;
		Attack->CanCombo = false;
		Attack->Drawable = true;
		Attack->Replay = true;
		Attack->HitNoon = HitNoon;
		Attack->HitBreak = false;
		Attack->CanHitFly = false;
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
		Attack->Mass = Mass;
		Attack->CanBeDisappear = CanBeDisappear;
		Attack->CanCrackOther = CanCrackOther;
		//初次更新
		Attack->OnUpdate(Belone->GetName() + "\\Attacks", Camera);
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
			iter->second.Effects.DrawAllEffection(i);
		}

	}
	void AttackManager::InsertAttacks(string BeloneName, string name, int maxstep, int drawlayer, double pre, int category, COLORREF color, CameraPosition Camera)
	{
		if (category == 0) { AttackObjects.insert(std::pair<string, AttackObj>(name, AttackObj(name, 0, 0, false, true, true))); }
		AttackObjects[name].SetName(name);
		AttackObjects[name].AutoLoadBitmaps(BeloneName + "\\Attacks", name, maxstep + 1, pre, true, color);
		AttackObjects[name].drawlayer = drawlayer;
		AttackObjects[name].Effects.AutoLoadEffections(color);
		AttackObjects[name].OnUpdate(BeloneName + "\\Attacks", Camera);
	}
	void AttackManager::InsertAttacks(string BeloneName, string name, int maxstep, int drawlayer, double pre, int category, int current, COLORREF color, CameraPosition Camera)
	{
		if (category == 0) { AttackObjects.insert(std::pair<string, AttackObj>(name + "_" + IntToString(0), AttackObj(name, 0, 0, false, true, true))); }
		AttackObjects[name + "_" + IntToString(0)].SetName(name);
		AttackObjects[name + "_" + IntToString(0)].AutoLoadBitmaps(BeloneName + "\\Attacks", name, maxstep + 1, pre, true, color);
		AttackObjects[name + "_" + IntToString(0)].drawlayer = drawlayer;
		AttackObjects[name + "_" + IntToString(0)].Effects.AutoLoadEffections(color);
		AttackObjects[name + "_" + IntToString(0)].OnUpdate(BeloneName + "\\Attacks", Camera);
		for (int i = 1; i < current; i++)
		{
			if (category == 0) { AttackObjects.insert(std::pair<string, AttackObj>(name + "_" + IntToString(i), AttackObj(name, 0, 0, false, true, true))); }
			AttackObjects[name + "_" + IntToString(i)].SetName(name);
			AttackObjects[name + "_" + IntToString(i)].AutoLoadBitmaps(BeloneName + "\\Attacks", name, maxstep + 1, pre, false, color);
			AttackObjects[name + "_" + IntToString(i)].drawlayer = drawlayer;
			AttackObjects[name + "_" + IntToString(i)].Effects.AutoLoadEffections(color);

			map<string, BitmapPicture>::iterator Iter;
			map<string, BitmapPicture>::iterator Iter2 = AttackObjects[name + "_" + IntToString(0)].BitmapPictures.begin();
			for (Iter = AttackObjects[name + "_" + IntToString(i)].BitmapPictures.begin(); Iter != AttackObjects[name + "_" + IntToString(i)].BitmapPictures.end(); Iter++)
			{
				Iter->second.CanPixelCollision = true;
				Iter->second.EffectRect = Iter2->second.EffectRect;
				Iter2++;
			}
			


			AttackObjects[name + "_" + IntToString(i)].OnUpdate(BeloneName + "\\Attacks", Camera);
		}
	}
}
