#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <limits>
#include "group.h"
#include "solution.h"

class Graph {

public:
    void loadGraph(const string& filepath); //path: dirpath/filename

    void addEdge(int u, int v, int w);
    void removeEdge(int u, int v);
    void findShortestPath(int u); //dijkstra

    int tezina_grane(int u,int v); //to do better
    int getSize();
    vector<vector<edge>>& getGraph();
    vector<int>& getWeights();
    vector<Group>& getGroups();

    int totalWeightOfShortestPath(int u,int i); //node weights and edge weights
    vector<int> verticesOnShortestPath(int u,int i);
    set<int> groupsOnShortestPath(int i,int j);

private:
    vector<vector<edge>> graph; //adjacency list
    vector<int> weights; //node weights
    vector<Group> groups;
    vector<vector<int>> parents; //parents[i][j]=node to get from j to i by shortestPath
    vector<vector<int>> shortestPath; //edge weights on shortestPath from j to i
};
