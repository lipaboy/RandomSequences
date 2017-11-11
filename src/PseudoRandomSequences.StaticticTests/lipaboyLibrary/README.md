# lipaboyLibrary
some useful functions for work (C++)

1) Maybe replace RightComparison template parameter on boost::function (that erases function type)? No. Your function type is only and one: bool rightComparison(T, T).
Add this type to template parameter as pointer on function

4)  Add new two parameters to Interval (RightComparison and LeftComparison). They intuitevly clear and undestandly but not obviously. May be need some instructions or help for it.

6) Testing

9) (2) Furthermore, I would like to add union interval and segment like the entire object (e.g. [-2, 3) U (5, +inf) ).
	- May be work with "<", "<=" symbols (from C++ STL). like container.add(5, "<=");
	- (1) Answer: I think you need to create wrapper around Set<T> because it can not contain of function (contain), or if so then wrapper will inherit from Interface IPlenty
	- What do I need the interface IPlenty for? May be combine such actions like do smth over collection by pattern like plenty language (set theory language). Like do smth over the whole collection accept this set (plenty).
