// MasonryChecker.cpp : Defines the entry point for the console application.
//

#include "stdafx.h" // Turn on in Visual Studio
#include "MasonryChecker.h"
#include <iostream>
#include <string>

DVLR::DVLR() {/* Default Constructor*/ }

const double DVLR::GetSlenderness()
{
	std::cout << "Determine the Slenderness Ratio:" << std::endl;

	Teff = GetTeff(); // Get the effective thickness
	std::cout << "Therefore, effective thickness, Teff = " << TeffEquation;
	std::cout << Teff << "mm" << std::endl;

	Heff = GetHeff(); // Get the effective hieght
	std::cout << "Therefore, effective height, Heff = " << RestraintFactor;
	std::cout << " * " << HWall << " = " << Heff << "mm" << std::endl;

	return Heff / Teff;
}

const double DVLR::GetTeff()
{
	// Ask the user for the thickness of each leaf
	for (int i = 0; i <= 1; i++) // Hard number of 2 used as this will always check a cavity wall!
	{
		bool IsValid = true;
		do {
			std::cout << "Enter the thickness of leaf " << i + 1 << ", t" << i + 1 << " [mm]:  ";
			std::cin >> TLeaf[i];

			//Checks if user input a number
			if (!std::cin)// or if(cin.fail())
			{
				// user didn't input a number
				std::cin.clear(); // reset failbit
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //skip Invalid input
																			   // next, request user reinput
				IsValid = false;
				std::cout << "Invalid input, please try again." << std::endl;
			}
			else
			{
				IsValid = true;
			}
		} while (!IsValid);
			
		std::cin.ignore(256, '\n'); // Clear the input buffer of the first 256 characters and newline characters
	}

	double t3 = (2 * (TLeaf[0] + TLeaf[1])) / 3;	// Effective thickness of two leaves
	double t4 = (TLeaf[0] >= TLeaf[1]) ? TLeaf[0] : TLeaf[1]; // Greatest thickness out of both leaves

	if (t4 >= t3)
	{
		TeffEquation = "Max{t1 ; t2} = ";
		return t4;
	}
	else
	{
		TeffEquation = "2(t1 + t2) / 3 = ";
		return t3;
	}
}

const double DVLR::GetHeff()
{
	bool IsValid = true;
	do
	{
		std::cout << "Please input the height of the Wall [mm]:  ";
		std::cin >> HWall;
		std::cin.ignore(1000, '\n');
		
		if (!std::cin) // or if(cin.fail())
		{
			// user didn't input a number
			std::cin.clear(); // reset failbit
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //skip Invalid input
																		   // next, request user reinput
			IsValid = false;
			std::cout << "Invalid input, please try again." << std::endl;
		}
		else
		{
			IsValid = true;
		}
	} while (!IsValid);

	RestraintFactor = GetRestraint();

	return HWall*RestraintFactor;
}

