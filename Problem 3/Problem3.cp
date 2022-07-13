/*
Author: Xin Xia
Class: ECE6122
Last Date Modified: 9/22/2021
Description: This is solution for Lab1 Problem3, calculating number of reflection given the initial location.
*/
#include<iostream>
#include<fstream>
#include<string>
#include<math.h>
#include<cmath>
using namespace std;
// Compute times of reflection depending on the input location x
float compCon(double location)
{
	int reflectTime=0;
	for (int i = 0; i <1000;i++)
	{	
		double position=(i + 2)* location;
		if (position-int(position)<0.0001 && int(position)%20==10*(i%2))
		{
			reflectTime = 2 * i + 1;
			break;
		}
	}	
	return reflectTime;
	}

int main(int argc, char* argv[])
{
	double x;
	x = atof(argv[1]);
	ofstream out("output3.txt");
	if (x <= -10 || x >= 10) 
	{ 
		cout << "Invalid Input"; 
	}
	else
	{
		cout << compCon(x);
	}
	return 0;
}
