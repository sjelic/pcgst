#include<iostream>
#include<string>
using namespace std;
#include <math.h>

string tipovi[3] = {"RANDOM","EUCLID","GRID"};

int main()
{
	for (int i = 0; i < 3; ++i)
	{
		for (int n = 100; n<= 900; n+=100)
		{
			int ms[] = {int(1.5*n), 2*n, int(n*log(n)), 2*int(n*log(n)), int(n*(n-1)/4.0)};
			for (int j=0; j<5; j++)
			{
				int ks[] = {int(log(n)), int(sqrt(n)), 2*int(sqrt(n)), int(n/4.0)};
				for (int l=0; l<4; l++)
				{
					cout<<tipovi[i]<<" "<<n<<" "<<ms[j]<<" "<<ks[l]<<" "<<10<<endl;
				}
				
			}
		}
		
	}
	return 0;
}