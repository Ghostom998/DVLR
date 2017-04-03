#include "stdafx.h"		// Turn on in Visual Studio
#include "MasonryChecker.h"
#include <iostream>
#include <string>		// Allows strings to be used
#include <iomanip>      // std::setprecision -> allows us to reduce the decimal places in the string stream
#include <sstream>		// String Stream: allows numbers to be cast as strings (i.e. to_string() )
#include <fstream>		// File Stream: allows input/output to file

bool OpenFile();

const int DVLR::PrintToFile()
{
	std::string FileName = "";

	// Take a name for the file
	std::cout << "Beware: Any .txt file with the same name will be overwritten without warning." << std::endl;
	std::cout << "Please enter the file name [*.txt]:  ";
	std::getline(std::cin, FileName);

	// Creates a .txt file with the user specified name
	std::ofstream writer(FileName + ".txt");
	// If error opening the file...
	if (!writer)
	{
		// ... Display an error message and close the program in error.
		std::cout << "Error creating the file." << std::endl;
		std::cout << "Please check that you have permission to save to the location or \ndo not already have a file of the same name open." << std::endl;
		std::exit(-1);
	}
	std::cout << "The file is saved in the location of the executable *.exe file" << std::endl;

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

	// Build MinFk at Support text
	std::string MinFkSup = PrintMinFkSup();
	// Write Min Fk at Support to File
	writer << MinFkSup << std::endl;

	// Build Eccentricity text
	std::string Eccentricity = PrintEccentricity();
	// Write PSF text to file
	writer << Eccentricity << std::endl;

	// Build MinFk text
	std::string MinFk = PrintMinFk();
	// Write MinFk text to file
	writer << MinFk << std::endl;

	// After finishing writing we close the file stream
	writer.close();

	// If write  is not successful
	if (!writer)
	{
		// ... Display an error message 
		std::cout << "Error writing to file." << std::endl;
		std::cout << "Please check that you have permission to save to the location or \ndo not already have a file of the same name open." << std::endl;
		// ...and close the program in error.
		std::exit(-1);
	}
	else
	{
		// Assumes that if there were no errors, then the file was written successfully
		std::cout << "The program has written to the file without error." << std::endl;
		bool ShouldUserOpenFile = OpenFile();
		if (ShouldUserOpenFile)
		{
			#ifdef _WIN32
			// Build the command to pass to system
			std::cout << "Note that Notepad must be closed before the program can be terminated." << std::endl;
			std::string Command = "notepad.exe " + FileName + ".txt";
			system(Command.c_str());
			std::exit(0);
			#elif __unix__
			// Build the command to pass to system - WIP 
			std::string Command = "gedit " + FileName + ".txt\n";
			execl(Command.c_str());
			std::exit(0);
			#else
			#error 
			std::cout << "OS not supported!\n" << std::endl;
			std::exit(-1);
			#endif
		}
		// Exit to main()
		return 0;
	}
}

bool OpenFile()
{
	// Flushes the input buffer
	//std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	std::cout << "\nDo you want to open the file output? (y/n):  ";
	std::string Response = "";
	std::getline(std::cin, Response);
	std::cout << std::endl;

	// Returns the bool true if the first letter begins
	// with the upper or lowercase "y" else returns false.
	return (Response[0] == 'y') || (Response[0] == 'Y');
}

