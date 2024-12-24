#pragma once

#include "graph.h"
#include "solution.h"
#include <cstdlib>
#include <cstdio>
#include <string>
#include <cstring>
#include <sstream>
#include <queue>

class Solver {

public:
    Solver(){}
    Solver(const string& filepath); // loadGraph from filepath

    void findShortestPath(int u); //dijkstra
    void generateEdges(Solution& s); //prim
    void resizeAtributes();
    Solution basicGreedy(); //greediest
    Solution randomGreedy(int r); // 1 out of r best choices in every iteration

    int totalWeightOfShortestPath(int u, int v);
    set<int> groupsOnShortestPath(int u,int v);
    vector<int> verticesOnShortestPath(int u,int v);

    int getGraphSize();
    vector<int>& getWeights();
    vector<Group>& getGroups();
    int tezina_grane(int u,int v); //to do better...



private:
    Graph* graph;

};


