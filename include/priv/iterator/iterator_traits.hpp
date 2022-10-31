#pragma once

#include <iterator>
#include "type_traits.hpp"

namespace ft
{

	template <class Iterator>
	struct iterator_traits
	{
		typedef typename Iterator::value_type value_type;
		typedef typename Iterator::difference_type difference_type;
		typedef typename Iterator::iterator_category iterator_category;
		typedef typename Iterator::pointer pointer;
		typedef typename Iterator::reference reference;
	};

	template <class T>
	struct iterator_traits<T *>
	{
		typedef T value_type;
		typedef ptrdiff_t difference_type;
		typedef std::random_access_iterator_tag iterator_category;
		typedef T *pointer;
		typedef T &reference;
	};

	template <class T>
	struct iterator_traits<const T *>
	{
		typedef T value_type;
		typedef ptrdiff_t difference_type;
		typedef std::random_access_iterator_tag iterator_category;
		typedef const T *pointer;
		typedef const T &reference;
	};

	template <class Tag>
	struct is_input_iterator_tag : public false_type
	{
	};
	template <>
	struct is_input_iterator_tag<std::input_iterator_tag> : public true_type
	{
	};

	template <class Iterator>
	struct is_input_iterator
	{
		static const bool value = is_input_iterator_tag<
			typename iterator_traits<Iterator>::iterator_category>::value;
	};

	template <class Tag>
	struct is_bidirectional_iterator_tag : public false_type
	{
	};
	template <>
	struct is_bidirectional_iterator_tag<std::bidirectional_iterator_tag> : public true_type
	{
	};

	template <class Iterator>
	struct is_bidirectional_iterator
	{
		static const bool value = is_bidirectional_iterator_tag<
			typename iterator_traits<Iterator>::iterator_category>::value;
	};

	template <class Tag>
	struct is_random_access_iterator_tag : public false_type
	{
	};
	template <>
	struct is_random_access_iterator_tag<std::random_access_iterator_tag> : public true_type
	{
	};

	template <class Iterator>
	struct is_random_access_iterator
	{
		static const bool value = is_random_access_iterator_tag<
			typename iterator_traits<Iterator>::iterator_category>::value;
	};

}
