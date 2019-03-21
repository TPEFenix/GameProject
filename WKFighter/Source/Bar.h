
#ifndef _BAR_H
#define _BAR_H

namespace game_framework
{
	class Bar :public BitmapPicture
	{
		friend class CDDraw;
	public:
		Bar();
		~Bar();
		virtual void Draw(int, int);//Ã¸¥X¹Ï«¬
	};
}
#endif // !1