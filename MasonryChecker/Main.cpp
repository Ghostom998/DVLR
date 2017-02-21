// MasonryChecker.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MasonryChecker.h"
#include <iostream>
//using namespace std;

/*
* float t1, t2, t3, t4, h, teff, heff, SR, l, op1, op2; // declaring the wall dimension variables
* float dle1, lle1, dlc1, llc1, dle2, lle2, dlc2, llc2, y1, y2, sw1, sw2, wult1, wult2; // declaring the load variables
* double e1, e2, ex1, ex2, ea1, ea2, et1, et2, em1, em2, B1, B2, SAF1, SAF2; // declaring eccentricity variables
* float A1, A2, ym, fkm1, fkm2, fka1, fka2; // declaring remaining variables
*/

void Intro();
void StartProgram();
DVLR Wall; // A wall object in the DVLR class

int main()
{
	Intro();
	StartProgram();
	system("pause");
	return 0;
}

void Intro()
{
	std::cout << "Consider the design vertical load resistance of load bearing cavity masonry" << std::endl;
	std::cout << "to BS 5628-1:2005, with eccentricity determined using the appendix b method:" << std::endl << std::endl;
	return;
}

void StartProgram()
{
	Wall.GetSlenderness();
	//Wall.GetUltLoad();
	/*Wall.GetBeta();
	Wall.GetSAF();
	Wall.GetMinFk(); */
	return;
}
