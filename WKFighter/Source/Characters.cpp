#pragma once
#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include <windows.h>
#include <vector>
#include "audio.h"
#include "gamelib.h"
#include "Keycode.h"
#include "KeyBoardState.h"
#include "WKBitmap.h"
#include "BattlePlayer.h"
#include "CollisionSensor.h"
#include "TypeConverter.h"
#include "WKAudio.h"
#include "AttackObj.h"
#include "Characters.h"
#include "EffectSprite.h"

using namespace std;
using namespace WKAudio_namespace;
using namespace CollisionSensor_namespace;
using namespace TypeConverter_namespace;

namespace game_framework
{
	#pragma region 新增動作教學
	/*
	如何新增一個角色動作
	1.準備好圖片，圖片命名規則(動作向右):角色名稱\動作名稱_Step.bmp
	※全部準備好後，準備左右相反的圖片，並且檔名為:角色名稱\動作名稱_Step_L.bmp

	2.先到void Matchstick::AutoLoadBitmaps(GPH)裡面新增動作→InsertAction("動作名稱", 動作最大數, color);

	3.如果有攻擊物件，至void Matchstick::AutoLoadAttacks(GPH)新增
	Attacks.InsertAttacks(GetName(), "動作名稱", 攻擊物件最大Step數, 繪製圖層, 圖片變化速度, 攻擊種類, color, Camera);

	4.在void Matchstick::OnUpdate(GPH)中新增相對應的函式

	5.編寫Goto函式，主要是用來控制進入該動作時的初始化變數。

	6.到BattlePlayer編寫CanTo。

	7.編寫On函式，動作的實體，以一個IF判斷是否在這個動作裡面，以STEP和TIMER控制動作流程，最後給予一段僵直時間讓角色可以到其他可行的動作(CanTo)。
	*/
	#pragma endregion

	Matchstick::Matchstick(int number) :BattlePlayer()
	{

		//能力值變數
		HP_Max = Matchstick_HP_Max;//最大生命值
		SP_Max = Matchstick_SP_Max;//最大氣力
		recovery = 0;
		Rush_cost = Matchstick_Rush_Cost;//衝刺消耗量
		StandbySPincrements = Matchstick_StandbySPincrements;
		RunningSPincrements = Matchstick_RunningSPincrements;
		RunSpeed = Matchstick_RunSpeed;
		ChargeSPincrements = Matchstick_ChargeSPincrements;

		//現狀變數
		SetName("Matchstick");
		PlayerNumber = number;//玩家編號
		if (number == 1)
		{
			IsRight = true;
		}
		else if (number == 2)
		{
			IsRight = false;
		}
		HP = HP_Max;//當前生命
		SP = SP_Max;//當前氣力
		CanControl = false;//可以控制
		Invincible = false;//無敵狀態
		Action = "待機";//動作狀態
		Step = 0;//當前步驟數
		visable = true;//是否可見
		CanPixelCollision = true;//是否使用像素碰撞，所有動作分割都會套用
		InSideCamera = true;//是否受到鏡頭影響
		Velocity_X = 0;//X速度
		Velocity_Y = 0;//Y速度
		Acceleration_X = 0;//X加速度
		Acceleration_Y = 0;//Y加速度
		Throughing = false;
		HitFly = false;
		BreakPoint = 60;
		Acceleration_gravity = 0.5;


	}
	Matchstick::~Matchstick()
	{

	}

	//讀檔在此
	void Matchstick::AutoLoadBitmaps(GPH)
	{
		//有效判定區BitRect
		BodyPicture.LoadTexture(color);


		InsertAction("待機", 1, color);
		InsertAction("移動", 4, color);
		InsertAction("衝刺", 1, color);
		InsertAction("跳躍", 4, color);
		InsertAction("防禦", 0, color);
		InsertAction("練氣", 3, color);
		InsertAction("受傷", 2, color);
		InsertAction("防禦受傷", 0, color);
		InsertAction("普攻1", 4, color);
		InsertAction("普攻2", 4, color);
		InsertAction("普攻3", 4, color);
		InsertAction("上普", 4, color);
		InsertAction("上特技", 3, color);
		InsertAction("下普", 5, color);
		InsertAction("特技1", 5, color);
		InsertAction("空普1", 4, color);
		InsertAction("空普2", 4, color);
		InsertAction("空下普", 7, color);
		InsertAction("空上普", 7, color);
		InsertAction("衝刺普", 0, color);
		InsertAction("衝刺特技", 1, color);
		InsertAction("大絕", 6, color);

		//LoadEffects
		Effects.AutoLoadEffections(color);
		//LoadAttacks
		AutoLoadAttacks(GPP);
		AnimationUpdate(Camera);
	}

	//註冊攻擊物件
	void Matchstick::AutoLoadAttacks(GPH)
	{
		Attacks.AttackObjects = map<string, AttackObj>();
		Attacks.InsertAttacks(GetName(), "Normal1", 0, 5, 16, 0, color, Camera);
		Attacks.InsertAttacks(GetName(), "Normal2", 0, 5, 16, 0, color, Camera);
		Attacks.InsertAttacks(GetName(), "Normal3", 0, 5, 16, 0, color, Camera);
		Attacks.InsertAttacks(GetName(), "Normal4", 0, 5, 16, 0, color, Camera);
		Attacks.InsertAttacks(GetName(), "Normal5", 0, 5, 16, 0, color, Camera);
		Attacks.InsertAttacks(GetName(), "Normal6", 0, 5, 16, 0, color, Camera);
		Attacks.InsertAttacks(GetName(), "Normal7", 0, 5, 16, 0, color, Camera);
		Attacks.InsertAttacks(GetName(), "RushSkill", 2, 5, 8, 0, color, Camera);
		Attacks.InsertAttacks(GetName(), "UpSkill", 2, 5, 8, 0, 1, color, Camera);
		Attacks.InsertAttacks(GetName(), "Counterattact", 4, 5, 20, 0, color, Camera);
		Attacks.InsertAttacks(GetName(), "Skill1", 2, 5, 20, 0, 5, color, Camera);//多一個參數是具有編號的
	}

