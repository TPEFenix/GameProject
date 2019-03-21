
#ifndef _BAR_H
#define _BAR_H

namespace game_framework
{
	class Bar :public  BitmapPicture
	{
		friend class CDDraw;
	public:
<<<<<<< HEAD
		Bar();						//init_hp = 200
		Bar(int);					//設定init_hp
		Bar(int, int);				//設定init_hp, 設定左方右方
		void ReduceHp(int);			//扣血
		void AddHp(int);			//回血
		bool IsDead();				//GG
		void SetPosition(int);		//設定左方右方
		void SetHp(int);			//設定current_hp
=======
		Bar(string, int, int, bool, bool, bool);

>>>>>>> f5ffc1c2fb2a9df7a1bd885a2cf36d45191b78e4
		~Bar();

		virtual void Draw(int, int);//繪出圖型
		
		int init_hp;				//初始血量
		int current_hp;				//即時血量
		int position;				//左右方
		bool isDead;				//GG
	};
}
#endif // !1