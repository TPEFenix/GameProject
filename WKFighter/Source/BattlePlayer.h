#pragma once
#include "Keycode.h"
#include "KeyBoardState.h"
#include "WKAudio.h"
using namespace std;
using namespace WKAudio_namespace;


namespace game_framework
{
    class BattlePlayer :public  BitmapAnimation
    {
    public:
        BattlePlayer();
        ~BattlePlayer();

        virtual void AnimationUpdate(CameraPosition);
        virtual void Draw(int,int);//��s�禡�A�B�H�۵�������
        virtual void AutoLoadBitmaps(string, int, bool, COLORREF, CameraPosition) = 0;//�̷ӦU�ۨ���Ū��
		virtual void OnUpdate(BattlePlayer *, CameraPosition, KeyBoardState, KeyBoardState, Audio_ID) = 0;//��s�禡�A�B�H�۵�������
		virtual void PhysicalMovement(CameraPosition, KeyBoardState, KeyBoardState);//���z���ʡA������@�ΡA���D�S��
        virtual void InsertBitmapPicture(string,int,COLORREF);//��BimapŪ������

		int PlayerNumber;//���a�s��
        double HP_Max;//�̤j�ͩR��
        double SP_Max;//�̤j��O
        double HP;//��e�ͩR
        double SP;//��e��O
        bool CanControl;//�i�H����
        bool Invincible;//�L�Ī��A
        string Action;//�ʧ@���A
		int Step;//��e�B�J��

		double Velocity_X;//X�t��
		double Velocity_Y;//Y�t��
		double Acceleration_X;//X�[�t��
		double Acceleration_Y;//Y�[�t��
		const double Acceleration_gravity = 2;//���O�[�t��






        /*
        //�ݩʦ���
        BitMapRectangle Rect;//�Ϥ�����x�� �x�ޮy�и���e�A�|�bŪ���ɮ׮ɳ]�w�����A�åB�i�H�b�~�����
        BitmapPicture *DisplayBitmap;//�û�����ܥΪ�Bitmap���V������BitmapPicture==��e����ܪ�BitmapPicture
        int Step;//��e�B�J��
        int AutoMaxStep;//�ϥΦ۰ʼ���ɪ��̤j�B�J��
        double AutoPlayTimer;//�ϥΦ۰ʼ���ɪ��p�ɾ�
        bool visable;//�O�_�i��
        bool CanPixelCollision;//�O�_�ϥι����I���A�Ҧ��ʧ@���γ��|�M��
        bool InSideCamera;
        map<string, BitmapPicture>  BitmapPictures;//��Animation���Ҧ��Ϥ��ʧ@
        */
    };
}


