//A plain european option class that calculates option prices, greeks and checks for put-call parity

#include "Option.hpp"
#include <iostream>

#ifndef European_Option_HPP
#define European_Option_HPP

class EuropeanOption : public Option
{
private:
	double S_val = 0;							//option pricing parameters
	double K_val = 0;
	double T_val = 1;
	double r_val = 0.05;
	double vol_val = 0.3;

	double d1() const;							//get d1 and d2 values for B-S formula
	double d2() const;
public:
	//Constructors and destructor
	EuropeanOption();
	EuropeanOption(const EuropeanOption& o);
	virtual ~EuropeanOption();

	//Selectors
	double S() const;
	double K() const;
	double T() const;
	double r() const;
	double vol() const;

	//Modifiers
	void S(double newS);
	void K(double newK);
	void T(double newT);
	void r(double newr);
	void vol(double newvol);
	void SetValues(const std::vector<double>& params);//set values from a vector of parameters
	void Print();								//prints values of all data members

	//Operator overloading
	EuropeanOption& operator=(const EuropeanOption& src);

	//B&S option prices
	double Call() const;						//get B-S price for call option
	double Put() const;							//get B-S price for put option

	//Put-Call parity
	bool Parity(double P, double C) const;		//check if a put-call pair satisfies PCP equation (with 0.1 tol)
	double Parity() const;						//return option price opposite to type_val, calculated from PCP

	//Sensitivities
	double DeltaCall() const;					//get delta value of a call option
	double DeltaPut() const;					//get delta value of a put option
	double Delta() const;						//get delta value given the internal type
	double Gamma() const;						//get gamma value of an option

	//Approximate sensitivities using divided differences approach (with tol 10^-6)
	double approximated_DeltaCall();
	double approximated_DeltaPut();
	double approximated_Delta();
	double approximated_Gamma();
};

#endif