#include "stdafx.h"
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
	getline(std::cin, FileName);

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
	// Write Eccentricity text to file
	writer << Eccentricity << std::endl;

	std::string SAFText = PrintSAF();
	// Write Small Area Factor To File
	writer << SAFText << std::endl;

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
	std::cout << "\nDo you want to open the file output? (y/n):  ";
	std::string Response = "";
	std::getline(std::cin, Response);
	std::cout << std::endl;

	// Returns the bool true if the first letter begins
	// with the upper or lowercase "y" else returns false.
	return (Response[0] == 'y') || (Response[0] == 'Y');
}

const std::string DVLR::PrintIntro(std::string& NameOfFile)
{
	std::string Introduction = "MasonryChecker. \nby Thomas Roberts, CWA.";
	Introduction.append("\n\nAn Open Source structural design program to consider the vertical load design of cavity masonry in accordance with BS 5628-1:2005.");
	Introduction.append("\nStay up to date with the project or contribute on GitHub https://github.com/Ghostom998/DVLR/");
	Introduction.append("\nWall Referenced: " + NameOfFile);
	return Introduction;
}

const std::string DVLR::PrintSlenderness()
{
	// Setup up string stream variables to round the inputs to 2 decimal places and convert numbers to strings
	std::string t1 = Str( TLeaf[0]);
	std::string t2 = Str( TLeaf[1]);
	std::string H = Str( HWall);
	std::string RFactor = Str( RestraintFactor);
	std::string EffectiveThickness = Str( Teff);
	std::string EffectiveHeight = Str( Heff);
	std::string SlendernessRatio = Str( SR);

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
	std::string SafetyFactor = Str( PSF, 1 );

	std::string PartialSafetyFactor = "\nConsider a Partial Safety Factor:";
	PartialSafetyFactor.append("\nConstruction control is " + ConstructionControl + ".");
	PartialSafetyFactor.append("\nManufacture control is " + ManufactureControl + ".");
	PartialSafetyFactor.append("\nHence,\nPartial Safety from table 4a, PSF: " + SafetyFactor + " [-]");
	return PartialSafetyFactor;
}

const std::string DVLR::PrintLoadings()
{
	std::string Length = Str( L );
	std::string PointFourH = Str( PtFourH);

	// Print user input characteristic loads
	std::string Loading = "\nConsider the characteristic loading at the top of the wall: ";
	std::string LoadAtTopOfWall = PrintLoadingsTopWall();
	Loading.append(LoadAtTopOfWall);

	for (int i = 0; i <= 1; i++)
	{
		std::string UnitweightLeaf = Str(UnitWeight[i]);
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
		std::cout << "Please \"create an issue\" in: https://github.com/Ghostom998/DVLR/issues/" << std::endl;
		std::exit(-1);
	}
	Loading.append(LoadLap);

	return Loading;
}

// TODO Print Eccentricity results
const std::string DVLR::PrintEccentricity()
{
	std::string Eccentricity = "\nConsider a reduction in load capacity due to eccentric load & slenderness.";
	std::string UserEccentricity = PrintUserEccentricity();
	std::string Ex = PrintEx();
	std::string Ea = PrintEa();
	std::string Et = PrintEt();
	std::string Em = PrintEm();
	std::string Beta = PrintBeta();
	Eccentricity.append(UserEccentricity + Ex + Ea + Et + Em + Beta);

	return Eccentricity;
}

