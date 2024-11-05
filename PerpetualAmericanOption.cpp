//A perpetual american option class that can calculate option prices

#include "PerpetualAmericanOption.hpp"
#include <cmath>

PerpetualAmericanOption::PerpetualAmericanOption()							//default constructor has just initialized data members
	:Option()
{
}

PerpetualAmericanOption::PerpetualAmericanOption(const PerpetualAmericanOption& o)		//copy constructor copies all data mebers
	:S_val(o.S_val), K_val(o.K_val), r_val(o.r_val),
	b_val(o.b_val), vol_val(o.vol_val)
{
	if (type() != o.type())
		toggle();
}

//Destructor
PerpetualAmericanOption::~PerpetualAmericanOption()
{
}

//Selectors (get value of respective data member)
double PerpetualAmericanOption::S() const { return S_val; }
double PerpetualAmericanOption::K() const { return K_val; }
double PerpetualAmericanOption::b() const { return b_val; }
double PerpetualAmericanOption::r() const { return r_val; }
double PerpetualAmericanOption::vol() const { return vol_val; }

//Modifiers (also check for negative input)
void PerpetualAmericanOption::S(double newS)
{
	if (newS < 0)
	{
		std::cout << "Underlying price can't be negative, setting to 0.\n";
		S_val = 0;
	}
	else
		S_val = newS;
}

void PerpetualAmericanOption::K(double newK)
{
	if (newK < 0)
	{
		std::cout << "Strike price can't be negative, setting to 0.\n";
		K_val = 0;
	}
	else
		K_val = newK;
}

void PerpetualAmericanOption::b(double newb)
{
	if (newb < 0)
	{
		std::cout << "Cost of carry can't be negative, setting to 5%.\n";
		b_val = 0.05;
	}
	else
		b_val = newb;
}

void PerpetualAmericanOption::r(double newr)
{
	if (newr < 0)
	{
		std::cout << "BS doesn't work with negative interest rate, setting to 5%.\n";
		r_val = 0.05;
	}
	else
		r_val = newr;
}

void PerpetualAmericanOption::vol(double newvol)
{
	if (newvol < 0)
	{
		std::cout << "Volatility can't be negative, setting to 30%.\n";
		S_val = 0;
	}
	else
		vol_val = newvol;
}

void PerpetualAmericanOption::SetValues(const std::vector<double>& params)
{
	S_val = params[0];
	K_val = params[1];
	r_val = params[2];
	b_val = params[3];
	vol_val = params[4];
}

//Operator overloading
PerpetualAmericanOption&
PerpetualAmericanOption::operator=(const PerpetualAmericanOption& src)		//assignment operator checks for self-assignment
{
	if (this == &src)
		return *this;
	else
	{
		S_val = src.S_val;
		K_val = src.K_val;
		b_val = src.b_val;
		r_val = src.r_val;
		vol_val = src.vol_val;
		if (type() != src.type())
			toggle();

		return *this;
	}
}

//Option prices
double PerpetualAmericanOption::Call() const								//get price for call option
{
	double y1 = 0.5 - b_val / (vol_val * vol_val) +
		sqrt(pow(b_val / (vol_val * vol_val) - 0.5, 2) + 2 * r_val / (vol_val * vol_val));
	
	return (K_val / (y1 - 1)) * pow(((y1-1)/y1)*(S_val/K_val), y1);
}

double PerpetualAmericanOption::Put() const									//get price for put option
{
	double y2 = 0.5 - b_val / (vol_val * vol_val) -
		sqrt(pow(b_val / (vol_val * vol_val) - 0.5, 2) + 2 * r_val / (vol_val * vol_val));

	return (K_val / (1 - y2)) * pow(((y2 - 1) / y2) * (S_val / K_val), y2);
}

