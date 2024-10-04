#include "solver.hpp"



void Solver::loadGraph(char* filename,string dirpath)
{
    char* path =(char*) malloc (sizeof(filename)+128);
    
    if(path==NULL)
    {
        fprintf(stderr,"malloc.\n");
        exit(EXIT_FAILURE);
    }
    strcat(path,dirpath.c_str());
    strcat(path,"/");
    strcat(path,filename);
    
    FILE* f = fopen(path,"r");
    if(f==NULL)
    {
        fprintf(stderr,"Neuspesno otvaranje fajla.\n");
        exit(EXIT_FAILURE);
    }

    char line[512];
    int n,e;
    fgets(line,512,f);
    while(strncmp(line,"SECTION Graph\n",13)!=0)
    {
            fgets(line,512,f);
    }

    fscanf(f,"Nodes %d\n",&n);
    fscanf(f,"Edges %d\n",&e);
    
    graph.resize(n);
    weights.resize(n);

    for(int i=0;i<n;i++)
    {
        fscanf(f,"N %d\n",&weights[i]);
    }

    int u,v,w;
    for(int i=0;i<e;i++)
    {
        fscanf(f,"E %d %d %d\n",&u,&v,&w);
        graph[u].push_back({v,w});
        graph[v].push_back({u,w});
    }

    while(strncmp(line,"SECTION Terminals\n",17)!=0)
            fgets(line,512,f);

    int gr_num;

    fscanf(f,"Terminals %d\n",&gr_num);
    
    groups.resize(gr_num);

    for(int i=0;i<gr_num;i++)
    {
        fscanf(f,"P %d\n",&groups[i].penalty);
    }
    
    for(int i=0;i<gr_num;i++)
    {
        fgets(line,512,f);
        istringstream iss((string)line);
        char t;
        iss >> t;
        int v;
        while(iss >> v)
        {
            groups[i].vertices.insert(v);
        }
    }
    
    fclose(f);
    resizeAtributes();
}

void Solver::format_txt(Solution& sol,string name, string dir_name)
{
    name="./"+dir_name+"/"+name;
    auto last_dot = name.find_last_of('.');
    name=name.substr(0,last_dot);
    char* filename = (char*)malloc(name.length()+5);
    if(filename==NULL)
    {
        cerr << "Error: malloc\n";
        return;
    }
    strcpy(filename, name.c_str());
    strcat(filename,".txt");
    FILE* f = fopen(filename,"w");
    if(f==NULL)
    {
        cerr << "Error: " << filename << " couldn't be opened.\n";
        return;
    }

    cout<<filename << " se pise...\n";

    fprintf(f,"SECTION Graph\n");
    fprintf(f,"Nodes %ld\n",graph.size());
    fprintf(f,"Edges %ld\n",sol.edges.size());

    for(pair<int,int> p : sol.edges)
    {
        fprintf(f,"E %d %d %d\n", p.first, p.second, tezina_grane(p.first,p.second,graph));
    }

    fprintf(f,"END\n");

    cout<<"Gotovo.\n";

    fclose(f); 
}

void Solver::resizeAtributes()
{
    int n = graph.size();
    shortest_path.resize(n);
    parents.resize(n);
}

void Solver::generateEdges(Solution& s)
{
    
    int n = s.getVertices().size();
    if(n==0)
        return;
    s.connection.resize(n);
    for(int i=0;i<n;i++)
        s.connection[i]={-1,numeric_limits<int>::max()};
    
	vector<bool> dodat(n, false); 
	set< pair<int, int> > pq;
    
	pq.insert({0, 0});

    while(!pq.empty()) {
		int v = pq.begin()->second;
		pq.erase(pq.begin());
		dodat[v] = true;

		for(edge e : graph[s.getVertices()[v]]) { // e koristi graph indekse....
            int indeks = indeks_u_s(e,s);
            if(indeks==-1)
                continue;
            //dobili smo s.vertices indeks suseda
			int duzina = e.w;
			if(!dodat[indeks] && duzina < s.connection[indeks].w) {
				pq.erase({s.connection[indeks].w, indeks});
				s.connection[indeks] = {v, duzina};
				pq.insert({s.connection[indeks].w, indeks});
			}
		}
	}

    //da ne bi modifikovao algoritam tek sad menjamo podatke u pogodniji oblik
    for(int u=1;u<s.connection.size();u++)
    {
         s.edges.insert(make_pair(s.vertices[u], s.vertices[s.connection[u].neighbour]));
    }

}

int Solver::totalWeightOfShortestPath(int u,int i)
{
    int totalWeight=weights[i];
    int pom=i;

    while(parents[u][pom]!=-1)
    {
        totalWeight+=weights[parents[u][pom]]+tezina_grane(pom,parents[u][pom],graph);
        pom=parents[u][pom];
    }

    return totalWeight;
}

set<int> Solver::groupsOnShortestPath(int i,int j)
{
    set<int> coveredGroups;
    for(int u : verticesOnShortestPath(i,j))
                for(int g=0;g<groups.size();g++)
                {
                    if(groups[g].vertices.find(u)!=groups[g].vertices.end())
                    {
                        coveredGroups.insert(g);
                    }
                }
    return coveredGroups;
}

vector<int> Solver::verticesOnShortestPath(int u,int i)
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

