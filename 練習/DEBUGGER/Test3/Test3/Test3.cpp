#include "stdafx.h"
#include <iostream> 
using namespace std;
int Fibonacci(int n)
{
	if (n == 0)
		return 0;
	if (n == 1)
		return 1;
	return (Fibonacci(n - 1) + Fibonacci(n - 2));

}
int main()
{
	cout << "Fibonacci: ";
	for (int i = 0; i<10; i++)
	{
		cout << Fibonacci(i) << " ";
	}
	system("pause");
	return 0;
}