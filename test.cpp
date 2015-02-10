#include <iostream>
#include <array>
#include <type_traits>
#include <algorithm>
#include <functional>
#include <cassert>
using namespace std;

template <size_t N>
struct matrix_desc 
{
	array<size_t, N> extents;
	size_t size;
	array<size_t, N> strides;
	
	matrix_desc() = default;
	matrix_desc(matrix_desc const &) = default;
	~matrix_desc() = default;

	template <class ... Exts>
	explicit matrix_desc(Exts ... exts);
	explicit matrix_desc(array<size_t, N> extents);
	template <class ... Idxs>
	size_t operator()(Idxs ... idxs) const;
private:
	// helper
	size_t compute_size();
	void compute_strides();
};


template <size_t N>
	template <class ... Exts>
	matrix_desc<N>::matrix_desc(Exts ... exts) 
		: extents{size_t(exts)...}, size{compute_size()}
	{
		static_assert(sizeof...(exts) == N, "Wrong number of indices");
		compute_strides();
	}

template <size_t N>
	matrix_desc<N>::matrix_desc(array<size_t, N> _extents) 
		: extents(_extents), size{compute_size()}
    {
        compute_strides();
    }

template <size_t N>
	template <class ... Idxs>
	size_t matrix_desc<N>::operator()(Idxs ... idxs) const
	{
		static_assert(sizeof...(idxs) == N, "Wrong number of indices");
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







