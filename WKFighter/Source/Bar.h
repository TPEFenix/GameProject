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
        int BelongPlayer;//�ݩ�֪�(�a���ξa�k)
        int newWidth;//�s���e��
        virtual void OnUpdate(double, double);
		virtual void Draw(int, int);//ø�X�ϫ�
	};
}
