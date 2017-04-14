// MasonryChecker.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"		// Turn on in Visual Studio
#include "MasonryChecker.h"
#include <iostream>
#include <string> // Allows strings to be used
#include <iomanip> // std::setprecision -> allows us to reduce the decimal places in the string stream

double GetValidNumber()
{
	bool IsValid;
	double Num;
	do
	{
		std::cin >> Num;
		//Checks if user input a number
		if (!std::cin) // or if(cin.fail())
		{
			// user didn't input a number
			std::cin.clear(); // reset failbit
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //skip Invalid input
																				// next, request user reinput
			IsValid = false;
			std::cout << "Invalid input, please try again:  ";
		}
		else
		{
			IsValid = true;
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear the input Buffer
		}
	} while (!IsValid);
	return Num;
}

bool IsValidCase(double UserSelection) // Selection for custom eccentricity cases
{
	if(UserSelection >= 1 || UserSelection <= 5)
	{
		return true;
	}
	std::cout << "Please enter a valid case:  ";
	return false;
}

// TODO - Pass whether 1 or 2 leaves? or create 2 wall objects
DVLR::DVLR() { /* Default Constructor*/ }

const double DVLR::GetSlenderness()
{
	std::cout << "Determine the Slenderness Ratio:" << std::endl;

	Teff = GetTeff(); // Get the effective thickness
	std::cout << "Therefore, effective thickness, Teff = " << TeffEquation;
	std::cout << Teff << "mm" << std::endl;

	Heff = GetHeff(); // Get the effective height
	std::cout << "Therefore, effective height, Heff = " << RestraintFactor;
	std::cout << " * " << HWall << " = " << Heff << "mm" << std::endl;

	// Get the opening dimensions
	GetOpenings();

	return Heff / Teff;
}

const double DVLR::GetTeff()
{
	// Ask the user for the thickness of each leaf
	for (int i = 0; i <= 1; i++) // Hard number of 2 used as this will always check a cavity wall!
	{
			std::cout << "Enter the thickness of leaf " << i + 1 << ", t" << i + 1 << " [mm]:  ";
			TLeaf[i] = GetValidNumber();
	}

	// Calculates the effective thicknesses
	auto t3 = (2 * (TLeaf[0] + TLeaf[1])) / 3; // Effective thickness of two leaves
	auto t4 = (TLeaf[0] >= TLeaf[1]) ? TLeaf[0] : TLeaf[1]; // Greatest thickness out of both leaves

	// Returns the greatest effective thickness
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
	std::cout << "Please input the height of the Wall [mm]:  ";
	HWall = GetValidNumber();

	RestraintFactor = GetRestraint();

	return HWall * RestraintFactor;
}

const double DVLR::GetRestraint()
{
	std::string Restraint;
	bool IsValid;

	do
	{
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
	}
	while (!IsValid);

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
	double PSFTable[2][2] = {{2.5, 2.8},{3.1, 3.5}};

	std::cout << "Please input normal or special CONSTRUCTION control (N/S):  ";
	int ConstrControl = GetPSFControl();
	(ConstrControl = 1) ? ConstructionControl = "normal" : ConstructionControl = "special";
	std::cout << "Please input normal or special MANUFACTURE control (N/S):  ";
	int ManufControl = GetPSFControl();
	(ManufControl = 1) ? ManufactureControl = "normal" : ManufactureControl = "special";

	return PSFTable[ConstrControl][ManufControl];
}

int DVLR::GetPSFControl()
{
	std::string FormOfControl = "";
	bool IsValid; // Validity of input

	do
	{
		getline(std::cin, FormOfControl);

		if (FormOfControl == "Normal" || FormOfControl == "normal" || FormOfControl == "N" || FormOfControl == "n")
		{
			return 1;
		}
		if (FormOfControl == "Special" || FormOfControl == "special" || FormOfControl == "S" || FormOfControl == "s")
		{
			return 0;
		}
		std::cout << "Please enter a valid input:  ";
		IsValid = false;
	}
	while (!IsValid);
	std::exit(-1);
}