const std::string DVLR::PrintIntro(std::string NameOfFile)
{
	std::string Introduction = "MasonryChecker. \nby Thomas Roberts, CWA.";
	Introduction.append("\n\nA program to consider the vertical load design of cavity masonry in accordance with BS 5628-1:2005.");
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

	std::ostringstream PointFourH;
	PointFourH << std::fixed << std::setprecision(1) << PtFourH;

	// Print user input characteristic loads
	std::string Loading = "\nConsider the characteristic loading at the top of the wall: ";
	std::string LoadAtTopOfWall = PrintLoadingsTopWall();
	Loading.append(LoadAtTopOfWall);

	Loading.append("\nSelf weight of Leaf 1, Ymas,Leaf1: " + UnitweightLeaf1.str() + "kN/m^3");
	Loading.append("\nSelf weight of Leaf 2, Ymas,Leaf2: " + UnitweightLeaf2.str() + "kN/m^3");

	Loading.append("\n\nWall Length & Openings:");
	Loading.append("\nLength of Wall, L: " + Length.str() + "mm");

	// Print user input openings and bearing lengths
	std::string Openings = PrintOpenings();
	Loading.append(Openings);

	// Print calc for Ult Line Load at top of wall
	std::string UltLineLoadTopWall = PrintUltLineLoadTopWall();
	Loading.append(UltLineLoadTopWall);

	// Print calc for selfweight at 0.4H
	std::string SelfWeight = PrintSelfWeight();
	Loading.append(SelfWeight);

	// Print calc for selfweight above openings
	std::string SWWallOver = PrintSWWallOver();
	Loading.append(SWWallOver);

	// Print Load Spread Length calcs
	if (SpreadCaseStatus != SpreadCase::NoLoadSpread) //only print if there is a load spread i.e. anything BUT no load spread
	{
		std::string LoadSpreadLength = PrintLoadSpreadLength();
		Loading.append(LoadSpreadLength);
	}

	// Print Load Lap Dependant on case - cases determined in DVLR::GetSpreadLoad()
	// Note: Ultimate load determined here
	std::string LoadLap = "";
	switch(SpreadCaseStatus)
	{
	case SpreadCase::NoLoadSpread:
		LoadLap = PrintNoLoadSpread();
		break;
	case SpreadCase::DblLoadSpreadLaps:
		LoadLap = PrintDoubleLoadSpread();
		break;
	case SpreadCase::DblLoadSpreadDoesNOTLap:
		// Same method as case "DblLoadSpreadLaps", branches into single load spread
		LoadLap = PrintDoubleLoadSpread();
		break;
	case SpreadCase::SglLoadSpread:
		LoadLap = PrintSingleSpread();
		break;
	default:
			// if none of the above conditions were met then there was clearly an error
			std::cout << "Error printing load spread. Please try again." << std::endl;
			std::cout << "If the error persists, please report the error with the steps to reproduce it." << std::endl;
			std::cout << "Please \"create an issue\" in: https://github.com/Ghostom998/DVLR/issues" << std::endl;
			std::exit(-1);
	}
	Loading.append(LoadLap);	

	return Loading;
}

// TODO Print Eccentricity results
const std::string DVLR::PrintEccentricity()
{
	std::string Eccentricity = "";
	return Eccentricity;
}

// TODO Print SAF
const std::string DVLR::PrintSAF()
{
	std::string SAF = "";
	return SAF;
}

// TODO Print minimum Fk required
const std::string DVLR::PrintMinFk()
{
	std::string MinFkResult = "";
	return MinFkResult;
}

// A sub print method
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

// TODO - Print opening height
const std::string DVLR::PrintOpenings()
{
	// Setup string streams
	std::ostringstream OpeningWidth;
	std::ostringstream BearingLength;
	// Setup write string
	std::string POpening = "";
	for (int i = 0; i <= 1; i++)
	{
		// If there is an opening
		if (Opening[i].IsOpening)
		{
			// Assign the string streams
			OpeningWidth << std::fixed << std::setprecision(2) << Opening[i].Width;
			BearingLength << std::fixed << std::setprecision(2) << Opening[i].BLength;

			// Write to file
			POpening.append("\nOpening Width, OpWidth,Leaf" + std::to_string(i + 1) + ": " + OpeningWidth.str() + "mm");
			POpening.append("\nBearing Length of member supporting Opening, BLength,Leaf" + std::to_string(i + 1) + ": " + BearingLength.str() + "mm");

			// Clears the string stream for the next usage
			OpeningWidth.clear();
			OpeningWidth.str("");
			BearingLength.clear();
			BearingLength.str("");
		}
	}
	return POpening;
}

