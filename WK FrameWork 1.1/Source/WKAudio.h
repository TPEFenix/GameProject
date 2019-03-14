#pragma once
using namespace std;
namespace WKAudio_namespace
{
    // 定義各種音效的編號
    //在這裡加入你要使用的聲音名稱，數值不能重複
    struct Audio_ID
    {
        const int Ding = 0;
    } ;
    void LoadSounds(int , string);
    void PlaySounds(int, bool);
}