	//註冊動作在此
	void Matchstick::OnUpdate(GPH)
	{

		InputJudge(KeyState_now, KeyState_last);
		CheckHit(GPP);
		OnStandby(GPP);
		OnRunning(GPP);
		OnRush(GPP);
		OnJump(GPP);
		OnGuard(GPP);
		OnCharge(GPP);
		OnHit(GPP);
		OnHitGuard(GPP);
		OnNormalAttack1(GPP);
		OnNormalAttack2(GPP);
		OnNormalAttack3(GPP);
		OnSkill1(GPP);
		OnAirAttack1(GPP);
		OnAirAttack2(GPP);
		OnAirDownAttack(GPP);
		OnAirUpAttack(GPP);
		OnDownAttack(GPP);
		OnUpAttack(GPP);
		OnRushAttack(GPP);
		OnRushSkill(GPP);
		OnUpSkill(GPP);
		OnUltimateSkill(GPP);
		//更新所有Effect的動作
		map<string, BitmapAnimation>::iterator Iter_Effect;
		for (Iter_Effect = Effects.Content.begin(); Iter_Effect != Effects.Content.end(); Iter_Effect++)
			Effects.EffectAutoUpdate(&(Iter_Effect->second), (int)(((Iter_Effect->second).PreAutoFrequence)), (Iter_Effect->second).loop, Camera);

		//更新所有Attacks的動作
		map<string, AttackObj>::iterator Iter_Attack;
		for (Iter_Attack = Attacks.AttackObjects.begin(); Iter_Attack != Attacks.AttackObjects.end(); Iter_Attack++)
			Attacks.AttackAutoUpdate(&(Iter_Attack->second), GetName(), (int)(((Iter_Attack->second).PreAutoFrequence)), (Iter_Attack->second).Replay, Camera);


		this->PhysicalMovement(GPP);
		AnimationUpdate(Camera);

		//雜項
		#pragma region 失衡值
		if (BreakPoint > 0 && BreakPoint < 90)
		{
			BreakPoint -= 0.075;
		}
		if (BreakPoint > 90)
		{
			BreakPoint = 90;
			BreakPointTimer = 0;
		}
		if (BreakPoint == 90)
		{
			BreakPoint = 90;
			BreakPointTimer += TIMER_TICK_MILLIDECOND;
			if (BreakPointTimer > 2000)
			{
				BreakPointTimer = 0;
				BreakPoint = 0;
			}
		}
		#pragma endregion

		#pragma region 回復血量
		if (recovery > 0)
		{
			GainHP(0.1);
			recovery -= 0.1;
		}
		#pragma endregion

		#pragma region 完美格檔判定
		if (Button_now.button_Guard == Button_last.button_Guard && Button_now.button_Guard == true)
		{
			BetweenTwiceClickTimer = 0;
			ClickDefendTimer += TIMER_TICK_MILLIDECOND;
		}
		if (Button_now.button_Guard == false)
		{
			BetweenTwiceClickTimer += TIMER_TICK_MILLIDECOND;
		}
		if (BetweenTwiceClickTimer > 100)
		{
			BetweenTwiceClickTimer = 0;
			ClickDefendTimer = 0;
		}
		#pragma endregion

		#pragma region 例外狀況

		if (!(Action == "大絕"))
		{
			Effects.Content["OraOraFire"].visable = false;
		}
		if (!(Action == "衝刺特技"))
		{
			Attacks.AttackObjects["RushSkill"].visable = false;
		}
		#pragma endregion



	}


	void Matchstick::GotoRush(GPH)
	{
		if (SP >= Rush_cost)
		{
			GainSP(-Rush_cost);
			Action = "衝刺";
			Step = 0;
			RushTimer = 0;
		}
	}
	void Matchstick::OnRush(GPH)
	{
		if (Action == "衝刺")
		{
			#pragma region 衝刺主程序
			RushTimer += TIMER_TICK_MILLIDECOND;
			if (Step == 1)
				Velocity_Y = 0;
			if (RushTimer < 40 && Step == 0)
				Velocity_X = 0;
			if (RushTimer >= 40 && Step == 0)
			{
				RushTimer = 0;
				Step = 1;
				Effects.BootEffect(&Effects.Content["Airboost"], Camera, Rect.X, Rect.X + 30, Rect.Y - 30, 0, 0, false, IsRight);
				Throughing = true;
				PlaySounds(Sounds.Rush, false);
				if (IsRight)
				{
					Velocity_X = 35;
					Acceleration_X = -3.5;
					Invincible = true;
				}
				else
				{
					Velocity_X = -35;
					Acceleration_X = +3.5;
					Invincible = true;
				}
			}
			else if (RushTimer <= 80 && Step == 1)//煞車
			{
				if (IsRight&&Velocity_X < 0)
					Velocity_X = 0;
				else if (IsRight == false && Velocity_X > 0)
					Velocity_X = 0;

				CanToRushAttack;
				CanToRushSkill;
			}
			#pragma endregion

			//正常結束
			if (RushTimer > 80 && Step == 1)
			{
				RushTimer = 0;
				Throughing = false;
				Invincible = false;
				Velocity_X = 0;
				Acceleration_X = 0;

				#pragma region 回收特效
				Effects.Content["AirBoost"].Step = 0;
				Effects.Content["AirBoost"].visable = false;
				Effects.Content["AirBoost"].OnUpdate("Effects", Camera);
				#pragma endregion

				#pragma region 判斷應該回到哪個動作
				if (OnGround)
					GotoStandby(GPP);
				else
					GotoDrop(GPP);
				#pragma endregion

			}

		}
	}

