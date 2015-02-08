#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <cassert>
#include <type_traits>
using namespace std;


template <size_t N>
struct matrix_desc {
    array<size_t, N> extents;
    size_t size;
    array<size_t, N> strides;
  public:
    template <class ... Exts>
    matrix_desc(Exts ... exts) : extents{exts...}
    {
    	size = ext_size(extents);
    	cout << "size: " << size << '\n';
        compute_strides(strides, extents);
    }
    matrix_desc(array<size_t, N> exts) : extents(exts), size{ ext_size(extents) }
    {
        compute_strides(strides, extents);
    }
    matrix_desc() = default;
    
    template <class ... Idxs>
    size_t operator()(Idxs ... idxs) const
    {
        size_t idx[N]{size_t(idxs)...};
        return inner_product(begin(strides), end(strides), idx, 0u);
    }
    
    // overall size from extents
	size_t ext_size(array<size_t, N> const & a, size_t s = 0)
	{
		return (s == N)? 1u : a[s] * ext_size(a, s+1);
	}
	
	// strides for indexing
	void compute_strides(array<size_t,N> & strs, array<size_t,N> const & exts)
	{
		strs[N-1] = 1;
		for (int i = strs.size()-1; i >= 0; --i)
				strs[i-1] = exts[i] * strs[i];
	}

};

/*--- matrix_list initilization ---*/

// matrix_list type generator. Given a type T and size_t N it
// generates N-times nested initializer_list<T>, type of which
// can be found through mx_list_gen<T,N>::type
 
template <class T, size_t N>
struct mx_list_gen {
    using type = initializer_list< typename mx_list_gen<T, N-1>::type >;
};

template <class T>
struct mx_list_gen<T,1> {
    using type = initializer_list<T>;
};

template <class T>
struct mx_list_gen<T,0>; // undefined on purpose

// the actual type that will be used
template <class T, size_t N>
using matrix_initializer = typename mx_list_gen<T,N>::type;


// check whether list contains sublists of same sizes (non-recursively)
template <class List>
bool check_list( List const & list )
{
    auto size = begin(list)->size(); // size of first sublist
    for (auto it = begin(list)+1; it != end(list); ++it)
        if(it->size() != size)
            return false;
    return true;
}

// deduce_extents()
template <size_t N, class List, class Iter>
enable_if_t<(N==1), void> deduce_extents( Iter& f, List const & list )
{
    ++f;
    *f = list.size();
}

template <size_t N, class List, class Iter>
enable_if_t<(N>1), void> deduce_extents( Iter& f, List const & list )
{
    assert(check_list(list));
    ++f;
    *f = list.size();
    deduce_extents<N-1>(f, *begin(list));
}

template <size_t N, class List>
array<size_t, N>  deduce_extents( List const & list )
{
    array<size_t,N> extents;
    assert(check_list(list));
    auto f = begin(extents);
    *f = list.size();
    deduce_extents<N-1>(f, *begin(list));
    return extents;
}
// end deduce_extents()

// insert flat
template <size_t N, class List, class Vector>
enable_if_t<(N==1), void> insert_flat(List const & list, Vector & v)
{
    v.insert(end(v), begin(list), end(list));
}

template <size_t N, class List, class Vector>
enable_if_t<(N>1), void> insert_flat(List const & list, Vector & v)
{
    for (auto it = begin(list); it != end(list); ++it)
        insert_flat<N-1>(*it, v);
}
// end insert flat

/*--- end matrix_list initilization ---*/

template <class T, size_t N>
class matrix
{
  public:
  	using iterator = typename std::vector<T>::iterator;
  	
  	constexpr size_t order() const { return N; }
  	
    template <class... Exts>
    matrix(Exts ... exts) : desc{size_t(exts)...}
    {
        static_assert(sizeof...(exts) == N, "MX: order mismatch");
        desc.size;
        elems.resize(desc.size);
        cout << "dbg " << elems.size() << '\n';
    }
    
    matrix(matrix const & m) = default;
    matrix & operator = (matrix const & m) = default;
    matrix(matrix && m) = default;
    matrix & operator = (matrix && m) = default;
    
    // list initialization
    matrix( matrix_initializer<T,N> l ) : desc{ deduce_extents<N>(l) }
    {
        insert_flat<N>(l, elems);
    }
    
    template <class ... Idxs>
    const T& operator()(Idxs ... idxs) const
    {
        static_assert(sizeof...(idxs) == N, "Wrong number of indices");
        return elems[ desc(idxs...) ];
    }
    template <class ... Idxs>
    T& operator()(Idxs ... idxs)
    {
        static_assert(sizeof...(idxs) == N, "Wrong number of indices");
        return elems[ desc(idxs...) ];
    }
    
    inline size_t extent(size_t idx) const
    {
        assert(idx < N);
        return desc.extents[idx];
    }
    inline size_t size() const
    {
        return elems.size();
    }
    
    inline iterator begin()
    {
    	return elems.begin();
    }
    inline iterator end()
    {
    	return elems.end();
    }
  private:
    vector<T> elems;
    matrix_desc<N> desc;
};

template <class Cont>
using Iterator = typename Cont::iterator;


template <class T, size_t N>
Iterator<matrix<T,N>> begin(matrix<T,N> const & m)
{
	return m.begin();
}

template <class T, size_t N>
Iterator<matrix<T,N>> end(matrix<T,N> const & m)
{
	return m.end();
}


