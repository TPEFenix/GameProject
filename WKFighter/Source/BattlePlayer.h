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
        virtual void OnUpdate(BattlePlayer *,CameraPosition, KeyBoardState, KeyBoardState, Audio_ID);//更新函式，且隨著視角移動
        virtual void Draw(int,int);//更新函式，且隨著視角移動
        virtual void AutoLoadBitmaps(string, int, bool, COLORREF, CameraPosition) = 0;//依照各自角色讀檔
        virtual void InsertBitmapPicture(string,int,COLORREF);

        double HP_Max;
        double SP_Max;
        double HP;
        double SP;
        bool CanControl;
        bool Invincible;
        string Action;


        /*
        //屬性成員
        BitMapRectangle Rect;//圖片材質矩形 掌管座標跟長寬，會在讀取檔案時設定完成，並且可以在外部更動
        BitmapPicture *DisplayBitmap;//永遠把顯示用的Bitmap指向對應的BitmapPicture==當前所顯示的BitmapPicture
        int Step;//當前步驟數
        int AutoMaxStep;//使用自動播放時的最大步驟數
        double AutoPlayTimer;//使用自動播放時的計時器
        bool visable;//是否可見
        bool CanPixelCollision;//是否使用像素碰撞，所有動作分割都會套用
        bool InSideCamera;
        map<string, BitmapPicture>  BitmapPictures;//該Animation的所有圖片動作
        */
    };
}


