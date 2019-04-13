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

    class Matchstick :public  BattlePlayer
    {
        #define Matchstick_HP_Max 500;
        #define Matchstick_SP_Max 100;
        #define Matchstick_Rush_Cost 8;
        #define Matchstick_RunSpeed 6;
        #define Matchstick_StandbySPincrements 0.2;
        #define Matchstick_RunningSPincrements 0.1;
        #define Matchstick_ChargeSPincrements 12;


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



        //Timer及次數控制參數------------------------------------------------------------------------------------------------------
        double NormalAttack1Timer = 0;
        double Shot1Timer = 0;
        int Shot1Current = 0;
        bool IsRushAttack = false;

    };
}