const std::string DVLR::PrintUltLineLoadTopWall()
{
	std::ostringstream EccDL;
	std::ostringstream EccLL;
	std::ostringstream ConcDL;
	std::ostringstream ConcLL;

	std::ostringstream UltLineLoad[2];
	UltLineLoad[0] << std::fixed << std::setprecision(2) << LoadOverWall.Leaf1;
	UltLineLoad[1] << std::fixed << std::setprecision(2) << LoadOverWall.Leaf2;

	int x = 0;
	std::string UltLineLoadTopWall = "\n\nUltimate Line Load at the top of the wall = 1.4(Ecc,DL + Conc,DL) + 1.6(Ecc,LL + Conc,LL)";
	for (int i = 0; i <= 1; i++)
	{
		EccDL << std::fixed << std::setprecision(2) << Load[ x ];
		EccLL << std::fixed << std::setprecision(2) << Load[x + 1];
		ConcDL << std::fixed << std::setprecision(2) << Load[x + 2];
		ConcLL << std::fixed << std::setprecision(2) << Load[x + 3];

		UltLineLoadTopWall.append("\nWult,TopOfWall,Leaf " + std::to_string(i + 1) + " = 1.4(" + EccDL.str());
		UltLineLoadTopWall.append("kN/m + " + ConcDL.str() + "kN/m) + 1.6(" + EccLL.str() + "kN/m + " );
		UltLineLoadTopWall.append(ConcLL.str() + "kN/m) = " + UltLineLoad[i].str() + "kN/m");

		// Increments the load array access by 4 spaces for the next leaf
		x += 4;

		// Clears the string stream for the next usage
		EccDL.clear();
		EccDL.str("");
		EccLL.clear();
		EccLL.str("");
		ConcDL.clear();
		ConcDL.str("");
		ConcLL.clear();
		ConcLL.str("");
	}
	return UltLineLoadTopWall;
}

const std::string DVLR::PrintSelfWeight()
{
	std::ostringstream Height;
	Height << std::fixed << std::setprecision(2) << (HWall / 1000);
	std::ostringstream Pt4Height;
	Pt4Height << std::fixed << std::setprecision(2) << PtFourH;
	std::ostringstream Ymas;
	std::ostringstream Swt;
	std::ostringstream Thickness;

	std::string SelfWt = "\n\nSelfWeight at 0.4H from the top of the wall:";
	SelfWt.append("\n0.4H = 0.4*" + Height.str() + "m = " + Pt4Height.str() + "m");
	SelfWt.append("\nSW,0.4H = Ymas*0.4H*t");

	for (int i = 0; i <= 1; i++)
	{
		// Create String stream
		Ymas << std::fixed << std::setprecision(2) << UnitWeight[i];
		Swt << std::fixed << std::setprecision(2) << SelfWeight[i];
		Thickness << std::fixed << std::setprecision(2) << TLeaf[i];

		// Write to file
		SelfWt.append("\nSW,0.4H,Leaf" + std::to_string(i+1) + " = " + Ymas.str() + "kN/m^3 * (" + Thickness.str() + " / 1000)m * " + Pt4Height.str() + "m = " + Swt.str() + "kN/m");

		// Clear String stream
		Ymas.clear();
		Ymas.str("");
		Swt.clear();
		Swt.str("");
		Thickness.clear();
		Thickness.str("");
	}

	return SelfWt;
}

