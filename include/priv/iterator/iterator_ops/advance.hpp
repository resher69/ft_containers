#pragma once

#include "../iterator_traits.hpp"

namespace ft {

template<class RdmIterator>
void	advance_impl(RdmIterator& it, typename iterator_traits<RdmIterator>::difference_type n, std::random_access_iterator_tag)
{
	it += n;
}

template<class BidirIterator>
void	advance_impl(BidirIterator& it, typename iterator_traits<BidirIterator>::difference_type n, std::bidirectional_iterator_tag)
{
	if (n < 0) {
		while (n++)
			--it;
	} else {
		while (n--)
			++it;
	}
}

template<class InputIterator>
void	advance_impl(InputIterator& it, typename iterator_traits<InputIterator>::difference_type n, std::input_iterator_tag)
{
	if (n < 0) {
		return;
	}
	while (n--)
		++it;
}

template<class Iterator>
void	advance(Iterator& it, typename iterator_traits<Iterator>::difference_type n)
{
	advance_impl(it, n, typename iterator_traits<Iterator>::iterator_category());
}

}
