#pragma once

#include<random>
#include<cmath>

#define ITER_NUM 250
#define LAMBDA 0.9				//control the speed of annealing
#define INIT_T 20.0
#define END_T 0.1
#define PI 3.141593
double k = 0.00002;


double myRandom(double left, double right) {	// generate a random num between left and right
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(1, 65535); // distribute results between 1 and 65535 inclusive.  

	return left + ((double)dist(gen) / 65535) * (right - left);
}

double func(double x) {
	return std::exp(-x)*sin(10 * PI*x)*sin(2 * PI*x);
}

std::vector<double> SA(double left, double right) {
	std::vector<double> x;
	std::vector<double> y;

	double best_x = myRandom(left, right);
	double best_y = func(best_x);

	double t = INIT_T;
	int cnt = 0;
	while (t > END_T) {
		//outer loop
		for (int i = 0; i < ITER_NUM; ++i) {
			// inner loop
			double cur_x = myRandom(left, right);
			double cur_y = func(cur_x);

			double delta = cur_y - best_y;

			if (delta < 0) {
				// if value of func get smaller, accept it
				best_x = cur_x;
				best_y = cur_y;
			}
			else {
				// else accept it according to a probability
				double tmp = myRandom(0.0, 1.0);
				if (exp(-delta / (t*k)) > tmp) {
					best_x = cur_x;
					best_y = cur_y;
				}
			}

			x.push_back(best_x);
			y.push_back(best_y);

		}

		t *= LAMBDA;
		++cnt;
	}
	std::cout << "x = " << best_x << ",  y = " << best_y << std::endl;
	return y;
}