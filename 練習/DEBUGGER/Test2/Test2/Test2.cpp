#include "stdafx.h"
#include <iostream> 
using namespace std;
void Swap(int num[], int indexA, int indexB)
{
	int tmp = num[indexA];
	num[indexA] = num[indexB];
	num[indexB] = tmp;
}
void BubbleSort(int num[], int length)
{
	for (int i = length - 1; i > 0; --i)
	{
		for (int j = 0; j < i; ++j)
		{
			if (num[j] > num[j + 1])
			{
				Swap(num, j, j + 1);
			}
		}
	}
}
int main()
{
	int num[5]; int length =
		5; num[0] = 12; num[1] = 42;
	num[2] = 23; num[3] = 51;
	num[4] = 8; cout <<
		"Array: "; for (int i = 0;
			i<length; i++)
	{
		cout << num[i] << " ";
	}
	cout << endl;
	BubbleSort(num, length);
	cout << "Bubble Sort: ";
	for (int i = 0; i<length; i++)
	{
		cout << num[i] << " ";
	}
	system("pause");
	return 0;
}