	void Matchstick::GotoNormalAttack1(GPH)
	{
		if (SP >= Matchstick_NormalAttack1_Cost)
		{
			GainSP(-Matchstick_NormalAttack1_Cost);
			Action = "普攻1";
			Step = 0;
			NormalAttack1Timer = 0;
		}
	}
	void Matchstick::OnNormalAttack1(GPH)
	{
		if (Action == "普攻1")
		{
			NormalAttack1Timer += TIMER_TICK_MILLIDECOND;

			#pragma region 動作主體
			//處理摩擦力
			ProduceFriction(1, 1);
			if (NormalAttack1Timer >= 16 && Step <= 2)
			{
				NormalAttack1Timer = 0;
				Step += 1;

				#pragma region 產生攻擊物件
				//出拳
				if (Step >= 3)
				{
					Velocity_X += Ahead(3.5);
					Attacks.AttackReset_Normal(
						&(Attacks.AttackObjects["Normal1"]), this, Enemy,
						Matchstick_NormalAttack1_Damage,
						2, 2, Rect.X + 72, Rect.X - 2, Rect.Y + 35, 0, 0,
						120, 30, "PunchHit", Sounds.NormalHit, Camera);
				}
				#pragma endregion


			}
			else if (NormalAttack1Timer >= 40 && Step == 3)
			{
				NormalAttack1Timer = 0;
				Step = 4;
			}
			#pragma endregion

			#pragma region 到別的動作
			if (NormalAttack1Timer < 100 && Step >= 4)
			{
				//到別的動作
				CanToNormalAttack2;
				CanToSkill1;
				CanToJump;
				CanToRush;
				CanToUpAttack;
				CanToDownAttack;
				CanToUpSkill;
			}
			else if (NormalAttack1Timer >= 100 && Step >= 4)
			{
				//正常結束
				GotoStandby(GPP);
			}
			#pragma endregion

		}
	}

	void Matchstick::GotoNormalAttack2(GPH)
	{
		if (SP >= Matchstick_NormalAttack2_Cost)
		{
			GainSP(-Matchstick_NormalAttack2_Cost);
			Action = "普攻2";
			Step = 0;
			NormalAttack1Timer = 0;
		}
	}
	void Matchstick::OnNormalAttack2(GPH)
	{
		if (Action == "普攻2")
		{
			NormalAttack1Timer += TIMER_TICK_MILLIDECOND;
			#pragma region 動作主體
			//處理摩擦力
			ProduceFriction(1, 1);
			if (NormalAttack1Timer >= 16 && Step <= 2)
			{
				NormalAttack1Timer = 0;
				Step += 1;
				if (Step >= 3)
				{
					Velocity_X += Ahead(3.5);
					#pragma region 產生攻擊物件
					//出拳
					Attacks.AttackReset_Normal(
						&(Attacks.AttackObjects["Normal1"]), this, Enemy,
						Matchstick_NormalAttack2_Damage,
						3.5, 2, Rect.X + 72, Rect.X - 2, Rect.Y + 35, 0, 0,
						120, 30, "PunchHit", Sounds.NormalHit, Camera);
					#pragma endregion
				}
			}
			else if (NormalAttack1Timer >= 50 && Step == 3)
			{
				NormalAttack1Timer = 0;
				Step = 4;
			}
			#pragma endregion

			#pragma region 到別的動作
			if (NormalAttack1Timer < 100 && Step >= 4)
			{
				//到別的動作
				CanToNormalAttack3;
				CanToSkill1;
				CanToJump;
				CanToRush;
				CanToUpAttack;
				CanToDownAttack;
				CanToUpSkill;

			}
			else if (NormalAttack1Timer >= 100 && Step >= 4)
			{
				//正常結束
				GotoStandby(GPP);
			}
			#pragma endregion

		}
	}

	void Matchstick::GotoNormalAttack3(GPH)
	{
		if (SP >= Matchstick_NormalAttack3_Cost)
		{
			GainSP(-Matchstick_NormalAttack3_Cost);
			Velocity_X += Ahead(5);
			Velocity_Y -= 5;
			Action = "普攻3";
			Step = 0;
			NormalAttack1Timer = 0;
		}
	}
	void Matchstick::OnNormalAttack3(GPH)
	{
		if (Action == "普攻3")
		{
			NormalAttack1Timer += TIMER_TICK_MILLIDECOND;
			#pragma region 動作主體
			ProduceFriction(0.2, 0.25);
			if (NormalAttack1Timer >= 84 && Step <= 2)
			{
				Step += 1;
				if (Step >= 3)
				{
					#pragma region 產生攻擊物件
					//基本設定
					Attacks.AttackReset_Normal(
						&(Attacks.AttackObjects["Normal2"]), this, Enemy,
						Matchstick_NormalAttack3_Damage,
						12, 7.5, Rect.X + 98, Rect.X - 7, Rect.Y + 17, 0, 0,
						200, 30, "PunchHit", Sounds.NormalHit, Camera);
					//額外設定
					Attacks.AttackObjects["Normal2"].CanHitFly = true;

					#pragma endregion
				}
			}
			else if (NormalAttack1Timer >= 150 && Step == 3)
			{
				NormalAttack1Timer = 0;
				Step = 4;
			}
			#pragma endregion

			#pragma region 到別的動作
			if (NormalAttack1Timer < 100 && Step >= 4)
			{
				CanToJump;
				CanToRush;
			}
			else if (NormalAttack1Timer >= 100 && Step >= 4)
			{
				//正常結束
				GotoStandby(GPP);
			}
			#pragma endregion

		}
	}

