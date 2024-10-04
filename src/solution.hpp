#ifndef SOLUTION_HPP
#define SOLUTION_HPP

#include<iostream>
#include<cstdlib>
#include<string>
#include<cstring>
#include<vector>
#include<set>

using namespace std;

typedef struct Edge{
    int neighbour;
    int w;
}edge;

typedef struct Group{
    int penalty;
    set<int> vertices;
}group;

class Solution {

public:

    Solution(){}

    void setFc(int fc);
    int getFc();

    vector<int> getVertices();

    void printVertices();

    bool contains(int u);//da li vertices ima taj cvor

//private:
    vector<int> vertices;
    set<int> groups;
    int fc;
    vector<edge> connection;
    set<pair<int,int> > edges;
    vector<int> numberOfUncoveredGroups; //u koliko grupa svaki cvor pripada a koje nisu u resenju
};

#endif