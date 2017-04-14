#include "stdafx.h"		// Turn on in Visual Studio
#include "MasonryChecker.h"
#include <iostream>
#include <string>		// Allows strings to be used
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
	std::string t1 = ConvertToString( TLeaf[0], 2);
	std::string t2 = ConvertToString( TLeaf[1], 2);
	std::string H = ConvertToString( HWall, 2);
	std::string RFactor = ConvertToString( RestraintFactor, 2);
	std::string EffectiveThickness = ConvertToString( Teff, 2);
	std::string EffectiveHeight = ConvertToString( Heff, 2);
	std::string SlendernessRatio = ConvertToString( SR, 2);

	std::string Slenderness = "\nConsider the Slenderness Ratio:";
	Slenderness.append("\nThickness of Leaf 1, t1: " + t1 + "mm");
	Slenderness.append("\nThickness of Leaf 2, t2: " + t2 + "mm");
	Slenderness.append("\nHeight of Wall, H: " + H + "mm");
	Slenderness.append("\nRestraint condition at the top of the wall is " + RestraintCondition + ".");
	Slenderness.append("\nRestraint factor to clause 28.3.1.1., RF: " + RFactor);
	Slenderness.append("\nEffective Thickness, Teff = " + TeffEquation + EffectiveThickness + "mm");
	Slenderness.append("\nEffective Height, Heff = RF*H = " + RFactor + "*" + H + " = " + EffectiveHeight + "mm");
	Slenderness.append("\nHence,\nSlenderness Ratio, SR = Heff/Teff = " + EffectiveHeight + "/" + EffectiveThickness);
	Slenderness.append(" = " + SlendernessRatio + " [-]");

	return Slenderness;
}

const std::string DVLR::PrintPSF()
{
	std::string SafetyFactor = ConvertToString( PSF, 1 );

	std::string PartialSafetyFactor = "\nConsider a Partial Safety Factor:";
	PartialSafetyFactor.append("\nConstruction control is " + ConstructionControl + ".");
	PartialSafetyFactor.append("\nManufacture control is " + ManufactureControl + ".");
	PartialSafetyFactor.append("\nHence,\nPartial Safety from table 4a, PSF: " + SafetyFactor + " [-]");
	return PartialSafetyFactor;
}

const std::string DVLR::PrintLoadings()
{
	std::string Length = ConvertToString( L, 2 );
	std::string PointFourH = ConvertToString( PtFourH, 2);
	
	// Print user input characteristic loads
	std::string Loading = "\nConsider the characteristic loading at the top of the wall: ";
	std::string LoadAtTopOfWall = PrintLoadingsTopWall();
	Loading.append(LoadAtTopOfWall);

	for (int i = 0; i <= 1; i++)
	{
		std::string UnitweightLeaf = ConvertToString(UnitWeight[i], 2);
		Loading.append("\nSelf weight of Leaf " + std::to_string(i+1) + ", Ymas,Leaf" + std::to_string(i+1) + ": " + UnitweightLeaf + "kN/m^3");
	}
	Loading.append("\n\nWall Length & Openings:");
	Loading.append("\nLength of Wall, L: " + Length + "mm");

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
	std::string LoadLap;
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
				 std::string Loads = ConvertToString(Load[x], 2);
				LoadTopOfWall.append(Loads + "kN/m");
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
	// Setup write string
	std::string POpening = "";
	for (int i = 0; i <= 1; i++)
	{
		// If there is an opening
		if (Opening[i].IsOpening)
		{
			// Assign the string streams
			std::string OpeningWidth = ConvertToString( Opening[i].Width, 2);
			std::string BearingLength = ConvertToString( Opening[i].BLength, 2);
			// Write to file
			POpening.append("\nOpening Width, OpWidth,Leaf" + std::to_string(i + 1) + ": " + OpeningWidth + "mm");
			POpening.append("\nBearing Length of member supporting Opening, BLength,Leaf" + std::to_string(i + 1) + ": " + BearingLength + "mm");
		}
	}
	return POpening;
}