const std::string DVLR::PrintUserEccentricity()
{
	std::string UserEccentricity = "\n\nConsider the Eccentricity from the load above:";
	for(int i = 0 ; i<=1 ; i++)
	{
		std::string CustEx = Str(Eccentricity[i]);
		std::string t = Str(TLeaf[i], 1);
		std::string Bearing = Str(BearingLength[i], 1);

		UserEccentricity.append("\nEccentric Load onto Leaf " + std::to_string(i+1));
		switch (Selection)
		{
		case UserEccentricity::FullBearing:
			UserEccentricity.append(" assumed to bear the full leaf width.\n");
			UserEccentricity.append("  Load Eccentricity = t/6 = " + t + "mm/6 = ");
			break;
		case UserEccentricity::Continuous:
			UserEccentricity.append(" assumed to bear continuously over the full leaf width.\n");
			UserEccentricity.append("  Load Eccentricity = t/3 = " + t + "mm/3 = ");
			break;
		case UserEccentricity::Hangars:
			UserEccentricity.append(" assumed to bear onto joist hangars.\n");
			UserEccentricity.append("  Load Eccentricity = t/2 = " + t + "mm/2 = ");
			break;
		case UserEccentricity::NotFullBearing:
			UserEccentricity.append(" assumed to bear " + Bearing + "mm onto along the leaf width.\n");
			UserEccentricity.append("  Load Eccentricity = t/2 - l/3 = " + t + "mm/2 - " + Bearing + "mm/3 = ");
			break;
		case UserEccentricity::Custom:
			UserEccentricity.append(" has a Custom Value = ");
			break;
		default:
			// If input is not a valid case, ask the user for a valid case.
			std::cout << "Error writing output. Please raise an issue on our GitHub page with the steps to reproduce the error" << std::endl;
			std::cout << "UserEccentricity.append(" << std::endl;
			UserEccentricity.append("##ERROR_WRITING_OUTPUT##\n");
			UserEccentricity.append("\nPlease raise an issue on the GitHub page with the steps to reproduce it.");
			UserEccentricity.append("https://github.com/Ghostom998/DVLR/issues/");
		}
		UserEccentricity.append(CustEx + "mm");
	}

	return UserEccentricity;
}

const std::string DVLR::PrintEx()
{
	std::string Text = "\nResultant Eccentricity Ex = M/N";
	Text.append("\nor Ex = ((EccDL + EccLL) * Eccentricity) / (EccDL + EccLL + CncDL + CncLL)");

	int x = 0;
	for (int i = 0; i <= 1; i++)
	{
		std::string Ecc = Str(Ex[i]);
		std::string EccDL = Str(Load[i][x + 0]);
		std::string EccLL = Str(Load[i][x + 1]);
		std::string ConcDL = Str(Load[i][x + 2]);
		std::string ConcLL = Str(Load[i][x + 3]);
		std::string LeafThickness = Str(TLeaf[i]);
		std::string UserEcc = Str(Eccentricity[i]);

		Text.append("\nEx,Leaf " + std::to_string(i+1) + " = ((" + EccDL + "kN/m + " + EccLL + "kN/m) * " + UserEcc + "mm)");
		Text.append(" / (" + EccDL + "kN/m + " + EccLL + "kN/m + " + ConcDL + "kN/m + " + ConcLL + "kN/m)");
		Text.append(" = " + Ecc + "mm");
		x += 4;
	}

	return Text;
}

const std::string DVLR::PrintEa()
{
	std::string Text = "\n\nAccidental Eccentricity, Ea = t[1/2400 * SR^2 - 0.015] for SR > 6";

	for(int i = 0 ; i <= 1 ; i++)
	{
		std::string AccEcc = Str(Ea[i]);
		std::string T = Str(TLeaf[i]);
		std::string SlRa = Str(SR);

		Text.append("\nEa,Leaf" + std::to_string(i + 1) + " = " + T + "mm[1/2400 * " + SlRa + "^2 - 0.015] = " + AccEcc + "mm");
	}
	return Text;
}

const std::string DVLR::PrintEt()
{
	std::string Text = "\n\nTotal Eccentricity at 0.4H, Et = 0.6Ex + Ea";

	for(int i = 0 ; i <= 1 ; i++)
	{
		std::string AccEcc = Str(Ea[i]);
		std::string ResEcc = Str(Ex[i]);
		std::string TotEcc = Str(Et[i]);

		Text.append("\nEt,Leaf" + std::to_string(i + 1) + " = (0.6 * " + ResEcc + "mm) + " + AccEcc + "mm = " + TotEcc + "mm");
	}
	return Text;
}

const std::string DVLR::PrintEm()
{
	std::string Text = "\n\nMaximum Eccentricity is the greater of Ex & Et";

	for(int i = 0 ; i <= 1 ; i++)
	{
		std::string ResEcc = Str(Ex[i]);
		std::string TotEcc = Str(Et[i]);
		std::string MaxEcc = Str(Em[i]);
		std::string Symbol;
		(Ex[i] > Et[i]) ? Symbol = ">" : Symbol = ">" ;

		Text.append("\nEx,Leaf" + std::to_string(i + 1) + " " + ResEcc + "mm " + Symbol + " Et,Leaf" + std::to_string(i + 1) + " " + TotEcc + "mm.");
		Text.append(" Hence, Em,Leaf" + std::to_string(i + 1) + " = " + MaxEcc + "mm");
	}
	return Text;
}

