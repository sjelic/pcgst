#include "solution.h"

void Solution::setCost(int x)
{
    cost=x;
}

int Solution::getCost()
{
    return cost;
}

vector<int>& Solution::getNodes()
{
    return nodes;
}

void Solution::printNodes()
{
    for(int i=0;i<nodes.size();i++)
    {
        cout<<nodes[i]<<endl;
    }
}

bool Solution::containsNode(int u)
{
    for(int v : nodes)
    {
        if(v==u)
            return true;
    }
    return false;
}

void Solution::toTxt(const std::string& name, const std::string& dirPath)
{
    string filepath = dirPath + "/" + name;
    
    auto last_dot = filepath.find_last_of('.');
    if (last_dot != string::npos)
    {
        filepath = filepath.substr(0, last_dot);
    }
    filepath += ".txt";

    ofstream file(filepath);
    if (!file)
    {
        cerr << "Error: " << filepath << " couldn't be opened.\n";
        return;
    }

    cout << filepath << " in progress...\n";

    file << "SECTION Graph\n";
    file << "Nodes " << nonCoveredGroupCount.size() << "\n";
    file << "Edges " << edges.size() << "\n";

    for (const auto& p : edges)
    {
        file << "E " << p.first << " " << p.second << " " << "w" <<"\n";//solver->tezina_grane(p.first, p.second) << "\n";
    }

    file << "END\n";
    cout << "Done.\n";
}

vector<edge>& Solution::getConnection()
{
    return connection;
}

set<pair<int,int>>& Solution::getEdges()
{
    return edges;
}

vector<int>& Solution::getNCGC()
{
    return nonCoveredGroupCount;
}

void Solution::addNode(int node)
{
    nodes.push_back(node);
}

void Solution::addGroup(int g)
{
    groups.insert(g);
}

bool Solution::containsGroup(int g)
{
    return groups.find(g)!=groups.end();
}