	void Matchstick::GotoSkill1(GPH)
	{
		if (SP >= Matchstick_Skill1_Cost)
		{
			GainSP(-Matchstick_Skill1_Cost);
			if (Velocity_Y > 0 && OnGround == false)
				Velocity_Y = 0;
			Action = "特技1";
			Step = 0;
			Shot1Timer = 0;
		}
	}
	void Matchstick::OnSkill1(GPH)
	{
		if (Action == "特技1")
		{

			Shot1Timer += TIMER_TICK_MILLIDECOND;

			#pragma region 動作主體
			if (OnGround)
				ProduceFriction(1, 1);
			else
				ProduceFriction(0.15, 0.75);
			if (Shot1Timer >= 50 && Step == 0)
			{
				Step = 1;
				if (Velocity_Y > 0 && OnGround == false)
					Velocity_Y = 0;
			}
			if (Shot1Timer >= 16 && Step >= 1 && Step <= 3)
			{
				Shot1Timer = 0;
				Step += 1;
				if (Step == 3)
				{
					#pragma region 產生攻擊物件
					//出拳
					Attacks.AttackReset_Shot(&(Attacks.AttackObjects["Skill1_" + IntToString(Shot1Current)]), this, Enemy,
						Matchstick_Skill1_Damage,
						2, 3.5,
						Rect.X + 50, Rect.X + 10, Rect.Y + 35, Ahead(4.5), 0,
						120, 1000, 2,
						true, true, true,
						"PunchHit", Sounds.NormalHit, Camera);
					Shot1Current += 1;
					if (Shot1Current >= 5)
						Shot1Current = 0;

					#pragma endregion
				}
			}
			else if (Shot1Timer >= 50 && Step == 4)
			{
				Shot1Timer = 0;
				Step = 5;
			}
			#pragma endregion

			#pragma region 到別的動作
			if (Shot1Timer < 100 && Step >= 5)
			{
				//到別的可能動作
				if (OnGround)
				{
					CanToNormalAttack1;
					CanToUpAttack;
					CanToDownAttack;
					CanToUpSkill;
				}
				else
				{
					CanToAirUpAttack;
					CanToAirDownAttack;
					CanToAirAttack1;
					CanToFastDrop;
				}

				CanToRush;
				CanToJump;
			}
			else if (Shot1Timer >= 100 && Step >= 5)
			{
				//正常結束
				if (OnGround)
					GotoStandby(GPP);
				else
					GotoDrop(GPP);
			}
			#pragma endregion

		}
	}

	void Matchstick::GotoAirAttack1(GPH)
	{
		if (SP >= Matchstick_AirAttack1_Cost)
		{
			GainSP(-Matchstick_AirAttack1_Cost);
			Action = "空普1";
			Step = 0;
			if (Velocity_Y > 0)
			{
				Velocity_Y = 0;
			}
			NormalAttack1Timer = 0;
		}
	}
	void Matchstick::OnAirAttack1(GPH)
	{
		if (Action == "空普1")
		{

			NormalAttack1Timer += TIMER_TICK_MILLIDECOND;

			#pragma region 左右移動
			if (CanControl&&Button_now.button_Right == false && CanControl&&Button_now.button_Left == false)
			{
				ProduceFriction(0.15, 1);
			}
			else if (CanControl&&Button_now.button_Right == true)
			{
				IsRight = true;
				RunAhead(0.5, RunSpeed / 2);
			}
			else if (CanControl&&Button_now.button_Left == true)
			{
				IsRight = false;
				RunAhead(0.5, RunSpeed / 2);
			}
			#pragma endregion

			#pragma region 動作主體
			if (NormalAttack1Timer >= 50 && Step == 0)
			{
				Step = 1;
			}
			if (NormalAttack1Timer >= 16 && Step >= 1 && Step <= 3)
			{
				NormalAttack1Timer = 0;
				Step += 1;
				#pragma region 產生攻擊物件
				if (Step == 3)
				{
					//出拳
					Attacks.AttackReset_Normal(
						&(Attacks.AttackObjects["Normal3"]), this, Enemy,
						Matchstick_AirAttack1_Damage,
						2, 5, Rect.X + 72, Rect.X - 2, Rect.Y + 60, Velocity_X / 3, 0,
						150, 30, "PunchHit", Sounds.NormalHit, Camera);
				}
				#pragma endregion
			}
			else if (NormalAttack1Timer >= 125 && Step == 3)
			{
				NormalAttack1Timer = 0;
				Step = 4;
			}
			#pragma endregion

			#pragma region 到別的動作
			if (NormalAttack1Timer < 125 && Step >= 4)
			{
				//到別的可能動作
				CanToAirAttack2;
				CanToSkill1;
				CanToRush;
				CanToAirDownAttack;
				CanToAirUpAttack;
				CanToJump;
				CanToFastDrop;
			}
			else if (NormalAttack1Timer >= 125 && Step >= 4)
			{
				//正常結束
				if (OnGround)
					GotoStandby(GPP);
				else
					GotoDrop(GPP);
			}
			#pragma endregion

		}
	}

