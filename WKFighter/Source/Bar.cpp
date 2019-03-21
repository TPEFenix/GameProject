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

	Bar::Bar() : BitmapPicture("RES\\hp.bmp", true, true)
	{
		init_hp = 200;
		current_hp = init_hp;
		position = 1;
		isDead = false;
		Rect.X = 50;
		Rect.Y = 50;
	}

	Bar::Bar(int hp) : BitmapPicture("RES\\hp.bmp", true, true)
	{
		init_hp = hp;
		current_hp = init_hp;
		position = 1;
		isDead = false;
		Rect.X = 50;
		Rect.Y = 50;
	}
	
	Bar::Bar(int hp, int pos) : BitmapPicture("RES\\hp.bmp", true, true)
	{
		init_hp = hp;
		current_hp = init_hp;
		position = pos;
		isDead = false;
		if (position = 1)
		{
			Rect.X = 50;
			Rect.Y = 50;
		}
		else
		{
			Rect.X = 600 + (200 - init_hp);
			Rect.Y = 50;
		}
	}

	void Bar::ReduceHp(int reduce)
	{
		current_hp = current_hp - reduce;
		if (current_hp < 0)
		{
			current_hp = 0;
			isDead = true;
		}
	}
	void Bar::AddHp(int add)
	{
		current_hp = current_hp + add;
		if (current_hp > init_hp)
			current_hp = init_hp;
	}

	bool Bar::IsDead()
	{
		if (current_hp <= 0)
			isDead = true;
		return isDead;
	}

	void Bar::SetPosition(int pos)
	{
		position = pos;
	}

	void Bar::SetHp(int hp)
	{
		current_hp = hp;
	}


	Bar::~Bar()
	{
	}

	void Bar::Draw(int CurrentLayer, int TargetLayer)
	{
		if (CurrentLayer == TargetLayer&&this->visable == true)//直到他的圖片層級才可以顯示
		{
			if (position = 1)
			{
				SetTopLeft(Rect.X_int, Rect.Y_int);
				ShowBitmap(this->Rect.X_int, this->Rect.Y_int, this->Rect.X_int + current_hp, this->Rect.Y_int + this->Rect.Height);
			}
			else
			{
				SetTopLeft(Rect.X_int + current_hp, Rect.Y_int);
				ShowBitmap(this->Rect.X_int, this->Rect.Y_int, this->Rect.X_int + current_hp, this->Rect.Y_int + this->Rect.Height);
			}
		}
	}

	void fuck()
	{

	}
}