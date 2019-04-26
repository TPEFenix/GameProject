#pragma once
#include "TypeConverter.h"
using namespace std;
using namespace TypeConverter_namespace;
namespace game_framework
{
    class SelectionBitmap : public BitmapAnimation
    {
    public:
        SelectionBitmap();
        ~SelectionBitmap();
        virtual void OnUpdate(int,int);
        virtual void AutoLoadBitmaps(string, COLORREF);//依照Name自動讀取檔名
    };
}
