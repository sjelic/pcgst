#include "solution.hpp"


void Solution::setFc(int x)
{
    fc=x;
}

int Solution::getFc()
{
    return fc;
}

vector<int> Solution::getVertices()
{
    return vertices;
}


void Solution::printVertices()
{
    for(int i=0;i<vertices.size();i++)
    {
        cout<<vertices[i]<<endl;
    }
}


bool Solution::contains(int u)
{
    for(int v : vertices)
    {
        if(v==u)
            return true;
    }
    return false;
}

