#ifndef ISEQUENCE_H
#define ISEQUENCE_H

#include <algorithm>
#include <boost\range\any_range.hpp>

//TODO: 

typedef boost::any_range<
		int,
		boost::bidirectional_traversal_tag,
		int,
		std::ptrdiff_t
	> IntAnyRange;

class ISequence {
	//boost::any
	//virtual 
};

#endif //ISEQUENCE_H
