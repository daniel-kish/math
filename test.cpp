#include <array>
#include <algorithm>
#include <cassert>

template <std::size_t N>
struct matrix_desc 
{
	std::array<std::size_t, N> extents;
	std::size_t size;
	std::array<std::size_t, N> strides;
	
	matrix_desc() = default;
	matrix_desc(matrix_desc const &) = default;
	~matrix_desc() = default;

	template <class ... Exts>
	explicit matrix_desc(Exts ... exts);
	
	explicit matrix_desc(std::array<std::size_t, N> extents);
	
	template <class ... Idxs>
	std::size_t operator()(Idxs ... idxs) const;
private:
	// helpers
	std::size_t compute_size();
	void compute_strides();
};


template <std::size_t N>
	template <class ... Exts>
	matrix_desc<N>::matrix_desc(Exts ... exts) 
		: extents{std::size_t(exts)...}, size{compute_size()}
	{
		static_assert(sizeof...(exts) == N, "Wrong number of indices");
		compute_strides();
	}

template <std::size_t N>
	matrix_desc<N>::matrix_desc(std::array<std::size_t, N> _extents) 
		: extents(_extents), size{compute_size()}
    {
        compute_strides();
    }

template <std::size_t N>
	template <class ... Idxs>
	std::size_t matrix_desc<N>::operator()(Idxs ... idxs) const
	{
		static_assert(sizeof...(idxs) == N, "Wrong number of indices");
		std::array<std::size_t,N> indices{std::size_t(idxs)...};
        return std::inner_product(begin(strides), end(strides), begin(indices), 0u);
	}

template <std::size_t N>
std::size_t matrix_desc<N>::compute_size()
{
	std::multiplies<std::size_t> mult;
	return std::accumulate(begin(extents), end(extents), 1, mult);
}

template <std::size_t N>
void matrix_desc<N>::compute_strides()
{
	strides[N-1] = 1;
	for (int i = strides.size()-1; i >= 0; --i)
		strides[i-1] = extents[i] * strides[i];
}
