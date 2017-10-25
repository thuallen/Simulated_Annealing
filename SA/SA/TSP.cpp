#include <iostream>  
#include <stdlib.h>   
#include <queue>  
#include <stack>  
#include <fstream>  
#include <iomanip>    // 本文用于输出对齐  
#include <ctime>  
#include <algorithm>  

#include "TSP.h"  

using namespace std;

// 城市数据格式转化  
void CityDataTranslate() {
	ifstream read_in;
	read_in.open("L:\\Coding\\TSP_SA模拟退火算法\\TSP_SA模拟退火算法\\ch150.txt");      // 待转换数据  
	if (!read_in.is_open())
	{
		cout << "文件读取失败." << endl;
		return;
	}

	ofstream fout("L:\\Coding\\TSP_SA模拟退火算法\\TSP_SA模拟退火算法\\city_150.txt");      // 转换后的数据存入文档 city_150.txt  

	double city_table[MAX_CITYNUM][MAX_CITYNUM];
	int city_No[MAX_CITYNUM];
	double city_x[MAX_CITYNUM];
	double city_y[MAX_CITYNUM];

	int vex_num;
	read_in >> vex_num;
	fout << vex_num << endl;

	for (int i = 0; i < vex_num; i++)
	{
		read_in >> city_No[i] >> city_x[i] >> city_y[i];

		fout << i + 1 << " ";
	}
	fout << endl;

	for (int i = 0; i < vex_num; i++)
	{
		city_table[i][i] = 0;
		for (int j = 0; j < vex_num; j++)
		{
			double temp = (city_x[i] - city_x[j])*(city_x[i] - city_x[j]) + (city_y[i] - city_y[j])*(city_y[i] - city_y[j]);
			city_table[i][j] = sqrt(temp);
			fout << city_table[i][j] << " ";
		}
		fout << endl;
	}
}

void CreateGraph(Graph &G) {
	ifstream read_in;
	read_in.open("L:\\Coding\\TSP_SA模拟退火算法\\TSP_SA模拟退火算法\\city_15.txt");
	if (!read_in.is_open())
	{
		cout << "文件读取失败." << endl;
		return;
	}

	read_in >> G.vex_num;
	// read_in >> G.arc_num;  
	G.arc_num = 0;
	for (int i = 0; i < G.vex_num; i++)
	{
		read_in >> G.vexs[i];
	}
	G.vexs[G.vex_num] = '\0';   // char的结束符.  

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
	cout << "无向图创建完毕，相关信息如下：" << endl;
	cout << "【顶点数】 G.vex_num = " << G.vex_num << endl;
	cout << "【边数】 G.arc_num = " << G.arc_num << endl;
	cout << "【顶点向量】 vexs[max_vexNum] = ";
	for (int i = 0; i < G.vex_num; i++)
	{
		cout << G.vexs[i] << " ";
	}
	cout << endl << "【邻接矩阵】 arcs[max_vexNum][max_vexNum] 如下：" << endl;
	for (int i = 0; i < G.vex_num; i++)
	{
		for (int j = 0; j < G.vex_num; j++)
		{
			cout << std::right << setw(4) << G.arcs[i][j] << " ";
		}
		cout << endl;
	}
}