const double DVLR::GetRestraint()
{
	std::string Restraint;
	bool IsValid = true;

	do {
		std::cout << "Please enter the restraint conditions of the wall (Simple / Partial / Enhanced ):  ";
		getline(std::cin, Restraint);

		if (Restraint == "Simple" || Restraint == "simple" || Restraint == "S" || Restraint == "s")
		{
			RestraintCondition = "simple";
			return 1.0;
		}
		else if (Restraint == "Partial" || Restraint == "partial" || Restraint == "P" || Restraint == "p")
		{
			RestraintCondition = "partial";
			return 0.875;
		}
		else if (Restraint == "Enhanced" || Restraint == "enhanced" || Restraint == "E" || Restraint == "e")
		{
			RestraintCondition = "enhanced";
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
	std::exit(-1);
}

void DVLR::IsSlendernessOK(double& SR)
{
	if (SR >= 27)
	{
		std::cout << "A SR greater than 27 is outside the scope of BS 5628-1:2005." << std::endl << std::endl;
		std::cout << "Program Terminated." << std::endl << std::endl;
		SR = 0;
		std::exit(-1);
	}
	else
	{
		std::cout << "SR < 27 and therefore within the scope of BS 5628-1." << std::endl << std::endl;
	}
}

const double DVLR::GetSafetyFactor()
{
	std::cout << "Determine the Partial Safety Factor :" << std::endl;

	// Defines the table that the Partial Safety Factor is taken from
	double PSFTable[2][2] = { { 2.5, 2.8 },{ 3.1, 3.5 } };

	int ConstrControl = GetConstrControl();
	int ManufControl = GetManufControl();


	return PSFTable[ConstrControl][ManufControl];
}

// Future suggestion: as GetConstrControl & GetManufControl take in the same values and return the same values, call these as one function with different arguments.
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
			ConstructionControl = "normal";
			return 1;
		}
		else if (ConstrControl == "Special" || ConstrControl == "special" || ConstrControl == "S" || ConstrControl == "s")
		{
			ConstructionControl = "special";
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
	std::exit(-1);
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
			ManufactureControl = "normal";
			return 1;
		}
		else if (ManufControl == "Special" || ManufControl == "special" || ManufControl == "S" || ManufControl == "s")
		{
			ManufactureControl = "special";
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
	std::exit(-1);
}

Wult DVLR::GetUltLoad()
{
	std::cout << "Determine the ultimate loading:" << std::endl;
	GetLoads(); // populate our load array with eccentric and concentric dead and live loads
	GetUltLineLoad(Load);
	GetSelfWeight();
	Wult WultSpreadLoad = GetSpreadLoad();
	return WultSpreadLoad;
}

void DVLR::GetLoads()
{
	int x = 0; // access to load array

	std::cout << "Please enter the applied line loading at the top of the wall:" << std::endl;

	for (int i = 1; i <= 2; i++)
	{
		std::cout << "Leaf " << i << ",";
		for (int j = 1; j <= 2; j++)
		{
			if (j == 1) { std::cout << "\n  Eccentric, "; }
			else { std::cout << "  Concentric, "; }
			for (int k = 1; k <= 2; k++)
			{
				bool IsValid = true;
				do
				{
					if (k == 1) { std::cout << "\n\tDead load [kN/m]: "; }
					else { std::cout << "\tLive load [kN/m]: "; }
					std::cin >> Load[x];

					//Checks if user input a number
					if (!std::cin) // or if(cin.fail())
					{
						// user didn't input a number
						std::cin.clear(); // reset failbit
						std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //skip Invalid input
																							// next, request user reinput
						IsValid = false;
						if (k == 1) { std::cout << "Invalid input, please try again."; }
						else { std::cout << "Invalid input, please try again.\n"; }
					}
					else
					{
						IsValid = true;
					}
				} while (!IsValid);
				x++;
			}
		}
	}
	return;
}

const void DVLR::GetSelfWeight()
{
	std::cout << "\nPlease enter the self-weight of masonry," << std::endl;
	for (int i = 0; i <= 1; i++)
	{
		bool IsValid = true;
		do
		{
			std::cout << "  Leaf " << i + 1 << " [kN/m^3]: ";
			std::cin >> UnitWeight[i];
			//Checks if user input a number
			if (!std::cin) // or if(cin.fail())
			{
				// user didn't input a number
				std::cin.clear(); // reset failbit
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //skip Invalid input
																					// next, request user reinput
				IsValid = false;
				std::cout << "Invalid input, please try again." << std::endl;
			}
			else
			{
				IsValid = true;
			}

		} while (!IsValid);
	}
	PtFourH = 0.4*HWall / 1000;

	std::cout << "Therefore, the self-weight at 0.4H => " << PtFourH << "m," << std::endl;

	for (int i = 0; i <= 1; i++)
	{
		SelfWeight[i] = UnitWeight[i] * (PtFourH)*(TLeaf[i] / 1000);
		std::cout << "Leaf " << i + 1 << ": " << SelfWeight[i] << "kN/m" << std::endl;
	}
	return;
}

// TODO Get SW over opening
/*const Wult DVLR::GetSelfWeightOverOpening(double* SWeight, double HeightofWall, double OpeningHeight)
{
Wult SWOO;

SWOO.Leaf1 = SWeight[0] * (HeightofWall - OpeningHeight);
SWOO.Leaf2 = SWeight[1] * (HeightofWall - OpeningHeight);

return SWOO;
}*/

Wult DVLR::GetUltLineLoad(double* Load)
{
	LoadOverWall.Leaf1 = (1.4*(Load[0] + Load[2])) + (1.6*(Load[1] + Load[3]));
	LoadOverWall.Leaf2 = (1.4*(Load[4] + Load[6])) + (1.6*(Load[5] + Load[7]));
	/// Temporary to view results
	std::cout << "Ultimate line load over wall, Leaf 1: " << LoadOverWall.Leaf1 << "kN/m" << std::endl;
	std::cout << "Ultimate Line load over wall, Leaf 2: " << LoadOverWall.Leaf2 << "kN/m" << std::endl;
	return LoadOverWall;
}

Wult DVLR::GetSpreadLoad()
{
	Wult WLoad;

	std::cout << "\nConsider load concentrations due to openings:";

	// Get the opening widths, bearing lengths and load spreads.
	GetOpenings();
	// If both opening widths are not zero
	if (OpWidth[0] != 0 && OpWidth[1] != 0)
	{
		// and if both of the load spreads lap
		if (Spread[0] + Spread[1] >= L)
		{
			WLoad.Message = "Both load spreads lap.";
			WLoad.Message.append("\nConsidering the load concentration from both load spreads lapping.");
			std::cout << WLoad.Message << std::endl;
			WLoad.Leaf1 = GetDoubleLapLoad(LoadOverWall.Leaf1, SelfWeight[0], OpWidth);
			WLoad.Leaf2 = GetDoubleLapLoad(LoadOverWall.Leaf2, SelfWeight[1], OpWidth);
		}
		// else return the greatest of the singly lapped loads
		else
		{
			WLoad.Message = "Load spreads do not lap.";
			WLoad.Message.append( "\nConsidering the load concentration from the greatest load concentration.");
			std::cout << WLoad.Message << std::endl;
			WLoad.Leaf1 = GetSingleLapLoad(LoadOverWall.Leaf1, SelfWeight[0], OpWidth);
			WLoad.Leaf2 = GetSingleLapLoad(LoadOverWall.Leaf2, SelfWeight[1], OpWidth);
		}
	}
	// if one width is not zero, return the singly lapped load
	else if (OpWidth[0] != 0 || OpWidth[1] != 0)
	{
		WLoad.Message = "Considering a load concentration from the spread load.";
		std::cout << WLoad.Message << std::endl;
		WLoad.Leaf1 = GetSingleLapLoad(LoadOverWall.Leaf1, SelfWeight[0], OpWidth);
		WLoad.Leaf2 = GetSingleLapLoad(LoadOverWall.Leaf2, SelfWeight[1], OpWidth);
	}
	// else both openings must be zero, simply calculate the ultimate line load
	else
	{
		WLoad.Message = "No load spread due to concentrated loads.";
		WLoad.Message.append( "\nConsidering the ultimate load at 0.4H from the top of the wall.");
		WLoad.Leaf1 = (1.4*SelfWeight[0]) + LoadOverWall.Leaf1;
		WLoad.Leaf2 = (1.4*SelfWeight[1]) + LoadOverWall.Leaf2;
	}
	return WLoad;
}

// Future suggestion: create an struct for openings including width, height and bearing length and simply declare the openings as an array i.e. Opening[2] for looping purposes
void DVLR::GetOpenings()
{
	bool IsValid = true;
	do {
		std::cout << "\nPlease enter the length of wall considered, L [mm]:  ";
		std::cin >> L;
		//Checks if user input a number
		if (!std::cin) // or if(cin.fail())
		{
			// user didn't input a number
			std::cin.clear(); // reset failbit
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //skip Invalid input
																				// next, request user reinput
			IsValid = false;
			std::cout << "Invalid input, please try again.";
		}
		else
		{
			IsValid = true;
		}
	} while (!IsValid);

	for (int i = 0; i <= 1; i++)
	{
		bool IsOpValid = true;
		do
		{
			std::cout << "Please enter the width of opening " << i + 1 << " [mm]:  ";
			std::cin >> OpWidth[i];
			//Checks if user input a number
			if (!std::cin) // or if(cin.fail())
			{
				// user didn't input a number
				std::cin.clear(); // reset failbit
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //skip Invalid input
																					// next, request user reinput
				IsOpValid = false;
				std::cout << "Invalid input, please try again." << std::endl;
			}
			else
			{
				IsOpValid = true;
			}
		} while (!IsOpValid);

		if (OpWidth[i] != 0)
		{
			bool IsBValid = true;
			do
			{
				std::cout << "Please enter the bearing length of the member forming opening " << i + 1 << " [mm]:  ";
				std::cin >> BLength[i];
				//Checks if user input a number
				if (!std::cin) // or if(cin.fail())
				{
					// user didn't input a number
					std::cin.clear(); // reset failbit
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //skip Invalid input
																						// next, request user reinput
					IsBValid = false;
					std::cout << "Invalid input, please try again." << std::endl;
				}
				else
				{
					IsBValid = true;
				}
			} while (!IsBValid);
				/*std::cout << "Please enter the height to the top of the opening " << i + 1 << " [mm]:  ";
				std::cin >> OpHeight[i];*/
			
			Spread[i] = { BLength[i] + (PtFourH * 1000) };
			//SelfWeightOverOpening[i] = GetSelfWeightOverOpening(SelfWeight, HWall, OpHeight[i]);
		}
	}
	return;
}

const double DVLR::GetSingleLapLoad(double UltLoad, double Selfweight, double* OpenWidth)
{
	if (OpenWidth[0] >= OpenWidth[1])
	{
		return ((UltLoad + (1.4*Selfweight)) + ((UltLoad*(OpenWidth[0] / 1000)) / (2 * (Spread[0] / 1000))));
	}
	else
	{
		return ((UltLoad + (1.4*Selfweight)) + (UltLoad *(OpenWidth[1] / 1000)) / (2 * (Spread[1] / 1000)));
	}
}

const double DVLR::GetDoubleLapLoad(double UltLoad, double Selfweight, double* OpenWidth)
{
	return (UltLoad + (1.4*Selfweight)) + (UltLoad *(OpenWidth[1] / 1000)) / (2 * (Spread[1] / 1000))
		+ (UltLoad *(OpenWidth[0] / 1000)) / (2 * (Spread[0] / 1000));
}

const Wult DVLR::GetBeta()
{
	std::cout << "\nConsider a load capacity reduction due to eccentricity & slenderness:" << std::endl;
	Wult B;
	int x = 0;
	for (int i = 0; i <= 1; i++)
	{
		Ex[i] = GetEx(Load[x], Load[x + 1], Load[x + 2], Load[x + 3], TLeaf[i]);
		x += 4;
		std::cout << "Loaded eccentricity of Leaf " << i + 1 << ": " << Ex[i] << " mm" << std::endl;
	}
	std::cout << std::endl;
	// Accidental Eccentricity
	for (int i = 0; i <= 1; i++)
	{
		Ea[i] = TLeaf[i] * (((SR*SR) / 2400) - 0.015);
		std::cout << "Accidental eccentricity of Leaf " << i + 1 << ": " << Ea[i] << " mm" << std::endl;
	}
	std::cout << std::endl;
	// Total Eccentricity at 04H from the top of the wall
	for (int i = 0; i <= 1; i++)
	{
		Et[i] = (0.6*Ex[i]) + Ea[i];
		std::cout << "Total Eccentricity at 0.4H from the top of Leaf " << i + 1 << ": " << Et[i] << " mm" << std::endl;
	}
	std::cout << std::endl;
	// Maximum Eccentricity
	for (int i = 0; i <= 1; i++)
	{
		Em[i] = (Ex[i] > Et[i]) ? Ex[i] : Et[i];
		std::cout << "Maximum Eccentricity of Leaf " << i + 1 << ": " << Em[i] << " mm" << std::endl;
	}
	std::cout << std::endl;

	B.Leaf1 = 1.1*(1 - ((2 * Em[0]) / TLeaf[0]));
	B.Leaf2 = 1.1*(1 - ((2 * Em[1]) / TLeaf[1]));

	return B;
}

const double DVLR::GetEx(double EccDL, double EccLL, double CncDL, double CncLL, double LeafThickness)
{
	double Ecc = ((EccDL + EccLL)*(LeafThickness / 6)) / (EccDL + EccLL + CncDL + CncLL); // e = M/N
	double EccMin = 0.05*LeafThickness;
	return (Ecc > EccMin) ? Ecc : EccMin;
}

const Wult DVLR::GetSmallAreaFactor()
{
	std::cout << std::endl << "Determine Small Area Factor:" << std::endl;
	SAF.Message = "Area of Leaf 1 = ";
	SAF.Leaf1 = GetSAF(TLeaf[0], L);
	std::string SAFMessageLeaf1 = SAF.Message;
	std::cout << SAF.Leaf1 << std::endl << std::endl;

	SAF.Message = "Area of Leaf 2 = ";
	SAF.Leaf2 = GetSAF(TLeaf[1], L);
	std::string SAFMessageLeaf2 = SAF.Message;
	std::cout << SAF.Leaf2 << std::endl << std::endl;
	
	return SAF;
}

const double DVLR::GetSAF(double& LeafThickness, double& WallLength)
{
	double Area = (LeafThickness/1000)*(WallLength/1000);

	SAF.Message.append(std::to_string(Area));

	if (Area < 0.2)
	{
		SAF.Message.append("m^2 < 0.2m^2, \nTherefore we will need to consider a small area factor.");
		SAF.Message.append("\nSAF = 0.7 + (1.5*Area) = ");
		std::cout << SAF.Message;
		return 0.7 + (1.5*Area);
	}
	else
	{
		SAF.Message.append("m^2 > 0.2m^2 and hence small area factors are ignored.");
		SAF.Message.append("\nSAF = ");
		std::cout << SAF.Message;
		return 1;
	}
}

const double DVLR::GetMinFk(double& Ym, double& Wult, double& B, double& SAF, double& t)
{
	return (Ym*Wult)/(B*SAF*t);
}
