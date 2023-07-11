#include <iostream>
#include "gst_gen.h"

using namespace std;

int main()
{
    string type, name, remark;
    //cin>>type;
    /*if(type=="rand") name = "RANDOM";
    if(type=="euclid") name = "EUCLIDEAN";
    if(type=="grid") name = "GRID";*/
    remark="T.-D. Nguyen and P.-T. Do: An ant colony optimization algorithm for solving Group Steiner Problem (2013)";
    int n,m,K,B;
    unordered_map<string,int> counter;
    while(cin>>type>>n>>m>>K>>B)
    {
        //int m = ( rand() % ((int)(n*(n-1)/4) - (int)(1.5*n)) ) + ((int)(1.5*n));
        //int K = ( rand() % ((int)(n/4) - (int)(log2(n))) ) + ((int)(log2(n)));
        counter[type]++;
        //cnt++;
        stringstream ss;
        if(counter[type]<=9) ss<<"0";
        ss<<counter[type]<<"_"<<type<<"_"<<"n="<<n<<"_"<<"m="<<m<<"_"<<"k="<<K<<"_B="<<B<<".stp";
        ofstream inFile(ss.str());
        generate_random_gst_instance(inFile, ss.str(), remark, type, m, n, K, B);
    }

    return 0;
}
