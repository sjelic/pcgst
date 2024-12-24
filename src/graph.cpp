#include "graph.h"

void Graph::addEdge(int u, int v, int w)
{
    graph[u].push_back({v,w});
    graph[v].push_back({u,w});
}

void Graph::removeEdge(int u, int v)
{
    //to do
}

void Graph::findShortestPath(int u)
{
    int n=graph.size();
    shortestPath[u].resize(n);
    parents[u].resize(n);

    for(int i=0;i<n;i++)
        shortestPath[u][i]=numeric_limits<int>::max();
    shortestPath[u][u]=weights[u];
    set<pair<int,int> > skup; // rastojanje,cvor
    skup.insert({shortestPath[u][u],u});

    parents[u][u]=-1;

    while(!skup.empty())
    {
        int v=skup.begin()->second;
        skup.erase(skup.begin());
        for(edge g : graph[v])
        {
            if(g.w+weights[g.neighbour]+shortestPath[u][v]<shortestPath[u][g.neighbour])
            {
                skup.erase({shortestPath[u][g.neighbour],g.neighbour});
                shortestPath[u][g.neighbour]=g.w+weights[g.neighbour]+shortestPath[u][v];
                skup.insert({shortestPath[u][g.neighbour],g.neighbour});
                parents[u][g.neighbour]=v;
            }
        }
    }
}

void Graph::loadGraph(const string& filepath)
{
    ifstream file(filepath);
    if (!file.is_open()) {
        throw runtime_error("Failed to open file: " + filepath);
    }

    string line;
    int n,e;
    
    while(getline(file, line) && line.find("SECTION Graph")==string::npos);

    file >> line >> n; 
    file >> line >> e; 

    graph.resize(n);
    weights.resize(n,0);
    shortestPath.resize(n);
    parents.resize(n);
    
    for(int i=0;i<n;i++)
    {
        file >> line >> weights[i];
        if (line != "N")
        {
        throw runtime_error("loadGraph: expected 'N' but got '"+line+"'");
        }
    }

    int u,v,w;
    for (int i = 0; i < e; ++i) {
        file >> line >> u >> v >> w;
        graph[u].push_back(edge(v, w));
        graph[v].push_back(edge(u, w));
        if (line != "E")
        {
        throw runtime_error("loadGraph: expected 'E' but got '"+line+"'");
        }
    }

    while(getline(file, line) && line.find("SECTION Terminals")==string::npos);

    int gr_num;
    file >> line >> gr_num; 
    
    groups.resize(gr_num);

    int pen;
    for(int i=0;i<gr_num;i++)
    {
        file >> line >> pen;
        if (line != "P")
        {
            throw runtime_error("loadGraph: expected 'P' but got '"+line+"'");
        }
        groups[i].setPenalty(pen);
    }

    for(int i=0;i<gr_num;i++)
    {
        file >> line; 
        if (line != "T")
        {
            throw runtime_error("loadGraph: expected 'T' but got '"+line+"'");
        }
        int v;
        while (file >> v)
        {
            groups[i].addNode(v);
            if(file.peek() == '\n' || file.eof()) {
                break;
            }
        }
    }

    file.close();
}

int Graph::tezina_grane(int u,int v)
{
    for(edge g : graph[u])
    {
        if(g.neighbour==v)
            return g.w;
    }
    return -1;//ne desava se
}

int Graph::getSize()
{
    return graph.size();
}

vector<vector<edge>>& Graph::getGraph()
{
    return graph;
}

vector<int>& Graph::getWeights()
{
    return weights;
}

vector<Group>& Graph::getGroups()
{
    return groups;
}

int Graph::totalWeightOfShortestPath(int u,int i)
{
    int totalWeight=weights[i];
    int pom=i;

    while(parents[u][pom]!=-1)
    {
        totalWeight+=weights[parents[u][pom]]+tezina_grane(pom,parents[u][pom]);
        pom=parents[u][pom];
    }

    return totalWeight;
}

vector<int> Graph::verticesOnShortestPath(int u,int i)
{
    vector<int> vertices;
    vertices.push_back(i);
    int pom=i;

    while(parents[u][pom]!=-1)
    {
        vertices.push_back(parents[u][pom]);
        pom=parents[u][pom];
    }

    return vertices;
}

set<int> Graph::groupsOnShortestPath(int i,int j)
{
    set<int> coveredGroups;
    for(int u : verticesOnShortestPath(i,j))
                for(int g=0;g<groups.size();g++)
                {
                    if(groups[g].containsNode(u))
                    {
                        coveredGroups.insert(g);
                    }
                }
    return coveredGroups;
}