TwoLeafStruct DVLR::GetUltLoad()
{
	std::cout << "Determine the ultimate loading:" << std::endl;
	GetLoads(); // populate our load array with eccentric and concentric dead and live loads
	GetUltLineLoad(Load);
	GetSelfWeight();
	TwoLeafStruct WultSpreadLoad = GetSpreadLoad();
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
				bool IsValid;
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
			std::cout << "  Leaf " << i + 1 << " [kN/m^3]: ";
			UnitWeight[i] = GetValidNumber();
	}
	PtFourH = 0.4 * HWall / 1000;
	std::cout << "Therefore, the self-weight at 0.4H => " << PtFourH << "m," << std::endl;

	for (int i = 0; i <= 1; i++)
	{
		SelfWeight[i] = UnitWeight[i] * (PtFourH) * (TLeaf[i] / 1000);
		std::cout << "Leaf " << i + 1 << ": " << SelfWeight[i] << "kN/m" << std::endl;
		// Display Self weight of masonry above opening
		for(int j = 0 ; j <= 1 ; j++)
		{
			if (Opening[j].IsOpening)
			{
				SelfWeightOverOpening[i] = GetSelfWeightOverOpening(UnitWeight, TLeaf, HWall, Opening[i].Height, i);
			}
		}
	}
	return;
}

const TwoLeafStruct DVLR::GetSelfWeightOverOpening(double* SWeight, double* LeafThickness, double HeightofWall, double OpeningHeight, int OpeningNumber)
{
	// Self weight Over Opening
	TwoLeafStruct SWOO;

	SWOO.Leaf1 = SWeight[0] * (LeafThickness[0] / 1000) * (HeightofWall - OpeningHeight) / 1000;
	SWOO.Leaf2 = SWeight[1] * (LeafThickness[1] / 1000) * (HeightofWall - OpeningHeight) / 1000;

	std::cout << "Self weight of masonry over opening " << OpeningNumber + 1 << " = SelfWeight * (HeightofWall - OpeningHeight)" << std::endl;
	std::cout << "SWOO, Leaf 1 = " << SWeight[0] << "kN/m^3 * " << LeafThickness[0] / 1000 << "m * (" << HeightofWall / 1000 << "m - " << OpeningHeight / 1000 << "m) = " << SWOO.Leaf1 << "kN/m" << std::endl;
	std::cout << "SWOO, Leaf 2 = " << SWeight[1] << "kN/m^3 * " << LeafThickness[1] / 1000 << "m * (" << HeightofWall / 1000 << "m - " << OpeningHeight / 1000 << "m) = " << SWOO.Leaf2 << "kN/m" << std::endl;

	return SWOO;
}

TwoLeafStruct DVLR::GetUltLineLoad(double* Load)
{
	LoadOverWall.Leaf1 = (1.4 * (Load[0] + Load[2])) + (1.6 * (Load[1] + Load[3]));
	LoadOverWall.Leaf2 = (1.4 * (Load[4] + Load[6])) + (1.6 * (Load[5] + Load[7]));
	/// View results
	std::cout << "Ultimate line load over wall, Leaf 1: " << LoadOverWall.Leaf1 << "kN/m" << std::endl;
	std::cout << "Ultimate Line load over wall, Leaf 2: " << LoadOverWall.Leaf2 << "kN/m" << std::endl;
	return LoadOverWall;
}

