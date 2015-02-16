#include <iostream>
#include <iomanip>
#include <array>
#include <cmath>
using namespace std;

template <class T, int deg>
struct Poly {
    array<T, deg+1> coefs;
    T operator()(T x)
    {
        T val{0.0};
        for (int i = 0; i <= deg; ++i)
        {
            val += pow(x,i) * coefs[i];
        }
        return val;
    }
};

template <class T, int deg>
auto derivate(Poly<T,deg> q) 
{
    Poly<T,deg-1> p;
    for (int i = 0; i <= deg-1; ++i)
        p.coefs[i] = (i+1) * q.coefs[i+1];
    return p;
}

template <class T> auto derivate(Poly<T, 0> q)
{
    Poly<T, 0> p;
    p.coefs[0] = 0;
    return p;
}

// a*x = b; return x
double solve(double a, double b)
{
	return b/a;
}

template <class Res>
double find_root(Res r, double x0, 
					double eps = 1.0E-12, unsigned limit = 50)
{
	auto jacob = derivate(r);
	unsigned counter{0u};
	while ( abs(r(x0)) > eps && counter <= limit )
	{
		x0 += solve(jacob(x0), -r(x0));
		counter++;
	}
    return x0;
}

int main()
{
    using namespace std;
    
	Poly<double, 2> p;
	p.coefs[0] = 0.0;
	p.coefs[1] = 0.0;
	p.coefs[2] = 1.0;
	double root = find_root(p, 1.0);
	cout << root << ' ' << p(root) << '\n';
}




