#pragma once
#define MAX_VEXNUM 30  
#define MAX_CITYNUM 150  
#include <vector>
#include <random>

const int ITER_NUMBER = 100;
const double SPEED = 0.98;                   
const double INITIAL_TEMPERATURE = 5000.0;   
const double MIN_TEMPERATURE = 10.0;        
const int MAX_INT = 999999;
const double K = 0.0005;

typedef struct {
	int vex_num, arc_num;                    
	char vexs[MAX_VEXNUM];                    
	double arcs[MAX_VEXNUM][MAX_VEXNUM];     // Adjacency matrix
}Graph;

typedef struct {
	double lenOfPath;
	char path[MAX_VEXNUM];
}TSP_solution;

void CreateGraph(Graph &G);
TSP_solution SA_TSP(Graph G, std::vector<double> &TSP_y);
TSP_solution findASolution(Graph G);
TSP_solution generateNewSolution(Graph G, TSP_solution bestSolution);
double CalculateLength(Graph G, TSP_solution newSolution);
void Display(Graph G, TSP_solution bestSoluion);