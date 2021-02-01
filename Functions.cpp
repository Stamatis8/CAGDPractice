#include "Functions.h"



double Raise(double base, int exponent) {
	if (exponent == 0) {
		if (base != 0)return 1;
		else {
			//*
			// Deals with 0^0
			//*
		}
	}
	else if (base == 1) {
		return 1;
	}
	else if (base == -1) {
		if (exponent % 2 == 0)return 1;
		else if (exponent % 2 != 0)return -1;
	}
	else if (base == 0) {
		return 0;
	}
	else if (exponent < 0) {
		return 1 / (base * pow(base, -1 * exponent - 1));
	}
	else {
		return base*pow(base, exponent - 1);
	}
}

double Factorial(signed int a)
{
	if (a == 0)return 1;
	return a * Factorial(a - 1);
}

bool IsIn(int i, std::vector<int> vector)
{
	for (int j : vector) {
		if (j == i)return true;
		else continue;
	}
	return false;
}

bool IsNear(double a, double b, double radius)
{
	if (a > b - radius && a < b + radius)return true;
	else return false;
}

double NChooseI(signed int n, int i) {
	if (i == 0 || i == n) {
		return 1;
	}
	else if (i <= n && i >= 0) {
		return Factorial(n) / (Factorial(i) * Factorial(n - i));
	}
	else return 0;
}
