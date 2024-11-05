//A perpetual american option class that can calculate option prices

#include "Option.hpp"
#include <iostream>

#ifndef Perpetual_American_Option_HPP
#define Perpetual_American_Option_HPP

class PerpetualAmericanOption : public Option
{
private:
	double S_val = 0;							//option pricing parameters
	double K_val = 0;
	double r_val = 0.05;
	double b_val = 0.06;
	double vol_val = 0.3;

public:
	//Constructors and destructor
	PerpetualAmericanOption();
	PerpetualAmericanOption(const PerpetualAmericanOption& o);
	virtual ~PerpetualAmericanOption();

	//Selectors
	double S() const;
	double K() const;
	double r() const;
	double b() const;
	double vol() const;

	//Modifiers
	void S(double newS);
	void K(double newK);
	void r(double newr);
	void b(double newb);
	void vol(double newvol);
	void SetValues(const std::vector<double>& params);//set values from a vector of parameters

	//Operator overloading
	PerpetualAmericanOption& operator=(const PerpetualAmericanOption& src);

	//Option prices
	double Call() const;						//get price for call option
	double Put() const;							//get price for put option
};

#endif
