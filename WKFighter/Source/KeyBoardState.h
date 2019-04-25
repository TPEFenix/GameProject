#pragma once
#include "Keycode.h"
using namespace std;
namespace game_framework {
    class KeyBoardState
    {
    public:
        KeyBoardState();
        ~KeyBoardState();
        bool ESC = false;
        bool Backspace = false;
        bool Tab = false;
        bool Clear = false;
        bool Enter = false;
        bool Space = false;
        bool Shift = false;
        bool Control = false;
        bool Alt = false;
        bool CapsLock = false;
        bool PageUp = false;
        bool PageDown = false;
        bool End = false;
        bool Home = false;
        bool Left = false;
        bool Up = false;
        bool Right = false;
        bool Down = false;
        bool Insert = false;
        bool Del = false;
        bool Help = false;
        bool A = false;
        bool B = false;
        bool C = false;
        bool D = false;
        bool E = false;
        bool F = false;
        bool G = false;
        bool H = false;
        bool I = false;
        bool J = false;
        bool K = false;
        bool L = false;
        bool M = false;
        bool N = false;
        bool O = false;
        bool P = false;
        bool Q = false;
        bool R = false;
        bool S = false;
        bool T = false;
        bool U = false;
        bool V = false;
        bool W = false;
        bool X = false;
        bool Y = false;
        bool Z = false;
        bool Num_1 = false;
        bool Num_2 = false;
        bool Num_3 = false;
        bool Num_4 = false;
        bool Num_5 = false;
        bool Num_6 = false;
        bool Num_7 = false;
        bool Num_8 = false;
        bool Num_9 = false;
        bool Num_0 = false;
        bool Pad_1 = false;
        bool Pad_2 = false;
        bool Pad_3 = false;
        bool Pad_4 = false;
        bool Pad_5 = false;
        bool Pad_6 = false;
        bool Pad_7 = false;
        bool Pad_8 = false;
        bool Pad_9 = false;
        bool Pad_0 = false;
        bool Pad_Enter = false;
        void UpdateState_Down(UINT btnIn);
        void UpdateState_Up(UINT btnIn);
        
    };

}