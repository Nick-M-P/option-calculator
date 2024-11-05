//Option abstract base class stores option type and provides basic interface:
//Price() method calculates corresponding type and Put-Call methods have to be impemented
//according to derived option type

#include <vector>

#ifndef Option_HPP
#define Option_HPP

class Option
{
private:
	char type_val = 'C';										//option type data-member ('C' or 'P')
public:
	char type() const { return type_val; };						//option type getter

	void toggle()												//method to switch type to the opposite
	{
		if (type_val == 'C')
			type_val = 'P';
		else
			type_val = 'C';
	}

	virtual double Call() const = 0;							//pure virtual functions for call and put prices
	virtual double Put() const = 0;

	double Price() const										//calculate option price based on its type
	{
		double res;
		if (type_val == 'C')
			res = Call();
		else
			res = Put();
		return res;
	}

	virtual void SetValues(const std::vector<double>& params) = 0;	//pvf to work with matrices of option parameters
};

//Global functions

inline std::vector<double>
mesh_array(double start, double step, double length);			//make a vector where every next element is incremented by step

template<typename T>
std::vector<double>
matrix_calc(const std::vector<std::vector<double>>& source,		//template function that allows to utilize class methods while working with matrices
			char type, double (T::* method)() const);

#ifndef Option_CPP												//for linking
#include "Option.cpp"
#endif

#endif