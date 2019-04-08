#pragma once
#include "stdafx.h"
#include "Keycode.h"
#include "KeyBoardState.h"
#include "CollisionSensor.h"
#include "WKAudio.h"
#include "EffectSprite.h"
#include "AttackObj.h"

using namespace std;
using namespace WKAudio_namespace;
using namespace CollisionSensor_namespace;


namespace game_framework
{
#pragma region Defines
    //程式編碼用定義

    //Game_Parameter_Header
#define GPH BattlePlayer *Enemy, CameraPosition Camera, KeyBoardState KeyState_now, KeyBoardState KeyState_last, Audio_ID Sounds, COLORREF color
    //Game_Parameter_Parameter
#define GPP Enemy, Camera, KeyState_now, KeyState_last, Sounds,color

    //GameCanTo
#define CanToStandby GotoStandby(GPP)
#define CanToRunning GotoRunning(GPP)
#define CanToJump if (CanControl&&Button_now.button_Jump&&Button_last.button_Jump == false){GotoJump(GPP);}
#define CanToRush if (CanControl&&Button_now.button_Rush&&Button_last.button_Rush == false&&Button_now.button_Down == false){GotoRush(GPP);}
#define CanToGuard  if (CanControl&&Button_now.button_Guard && Button_now.button_Down == false && OnGround){GotoGuard(GPP);}
#define CanToCharge  if (CanControl&&Button_now.button_Guard&&Button_last.button_Guard == false && Button_now.button_Down && OnGround){GotoCharge(GPP);}
#define CanToNormalAttack1  if (CanControl&&Button_now.button_Attack&&Button_now.button_Up == false && Button_now.button_Down == false&&Button_last.button_Attack == false && OnGround){GotoNormalAttack1(GPP);}
#define CanToSkill1  if (CanControl&&Button_now.button_Skill&&Button_now.button_Up == false&& Button_now.button_Down == false&&Button_last.button_Skill == false){GotoSkill1(GPP);}
#define CanToAirAttack1  if (CanControl&&OnGround == false&&Button_now.button_Up == false && Button_now.button_Down == false &&Button_now.button_Attack&& Button_last.button_Attack == false){GotoAirAttack1(GPP);}
#define CanToAirAttack2  if (CanControl&&OnGround == false&&Button_now.button_Up == false && Button_now.button_Down == false &&Button_now.button_Attack&& Button_last.button_Attack == false){GotoAirAttack2(GPP);}
#define CanToAirDownAttack if (CanControl&&OnGround == false&&Button_now.button_Up == false && Button_now.button_Down == true &&Button_now.button_Attack&& Button_last.button_Attack == false){GotoAirDownAttack(GPP);}
#define CanToAirUpAttack if (CanControl&&OnGround == false&&Button_now.button_Up == true && Button_now.button_Down == false &&Button_now.button_Attack&& Button_last.button_Attack == false){GotoAirUpAttack(GPP);}

//Inputconfigure
#define Player1_Left				    A 
#define Player1_Right			    D 
#define Player1_Up				    W 
#define Player1_Down			    S 
#define Player1_Attack			    G 
#define Player1_Skill			        T 
#define Player1_Technique	    Y 
#define Player1_Jump			    F 
#define Player1_Rush			    R 
#define Player1_Guard			    E 
#define Player2_Left				    Left
#define Player2_Right			    Right
#define Player2_Up				    Up 
#define Player2_Down			    Down 
#define Player2_Attack			    K
#define Player2_Skill			        L 
#define Player2_Technique	    O 
#define Player2_Jump			    J 
#define Player2_Rush			    I 
#define Player2_Guard			    P
//共用設定
#define GuardSPCost                0.15
#define MaxGravity                8
#pragma endregion


    struct BattleInput
    {
        bool button_Right = false;
        bool button_Left = false;
        bool button_Up = false;
        bool button_Down = false;
        bool button_Attack = false;
        bool button_Skill = false;
        bool button_Technique = false;
        bool button_Jump = false;
        bool button_Rush = false;
        bool button_Guard = false;
    };

    class BattlePlayer :public  BitmapAnimation
    {
    public:
        BattlePlayer();
        virtual ~BattlePlayer();

        //共用建置函數(大多需要繼承額外撰寫)
        virtual void AnimationUpdate(CameraPosition);
        virtual void Draw(int, int, CameraPosition);//更新函式，且隨著視角移動
        virtual void AutoLoadBitmaps(GPH);//依照各自角色讀檔
        virtual void OnUpdate(GPH);//更新函式，且隨著視角移動
        virtual void InsertBitmapPicture(string, int, COLORREF);//使BimapAnimation讀取圖檔
        virtual void InsertAction(string, int, COLORREF);//讀取一個動作的所有圖檔

        //共用基礎動作函數
        virtual void CheckHit(GPH);//受傷觸發瞬間函式
        virtual void InputJudge(KeyBoardState, KeyBoardState);//輸入更新按鈕
        //待機動作


        virtual void GotoStandby(GPH);
        virtual void OnStandby(GPH);
        //跑步動作
        virtual void GotoRunning(GPH);
        virtual void OnRunning(GPH);
        //衝刺動作
        virtual void GotoRush(GPH);
        virtual void OnRush(GPH);
        //跳躍動作
        virtual void GotoJump(GPH);
        virtual void OnJump(GPH);
        //防禦動作
        virtual void GotoGuard(GPH);
        virtual void OnGuard(GPH);
        //練氣動作
        virtual void GotoCharge(GPH);
        virtual void OnCharge(GPH);
        //受傷動作
        virtual void OnHit(GPH);
        virtual void OnHitGuard(GPH);


