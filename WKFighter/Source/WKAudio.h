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
        const int CutIn = 10;
        const int NormalHit2 = 11;
        const int SbDown = 12;
        const int DoubleHelixXi = 13;
        const int Title = 14;
        const int Beep = 15;
        const int Choose = 16;
        const int SliceHit = 17;
        const int light1 = 18;
        const int light2 = 19;
    };
    void LoadSounds(int , string);
    void PlaySounds(int, bool);
	void StopSounds(int soundID);
}
