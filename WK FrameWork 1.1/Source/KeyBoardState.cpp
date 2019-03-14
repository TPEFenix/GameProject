#include "stdafx.h"
#include "KeyBoardState.h"
#include "Keycode.h"

namespace game_framework {

    KeyBoardState::KeyBoardState()
    {

    }


    KeyBoardState::~KeyBoardState()
    {
    }

    void KeyBoardState::UpdateState_Down(UINT btnIn)
    {
        Keycode Keys;
        if (btnIn == Keys.A)A = true;
        else if (btnIn == Keys.B)B = true;
        else if (btnIn == Keys.C)C = true;
        else if (btnIn == Keys.D)D = true;
        else if (btnIn == Keys.E)E = true;
        else if (btnIn == Keys.F)F = true;
        else if (btnIn == Keys.G)G = true;
        else if (btnIn == Keys.H)H = true;
        else if (btnIn == Keys.I)I = true;
        else if (btnIn == Keys.J)J = true;
        else if (btnIn == Keys.K)K = true;
        else if (btnIn == Keys.L)L = true;
        else if (btnIn == Keys.M)M = true;
        else if (btnIn == Keys.N)N = true;
        else if (btnIn == Keys.O)O = true;
        else if (btnIn == Keys.P)P = true;
        else if (btnIn == Keys.Q)Q = true;
        else if (btnIn == Keys.R)R = true;
        else if (btnIn == Keys.S)S = true;
        else if (btnIn == Keys.T)T = true;
        else if (btnIn == Keys.U)U = true;
        else if (btnIn == Keys.V)V = true;
        else if (btnIn == Keys.W)W = true;
        else if (btnIn == Keys.X)X = true;
        else if (btnIn == Keys.Y)Y = true;
        else if (btnIn == Keys.Z)Z = true;
        else if (btnIn == Keys.ESC)ESC = true;
        else if (btnIn == Keys.Backspace)Backspace = true;
        else if (btnIn == Keys.Tab)Tab = true;
        else if (btnIn == Keys.Clear) Clear = true;
        else if (btnIn == Keys.Enter)Enter = true;
        else if (btnIn == Keys.Space) Space = true;
        else if (btnIn == Keys.Shift)Shift = true;
        else if (btnIn == Keys.Control)Control = true;
        else if (btnIn == Keys.Alt)Alt = true;
        else if (btnIn == Keys.CapsLock)CapsLock = true;
        else if (btnIn == Keys.PageUp)PageUp = true;
        else if (btnIn == Keys.PageDown) PageDown = true;
        else if (btnIn == Keys.End)End = true;
        else if (btnIn == Keys.Home) Home = true;
        else if (btnIn == Keys.Left)Left = true;
        else if (btnIn == Keys.Up)Up = true;
        else if (btnIn == Keys.Right) Right = true;
        else if (btnIn == Keys.Down)Down = true;
        else if (btnIn == Keys.Insert)Insert = true;
        else if (btnIn == Keys.Del)Del = true;
        else if (btnIn == Keys.Help) Help = true;
        else if (btnIn == Keys.Num_1)Num_1 = true;
        else if (btnIn == Keys.Num_2)Num_2 = true;
        else if (btnIn == Keys.Num_3)Num_3 = true;
        else if (btnIn == Keys.Num_4)Num_4 = true;
        else if (btnIn == Keys.Num_5)Num_5 = true;
        else if (btnIn == Keys.Num_6)Num_6 = true;
        else if (btnIn == Keys.Num_7)Num_7 = true;
        else if (btnIn == Keys.Num_8)Num_8 = true;
        else if (btnIn == Keys.Num_9)Num_9 = true;
        else if (btnIn == Keys.Num_0)Num_0 = true;
        else if (btnIn == Keys.Pad_1)Pad_1 = true;
        else if (btnIn == Keys.Pad_2)Pad_2 = true;
        else if (btnIn == Keys.Pad_3)Pad_3 = true;
        else if (btnIn == Keys.Pad_4)Pad_4 = true;
        else if (btnIn == Keys.Pad_5)Pad_5 = true;
        else if (btnIn == Keys.Pad_6)Pad_6 = true;
        else if (btnIn == Keys.Pad_7)Pad_7 = true;
        else if (btnIn == Keys.Pad_8)Pad_8 = true;
        else if (btnIn == Keys.Pad_9)Pad_9 = true;
        else if (btnIn == Keys.Pad_0)Pad_0 = true;
        else if (btnIn == Keys.Pad_Enter)Pad_Enter = true;
    }

