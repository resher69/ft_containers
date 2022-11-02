#pragma once

#include "iterator.hpp"
#include "algorithm.hpp"
#include <sstream>
#include <memory>

#include <iostream>

namespace ft {

template< class T, class Allocator = std::allocator<T> >
class vector {
	public:
		typedef T											value_type;
		typedef Allocator									allocator_type;
		typedef typename allocator_type::size_type			size_type;
		typedef typename allocator_type::difference_type	difference_type;
		typedef typename allocator_type::pointer			pointer;
		typedef typename allocator_type::const_pointer		const_pointer;
		typedef T&											reference;
		typedef const T&									const_reference;
		typedef pointer										iterator;
		typedef const_pointer								const_iterator;
		typedef ft::reverse_iterator<iterator>				reverse_iterator;
		typedef ft::reverse_iterator<const_iterator>		const_reverse_iterator;

	private:
		allocator_type	_allocator;
		pointer			_data;
		size_type		_size;
		size_type		_capacity;

	public:

	#pragma region Constructors

	vector()
		: _allocator(allocator_type()), _data(pointer()), _size(0), _capacity(0)
	{	}

	explicit vector(const allocator_type& alloc)
		: _allocator(alloc), _data(pointer()), _size(0), _capacity(0)
	{	}

	explicit vector(size_type count, const_reference value = value_type(), const allocator_type& alloc = allocator_type())
		: _allocator(alloc), _data(pointer()), _size(0), _capacity(0)
	{
		this->assign(count, value);
	}

	//SFINAE
	template<class InputIterator>
	explicit vector(InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type(),
		typename ft::enable_if<!ft::is_integral<InputIterator>::value >::type* = 0)
		: _allocator(alloc), _data(pointer()), _size(0), _capacity(0)
	{
		this->assign(first, last);
	}

	vector(const vector& other)
		: _allocator(other._allocator), _data(pointer()), _size(0), _capacity(0)
	{
		vector::operator=(other);
	}

	#pragma endregion Constructors

	#pragma region Destructor

	~vector()
	{
		this->clear();
		this->deallocate(_data, _capacity);
	}

	#pragma endregion Destructor

	void assign(size_type count, const_reference value)
	{
		this->clear();
		this->reserve(count);
		_size = count;
		for (size_type i = 0; i < count; i++) {
			_allocator.construct(_data + i, value);
		}
	}

	//SFINAE
	template<class InputIterator>
	void assign(InputIterator first,
		typename ft::enable_if<!ft::is_integral<InputIterator>::value, InputIterator >::type last)
	{
		this->clear();
        for (; first != last; ++first) {
            this->push_back(*first);
        }
	}

	vector& operator=(const vector& other)
	{
		_allocator = other._allocator;
		this->reserve(other.size());
		this->assign(other.begin(), other.end());
		return *this;
	}

	allocator_type get_allocator() const
	{
		return _allocator;
	}

	#pragma region Element_access

	reference at(size_type pos)
	{
		if (pos >= _size) {
			this->throw_out_of_range_error(pos);
		}
		return _data[pos];
	}

	const_reference at(size_type pos) const
	{
		if (pos >= _size) {
			this->throw_out_of_range_error(pos);
		}
		return _data[pos];
	}

	reference operator[](size_type pos)
	{
		return _data[pos];
	}

	const_reference operator[](size_type pos) const
	{
		return _data[pos];
	}

	reference front()
	{
		return *_data;
	}

	reference back()
	{
		return *(this->end() - 1);
	}

	const_reference front() const
	{
		return *_data;
	}

	const_reference back() const
	{
		return *(this->end() - 1);
	}

	pointer data()
	{
		return _data;
	}

	const_pointer data() const
	{
		return _data;
	}

	#pragma endregion Element_access

	#pragma region Iterators

	iterator begin()
	{
		return _data;
	}

	const_iterator begin() const
	{
		return _data;
	}

	iterator end()
	{
		return _data + _size;
	}

	const_iterator end() const
	{
		return _data + _size;
	}

	reverse_iterator rbegin()
	{
		return reverse_iterator(this->end());
	}

	const_reverse_iterator rbegin() const
	{
		return reverse_iterator(this->end());
	}

	reverse_iterator rend()
	{
		return reverse_iterator(this->begin());
	}

	const_reverse_iterator rend() const
	{
		return reverse_iterator(this->begin());
	}

	#pragma endregion Iterators

	#pragma region Capacity

	bool empty() const
	{
		return _size == 0;
	}

	size_type size() const
	{
		return _size;
	}

	size_type max_size() const
	{
		return ft::min(_allocator.max_size(), static_cast<size_type>(std::numeric_limits<difference_type>::max()));
	}

	void reserve(size_type new_cap)
	{
		if (new_cap == 0 || new_cap <= _capacity) {
			return ;
		}
		this->check_max_length(new_cap);
		pointer tmp = this->allocate(new_cap);
		for (size_type i = 0; i < _size; i++) {
			_allocator.construct(tmp + i, _data[i]);
			_allocator.destroy(&_data[i]);
		}

		this->deallocate(_data, _capacity);
		_data = tmp;
		_capacity = new_cap;
	}

	size_type capacity() const
	{
		return _capacity;
	}

	#pragma endregion Capacity

	#pragma region Modifiers

	void clear()
	{
		if (_size == 0) {
			return ;
		}
		for (size_type i = 0; i < _size; i++) {
			_allocator.destroy(&_data[i]);
		}
		_size = 0;
	}

