/*
Author: Xin Xia
Class: ECE6122
Last Date Modified: 9/22/2021
Description: This is solution for Lab1 Problem2, calculating number of black squares after given moves.
*/
#include <iostream>
#include<fstream>
#include <string>
#include<array>
#include <unordered_map>
using namespace std;
// define a structure to record information
typedef unordered_map<string,string> dict;

int main(int argc, char* argv[])
{
	ofstream out("output2.txt"); // output file
	float check = atof(argv[1]);
	if (check < 0 || check != int(check))
	{
		out << "Invalid Input";
	} // if input negative or float numbers, print "Invalid Input" }
	else
	{
		int x = 0;
		int y = 0; // initial position of ant (x,y)
		int ori = 0; // 0:up, 1:right, 2:down, 3:left
		string color = "white"; // initial color of the square
		string position = "(0,0)"; // initial string of position
		unsigned long step = atoll(argv[1]); // number of moves from command line
		unsigned long numBlack = 0; // initial number of black squares
		dict info; // the information of history squares
		for (int i = 1;i <= step;i++)
		{
			// get color of current square
			int flag = 1; // the flag indicates whether current square has been reached
			for (auto& item : info)
			{
				if (item.first == position)
				{
					color = item.second;
					flag = 0;
				}
				else { flag *= 1; }
			}
			// compute orientation of next move, flip current square,
			// count number of black squares
			if (color == "white")
			{
				if (ori == 3) { ori = 0; }
				else { ori += 1; }
				color = "black";
				numBlack += 1;
			}
			else
			{
				if (ori == 0) { ori = 3; }
				else { ori -= 1; }
				color = "white";
				numBlack -= 1;
			}
			// move to next square
			if (ori == 0) { y += 1; } // up
			else if (ori == 1) { x += 1; } // right
			else if (ori == 2) { y -= 1; } // down
			else { x -= 1; } // left
			// update or insert the information of the old square
			if (flag == 0) { info.at(position) = color; }
			else { info.insert({ position, color }); }
			// update position of new square and initialize color
			position = "(" + to_string(x) + "," + to_string(y) + ")";
			color = "white";
		}
		out << numBlack;
		//cout << numBlack;
		return 0;
	}
}
