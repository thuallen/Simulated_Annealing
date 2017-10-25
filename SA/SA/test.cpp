#include <iostream>  
#include <fstream>
#include <sstream>
#include <ctime> 
#include "multi_min.h"
#include "TSP.h"

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
	
	

	/*
	//part 2: TSP
	// CityDataTranslate();   // 创建城市表

	time_t T_begin = clock();
	Graph G;
	CreateGraph(G);

	// 1. initial w and target function f(w)
	// 2. random production new solution - eg: ABCDEA --> ABCEDA
	// 3. judge whether accepted new solution or not
	// 4. Simulate Annealing

	TSP_solution bestSoluion = SA_TSP(G);
	Display(G, bestSoluion);

	time_t T_end = clock();
	double RunningTime = double(T_end - T_begin) / CLOCKS_PER_SEC;

	std::cout << "程序运行时间 RunningTime = " << RunningTime << std::endl;
	*/
	

	system("pause");
	return 0;
}