TwoLeafStruct DVLR::GetSpreadLoad()
{
	TwoLeafStruct WLoad;
	double SwooLF1[2] = { SelfWeightOverOpening[0].Leaf1 ,SelfWeightOverOpening[1].Leaf1 };
	double SwooLF2[2] = { SelfWeightOverOpening[0].Leaf2 ,SelfWeightOverOpening[1].Leaf2 };

	// If both opening widths are not zero
	if (Opening[0].IsOpening && Opening[1].IsOpening)
	{
		// and if both of the load spreads lap
		if (Opening[0].Spread + Opening[1].Spread >= L)
		{
			WLoad.Message = "Both load spreads lap.";
			WLoad.Message.append("\nConsidering the load concentration from both load spreads lapping.");
			std::cout << WLoad.Message << std::endl;
			// Opening array passed as both are required per leaf calculation!
			WLoad.Leaf1 = GetDoubleLapLoad(LoadOverWall.Leaf1, SelfWeight[0], Opening[0].Width, Opening[1].Width, Opening[0].Spread, Opening[1].Spread, SelfWeightOverOpening[0].Leaf1, SelfWeightOverOpening[1].Leaf1);
			WLoad.Leaf2 = GetDoubleLapLoad(LoadOverWall.Leaf2, SelfWeight[1], Opening[0].Width, Opening[1].Width, Opening[0].Spread, Opening[1].Spread, SelfWeightOverOpening[0].Leaf2, SelfWeightOverOpening[1].Leaf2);
			// Tells the print output which case to print i.e. No load spread
			SpreadCaseStatus = SpreadCase::DblLoadSpreadLaps;
		}
		// else return the greatest of the singly lapped loads
		else
		{
			WLoad.Message = "Load spreads do not lap.";
			WLoad.Message.append("\nConsidering the load concentration from the greatest load concentration.");
			std::cout << WLoad.Message << std::endl;
			WLoad.Leaf1 = GetSingleLapLoad(LoadOverWall.Leaf1, SelfWeight[0], &Opening[2], SelfWeightOverOpening[0].Leaf1, SelfWeightOverOpening[1].Leaf1);
			WLoad.Leaf2 = GetSingleLapLoad(LoadOverWall.Leaf2, SelfWeight[1], &Opening[2], SelfWeightOverOpening[0].Leaf2, SelfWeightOverOpening[1].Leaf2);
			// Tells the print output which case to print i.e. No load spread
			SpreadCaseStatus = SpreadCase::DblLoadSpreadDoesNOTLap;
		}
	}
	// if one width is not zero, return the singly lapped load
	else if (Opening[0].IsOpening || Opening[1].IsOpening)
	{
		WLoad.Message = "Considering a load concentration from the spread load.";
		std::cout << WLoad.Message << std::endl;

		WLoad.Leaf1 = GetSingleLapLoad(LoadOverWall.Leaf1, SelfWeight[0], &Opening[2], SelfWeightOverOpening[0].Leaf1, SelfWeightOverOpening[1].Leaf1);
		WLoad.Leaf2 = GetSingleLapLoad(LoadOverWall.Leaf2, SelfWeight[1], &Opening[2], SelfWeightOverOpening[0].Leaf2, SelfWeightOverOpening[1].Leaf2);
		// Tells the print output which case to print i.e. No load spread
		SpreadCaseStatus = SpreadCase::SglLoadSpread;
	}
	// else both openings must be zero, simply calculate the ultimate line load
	else
	{
		WLoad.Message = "No load spread due to concentrated loads.";
		WLoad.Message.append("\nConsidering the ultimate load at 0.4H from the top of the wall.");
		WLoad.Leaf1 = (1.4 * SelfWeight[0]) + LoadOverWall.Leaf1;
		WLoad.Leaf2 = (1.4 * SelfWeight[1]) + LoadOverWall.Leaf2;
		// Tells the print output which case to print i.e. No load spread
		SpreadCaseStatus = SpreadCase::NoLoadSpread;
	}
	return WLoad;
}

void DVLR::GetOpenings()
{
	std::cout << "\nPlease enter the length of wall considered, L [mm]:  ";
	L = GetValidNumber();

	for (int i = 0; i <= 1; i++)
	{
		std::cout << "Please enter the width of opening " << i + 1 << " [mm]:  ";
		Opening[i].Width = GetValidNumber();
			
		// If the opening width is not zero
		if (Opening[i].Width != 0)
		{
			// ..There must be an opening present
			Opening[i].IsOpening = true;
			// ..Get the bearing length...
			std::cout << "Please enter the bearing/pad stone length of the member forming opening " << i + 1 << " [mm]:  ";
			Opening[i].BLength = GetValidNumber();
			// .. and get the height to the top of the opening
			std::cout << "Please enter the height to the top of the opening " << i + 1 << " [mm]:  ";
			Opening[i].Height = GetValidNumber();

			double Pt6H = 0.6*HWall; // millimeters
			/// Load Spread Length
			Opening[i].Spread = SpreadLength(Opening[i].BLength, Pt6H, L, Opening[i].Height, i);
		}
	}
	return;
}

