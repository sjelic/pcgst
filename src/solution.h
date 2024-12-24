#pragma once

#include "graph.h"
#include "solver.h"

class Solution {

public:

    Solution(){}

    void setCost(int cost);
    int getCost();

    vector<int>& getNodes();
    vector<edge>& getConnection();
    set<pair<int,int>>& getEdges();
    vector<int>& getNCGC();

    void addNode(int node);
    void addGroup(int g);
    bool containsGroup(int g);

    void printNodes();
    bool containsNode(int u);

    void toTxt(Solver* solver,const string& name,const string& dir_name); //generates .txt

private:
    vector<int> nodes;
    set<int> groups;
    int cost;
    vector<edge> connection;
    set<pair<int,int> > edges;
    vector<int> nonCoveredGroupCount; //ncgc[u] = broj grupa kojem u pripada a nisu u resenju
};
