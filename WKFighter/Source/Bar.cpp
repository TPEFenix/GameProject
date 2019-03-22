#include "stdafx.h"
#include <ddraw.h>
#include <windows.h>
#include <stdio.h>
#include <sstream>
#include "gamelib.h"
#include "WKBitmap.h"
#include "TypeConverter.h"
#include "Bar.h"

namespace game_framework
{

	Bar::Bar() : BitmapPicture()
	{
		init_hp = 250;
		current_hp = init_hp;
		position = 1;
		Rect.X_int = 50;
		Rect.Y_int = 50;
		newWidth = 250;
	}

	Bar::Bar(int hp) : BitmapPicture()
	{
		init_hp = hp;
		current_hp = init_hp;
		position = 1;
		Rect.X_int = 50;
		Rect.Y_int = 50;
		newWidth = 250;
	}
	
	Bar::Bar(int hp, int pos) : BitmapPicture("RES\\hp.bmp", true, true)
	{
		init_hp = hp;
		current_hp = init_hp;
		position = pos;
		newWidth = 250;
		if (position == 1)
		{
			this->Rect.X_int = 50;
			this->Rect.Y_int = 50;
		}
		else
		{
			this->Rect.X_int = 550;// +(200 - init_hp);
			this->Rect.Y_int = 50;
		}
	}

	void Bar::ReduceHp(int reduce)
	{
		current_hp = current_hp - reduce;
		if (current_hp <= 0)
		{
			current_hp = 1;
		}
		OnUpdate((double)current_hp, (double)init_hp, Rect.X_int, Rect.Y_int);
	}
	void Bar::AddHp(int add)
	{
		current_hp = current_hp + add;
		if (current_hp > init_hp)
			current_hp = init_hp;
		OnUpdate((double)current_hp, (double)init_hp, Rect.X_int, Rect.Y_int);
	}


	void Bar::OnUpdate(double now, double max, int X, int Y)
	{
		this->newWidth = int((now / max)*this->Rect.Width);
		if (newWidth == 0)
			newWidth = 1;
		Rect.X_int = X;
		Rect.Y_int = Y;
	}

	void Bar::SetPosition(int pos)
	{
		position = pos;
		if (position == 1)
		{
			this->Rect.X_int = 50;
			this->Rect.Y_int = 50;
		}
		else
		{
			this->Rect.X_int = 550 + (200 - init_hp);
			this->Rect.Y_int = 50;
		}
	}

	void Bar::SetHp(int hp)
	{
		current_hp = hp;
		OnUpdate((double)current_hp, (double)init_hp, Rect.X_int, Rect.Y_int);
	}


    Bar::~Bar()
	{
	}
 
	void Bar::Draw(int CurrentLayer, int TargetLayer)
	{
		if (CurrentLayer == TargetLayer&&this->visable == true)//直到他的圖片層級才可以顯示
		{
			if (position == 1)
			{
				SetTopLeft(Rect.X_int, Rect.Y_int);
				ShowBitmap(this->Rect.X_int, this->Rect.Y_int, this->Rect.X_int + newWidth, this->Rect.Y_int + this->Rect.Height);
			}
			else
			{
				SetTopLeft(Rect.X_int, Rect.Y_int);
				ShowBitmap(this->Rect.X_int + Rect.Width - newWidth, this->Rect.Y_int, this->Rect.X_int + Rect.Width, this->Rect.Y_int + this->Rect.Height);
			}
		}
	}

}