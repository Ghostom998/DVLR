#ifndef MASONRYCHECKER_H
#define MASONRYCHECKER_H

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
public:

	DVLR(); // Constructor

			// Main member functions
	const float GetSlenderness();
	const float GetUltLoad();
	const float GetBeta();
	const float GetSAF(float&, float&);
	const float GetMinFk();

	// Sub-member functions (i.e. members called from the main members above and not from main)
	const float GetTeff();
	const float GetHeff();
	const float GetRestraint();
	const double GetSafetyFactor();
	const float GetSelfWeight();

	const bool DoesLoadSpreadLap();
	const float GetSpreadLoad();

	//	bool CheckValidInput(); commented out until implemented

private:

	/// Leaf thicknesses
	float TLeaf[2] = { 0, 0 };
	float Teff = 0;
	/// Wall hieght
	float HWall = 0;
	/// Wall effective hieght
	float Heff = 0;
	/// Partial Safety Factor
	double PSF = 0;
	/// DL, LL, eccentric, concentric, leaf 1, leaf 2
	float Load[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	/// Opening widths
	float OpenWidth[2] = { 0, 0 };

};

#endif // MASONRYCHECKER_H