        //攻擊動作
        virtual void GotoNormalAttack1(GPH);//需要每個角色個別撰寫
        virtual void OnNormalAttack1(GPH);//需要每個角色個別撰寫
        virtual void GotoAirAttack1(GPH);//需要每個角色個別撰寫
        virtual void OnAirAttack1(GPH);//需要每個角色個別撰寫
        virtual void GotoSkill1(GPH);//需要每個角色個別撰寫
        virtual void OnSkill1(GPH);//需要每個角色個別撰寫
        virtual void GotoUpAttack(GPH);//需要每個角色個別撰寫
        virtual void OnUpAttack(GPH);//需要每個角色個別撰寫
        virtual void GotoDownAttack(GPH);//需要每個角色個別撰寫
        virtual void OnDownAttack(GPH);//需要每個角色個別撰寫
        virtual void GotoRushAttack(GPH);//需要每個角色個別撰寫
        virtual void OnRushAttack(GPH);//需要每個角色個別撰寫
        virtual void GotoAirUpAttack(GPH);//需要每個角色個別撰寫
        virtual void OnAirUpAttack(GPH);//需要每個角色個別撰寫
        virtual void GotoAirDownAttack(GPH);//需要每個角色個別撰寫
        virtual void OnAirDownAttack(GPH);//需要每個角色個別撰寫
        virtual void GotoUpSkill(GPH);//需要每個角色個別撰寫
        virtual void OnUpSkill(GPH);//需要每個角色個別撰寫
        virtual void GotoDownSkill(GPH);//需要每個角色個別撰寫
        virtual void OnDownSkill(GPH);//需要每個角色個別撰寫
        virtual void GotoRushSkill(GPH);//需要每個角色個別撰寫
        virtual void OnRushSkill(GPH);//需要每個角色個別撰寫
        virtual void GotoAirUpSkill(GPH);//需要每個角色個別撰寫
        virtual void OnAirUpSkill(GPH);//需要每個角色個別撰寫
        virtual void GotoAirDownSkill(GPH);//需要每個角色個別撰寫
        virtual void OnAirDownSkill(GPH);//需要每個角色個別撰寫

        //套裝函式------------------------------------------------------------------------------------------------------
        virtual void AddSP(double mathin);//增加SP
        virtual void GainHP(double mathin);//增加HP
        virtual void GainSP(double mathin);//增加SP
        virtual void ProduceFriction(double, double);//處理摩擦力(讓角色漸漸慢下來)
        virtual void GotoDrop(GPH);
        virtual void LoopStep(int);//(重複同樣動作)
        virtual void RunAhead(double, double);//(正確方向走)
        virtual void PhysicalMovement(GPH);//物理移動，全角色共用，除非特例
        double Ahead(double move);//依照正確方向行動回傳正負(以右為準)



        //能力值變數(由子類別初始化)------------------------------------------------------------------------------------------------------
        double HP_Max;//最大生命值
        double SP_Max;//最大氣力
        double recovery;
        double Rush_cost;//衝刺消耗氣力量
        double RunSpeed;//跑速
        double StandbySPincrements;//待機時回復的氣力量
        double RunningSPincrements;//移動時回復的氣力量
        double ChargeSPincrements;//移動時回復的氣力量

        //現狀變數------------------------------------------------------------------------------------------------------
        int PlayerNumber;//玩家編號
        double HP;//當前生命
        double SP;//當前氣力
        bool CanControl;//可以控制
        bool Invincible;//無敵狀態
        bool visable;//是否可見
        bool CanPixelCollision;//是否使用像素碰撞，所有動作分割都會套用
        bool InSideCamera;//是否受到鏡頭影響
        bool IsRight;//面相右邊
        bool OnGround;//是否在地面上
        bool Throughing;//可否穿越
        bool HitFly;//被擊非
        string Action;//動作狀態
        int Step;//當前步驟數
        vector<bool> AttributeState;//異常狀態

        //Timer及次數控制參數------------------------------------------------------------------------------------------------------
        double SPincrementsTimer = 0;
        double StandbyTimer = 0;
        double RunningTimer = 0;
        double RushTimer = 0;
        double JumpTimer = 0;
        double ChargeTimer = 0;
        double ChargeTimer2 = 0;
        int Chargecount = 0;
        double BeHitTimer = 0;
        double BeHitTimeMax = 0;
        double NotHitTimer = 0;
        double OnHitTimer = 0;
        //輸入按鍵參數------------------------------------------------------------------------------------------------------
        BattleInput Button_now;//現在的鍵盤狀態
        BattleInput Button_last;//上一瞬間的鍵盤狀態

        //物理參數------------------------------------------------------------------------------------------------------
        double Velocity_X;//X速度
        double Velocity_Y;//Y速度
        double Acceleration_X;//X加速度
        double Acceleration_Y;//Y加速度
        double Acceleration_gravity = 0.5;//重力加速度

        //成員------------------------------------------------------------------------------------------------------
        BitmapPicture *DisplayBitmap;//永遠把顯示用的Bitmap指向對應的BitmapPicture==當前所顯示的BitmapPicture
        BitmapPicture BodyPicture;//有效碰撞區

        BitMapRectangle Rect;//圖片材質矩形 掌管座標跟長寬，會在讀取檔案時設定完成，並且可以在外部更動
        BitMapRectangle BodyRect;//人物碰撞的矩形

        map<string, BitmapPicture>  BitmapPictures;//該Animation的所有圖片動作
        EffectSprite Effects;//特效物件集合
        AttackManager Attacks;//攻擊物件集合
    };
}


