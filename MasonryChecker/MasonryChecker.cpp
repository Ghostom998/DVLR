// DVLR.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MasonryChecker.h"
#include <iostream>
#include <string>

DVLR::DVLR()
{
	// Default Constructor
}

const float DVLR::GetSlenderness()
{
	std::cout << "Determine the Slenderness Ratio:" << std::endl;

	Teff = GetTeff(); // Get the effective thickness
	std::cout << "Therefore, effective thickness, Teff = " << TeffEquation;
	std::cout << Teff << "mm" << std::endl;

	Heff = GetHeff(); // Get the effective hieght
	std::cout << "Therefore, effective height, Heff = " << RestraintFactor;
	std::cout << " * " << HWall << " = " << Heff << "mm" << std::endl;

	return Heff / Teff; // TODO return the slenderness
}

// Returns the safety factor based on text input
const double DVLR::GetSafetyFactor()
{
	std::cout << "Determine the Partial Safety Factor :" << std::endl;

	// Defines the table that the Partial Safety Factor is taken from
	double PSFTable[2][2] = { { 2.5, 2.8 },{ 3.1, 3.5 } };

	int ConstrControl = GetConstrControl();
	int ManufControl = GetManufControl();

	return PSFTable[ConstrControl][ManufControl];
}

const float DVLR::GetUltLoad()
{
	// TODO Get loads
	// Factor them up
	// Check the spread (and if they spread)
	// Determine the total load

	return 0.0f;
}

const float DVLR::GetTeff()
{
	// Ask the user for the thickness of each leaf
	for (int i = 1; i <= 2; i++) // Hard number of 2 used as this will always check a cavity wall!
	{
		std::cout << "Enter the thickness of leaf " << i << ", t" << i << " [mm]:  ";
		std::cin >> TLeaf[i];
		std::cin.ignore(256, '\n'); // Clear the input buffer of the first 256 characters and newline characters
									// TODO pass input into function that checks if value is a number and over 0 => !CheckValidInput(TLeaf[i]); in a do-while loop
	}

	float t3 = (2 * (TLeaf[1] + TLeaf[2])) / 3;	// Effective thickness of two leaves
	float t4 = (TLeaf[1] >= TLeaf[2]) ? TLeaf[1] : TLeaf[2]; // Greatest thickness out of both leaves

	if (t4 >= t3)
	{
		TeffEquation = "Max{T1 ; T2} = ";
		return t4;
	}
	else
	{
		TeffEquation = "(TLeaf[1] + TLeaf[2])) / 3 = ";
		return t3;
	}
}

const float DVLR::GetHeff()
{

	std::cout << "Please input the height of the Wall [mm]:  ";
	std::cin >> HWall; // TODO pass input into function that checks if value is a number and over 0 => !CheckValidInput(TLeaf[i]); in a do-while loop
	std::cin.ignore(1000, '\n');
	RestraintFactor = GetRestraint();

	return HWall*RestraintFactor;
}

const float DVLR::GetRestraint()
{
	std::string Restraint;
	bool IsValid = true;

	do {
		std::cout << "Please enter the restraint conditions of the wall (Simple / Partial / Enhanced ):  ";
		getline(std::cin, Restraint);

		if (Restraint == "Simple" || Restraint == "simple" || Restraint == "S" || Restraint == "s")
		{
			return 1.0;
		}
		else if (Restraint == "Partial" || Restraint == "partial" || Restraint == "P" || Restraint == "p")
		{
			return 0.875;
		}
		else if (Restraint == "Enhanced" || Restraint == "enhanced" || Restraint == "E" || Restraint == "e")
		{
			return 0.75;
		}
		else
		{
			std::cout << "Please enter a valid input." << std::endl;
			IsValid = false;
			continue;
		}
		std::cin.ignore(1000, '\n'); // Clear input buffer to prevent errors
	} while (!IsValid);
	return 0.0;
}

const void DVLR::IsSlendernessOK(float& SR)
{
	if (SR >= 27)
	{
		std::cout << "SR greater than 27 is outside the scope of BS 5628-1:2005." << std::endl << std::endl;
		std::cout << "Program Terminated" << std::endl << std::endl;
		SR = 0;
		std::exit(-1);
	}
	else
	{
		std::cout << "SR < 27 and therefore within the scope of BS 5628-1." << std::endl << std::endl;
	}
}


int DVLR::GetConstrControl()
{
	std::string ConstrControl = "";
	bool IsCValid = true; // Validity of CONSTRUCTION control

	do
	{
		std::cout << "Please input normal or special CONSTRUCTION control (N/S):  ";
		getline(std::cin, ConstrControl);

		if (ConstrControl == "Normal" || ConstrControl == "normal" || ConstrControl == "N" || ConstrControl == "n")
		{
			return 1;
		}
		else if (ConstrControl == "Special" || ConstrControl == "special" || ConstrControl == "S" || ConstrControl == "s")
		{
			return 0;
		}
		else
		{
			std::cout << "Please enter a valid input." << std::endl;
			IsCValid = false;
			continue;
		}
		std::cin.ignore(1000, '\n'); // Clear buffer to prevent errors
	} while (!IsCValid);
	return -1;
}

int DVLR::GetManufControl()
{
	std::string ManufControl = "";
	bool IsMValid = true; // Validity of MANUFACTURE control

	do {
		std::cout << "Please input normal or special MANUFACTURE control (N/S):  ";
		getline(std::cin, ManufControl);

		if (ManufControl == "Normal" || ManufControl == "normal" || ManufControl == "N" || ManufControl == "n")
		{
			return 1;
		}
		else if (ManufControl == "Special" || ManufControl == "special" || ManufControl == "S" || ManufControl == "s")
		{
			return 0;
		}
		else
		{
			std::cout << "Please enter a valid input." << std::endl;
			IsMValid = false;
			continue;
		}
		std::cin.ignore(1000, '\n'); // Clear buffer to prevent errors
	} while (!IsMValid);
	return -1;
}