void Solver::dijkstra(int u)
{
    int n=graph.size();
    shortest_path[u].resize(n);
    parents[u].resize(n);

    for(int i=0;i<n;i++)
        shortest_path[u][i]=numeric_limits<int>::max();
    shortest_path[u][u]=weights[u];
    set<pair<int,int> > skup; // rastojanje,cvor
    skup.insert({shortest_path[u][u],u});

    parents[u][u]=-1;

    while(!skup.empty())
    {
        int v=skup.begin()->second;
        skup.erase(skup.begin());
        for(edge g : graph[v])
        {
            if(g.w+weights[g.neighbour]+shortest_path[u][v]<shortest_path[u][g.neighbour])
            {
                skup.erase({shortest_path[u][g.neighbour],g.neighbour});
                shortest_path[u][g.neighbour]=g.w+weights[g.neighbour]+shortest_path[u][v];
                skup.insert({shortest_path[u][g.neighbour],g.neighbour});
                parents[u][g.neighbour]=v;
            }
        }
    }
}


Solution Solver::basic_greedy()
{
    int n=graph.size();
    int gr_num=groups.size();
    Solution s = Solution();
    s.numberOfUncoveredGroups.resize(n);

    s.fc=0;

    for(int i=0;i<n;i++)
        s.numberOfUncoveredGroups[i]=0;
    
    for(int g=0;g<groups.size();g++)
    {
        s.fc+=groups[g].penalty; //prazno resenje ima sumu svih penala za fc
        for(int v : groups[g].vertices)
        {
            s.numberOfUncoveredGroups[v] +=1;
        }
    }

    // Prazno resenje je napravljeno.

    set<int> candidates;   //kandidati za cvor za najvece smanjenje fc
    for(int i=0;i<n;i++)
    {
        if(s.numberOfUncoveredGroups[i]>0)
            candidates.insert(i);  //samo ako mogu da smanje penale tj postoji nepokrivena grupa kojoj pripada
    }
    
    //trazimo prvi cvor
    int prvi=-1;
    int min_fc=s.fc;
    for(int cand : candidates)
    {
        int p_fc=s.fc;
        int penali=0;
        for(int g=0;g<gr_num;g++)
        {
            if(groups[g].vertices.find(cand)!=groups[g].vertices.end())
            {
                penali+=groups[g].penalty;
            }
        }
        p_fc=p_fc+weights[cand]-penali;
        if(min_fc>p_fc)
        {   
            min_fc=p_fc;
            prvi=cand;
        }
    }
    if(min_fc<s.fc)
    {
        s.fc=min_fc;
        for(int g=0;g<gr_num;g++)
        {
            if(groups[g].vertices.find(prvi)!=groups[g].vertices.end())
            {
                s.groups.insert(g); 
                //dodali smo grupu pa izbacujemo kandidate kojima je to bila poslednja nepokrivena grupa
                for(int cv : groups[g].vertices)
                {
                    s.numberOfUncoveredGroups[cv]--;
                    if(s.numberOfUncoveredGroups[cv]==0)
                        candidates.erase(cv);
                }
            }
        }
        s.vertices.push_back(prvi);
        candidates.erase(prvi); 
    }
    else
    {
        return s; //prazno resenje najbolje...
    }

    // Resenje sad ima jedan cvor.
    dijkstra(prvi);

    //ostali cvorovi
    while(!candidates.empty())
    {
        int minimum = numeric_limits<int>::max(); //nova vrednost fc
        int cand=-1; // put do ovog cvora dodajemo u resenje
        int cvor_do_kojeg=-1; // od ovog cvora iz resenja
        for(int p_cand : candidates)
        {
            int p_min=s.fc;  
            int p_cvor_do_kojeg=-1;  
            for(int v : s.vertices) // v predstavlja cvor do kojeg
            {
                int tezina_puta = totalWeightOfShortestPath(v,p_cand)-weights[v];
                int penali=0;
                //racunamo penale koji bi se oduzeli
                for(int g : groupsOnShortestPath(v,p_cand))
                {
                    if(s.groups.find(g)==s.groups.end())
                    {
                        penali+=groups[g].penalty; //ako grupa nije u solutionu oduzecemo penal
                    }
                }
                if(p_min>s.fc+tezina_puta-penali)
                {
                    p_min = s.fc+tezina_puta-penali;
                    p_cvor_do_kojeg=v;
                }
            } 
            // za ovaj p_cand smo nasli najbolji cvor do kojeg
            if(p_min<minimum)
            {
                minimum=p_min;
                cand=p_cand;
                cvor_do_kojeg=p_cvor_do_kojeg;
            }
        }
        //ovde smo nasli i cand i cvor do kojeg
        if(minimum<s.fc) // ako popravljamo fc
        {
            //update solutiona
            for(int u : verticesOnShortestPath(cvor_do_kojeg,cand))
            {
                if(u!=cvor_do_kojeg)
                    s.vertices.push_back(u);
            }
            for(int g : groupsOnShortestPath(cvor_do_kojeg,cand))
            {
                s.groups.insert(g);
                //dodali smo grupu pa izbacujemo kandidate kojima je to bila poslednja nepokrivena grupa
                for(int cv : groups[g].vertices)
                {
                    s.numberOfUncoveredGroups[cv]--;
                    if(s.numberOfUncoveredGroups[cv]==0)
                        candidates.erase(cv);
                }
            }
            s.fc = minimum;
            //izbaci put iz kandidata
            for(int c : verticesOnShortestPath(cvor_do_kojeg,cand))
            {
                candidates.erase(c);
                //trebace nam najkraci puutevi do novih cvorova iz solutiona
                dijkstra(c);
            }
            
        }
        else
        {
            //nijedan cvor ne poboljsava ciljnu funkciju
            return s;
        }
    }

    return s;
}

int indeks_u_s(edge e,Solution& s)
{
    for(int i=0;i<s.getVertices().size();i++)
    {
        if(s.getVertices()[i]==e.neighbour)
        {
            return i;
        }
    }
    return -1;
}

int tezina_grane(int u,int v,vector<vector<edge>>& graph)
{
    for(edge g : graph[u])
    {
        if(g.neighbour==v)
            return g.w;
    }
    return -1;//ne desava se
}