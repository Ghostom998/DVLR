#ifndef DVLR_H // Inclusion guards
#define DVLR_H

#include <string>

// Ultimate loading per leaf. Allows functions to return two values. Values initialised to 0
struct Wult
{
	double Leaf1 = 0;
	double Leaf2 = 0;
	std::string Message = "";
};

// Required Fk per leaf. Allows functions to return two values. Values initialised to 0
struct Fk
{
	double Leaf1 = 0;
	double Leaf2 = 0;
};

enum class SpreadCase
{
	Invalid_status,
	NoLoadSpread,
	DblLoadSpreadLaps,
	DblLoadSpreadDoesNOTLap,
	SglLoadSpread
};

class DVLR
{
public: // Methods

	DVLR(); // Constructor, sets the default type

			/* Note: Methods (or class member functions) below are shown in heirarchy, i.e. in
			/	GetSlenderness();
			/	->	GetHeff();
			/	-->		GetRestraint();
			/	GetSlenderness calls GetHeff which calls GetRestraint
			/	this is not a requirement in C++, but makes it easier to track the program from the header file
			*/

	void StartProgram();

	const double GetSlenderness();
		const double GetTeff();
		const double GetHeff();
			const double GetRestraint();
	void IsSlendernessOK(double&);

	const double GetSafetyFactor();
		int GetConstrControl();
		int GetManufControl();

	// TODO Include for out of plane wall load I.E. beam perpendicular to wall
	// Point load with independant load. Maybe ask user if beam is present to prevent unnecessary output.
	Wult GetUltLoad();
		void GetLoads();
		void GetOpenings();
			double SpreadLength(double, double, double, int);
		Wult GetSpreadLoad();
		const void GetSelfWeight();
		//const Wult GetSelfWeightOverOpening(double*, double, double);
		const double GetSingleLapLoad(double, double, double*);
		const double GetDoubleLapLoad(double, double, double*);
		Wult GetUltLineLoad(double*);

	const Wult GetBeta();
		// TODO Method to ask user if they would like to use the default value of t/6? (y/n/help)
		// and if not, enter a custom value per leaf. a help function will elaborate on recomended values:
		// 1. Default: t/6 for simply supported joists and slabs bearing onto the full width of the wall. 
		// 2. t/3 for slabs or joists spanning continuously over the wall,
		// 3. t/2 for joists on hangars or similar condition 
		// 4. t/2-l/3 for simply supported joists and slabs NOT bearing onto the full width of the wall
		// 5. A custom value. Note that entering a zero value will result in a minimum eccentricity of 0.05t being used.
		// (produce case, loop with default asking for a valid answer).
		const double GetEx(double, double, double, double, double);

	const Wult GetSmallAreaFactor();
		const double GetSAF(double&, double&);

	const double GetMinFk(double&, double&, double&, double&, double&);

	// TODO Method to determine minFk based on bearing beneath the lintel const CheckLintelBearing();
	// BLength < 2t => 1.5fk/ym  ;  BLength < 3t => 1.25fk/ym

// Methods to print *.txt output

	// PrintToFile() will take the file name, run the sub methods, run error checks and close the file at the end.
	// Each sub method will write to the file at the end of its method
	// We will have PrintToFile() display if the file write was successfully or warn that it wasnt. We may ask the user if they wish to TryAgain?
	// The file will then exit to main which will return 0;
	const int PrintToFile();

		// Print the introduction section
		const std::string PrintIntro(std::string);

		// Print the slenderness ratio and its components
		const std::string PrintSlenderness();

		// Print the safety factor and quality control
		const std::string PrintPSF();

		// Print the loadings openings and concentrated loads
		const std::string PrintLoadings();
			const std::string PrintLoadingsTopWall();
			const std::string PrintOpenings();
			const std::string PrintUltLineLoadTopWall();
			const std::string PrintSelfWeight();
			const std::string PrintLoadSpreadLength();
			const std::string PrintNoLoadSpread();
			const std::string PrintDoubleLoadSpread();
			const std::string PrintNotDoubleLoadSpread();
			const std::string PrintSingleSpread();

		// Print the eccentricity and the capacity reduction factor
		const std::string PrintEccentricity();


		const std::string PrintMinFk();



private: // Members

	/// Leaf thicknesses
	double TLeaf[2] = { 0, 0 };
	double Teff = 0;
	std::string TeffEquation = "";
	/// Wall hieght
	double HWall = 0;
	/// Restraint Condition i.e. Enhanced for txt output
	std::string RestraintCondition = "";
	/// Heff Restraint Factor
	double RestraintFactor = 0;
	/// Wall effective hieght
	double Heff = 0;
	/// 0.4H from the top of the wall
	double PtFourH = 0;
	/// Length of wall considered
	double L = 0;
	/// Opening width
	double OpWidth[2] = { 0, 0 };
	/// Bearing length
	double BLength[2] = { 0, 0 };
	/// Height to the top of the opening
	// double OpHeight[2] = { 0, 0 };
	/// Slenderness Ratio
	double SR = 0;
	/// Partial Safety Factor
	double PSF = 0;
	/// Construction control
	std::string ConstructionControl = "";
	/// Manufacture Control
	std::string ManufactureControl = "";
	/// DL, LL, eccentric, concentric, leaf 1, leaf 2
	double Load[8] =
	{
		0, // 0. Leaf 1, Eccentric, Dead load
		0, // 1. Leaf 1, Eccentric, Live load
		0, // 2. Leaf 1, Concentric, Dead load
		0, // 3. Leaf 1, Concentric, Live load
		0, // 4. Leaf 2, Eccentric, Dead load
		0, // 5. Leaf 2 Eccentric, Live load
		0, // 6. Leaf 2 Concentric, Dead load
		0  // 7. Leaf 2 Concentric, Live load
	};
	/// Masonry Self weight
	double UnitWeight[2] = { 0 , 0 };
	double SelfWeight[2] = { 0 , 0 };
	//Wult SelfWeightOverOpening[2] = { 0 , 0 };
	/// Ultimate Load
	Wult WultLoad;
	/// Factored load over wall w/o wall self weight and load concentration
	Wult LoadOverWall;
	/// Load Spread Length, assumed at 45 degrees from edge of member bearing
	double Spread[2] = {0, 0};
	/// Loaded Eccentricity
	double Ex[2] = { 0, 0 };
	double Ea[2] = { 0, 0 };
	double Et[2] = { 0, 0 };
	double Em[2] = { 0, 0 };
	/// Capacity reduction factor
	Wult Beta;
	/// Small area factor
	Wult SAF;
	/// Minimum required masonry strength
	Fk MinFk;
	/// Displays the correct message in the txt output
	std::string SpreadLengthMessage[2];
	/// A default case which should indicate whether or not the program is properly assigning 
	SpreadCase SpreadCaseStatus = SpreadCase::Invalid_status;

};

#endif // DVLR_H
