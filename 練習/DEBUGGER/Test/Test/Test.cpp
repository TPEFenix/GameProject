#include "stdafx.h"
#include "stdafx.h"
#include <iostream>
using namespace System;
using namespace std;


//int main(array<System::String ^> ^args)  = ¦³°ÝÃD 
void InputWeight(double* weight)
{
	cout << "please enter your weight(kg): " << endl;
	cin >> *weight;
}
double GetBMI(double height, double weight)
{
	double bmi;
	bmi = weight / pow(height / 100, 2);
	return bmi;
}
int main()
{
	double height = 0;
	double weight = 0;
	cout << "please enter your height(cm): " << endl;
	cin >> height; InputWeight(&weight);
	cout << "BMI = ";
	cout << GetBMI(height, weight);
	system("pause"); return 0;
}