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
	Intro();
	Wall.StartProgram();

	// User option to save file
	auto SaveFile = AskToSaveFile();
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

	WultLoad = GetUltLoad();
	for (int i = 0 ; i<=1 ; i++)
	{
		std::cout << "Ultimate line load in Leaf " << i+1 << ": " << WultLoad.Leaf[i] << " kN / m" << std::endl;
	}

	if (Opening[0].IsOpening || Opening[1].IsOpening) std::cout << "\nCheck the bearing at the supports:" << std::endl;
	//std::cout << "Min Bearing Strength required at support " << std::endl;
	for (int i = 0; i <= 1; i++) // Opening
	{
		if (Opening[i].IsOpening)
		{
			std::cout << "Min Bearing Strength required at Support " << i + 1 << std::endl;
			for (int j = 0; j <= 1; j++) // Leaf
			{
				MinBStrength[i] = CheckLintelBearing(Opening[i].BLength, TLeaf, LoadOverWall, SelfWeightOverOpening[i], Opening[i].Width, PSF, i);
				std::cout << "  Leaf " << i + 1 << ": " << MinBStrength[i].Leaf[j] << "N/mm2" << std::endl;
			}
		}
	}

	Beta = GetBeta();
	for (int i = 0 ; i <= 1 ; i++)
	{
		std::cout << "Capacity Reduction Factor to Leaf " << i + 1 << ", Beta: " << Beta.Leaf[i] << std::endl;
	}

	SAF = GetSmallAreaFactor(); // Messages inside function

	std::cout << "Determine Minimum required masonry strength:" << std::endl;
	for (int i = 0; i <= 1; i++)
	{
		MinFk.Leaf[i] = GetMinFk(PSF, WultLoad.Leaf[i], Beta.Leaf[i], SAF.Leaf[i], TLeaf[i]);
		std::cout << "Minimum required masonry strength to Leaf " << i+1 << ", Fk = " << MinFk.Leaf[i] << "N/mm2" << std::endl;
	}

	return;
}

bool AskToSaveFile()
{
	std::cout << "\nDo you want to save the output (y/n)? ";
	std::string Response = "";
	std::getline(std::cin, Response);
	std::cout << std::endl;
	return (Response[0] == 'y') || (Response[0] == 'Y');
}
