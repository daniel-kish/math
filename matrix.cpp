#include <chrono>
#include <array>
#include <iostream>

#include "matrix.h"
using namespace std;

int main() 
{
	using namespace chrono;
	
	matrix<double,2> m(1000,1000);
	
	int i=0;
	
	auto t0 = high_resolution_clock::now();
	for (double& a : m)
		++a;
	auto t1 = high_resolution_clock::now();
	cout << duration_cast<microseconds>(t1-t0).count() << '\n';
	cout << m(3,4) << '\n';	
}










