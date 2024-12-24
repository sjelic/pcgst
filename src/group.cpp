#include "group.h"

void Group::addNode(int u)
{
    nodes.insert(u);
}

void Group::removeNode(int u)
{
    nodes.erase(u);
}

bool Group::containsNode(int u)
{
    return nodes.find(u)!=nodes.end();
}

void Group::setPenalty(int p)
{
    penalty=p;
}

int Group::getPenalty()
{
    return penalty;
}

set<int>& Group::getNodes()
{
    return nodes;
}