// TODO - Move to TxtOutput.cpp ???
const double DVLR::SpreadLength(double BearingLength, double Pt6H, double Length, double OpeningHieght, int i)
{
	double Spread;
	if ((OpeningHieght - Pt6H) <= 0)
	{
		Spread = 0;
	}
	else
	{
		// millimeters
		Spread = BearingLength + (OpeningHieght - Pt6H);
	}

	std::string SpreadLength = ConvertToString( Spread, 2);
	std::string WallLength = ConvertToString(Length, 2);
	std::string PointSixH = ConvertToString(Pt6H, 2);
	std::string OpHieght = ConvertToString(OpeningHieght, 2);
	std::string Blength = ConvertToString(BearingLength, 2);

	SpreadLengthMessage[i] = Blength + "mm + (" + OpHieght + "mm - " + PointSixH + "mm) = ";

	if (Spread > Length)
	{
		SpreadLengthMessage[i].append(SpreadLength + "mm > " + WallLength + "mm");
		return Length;
	}
		SpreadLengthMessage[i].append(SpreadLength + "mm < " + WallLength + "mm");
		return Spread;
}

const double DVLR::GetSingleLapLoad(double UltLoad, double Selfweight, StructuralOpenings OpenWidth[2], double SelfWeightOverOpening1, double SelfWeightOverOpening2)
{
	auto Part1 = UltLoad + (1.4 * Selfweight);
	auto Part2 = (UltLoad + 1.4*SelfWeightOverOpening1) * (OpenWidth[0].Width / 1000) / (2 * (Opening[0].Spread / 1000));
	auto Part3 = (UltLoad + 1.4*SelfWeightOverOpening2) * (OpenWidth[1].Width / 1000) / (2 * (Opening[1].Spread / 1000));

	if (OpenWidth[0].Width >= OpenWidth[1].Width)
	{
		return Part1 + Part2;
	}
	return Part1 + Part3;
}

const double DVLR::GetDoubleLapLoad(double UltLoad, double Selfweight, double OpenWidth1, double OpenWidth2, double Spread1, double Spread2, double SelfWeightOverOpening1, double SelfWeightOverOpening2)
{
	auto Part1 = UltLoad + 1.4 * Selfweight;
	auto Part2 = (UltLoad + 1.4*SelfWeightOverOpening2) * (OpenWidth2 / 1000) / (2 * (Spread2 / 1000));
	auto Part3 = (UltLoad + 1.4*SelfWeightOverOpening1) * (OpenWidth1 / 1000) / (2 * (Spread1 / 1000));
	std::cout << "Opening Width1: " << OpenWidth1 << " ; Opening Width2: " << OpenWidth2 << std::endl;
	std::cout << "Opening Spread1: " << Spread1 << " ; Opening Spread2: " << Spread2 << std::endl;
	return Part1 + Part2 + Part3;
}

