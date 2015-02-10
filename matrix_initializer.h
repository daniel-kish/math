#ifndef MATRIX_INITIALIZER_H
#define MATRIX_INITIALIZER_H


#include <type_traits>
#include <utility>
#include <array>

// matrix_list type generator. Given a type T and size_t N it
// generates N-times nested initializer_list<T>, type of which
// can be found through mx_list_gen<T,N>::type
template <class T, std::size_t N>
struct mx_list_gen {
    using type = std::initializer_list< typename mx_list_gen<T, N-1>::type >;
};

template <class T>
struct mx_list_gen<T,1> {
    using type = std::initializer_list<T>;
};

template <class T>
struct mx_list_gen<T,0>; // undefined on purpose

// the actual type that will be used
template <class T, std::size_t N>
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
template <std::size_t N, class List, class Iter>
std::enable_if_t<(N==1), void> deduce_extents( Iter& f, List const & list )
{
    ++f;
    *f = list.size();
}

template <size_t N, class List, class Iter>
std::enable_if_t<(N>1), void> deduce_extents( Iter& f, List const & list )
{
    assert(check_list(list));
    ++f;
    *f = list.size();
    deduce_extents<N-1>(f, *begin(list));
}

template <std::size_t N, class List>
std::array<std::size_t, N>  deduce_extents( List const & list )
{
    std::array<std::size_t,N> extents;
    assert(check_list(list));
    auto f = begin(extents);
    *f = list.size();
    deduce_extents<N-1>(f, *begin(list));
    return extents;
}
// end deduce_extents()


// insert flat
template <std::size_t N, class List, class Vector>
std::enable_if_t<(N==1), void> insert_flat(List const & list, Vector & v)
{
    v.insert(end(v), begin(list), end(list));
}

template <std::size_t N, class List, class Vector>
std::enable_if_t<(N>1), void> insert_flat(List const & list, Vector & v)
{
    for (auto it = begin(list); it != end(list); ++it)
        insert_flat<N-1>(*it, v);
}
// end insert flat


#endif // MATRIX_INITIALIZER_H
