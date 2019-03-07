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
void SelectionSort(int num[], int length)
{
	int i, j, max;
	for (i = 0; i<length; i++)
	{
		max = i;
		for (j = i + 1; j<length; j++)
		{
			if (num[j] > num[max])
				max = j;
		}
		Swap(num, i, max);
	}
}

int main()
{
	int num[10] = { 12, 42, 23, 51, 8, 31, 24, 57, 78, 33 };
	int length = 10; cout << "Array: "; for (int i = 0;
		i<length; i++)
	{
		cout << num[i] << " ";
	}
	cout << endl; //Bubble
	BubbleSort(num, length);
	cout << "Bubble Sort: ";
	for (int i = 0; i<length; i++)
	{
		cout << num[i] << " ";
	}
	cout << endl;
	//Selection Sort
	SelectionSort(num, length);
	cout << "Selection Sort: ";
	for (int i = 0; i<length; i++)
	{
		cout << num[i] << " ";
	}
	return 0;
}