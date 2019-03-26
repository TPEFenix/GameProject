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

        virtual void OnUpdate(string, CameraPosition);//Effectfolder�A�b�ɦW�e�w�[�r��

        //�{���ܼ�
        double AliveTimer = 0;
        double ComboTimer = 0;
        double Timer1 = 0;
        double Timer2 = 0;
        bool IsHited;//�w�g����

        //�ݩ��ܼ�
        double Damage;//�ˮ`
        double SP_Damege;//��SP�y�����
        double HitTime;//�ϼĤH�����ɶ�
        double MaxAliveTime;//�����̤j��ܮɶ�

        double HitVelocity_X;
        double HitVelocity_Y;

        int Attributes;//���A�ݩ�-1=�L
        bool CanCombo;//�i��������
        bool Drawable;//�O�_�ݭnø�s�X��
        bool Replay;//�O�_�N�������ɫ��򼷩�
        bool HitNoon;//�O�_�������N����
        bool HitBreak;//�O�_�}��


        string HitEffect;//�����S�ĦW��
        int HitSound;//�����n��ID

        double Ahead(double move);
    };





    class AttackManager
    {
    public:
        AttackManager();
        ~AttackManager();
        map<string, AttackObj> AttackObjects;//�x�s�Ҧ���������
        virtual void AttackAutoUpdate(AttackObj * Attack, string BeloneName, int tick, bool replay, CameraPosition Camera);
        virtual void AttackReset(AttackObjPH);
        virtual void DrawAllAttacks(int);
        virtual void InsertAttacks(string BeloneName, string name, int maxstep, int drawlayer, double pre, int category, COLORREF color, CameraPosition Camera);

    };
}
