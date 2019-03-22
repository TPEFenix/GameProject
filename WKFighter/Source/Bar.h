
#ifndef _BAR_H
#define _BAR_H

namespace game_framework
{
	class Bar :public  BitmapPicture
	{
		friend class CDDraw;
	public:

		Bar();						//init_hp = 200
		Bar(int);					//�]�winit_hp
		Bar(int, int);				//�]�winit_hp, �]�w����k��
        ~Bar();
		void ReduceHp(int);			//����
		void AddHp(int);			//�^��
		bool IsDead();				//GG
		void OnUpdate(double,double);
		void SetPosition(int);		//�]�w����k��
		void SetHp(int);			//�]�wcurrent_hp
		virtual void Draw(int, int);//ø�X�ϫ�
		int init_hp;				//��l��q
		int current_hp;				//�Y�ɦ�q
		int position;				//���k��
		int newWidth;

	};
}
#endif // !1