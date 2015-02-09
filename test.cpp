#include <iostream>
#include <array>
#include <type_traits>
#include <algorithm>
#include <functional>
using namespace std;

template <size_t N>
struct matrix_desc {
public:
	matrix_desc() = default;

	template <class ... Exts>
	explicit matrix_desc(Exts ... exts);

	explicit matrix_desc(array<size_t, N> extents);

	template <class ... Idxs>
	size_t operator()(Idxs ... idxs) const;

//private:
	array<size_t, N> extents;
	size_t size;
	array<size_t, N> strides;

	// helper-functions
	size_t ext_size();
	void compute_strides();
};

template <size_t N>
	template <class ... Exts>
	matrix_desc<N>::matrix_desc(Exts ... exts) 
		: extents{size_t(exts)...}, size{compute_size()}
	{
		static_assert(sizeof...(exts) == N, "Wrong number of indices");
		compute_strides(strides, extents);
	}

template <size_t N>
	matrix_desc<N>::matrix_desc(array<size_t, N> _extents) 
		: extents(_extents), size{compute_size()}
    {
        compute_strides(strides, extents);
    }

template <size_t N>
	template <class ... Idxs>
	size_t matrix_desc<N>::operator()(Idxs ... idxs) const
	{
		array<size_t,N> indices{size_t(idxs)...};
        return inner_product(begin(strides), end(strides), begin(indices), 0u);
	}

template <size_t N>
	size_t matrix_desc<N>::compute_size()
	{
		multiplies<size_t> mult;
		return accumulate(begin(extents), end(extents), 1, mult);
	}

template <size_t N>
	void matrix_desc<N>::compute_strides()
	{
		strides[N-1] = 1;
		for (int i = strides.size()-1; i >= 0; --i)
			strides[i-1] = extents[i] * strides[i];
	}

int main()
{
	matrix_desc<2> d(2,3);
	cout << d.size << '\n';
	for(auto& e : d.extents)
		cout << e << ' ';
	cout << '\n';
	
	for(auto& e : d.strides)
		cout << e << ' ';
	cout << '\n';
}






