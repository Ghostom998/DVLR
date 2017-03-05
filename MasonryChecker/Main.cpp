// MasonryChecker.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MasonryChecker.h"
#include <iostream>
#include <limits>
//using namespace std;

/*
* double t1, t2, t3, t4, h, teff, heff, SR, l, op1, op2; // declaring the wall dimension variables
* double dle1, lle1, dlc1, llc1, dle2, lle2, dlc2, llc2, y1, y2, sw1, sw2, wult1, wult2; // declaring the load variables
* double e1, e2, ex1, ex2, ea1, ea2, et1, et2, em1, em2, B1, B2, SAF1, SAF2; // declaring eccentricity variables
* double A1, A2, ym, fkm1, fkm2, fka1, fka2; // declaring remaining variables
*/

void Intro();
bool AskToSaveFile();
DVLR Wall; // A wall object in the DVLR class

int main()
{
	bool SaveFile = false;

	Intro();
	Wall.StartProgram();

	// User option to save file
	SaveFile = AskToSaveFile();
	if (SaveFile)
	{
		// Writes to file
		Wall.PrintToFile();
	}

	// Pauses the system for debugging purposes
	std::cout << "Press ENTER to continue..." << std::flush;
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	return 0;
}

void Intro()
{
	std::cout << "Consider the design vertical load resistance of load bearing cavity masonry" << std::endl;
	std::cout << "to BS 5628-1:2005, with eccentricity determined using the appendix b method:" << std::endl << std::endl;
	return;
}

void DVLR::StartProgram()
{
	SR = GetSlenderness();
	std::cout << "And therefore the Slenderness Ratio, SR = Heff/Teff = " << Heff << " / " << Teff << " = ";
	std::cout << SR << std::endl << std::endl;
	IsSlendernessOK(SR);

	PSF = GetSafetyFactor();
	std::cout << "And therefore the Partial Safety Factor to be adopted, PSF = " << PSF << std::endl << std::endl;

	WultLoad = Wall.GetUltLoad();
	std::cout << "Ultimate line load in Leaf 1: " << WultLoad.Leaf1 << " kN/m" << std::endl;
	std::cout << "Ultimate line load in Leaf 2: " << WultLoad.Leaf2 << " kN/m" << std::endl;

	Beta = GetBeta();
	std::cout << "Capacity Reduction Factor to Leaf 1, Beta: " << Beta.Leaf1 << std::endl;
	std::cout << "Capacity Reduction Factor to Leaf 2, Beta: " << Beta.Leaf2 << std::endl;
	
	SAF = GetSmallAreaFactor(); // Messages inside function

	std::cout << "Determine Minimum required masonry strength:" << std::endl;
	MinFk.Leaf1 = GetMinFk(PSF, WultLoad.Leaf1,  Beta.Leaf1,  SAF.Leaf1, TLeaf[0]);
	MinFk.Leaf2 = GetMinFk(PSF, WultLoad.Leaf2, Beta.Leaf2, SAF.Leaf2, TLeaf[1]);
	std::cout << "Minimum required masonry strength to Leaf 1, Fk = " << MinFk.Leaf1 << "N/mm2" << std::endl;
	std::cout << "Minimum required masonry strength to Leaf 2, Fk = " << MinFk.Leaf2 << "N/mm2" << std::endl;
	
	return;
}

bool AskToSaveFile()
{
	// Flushes the input buffer
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	std::cout << "\nDo you want to save the output (y/n)? ";
	std::string Response = "";
	std::getline(std::cin, Response);
	std::cout << std::endl;

	// Returns the bool true if the first letter begins 
	// with the upper or lowercase "y" else returns false.
	return (Response[0] == 'y') || (Response[0] == 'Y');
}