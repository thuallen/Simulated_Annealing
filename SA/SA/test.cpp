#include <iostream>  
#include <fstream>
#include <sstream>
#include "multi_min.h"

int main() {
	// part 1: find minimum
	double left = -1.0;
	double right = 1.0;
	// find minimum st x between left and right
	std::vector<double> res = SA(left, right);

	std::fstream fs;
	fs.open("C:\\Users\\Allen\\Documents\\GitHub\\Simulated_Annealing\\SA\\func_min.txt", std::ios::out);
	for (int i = 0; i < res.size(); ++i) {
		fs << res[i] << std::endl;
	}


	//part 2: TSP 


	system("pause");
	return 0;
}