	void Matchstick::GotoAirAttack2(GPH)
	{
		if (SP >= Matchstick_AirAttack2_Cost)
		{
			GainSP(-Matchstick_AirAttack2_Cost);
			Action = "空普2";
			Step = 0;
			Velocity_X += Ahead(4);
			Velocity_Y = -3.5;
			NormalAttack1Timer = 0;
		}
	}
	void Matchstick::OnAirAttack2(GPH)
	{
		if (Action == "空普2")
		{
			ProduceFriction(0.1, 0.15);
			NormalAttack1Timer += TIMER_TICK_MILLIDECOND;
			if (NormalAttack1Timer >= 84 && Step <= 2)
			{
				Step += 1;
				if (Step >= 3)
				{
					//基礎設定
					Attacks.AttackReset_Normal(
						&(Attacks.AttackObjects["Normal2"]), this, Enemy,
						Matchstick_AirAttack2_Damage,
						12, 7.5, Rect.X + 95, Rect.X, Rect.Y + 40, Velocity_X / 3, 0,
						250, 50, "PunchHit", Sounds.NormalHit, Camera);
					//額外設定
					Attacks.AttackObjects["Normal2"].CanHitFly = true;
				}
			}
			else if (NormalAttack1Timer >= 150 && Step == 3)
			{
				NormalAttack1Timer = 0;
				Step = 4;
			}
			else if (NormalAttack1Timer < 100 && Step >= 4)
			{
				//到別的動作
			}
			else if (NormalAttack1Timer >= 100 && Step >= 4)
			{
				//正常結束
				if (OnGround)
					GotoStandby(GPP);
				else
					GotoDrop(GPP);
			}

		}
	}

	void Matchstick::GotoUpAttack(GPH)
	{
		if (SP >= Matchstick_UpAttack_Cost)
		{
			GainSP(-Matchstick_UpAttack_Cost);
			Action = "上普";
			Step = 0;
			NormalAttack1Timer = 0;
		}
	}
	void Matchstick::OnUpAttack(GPH)
	{
		if (Action == "上普")
		{
			NormalAttack1Timer += TIMER_TICK_MILLIDECOND;

			#pragma region 動作主體
			//處理摩擦力
			ProduceFriction(1, 1);
			if (NormalAttack1Timer >= 80 && Step == 0)
			{
				NormalAttack1Timer = 0;
				Step = 1;
			}
			else if (NormalAttack1Timer >= 16 && Step >= 1 && Step <= 2)
			{
				NormalAttack1Timer = 0;
				Step += 1;
				#pragma region 產生攻擊物件
				//出拳
				if (Step == 3)
				{
					Velocity_X += Ahead(7);
					//基礎設定
					Attacks.AttackReset_Normal(
						&(Attacks.AttackObjects["Normal2"]), this, Enemy,
						Matchstick_UpAttack_Damage,
						2, 12, Rect.X + 98, Rect.X - 7, Rect.Y + 17, 0, 0,
						220, 30, "PunchHit", Sounds.NormalHit, Camera);
				}
				#pragma endregion
			}
			else if (NormalAttack1Timer >= 40 && Step == 3)
			{
				NormalAttack1Timer = 0;
				Step = 4;
			}
			#pragma endregion

			#pragma region 到別的動作
			if (NormalAttack1Timer < 100 && Step >= 4)
			{
				//到別的動作
				CanToNormalAttack1;
				CanToSkill1;
				CanToJump;
				CanToRush;
				CanToDownAttack;
				CanToUpSkill;
			}
			else if (NormalAttack1Timer >= 100 && Step >= 4)
			{
				//正常結束
				GotoStandby(GPP);
			}
			#pragma endregion

		}
	}

	void Matchstick::GotoDownAttack(GPH)
	{
		if (SP >= Matchstick_DownAttack_Cost)
		{
			GainSP(-Matchstick_DownAttack_Cost);
			Action = "下普";
			Step = 0;
			NormalAttack1Timer = 0;
		}
	}
	void Matchstick::OnDownAttack(GPH)
	{
		if (Action == "下普")
		{
			NormalAttack1Timer += TIMER_TICK_MILLIDECOND;

			#pragma region 動作主體
			//處理摩擦力
			ProduceFriction(1, 1);
			if (NormalAttack1Timer >= 150 && Step == 0)
			{
				NormalAttack1Timer = 0;
				Step = 1;
			}
			else if (NormalAttack1Timer >= 16 && Step >= 1 && Step <= 3)
			{
				NormalAttack1Timer = 0;
				Step += 1;
				#pragma region 產生攻擊物件
				//出拳
				if (Step >= 4)
				{
					Velocity_X += Ahead(3.5);
					Attacks.AttackReset_Normal(
						&(Attacks.AttackObjects["Normal1"]), this, Enemy,
						Matchstick_DownAttack_Damage,
						2, 3, Rect.X + 74, Rect.X, Rect.Y + 35, 0, 0,
						200, 30, "PunchHit", Sounds.NormalHit, Camera);
					Attacks.AttackObjects["Normal1"].HitBreak = true;
				}
				#pragma endregion
			}
			else if (NormalAttack1Timer >= 40 && Step == 4)
			{
				NormalAttack1Timer = 0;
				Step = 5;
			}
			#pragma endregion

			#pragma region 到別的動作
			if (NormalAttack1Timer < 100 && Step >= 5)
			{
				//到別的動作
				CanToNormalAttack1;
				CanToSkill1;
				CanToJump;
				CanToRush;
				CanToUpSkill;
			}
			else if (NormalAttack1Timer >= 100 && Step >= 5)
			{
				//正常結束
				GotoStandby(GPP);
			}
			#pragma endregion

		}
	}

