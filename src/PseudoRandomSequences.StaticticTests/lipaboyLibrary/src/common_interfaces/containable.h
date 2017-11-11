#ifndef IPLENTY_H
#define IPLENTY_H

//TODO: rename to Containable
template <class T>
class Containable {
public:
	virtual bool contains(const T& element) const = 0;

	bool containsAll(T element) { return contains(element); }
	template <class... Args>
	bool containsAll(T element, Args... args) { return contains(element) && containsAll(args...); }

	bool containsAny(T element) { return contains(element); }
	template <class... Args>
	bool containsAny(T element, Args... args) { return contains(element) || containsAny(args...); }
	
	//TODO: add containsNone because it isn't denying containsAll
};

#endif //IPLENTY_H
