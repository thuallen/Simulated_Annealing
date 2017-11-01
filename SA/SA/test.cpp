#include <iostream>  
#include <fstream>
#include <sstream>
#include "multi_min.h"
#include "TSP.h"

int main() {
	/*
	// part 1: find minimum
	double left = -1.0;
	double right = 1.0;
	// find minimum st x between left and right

	std::string filePath = "C:\\Users\\Allen\\Documents\\GitHub\\Simulated_Annealing\\SA\\data\\fun_min\\func_min";
	for (int i = 0; i < 20; ++i) {
		//std::cout << "Experiment NO." << i + 1 << std::endl;
		std::vector<double> res = SA(left, right);
		std::fstream fs;
		std::stringstream ss;
		std::string tmp;
		ss << i + 1;
		ss >> tmp;
		std::string realPath = filePath + tmp + ".txt";
		fs.open(realPath, std::ios::out);
		for (int i = 0; i < res.size(); ++i) {
			fs << res[i] << std::endl;
		}
		fs.close();
	}
	*/

	
	//part 2: TSP
	Graph G;
	CreateGraph(G);
	TSP_solution bestSoluion[20];
	std::fstream fs_TSP;
	std::string filePath = "C:\\Users\\Allen\\Documents\\GitHub\\Simulated_Annealing\\SA\\data\\tsp\\TSP_ans";
	for (int i = 0; i < 20; ++i) {
		std::vector<double> TSP_y;				// record the changing of length_path
		bestSoluion[i] = SA_TSP(G, TSP_y);
		std::cout << "第" << i + 1 << "次运行：" << std::endl;
		Display(G, bestSoluion[i]);
		std::stringstream ss;
		std::string tmp;
		ss << i + 1;
		ss >> tmp;
		std::string realPath = filePath + tmp + ".txt";
		std::cout << realPath << std::endl;
		fs_TSP.open(realPath, std::ios::out);
		for (unsigned int i = 0; i < TSP_y.size(); i += 1) {
		//std::cout << TSP_y[i] << std::endl;
			fs_TSP << TSP_y[i] << std::endl;
		}
		fs_TSP.close();
	}
	
	system("pause");
	return 0;
}