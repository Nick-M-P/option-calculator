//Option abstract base class stores option type and provides basic interface:
//Price() method calculates corresponding type and Put-Call methods have to be impemented
//according to derived option type

#include "Option.hpp"

#ifndef Option_CPP
#define Option_CPP

std::vector<double>
mesh_array(double start, double step, double length)					//make a vector where every next element is incremented by step
{
	std::vector<double> res;

	for (int i = 0; i < length; i++)
		res.push_back(start + i * step);

	return res;
}

template<typename T>
std::vector<double>
matrix_calc(const std::vector<std::vector<double>>& source,				//template function that allows to utilize class methods while working with matrices
			char type, double (T::* method)() const)
{
	T temp;																//make a temporary Option object to utilize
																		//its option pricing functionality

	if (temp.type() != type)											//check if the type is correct
		temp.toggle();

	std::vector<double> res;											//make a vector to store obtained results

	for (int i = 0; i < source[0].size(); i++)							//iterating through the matrix
	{
		std::vector<double> buff;										//create a buffer for each row of parameters

		for (int ii = 0; ii < source.size(); ii++)						//fill it
		{
			buff.push_back(source[ii][i]);
		}

		temp.SetValues(buff);											//set our Option values

		res.push_back((temp.*method)());								//and apply the method (calculate price, greeks etc)
	}

	return res;															//return the vector
}

#endif