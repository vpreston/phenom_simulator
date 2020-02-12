/*
* Script which walks through a simple Lorenz attractor implementation and solver
*
* Created on: Feb. 11, 2020
* Author: Victoria Preston
*/

#include <iostream>
using namespace std;

// define the differential equation
float func(float x, float y)
{
	return (x + y + x * y);
}

// euler's formula
void euler(float x0, float y, float h, float x)
{
	float temp = -0;

	while (x0 < x) {
		temp = y;
		y = y + h * func(x0, y);
		x0 = x0 + h;
	}

	cout << "Approximate solution at x = " << x << " is " << y << endl;

}

int main()
{
	float x0 = 0;
	float y0 = 1;
	float h = 0.025;

	float x = 0.1;

	euler(x0, y0, h, x);
	return 0;
}