const std::string DVLR::PrintBeta()
{
	std::string Text = "\n\nCapacity Reduction Factor, Beta, is given Thus:";
	Text.append("\nBeta = 1.1 * (1 - ((2 * Em) / t))");
	for(int i = 0 ; i <= 1 ; i++)
	{
		std::string B = Str(Beta.Leaf[i]);
		std::string MaxEcc = Str(Em[i]);
		std::string T = Str(TLeaf[i]);

		Text.append("\nBeta,Leaf" + std::to_string(i + 1) + " = 1.1 * (1 - ((2 * " + MaxEcc + "mm)/ " + T + "mm)) = " + B);
	}
	return Text;
}

// TODO Print SAF
const std::string DVLR::PrintSAF()
{
	std::string SmallAreaFactor;
	return SmallAreaFactor;
} 

// TODO Print minimum Fk required
const std::string DVLR::PrintMinFk()
{
	std::string MinFkResult = "\n\nMinimum Fk required is given thus, Fk = (Ym * Wult) / (Beta * SAF * t)";
	for(int i = 0 ; i <= 1 ; i++)
	{
		std::string B = Str(Beta.Leaf[i]);
		std::string Ym = Str(PSF);
		std::string Wult = Str(WultLoad.Leaf[i]);
		std::string SmallAreaFactor = Str(SAF.Leaf[i]);
		std::string LeafThickness = Str(TLeaf[i]);
		std::string Strength = Str(MinFk.Leaf[i]);

		MinFkResult.append("\nFk,Leaf" + std::to_string(i + 1) + " = (" + Ym + " * " + Wult + "kN/m) / (" + B + " * " + SmallAreaFactor + " * " + LeafThickness + "mm)");
		MinFkResult.append(" = " + Strength + "N/mm^2");
	}
	return MinFkResult;
}