const std::string DVLR::PrintSWWallOver()
{
	std::ostringstream Height;
	Height << std::fixed << std::setprecision(2) << (HWall / 1000);
	std::ostringstream OpHeight;
	std::ostringstream Ymas;
	std::ostringstream SWOverOpening[2][2]; // 1st member opening, 2nd member leaf
	SWOverOpening[0][0] << std::fixed << std::setprecision(2) << SelfWeightOverOpening[0].Leaf1;
	SWOverOpening[0][1] << std::fixed << std::setprecision(2) << SelfWeightOverOpening[0].Leaf2;
	SWOverOpening[1][0] << std::fixed << std::setprecision(2) << SelfWeightOverOpening[1].Leaf1;
	SWOverOpening[1][1] << std::fixed << std::setprecision(2) << SelfWeightOverOpening[1].Leaf2;
	std::ostringstream Thickness;

	std::string SelfWt = "\nSelfWeight of wall above opening(s):";
	SelfWt.append("\nSW,Op = SWeight * (LeafThickness / 1000) * (HeightofWall - OpeningHeight)/1000");

	for (int i = 0; i <= 1; i++) // Opening
	{
		SelfWt.append("\nSelf Weight over Opening " + std::to_string(i + 1) + ":");
		for (int j = 0; j <= 1; j++) // Leaf
		{
			OpHeight << std::fixed << std::setprecision(2) << Opening[i].Height / 1000;
			Ymas << std::fixed << std::setprecision(2) << UnitWeight[j];
			Thickness << std::fixed << std::setprecision(2) << TLeaf[j]/1000;

			SelfWt.append("\n" + Ymas.str() + "kN/m^3 * (" + Thickness.str() + "m * (" + Height.str() + "m - " + OpHeight.str() + "m) = ");
			SelfWt.append(SWOverOpening[i][j].str() + "kN/m");

			OpHeight.clear();
			OpHeight.str("");
			Ymas.clear();
			Ymas.str("");
			Thickness.clear();
			Thickness.str("");
		}
	}

	//std::string SWOO = "";
	return SelfWt;
}

const std::string DVLR::PrintLoadSpreadLength()
{
	// Create String Stream
	std::ostringstream SpreadLength;
	std::ostringstream WallHeight;
	WallHeight << std::fixed << std::setprecision(2) << HWall;
	std::ostringstream Pt6H;
	Pt6H << std::fixed << std::setprecision(2) << HWall*0.6;

	std::string LoadSpeadLength = "\n\nLoad Spread Length, Lspread = BLength + (OpeningHieght - 0.6H) < L";
	LoadSpeadLength.append("\n0.6H = 0.6 * " + WallHeight.str() + "mm = " + Pt6H.str() + "mm");

	for (int i = 0; i <= 1; i++)
	{
		SpreadLength << std::fixed << std::setprecision(2) << Opening[i].Spread;

		if (Opening[i].IsOpening)
		{
			LoadSpeadLength.append("\nLspread Opening " + std::to_string(i+1) + " = " + SpreadLengthMessage[i]);
			LoadSpeadLength.append("\nTherefore, Lspread Opening " + std::to_string(i + 1) + " = " + SpreadLength.str() + "m");
		}
		// Clear the string streams
		SpreadLength.clear();
		SpreadLength.str("");
	}

	return LoadSpeadLength;
}

// If no load spread (due to no openings), show ultimate load calculation
const std::string DVLR::PrintNoLoadSpread()
{
	std::ostringstream SW;
	std::ostringstream LoadLeaf[2];
	LoadLeaf[0] << std::fixed << std::setprecision(2) << LoadOverWall.Leaf1;
	LoadLeaf[1] << std::fixed << std::setprecision(2) << LoadOverWall.Leaf2;
	std::ostringstream UltLoad[2];
	UltLoad[0] << std::fixed << std::setprecision(2) << WultLoad.Leaf1;
	UltLoad[1] << std::fixed << std::setprecision(2) << WultLoad.Leaf2;

	std::string NoLoadSpread = "\n\nConsider the ultimate load on the wall:";
	NoLoadSpread.append("\nWult = 1.4*SelfWeight + Wult,TopOfWall");

	for (int i = 0; i <= 1; i++)
	{
		SW << std::fixed << std::setprecision(2) << SelfWeight[i];
		NoLoadSpread.append("\nWult,Leaf" + std::to_string(i+1) + " = 1.4*" + SW.str() + "kN/m + "+ LoadLeaf[i].str() + "kN/m = " + UltLoad[i].str() + "kN/m");
		SW.clear();
		SW.str("");
	}
	
	return NoLoadSpread;
}

