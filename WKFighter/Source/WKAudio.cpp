#include "stdafx.h"
#include <ddraw.h>
#include "gamelib.h"
#include "audio.h"
#include "WKAudio.h"

using namespace std;
using namespace game_framework;

namespace WKAudio_namespace
{
    void LoadSounds(int ID, string path)
    {
        //範例
        //CAudio::Instance()->Load(0, "sounds\\ding.wav");
        CAudio::Instance()->Load(ID, &path[0]);	// 載入編號0的聲音ding.wav
    }
    //播放聲音(ID,是否重複撥放)
    void PlaySounds(int soundID, bool replay)
    {
        CAudio::Instance()->Play(soundID, replay);
    }
	void StopSounds(int soundID)
	{
		CAudio::Instance()->Stop(soundID);
	}

}
