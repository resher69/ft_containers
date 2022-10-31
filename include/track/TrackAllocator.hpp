#pragma once

#include <map>
#include <set>
#include <iostream>

namespace ft {
template<class T>
class TrackAllocator : public std::allocator<T> {
	public:
	typedef size_t size_type;
	typedef T* pointer;
	typedef const T* const_pointer;
	typedef T& reference;
	typedef const T& const_reference;

	template<typename _Tp1>
	struct rebind
	{
		typedef TrackAllocator<_Tp1> other;
	};

	pointer allocate(size_type n, const void *hint = 0)
	{
		pointer alloc = std::allocator<T>::allocate(n, hint);
		_allocations[alloc] = n * sizeof(T);
		return alloc;
	}

	void deallocate(pointer p, size_type n)
	{
		try {
			size_type allocated = _allocations.at(p);
			if (allocated != n * sizeof(T)) {
				std::cerr << "Deallocation error: trying to free buffer of " << n * sizeof(T) << " bytes at address " << p << " but buffer was allocated with size " << allocated << std::endl;
			} else {
				_allocations.erase(p);
			}
		} catch(std::exception& e) {
			std::cerr << "Deallocation error: trying to free pointer " << p << " which was not allocated" << std::endl;
		}
		return std::allocator<T>::deallocate(p, n);
	}

	void construct(pointer p, const_reference value)
	{
		if (_constructed.find(p) != _constructed.end()) {
			std::cerr << "attempt to re-create object at address " << p << std::endl;
		} else {
			std::cerr << "creating object at address " << p << std::endl;
			_constructed.insert(p);
		}
		uintptr_t addr = (uintptr_t)p;
		if (addr % sizeof(T) != 0) {
			std::cerr << "Warning: constructing object at unaligned address" << std::endl;
		}
		bool found = false;
		for (typename std::map<pointer, size_type>::iterator it = _allocations.begin(); it != _allocations.end(); ++it) {
			if (addr >= (uintptr_t)it->first && addr < ((uintptr_t)it->first) + it->second) {
				found = true;
				break ;
			}
		}
		if (!found) {
			std::cerr << "Error: constructing object at address " << p << "outside of any allocated buffer" << std::endl;
		}
		std::allocator<T>::construct(p, value);
	}

	void destroy(pointer p)
	{
		if (_constructed.find(p) != _constructed.end()) {
			std::cerr << "destroying object at address " << p << std::endl;
			_constructed.erase(p);
		} else {
			std::cerr << "trying to destroy object at address " << p << " which was not constructed" << std::endl;
		}
		std::allocator<T>::destroy(p);
	}

	TrackAllocator() throw(): std::allocator<T>(), _allocations(), _constructed()
	{}

	TrackAllocator(const TrackAllocator &a) throw(): std::allocator<T>(a), _allocations(), _constructed() { }

	template <class U>
	TrackAllocator(const TrackAllocator<U> &a) throw(): std::allocator<T>(a), _allocations(), _constructed() { }
    ~TrackAllocator() throw() {
		if (_allocations.size() != 0) {
			std::cerr << "Memory Leak Error: Allocator destroyed with " << _allocations.size() << " buffers not deallocated:\n";
			int i = 1;
			for (typename std::map<pointer, size_type>::iterator it = _allocations.begin(); it != _allocations.end(); ++it) {
				std::cerr << i++ << " (" << it->second << " bytes) ADDRESS: " << it->first << " [" << it->second << "]\n";
			}
			if (_constructed.size() != 0) {
				std::cerr << "The buffers holds " << _constructed.size() << " objects not destroyed\n";
			}
			std::cerr << std::flush;
		}
	}

	private:
		std::map<pointer, size_type> _allocations;
		std::set<pointer> _constructed;
};
}
