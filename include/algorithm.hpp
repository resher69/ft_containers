#pragma once

namespace ft {

template<class T>
const T& max(const T& a, const T& b)
{
    return (a < b) ? b : a;
}

template<class T, class Compare>
const T& max(const T& a, const T& b, Compare comp)
{
    return (comp()(a, b)) ? b : a;
}

template<class T>
const T& min(const T& a, const T& b)
{
    return (b < a) ? b : a;
}

template<class T, class Compare>
const T& min(const T& a, const T& b, Compare comp)
{
    return (comp()(b, a)) ? b : a;
}

template<class T>
void swap(T& a, T& b)
{
	T tmp = a;
	a = b;
	b = tmp;
}

template<class InputIterator1, class InputIterator2>
bool lexicographical_compare( InputIterator1 first1, InputIterator1 last1,
                              InputIterator2 first2, InputIterator2 last2 )
{
	for (; (first1 != last1) && (first2 != last2); ++first1, (void)++first2) {
		if (*first1 < *first2) return true;
		if (*first2 < *first1) return false;
	}
	return (first1 == last1) && (first2 != last2);
}

template<class InputIterator1, class InputIterator2, class Compare>
bool lexicographical_compare( InputIterator1 first1, InputIterator1 last1,
                              InputIterator2 first2, InputIterator2 last2,
                              Compare comp )
{
	for (; (first1 != last1) && (first2 != last2); ++first1, (void)++first2) {
		if (comp(*first1, *first2)) return true;
		if (comp(*first2, *first1)) return false;
	}
	return (first1 == last1) && (first2 != last2);
}

template<class InputIterator1, class InputIterator2>
bool equal( InputIterator1 first1, InputIterator1 last1,
            InputIterator2 first2 )
{
	for (; first1 != last1; ++first1, ++first2) {
		if (!(*first1 == *first2)) {
			return false;
		}
	}
	return true;
}

template<class InputIterator1,
          class InputIterator2,
          class BinaryPredicate>
bool equal( InputIterator1 first1,
            InputIterator1 last1,
            InputIterator2 first2,
            BinaryPredicate p )
{
	for (; first1 != last1; ++first1, ++first2) {
		if (!p(*first1, *first2)) {
			return false;
		}
	}
	return true;
}

}
