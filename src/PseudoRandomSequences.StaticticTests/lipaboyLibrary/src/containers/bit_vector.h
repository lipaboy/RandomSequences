#ifndef BIT_CONTAINER_H
#define BIT_CONTAINER_H

#include <vector>
#include <algorithm>
#include <initializer_list>

#include <boost\range\any_range.hpp>

//Unfortunately, I invented the bicycle: std::bitset<size_t N>

namespace LipaboyLib {

	typedef boost::any_range<
		int,
		boost::bidirectional_traversal_tag,
		int,
		std::ptrdiff_t
	> IntAnyRange;

	//TODO: think about generalizing true and false (etc. #define true BIT, false ZERO, but without define)
	typedef bool BitType;

	template <typename T>
	inline T addBitOne(T number, uint32_t position) { return number | (1u << position); }
	template <typename T>
	inline void setBitOne(T& number, uint32_t position) { number |= (1u << position); }
	// [ (sign * max) xor (((sign * max) xor number) | (1 << pos)) ] == [ (bit == 0) ? ~(~number | (1 << pos)) : number | (1 << pos) ]
	template <typename T>
	inline void setBit(T& number, uint32_t position, BitType bit) { 
		number = ((!bit) * (~static_cast<T>(0))) ^ (
			(
				(
					(!bit) * (~static_cast<T>(0))
				) ^ number
			) | (1 << position)
		); 
		//this realization works better on ~10ms with size 2e6
		/*number = (bit == 1) ? (number | (1u << position))
			: (number & ~(1u << position));*/
	}

	//Useless: We already have std::vector<bool>. If you want to create smth new 
	//the add iterator for it (i.e. ForwardIterator).

	template <typename T = uint32_t, typename IndexType = uint32_t>
	class BitVector {
	public:
		BitVector() : container(), _size(0) {}
		BitVector(std::initializer_list<BitType> initList);

		BitType get(IndexType index) const { 
			return container[index / (sizeof(T) * 8)] & (static_cast<T>(1) << (index % (sizeof(T) * 8)));
		}
		//TODO: I feel so upset that I need to write methods such as vector's
		void pushBack(BitType newElem) { ((++_size) % (sizeof(T) * 8) == 1) 
			? container.push_back(static_cast<T>(newElem)) : set(_size - 1, newElem); }
		void set(IndexType index, BitType bit) {
			setBit(container[index / (sizeof(T) * 8)], index % (sizeof(T) * 8), bit);
		}

		IndexType size() const { return _size; }

	private:
		//void setBitOne(IndexType index) { container[index / (sizeof(T) * 8)] |= (static_cast<T>(1) << (index % (sizeof(T) * 8))); }
	private:
		std::vector<T> container;
		IndexType _size;
	};

	template<typename T, typename IndexType>
	inline BitVector<T, IndexType>::BitVector(std::initializer_list<BitType> initList)
		: container(1 + (initList.size() - 1) / (sizeof(T) * 8)), _size(initList.size())
	{
		IndexType i = 0;
		IndexType currInd = -1;
		for (std::initializer_list<BitType>::iterator it = initList.begin(); it != initList.end(); 
				it++, i++) 
		{	
			if (i % (sizeof(T) * 8) == 0)	//for uint32_t it is 32
				currInd++;

			if (*it == true)
				setBitOne(container[currInd], i % (sizeof(T) * 8));
		}
	}

}

#endif	//BIT_CONTAINER_H