    void KeyBoardState::UpdateState_Up(UINT btnIn)
    {
        Keycode Keys;
        if (btnIn == Keys.A)A = false;
        else if (btnIn == Keys.B)B = false;
        else if (btnIn == Keys.C)C = false;
        else if (btnIn == Keys.D)D = false;
        else if (btnIn == Keys.E)E = false;
        else if (btnIn == Keys.F)F = false;
        else if (btnIn == Keys.G)G = false;
        else if (btnIn == Keys.H)H = false;
        else if (btnIn == Keys.I)I = false;
        else if (btnIn == Keys.J)J = false;
        else if (btnIn == Keys.K)K = false;
        else if (btnIn == Keys.L)L = false;
        else if (btnIn == Keys.M)M = false;
        else if (btnIn == Keys.N)N = false;
        else if (btnIn == Keys.O)O = false;
        else if (btnIn == Keys.P)P = false;
        else if (btnIn == Keys.Q)Q = false;
        else if (btnIn == Keys.R)R = false;
        else if (btnIn == Keys.S)S = false;
        else if (btnIn == Keys.T)T = false;
        else if (btnIn == Keys.U)U = false;
        else if (btnIn == Keys.V)V = false;
        else if (btnIn == Keys.W)W = false;
        else if (btnIn == Keys.X)X = false;
        else if (btnIn == Keys.Y)Y = false;
        else if (btnIn == Keys.Z)Z = false;
        else if (btnIn == Keys.ESC)ESC = false;
        else if (btnIn == Keys.Backspace)Backspace = false;
        else if (btnIn == Keys.Tab)Tab = false;
        else if (btnIn == Keys.Clear) Clear = false;
        else if (btnIn == Keys.Enter)Enter = false;
        else if (btnIn == Keys.Space) Space = false;
        else if (btnIn == Keys.Shift)Shift = false;
        else if (btnIn == Keys.Control)Control = false;
        else if (btnIn == Keys.Alt)Alt = false;
        else if (btnIn == Keys.CapsLock)CapsLock = false;
        else if (btnIn == Keys.PageUp)PageUp = false;
        else if (btnIn == Keys.PageDown) PageDown = false;
        else if (btnIn == Keys.End)End = false;
        else if (btnIn == Keys.Home) Home = false;
        else if (btnIn == Keys.Left)Left = false;
        else if (btnIn == Keys.Up)Up = false;
        else if (btnIn == Keys.Right) Right = false;
        else if (btnIn == Keys.Down)Down = false;
        else if (btnIn == Keys.Insert)Insert = false;
        else if (btnIn == Keys.Del)Del = false;
        else if (btnIn == Keys.Help) Help = false;
        else if (btnIn == Keys.Num_1)Num_1 = false;
        else if (btnIn == Keys.Num_2)Num_2 = false;
        else if (btnIn == Keys.Num_3)Num_3 = false;
        else if (btnIn == Keys.Num_4)Num_4 = false;
        else if (btnIn == Keys.Num_5)Num_5 = false;
        else if (btnIn == Keys.Num_6)Num_6 = false;
        else if (btnIn == Keys.Num_7)Num_7 = false;
        else if (btnIn == Keys.Num_8)Num_8 = false;
        else if (btnIn == Keys.Num_9)Num_9 = false;
        else if (btnIn == Keys.Num_0)Num_0 = false;
        else if (btnIn == Keys.Pad_1)Pad_1 = false;
        else if (btnIn == Keys.Pad_2)Pad_2 = false;
        else if (btnIn == Keys.Pad_3)Pad_3 = false;
        else if (btnIn == Keys.Pad_4)Pad_4 = false;
        else if (btnIn == Keys.Pad_5)Pad_5 = false;
        else if (btnIn == Keys.Pad_6)Pad_6 = false;
        else if (btnIn == Keys.Pad_7)Pad_7 = false;
        else if (btnIn == Keys.Pad_8)Pad_8 = false;
        else if (btnIn == Keys.Pad_9)Pad_9 = false;
        else if (btnIn == Keys.Pad_0)Pad_0 = false;
        else if (btnIn == Keys.Pad_Enter)Pad_Enter = false;

    }

}