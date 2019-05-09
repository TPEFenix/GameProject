#pragma once
#include "Keycode.h"
#include "KeyBoardState.h"
#include "CollisionSensor.h"
#include "TypeConverter.h"
#include "WKAudio.h"
#include "AttackObj.h"
#include "EffectSprite.h"


using namespace std;
using namespace WKAudio_namespace;
using namespace CollisionSensor_namespace;
using namespace TypeConverter_namespace;


namespace game_framework
{
    #define CanToNormalAttack2  if (CanControl&&Button_now.button_Attack&&Button_now.button_Up == false && Button_now.button_Down == false&&Button_last.button_Attack == false && OnGround){GotoNormalAttack2(GPP);}
    #define CanToNormalAttack3  if (CanControl&&Button_now.button_Attack&&Button_now.button_Up == false && Button_now.button_Down == false&&Button_last.button_Attack == false && OnGround){GotoNormalAttack3(GPP);}

    #pragma region 火柴人
    class Matchstick :public  BattlePlayer
    {
        #define Matchstick_HP_Max 1000
        #define Matchstick_SP_Max 100
        #define Matchstick_Rush_Cost 8
        #define Matchstick_RunSpeed 7.5
        #define Matchstick_StandbySPincrements 0.3
        #define Matchstick_RunningSPincrements 0.15
        #define Matchstick_ChargeSPincrements 13

        #define Matchstick_NormalAttack1_Cost 2.5
        #define Matchstick_NormalAttack1_Damage 30
        #define Matchstick_NormalAttack2_Cost 2.5
        #define Matchstick_NormalAttack2_Damage 30
        #define Matchstick_NormalAttack3_Cost 5
        #define Matchstick_NormalAttack3_Damage 70
        #define Matchstick_AirAttack1_Cost 2.5
        #define Matchstick_AirAttack1_Damage 30
        #define Matchstick_AirAttack2_Cost 5
        #define Matchstick_AirAttack2_Damage 70
        #define Matchstick_UpAttack_Cost 4
        #define Matchstick_UpAttack_Damage 50
        #define Matchstick_UpSkill_Cost 8
        #define Matchstick_UpSkill_Damage 15
        #define Matchstick_DownAttack_Cost 8
        #define Matchstick_DownAttack_Damage 80
        #define Matchstick_RushAttack_Cost 4
        #define Matchstick_RushAttack_Damage 12
        #define Matchstick_RushSkill_Cost 8
        #define Matchstick_RushSkill_Damage 20
        #define Matchstick_AirUpAttack_Cost 4
        #define Matchstick_AirUpAttack_Damage 50
        #define Matchstick_AirDownAttack_Cost 4
        #define Matchstick_AirDownAttack_Damage 50
        #define Matchstick_Skill1_Cost 4
        #define Matchstick_Skill1_Damage 25
        #define Matchstick_UltimateSkill_Cost 0
        #define Matchstick_UltimateSkill_Damage1 5
        #define Matchstick_UltimateSkill_Damage2 200

    public:
        Matchstick(int);
        ~Matchstick();
        virtual void AutoLoadBitmaps(GPH);//依照各自角色讀檔
        virtual void AutoLoadAttacks(GPH);
        virtual void OnUpdate(GPH);//更新函式，且隨著視角移動

                                   //衝刺覆寫
        virtual void GotoRush(GPH);
        virtual void OnRush(GPH);


        virtual void GotoNormalAttack1(GPH);
        virtual void OnNormalAttack1(GPH);
        virtual void GotoNormalAttack2(GPH);
        virtual void OnNormalAttack2(GPH);
        virtual void GotoNormalAttack3(GPH);
        virtual void OnNormalAttack3(GPH);
        virtual void GotoSkill1(GPH);
        virtual void OnSkill1(GPH);
        virtual void GotoAirAttack1(GPH);
        virtual void OnAirAttack1(GPH);
        virtual void GotoAirAttack2(GPH);
        virtual void OnAirAttack2(GPH);
        virtual void GotoUpAttack(GPH);
        virtual void OnUpAttack(GPH);
        virtual void GotoDownAttack(GPH);
        virtual void OnDownAttack(GPH);
        virtual void GotoRushAttack(GPH);
        virtual void OnRushAttack(GPH);
        virtual void GotoAirUpAttack(GPH);
        virtual void OnAirUpAttack(GPH);
        virtual void GotoAirDownAttack(GPH);
        virtual void OnAirDownAttack(GPH);
        virtual void GotoUpSkill(GPH);
        virtual void OnUpSkill(GPH);
        virtual void GotoDownSkill(GPH);
        virtual void OnDownSkill(GPH);
        virtual void GotoRushSkill(GPH);
        virtual void OnRushSkill(GPH);
        virtual void GotoAirUpSkill(GPH);
        virtual void OnAirUpSkill(GPH);
        virtual void GotoAirDownSkill(GPH);
        virtual void OnAirDownSkill(GPH);
        virtual void GotoUltimateSkill(GPH);
        virtual void OnUltimateSkill(GPH);


