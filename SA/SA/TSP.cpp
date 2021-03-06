#include <iostream>  
#include <stdlib.h>   
#include <queue>  
#include <stack>  
#include <fstream>  
#include <iomanip>     
#include <ctime>  
#include <algorithm>  

#include "TSP.h"  

using namespace std;

double myrn01() {
	double left = 0.0, right = 1.0;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(1, 65536);

	return left + ((double)dist(gen) / 65535) * (right - left);
}

// transform the coordinate of cities to distance
void CityDataTranslate() {
	ifstream read_in;
	read_in.open("C:\\Users\\Allen\\Documents\\GitHub\\Simulated_Annealing\\SA\\data\\tsp\\city_info.txt");      
	if (!read_in.is_open()){
		cout << "file reading failed." << endl;
		return;
	}

	double city_table[MAX_CITYNUM][MAX_CITYNUM];
	char city_No[MAX_CITYNUM];
	double city_x[MAX_CITYNUM];
	double city_y[MAX_CITYNUM];

	int vex_num;
	read_in >> vex_num;

	fstream fout;
	fout.open("C:\\Users\\Allen\\Documents\\GitHub\\Simulated_Annealing\\SA\\data\\tsp\\dist_info.txt", ios::out);

	fout << vex_num << endl;

	for (int i = 0; i < vex_num; i++){
		read_in >> city_No[i] >> city_x[i] >> city_y[i];
		//cout << "city " << city_No[i] << " X = " << city_x[i] << " Y = " << city_y[i] << endl;
		fout << city_No[i] << " ";
	}
	fout << endl;

	for (int i = 0; i < vex_num; i++){
		city_table[i][i] = 0;
		for (int j = 0; j < vex_num; j++){
			double temp = (city_x[i] - city_x[j])*(city_x[i] - city_x[j]) + (city_y[i] - city_y[j])*(city_y[i] - city_y[j]);
			city_table[i][j] = sqrt(temp);
			fout << city_table[i][j];
			if (j < vex_num - 1) {
				fout<< " ";
			}
		}
		fout << endl;
	}
}

void CreateGraph(Graph &G) {
	CityDataTranslate();
	ifstream read_in;
	read_in.open("C:\\Users\\Allen\\Documents\\GitHub\\Simulated_Annealing\\SA\\data\\tsp\\dist_info.txt");
	if (!read_in.is_open()){
		cout << "file reading failed." << endl;
		return;
	}

	read_in >> G.vex_num;
	// read_in >> G.arc_num;  
	G.arc_num = 0;
	for (int i = 0; i < G.vex_num; i++){
		
		read_in >> G.vexs[i];
		//cout << " " << G.vexs[i] << " " << endl;
	}
	G.vexs[G.vex_num] = '\0';     

	for (int i = 0; i < G.vex_num; i++){
		for (int j = 0; j < G.vex_num; j++){
			read_in >> G.arcs[i][j];

			// calculate the arc_num  
			if (G.arcs[i][j] > 0){
				G.arc_num++;
			}
		}
	}

	// display  
	cout << "Graph info：" << endl;
	cout << "G.vex_num = " << G.vex_num << endl;
	cout << "G.arc_num = " << G.arc_num << endl;
	cout << "vexs[max_vexNum] = ";
	for (int i = 0; i < G.vex_num; i++){
		cout << G.vexs[i] << " ";
	}
	cout << endl << endl << "Adjacency matrix：" << endl;
	for (int i = 0; i < G.vex_num; i++){
		for (int j = 0; j < G.vex_num; j++){
			cout << right << setw(4) << G.arcs[i][j] << " ";
		}
		cout << endl;
	}
}

TSP_solution SA_TSP(Graph G, std::vector<double> &TSP_y) {
	srand(unsigned(time(0)));
 
	double Current_Temperature = INITIAL_TEMPERATURE;

	TSP_solution bestSolution = findASolution(G);
	
	TSP_solution Current_solution;
 
	while (MIN_TEMPERATURE < Current_Temperature) { 
		for (int i = 0; i < ITER_NUMBER; i++){
			Current_solution = generateNewSolution(G, bestSolution);
			//cout << Current_solution.lenOfPath<<" "<<bestSolution.lenOfPath << endl;
			if (Current_solution.lenOfPath <= bestSolution.lenOfPath) {  // accept it 
				if (Current_solution.lenOfPath == bestSolution.lenOfPath){
					// cout<<"不同路径出现相同的最优解."<<endl;  
				}
				bestSolution = Current_solution;
			}else {     
				double delta = bestSolution.lenOfPath - Current_solution.lenOfPath;
				double tmp1 = exp(delta / (Current_Temperature* K)),tmp2 = myrn01();
 				//cout << tmp1 << " " << tmp2 << endl;
 				if ( tmp1>tmp2 ){
					bestSolution = Current_solution;
				}
			}
			TSP_y.push_back(bestSolution.lenOfPath);
		}
		
		Current_Temperature *= SPEED;  
	} // while  
	return bestSolution;
}

