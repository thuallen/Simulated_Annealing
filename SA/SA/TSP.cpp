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

// transform the coordinate of cities to distance
void CityDataTranslate() {
	ifstream read_in;
	read_in.open("C:\\Users\\Allen\\Documents\\GitHub\\Simulated_Annealing\\SA\\city_info.txt");      
	if (!read_in.is_open())
	{
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
	fout.open("C:\\Users\\Allen\\Documents\\GitHub\\Simulated_Annealing\\SA\\dist_info.txt", ios::out);

	fout << vex_num << endl;

	for (int i = 0; i < vex_num; i++)
	{
		read_in >> city_No[i] >> city_x[i] >> city_y[i];
		//cout << "city " << city_No[i] << " X = " << city_x[i] << " Y = " << city_y[i] << endl;
		fout << city_No[i] << " ";
	}
	fout << endl;

	for (int i = 0; i < vex_num; i++)
	{
		city_table[i][i] = 0;
		for (int j = 0; j < vex_num; j++)
		{
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
	read_in.open("C:\\Users\\Allen\\Documents\\GitHub\\Simulated_Annealing\\SA\\dist_info.txt");
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

	for (int i = 0; i < G.vex_num; i++)
	{
		for (int j = 0; j < G.vex_num; j++)
		{
			read_in >> G.arcs[i][j];

			// calculate the arc_num  
			if (G.arcs[i][j] > 0)
			{
				G.arc_num++;
			}
		}
	}

	// display  
	cout << "Graph info��" << endl;
	cout << "G.vex_num = " << G.vex_num << endl;
	cout << "G.arc_num = " << G.arc_num << endl;
	cout << "vexs[max_vexNum] = ";
	for (int i = 0; i < G.vex_num; i++)
	{
		cout << G.vexs[i] << " ";
	}
	cout << endl << endl << "Adjacency matrix��" << endl;
	for (int i = 0; i < G.vex_num; i++)
	{
		for (int j = 0; j < G.vex_num; j++)
		{
			cout << right << setw(4) << G.arcs[i][j] << " ";
		}
		cout << endl;
	}
}

TSP_solution SA_TSP(Graph G) {
	srand(unsigned(time(0)));
 
	double Current_Temperature = INITIAL_TEMPERATURE;

	TSP_solution Best_solution;
	Best_solution.length_path = MAX_INT;

	// origin path 
	for (int i = 0; i < G.vex_num; i++)
	{
		Best_solution.path[i] = 'A' + i;
	}

	random_shuffle(Best_solution.path + 1, Best_solution.path + G.vex_num);
 
	TSP_solution Current_solution;
 
	while (MIN_TEMPERATURE < Current_Temperature) { 
		for (int i = 0; i < LEGNTH_Mapkob; i++)
		{
			Current_solution = FindNewSolution(G, Best_solution);
			if (Current_solution.length_path <= Best_solution.length_path)   // accept it 
			{
				if (Current_solution.length_path == Best_solution.length_path)
				{
					// cout<<"��ͬ·��������ͬ�����Ž�."<<endl;  
				}
				Best_solution = Current_solution;
			}
			else {     
				if ((int)exp((Best_solution.length_path - Current_solution.length_path) / Current_Temperature) * 100 > (rand() * 101))
				{
					Best_solution = Current_solution;
				}
			}
		}
		Current_Temperature *= SPEED;  

	} // while  

	return Best_solution;
}

TSP_solution FindNewSolution(Graph G, TSP_solution bestSolution) {
	// generate a new solution  
	TSP_solution newSolution;

	int i = rand() % (G.vex_num - 1) + 1;   // % ȡ�� -> ���������������[1, G.vex_num - 1]  
	int j = rand() % (G.vex_num - 1) + 1;

	if (i > j)
	{
		int temp = i;
		i = j;
		j = temp;
	}
	else if (i == j)
	{   // ��ʾ�����������û�иı������, ����·������Ϊ��󲢽����ú���  

		newSolution = bestSolution;
		return newSolution;
	}

	/* way 2 */
	int choose = rand() % 3;
	if (choose == 0)
	{   // ������������������е�λ��  
		char temp = bestSolution.path[i];
		bestSolution.path[i] = bestSolution.path[j];
		bestSolution.path[j] = temp;
	}
	else if (choose == 1)
	{   // ������ó��е�λ��  
		reverse(bestSolution.path + i, bestSolution.path + j);
	}
	else {   // �����λ���е�λ��  
		if (j + 1 == G.vex_num) //�߽紦������    
		{
			newSolution = bestSolution;
			return newSolution;
		}
		rotate(bestSolution.path + i, bestSolution.path + j, bestSolution.path + j + 1);
	}
	newSolution = bestSolution;
	newSolution.path[G.vex_num] = newSolution.path[0];   // �յ�����ʼ����ͬ  
	newSolution.path[G.vex_num + 1] = '\0';
	newSolution.length_path = CalculateLength(G, newSolution);

	return newSolution;
}

int CalculateLength(Graph G, TSP_solution newSolution) {
	int _length = 0;

	for (int i = 0; i < G.vex_num - 1; i++)
	{
		int _startCity = (int)newSolution.path[i] - 'A';
		int _endCity = (int)newSolution.path[i + 1] - 'A';
		if (G.arcs[_startCity][_endCity] == -1)
		{
			return MAX_INT;
		}
		else {
			_length += G.arcs[_startCity][_endCity];
		}
	}

	// �жϸ�·���Ƿ��ܻص���ʼ����  
	if (G.arcs[(int)newSolution.path[G.vex_num - 1] - 'A'][(int)newSolution.path[0] - 'A'] == -1)
	{
		return MAX_INT;
	}
	else {
		_length += G.arcs[(int)newSolution.path[G.vex_num - 1] - 'A'][(int)newSolution.path[0] - 'A'];

		return _length;
	}

}

void Display(Graph G, TSP_solution bestSoluion) {
	cout << endl << "****************************** TSP_SA - BestSolution ******************************" << endl;

	cout << endl << "bestSoluion.path= ";
	for (int i = 0; i < G.vex_num; i++)
	{
		cout << bestSoluion.path[i] << "-->";
	}
	cout << bestSoluion.path[G.vex_num] << endl;

	cout << endl << "bestSoluion.length_path = " << bestSoluion.length_path << endl;;

	cout << endl << "***********************************************************************************" << endl << endl;
}