	void Matchstick::GotoRushAttack(GPH)
	{
		if (SP >= Matchstick_RushAttack_Cost)
		{
			GainSP(-Matchstick_RushAttack_Cost);
			if (Velocity_Y > 0 && OnGround == false)
				Velocity_Y = 0;
			RushTimer = 0;
			IsRushAttack = false;
			Throughing = false;
			Invincible = false;
			Velocity_X = 0;
			Acceleration_X = 0;
			Action = "衝刺普";
			Step = 0;
			NormalAttack1Timer = 0;
		}
	}
	void Matchstick::OnRushAttack(GPH)
	{
		if (Action == "衝刺普")
		{

			NormalAttack1Timer += TIMER_TICK_MILLIDECOND;

			#pragma region 動作主體
			if (NormalAttack1Timer <= 140 && Step == 0)
			{
				Velocity_Y = 0;
				Acceleration_X = Ahead(1.2);
				if (abs(Velocity_X) > 12)
				{
					Velocity_X = Ahead(12);
				}
				#pragma region 更新攻擊物件位置
				if (IsRight)
					Attacks.AttackObjects["Normal6"].Rect.X = Rect.X + 60;
				else
					Attacks.AttackObjects["Normal6"].Rect.X = Rect.X - 10;
				Attacks.AttackObjects["Normal6"].Rect.Y = Rect.Y + 35;
				if (Attacks.AttackObjects["Normal6"].IsHited&&Attacks.AttackObjects["Normal6"].ComboTimer > TIMER_TICK_MILLIDECOND * 2)
				{
					Attacks.AttackObjects["Normal6"].ComboTimer = 0;
					Attacks.AttackObjects["Normal6"].IsHited = false;
				}
				#pragma endregion

				#pragma region 產生攻擊物件
				if (IsRushAttack == false)
				{
					IsRushAttack = true;
					//基礎設定
					Attacks.AttackReset_Normal(
						&(Attacks.AttackObjects["Normal6"]), this, Enemy,
						Matchstick_RushAttack_Damage,
						8.5, 1, Rect.X + 98, 0, 0, 0, 0,
						50, 100, "PunchHit", Sounds.NormalHit, Camera);
					//額外設定
					Attacks.AttackObjects["Normal6"].CanCombo = true;//可連擊
					Attacks.AttackObjects["Normal6"].HitNoon = false;
				}
				#pragma endregion
			}
			#pragma endregion

			#pragma region 到別的動作
			if (NormalAttack1Timer > 140 && Step == 0)
			{
				Acceleration_X = 0;
				Velocity_X = 0;
				#pragma region 判斷應該回到哪個動作
				if (OnGround)
					GotoStandby(GPP);
				else
					GotoDrop(GPP);
				#pragma endregion
			}
			#pragma endregion

		}
	}

	void Matchstick::GotoAirUpAttack(GPH)
	{
		if (SP >= Matchstick_AirUpAttack_Cost)
		{
			GainSP(-Matchstick_AirUpAttack_Cost);
			Action = "空上普";
			Step = 0;
			if (Velocity_Y > 0)
				Velocity_Y = 0;
			NormalAttack1Timer = 0;
		}
	}
	void Matchstick::OnAirUpAttack(GPH)
	{
		if (Action == "空上普")
		{
			#pragma region 左右移動
			if (CanControl&&Button_now.button_Right == false && CanControl&&Button_now.button_Left == false)
			{
				ProduceFriction(0.15, 1);
			}
			else if (CanControl&&Button_now.button_Right == true)
			{
				IsRight = true;
				RunAhead(0.5, RunSpeed / 2);
			}
			else if (CanControl&&Button_now.button_Left == true)
			{
				IsRight = false;
				RunAhead(0.5, RunSpeed / 2);
			}
			#pragma endregion

			#pragma region 動作主體
			NormalAttack1Timer += TIMER_TICK_MILLIDECOND;
			if (NormalAttack1Timer >= 100 && Step == 0)
			{
				Step = 1;
				if (Velocity_Y > 1)
					Velocity_Y = 1;
			}
			if (NormalAttack1Timer >= 16 && Step >= 1 && Step <= 3)
			{
				NormalAttack1Timer = 0;
				Step += 1;
				Velocity_Y = -6;

				#pragma region 產生攻擊物件
				if (Step == 2)
				{
					//基礎設定
					Attacks.AttackReset_Normal(
						&(Attacks.AttackObjects["Normal5"]), this, Enemy,
						Matchstick_AirUpAttack_Damage,
						2.5, 12, Rect.X + 20, Rect.X, Rect.Y, Velocity_X / 4, 0,
						250, 30, "PunchHit", Sounds.NormalHit, Camera);
				}
				#pragma endregion

			}
			else if (NormalAttack1Timer >= 16 && Step >= 4 && Step <= 6)
			{
				if (Velocity_Y > 2)
					Velocity_Y = 2;
				NormalAttack1Timer = 0;
				Step += 1;
			}
			#pragma endregion

			#pragma region 到別的動作
			if (NormalAttack1Timer < 100 && Step >= 7)
			{
				if (Velocity_Y > 1)
					Velocity_Y = 1;
				//到別的可能動作
				CanToSkill1;
				CanToRush;
				CanToJump;
				CanToAirAttack1;
				CanToAirDownAttack;
				CanToFastDrop;
			}
			else if (NormalAttack1Timer >= 100 && Step >= 7)
			{
				//正常結束
				if (OnGround)
					GotoStandby(GPP);
				else
					GotoDrop(GPP);
			}
			#pragma endregion
		}
	}

