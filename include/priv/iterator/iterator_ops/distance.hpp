#pragma once

#include "../iterator_traits.hpp"

namespace ft {

template<class RdmIterator>
typename iterator_traits<RdmIterator>::difference_type	distance_impl(RdmIterator first, RdmIterator last, std::random_access_iterator_tag)
{
	return last - first;
}

template<class InputIterator>
typename iterator_traits<InputIterator>::difference_type	distance_impl(InputIterator first, InputIterator last, std::input_iterator_tag)
{
	typename iterator_traits<InputIterator>::difference_type diff(0);

	for (; first != last ; ++first) {
		++diff;
	}
	return diff;
}

template<class Iterator>
typename iterator_traits<Iterator>::difference_type	distance(Iterator first, Iterator last,
	typename ft::enable_if<!ft::is_integral<Iterator>::value >::type* = 0)
{
	return distance_impl(first, last, typename iterator_traits<Iterator>::iterator_category());
}

}
