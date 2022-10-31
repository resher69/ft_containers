#pragma once

namespace ft {

template<class T>
struct less {
	typedef bool	result_type;
	typedef T		first_argument_type;
	typedef T		second_argument_type;

	bool operator()(const T &lhs, const T &rhs) const
	{
		return lhs < rhs;
	}
};

}
