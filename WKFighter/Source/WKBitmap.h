#pragma once
#include "TypeConverter.h"
using namespace std;
using namespace TypeConverter_namespace;

namespace game_framework
{
	struct BitMapRectangle
	{
		int X_int = 0;
		int Y_int = 0;
		double X = 0;
		double Y = 0;
		int Width = 0;
		int Height = 0;
	};
	struct CameraPosition
	{
		int X = 0;
		int Y = 0;
		double X_double = 0;
		double Y_double = 0;
	};
	class BitmapPicture :public   CMovingBitmap
	{
		friend class CDDraw;
	public:
		//隨緣
		BitmapPicture();
		//(是否可見)
		BitmapPicture(bool);
		//(路徑,是否可見)
		BitmapPicture(string, bool);
		//(路徑,是否可見,是否受到鏡頭影響)
		BitmapPicture(string, bool, bool);
		//(路徑,X,Y,是否可見,是否可碰撞,是否受到鏡頭影響)
		BitmapPicture(string, int, int, bool, bool, bool);
		//解構子
		~BitmapPicture();

		//方法函式
		//讀取圖檔(只需輸入透明色)
		void LoadTexture(COLORREF);
		//讀取圖檔(使用ResourcePath,透明色)
		void LoadTexture(bool, COLORREF);
		//讀取圖檔(路徑,是否可像素碰撞,透明色)
		void LoadTexture(char *, bool, COLORREF);
		void OnUpdate();
		void OnUpdate(CameraPosition);
		//繪出圖型
		virtual void Draw(int, int);
		virtual void Draw(int, int,double);

		//設定資源路徑連結
		void SetResourcePath(string);
		//得到資源路徑連結位置
		string GetResourcePath();

		//屬性成員
		BitMapRectangle Rect;//圖片材質矩形 掌管座標跟長寬，會在讀取檔案時設定完成，並且可以在外部更動
		vector<vector<bool> > EffectRect;//碰撞有效區域，執行像素碰撞使用
		bool visable;//是否可見
		bool CanPixelCollision;//是否可像素碰撞，如果該圖片無需使用則關閉，可大幅提升效率
		bool InSideCamera;

	private:
		string ResourcePath;//更改名字後建議重讀圖檔
	};
	class BitmapAnimation
	{
	public:
		//建構子
		BitmapAnimation();
		//(是否可見)
		BitmapAnimation(bool);
		//(路徑,是否可見)
		BitmapAnimation(string, bool);
		//(路徑,是否可見,是否受到鏡頭影響)
		BitmapAnimation(string, bool, bool);
		//(路徑,X,Y,是否可見,是否可碰撞,是否受到鏡頭影響)
		BitmapAnimation(string, int, int, bool, bool, bool);
		//解構子
		~BitmapAnimation();

		//方法函式
		void AutoPlay(int, bool);//自動撥放圖片(頻率,是否重複撥放)
		virtual void AutoLoadBitmaps(string, int, bool, COLORREF);//依照Name自動讀取檔名
		virtual void AutoLoadBitmaps(string, string, int, double, bool, COLORREF);//依照資料夾跟Name自動讀取檔名    //Effect要有左右
		virtual void OnUpdate();//更新函式，放在OnMove中(圖片才能動)
		virtual void OnUpdate(CameraPosition);//更新函式，且隨著視角移動
		virtual void OnUpdate(string, CameraPosition);//Effectfolder，在檔名前預加字串
		string GetName();//得到Name
		void SetName(string);//更改名字後需要立刻重讀圖檔!!

		//屬性變數
		int AutoMaxStep;//使用自動播放時的最大步驟數
		int drawlayer;//繪圖圖層
		double PreAutoFrequence;//預設的播放頻率
		bool visable;//是否可見
		bool CanPixelCollision;//是否使用像素碰撞，所有動作分割都會套用
		bool InSideCamera;//是否受到鏡頭影響
		bool BitmapisRight;//是否向右
		bool TrackPoint;//是否持續追蹤某一點
		bool loop = false;//是否重複("預設=否")

		//現狀變數
		double AutoPlayTimer;//使用自動播放時的計時器
		int Step;//當前步驟數
        double Acceleration_X;//X加速度
        double Acceleration_Y;//Y加速度
        double Velocity_X = 0;//X軸速度
        double Velocity_Y = 0;//Y軸速度
        double Acceleration_gravity = 0.5;//重力加速度

		//屬性成員
		BitMapRectangle Rect;//圖片材質矩形 掌管座標跟長寬，會在讀取檔案時設定完成，並且可以在外部更動
		BitmapPicture *DisplayBitmap;//永遠把顯示用的Bitmap指向對應的BitmapPicture==當前所顯示的BitmapPicture
		map<string, BitmapPicture>  BitmapPictures;//該Animation的所有圖片動作
	private:
		string Name;//此可動圖檔的名稱[資源命名規則:放在Content\\Bitmaps\\Name\\Name_Step.bmp]
	};
}

