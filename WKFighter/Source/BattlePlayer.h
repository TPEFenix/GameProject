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

        virtual void AnimationUpdat(CameraPosition);
        virtual void OnUpdate(BattlePlayer *,CameraPosition, KeyBoardState, KeyBoardState, Audio_ID);//��s�禡�A�B�H�۵�������
        virtual void Draw(int,int);//��s�禡�A�B�H�۵�������
        virtual void AutoLoadBitmaps(string, int, bool, COLORREF, CameraPosition) = 0;//�̷ӦU�ۨ���Ū��
        virtual void InsertBitmapPicture(string,int,COLORREF);

        double HP_Max;
        double SP_Max;
        double HP;
        double SP;
        bool CanControl;
        bool Invincible;
        string Action;


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