void GetRandomArray(int arry[], int number){
	int i, k;
	srand((int)time(0));
	for (i = 0; i<number - 1; i++){
		k = rand() % (number - i) + i;
		swap(arry[i], arry[k]);
	}

}

void setVal(TSP_solution &s, char p[], double len) {
	s.lenOfPath = len;
	int i = 0;
	for (i = 0; p[i] != '\0'; ++i) {
		s.path[i] = p[i];
	}
	s.path[i] = '\0';
}

TSP_solution generateNewSolution(Graph G, TSP_solution bestSolution) {
	// 产生新的解  
	TSP_solution newSolution;

	// 起始城市固定为A, 终点也要返回A, 即需要关注起点A和终点A之间的所有城市  
	int i = rand() % (G.vex_num - 1) + 1;   // % 取余 -> 即将随机数控制在[1, G.vex_num - 1]  
	int j = rand() % (G.vex_num - 1) + 1;

	if (i > j){
		int temp = i;
		i = j;
		j = temp;
	}else if (i == j){   // 表示产生的随机数没有改变的作用, 将此路程设置为最大并结束该函数  
		newSolution = bestSolution;
		return newSolution;
	}

	/* way 2 */
	int choose = rand() % 3;
	if (choose == 0){   // 随机交换任意两个城市的位置  
		char temp = bestSolution.path[i];
		bestSolution.path[i] = bestSolution.path[j];
		bestSolution.path[j] = temp;
	}else if (choose == 1){   // 随机逆置城市的位置  
		reverse(bestSolution.path + i, bestSolution.path + j);
	}else {   // 随机移位城市的位置  
		if (j + 1 == G.vex_num) { //边界处不处理    
			newSolution = bestSolution;
			return newSolution;
		}
		rotate(bestSolution.path + i, bestSolution.path + j, bestSolution.path + j + 1);
	}
	newSolution = bestSolution;
	newSolution.path[G.vex_num] = newSolution.path[0];   // 终点与起始点相同  
	newSolution.path[G.vex_num + 1] = '\0';
	newSolution.lenOfPath = CalculateLength(G, newSolution);

	return newSolution;
}

double CalculateLength(Graph G, TSP_solution newSolution) {
	double _length = 0.0;

	for (int i = 0; i < G.vex_num - 1; i++){
		int _startCity = (int)newSolution.path[i] - 65;
		int _endCity = (int)newSolution.path[i + 1] - 65;
		if (G.arcs[_startCity][_endCity] == -1){
			return MAX_INT;
		}else {
			_length += G.arcs[_startCity][_endCity];
		}
	}

	// 判断该路径是否能回到起始城市  
	if (G.arcs[(int)newSolution.path[G.vex_num - 1] - 65][(int)newSolution.path[0] - 65] == -1){
		return MAX_INT;
	}else {
		_length += G.arcs[(int)newSolution.path[G.vex_num - 1] - 65][(int)newSolution.path[0] - 65];

		return _length;
	}

}

TSP_solution findASolution(Graph G){
	int n = G.vex_num;
	int tmp[MAX_VEXNUM];
	char mpath[MAX_VEXNUM];

	mpath[0] = 'A';
	mpath[n] = 'A';
	for (int i = 0; i < n-1; ++i) {		
		tmp[i] = i + 1;
	}
	
	int num = n - 1;
	GetRandomArray(tmp, num);
	for (int i = 1; i <= n - 1; ++i) {
		mpath[i] = tmp[i - 1]+'A';
	}

	mpath[n + 1] = '\0';
	TSP_solution slt;
	double len = 0.0;
	
	for (int i = 0; i < n; ++i) {
		len += G.arcs[mpath[i] - 'A'][mpath[i + 1] - 'A'];
	}

	setVal(slt, mpath, len);

	return slt;
}

void Display(Graph G, TSP_solution bestSoluion) {
	cout << endl << "****************************** TSP_SA - BestSolution ******************************" << endl;

	cout << endl << "bestSoluion.path= ";
	for (int i = 0; i < G.vex_num; i++){
		cout << bestSoluion.path[i] << "-->";
	}
	cout << bestSoluion.path[G.vex_num] << endl;

	cout << endl << "bestSoluion.lenOfPath = " << bestSoluion.lenOfPath << endl;;

	cout << endl << "***********************************************************************************" << endl << endl;
}