#include "stdafx.h"
#include <sstream>
#include "TypeConverter.h"

using namespace std;

namespace TypeConverter_namespace
{
    string IntToString(int intin)
    {
        stringstream SS = stringstream();
        SS << intin;
        string StepString;
        SS >> StepString;
        return StepString;
    }
    int StringToInt(string str)
    {
        return stoi(str);
    }

}