const std::string DVLR::PrintLoadingsTopWall()
{
	std::string LoadTopOfWall = "";
	// Display the load in a similar was as we did in the console.
	for (int i = 0; i <= 1; i++) // Leaf
	{
		int x = 0;
		LoadTopOfWall.append("\nLeaf " + std::to_string(i+1) + ",");
		for (int j = 1; j <= 2; j++)
		{
			if (j == 1) { LoadTopOfWall.append("\n  Eccentric, "); }
			else { LoadTopOfWall.append("\n  Concentric, "); }
			for (int k = 1; k <= 2; k++)
			{
				if (k == 1) { LoadTopOfWall.append("\n\tDead load: "); }
				else { LoadTopOfWall.append("\n\tLive load: "); }

				// Write the loads out as a string
				std::string Loads = Str(Load[i][x]);
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
			std::string OpeningWidth = Str( Opening[i].Width);
			std::string BearingLength = Str( Opening[i].BLength);
			// Write to file
			POpening.append("\nOpening Width, OpWidth,Leaf" + std::to_string(i + 1) + ": " + OpeningWidth + "mm");
			POpening.append("\nBearing Length of member supporting Opening, BLength,Leaf" + std::to_string(i + 1) + ": " + BearingLength + "mm");
		}
	}
	return POpening;
}

const std::string DVLR::PrintUltLineLoadTopWall()
{
	std::string UltLineLoad[2] = { Str(LoadOverWall.Leaf[0]) ,Str(LoadOverWall.Leaf[1]) };

	int x = 0;
	std::string UltLineLoadTopWall = "\n\nUltimate Line Load at the top of the wall = 1.4(Ecc,DL + Conc,DL) + 1.6(Ecc,LL + Conc,LL)";
	for (int i = 0; i <= 1; i++)
	{
		std::string EccDL = Str( Load[i][ x ]);
		std::string EccLL = Str( Load[i][x + 1]);
		std::string ConcDL = Str( Load[i][x + 2]);
		std::string ConcLL = Str( Load[i][x + 3]);

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
	std::string Height = Str(HWall, 2, 0.001);
	std::string Pt4Height = Str(PtFourH);

	std::string SelfWt = "\n\nSelfWeight at 0.4H from the top of the wall:";
	SelfWt.append("\n0.4H = 0.4*" + Height + "m = " + Pt4Height + "m");
	SelfWt.append("\nSW,0.4H = Ymas*0.4H*t");

	for (int i = 0; i <= 1; i++)
	{
		// Create String stream
		std::string Ymas = Str( UnitWeight[i]);
		std::string Swt = Str( SelfWeight[i]);
		std::string Thickness = Str( TLeaf[i]);
		// Write to file
		SelfWt.append("\nSW,0.4H,Leaf" + std::to_string(i+1) + " = " + Ymas + "kN/m^3 * (" + Thickness + " / 1000)m * " + Pt4Height + "m = " + Swt + "kN/m");
	}

	return SelfWt;
}

const std::string DVLR::PrintSWWallOver()
{
	std::string Height = Str(HWall, 2, 0.001);
	std::string SWOverOpening[2][2]; // 1st member opening, 2nd member leaf
	for (int i = 0; i <= 1; i++)
	{
		for (int j = 0; j <= 1; j++)
		{
			SWOverOpening[i][j] = Str(SelfWeightOverOpening[i].Leaf[j]);
		}
	}

	std::string SelfWt = "\nSelfWeight of wall above opening(s):";
	SelfWt.append("\nSW,Op = SWeight * (LeafThickness / 1000) * (HeightofWall - OpeningHeight)/1000");

	for (int i = 0; i <= 1; i++) // Opening No.#
	{
		SelfWt.append("\nSelf Weight over Opening " + std::to_string(i + 1) + ":");
		for (int j = 0; j <= 1; j++) // Leaf No.#
		{
			std::string OpHeight = Str(Opening[i].Height, 2, 0.001);
			std::string Ymas = Str(UnitWeight[j]);
			std::string Thickness = Str(TLeaf[j], 2, 0.001);

			SelfWt.append("\n" + Ymas + "kN/m^3 * (" + Thickness + "m * (" + Height + "m - " + OpHeight + "m) = ");
			SelfWt.append(SWOverOpening[i][j] + "kN/m");
		}
	}
	return SelfWt;
}

const std::string DVLR::PrintLoadSpreadLength()
{
	// Create String Stream
	std::string WallHeight = Str( HWall);
	std::string Pt6H = Str( HWall, 2, 0.6 );

	std::string LoadSpeadLength = "\n\nLoad Spread Length, Lspread = BLength + (OpeningHieght - 0.6H) < L";
	LoadSpeadLength.append("\n0.6H = 0.6 * " + WallHeight + "mm = " + Pt6H + "mm");

	for (int i = 0; i <= 1; i++)
	{
		std::string SpreadLength = Str(Opening[i].Spread);
		std::string SpreadLengthMessage = PrintSpreadLengthMessage(i);
		if (Opening[i].IsOpening)
		{
			LoadSpeadLength.append("\nLspread Opening " + std::to_string(i+1) + " = " + SpreadLengthMessage);
			LoadSpeadLength.append("\nTherefore, Lspread Opening " + std::to_string(i + 1) + " = " + SpreadLength + "m");
		}
	}
	return LoadSpeadLength;
}

const std::string DVLR::PrintSpreadLengthMessage(int i)
{
	std::string SpreadLength = Str(Opening[i].Spread);
	std::string WallLength = Str(L);
	std::string PointSixH = Str(HWall, 2, 0.6);
	std::string OpHieght = Str(HWall);
	std::string Blength = Str(Opening[i].BLength);

	std::string LoadSpreadLength = Blength + "mm + (" + OpHieght + "mm - " + PointSixH + "mm) = ";
	if (Opening[i].Spread > L)
	{
		LoadSpreadLength.append(SpreadLength + "mm > " + WallLength + "mm");
		return LoadSpreadLength;
	}
	LoadSpreadLength.append(SpreadLength + "mm < " + WallLength + "mm");
	return LoadSpreadLength;
}

const std::string DVLR::PrintNoLoadSpread()
{
	std::string LoadLeaf[2] = { Str(LoadOverWall.Leaf[0]), Str(LoadOverWall.Leaf[1]) };
	std::string UltLoad[2] = { Str(WultLoad.Leaf[0]), Str(WultLoad.Leaf[1]) };

	std::string NoLoadSpread = "\n\nConsider the ultimate load on the wall:";
	NoLoadSpread.append("\nWult = 1.4*SelfWeight + Wult,TopOfWall");

	for (int i = 0; i <= 1; i++)
	{
		std::string SW = Str(SelfWeight[i]);
		NoLoadSpread.append("\nWult,Leaf" + std::to_string(i+1) + " = 1.4*" + SW + "kN/m + "+ LoadLeaf[i] + "kN/m = " + UltLoad[i] + "kN/m");
	}

	return NoLoadSpread;
}

void DVLR::PrintLoadDoesNotLap(std::string Length, std::string DoubleLoadSpread)
{
	DoubleLoadSpread.append("< ");
	DoubleLoadSpread.append(Length + "mm");
	DoubleLoadSpread.append("\nBoth loads do NOT lap and we will therefore need to design for the greatest concentration.");
	DoubleLoadSpread.append("\nProceeding to the load spread analysis from the greater load at opening " + BiggestOpening);
	std::string SingleSpreadLoad = PrintSingleSpread();
	DoubleLoadSpread.append(SingleSpreadLoad);
}

// TODO - Refactor??
const std::string DVLR::PrintDoubleLoadSpread()
{
	std::string SpreadLength[2] = { Str(Opening[0].Spread), Str(Opening[1].Spread) };
	double AddSpreadLength = Opening[0].Spread + Opening[1].Spread;
	std::string SpreadLengthSum = Str(AddSpreadLength);
	std::string Length = Str(L);

	std::string DoubleLoadSpread = "\n\nCheck whether the two load spreads lap:";
	DoubleLoadSpread.append("\nLoads lap if: Lspread1 + Lspread2 > L");
	DoubleLoadSpread.append("\n" + SpreadLength[0] + "mm + " + SpreadLength[1] + "mm = " + SpreadLengthSum + "mm ");
	if (SpreadCaseStatus == SpreadCase::DblLoadSpreadDoesNOTLap)
	{
		PrintLoadDoesNotLap(Length, DoubleLoadSpread);
	}
	else
	{
		// Call the string stream only if branching to double load spread calc
		std::string UltTopWall[2] = { Str(LoadOverWall.Leaf[0]), Str(LoadOverWall.Leaf[1]) };
		std::string OpeningWidth[2] = { Str(Opening[0].Width), Str(Opening[1].Width) };
		std::string Wult[2] = { Str(WultLoad.Leaf[0]), Str(WultLoad.Leaf[1]) };
		std::string SWOverOpening[2][2]; // 1st member opening, 2nd member leaf
		for (int i = 0; i <= 1; i++)
		{
			for (int j = 0; j <= 1; j++)
			{
				SWOverOpening[i][j] = Str(SelfWeightOverOpening[i].Leaf[j]);
			}
		}

		DoubleLoadSpread.append("> ");
		DoubleLoadSpread.append(Length + "mm");
		DoubleLoadSpread.append("\nAnd hence both load spreads lap. \nWult = ");
		DoubleLoadSpread.append("(Wult,TopOfWall + (1.4 * Selfweight@0.4H)) + (Wult,TopOfWall + (1.4*SelfWeightOverOpening)) * (OpenWidth1 / 1000)) / (2 * (Lspread1 / 1000)) + ");
		DoubleLoadSpread.append("(Wult,TopOfWall * (OpenWidth2 / 1000)) / (2 * (Lspread2 / 1000))");
		for (int i = 0; i <= 1; i++) // Loops per Leaf
		{
			std::string SW = Str(SelfWeight[i], 2);
			DoubleLoadSpread.append("\nWult,Leaf" + std::to_string(i + 1) + " = (");
			DoubleLoadSpread.append(UltTopWall[i] + "kN/m + (1.4*" + SW + "kN/m)) + ((" + UltTopWall[i] + "kN/m + (1.4 * " + SWOverOpening[0][i] + "kN/m)) * (" + OpeningWidth[0] + "mm / 1000)) ");
			DoubleLoadSpread.append("/ (2 * (" + SpreadLength[0] + "mm / 1000)) + (" + UltTopWall[i] + "kN/m + (1.4 * " + SWOverOpening[1][i] + "kN/m)) * (" + OpeningWidth[1] + "mm / 1000)) / (2 * (");
			DoubleLoadSpread.append(SpreadLength[1] + "mm / 1000))");
			DoubleLoadSpread.append("\n= " + Wult[i] + "kN/m");
		}
	}
	return DoubleLoadSpread;
}

const std::string DVLR::PrintSingleSpread()
{
	std::string Length = Str( L);
	// Values based on greatest opening size
	std::string UltTopWall[2] = { Str( LoadOverWall.Leaf[0]), Str(LoadOverWall.Leaf[1]) };
	std::string Wult[2] = { Str(WultLoad.Leaf[0]), Str(WultLoad.Leaf[1]) };

	int x;
	// Assign string operators based on greatest load
	(Opening[0].Width >= Opening[1].Width) ? x = 0 : x = 1;
	BiggestOpening = std::to_string(x+1);
	std::string SpreadLength = Str(Opening[x].Spread);
	std::string OpeningWidth = Str(Opening[x].Spread);
	std::string SWOO[2] = { Str(SelfWeightOverOpening[x].Leaf[0]), Str(SelfWeightOverOpening[x].Leaf[1]) };

	// Begin to write content
	std::string SingleLoadSpread = "\n\nConsider the load spread from opening " + BiggestOpening + ":";
	SingleLoadSpread.append("\nWult = (Wult,TopOfWall + (1.4 * Selfweight@0.4H)) + ((Wult,TopOfWall + (1.4*SelfWeightOverOpening)) * (OpenWidth" + BiggestOpening + " / 1000)) / (2 * (Lspread" + BiggestOpening + " / 1000))");

	for (int i = 0; i <= 1; i++) // Loops per Leaf
	{
		std::string SW = Str(SelfWeight[i]);
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
	auto MinFkSup = PrintLoadAtSupport();
	
	// Print the factor
	MinFkSup.append(PrintSupportFactor());

	// Print MinFkSup

	return MinFkSup;
}

const std::string DVLR::PrintLoadAtSupport()
{
	std::string Wult[2] = { Str(LoadOverWall.Leaf[0]), Str(LoadOverWall.Leaf[1]) };
	std::string SWOverOpening[2][2]; // 1st member opening, 2nd member leaf
	std::string Wsup[2][2];  // 1st member opening, 2nd member leaf
	for (int i = 0; i <= 1; i++)
	{
		for (int j = 0; j <= 1; j++)
		{
			SWOverOpening[i][j] = Str(SelfWeightOverOpening[i].Leaf[j]);
			Wsup[i][j] = Str(LoadAtSupport[i].Leaf[j]);
		}
	}
	std::string MinFkSup = "\nDetermine load at support.\n";
	MinFkSup.append("Wsup = (Wult+SWOverOpening)*((OpLength*0.5)+BLength)\n");

	// Print Load at Support Calculation
	for (int i = 0; i <= 1; i++) // Per Opening
	{
		MinFkSup.append("Opening " + std::to_string(i + 1) + ":\n");
		for (int j = 0; j <= 1; j++) // Per Leaf
		{
			std::string OpBLength = Str(Opening[i].BLength, 3, 0.001);
			std::string OpLength = Str(Opening[i].Width, 3, 0.001);

			MinFkSup.append("  Leaf " + std::to_string(j + 1) + " = (" + Wult[i] + "kN/m + " + SWOverOpening[i][j] + "kN/m");
			MinFkSup.append("*((" + OpLength + "m * 0.5)+" + OpBLength + "m) = " + Wsup[i][j] + "kN\n");
		}
	}
	return MinFkSup;
}

const std::string DVLR::PrintSupportFactor()
{
	std::string SupportFactorText;
	std::string Condition;

	for (int i = 0; i <= 1; i++) // Per Opening
	{
		std::string BearingLength = Str(Opening[i].BLength, 0);
		for (int j = 0; j <= 1; j++) // Per Leaf
		{
			std::string BearCoeff = Str(MinBearCoeff[i].Leaf[j]);
			if (Opening[i].BLength < (2 * TLeaf[j]))
			{
				Condition = " < 2t";
				
			}
			if (Opening[i].BLength < (3 * TLeaf[j]))
			{
				Condition = " < 3t";
			}
			else
			{
				Condition = " > 2t";
			}
			SupportFactorText.append("\nBearing Length at opening " + std::to_string(i+1) + ", leaf " + std::to_string(j+1) + " = " + BearingLength);
			SupportFactorText.append(Condition + ", therefore, Bearing Capacity Coefficient = " + BearCoeff);
		}
	}
	return SupportFactorText;
}
