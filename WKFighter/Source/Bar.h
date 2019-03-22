#pragma once


namespace game_framework
{
	class Bar :public  BitmapPicture
	{
		friend class CDDraw;
	public:
        Bar();
		Bar(string, int, int, int, bool);
        ~Bar();
        int BelongPlayer;//屬於誰的(靠左或靠右)
        int newWidth;//新的寬度
        virtual void OnUpdate(double, double);
		virtual void Draw(int, int);//繪出圖型
	};
}
