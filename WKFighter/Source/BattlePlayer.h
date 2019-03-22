#pragma once
#include "Keycode.h"
#include "KeyBoardState.h"
#include "WKAudio.h"
using namespace std;
using namespace WKAudio_namespace;



namespace game_framework
{
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
        ~BattlePlayer();
    
        //�@�Ϋظm���(�j�h�ݭn�~���B�~���g)
        virtual void AnimationUpdate(CameraPosition);
        virtual void Draw(int,int);//��s�禡�A�B�H�۵�������
        virtual void AutoLoadBitmaps(CameraPosition,COLORREF);//�̷ӦU�ۨ���Ū��
		virtual void OnUpdate(BattlePlayer *, CameraPosition, KeyBoardState, KeyBoardState, Audio_ID);//��s�禡�A�B�H�۵�������
		virtual void PhysicalMovement(CameraPosition, KeyBoardState, KeyBoardState);//���z���ʡA������@�ΡA���D�S��
        virtual void InsertBitmapPicture(string,int,COLORREF);//��BimapŪ������
        
        //�@�ΰ�¦�ʧ@���
        virtual void InputJudge(KeyBoardState, KeyBoardState);//��J��s���s
        virtual void OnStandby(BattlePlayer *, CameraPosition, KeyBoardState, KeyBoardState, Audio_ID);
        virtual void OnRunning(BattlePlayer *, CameraPosition, KeyBoardState, KeyBoardState, Audio_ID);

		int PlayerNumber;//���a�s��
        double HP_Max;//�̤j�ͩR��
        double SP_Max;//�̤j��O
        double HP;//��e�ͩR
        double SP;//��e��O
        bool CanControl;//�i�H����
        bool Invincible;//�L�Ī��A
		bool visable;//�O�_�i��
		bool CanPixelCollision;//�O�_�ϥι����I���A�Ҧ��ʧ@���γ��|�M��
		bool InSideCamera;//�O�_�������Y�v�T
		bool IsRight;//���ۥk��
        bool OnGround;//�O�_�b�a���W
        string Action;//�ʧ@���A
        int Step;//��e�B�J��

        //Timer�Ѽ�
        double StandbyTimer = 0;
        double RunningTimer = 0;

        //��J����Ѽ�
        BattleInput Button_now;
        BattleInput Button_last;
        


        //���z�Ѽ�
		double Velocity_X;//X�t��
		double Velocity_Y;//Y�t��
		double Acceleration_X;//X�[�t��
		double Acceleration_Y;//Y�[�t��
		double Acceleration_gravity = 0.1;//���O�[�t��

        //��ܦ���
        BitmapPicture *DisplayBitmap;//�û�����ܥΪ�Bitmap���V������BitmapPicture==��e����ܪ�BitmapPicture
		BitMapRectangle Rect;//�Ϥ�����x�� �x�ޮy�и���e�A�|�bŪ���ɮ׮ɳ]�w�����A�åB�i�H�b�~�����
		map<string, BitmapPicture>  BitmapPictures;//��Animation���Ҧ��Ϥ��ʧ@
    };
}


