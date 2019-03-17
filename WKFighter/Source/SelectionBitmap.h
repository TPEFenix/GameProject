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
        virtual void AutoLoadBitmaps(string, int, bool, COLORREF);//依照Name自動讀取檔名

    };
}
