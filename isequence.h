#ifndef ISEQUENCE_H
#define ISEQUENCE_H

#include <algorithm>
#include <boost\range\any_range.hpp>

//TODO: make for different types (not only for int). But I think it is too redundancy.

//template <typename T>
typedef boost::any_range<
		int,
		boost::bidirectional_traversal_tag,
		int,
		std::ptrdiff_t
	> IntAnyRange;

class ISequence {
public:
	
	virtual IntAnyRange getRange() = 0;

	virtual const IntAnyRange getConstRange() const = 0;

	//Enough?? Or may be need to add constant version.
};

class VectorSequence : public ISequence {
public:
	VectorSequence(std::initializer_list<int> & list) : vect(list) {}
	VectorSequence(const std::vector<int>& _vect) : vect(_vect) {}

	virtual IntAnyRange getRange() { return vect; }

	virtual const IntAnyRange getConstRange() const { return vect; }

private:
	std::vector<int> vect;

};

#endif //ISEQUENCE_H
