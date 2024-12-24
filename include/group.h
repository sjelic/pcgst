#pragma once

#include <set>

using namespace std;

struct edge{
    int neighbour;
    int w;
    edge() : neighbour(0), w(0) {}
    edge(int n, int w) : neighbour(n), w(w) {}
};

class Group {
public:
    void addNode(int node);
    void removeNode(int node);
    bool containsNode(int node);
    void setPenalty(int p);
    int getPenalty();
    set<int>& getNodes();

private:
    int id;   
    int penalty;            
    set<int> nodes;         
};
