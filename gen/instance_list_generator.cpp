#include<iostream>
#include<string>
using namespace std;

string tipovi[3] = {"RANDOM","EUCLID","GRID"};

int main()
{
	for (int i = 0; i < 3; ++i)
	{
		for (int fact = 10; fact <= 100; fact = fact*10)
		{
			int vs = fact * 3;
			for (int n = fact; n < fact*10; n = n + vs)
			{
				int es = (n*n - 9*n)/8;
				for (int m = 2*n; m < n*(n-1)/4; m = m + es)
				{
					int gs = n/4;
					for (int k = gs; k < n; k = k + gs)
					{
						cout<<tipovi[i]<<" "<<n<<" "<<m<<" "<<k<<" "<<10<<endl;
					}
				}
			}
		}
		
	}
	return 0;
}