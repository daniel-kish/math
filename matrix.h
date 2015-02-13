#include <vector>
#include <iostream>

#include "matrix_desc.h"
#include "matrix_initializer.h"
																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																														
template <class T, std::size_t N>
class matrix
{
public:
	constexpr std::size_t order() const { return N; }

	matrix() = default;					// empty matrix
	matrix(matrix const & m) = default;
	matrix & operator = (matrix const & m) = default;
	matrix(matrix && m) = default;
	matrix & operator = (matrix && m) = default;

	template <class... Exts>
	matrix(Exts ... exts);				// extents initialization
	matrix(matrix_initializer<T,N>);	// list initialization

	template <class ... Idxs>
	const T& operator()(Idxs ... idxs) const;
	template <class ... Idxs>
	T& operator()(Idxs ... idxs);

	inline std::size_t extent(size_t idx) const
	{
		assert(idx < N);
		return desc.extents[idx];
	}
	inline std::size_t size() const { return elems.size(); }

	// container-like iterator interface
	using iterator = typename std::vector<T>::iterator;
	inline iterator begin() { return elems.begin(); }
	inline iterator end() { return elems.end(); }
private:
	matrix_desc<N> desc;
	std::vector<T> elems;
};

template <class T, std::size_t N>
	template <class... Exts>
    matrix<T,N>::matrix(Exts ... exts) : desc(exts...), elems(desc.size)
    {
    	static_assert(sizeof...(exts) == N, "MX: order mismatch");
    }

template <class T, std::size_t N>
matrix<T,N>::matrix( matrix_initializer<T,N> l ) : desc{ deduce_extents<N>(l) }
{
    insert_flat<N>(l, elems);
}

template <class T, std::size_t N>
	template <class ... Idxs>
	const T& matrix<T,N>::operator()(Idxs ... idxs) const
	{
		static_assert(sizeof...(idxs) == N, "Wrong number of indices");
        return elems[ desc(idxs...) ];
	}

template <class T, std::size_t N>
	template <class ... Idxs>
	T& matrix<T,N>::operator()(Idxs ... idxs)
	{
		static_assert(sizeof...(idxs) == N, "Wrong number of indices");
        return elems[ desc(idxs...) ];
	}

template <class Cont>
using Iterator = typename Cont::iterator;

template <class T, std::size_t N>
Iterator<matrix<T,N>> begin(matrix<T,N> const & m)
{
	return m.begin();
}

template <class T, std::size_t N>
Iterator<matrix<T,N>> end(matrix<T,N> const & m)
{
	return m.end();
}
