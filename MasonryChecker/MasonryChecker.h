#ifndef DVLR_H // Inclusion guards
#define DVLR_H

#include <string>

// Ultimate loading per leaf. Allows functions to return two values. Values initialized to 0
struct Wult
{
	double Leaf1 = 0;
	double Leaf2 = 0;
	std::string Message = "";
};

// Required Fk per leaf. Allows functions to return two values. Values initialized to 0
struct Fk
{
	double Leaf1 = 0;
	double Leaf2 = 0;
};

// Opening object created in the DVLR class
struct StructuralOpenings
{
	// Assumes there is no opening unless the user specifically inputs otherwise
	bool IsOpening = false; 
	double Width = 0;
	double BLength = 0;
	double Height = 0;
	double Spread = 0;
};

enum class SpreadCase
{
	Invalid_status,
	NoLoadSpread,
	DblLoadSpreadLaps,
	DblLoadSpreadDoesNOTLap,
	SglLoadSpread
};

enum class UserEccentricity
{
	Invalid_Status,
	FullBearing,
	Continuous,
	Hangars,
	NotFullBearing,
	Custom
};

class DVLR
{
public: // Methods

	DVLR(); // Constructor, sets the default type

			/* Note: Methods (or class member functions) below are shown in hierarchy, i.e. in
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
	static void IsSlendernessOK(double&);

	const double GetSafetyFactor();
		int GetConstrControl();
		int GetManufControl();

	// TODO Include for out of plane wall load I.E. beam perpendicular to wall
	// Point load with independent load. Maybe ask user if beam is present to prevent unnecessary output.
	Wult GetUltLoad();
		void GetLoads();
		void GetOpenings();
			double SpreadLength(double, double, double, double, int);
		Wult GetSpreadLoad();
		const void GetSelfWeight();
		const Wult GetSelfWeightOverOpening(double*, double*, double, double, int);
		const double GetSingleLapLoad(double, double, struct StructuralOpenings OpenWidth[2], double, double);
		const double GetDoubleLapLoad(double, double, struct StructuralOpenings OpenWidth[2], double, double);
		Wult GetUltLineLoad(double*);

	const Wult GetBeta();
		
		const bool IsEccentricityDefault();
		const double GetUserEccentricity(double, int);
		const double CustomBearing(double, int);
		const double GetCustomEccentricity(double, int);
		const double GetEx(double, double, double, double, double, double);

	const Wult GetSmallAreaFactor();
		const double GetSAF(double&, double&);

	const double GetMinFk(double&, double&, double&, double&, double&);

	const double CheckLintelBearing();
	// BLength < 2t = 1.5fk/ym  ;  BLength < 3t => 1.25fk/ym

// Methods to print *.txt output

	// The main print method. Controls and checks the other sub methods listed below.
	const int PrintToFile();

		// Print the introduction section
	static const std::string PrintIntro(std::string);

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
			const std::string PrintSWWallOver();
			const std::string PrintLoadSpreadLength();
			const std::string PrintNoLoadSpread();
			const std::string PrintDoubleLoadSpread();
			const std::string PrintSingleSpread();

		// Print the eccentricity, SAF and the capacity reduction factor
		const std::string PrintEccentricity();

		const std::string PrintSAF();

		const std::string PrintMinFk();

private: // Members

	/// Leaf thicknesses
	double TLeaf[2] = { 0, 0 };
	double Teff = 0;
	std::string TeffEquation = "";
	/// Wall height
	double HWall = 0;
	/// Restraint Condition i.e. Enhanced for text output
	std::string RestraintCondition = "";
	/// Heff Restraint Factor
	double RestraintFactor = 0;
	/// Wall effective height
	double Heff = 0;
	/// 0.4H from the top of the wall
	double PtFourH = 0;
	/// Length of wall considered
	double L = 0;
	/// Opening structure. Contains opening widths, heights, load spread lengths and bearing lengths. Member 0 = opening 1, 1 = opening 2.
	StructuralOpenings Opening[2];
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
	Wult SelfWeightOverOpening[2] = { 0 , 0 };
	/// Ultimate Load
	Wult WultLoad;
	/// Factored load over wall w/o wall self weight and load concentration
	Wult LoadOverWall;
	/// Loaded Eccentricity
	UserEccentricity Selection = UserEccentricity::Invalid_Status;
	double CustomEccentricity[2] = { 0 , 0 };
	double Eccentricity[2] = { 0 , 0 };
	double Ex[2] = { 0, 0 };
	double Ea[2] = { 0, 0 };
	double Et[2] = { 0, 0 };
	double Em[2] = { 0, 0 };
	/// Custom Bearing Length of joists / slab
	double BearingLength[2] = { 0 , 0 };
	/// Capacity reduction factor
	Wult Beta;
	/// Small area factor
	Wult SAF;
	/// Minimum required masonry strength
	Fk MinFk;
	/// Displays the correct message in the text output
	std::string SpreadLengthMessage[2];
	/// Displays the greatest opening size to tell the user which is causing the greatest load concentration.
	std::string BiggestOpening;
	/// A default case which should indicate whether or not the program is properly assigning the case
	SpreadCase SpreadCaseStatus = SpreadCase::Invalid_status;

};

#endif // DVLR_H
