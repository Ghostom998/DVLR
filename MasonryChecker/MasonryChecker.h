#ifndef DVLR_H // Inclusion guards
#define DVLR_H

#include <string>

// Ultimate loading per leaf. Allows functions to return two values. Values initialised to 0
struct Wult
{
	float Leaf1 = 0;
	float Leaf2 = 0;
};

// Required Fk per leaf. Allows functions to return two values. Values initialised to 0
struct Fk
{
	float Leaf1 = 0;
	float Leaf2 = 0;
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

	const float GetSlenderness();
		const float GetTeff();
		const float GetHeff();
			const float GetRestraint();
	const void IsSlendernessOK(float&);

	const double GetSafetyFactor();
		int GetConstrControl();
		int GetManufControl();

	const float GetUltLoad();
	const float GetBeta();
	const float GetSAF(float&, float&);
	const float GetMinFk();

	// Sub-member functions (i.e. members called from the main members above and not from main)

	const float GetSelfWeight();
	const bool DoesLoadSpreadLap();
	const float GetSpreadLoad();

	//	bool CheckValidInput(); commented out until implemented

private: // Members

	/// Leaf thicknesses
	float TLeaf[2] = { 0, 0 };
	float Teff = 0;
	std::string TeffEquation = "";
	/// Wall hieght
	float HWall = 0;
	/// Heff Restraint Factor
	float RestraintFactor = 0;
	/// Wall effective hieght
	float Heff = 0;
	/// Slenderness Ratio
	float SR = 0;
	/// Partial Safety Factor
	double PSF = 0;
	/// DL, LL, eccentric, concentric, leaf 1, leaf 2
	float Load[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	/// Ultimate Design Load
	float Wult = 0;
	/// Opening widths
	float OpenWidth[2] = { 0, 0 };

};

#endif // DVLR_H
