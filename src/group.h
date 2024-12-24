#pragma once

#include <set>

using namespace std;

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
