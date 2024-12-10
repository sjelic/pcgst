#include <iostream>
#include "solution.hpp"
#include "solver.hpp"
#include <chrono>

int main(int argc,char* args[])
{
    auto start = chrono::high_resolution_clock::now();

    Solver s = Solver();
    s.loadGraph(args[1],"../data"); 
    Solution sol = s.basic_greedy();
    s.generateEdges(sol);
    s.format_txt(sol,args[1],"solutions");

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    cout<<"Execution time: " << elapsed.count() <<endl;

    return 0;
}
