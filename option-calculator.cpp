// CLI implementation of option price calculator
// This file contains the 'main' function. Program execution begins and ends there.

#include "EuropeanOption.hpp"
//#include "PerpetualAmericanOption.hpp"				//is not integrated in CLI yet
#include <iostream>
#include <cstdlib>										//for std::atof
#include <fstream>										//for std::ifstream, ofstream
#include <string>										//for std::getline
#include <sstream>										//for std::istringstream


int main(int argc, char* argv[]) {

	EuropeanOption opt;

	if (argc == 2) {

		std::string arg = argv[1];

		if (arg == "--help" || arg == "-h")
			std::cout << "\n*** A lightweight, portable and multi-functional plain vanilla option price calculator based "
			<< "on B-S formula *** \n\nUsage:\n\n"
			<< "option-calculator 1 2 3 4 5\n"
			<< "Where 1 - time to maturity in years (ex. 0.5)\n"
			<< "2 - Strike price (ex. 65)\n"
			<< "3 - Underlying volatility (ex. 0.3)\n"
			<< "4 - Risk-free interest rate (ex. 0.05)\n"
			<< "5 - Stock price (ex. 100)\n"
			<< "Outputs both Call and put prices to console.\n\n"
			<< "option-calculator inputs.txt outputs.txt\n"
			<< "Where each line in inputs.txt is 1 2 3 4 5 as described above - calculates respective call + put prices and saves "
			<< "in outputs.txt\n\n"
			<< "Enjoy :^)\n\n";
			 
	}
	else if (argc == 3) {

		std::string arg1 = argv[1];
		std::string arg2 = argv[2];

		if (arg1.substr(arg1.size() - 4) != ".txt" || arg2.substr(arg2.size() - 4) != ".txt") {
			std::cerr << "Incorrect command, please check --help." << std::endl;
			return 1;
		}

		std::ifstream inputFile(arg1);
		std::ofstream outputFile(arg2);

		if (!inputFile || !outputFile) {
			std::cerr << "Error opening files: " << arg1  << " " << arg2 << std::endl;
			return 1;
		}

		std::string line;

		int cnt = 1;

		while (std::getline(inputFile, line)) {

			std::istringstream iss(line);

			std::vector<std::string> params;

			std::string word;

			while (iss >> word) params.push_back(word);

			opt.T(stof(params[0]));
			opt.K(stof(params[1]));
			opt.vol(stof(params[2]));
			opt.r(stof(params[3]));
			opt.S(stof(params[4]));

			outputFile << "Option #" << std::to_string(cnt++) << ": Call = " << std::to_string(opt.Call())
				<< ", Put = " << std::to_string(opt.Put()) << '\n';

		}

		inputFile.close();
		outputFile.close();

	}
	else if (argc == 6) {

		opt.T( atof(argv[1]) );
		opt.K( atof(argv[2]) );
		opt.vol( atof(argv[3]) );
		opt.r( atof(argv[4]) );
		opt.S( atof(argv[5]) );

		std::cout << "European option prices: Call = " << opt.Call()
			<< ", Put = " << opt.Put() << std::endl;

	} else {
		std::cerr << "Incorrect command, please check --help." << std::endl;
		return 1;
	}

	return 0;

}