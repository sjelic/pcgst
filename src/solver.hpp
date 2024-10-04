#ifndef SOLVER_HPP
#define SOLVER_HPP

#include<iostream>
#include<cstdlib>
#include <cstdio>
#include<string>
#include<cstring>
#include<vector>
#include<set>
#include <sstream>
#include <queue>
#include <limits>
#include"solution.hpp"

using namespace std;

class Solver {

public:
    Solver(){}

    void loadGraph(char* filename,string dirpath); //ucitava graph iz fajla dirpath/filename
    void resizeAtributes();
    void format_txt(Solution& sol,string name,string dir_name); //generise name.txt fajl 

    void dijkstra(int u); //popunjava parents i sh_path samo za cvor u
    void generateEdges(Solution& s); //prim
    Solution basic_greedy(); //generates vertices, najpohlepnija verzija

    int totalWeightOfShortestPath(int u, int v);
    set<int> groupsOnShortestPath(int u,int v);
    vector<int> verticesOnShortestPath(int u,int v);

private:
    vector<vector<edge> > graph;
    vector<int> weights;
    vector<group> groups;
    vector<vector<int> > parents;
    vector<vector<int> > shortest_path;
};

int indeks_u_s(edge e,Solution& s);
int tezina_grane(int u,int v,vector<vector<edge>>& graph);

#endif