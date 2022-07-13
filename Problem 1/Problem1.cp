/*
Author: Xin Xia
Class: ECE6122
Last Date Modified: 9/22/2021
Description: This is solution for Lab1 Problem1, calculating prime factors of given input.
*/

#include <iostream>
#include<fstream>
#include<string>
using namespace std;

bool GetPrimeFactors(const unsigned long ulInputNumber, string& strOutput)
{		
	if (ulInputNumber == 1 || ulInputNumber == 0)
	{	
		strOutput = "No prime factors";
		return false;
	} // if input 1 or 0, print "No prime factors"
	else if(ulInputNumber == 2)
	{	
		strOutput = "2";
		return true;
	}
	else
	{
		int x = ulInputNumber;
		for (int i = 2; i <= x; i++)
			if (x % i == 0)
			{
				strOutput += (to_string(i) + ",");
				x = x / i;
				i = 1;
			}
			else 
			{ continue; }
		if(x>1){ strOutput += to_string(x); }
		else { strOutput.pop_back(); }	
		return true;
	}// if input>=2, calculate prime factors
}

int main(int argc, char* argv[])
{
	string strOutput = "";
	ofstream out("output1.txt");
	float check = atof(argv[1]);
	if (check < 0 || check != int(check))
	{
		out << "Invalid Input";
	} // if input negative or float numbers, print "Invalid Input" 
	else 
	{ 
		unsigned long x = atoll(argv[1]);
		GetPrimeFactors(x, strOutput);
		out << strOutput;
	}// if input positive integer numbers, print its prime factors
	return 0;
}