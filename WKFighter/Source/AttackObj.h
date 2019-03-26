#pragma once
#include "stdafx.h"
#include "CollisionSensor.h"
#include "TypeConverter.h"
#include "WKAudio.h"



using namespace std;
using namespace WKAudio_namespace;
using namespace CollisionSensor_namespace;

namespace game_framework
{
#define AttackObjPH AttackObj *Attack, string BeloneName, double Damage ,double SP_Damege, bool IsRight,double HitVelocity_X, double HitVelocity_Y,double XR,double XL, double Y,double VX,double VY,double HitTime,double MaxAliveTime,int Attributes, bool CanCombo ,bool Drawable,bool Replay ,bool HitNoon,bool HitBreak,string HitEffect,int HitSound,CameraPosition Camera
    
    
    
    
    class AttackObj :public BitmapAnimation
    {
    public:
        AttackObj();
        AttackObj(string, int, int, bool, bool, bool);
        ~AttackObj();

        virtual void OnUpdate(string, CameraPosition);//Effectfolder，在檔名前預加字串

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

        int Attributes;//狀態屬性-1=無
        bool CanCombo;//可持續擊中
        bool Drawable;//是否需要繪製出來
        bool Replay;//是否將攻擊圖檔持續撥放
        bool HitNoon;//是否攻擊完就消失
        bool HitBreak;//是否破防


        string HitEffect;//擊中特效名稱
        int HitSound;//擊中聲音ID

        double Ahead(double move);
    };





    class AttackManager
    {
    public:
        AttackManager();
        ~AttackManager();
        map<string, AttackObj> AttackObjects;//儲存所有攻擊物件
        virtual void AttackAutoUpdate(AttackObj * Attack, string BeloneName, int tick, bool replay, CameraPosition Camera);
        virtual void AttackReset(AttackObjPH);
        virtual void DrawAllAttacks(int);
        virtual void InsertAttacks(string BeloneName, string name, int maxstep, int drawlayer, double pre, int category, COLORREF color, CameraPosition Camera);

    };
}
