
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
		Bar(int);					//�]�winit_hp
		Bar(int, int);				//�]�winit_hp, �]�w����k��
		void ReduceHp(int);			//����
		void AddHp(int);			//�^��
		bool IsDead();				//GG
		void SetPosition(int);		//�]�w����k��
		void SetHp(int);			//�]�wcurrent_hp
=======
		Bar(string, int, int, bool, bool, bool);

>>>>>>> f5ffc1c2fb2a9df7a1bd885a2cf36d45191b78e4
		~Bar();

		virtual void Draw(int, int);//ø�X�ϫ�
		
		int init_hp;				//��l��q
		int current_hp;				//�Y�ɦ�q
		int position;				//���k��
		bool isDead;				//GG
	};
}
#endif // !1