	iterator insert(iterator pos, const T& value)
	{
		size_type index = pos - _data;
		if (pos == this->end()) {
			this->push_back(value);
		} else {
			this->insert(pos, 1, value);
		}
		return iterator(_data + index);
	}

	void insert(iterator pos, size_type count, const_reference value)
	{
		size_type index = pos - _data;

		this->reserve(_size + count);

		iterator mv_src = this->end() - 1;
		iterator mv_dst = mv_src + count;
		for(; mv_src >= _data + index; --mv_src, --mv_dst) {
			_allocator.construct(ft::addressof(*mv_dst), *mv_src);
			_allocator.destroy(ft::addressof(*mv_src));
		}
		++mv_src;
		for (size_type i = 0; i < count; i++) {
			_allocator.construct(ft::addressof(*mv_src), value);
			++mv_src;
		}
		_size += count;
	}

	template<class InputIterator>
	void insert(iterator pos, InputIterator first, InputIterator last,
		typename ft::enable_if<!ft::is_integral<InputIterator>::value >::type* = 0)
	{
		size_type index = pos - _data;
		size_type count = 0;

		count = std::distance(first, last);

		this->reserve(_size + count);

		for (size_type i = 0; i < count; ++i) {
			_allocator.construct(_data + _size + i, *first);
		}
		for (difference_type i = _size - 1; i >= 0 && i >= (difference_type)index; --i) {
			_data[i + count] = _data[i];
		}
		for (size_type i = index; i < index + count; ++i) {
			_data[i] = *first++;
		}
		_size += count;
	}

	iterator erase(iterator pos)
	{
		if (pos + 1 != this->end()) {
			std::copy(pos + 1, this->end(), pos);
		}
		--_size;
		_allocator.destroy(_data + _size);
		return pos;
	}

	iterator erase(iterator first, iterator last)
	{
		if (first != last) {
            if (last != this->end()) {
                std::copy(last, this->end(), first);
            }
            pointer new_end = first + (this->end() - last);
            this->destroy_from(new_end);
        }
        return first;
	}

	void push_back(const_reference value)
	{
		if (_size + 1 > _capacity) {
			this->extend();
		}
		this->unchecked_push_back(value);
	}

	void pop_back()
	{
		_allocator.destroy(_data + --_size);
	}

	void resize(size_type count, value_type value = value_type())
	{
		if (count > _size) {
			this->insert(this->end(), count - _size, value);
		} else if (count < _size) {
			this->destroy_from(_data + count);
		}
	}

	void swap(vector& other)
	{
		ft::swap(_allocator, other._allocator);
		ft::swap(_size, other._size);
		ft::swap(_capacity, other._capacity);
		ft::swap(_data, other._data);
	}

	#pragma endregion Modifiers

	private:

	#pragma region Utilities

	void throw_length_error(const char *reason) const
	{
		throw std::length_error(reason);
	}

	void check_max_length(size_type n) const
	{
		if (n > this->max_size()) {
			this->throw_length_error("attempt to create ft::vector with a size exceeding max_size()");
		}
	}

	void throw_out_of_range_error(size_type pos) const
	{
		std::stringstream str;
		str << "attempt to access index " << pos << " of ft::vector of size " << _size;
		throw std::out_of_range(str.str());
	}

	pointer allocate(size_type n)
	{
		if (n == 0) {
			return pointer();
		}
		pointer tmp = _allocator.allocate(n);
		return tmp;
	}

	void deallocate(pointer ptr, size_type n)
	{
		if (ptr) {
			_allocator.deallocate(ptr, n);
		}
	}

	void extend()
	{
		if (_capacity == 0) {
			this->reserve(1);
			return;
		}
		if (_capacity * 2 > this->max_size() && _capacity + 1 <= this->max_size()) {
			this->reserve(this->max_size());
			return ;
		} else if (_capacity + 1 > this->max_size()) {
			this->throw_length_error("Cannot extend past max_size()");
		}
		this->reserve(_capacity * 2);
	}

	void destroy_from(iterator it)
	{
		iterator tmp = it;
		while (it != this->end()) {
			_allocator.destroy(it++);
		}
		_size = tmp - _data;
	}

	void unchecked_push_back(const_reference value)
	{
		_allocator.construct(_data + _size++, value);
	}

	#pragma endregion Utilities

};

template< class T, class Alloc >
bool operator==( const ft::vector<T,Alloc>& lhs,
                 const ft::vector<T,Alloc>& rhs )
{
	if (lhs.size() != rhs.size()) {
		return false;
	}
	return ft::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template< class T, class Alloc >
bool operator!=( const ft::vector<T,Alloc>& lhs,
                 const ft::vector<T,Alloc>& rhs )
{
	return !(lhs == rhs);
}

template< class T, class Alloc >
bool operator<( const ft::vector<T,Alloc>& lhs,
                const ft::vector<T,Alloc>& rhs )
{
	return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template< class T, class Alloc >
bool operator<=( const ft::vector<T,Alloc>& lhs,
                 const ft::vector<T,Alloc>& rhs )
{
	return !(lhs > rhs);
}

template< class T, class Alloc >
bool operator>( const ft::vector<T,Alloc>& lhs,
                const ft::vector<T,Alloc>& rhs )
{
	return (rhs < lhs);
}

template< class T, class Alloc >
bool operator>=( const ft::vector<T,Alloc>& lhs,
                 const ft::vector<T,Alloc>& rhs )
{
	return !(lhs < rhs);
}

template< class T, class Alloc >
void swap( ft::vector<T,Alloc>& lhs,
           ft::vector<T,Alloc>& rhs )
{
	lhs.swap(rhs);
}

}