const std::string DVLR::PrintUltLineLoadTopWall()
{
	std::string UltLineLoad[2] = { ConvertToString(LoadOverWall.Leaf1, 2) ,ConvertToString(LoadOverWall.Leaf2, 2) };

	int x = 0;
	std::string UltLineLoadTopWall = "\n\nUltimate Line Load at the top of the wall = 1.4(Ecc,DL + Conc,DL) + 1.6(Ecc,LL + Conc,LL)";
	for (int i = 0; i <= 1; i++)
	{
		std::string EccDL = ConvertToString( Load[ x ], 2);
		std::string EccLL = ConvertToString( Load[x + 1], 2);
		std::string ConcDL = ConvertToString( Load[x + 2], 2);
		std::string ConcLL = ConvertToString( Load[x + 3], 2);

		UltLineLoadTopWall.append("\nWult,TopOfWall,Leaf " + std::to_string(i + 1) + " = 1.4(" + EccDL);
		UltLineLoadTopWall.append("kN/m + " + ConcDL + "kN/m) + 1.6(" + EccLL + "kN/m + " );
		UltLineLoadTopWall.append(ConcLL + "kN/m) = " + UltLineLoad[i] + "kN/m");

		// Increments the load array access by 4 spaces for the next leaf
		x += 4;
	}
	return UltLineLoadTopWall;
}

const std::string DVLR::PrintSelfWeight()
{
	std::string Height = ConvertToString(HWall, 2, 0.001);
	std::string Pt4Height = ConvertToString(PtFourH, 2);

	std::string SelfWt = "\n\nSelfWeight at 0.4H from the top of the wall:";
	SelfWt.append("\n0.4H = 0.4*" + Height + "m = " + Pt4Height + "m");
	SelfWt.append("\nSW,0.4H = Ymas*0.4H*t");

	for (int i = 0; i <= 1; i++)
	{
		// Create String stream
		std::string Ymas = ConvertToString( UnitWeight[i], 2);
		std::string Swt = ConvertToString( SelfWeight[i], 2);
		std::string Thickness = ConvertToString( TLeaf[i], 2);
		// Write to file
		SelfWt.append("\nSW,0.4H,Leaf" + std::to_string(i+1) + " = " + Ymas + "kN/m^3 * (" + Thickness + " / 1000)m * " + Pt4Height + "m = " + Swt + "kN/m");
	}

	return SelfWt;
}

const std::string DVLR::PrintSWWallOver()
{
	std::string Height = ConvertToString(HWall, 2, 0.001);
	std::string SWOverOpening[2][2]; // 1st member opening, 2nd member leaf
	SWOverOpening[0][0] = ConvertToString( SelfWeightOverOpening[0].Leaf1, 2);
	SWOverOpening[0][1] = ConvertToString( SelfWeightOverOpening[0].Leaf2, 2);
	SWOverOpening[1][0] = ConvertToString( SelfWeightOverOpening[1].Leaf1, 2);
	SWOverOpening[1][1] = ConvertToString( SelfWeightOverOpening[1].Leaf2, 2);

	std::string SelfWt = "\nSelfWeight of wall above opening(s):";
	SelfWt.append("\nSW,Op = SWeight * (LeafThickness / 1000) * (HeightofWall - OpeningHeight)/1000");

	for (int i = 0; i <= 1; i++) // Opening No.#
	{
		SelfWt.append("\nSelf Weight over Opening " + std::to_string(i + 1) + ":");
		for (int j = 0; j <= 1; j++) // Leaf No.#
		{
			std::string OpHeight = ConvertToString(Opening[i].Height, 2, 0.001);
			std::string Ymas = ConvertToString(UnitWeight[j], 2);
			std::string Thickness = ConvertToString(TLeaf[j], 2, 0.001);

			SelfWt.append("\n" + Ymas + "kN/m^3 * (" + Thickness + "m * (" + Height + "m - " + OpHeight + "m) = ");
			SelfWt.append(SWOverOpening[i][j] + "kN/m");
		}
	}
	return SelfWt;
}

const std::string DVLR::PrintLoadSpreadLength()
{
	// Create String Stream
	std::string WallHeight = ConvertToString( HWall, 2 );
	std::string Pt6H = ConvertToString( HWall, 2, 0.6 );

	std::string LoadSpeadLength = "\n\nLoad Spread Length, Lspread = BLength + (OpeningHieght - 0.6H) < L";
	LoadSpeadLength.append("\n0.6H = 0.6 * " + WallHeight + "mm = " + Pt6H + "mm");

	for (int i = 0; i <= 1; i++)
	{
		std::string SpreadLength = ConvertToString(Opening[i].Spread, 2);

		if (Opening[i].IsOpening)
		{
			LoadSpeadLength.append("\nLspread Opening " + std::to_string(i+1) + " = " + SpreadLengthMessage[i]);
			LoadSpeadLength.append("\nTherefore, Lspread Opening " + std::to_string(i + 1) + " = " + SpreadLength + "m");
		}
	}

	return LoadSpeadLength;
}

