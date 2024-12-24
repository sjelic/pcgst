#include <iostream>
#include "solver.h"
#include <chrono>

void runTest(string& filename)
{
    auto start = chrono::high_resolution_clock::now();
    
    Solver* s = new Solver("../data/tests/"+filename);
    cout<<"posle ucitavanja\n";
    Solution sol = s->basicGreedy();
    s->generateEdges(sol);
    sol.toTxt(filename,"../data/solutions");

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    cout<<"Execution time: " << elapsed.count() <<endl;
}

int main()
{
    ifstream file("../data/instances.txt");
    if (!file.is_open()) {
        throw runtime_error("Failed to open instances.txt");
    }

    string filename;
    while(getline(file,filename))
    {
        runTest(filename);
    }

    return 0;
}
