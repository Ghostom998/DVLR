#include "stdafx.h"		// Turn on in Visual Studio
#include "MasonryChecker.h"
#include <iostream>
#include <string>		// Allows strings to be used
#include <iomanip>      // std::setprecision
#include <sstream>		// String Stream: allows numbers to be cast as strings (i.e. to_string() )
#include <fstream>		// File Stream: allows input/output to file


const int DVLR::PrintToFile()
{
	// Take a name for the file
	std::string FileName = "";
	std::cout << "Please enter the file name [*.txt]:  ";
	std::getline(std::cin, FileName);

	// Creates a .txt file with the user specified name
	std::ofstream writer(FileName + ".txt");
	// If error opening the file...
	if (!writer)
	{
		// ... Display an error message and close the program in error.
		std::cout << "Error opening the file." << std::endl;
		std::cout << "Please check that the file does not already exist or is open." << std::endl;
		std::exit(-1);
	}
	std::cout << "The file will be saved in the location of the executable *.exe file" << std::endl;

	// Get introduction text...
	std::string Intro = PrintIntro(FileName);
	// ...then write introduction to file.
	writer << Intro << std::endl;

	// Build slenderness text
	std::string SlenderNess = PrintSlenderness();
	// Write slenderness text to file
	writer << SlenderNess << std::endl;

	// Build PSF text
	std::string PartialSafetyFactor = PrintPSF();
	// Write PSF text to file
	writer << PartialSafetyFactor << std::endl;

	// Build loadings text
	std::string Loadings = PrintLoadings();
	// Write loadings text to file
	writer << Loadings << std::endl;

	// Build Eccentricity text
	std::string Eccentricity = PrintEccentricity();
	// Write PSF text to file
	writer << Eccentricity << std::endl;

	// Build MinFk text
	std::string MinFk = PrintMinFk();
	// Write MinFk text to file
	writer << MinFk << std::endl;

	// close filestream
	writer.close();

	// If write  is not successfull
	if (!writer)
	{
		// ... Display an error message and close the program in error.
		std::cout << "Error writing to file." << std::endl;
		std::cout << "Please check that the file does not already exist or is open." << std::endl;
		std::exit(-1);
	}
	else
	{
		// Assumes that if there were no errors, then the file was written succesfully
		std::cout << "\nThe program has written to the file without error." << std::endl;
		return 0;
	}
}

const std::string DVLR::PrintIntro(std::string NameOfFile)
{
	std::string Introduction = "Masonry Design under Vertical Load.";
	Introduction.append("\n\nConsider the vertical load resistance of a cavity masonry wall in accordance with BS 5628-1:2005.");
	Introduction.append("\nWall Referenced: " + NameOfFile);
	return Introduction;
}

const std::string DVLR::PrintSlenderness()
{
	// Setup up string stream variables to round the inputs to 2 decimal places and convert numbers to strings
	std::ostringstream t1;
	t1 << std::fixed << std::setprecision(2) << TLeaf[0];
	std::ostringstream t2;
	t2 << std::fixed << std::setprecision(2) << TLeaf[1];
	std::ostringstream H;
	H << std::fixed << std::setprecision(2) << HWall;
	std::ostringstream RFactor;
	RFactor << std::fixed << std::setprecision(2) << RestraintFactor;
	std::ostringstream EffectiveThickness;
	EffectiveThickness << std::fixed << std::setprecision(2) << Teff;
	std::ostringstream EffectiveHeight;
	EffectiveHeight << std::fixed << std::setprecision(2) << Heff;
	std::ostringstream SlendernessRatio;
	SlendernessRatio << std::fixed << std::setprecision(2) << SR;

	std::string Slenderness = "\nConsider the Slenderness Ratio:";
	Slenderness.append("\nThickness of Leaf 1, t1: " + t1.str() + "mm");
	Slenderness.append("\nThickness of Leaf 2, t2: " + t2.str() + "mm");
	Slenderness.append("\nHeight of Wall, H: " + H.str() + "mm");
	Slenderness.append("\nRestraint condition at the top of the wall is " + RestraintCondition + ".");
	Slenderness.append("\nRestraint factor to clause 28.3.1.1., RF: " + RFactor.str());

	Slenderness.append("\nEffective Thickness, Teff = " + TeffEquation + EffectiveThickness.str() + "mm");
	Slenderness.append("\nEffective Height, Heff = RF*H = " + RFactor.str() + "*" + H.str() + " = " + EffectiveHeight.str() + "mm");
	Slenderness.append("\nHence,\nSlenderness Ratio, SR = Heff/Teff = " + EffectiveHeight.str() + "/" + EffectiveThickness.str());
	Slenderness.append(" = " + SlendernessRatio.str() + " [-]");

	return Slenderness;
}