const TwoLeafStruct DVLR::GetBeta()
{
	std::cout << "\nConsider a load capacity reduction due to eccentricity & slenderness:" << std::endl;
	TwoLeafStruct B;

	auto UseDefaultEccentricity = IsEccentricityDefault();

	if (!UseDefaultEccentricity)
	{
		std::cout << std::endl;
		for (int i = 0; i <= 1; i++)
		{
			Eccentricity[i] = GetUserEccentricity(TLeaf[i], i);
			std::cout << "Custom Eccentricity: " << Eccentricity[i] << "mm" << std::endl << std::endl;
		}
	}
	else
	{
		for (int i = 0; i <= 1; i++)
		{
			Eccentricity[i] = TLeaf[i] / 6;
		}
	}

	int x = 0;
	for (int i = 0; i <= 1; i++)
	{
		Ex[i] = GetEx(Load[x], Load[x + 1], Load[x + 2], Load[x + 3], TLeaf[i], Eccentricity[i]);
		x += 4;
		std::cout << "Resultant eccentricity of Leaf " << i + 1 << ": " << Ex[i] << " mm" << std::endl;
	}
	std::cout << std::endl;

	// Accidental Eccentricity
	if(SR <= 6)
	{
		Ea[0] = 0;
		Ea[1] = 0;
	}
	else
	{
		for (int i = 0; i <= 1; i++)
		{
			Ea[i] = TLeaf[i] * (((SR * SR) / 2400) - 0.015);
			std::cout << "Accidental eccentricity of Leaf " << i + 1 << ": " << Ea[i] << " mm" << std::endl;
		}
		std::cout << std::endl;
	}

	// Total Eccentricity at 04H from the top of the wall
	for (int i = 0; i <= 1; i++)
	{
		Et[i] = (0.6 * Ex[i]) + Ea[i];
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

	B.Leaf1 = 1.1 * (1 - ((2 * Em[0]) / TLeaf[0]));
	B.Leaf2 = 1.1 * (1 - ((2 * Em[1]) / TLeaf[1]));

	return B;
}

bool DVLR::IsEccentricityDefault() const
{
	std::string IsEccentricityDefault = "";
	bool IsValid; // Validity of input

	do
	{
		std::cout << "Would you like to use the default eccentricity of t/6? " << std::endl;
		std::cout << "Note that different values per leaf can be selected. (yes/no/help):  ";
		getline(std::cin, IsEccentricityDefault);

		if (IsEccentricityDefault == "Yes" || IsEccentricityDefault == "yes" || IsEccentricityDefault == "Y" || IsEccentricityDefault == "y")
		{
			return true;
		}
		if (IsEccentricityDefault == "No" || IsEccentricityDefault == "no" || IsEccentricityDefault == "N" || IsEccentricityDefault == "n")
		{
			return false;
		}
		if (IsEccentricityDefault == "Help" || IsEccentricityDefault == "help" || IsEccentricityDefault == "H" || IsEccentricityDefault == "h")
		{
			return false;
		}
			std::cout << "Please enter a valid input." << std::endl;
			IsValid = false;
	}
	while (!IsValid);
	return false;
}

const double DVLR::GetUserEccentricity(double Thickness, int Leaf)
{
	double Custom;
	double NotFullBearing;

	do
	{
		std::cout << "Custom Eccentricity to Leaf " << Leaf + 1 << "." << std::endl;
		std::cout << "Please enter the number from below which represents the desired case: " << std::endl;
		std::cout << "1. Default: t/6 recommended for simply supported joists and slabs bearing onto the full width of the wall." << std::endl;
		std::cout << "2. t/3 recommended for slabs or joists spanning continuously over the wall." << std::endl;
		std::cout << "3. t/2 recommended for joists on hangars or similar condition." << std::endl;
		std::cout << "4. t/2-l/3 for simply supported joists and slabs NOT bearing onto the full width of the wall." << std::endl;
		std::cout << "5. A custom value. Note that entering a zero value will result in a minimum eccentricity of 0.05t being used." << std::endl;
		std::cout << "Case:  ";

		int UserSelection = GetValidNumber();
			
		// Assuming input is a number, pass it into the switch. Breaks ignored due to returns. i.e. unreachable code.
		// Enums generated for text generation purposes
		switch (UserSelection)
		{
		case 1:
			Selection = UserEccentricity::FullBearing;
			return (Thickness / 6);
		case 2:
			Selection = UserEccentricity::Continuous;
			return (Thickness / 3);
		case 3:
			Selection = UserEccentricity::Hangars;
			return (Thickness / 2);
		case 4:
			Selection = UserEccentricity::NotFullBearing;
			NotFullBearing = CustomBearing(Thickness, Leaf);
			return NotFullBearing;
		case 5:
			Selection = UserEccentricity::Custom;
			Custom = GetCustomEccentricity(Thickness, Leaf);
			return Custom;
		default:
			// If input is not a valid case, ask the user for a valid case.
			std::cout << "Please enter a valid input.\n" << std::endl;
			Selection = UserEccentricity::Invalid_Status;
		}
	}
	while (Selection == UserEccentricity::Invalid_Status);
	return 0.0;
}

const double DVLR::CustomBearing(double Thickness, int Leaf)
{
	bool IsValid;
	do
	{
		std::cout << "Note that value must be less than the leaf thickness and greater than zero." << std::endl;
		std::cout << "Enter the custom bearing length [mm]:  ";
		std::cin >> BearingLength[Leaf];
		// 'Safety net' in case the user does not enter a number it does not crash the program!
		if (!std::cin || BearingLength[Leaf] > Thickness || BearingLength[Leaf] < 0)// or if(cin.fail())
		{
			// user didn't input a number
			std::cin.clear(); // reset failbit
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //skip Invalid input
			// next, request user reinput
			IsValid = false;
			std::cout << "Invalid input, please try again.\n";
		}
		else
		{
			IsValid = true;
		}
	}
	while (!IsValid);

	return (Thickness / 2) - (BearingLength[Leaf] / 3);
}

const double DVLR::GetCustomEccentricity(double Thickness, int Leaf)
{
	bool IsValid;
	do
	{
		std::cout << "Note that value must be less than t/2 and greater than zero." << std::endl;
		std::cout << "Enter the custom eccentricity [mm]:  ";
		std::cin >> CustomEccentricity[Leaf];
		// 'Safety net' in case the user does not enter a number it does not crash the program!
		if (!std::cin || CustomEccentricity[Leaf] > (Thickness / 2) || CustomEccentricity[Leaf] < 0)// or if(cin.fail())
		{
			// user didn't input a number
			std::cin.clear(); // reset failbit
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //skip Invalid input
			// next, request user reinput
			IsValid = false;
			std::cout << "Invalid input, please try again [mm]:  ";
		}
		else
		{
			IsValid = true;
		}
	}
	while (!IsValid);

	return CustomEccentricity[Leaf];
}

const double DVLR::GetEx(double EccDL, double EccLL, double CncDL, double CncLL, double Thickness, double Eccentricity)
{
	double Ecc = ((EccDL + EccLL) * Eccentricity) / (EccDL + EccLL + CncDL + CncLL); // e = M/N
	double MinEcc = 0.05 * Thickness;
	return (Ecc > MinEcc) ? Ecc : MinEcc;
}

const TwoLeafStruct DVLR::GetSmallAreaFactor()
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
	double Area = (LeafThickness / 1000) * (WallLength / 1000);

	SAF.Message.append(std::to_string(Area));

	if (Area < 0.2)
	{
		SAF.Message.append("m^2 < 0.2m^2, \nTherefore we will need to consider a small area factor.");
		SAF.Message.append("\nSAF = 0.7 + (1.5*Area) = ");
		std::cout << SAF.Message;
		return 0.7 + (1.5 * Area);
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
	return (Ym * Wult) / (B * SAF * t);
}

const TwoLeafStruct DVLR::CheckLintelBearing(double& BLength, double LeafThickness[2], TwoLeafStruct LineLoadOverWall, TwoLeafStruct SWOO, double& OpLength, double& SafetyFactor)
{
	TwoLeafStruct LoadAtSupport;
	TwoLeafStruct MinBearingStrength;
	// Get the bearing coefficient based on the bearing length and leaf thickness
	MinBearCoeff.Leaf1 = GetMinBearCoeff(BLength, LeafThickness[0]);
	MinBearCoeff.Leaf2 = GetMinBearCoeff(BLength, LeafThickness[1]);

	// Get the Reaction at the support
	LoadAtSupport.Leaf1 = GetLoadAtSupport(LineLoadOverWall.Leaf1, SWOO.Leaf1, OpLength, BLength);
	LoadAtSupport.Leaf2 = GetLoadAtSupport(LineLoadOverWall.Leaf2, SWOO.Leaf2, OpLength, BLength);

	MinBearingStrength.Leaf1 = (LoadAtSupport.Leaf1 * SafetyFactor *  1000) / (MinBearCoeff.Leaf1 * LeafThickness[0] * BLength);
	MinBearingStrength.Leaf2 = (LoadAtSupport.Leaf2 * SafetyFactor * 1000) / (MinBearCoeff.Leaf2 * LeafThickness[1] * BLength);

	return MinBearingStrength;
}

const double DVLR::GetMinBearCoeff(double& BLength, double LeafThickness)
{
		if (BLength < (2 * LeafThickness))
		{
			return 1.50;
		}
		if (BLength < (3 * LeafThickness))
		{
			return 1.25;
		}
	// Default	
	return 1.00;
}

const double DVLR::GetLoadAtSupport(double Wult, double SWOverOpening, double OpLength, double BLength) 
{return (Wult+SWOverOpening)*(((OpLength/1000)*0.5)+(BLength/1000));}