	void Matchstick::GotoAirDownAttack(GPH)
	{
		if (SP >= Matchstick_AirDownAttack_Cost)
		{
			GainSP(-Matchstick_AirDownAttack_Cost);
			Action = "空下普";
			Step = 0;
			if (Velocity_Y > 0)
				Velocity_Y = 0;
			NormalAttack1Timer = 0;
		}
	}
	void Matchstick::OnAirDownAttack(GPH)
	{
		if (Action == "空下普")
		{
			NormalAttack1Timer += TIMER_TICK_MILLIDECOND;

			#pragma region 左右移動
			if (CanControl&&Button_now.button_Right == false && CanControl&&Button_now.button_Left == false)
			{
				ProduceFriction(0.15, 1);
			}
			else if (CanControl&&Button_now.button_Right == true)
			{
				IsRight = true;
				RunAhead(0.5, RunSpeed / 2);
			}
			else if (CanControl&&Button_now.button_Left == true)
			{
				IsRight = false;
				RunAhead(0.5, RunSpeed / 2);
			}
			#pragma endregion

			#pragma region 動作主體
			if (NormalAttack1Timer >= 75 && Step == 0)
			{
				if (Velocity_Y > 1)
					Velocity_Y = 1;
				Step = 1;
			}
			if (NormalAttack1Timer >= 8 && Step >= 1 && Step <= 3)
			{
				NormalAttack1Timer = 0;
				Step += 1;
			}
			else if (NormalAttack1Timer >= 20 && Step >= 4 && Step <= 6)
			{
				NormalAttack1Timer = 0;
				Step += 1;
				#pragma region 產生攻擊物件
				if (Step == 5)
				{
					//基礎設定
					Attacks.AttackReset_Normal(
						&(Attacks.AttackObjects["Normal4"]), this, Enemy,
						Matchstick_AirDownAttack_Damage,
						2.5, -16, Rect.X + 60, Rect.X, Rect.Y + 10, Velocity_X / 2, Velocity_Y / 3,
						200, 30, "PunchHit", Sounds.NormalHit, Camera);
				}
				#pragma endregion
			}
			#pragma endregion

			#pragma region 到別的動作
			if (NormalAttack1Timer > 50 && NormalAttack1Timer < 120 && Step >= 7)
			{
				if (Velocity_Y > 1)
					Velocity_Y = 1;
				//到別的可能動作
				CanToSkill1;
				CanToRush;
				CanToJump;
				CanToFastDrop;
			}
			else if (NormalAttack1Timer >= 120 && Step >= 7)
			{
				//正常結束
				if (OnGround)
					GotoStandby(GPP);
				else
					GotoDrop(GPP);
			}
			#pragma endregion
		}
	}

	void Matchstick::GotoUpSkill(GPH)
	{
		if (SP >= Matchstick_UpSkill_Cost)
		{
			GainSP(-Matchstick_UpSkill_Cost);
			Action = "上特技";
			Step = 0;
			NormalAttack1Timer = 0;
		}
	}
	void Matchstick::OnUpSkill(GPH)
	{
		if (Action == "上特技")
		{
			NormalAttack1Timer += TIMER_TICK_MILLIDECOND;

			#pragma region 動作主體
			//處理摩擦力
			ProduceFriction(1, 1);
			if (NormalAttack1Timer >= 150 && Step == 0)
			{
				NormalAttack1Timer = 0;
				Step = 1;
			}
			else if (NormalAttack1Timer >= 16 && Step >= 1 && Step <= 2)
			{
				NormalAttack1Timer = 0;
				Step += 1;

				#pragma region 產生攻擊物件
				//出拳
				if (Step == 3)
				{
					PlaySounds(Sounds.Fire1, false);
					//基礎設定
					Attacks.AttackReset_Shot(&(Attacks.AttackObjects["UpSkill_" + IntToString(UpSkillCurrent)]), this, Enemy,
						Matchstick_UpSkill_Damage,
						1, 13,
						Rect.X, Rect.X, Rect.Y + 17, 0, -9,
						50, 400, 5,
						true, true, true,
						"PunchHit", Sounds.NormalHit, Camera);
					//額外設定
					Attacks.AttackObjects["UpSkill_" + IntToString(UpSkillCurrent)].Drawable = true;
					Attacks.AttackObjects["UpSkill_" + IntToString(UpSkillCurrent)].HitBreak = false;
					Attacks.AttackObjects["UpSkill_" + IntToString(UpSkillCurrent)].HitNoon = false;
					Attacks.AttackObjects["UpSkill_" + IntToString(UpSkillCurrent)].CanCombo = true;
					UpSkillCurrent = 0;
				}
				#pragma endregion
			}
			#pragma endregion

			#pragma region 到別的動作
			if (NormalAttack1Timer >= 40 && NormalAttack1Timer < 100 && Step >= 3)
			{
				//到別的動作
				CanToNormalAttack1;
				CanToSkill1;
				CanToJump;
				CanToRush;
				CanToDownAttack;
			}
			else if (NormalAttack1Timer >= 100 && Step >= 3)
			{
				//正常結束
				GotoStandby(GPP);
			}
			#pragma endregion

		}
	}

	void Matchstick::GotoDownSkill(GPH)
	{
	}
	void Matchstick::OnDownSkill(GPH)
	{
	}