const std::string DVLR::PrintPSF()
{
	std::ostringstream SafetyFactor;
	SafetyFactor << std::fixed << std::setprecision(1) << PSF;

	std::string PartialSafetyFactor = "\nConsider a Partial Safety Factor:";
	PartialSafetyFactor.append("\nConstruction control is " + ConstructionControl + ".");
	PartialSafetyFactor.append("\nManufacture control is " + ManufactureControl + ".");
	PartialSafetyFactor.append("\nHence,\nPartial Safety from table 4a, PSF: " + SafetyFactor.str() + " [-]");
	return PartialSafetyFactor;
}

const std::string DVLR::PrintLoadings()
{
	std::ostringstream UnitweightLeaf1;
	UnitweightLeaf1 << std::fixed << std::setprecision(2) << UnitWeight[0];
	std::ostringstream UnitweightLeaf2;
	UnitweightLeaf2 << std::fixed << std::setprecision(2) << UnitWeight[1];
	std::ostringstream Length;
	Length << std::fixed << std::setprecision(2) << L;
	std::ostringstream OpeningWidth;
	std::ostringstream BearingLength;

	std::ostringstream PointFourH;
	PointFourH << std::fixed << std::setprecision(1) << PtFourH;

	std::string Loading = "\nConsider the characteristic loading at the top of the wall: ";
	std::string LoadAtTopOfWall = PrintLoadingsTopWall();
	Loading.append(LoadAtTopOfWall);
	
	Loading.append("\nSelfweight of Leaf 1, Ymas,Leaf1: " + UnitweightLeaf1.str() + "kN/m^3");
	Loading.append("\nSelfweight of Leaf 2, Ymas,Leaf2: " + UnitweightLeaf2.str() + "kN/m^3");
	Loading.append("\nLength of Wall, L: " + Length.str() + "mm");

	for (int i = 0; i <= 1; i++)
	{
		// If there is an opening
		if (OpWidth[i] != 0)
		{
			// Assign the string streams
			OpeningWidth << std::fixed << std::setprecision(2) << OpWidth[i];
			BearingLength << std::fixed << std::setprecision(2) << BLength[i];

			// Write to file
			Loading.append("\nOpening Width, OpWidth,Leaf" + std::to_string(i + 1) + ": " + OpeningWidth.str() + "mm");
			Loading.append("\nBearing Length of member supporting Opening, BLength,Leaf" + std::to_string(i + 1) + ": " + BearingLength.str() + "mm");

			// Clears the string stream for the next usage
			OpeningWidth.clear();
			OpeningWidth.str("");
			BearingLength.clear();
			BearingLength.str("");
		}
	}

	// TODO Display ult. line load at top of wall, selfweight of wall at 0.4H from top, load spread length, load laps and ultimate load

	return Loading;
}

const std::string DVLR::PrintLoadingsTopWall()
{
	// Creates a string stream to convert loads from numbers to strings
	std::ostringstream Loads;

	std::string LoadTopOfWall = "";
	// Display the load in a similar was as we did in the console.
	int x = 0;
	for (int i = 1; i <= 2; i++)
	{
		LoadTopOfWall.append("\nLeaf " + std::to_string(i) + ",");
		for (int j = 1; j <= 2; j++)
		{
			if (j == 1) { LoadTopOfWall.append("\n  Eccentric, "); }
			else { LoadTopOfWall.append("\n  Concentric, "); }
			for (int k = 1; k <= 2; k++)
			{
				if (k == 1) { LoadTopOfWall.append("\n\tDead load: "); }
				else { LoadTopOfWall.append("\n\tLive load: "); }

				// Write the loads out as a string
				Loads << std::fixed << std::setprecision(2) << Load[x];
				LoadTopOfWall.append(Loads.str() + "kN/m");
				// Clears the string stream for the next usage
				Loads.clear();
				Loads.str("");
				// Progresses to the next member in Load[] array
				x++;
			}
		}
	}
	return LoadTopOfWall;
}

// TODO Print Eccentricity results
const std::string DVLR::PrintEccentricity()
{
	std::string Eccentricity = "";
	return Eccentricity;
}

// TODO Print minimum Fk required
const std::string DVLR::PrintMinFk()
{
	std::string MinFkResult = "";
	return MinFkResult;
}