// Calculate ult load of lapped loads
const std::string DVLR::PrintDoubleLoadSpread()
{
	std::ostringstream SpreadLength[2];
	SpreadLength[0] << std::fixed << std::setprecision(2) << Opening[0].Spread;
	SpreadLength[1] << std::fixed << std::setprecision(2) << Opening[1].Spread;
	std::ostringstream SpreadLengthSum;
	SpreadLengthSum << std::fixed << std::setprecision(2) << Opening[0].Spread + Opening[1].Spread;
	std::ostringstream Length;
	Length << std::fixed << std::setprecision(2) << L;

	std::string DoubleLoadSpread = "\n\nCheck whether the two load spreads lap:";
	DoubleLoadSpread.append("\nLoads lap if: Lspread1 + Lspread2 > L");
	DoubleLoadSpread.append("\n" + SpreadLength[0].str() + "mm + " + SpreadLength[1].str() + "mm = " + SpreadLengthSum.str() + "mm ");
	if (SpreadCaseStatus == SpreadCase::DblLoadSpreadDoesNOTLap)
	{
		DoubleLoadSpread.append("< ");
		DoubleLoadSpread.append(Length.str() + "mm");
		DoubleLoadSpread.append("\nBoth loads do NOT lap and we will therefore need to design for the greatest concentration.");
		DoubleLoadSpread.append("\nProceeding to the load spread analysis from the greater load at opening " + BiggestOpening);
		std::string SingleSpreadLoad = PrintSingleSpread();
		DoubleLoadSpread.append(SingleSpreadLoad);
	}
	else
	{
		// Call the string stream only if branching to double load spread calc
		std::ostringstream SW;
		std::ostringstream UltTopWall[2];
		UltTopWall[0] << std::fixed << std::setprecision(2) << LoadOverWall.Leaf1;
		UltTopWall[1] << std::fixed << std::setprecision(2) << LoadOverWall.Leaf2;
		std::ostringstream OpeningWidth[2];
		OpeningWidth[0] << std::fixed << std::setprecision(2) << Opening[0].Width;
		OpeningWidth[1] << std::fixed << std::setprecision(2) << Opening[1].Width;
		std::ostringstream Wult[2];
		Wult[0] << std::fixed << std::setprecision(2) << WultLoad.Leaf1;
		Wult[1] << std::fixed << std::setprecision(2) << WultLoad.Leaf2;
		std::ostringstream SWOverOpening[2][2]; // 1st member opening, 2nd member leaf
		SWOverOpening[0][0] << std::fixed << std::setprecision(2) << SelfWeightOverOpening[0].Leaf1;
		SWOverOpening[0][1] << std::fixed << std::setprecision(2) << SelfWeightOverOpening[0].Leaf2;
		SWOverOpening[1][0] << std::fixed << std::setprecision(2) << SelfWeightOverOpening[1].Leaf1;
		SWOverOpening[1][1] << std::fixed << std::setprecision(2) << SelfWeightOverOpening[1].Leaf2;

		DoubleLoadSpread.append("> ");
		DoubleLoadSpread.append(Length.str() + "mm");
		DoubleLoadSpread.append("\nAnd hence both load spreads lap. \nWult = ");
		DoubleLoadSpread.append("(Wult,TopOfWall + (1.4 * Selfweight@0.4H)) + (Wult,TopOfWall + (1.4*SelfWeightOverOpening)) * (OpenWidth1 / 1000)) / (2 * (Lspread1 / 1000)) + ");
		DoubleLoadSpread.append("(Wult,TopOfWall * (OpenWidth2 / 1000)) / (2 * (Lspread2 / 1000))");
		for (int i = 0; i <= 1; i++) // Loops per Leaf
		{
			SW << std::fixed << std::setprecision(2) << SelfWeight[i];
			DoubleLoadSpread.append("\nWult,Leaf" + std::to_string(i + 1) + " = (");
			DoubleLoadSpread.append(UltTopWall[i].str() + "kN/m + (1.4*" + SW.str() + "kN/m)) + ((" + UltTopWall[i].str() + "kN/m + (1.4 * " + SWOverOpening[0][i].str() + "kN/m)) * (" + OpeningWidth[0].str() + "mm / 1000)) ");
			DoubleLoadSpread.append("/ (2 * (" + SpreadLength[0].str() + "mm / 1000)) + (" + UltTopWall[i].str() + "kN/m + (1.4 * " + SWOverOpening[1][i].str() + "kN/m)) * (" + OpeningWidth[1].str() + "mm / 1000)) / (2 * (");
			DoubleLoadSpread.append(SpreadLength[1].str() + "mm / 1000))");
			DoubleLoadSpread.append("\n= " + Wult[i].str() + "kN/m");
			SW.clear();
			SW.str("");
		}
	}
	return DoubleLoadSpread;
}

