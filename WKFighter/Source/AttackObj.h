#pragma once
#include "stdafx.h"
#include "CollisionSensor.h"
#include "TypeConverter.h"
#include "EffectSprite.h"
#include "WKAudio.h"



using namespace std;
using namespace WKAudio_namespace;
using namespace CollisionSensor_namespace;


namespace game_framework
{
    #define AttackObjPH AttackObj *Attack, string BeloneName,BattlePlayer *Belone,BattlePlayer *Target,double Damage ,double SP_Damege,int Mass, bool IsRight,double HitVelocity_X, double HitVelocity_Y,double XR,double XL, double Y,double VX,double VY,double HitTime,double MaxAliveTime,int Attributes, bool CanCombo ,bool Drawable,bool Replay ,bool HitNoon,bool HitBreak,bool CanHitFly,bool CanBeDisappear,bool CanCrackOther,string HitEffect,int HitSound,CameraPosition Camera
    #define AttackObjPH_Normal AttackObj *Attack, BattlePlayer *Belone, BattlePlayer *Target, double Damage,  double HitVelocity_X, double HitVelocity_Y, double XR, double XL, double Y, double VX, double VY, double HitTime, double MaxAliveTime, string HitEffect,const int HitSound, CameraPosition Camera
    #define AttackObjPH_Shot AttackObj *Attack, BattlePlayer *Belone, BattlePlayer *Target, double Damage,  double HitVelocity_X, double HitVelocity_Y, double XR, double XL, double Y, double VX, double VY, double HitTime, double MaxAliveTime,int Mass,bool HitNoon,bool CanBeDisappear,bool CanCrackOther, string HitEffect,const int HitSound, CameraPosition Camera
    ////
    #define PerfectBlockTime 100
    ////

    class BattlePlayer;
    class EffectSprite;

    class AttackObj :public BitmapAnimation
    {
    public:
        AttackObj();
        AttackObj(string, int, int, bool, bool, bool);
        ~AttackObj();


        virtual void OnUpdate(string, CameraPosition);
		virtual void HitAction(CameraPosition);
        //現狀變數
        double AliveTimer = 0;
        double ComboTimer = 0;

        double Timer1 = 0;
        double Timer2 = 0;
        bool IsHited;//已經擊中

                     //屬性變數
        double Damage;//傷害
        double SP_Damege;//對SP造成減少
        double HitTime;//使敵人僵直時間
        double MaxAliveTime;//攻擊最大顯示時間

        double HitVelocity_X;
        double HitVelocity_Y;

        int Mass;//武器力度
        int Attributes;//狀態屬性-1=無
        bool CanCombo;//可持續擊中
        bool Drawable;//是否需要繪製出來
        bool Replay;//是否將攻擊圖檔持續撥放
        bool HitNoon;//是否攻擊完就消失
        bool HitBreak;//是否破防
        bool CanHitFly;//是否可擊飛
        bool CanBeDisappear;//可被抵銷
        bool CanCrackOther;//可抵銷別的攻擊
        bool GravityEffect;//是否受重力影響
		bool UseRectCollision;//使用矩形碰撞判定


        string HitEffect;//擊中特效名稱
        int HitSound;//擊中聲音ID


        EffectSprite Effects;
        double Ahead(double move);

        BattlePlayer *Belone;
        BattlePlayer *Target;

    };





    class AttackManager
    {
    public:
        AttackManager();
        ~AttackManager();
        map<string, AttackObj> AttackObjects;//儲存所有攻擊物件
        virtual void AttackAutoUpdate(AttackObj * Attack, string BeloneName, int tick, bool replay, CameraPosition Camera);
        virtual void AttackReset(AttackObjPH);
        virtual void AttackReset_Normal(AttackObjPH_Normal);
        virtual void AttackReset_Shot(AttackObjPH_Shot);
        virtual void DrawAllAttacks(int);
        virtual void InsertAttacks(string BeloneName, string name, int maxstep, int drawlayer, double pre, int category, COLORREF color, CameraPosition Camera);
        virtual void InsertAttacks(string BeloneName, string name, int maxstep, int drawlayer, double pre, int category, int current, COLORREF color, CameraPosition Camera);
    };
}
