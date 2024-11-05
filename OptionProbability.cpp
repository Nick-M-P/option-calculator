//This file implements probability related functions to use in option pricing formulae

#include "OptionProbability.hpp"

namespace OptionProbability {

    double rule_simpson(                            //numerical approximation of definite integral
        double a, double b, double n, double (*func)(double))
    {
        double h = (b - a) / n;
        double res = 0;
        res += func(a);
        res += func(b);

        double sum = 0;

        for (int i = 1; i <= n - 1; i++)
        {
            double a_i = a + h * i;
            sum += func(a_i);
        }

        res += (sum * 2);

        double sum_xi = 0;

        for (int i = 1; i <= n; i++)
        {
            double a_l = a + h * (i - 1);
            double a_r = a + h * i;
            double x_i = (a_l + a_r) / 2;
            sum_xi += func(x_i);
        }

        res += (sum_xi * 4);

        res *= (h / 6);

        return res;
    }

    double std_N(double x)                          //integrated part of standard normal pdf
    {
        return (exp(pow(x, 2) / -2));
    }

    double n(double x)								//standrad normal PDF
    {
        return std_N(x) * PI_base;
    }

    double simpson_cdf(double x)                    //numerical approximation of standard normal CDF using Simpson's rule
    {
        if (x == 0) return 0.5;

        double I_old = 0;

        double I_new = 0;

        if (x > 0) {

            I_old = (0.5 + rule_simpson(0, x, 4, &std_N) * PI_base);

            I_new = (0.5 + rule_simpson(0, x, 8, &std_N) * PI_base);

        }
        else {

            I_old = (0.5 - rule_simpson(x, 0, 4, &std_N) * PI_base);

            I_new = (0.5 - rule_simpson(x, 0, 8, &std_N) * PI_base);

        }

        for (int n = 16; n < 10000; n *= 2)
        {
            if (fabs(I_new - I_old) < tol) return I_new;

            else
            {
                I_old = I_new;

                I_new = (0.5 - rule_simpson(x, 0, n, &std_N) * PI_base);
            }
        }

    }

    double N(double x)								//interface for standrad normal CDF calculation
    {
        return simpson_cdf(x);
    }

}