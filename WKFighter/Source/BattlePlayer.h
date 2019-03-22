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
    
        //共用建置函數(大多需要繼承額外撰寫)
        virtual void AnimationUpdate(CameraPosition);
        virtual void Draw(int,int);//更新函式，且隨著視角移動
        virtual void AutoLoadBitmaps(CameraPosition,COLORREF);//依照各自角色讀檔
		virtual void OnUpdate(BattlePlayer *, CameraPosition, KeyBoardState, KeyBoardState, Audio_ID);//更新函式，且隨著視角移動
		virtual void PhysicalMovement(CameraPosition, KeyBoardState, KeyBoardState);//物理移動，全角色共用，除非特例
        virtual void InsertBitmapPicture(string,int,COLORREF);//使Bimap讀取圖檔
        
        //共用基礎動作函數
        virtual void InputJudge(KeyBoardState, KeyBoardState);//輸入更新按鈕
        virtual void OnStandby(BattlePlayer *, CameraPosition, KeyBoardState, KeyBoardState, Audio_ID);
        virtual void OnRunning(BattlePlayer *, CameraPosition, KeyBoardState, KeyBoardState, Audio_ID);

		int PlayerNumber;//玩家編號
        double HP_Max;//最大生命值
        double SP_Max;//最大氣力
        double HP;//當前生命
        double SP;//當前氣力
        bool CanControl;//可以控制
        bool Invincible;//無敵狀態
		bool visable;//是否可見
		bool CanPixelCollision;//是否使用像素碰撞，所有動作分割都會套用
		bool InSideCamera;//是否受到鏡頭影響
		bool IsRight;//面相右邊
        bool OnGround;//是否在地面上
        string Action;//動作狀態
        int Step;//當前步驟數

        //Timer參數
        double StandbyTimer = 0;
        double RunningTimer = 0;

        //輸入按鍵參數
        BattleInput Button_now;
        BattleInput Button_last;
        


        //物理參數
		double Velocity_X;//X速度
		double Velocity_Y;//Y速度
		double Acceleration_X;//X加速度
		double Acceleration_Y;//Y加速度
		double Acceleration_gravity = 0.1;//重力加速度

        //顯示成員
        BitmapPicture *DisplayBitmap;//永遠把顯示用的Bitmap指向對應的BitmapPicture==當前所顯示的BitmapPicture
		BitMapRectangle Rect;//圖片材質矩形 掌管座標跟長寬，會在讀取檔案時設定完成，並且可以在外部更動
		map<string, BitmapPicture>  BitmapPictures;//該Animation的所有圖片動作
    };
}