// If no load spread (due to no openings), show ultimate load calculation
const std::string DVLR::PrintNoLoadSpread()
{
	std::string LoadLeaf[2] = { ConvertToString(LoadOverWall.Leaf1, 2), ConvertToString(LoadOverWall.Leaf2, 2) };
	std::string UltLoad[2] = { ConvertToString(WultLoad.Leaf1, 2), ConvertToString(WultLoad.Leaf2, 2) };

	std::string NoLoadSpread = "\n\nConsider the ultimate load on the wall:";
	NoLoadSpread.append("\nWult = 1.4*SelfWeight + Wult,TopOfWall");

	for (int i = 0; i <= 1; i++)
	{
		std::string SW = ConvertToString(SelfWeight[i], 2);
		NoLoadSpread.append("\nWult,Leaf" + std::to_string(i+1) + " = 1.4*" + SW + "kN/m + "+ LoadLeaf[i] + "kN/m = " + UltLoad[i] + "kN/m");
	}
	
	return NoLoadSpread;
}

// Calculate ult load of lapped loads
const std::string DVLR::PrintDoubleLoadSpread()
{
	std::string SpreadLength[2] = { ConvertToString(Opening[0].Spread,2), ConvertToString(Opening[1].Spread,2) };
	float AddSpreadLength = Opening[0].Spread + Opening[1].Spread;
	std::string SpreadLengthSum = ConvertToString(AddSpreadLength, 2);
	std::string Length = ConvertToString(L, 2);

	std::string DoubleLoadSpread = "\n\nCheck whether the two load spreads lap:";
	DoubleLoadSpread.append("\nLoads lap if: Lspread1 + Lspread2 > L");
	DoubleLoadSpread.append("\n" + SpreadLength[0] + "mm + " + SpreadLength[1] + "mm = " + SpreadLengthSum + "mm ");
	if (SpreadCaseStatus == SpreadCase::DblLoadSpreadDoesNOTLap)
	{
		DoubleLoadSpread.append("< ");
		DoubleLoadSpread.append(Length + "mm");
		DoubleLoadSpread.append("\nBoth loads do NOT lap and we will therefore need to design for the greatest concentration.");
		DoubleLoadSpread.append("\nProceeding to the load spread analysis from the greater load at opening " + BiggestOpening);
		std::string SingleSpreadLoad = PrintSingleSpread();
		DoubleLoadSpread.append(SingleSpreadLoad);
	}
	else
	{
		// Call the string stream only if branching to double load spread calc
		std::string UltTopWall[2] = { ConvertToString(LoadOverWall.Leaf1, 2), ConvertToString(LoadOverWall.Leaf2, 2) };
		std::string OpeningWidth[2] = { ConvertToString(Opening[0].Width, 2), ConvertToString(Opening[1].Width, 2) };
		std::string Wult[2] = { ConvertToString(WultLoad.Leaf1, 2), ConvertToString(WultLoad.Leaf2, 2) };
		std::string SWOverOpening[2][2]; // 1st member opening, 2nd member leaf
		SWOverOpening[0][0] = ConvertToString(SelfWeightOverOpening[0].Leaf1, 2);
		SWOverOpening[0][1] = ConvertToString(SelfWeightOverOpening[0].Leaf2, 2);
		SWOverOpening[1][0] = ConvertToString(SelfWeightOverOpening[1].Leaf1, 2);
		SWOverOpening[1][1] = ConvertToString(SelfWeightOverOpening[1].Leaf2, 2);

		DoubleLoadSpread.append("> ");
		DoubleLoadSpread.append(Length + "mm");
		DoubleLoadSpread.append("\nAnd hence both load spreads lap. \nWult = ");
		DoubleLoadSpread.append("(Wult,TopOfWall + (1.4 * Selfweight@0.4H)) + (Wult,TopOfWall + (1.4*SelfWeightOverOpening)) * (OpenWidth1 / 1000)) / (2 * (Lspread1 / 1000)) + ");
		DoubleLoadSpread.append("(Wult,TopOfWall * (OpenWidth2 / 1000)) / (2 * (Lspread2 / 1000))");
		for (int i = 0; i <= 1; i++) // Loops per Leaf
		{
			std::string SW = ConvertToString(SelfWeight[i], 2);
			DoubleLoadSpread.append("\nWult,Leaf" + std::to_string(i + 1) + " = (");
			DoubleLoadSpread.append(UltTopWall[i] + "kN/m + (1.4*" + SW + "kN/m)) + ((" + UltTopWall[i] + "kN/m + (1.4 * " + SWOverOpening[0][i]) + "kN/m)) * (" + OpeningWidth[0] + "mm / 1000)) ";
			DoubleLoadSpread.append("/ (2 * (" + SpreadLength[0] + "mm / 1000)) + (" + UltTopWall[i] + "kN/m + (1.4 * " + SWOverOpening[1][i] + "kN/m)) * (" + OpeningWidth[1] + "mm / 1000)) / (2 * (");
			DoubleLoadSpread.append(SpreadLength[1] + "mm / 1000))");
			DoubleLoadSpread.append("\n= " + Wult[i] + "kN/m");
		}
	}
	return DoubleLoadSpread;
}

