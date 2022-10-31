#pragma once

#include <cassert>
#include "type_traits.hpp"
#include "advance.hpp"

namespace ft {

template<class Iterator>
typename enable_if<
	is_input_iterator<Iterator>::value,
	Iterator
>::type next(Iterator it, typename iterator_traits<Iterator>::difference_type diff = 1)
{
	if (diff < 0) {
		assert(is_bidirectional_iterator<Iterator>::value);
	}
	advance(it, diff);
	return it;
}

}
