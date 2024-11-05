//A plain european option class that calculates option prices, greeks and checks for put-call parity

#include "EuropeanOption.hpp"
#include "OptionProbability.hpp"

//Constructors

EuropeanOption::EuropeanOption()							//default constructor has just initialized data members
	:Option()
{
}

EuropeanOption::EuropeanOption(const EuropeanOption& o)		//copy constructor copies all data mebers
	:S_val(o.S_val), K_val(o.K_val), T_val(o.T_val),
	r_val(o.r_val), vol_val(o.vol_val)
{
	if (type() != o.type()) toggle();
}

//Destructor
EuropeanOption::~EuropeanOption()
{
}

//Selectors (get value of respective data member)
double EuropeanOption::S() const { return S_val; }
double EuropeanOption::K() const { return K_val; }
double EuropeanOption::T() const { return T_val; }
double EuropeanOption::r() const { return r_val; }
double EuropeanOption::vol() const { return vol_val; }

//Modifiers (also check for negative input)
void EuropeanOption::S(double newS)
{
	if (newS < 0)
	{
		std::cout << "Underlying price can't be negative, setting to 1.\n";
		S_val = 1;
	} else S_val = newS;
}

void EuropeanOption::K(double newK)
{
	if (newK < 0)
	{
		std::cout << "Strike price can't be negative, setting to 0.\n";
		K_val = 0;
	} else K_val = newK;
}

void EuropeanOption::T(double newT)
{
	if (newT < 0)
	{
		std::cout << "Time to maturity can't be negative, setting to 1.\n";
		T_val = 1;
	} else T_val = newT;
}

void EuropeanOption::r(double newr)
{
	if (newr < 0)
	{
		std::cout << "B-S model doesn't work with negative interest rate, setting to 5%.\n";
		r_val = 0.05;
	} else r_val = newr;
}

void EuropeanOption::vol(double newvol)
{
	if (newvol < 0)
	{
		std::cout << "Volatility can't be negative, setting to 30%.\n";
		vol_val = 0.3;
	} else vol_val = newvol;
}

void EuropeanOption::SetValues(const std::vector<double>& params)	//get values from a vector (need to be ordered according to data members)
{
	S_val = params[0];
	K_val = params[1];
	T_val = params[2];
	r_val = params[3];
	vol_val = params[4];
}

void EuropeanOption::Print()								//prints option info
{
	std::cout << "A " << ((type() == 'C') ? ("call") : ("put")) << " option "
		"with strike price at " << K_val << " ,expiring in " << T_val << " year(s).\n"
		"Its underlying price is " << S_val << " and volatility is " << vol_val << " %.\n"
		"Interest rate is " << r_val << " %.\n"
		"The option price is " << Price() << " .\n";
}

//Operator overloading
EuropeanOption&
EuropeanOption::operator=(const EuropeanOption& src)		//assignment operator checks for self-assignment
{
	if (this == &src)
		return *this;
	else
	{
		S_val = src.S_val;
		K_val = src.K_val;
		T_val = src.T_val;
		r_val = src.r_val;
		vol_val = src.vol_val;
		if (type() != src.type())
			toggle();

		return *this;
	}
}

double EuropeanOption::d1() const							//get d1 value for B-S pricing formula
{
	return (log(S_val / K_val) + (r_val + vol_val * vol_val / 2) * T_val) / (vol_val * sqrt(T_val));
}

double EuropeanOption::d2() const							//get d2 value for B-S pricing formula
{
	return d1() - (vol_val * sqrt(T_val));
}

//B-S option prices
double EuropeanOption::Call() const							//get B-S price for call option
{
	return S_val * OptionProbability::N(d1()) - K_val * exp(-r_val * T_val) * OptionProbability::N(d2());
}

double EuropeanOption::Put() const							//get B-S price for put option
{
	return K_val * exp(-r_val * T_val) * OptionProbability::N(-d2()) - S_val * OptionProbability::N(-d1());
}

//Put-Call parity
bool EuropeanOption::Parity(double P, double C) const		//check if a put-call pair satisfies PCP equation (with 0.1 tol)
{
	return (abs(C + K_val * exp(-r_val * T_val) - P - S_val) < 0.1);
}

double EuropeanOption::Parity() const						//return option price opposite to type_val, calculated from PCP
{
	if (type() == 'C')
		return Price() + K_val * exp(-r_val * T_val) - S_val;
	else
		return Price() + S_val - K_val * exp(-r_val * T_val);
}

//Sensitivities
double EuropeanOption::DeltaCall() const					//get delta value of a call option
{
	return OptionProbability::N(d1());
}

double EuropeanOption::DeltaPut() const						//get delta value of a put option
{
	return - OptionProbability::N(-d1());
}

double EuropeanOption::Delta() const						//get delta value given the internal type
{
	if (type() == 'C')
		return DeltaCall();
	else
		return DeltaPut();
}

double EuropeanOption::Gamma() const						//get gamma value of an option
{
	return OptionProbability::n(d1()) / (S_val * vol_val * sqrt(T_val));
}

//Approximate sensitivities using divided differences approach (with tol 10^-6)
double EuropeanOption::approximated_DeltaCall()
{
	double h = 1;											//set initial step to 1
	double tol = 0.000001;
	double val_old;
	double val_new;

	double p1;
	double p2;

	S_val += h;												//get option prices for S +- h
	p1 = Call();
	S_val -= h * 2;
	p2 = Call();
	S_val += h;
	val_old = (p1 - p2) / (2 * h);

	while (true)											//start a loop where we decrease h by power of 10
	{
		h = h / 10.0;

		S_val += h;											//get new option prices
		p1 = Call();
		S_val -= h * 2;
		p2 = Call();
		S_val += h;
		val_new = (p1 - p2) / (2 * h);

		if (abs(val_new - val_old) > tol)					//keep iterating while change of the resulting value from reduced step is higher than tol
			val_old = val_new;
		else
			break;
	}
	return val_new;
}

double EuropeanOption::approximated_DeltaPut()				//the same approach for the rest of the approximated greeks
{
	double h = 1;
	double tol = 0.000001;
	double val_old;
	double val_new;

	double p1;
	double p2;

	S_val += h;
	p1 = Put();
	S_val -= h * 2;
	p2 = Put();
	S_val += h;
	val_old = (p1 - p2) / (2 * h);

	while (true)
	{
		h = h / 10.0;
		S_val += h;
		p1 = Put();
		S_val -= h * 2;
		p2 = Put();
		S_val += h;
		val_new = (p1 - p2) / (2 * h);

		if (abs(val_new - val_old) > tol)
			val_old = val_new;
		else
			break;
	}
	return val_new;
}

double EuropeanOption::approximated_Delta()
{
	if (type() == 'C')
		return approximated_DeltaCall();
	else
		return approximated_DeltaPut();
}

double EuropeanOption::approximated_Gamma()
{
	double h = 1;
	double tol = 0.000001;
	double val_old;
	double val_new;

	double p;
	double p1;
	double p2;

	p = Call();
	S_val += h;
	p1 = Call();
	S_val -= h * 2;
	p2 = Call();
	S_val += h;
	val_old = (p1 - 2 * p + p2) / (h * h);

	while (true)
	{
		h = h / 10.0;
		S_val += h;
		p1 = Call();
		S_val -= h * 2;
		p2 = Call();
		S_val += h;
		val_new = (p1 - 2 * p + p2) / (h * h);

		if (abs(val_new - val_old) > tol)
			val_old = val_new;
		else
			break;
	}
	return val_new;
}