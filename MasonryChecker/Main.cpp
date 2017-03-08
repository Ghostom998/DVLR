// MasonryChecker.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MasonryChecker.h"
#include <iostream>
#include <limits>

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
	std::cout << "Welcome to MasonryChecker, a program to consider the vertical" << std::endl;
	std::cout << "load design of cavity masonry in accordance with BS 5628 - 1:2005." << std::endl;
	std::cout << "To exit the program at any time press ctrl+c." << std::endl << std::endl;
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