        //Timer及次數控制參數------------------------------------------------------------------------------------------------------
        double NormalAttack1Timer = 0;
        double UltraSkillTimer1 = 0;
        double Shot1Timer = 0;
        int Shot1Current = 0;
        int UpSkillCurrent = 0;
        bool IsRushAttack = false;
        double UltraSkillcostSP = 0;

    };
    #pragma endregion


    #pragma region Rina
    class Rina :public  BattlePlayer
    {
        #define Rina_HP_Max 1000
        #define Rina_SP_Max 100
        #define Rina_Rush_Cost 8
        #define Rina_RunSpeed 8
        #define Rina_StandbySPincrements 0.3
        #define Rina_RunningSPincrements 0.15
        #define Rina_ChargeSPincrements 13

        #define Rina_NormalAttack1_Cost 2.5
        #define Rina_NormalAttack1_Damage 30
        #define Rina_NormalAttack2_Cost 2.5
        #define Rina_NormalAttack2_Damage 30
        #define Rina_NormalAttack3_Cost 5
        #define Rina_NormalAttack3_Damage 70
        #define Rina_AirAttack1_Cost 2.5
        #define Rina_AirAttack1_Damage 30
        #define Rina_AirAttack2_Cost 5
        #define Rina_AirAttack2_Damage 70
        #define Rina_UpAttack_Cost 4
        #define Rina_UpAttack_Damage 50
        #define Rina_UpSkill_Cost 8
        #define Rina_UpSkill_Damage 15
        #define Rina_DownSkill_Cost 15
        #define Rina_DownSkill_Damage 20
        #define Rina_DownAttack_Cost 8
        #define Rina_DownAttack_Damage 80
        #define Rina_RushAttack_Cost 5
        #define Rina_RushAttack_Damage 20
        #define Rina_RushSkill_Cost 8
        #define Rina_RushSkill_Damage 20
        #define Rina_AirUpAttack_Cost 4
        #define Rina_AirUpAttack_Damage 50
        #define Rina_AirDownAttack_Cost 4
        #define Rina_AirDownAttack_Damage 50
        #define Rina_Skill1_Cost 4
        #define Rina_Skill1_Damage 25
        #define Rina_UltimateSkill_Cost 0
        #define Rina_UltimateSkill_Damage1 5
        #define Rina_UltimateSkill_Damage2 200

    public:
        Rina(int);
        ~Rina();
        virtual void AutoLoadBitmaps(GPH);//依照各自角色讀檔
        virtual void AutoLoadAttacks(GPH);
        virtual void OnUpdate(GPH);//更新函式，且隨著視角移動

        //衝刺覆寫
        virtual void GotoRush(GPH);
        virtual void OnRush(GPH);
        //跑步覆寫
        virtual void GotoRunning(GPH);
        virtual void OnRunning(GPH);
        //跳躍覆寫
        virtual void GotoJump(GPH);
        virtual void OnJump(GPH);
        //練氣覆寫
        virtual void GotoCharge(GPH);
        virtual void OnCharge(GPH);


        virtual void GotoNormalAttack1(GPH);
        virtual void OnNormalAttack1(GPH);
        virtual void GotoNormalAttack2(GPH);
        virtual void OnNormalAttack2(GPH);
        virtual void GotoNormalAttack3(GPH);
        virtual void OnNormalAttack3(GPH);
        virtual void GotoSkill1(GPH);
        virtual void OnSkill1(GPH);
        virtual void GotoAirAttack1(GPH);
        virtual void OnAirAttack1(GPH);
        virtual void GotoAirAttack2(GPH);
        virtual void OnAirAttack2(GPH);
        virtual void GotoUpAttack(GPH);
        virtual void OnUpAttack(GPH);
        virtual void GotoDownAttack(GPH);
        virtual void OnDownAttack(GPH);
        virtual void GotoRushAttack(GPH);
        virtual void OnRushAttack(GPH);
        virtual void GotoAirUpAttack(GPH);
        virtual void OnAirUpAttack(GPH);
        virtual void GotoAirDownAttack(GPH);
        virtual void OnAirDownAttack(GPH);
        virtual void GotoUpSkill(GPH);
        virtual void OnUpSkill(GPH);
        virtual void GotoDownSkill(GPH);
        virtual void OnDownSkill(GPH);
        virtual void GotoRushSkill(GPH);
        virtual void OnRushSkill(GPH);
        virtual void GotoAirUpSkill(GPH);
        virtual void OnAirUpSkill(GPH);
        virtual void GotoAirDownSkill(GPH);
        virtual void OnAirDownSkill(GPH);
        virtual void GotoUltimateSkill(GPH);
        virtual void OnUltimateSkill(GPH);


        //Timer及次數控制參數------------------------------------------------------------------------------------------------------
        double NormalAttack1Timer = 0;
        double JumpTimer2 = 0;

        double UltraSkillTimer1 = 0;
        double Shot1Timer = 0;
        int Shot1Current = 0;
        int UpSkillCurrent = 0;

        bool IsRushAttack = false;
        double UltraSkillcostSP = 0;

        double DownSkillTimer = 0;
        double DownSkillTimer2 = 0;
        int DownSkillCurrent = 0;
        double DownSkillXpoint = 0;
        double DownSkillYpoint = 0;
        bool useDownSkill = false;
    };
    #pragma endregion

}
