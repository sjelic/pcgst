#include "solver.h"

Solver::Solver(const string& filepath)
{
    graph->loadGraph(filepath);
}

void Solver::findShortestPath(int u)
{
    graph->findShortestPath(u);
}

void Solver::generateEdges(Solution& sol)
{
    graph->generateEdges(sol);
}

Solution Solver::basicGreedy()
{
    int n=getGraphSize();
    int gr_num=getGroups().size();
    Solution s = Solution();
    s.setCost(0);
    s.getNCGC().resize(n);
    for(int i=0;i<n;i++)
        s.getNCGC()[i]=0;
    
    for(int g=0;g<gr_num;g++)
    {
        s.setCost(s.getCost()+getGroups()[g].getPenalty()); //prazno resenje ima sumu svih penala za fc
        for(int v : getGroups()[g].getNodes())
        {
            s.getNCGC()[v] +=1;
        }
    }

    // Prazno resenje je napravljeno.

    set<int> candidates;   //kandidati za cvor za najvece smanjenje fc
    for(int i=0;i<n;i++)
    {
        if(s.getNCGC()[i]>0)
            candidates.insert(i);  //samo ako mogu da smanje penale tj postoji nepokrivena grupa kojoj pripada
    }
    
    //trazimo prvi cvor
    int prvi=-1;
    int min_fc=s.getCost();
    for(int cand : candidates)
    {
        int p_fc=s.getCost();
        int penali=0;
        for(int g=0;g<gr_num;g++)
        {
            if(getGroups()[g].containsNode(cand))
            {
                penali+=getGroups()[g].getPenalty();
            }
        }
        p_fc=p_fc+getWeights()[cand]-penali;
        if(min_fc>p_fc)
        {   
            min_fc=p_fc;
            prvi=cand;
        }
    }
    if(min_fc<s.getCost())
    {
        s.setCost(min_fc);
        for(int g=0;g<gr_num;g++)
        {
            if(getGroups()[g].containsNode(prvi))
            {
                s.addGroup(g); 
                //dodali smo grupu pa izbacujemo kandidate kojima je to bila poslednja nepokrivena grupa
                for(int cv : getGroups()[g].getNodes())
                {
                    s.getNCGC()[cv]--;
                    if(s.getNCGC()[cv]==0)
                        candidates.erase(cv);
                }
            }
        }
        s.addNode(prvi);
        candidates.erase(prvi); 
    }
    else
    {
        return s; //prazno resenje najbolje...
    }

    // Resenje sad ima jedan cvor.
    findShortestPath(prvi);

    //ostali cvorovi
    while(!candidates.empty())
    {
        int minimum = numeric_limits<int>::max(); //nova vrednost fc
        int cand=-1; // put do ovog cvora dodajemo u resenje
        int cvor_do_kojeg=-1; // od ovog cvora iz resenja
        for(int p_cand : candidates)
        {
            int p_min=s.getCost();  
            int p_cvor_do_kojeg=-1;  
            for(int v : s.getNodes()) // v predstavlja cvor do kojeg
            {
                int tezina_puta = totalWeightOfShortestPath(v,p_cand)-getWeights()[v];
                int penali=0;
                //racunamo penale koji bi se oduzeli
                for(int g : groupsOnShortestPath(v,p_cand))
                {
                    if(!s.containsGroup(g))
                    {
                        penali+=getGroups()[g].getPenalty(); //ako grupa nije u solutionu oduzecemo penal
                    }
                }
                if(p_min>s.getCost()+tezina_puta-penali)
                {
                    p_min = s.getCost()+tezina_puta-penali;
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
        if(minimum<s.getCost()) // ako popravljamo fc
        {
            //update solutiona
            for(int u : verticesOnShortestPath(cvor_do_kojeg,cand))
            {
                if(u!=cvor_do_kojeg)
                    s.addNode(u);
            }
            for(int g : groupsOnShortestPath(cvor_do_kojeg,cand))
            {
                s.addGroup(g);
                //dodali smo grupu pa izbacujemo kandidate kojima je to bila poslednja nepokrivena grupa
                for(int cv : getGroups()[g].getNodes())
                {
                    s.getNCGC()[cv]--;
                    if(s.getNCGC()[cv]==0)
                        candidates.erase(cv);
                }
            }
            s.setCost(minimum);
            //izbaci put iz kandidata
            for(int c : verticesOnShortestPath(cvor_do_kojeg,cand))
            {
                candidates.erase(c);
                //trebace nam najkraci puutevi do novih cvorova iz solutiona
                findShortestPath(c);
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

Solution Solver::randomGreedy(int r)
{
    int n=getGraphSize();
    Solution s = Solution();
    s.setCost(0);
    s.getNCGC().resize(n);
    for(int i=0;i<n;i++)
        s.getNCGC()[i]=0;
    
    for(int g=0;g<getGroups().size();g++)
    {
        s.setCost(s.getCost()+getGroups()[g].getPenalty()); //prazno resenje ima sumu svih penala za fc
        for(int v : getGroups()[g].getNodes())
        {
            s.getNCGC()[v] +=1;
        }
    }

    set<int> candidates;   //kandidati za cvor za najvece smanjenje fc
    for(int i=0;i<n;i++)
    {
        if(s.getNCGC()[i]>0)
            candidates.insert(i);  //samo ako mogu da smanje penale tj postoji nepokrivena grupa kojoj pripada
    }
    //pq najboljih r opcija za kandidata
    //trazimo prvi cvor
    priority_queue<pair<int,int>> pq; // first: fc , second: kandidat
    
    for(int cand : candidates)
    {
        int p_fc=s.getCost();
        int penali=0;
        for(int g=0;g<getGroups().size();g++)
        {
            if(getGroups()[g].containsNode(cand))
            {
                penali+=getGroups()[g].getPenalty();
            }
        }
        p_fc=p_fc+getWeights()[cand]-penali;
        //update pq-a
        if(pq.size()<r && p_fc<s.getCost()) //ako resenje poboljsava a pq nije napunjen
        {
            pq.push({p_fc,cand});
        }
        else if(pq.size()==r && (pq.top().first)>p_fc) //ako je bolji od najgoreg u punom pq-u
        {
            pq.pop();
            pq.push({p_fc,cand});
        }
    }
    //uzimamo random kandidata iz pq-a i updatujemo solution
    if(pq.size()!=0)
    {
        int random = rand() % pq.size();
        for(int i=0;i<random;i++)
           pq.pop();

        int prvi = pq.top().second;
        s.setCost(pq.top().first);

        for(int g=0;g<getGroups().size();g++)
        {
            if(getGroups()[g].containsNode(prvi))
            {
                s.addGroup(g); 
                //dodali smo grupu pa izbacujemo kandidate kojima je to bila poslednja nepokrivena grupa
                for(int cv : getGroups()[g].getNodes())
                {
                    s.getNCGC()[cv]--;
                    if(s.getNCGC()[cv]==0)
                        candidates.erase(cv);
                }
            }
        }
        s.addNode(prvi);
        candidates.erase(prvi); 
        // Resenje sad ima jedan cvor.
        findShortestPath(prvi);
    }
    else //prazan pq znaci niko ne poboljsava fc
    {
        return s;
    }

    

    //ostali cvorovi
    while(!candidates.empty())
    {
        priority_queue<pair<int,pair<int,int>>> pq; // (fc,(kandidat,cvor do kojeg))
        for(int p_cand : candidates)
        {
            int p_min=s.getCost();  
            int p_cvor_do_kojeg=-1;  
            for(int v : s.getNodes()) // v predstavlja cvor do kojeg
            {
                int tezina_puta = totalWeightOfShortestPath(v,p_cand)-getWeights()[v];
                int penali=0;
                //racunamo penale koji bi se oduzeli
                for(int g : groupsOnShortestPath(v,p_cand))
                {
                    if(!s.containsGroup(g))
                    {
                        penali+=getGroups()[g].getPenalty(); //ako grupa nije u solutionu oduzecemo penal
                    }
                }
                if(p_min>s.getCost()+tezina_puta-penali)
                {
                    p_min = s.getCost()+tezina_puta-penali;
                    p_cvor_do_kojeg=v;
                }
            } 
            // za ovaj p_cand smo nasli najbolji cvor do kojeg
            // ispitujemo da li treba dodati p_min,p_cand,p_cvordokojeg u pq
            if(p_min<s.getCost() && pq.size()<r)
            {
                pq.push({p_min,{p_cand,p_cvor_do_kojeg}});
            }
            else if (p_min<s.getCost() && pq.size()==r && pq.top().first > p_min)
            {
                pq.pop();
                pq.push({p_min,{p_cand,p_cvor_do_kojeg}});
            }
            
        }
        //uzimamo random iz pq-a
        if(pq.size()>0)
        {
            int random = rand() % r;
            for(int i=0;i<random;i++)
                pq.pop();
            
            int nova_fc = pq.top().first;
            int cand = pq.top().second.first;
            int cvor_do_kojeg = pq.top().second.second;

            //update solutiona
            for(int u : verticesOnShortestPath(cvor_do_kojeg,cand))
            {
                if(u!=cvor_do_kojeg)
                    s.addNode(u);
            }
            for(int g : groupsOnShortestPath(cvor_do_kojeg,cand))
            {
                s.addGroup(g);
                //dodali smo grupu pa izbacujemo kandidate kojima je to bila poslednja nepokrivena grupa
                for(int cv : getGroups()[g].getNodes())
                {
                    s.getNCGC()[cv]--;
                    if(s.getNCGC()[cv]==0)
                        candidates.erase(cv);
                }
            }
            s.setCost(nova_fc);
            //izbaci put iz kandidata
            for(int c : verticesOnShortestPath(cvor_do_kojeg,cand))
            {
                candidates.erase(c);
                findShortestPath(c);
            }
            
        }
        else
        {
            //pq prazan znaci nijedan cvor ne poboljsava ciljnu funkciju
            return s;
        }
    }

    return s;
}

int Solver::getGraphSize()
{
    return graph->getSize();
}

vector<int>& Solver::getWeights()
{
    return graph->getWeights();
}

vector<Group>& Solver::getGroups()
{
    return graph->getGroups();
}

int Solver::tezina_grane(int u,int v)
{
    for(edge g : graph->getGraph()[u])
    {
        if(g.neighbour==v)
            return g.w;
    }
    return -1;//ne desava se
}