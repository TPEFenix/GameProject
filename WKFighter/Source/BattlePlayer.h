#pragma once
#include "Keycode.h"
#include "KeyBoardState.h"
#include "WKAudio.h"
#include "CollisionSensor.h"
#include "AttackObj.h"

using namespace std;
using namespace WKAudio_namespace;
using namespace CollisionSensor_namespace;



namespace game_framework
{
#pragma region Defines
    //�{���s�X�Ωw�q

    //Game_Parameter_Header
#define GPH BattlePlayer *Enemy, CameraPosition Camera, KeyBoardState KeyState_now, KeyBoardState KeyState_last, Audio_ID Sounds
    //Game_Parameter_Parameter
#define GPP Enemy, Camera, KeyState_now, KeyState_last, Sounds


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
#define Player2_Skill			        I 
#define Player2_Technique	    O 
#define Player2_Jump			    J 
#define Player2_Rush			    U 
#define Player2_Guard			    P
//�@�γ]�w
#define GuardSPCost                0.2
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

        //�@�Ϋظm���(�j�h�ݭn�~���B�~���g)
        virtual void AnimationUpdate(CameraPosition);
        virtual void Draw(int, int, CameraPosition);//��s�禡�A�B�H�۵�������
        virtual void AutoLoadBitmaps(CameraPosition, COLORREF);//�̷ӦU�ۨ���Ū��
        virtual void OnUpdate(GPH);//��s�禡�A�B�H�۵�������
        virtual void InsertBitmapPicture(string, int, COLORREF);//��BimapAnimationŪ������
        virtual void InsertAction(string, int, COLORREF);//Ū���@�Ӱʧ@���Ҧ�����

        //�@�ΰ�¦�ʧ@���
        virtual void InputJudge(KeyBoardState, KeyBoardState);//��J��s���s
        virtual void GotoStandby(GPH);
        virtual void GotoRunning(GPH);
        virtual void GotoRush(GPH);
        virtual void GotoJump(GPH);
        virtual void GotoGuard(GPH);
        virtual void GotoCharge(GPH);
        virtual void OnStandby(GPH);
        virtual void OnRunning(GPH);
        virtual void OnJump(GPH);
        virtual void OnGuard(GPH);
        virtual void OnCharge(GPH);
        virtual void GotoNormalAttack1(GPH);//�ݭn�C�Ө���ӧO���g
        virtual void OnNormalAttack1(GPH);//�ݭn�C�Ө���ӧO���g

        //�M�˨禡
        virtual void AddSP(double mathin);//�W�[SP
        virtual void ProduceFriction(double, double);//�B�z�����O(�����⺥���C�U��)
        virtual void LoopStep(int);//(���ƦP�˰ʧ@)
        virtual void RunAhead(double, double);//(���T��V��)
        virtual void PhysicalMovement(BattlePlayer *, CameraPosition, KeyBoardState, KeyBoardState);//���z���ʡA������@�ΡA���D�S��
        double Ahead(double move);

        //�S�Ĭ����禡
        virtual void EffectAutoUpdate(BitmapAnimation*, int, bool, CameraPosition);//(���S�Ħ۰ʧ�s���A�A��bUpdate��)
        virtual void EffectReset(BitmapAnimation*, CameraPosition, double, double, double);
        virtual void DrawAllEffection(int, int, CameraPosition);//��s�禡�A�B�H�۵�������
        virtual void AutoLoadEffections(CameraPosition, COLORREF);//Ū���C���������S��
        virtual void InsertEffection(string, int,int, double, COLORREF);//Ū���C���������S��

        //������������禡
        virtual void AttackAutoUpdate(BitmapAnimation*, int, bool, CameraPosition);
        virtual void AttackReset(BitmapAnimation*, CameraPosition, double, double, double);
        virtual void DrawAllAttacks(int, int, CameraPosition);
        virtual void AutoLoadAttacks(CameraPosition, COLORREF);
        virtual void InsertAttacks(string, int, int, double, COLORREF);


        //��O���ܼ�(�Ѥl���O��l��)------------------------------------------------------------------------------------------------------
        double HP_Max;//�̤j�ͩR��
        double SP_Max;//�̤j��O
        double Rush_cost;//�Ĩ���Ӯ�O�q
        double RunSpeed;//�]�t
        double StandbySPincrements;//�ݾ��ɦ^�_����O�q
        double RunningSPincrements;//���ʮɦ^�_����O�q
        double ChargeSPincrements;//���ʮɦ^�_����O�q

        //�{���ܼ�------------------------------------------------------------------------------------------------------
        int PlayerNumber;//���a�s��
        double HP;//��e�ͩR
        double SP;//��e��O
        bool CanControl;//�i�H����
        bool Invincible;//�L�Ī��A
        bool visable;//�O�_�i��
        bool CanPixelCollision;//�O�_�ϥι����I���A�Ҧ��ʧ@���γ��|�M��
        bool InSideCamera;//�O�_�������Y�v�T
        bool IsRight;//���ۥk��
        bool OnGround;//�O�_�b�a���W
        bool Throughing;
        string Action;//�ʧ@���A
        int Step;//��e�B�J��

        //Timer�Φ��Ʊ���Ѽ�------------------------------------------------------------------------------------------------------
        double SPincrementsTimer = 0;
        double StandbyTimer = 0;
        double RunningTimer = 0;
        double RushTimer = 0;
        double JumpTimer = 0;
        double ChargeTimer = 0;
        double ChargeTimer2 = 0;
        int Chargecount = 0;
        //��J����Ѽ�------------------------------------------------------------------------------------------------------
        BattleInput Button_now;//�{�b����L���A
        BattleInput Button_last;//�W�@��������L���A

        //���z�Ѽ�------------------------------------------------------------------------------------------------------
        double Velocity_X;//X�t��
        double Velocity_Y;//Y�t��
        double Acceleration_X;//X�[�t��
        double Acceleration_Y;//Y�[�t��
        double Acceleration_gravity = 0.6;//���O�[�t��

        //��ܦ���------------------------------------------------------------------------------------------------------
        BitmapPicture *DisplayBitmap;//�û�����ܥΪ�Bitmap���V������BitmapPicture==��e����ܪ�BitmapPicture
        BitmapPicture BodyPicture;//���ĸI����

        BitMapRectangle Rect;//�Ϥ�����x�� �x�ޮy�и���e�A�|�bŪ���ɮ׮ɳ]�w�����A�åB�i�H�b�~�����
        BitMapRectangle BodyRect;//�H���I�����x��

        map<string, BitmapPicture>  BitmapPictures;//��Animation���Ҧ��Ϥ��ʧ@
        map<string, BitmapAnimation>  Effections;//�x�s�Ҧ��S��

        //��������--------------------------------------------------------------------------------------------------------
        map<string, AttackObj> AttackObjects;//�x�s�Ҧ���������

    };
}


