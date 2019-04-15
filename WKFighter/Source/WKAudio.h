#pragma once
using namespace std;
namespace WKAudio_namespace
{
    // �w�q�U�ح��Ī��s��
    //�b�o�̥[�J�A�n�ϥΪ��n���W�١A�ƭȤ��୫��
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
