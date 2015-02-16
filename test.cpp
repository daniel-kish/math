#include <iostream>

#include "matrix.h"

auto f()
{
	matrix<int,2> m(10,10);
	m(9,8) = 908;
	return m;
}

int main() 
try
{
	using namespace std;

	matrix<int,2> m(10,10);
	
	m(9,7) += 23;
	cout << m(9,7) << '\n';
	
	for (auto& e : m)
		e++;
	cout << m(0,4) << '\n';
	
	matrix<int,2> copy(f());
	
	cout << copy(9,8) << '\n';
}

catch(std::bad_alloc& e)
{
	std::cout << "not enough memory!\n";
	return 1;
}