// Calculate ult load of (greatest) spread load
const std::string DVLR::PrintSingleSpread()
{
	std::string Length = ConvertToString( L, 2);
	// Values based on greatest opening size
	std::string UltTopWall[2] = { ConvertToString( LoadOverWall.Leaf1, 2), ConvertToString(LoadOverWall.Leaf2, 2) };
	std::string Wult[2] = { ConvertToString(WultLoad.Leaf1, 2), ConvertToString(WultLoad.Leaf2, 2) };

	int x;
	// Assign string operators based on greatest load
	(Opening[0].Width >= Opening[1].Width) ? x = 0 : x = 1;
		BiggestOpening = std::to_string(x+1);
		std::string SpreadLength = ConvertToString(Opening[x].Spread, 2);
		std::string OpeningWidth = ConvertToString(Opening[x].Spread, 2);
		std::string SWOO[2] = { ConvertToString(SelfWeightOverOpening[x].Leaf1, 2), ConvertToString(SelfWeightOverOpening[x].Leaf2, 2) };

	// Begin to write content
	std::string SingleLoadSpread = "\n\nConsider the load spread from opening " + BiggestOpening + ":";
	SingleLoadSpread.append("\nWult = (Wult,TopOfWall + (1.4 * Selfweight@0.4H)) + ((Wult,TopOfWall + (1.4*SelfWeightOverOpening)) * (OpenWidth" + BiggestOpening + " / 1000)) / (2 * (Lspread" + BiggestOpening + " / 1000))");
	
	for (int i = 0; i <= 1; i++) // Loops per Leaf
	{
		std::string SW = ConvertToString(SelfWeight[i], 2);
		SingleLoadSpread.append("\n" + UltTopWall[i] + "kN/m + (1.4*" + SW + "kN/m)) + ((" + UltTopWall[i] + "kN/m + (1.4 * " + SWOO[i] + "kN/m)) * (" + OpeningWidth + "mm / 1000)) ");
		SingleLoadSpread.append("/ (2 * (" + SpreadLength + "mm / 1000)) = ");
		SingleLoadSpread.append(Wult[i] + "kN/m");
	}
	return SingleLoadSpread;
}

// TODO - Finish 
const std::string DVLR::PrintMinFkSup()
{
	// Opening[i].BLength, TLeaf, LoadOverWall, SelfWeightOverOpening[i], Opening[i].Width, PSF
	// Suggested to make a matrix array to deal with different leaves at different opening and fit in loops
	std::string SWOverOpening[2][2]; // 1st member opening, 2nd member leaf
	SWOverOpening[0][0] = ConvertToString(SelfWeightOverOpening[0].Leaf1, 2);
	SWOverOpening[0][1] = ConvertToString(SelfWeightOverOpening[0].Leaf2, 2);
	SWOverOpening[1][0] = ConvertToString(SelfWeightOverOpening[1].Leaf1, 2);
	SWOverOpening[1][1] = ConvertToString(SelfWeightOverOpening[1].Leaf2, 2);

	std::string MinFkSup = "\nDetermine load at support.\n";
	MinFkSup.append("Wsup = (Wult+SWOverOpening)*((OpLength*0.5)+BLength)\n");

	// Print Load at Support Calculation 
	for (int i = 0 ; i <= 1 ; i++) // Per Opening
	{
		MinFkSup.append("Opening " + std::to_string(i+1) + ":\n");
		for (int j = 0; j <= 1; j++) // Per Leaf
		{
			std::string OpBLength = ConvertToString(Opening[i].BLength, 2);
			std::string LeafThickness = ConvertToString(TLeaf[i], 2);
			MinFkSup.append("  Leaf " + std::to_string(j+1) + " = (" + /*Wult*/ + "+SWOverOpening)*((OpLength*0.5)+BLength)\n");
		}
	}
	// Print MinFkSup

	return MinFkSup;
}