TSP_solution SA_TSP(Graph G) {
	srand(unsigned(time(0)));

	// 当前温度  
	double Current_Temperature = INITIAL_TEMPERATURE;

	// 最优解  
	TSP_solution Best_solution;
	Best_solution.length_path = MAX_INT;

	// 初始路径  
	for (int i = 0; i < G.vex_num; i++)
	{
		Best_solution.path[i] = 'A' + i;
	}
	random_shuffle(Best_solution.path + 1, Best_solution.path + G.vex_num);

	// 当前解, 与最优解比较  
	TSP_solution Current_solution;

	// 模拟退火过程  
	while (MIN_TEMPERATURE < Current_Temperature) {
		// 满足迭代次数  
		for (int i = 0; i < LEGNTH_Mapkob; i++)
		{
			Current_solution = FindNewSolution(G, Best_solution);
			if (Current_solution.length_path <= Best_solution.length_path)   // 接受新解  
			{
				if (Current_solution.length_path == Best_solution.length_path)
				{
					// cout<<"不同路径出现相同的最优解."<<endl;  
				}
				Best_solution = Current_solution;
			}
			else {   // 按 Metropolis 判断是否接受  
				if ((int)exp((Best_solution.length_path - Current_solution.length_path) / Current_Temperature) * 100 > (rand() * 101))
				{
					Best_solution = Current_solution;
				}
				else {
					// cout<<"不接受当前解."<<endl;  
				}
			}
		}
		Current_Temperature *= SPEED;  // 按 SPEED 速率退火  

	} // while  

	return Best_solution;
}

TSP_solution FindNewSolution(Graph G, TSP_solution bestSolution) {
	// 产生新的解  
	TSP_solution newSolution;

	// 起始城市固定为A, 终点也要返回A, 即需要关注起点A和终点A之间的所有城市  
	int i = rand() % (G.vex_num - 1) + 1;   // % 取余 -> 即将随机数控制在[1, G.vex_num - 1]  
	int j = rand() % (G.vex_num - 1) + 1;

	if (i > j)
	{
		int temp = i;
		i = j;
		j = temp;
	}
	else if (i == j)
	{   // 表示产生的随机数没有改变的作用, 将此路程设置为最大并结束该函数  

		newSolution = bestSolution;
		return newSolution;
	}

	/* way 2 */
	int choose = rand() % 3;
	if (choose == 0)
	{   // 随机交换任意两个城市的位置  
		char temp = bestSolution.path[i];
		bestSolution.path[i] = bestSolution.path[j];
		bestSolution.path[j] = temp;
	}
	else if (choose == 1)
	{   // 随机逆置城市的位置  
		reverse(bestSolution.path + i, bestSolution.path + j);
	}
	else {   // 随机移位城市的位置  
		if (j + 1 == G.vex_num) //边界处不处理    
		{
			newSolution = bestSolution;
			return newSolution;
		}
		rotate(bestSolution.path + i, bestSolution.path + j, bestSolution.path + j + 1);
	}
	newSolution = bestSolution;
	newSolution.path[G.vex_num] = newSolution.path[0];   // 终点与起始点相同  
	newSolution.path[G.vex_num + 1] = '\0';
	newSolution.length_path = CalculateLength(G, newSolution);

	return newSolution;
}

int CalculateLength(Graph G, TSP_solution newSolution) {
	int _length = 0;

	for (int i = 0; i < G.vex_num - 1; i++)
	{
		int _startCity = (int)newSolution.path[i] - 65;
		int _endCity = (int)newSolution.path[i + 1] - 65;
		if (G.arcs[_startCity][_endCity] == -1)
		{
			return MAX_INT;
		}
		else {
			_length += G.arcs[_startCity][_endCity];
		}
	}

	// 判断该路径是否能回到起始城市  
	if (G.arcs[(int)newSolution.path[G.vex_num - 1] - 65][(int)newSolution.path[0] - 65] == -1)
	{
		return MAX_INT;
	}
	else {
		_length += G.arcs[(int)newSolution.path[G.vex_num - 1] - 65][(int)newSolution.path[0] - 65];

		return _length;
	}

}

void Display(Graph G, TSP_solution bestSoluion) {
	cout << "****************************** TSP_SA - BestSolution ******************************" << endl;

	cout << "最优路径,bestSoluion.path[ ] = ";
	for (int i = 0; i < G.vex_num; i++)
	{
		cout << bestSoluion.path[i] << "-->";
	}
	cout << bestSoluion.path[G.vex_num] << endl;

	cout << "最优路径,bestSoluion.length_path = " << bestSoluion.length_path << endl;;


	cout << "***********************************************************************************" << endl;
}