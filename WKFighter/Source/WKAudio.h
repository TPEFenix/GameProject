#pragma once
using namespace std;
namespace WKAudio_namespace
{
    // 定義各種音效的編號
    //在這裡加入你要使用的聲音名稱，數值不能重複
    struct Audio_ID
    {
        const int Ding = 0;
        const int Rush = 1;
        const int Jump = 2;
        const int SPCharge = 3;
        const int BKMusic = 4;
        const int NormalHit = 5;
        const int HitWall = 6;
        const int Disable = 7;
        const int Stoned = 8;
        const int Fire1 = 9;
    };
    void LoadSounds(int , string);
    void PlaySounds(int, bool);
	void StopSounds(int soundID);
}
