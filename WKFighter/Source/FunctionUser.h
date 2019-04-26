#pragma once
#include <thread>

using namespace std;
using namespace game_framework;

namespace FunctionUser_namespace
{
    void LoadingResource(void (LoadingFunction)(), thread *mThread, bool *started, bool *finished);
    void PlayEffect(BattlePlayer *Master, string EffectName, CameraPosition Camera, double XR, double XL, double Y);
    void PlayEffect(EffectSprite *Effects, string EffectName, CameraPosition Camera, double XR, double XL, double Y,bool IsRight);
    void DisableEffect(BattlePlayer *Master, string EffectName);
    void DisableEffect(EffectSprite *Effects, string EffectName);
}

