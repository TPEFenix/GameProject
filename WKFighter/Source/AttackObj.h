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
#define AttackObjPH AttackObj *Attack, string BeloneName, string HitEffect, int HitSound,double Damage, double XR, double XL, double Y, double VX, double VY, double HitVX, double HitVY,double HitTime, bool Track, bool IsRight, bool CanCombo, bool drawable,bool HitBreak, CameraPosition Camera 
    class AttackObj :public BitmapAnimation
    {
    public:
        AttackObj();
        AttackObj(string, int, int, bool, bool, bool);
        ~AttackObj();

        virtual void OnUpdate(string, CameraPosition);//Effectfolder�A�b�ɦW�e�w�[�r��

        //�{���ܼ�
        double HitVelocity_X = 0;
        double HitVelocity_Y = 0;
        double Timer1 = 0;
        double Timer2 = 0;
        bool IsHited;//�w�g����

        //�ݩ��ܼ�
        double Damage = 0;//�ˮ`
        bool CanCombo;//�i��������
        bool Drawable;//�O�_�ݭnø�s�X��
        string HitEffect;
        int HitSound;
        double HitTime;
        bool HitBreak;

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
        virtual void InsertAttacks(string BeloneName, string name, int maxstep, int drawlayer, double pre, COLORREF color, CameraPosition Camera);

    };


}
