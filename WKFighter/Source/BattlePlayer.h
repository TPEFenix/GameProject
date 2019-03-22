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
        virtual void Draw(int,int);//更新函式，且隨著視角移動
        virtual void AutoLoadBitmaps(string, int, bool, COLORREF, CameraPosition) = 0;//依照各自角色讀檔
		virtual void OnUpdate(BattlePlayer *, CameraPosition, KeyBoardState, KeyBoardState, Audio_ID) = 0;//更新函式，且隨著視角移動
		virtual void PhysicalMovement(CameraPosition, KeyBoardState, KeyBoardState);//物理移動，全角色共用，除非特例
        virtual void InsertBitmapPicture(string,int,COLORREF);//使Bimap讀取圖檔

		int PlayerNumber;//玩家編號
        double HP_Max;//最大生命值
        double SP_Max;//最大氣力
        double HP;//當前生命
        double SP;//當前氣力
        bool CanControl;//可以控制
        bool Invincible;//無敵狀態
        string Action;//動作狀態
		int Step;//當前步驟數

		double Velocity_X;//X速度
		double Velocity_Y;//Y速度
		double Acceleration_X;//X加速度
		double Acceleration_Y;//Y加速度
		const double Acceleration_gravity = 2;//重力加速度






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