	void Matchstick::GotoRushSkill(GPH)
	{
		if (SP >= Matchstick_RushSkill_Cost)
		{
			GainSP(-Matchstick_RushSkill_Cost);
			if (Velocity_Y > 0 && OnGround == false)
				Velocity_Y = 0;
			RushTimer = 0;
			IsRushAttack = false;
			Throughing = true;
			Invincible = false;
			Velocity_X = 0;
			Acceleration_X = 0;
			Action = "衝刺特技";
			Step = 0;
			NormalAttack1Timer = 0;
		}
	}
	void Matchstick::OnRushSkill(GPH)
	{
		if (Action == "衝刺特技")
		{

			NormalAttack1Timer += TIMER_TICK_MILLIDECOND;

			#pragma region 動作主體
			if (NormalAttack1Timer <= 150 && Step == 0)
			{
				Velocity_Y = -4;
				Velocity_X = Ahead(2.5);
			}
			else if (NormalAttack1Timer > 150 && Step == 0)
			{
				Step = 1;
				NormalAttack1Timer = 0;
			}
			if (NormalAttack1Timer <= 200 && Step == 1)
			{
				Velocity_Y = 0;
				Acceleration_X = Ahead(4);
				if (abs(Velocity_X) > 15)
				{
					Velocity_X = Ahead(15);
				}
				#pragma region 更新攻擊物件位置
				if (IsRight)
					Attacks.AttackObjects["RushSkill"].Rect.X = Rect.X - 47;
				else
					Attacks.AttackObjects["RushSkill"].Rect.X = Rect.X - 15;
				Attacks.AttackObjects["RushSkill"].Rect.Y = Rect.Y + 0;

				if (Attacks.AttackObjects["RushSkill"].IsHited&&Attacks.AttackObjects["RushSkill"].ComboTimer > TIMER_TICK_MILLIDECOND * 4)
				{
					Attacks.AttackObjects["RushSkill"].ComboTimer = 0;
					Attacks.AttackObjects["RushSkill"].IsHited = false;
				}

				#pragma endregion

				#pragma region 產生攻擊物件
				if (IsRushAttack == false)
				{
					IsRushAttack = true;
					PlaySounds(Sounds.Fire1, false);
					//基礎設定
					Attacks.AttackReset_Shot(&(Attacks.AttackObjects["RushSkill"]), this, Enemy,
						Matchstick_RushSkill_Damage,
						12, 5,
						Rect.X - 47, Rect.X - 15, Rect.Y, 0, 0,
						250, 200, 5,
						true, true, true,
						"PunchHit", Sounds.NormalHit, Camera);

					//額外設定
					Attacks.AttackObjects["RushSkill"].Drawable = true;
					Attacks.AttackObjects["RushSkill"].HitBreak = false;
					Attacks.AttackObjects["RushSkill"].HitNoon = false;
					Attacks.AttackObjects["RushSkill"].CanCombo = true;
				}
				#pragma endregion
			}

			#pragma endregion

			#pragma region 到別的動作
			if ((NormalAttack1Timer > 200) && Step == 1)
			{
				Throughing = false;
				Acceleration_X = 0;
				Velocity_X /= 2;
				#pragma region 判斷應該回到哪個動作
				if (OnGround)
					GotoStandby(GPP);
				else
					GotoDrop(GPP);
				#pragma endregion
			}
			#pragma endregion

		}
	}

	void Matchstick::GotoAirUpSkill(GPH)
	{
	}
	void Matchstick::OnAirUpSkill(GPH)
	{
	}

	void Matchstick::GotoAirDownSkill(GPH)
	{
	}
	void Matchstick::OnAirDownSkill(GPH)
	{
	}

	void Matchstick::GotoUltimateSkill(GPH)
	{
		if (SP > (SP_Max / 3))
		{
			UltraSkillcostSP = SP - (SP_Max / 3);
			SP = 0;
			Action = "大絕";
			Step = 0;
			NormalAttack1Timer = 0;
			UltraSkillTimer1 = 0;
		}
	}
	void Matchstick::OnUltimateSkill(GPH)
	{
		if (Action == "大絕")
		{

			NormalAttack1Timer += TIMER_TICK_MILLIDECOND;
			UltraSkillTimer1 += TIMER_TICK_MILLIDECOND;
			#pragma region 動作主體
			//處理摩擦力
			ProduceFriction(1, 1);
			if (NormalAttack1Timer >= 300 && Step == 0)
			{
				PlaySounds(Sounds.CutIn, false);
				NeedCutIn = true;
				NormalAttack1Timer = 0;
				Step = 1;
				Effects.BootEffect(&Effects.Content["Matchstick_US"], Camera, 800, 800, 0, 0, 0, false, true);
				Effects.BootEffect(&Effects.Content["OraOraFire"], Camera, Rect.X + 60, Rect.X - 60, Rect.Y, 0, 0, false, IsRight);
				Effects.Content["OraOraFire"].loop = true;
			}
			if (Step >= 1 && Step <= 5 && NormalAttack1Timer >= 40)
			{
				if (IsRight)
					Effects.Content["OraOraFire"].Rect.X = Rect.X + 60;
				else
					Effects.Content["OraOraFire"].Rect.X = Rect.X - 60;
				Effects.Content["OraOraFire"].Rect.Y = Rect.Y;
				Effects.Content["OraOraFire"].visable = true;
				NormalAttack1Timer = 0;
				Step += 1;
				if (Step > 5)
				{
					Step = 1;
				}
				#pragma region 產生攻擊物件
				Attacks.AttackReset_Normal(
					&(Attacks.AttackObjects["Normal7"]), this, Enemy,
					Matchstick_UltimateSkill_Damage1*(1 + (UltraSkillcostSP / SP_Max)),
					0, 0.75, Rect.X + 60, Rect.X - 60, Rect.Y, 0, 0,
					100, 30, "PunchHit", Sounds.NormalHit2, Camera);
				#pragma endregion
			}
			if (UltraSkillTimer1 >= 1200 && Step < 6)
			{
				Step = 6;
				Attacks.AttackReset_Normal(
					&(Attacks.AttackObjects["Normal7"]), this, Enemy,
					Matchstick_UltimateSkill_Damage2*(1 + (UltraSkillcostSP / SP_Max)),
					15, 10, Rect.X + 60, Rect.X - 60, Rect.Y, 0, 0,
					200, 30, "PunchHit", Sounds.Stoned, Camera);
				Attacks.AttackObjects["Normal7"].CanHitFly = true;
				Attacks.AttackObjects["Normal7"].HitBreak = false;
				Effects.Content["OraOraFire"].visable = false;
				Effects.Content["OraOraFire"].loop = false;
			}

			if (Step == 6 && UltraSkillTimer1 > 1700)
			{
				GotoStandby(GPP);
			}

			#pragma endregion


		}
	}



}