// Calculate ult load of (greatest) spread load
const std::string DVLR::PrintSingleSpread()
{
	std::ostringstream SW;
	std::ostringstream Length;
	Length << std::fixed << std::setprecision(2) << L;

	// Values based on greatest opening size
	std::ostringstream UltTopWall[2];
	UltTopWall[0] << std::fixed << std::setprecision(2) << LoadOverWall.Leaf1;
	UltTopWall[1] << std::fixed << std::setprecision(2) << LoadOverWall.Leaf2;
	std::ostringstream Wult[2];
	Wult[0] << std::fixed << std::setprecision(2) << WultLoad.Leaf1;
	Wult[1] << std::fixed << std::setprecision(2) << WultLoad.Leaf2;
	std::ostringstream OpeningWidth;
	std::ostringstream SpreadLength;
	std::ostringstream SWOO[2];

	// Assign string stream operators based on greatest load
	if (Opening[0].Width >= Opening[1].Width)
	{
		BiggestOpening = "1";
		SpreadLength << std::fixed << std::setprecision(2) << Opening[0].Spread;
		OpeningWidth << std::fixed << std::setprecision(2) << Opening[0].Width;
		SWOO[0] << std::fixed << std::setprecision(2) << SelfWeightOverOpening[0].Leaf1;
		SWOO[1] << std::fixed << std::setprecision(2) << SelfWeightOverOpening[0].Leaf2;
	}
	else
	{
		BiggestOpening = "2";
		SpreadLength << std::fixed << std::setprecision(2) << Opening[1].Spread;
		OpeningWidth << std::fixed << std::setprecision(2) << Opening[1].Width;
		SWOO[0] << std::fixed << std::setprecision(2) << SelfWeightOverOpening[1].Leaf1;
		SWOO[1] << std::fixed << std::setprecision(2) << SelfWeightOverOpening[1].Leaf2;
	}

	//return ((UltLoad + (1.4 * Selfweight)) + (((UltLoad + (1.4*SelfWeightOverOpening[0])) * (OpenWidth[0].Width / 1000)) / (2 * (Opening[0].Spread / 1000))));

	std::string SingleLoadSpread = "\n\nConsider the load spread from opening " + BiggestOpening + ":";
	SingleLoadSpread.append("\nWult = (Wult,TopOfWall + (1.4 * Selfweight@0.4H)) + ((Wult,TopOfWall + (1.4*SelfWeightOverOpening)) * (OpenWidth" + BiggestOpening + " / 1000)) / (2 * (Lspread" + BiggestOpening + " / 1000))");
	
	for (int i = 0; i <= 1; i++) // Loops per Leaf
	{
		SW << std::fixed << std::setprecision(2) << SelfWeight[i];
		SingleLoadSpread.append("\n" + UltTopWall[i].str() + "kN/m + (1.4*" + SW.str() + "kN/m)) + ((" + UltTopWall[i].str() + "kN/m + (1.4 * " + SWOO[i].str() + "kN/m)) * (" + OpeningWidth.str() + "mm / 1000)) ");
		SingleLoadSpread.append("/ (2 * (" + SpreadLength.str() + "mm / 1000)) = ");
		SingleLoadSpread.append(Wult[i].str() + "kN/m");
		SW.clear();
		SW.str("");
	}
	return SingleLoadSpread;
}

// TODO - Finish 
const std::string DVLR::PrintMinFkSup()
{
	// Opening[i].BLength, TLeaf, LoadOverWall, SelfWeightOverOpening[i], Opening[i].Width, PSF
	// Suggested to make a matrix array to deal with different leaves at different opening and fit in loops

	std::string MinFkSup = "";

	// Print Load at Support Calc
	// Print MinFkSup

	return MinFkSup;
}
