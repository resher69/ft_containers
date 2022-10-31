#pragma once

namespace ft {

struct false_type { static const bool value = false; };
struct true_type  { static const bool value = true;  };

template<bool B, class T = void>
struct enable_if {};

template<class T>
struct enable_if<true, T> { typedef T	type; };

template<class T>	struct is_integral								: public false_type {};
template<>			struct is_integral<bool>						: public true_type {};
template<>			struct is_integral<char>						: public true_type {};
template<>			struct is_integral<short>						: public true_type {};
template<>			struct is_integral<int>							: public true_type {};
template<>			struct is_integral<long>						: public true_type {};
template<>			struct is_integral<long long>					: public true_type {};

template<>			struct is_integral<unsigned char>				: public true_type {};
template<>			struct is_integral<unsigned short>				: public true_type {};
template<>			struct is_integral<unsigned int>				: public true_type {};
template<>			struct is_integral<unsigned long>				: public true_type {};
template<>			struct is_integral<unsigned long long>			: public true_type {};

template<>			struct is_integral<const bool>					: public true_type {};
template<>			struct is_integral<const char>					: public true_type {};
template<>			struct is_integral<const short>					: public true_type {};
template<>			struct is_integral<const int>					: public true_type {};
template<>			struct is_integral<const long>					: public true_type {};
template<>			struct is_integral<const long long>				: public true_type {};

template<>			struct is_integral<const unsigned char>			: public true_type {};
template<>			struct is_integral<const unsigned short>		: public true_type {};
template<>			struct is_integral<const unsigned int>			: public true_type {};
template<>			struct is_integral<const unsigned long>			: public true_type {};
template<>			struct is_integral<const unsigned long long>	: public true_type {};

}
