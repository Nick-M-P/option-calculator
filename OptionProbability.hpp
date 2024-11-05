//Header for probability related functions to use in option pricing formulae

//#include <boost/math/distributions/normal.hpp>
//using namespace boost::math;

#include <cmath>

#ifndef Option_Probability_HPP
#define Option_Probability_HPP

namespace OptionProbability {

	static const double PI = 3.141592653589793238463;
	static const double PI_base = (1 / pow(2 * PI, 0.5));	//for standard normal cdf
	static double tol = pow(10, -12);						//tolerance level for Simpson's rule

	double rule_simpson(							//numerical approximation of definite integral
		double a, double b, double n, double (*func)(double));

	double std_N(double x);							//integrated part of standard normal pdf

	double simpson_cdf(double x);					//calculate Standard normal CDF using Simpson's rule

	double n(double x);								//standrad normal PDF

	double N(double x);								//standardized notation for standrad normal CDF calculation

}

#endif
