/**
 *  Defines template a specialization for containers with random accesss iterators
 *  (deque, array, string) and the priority_queue<T>.  This prevents them from 
 *  being used explicity (priority_queue) or as a backend (make_heap).
**/

#include <deque>
#include <array>
#include <string>
#include <queue>
#include <vector>

#ifndef SNEAKY_PREVENT_STD_CONTAINERS_H
#define SNEAKY_PREVENT_STD_CONTAINERS_H

namespace sneaky_prevent_std_containers {
struct sneaky_t;
struct internal_sneaky_t {
    internal_sneaky_t() { _x = -1; }
    internal_sneaky_t(wchar_t x) { _x = x; }
    bool operator<(const internal_sneaky_t& b) const { return _x < b._x; }
    bool operator>(const internal_sneaky_t& b) const { return _x > b._x; }
    bool operator==(const internal_sneaky_t& b) const { return _x == b._x; }
	operator sneaky_t() const;
	operator const sneaky_t&() const;
    wchar_t _x;
	virtual void make_it_polymorphic() const {}
	virtual ~internal_sneaky_t() {}
};

struct sneaky_t : public internal_sneaky_t {
	sneaky_t() {}
	sneaky_t( const internal_sneaky_t& s ) : internal_sneaky_t(s._x) { }
	sneaky_t( const sneaky_t& s ) : internal_sneaky_t(s.internal_sneaky_t::_x) { }
    sneaky_t(wchar_t x) : internal_sneaky_t(x) {} 
    bool operator<(const sneaky_t& b) const { return internal_sneaky_t::operator<(b); }
    bool operator>(const sneaky_t& b) const { return internal_sneaky_t::operator>(b); }
    bool operator==(const sneaky_t& b) const { return internal_sneaky_t::operator==(b); }
	sneaky_t& operator=( const sneaky_t& r ) { this->_x = r._x; return *this; }
	static void set_test_failure( const char* const n );
	static bool get_test_passed() { return test_passed; }
	private:
	static bool test_passed;
};

internal_sneaky_t::operator sneaky_t() const 
{
	return sneaky_t( _x );
}

internal_sneaky_t::operator const sneaky_t&() const 
{
	return dynamic_cast<const sneaky_t&>(*this);
}

bool sneaky_t::test_passed = true;

void sneaky_t::set_test_failure( const char* const n )
{
	if( !test_passed ) return;
	std::cerr << std::endl;
	std::cerr << "Please don't use std::"<< n << " or it's variants to satisfy this project's sorting requirements." << std::endl;
	std::cerr << "Implement merge sort as specified in the project write-up." << std::endl << std::endl;
	test_passed = false; 
}

// so sneaky_ts can be printed in debug code (without this, students
// simply get build errors)
static inline std::ostream& operator<<( std::ostream& os, const sneaky_t& soo )
{
	const internal_sneaky_t& s(soo);
	return os << "st(" << s._x  << ")" ;
}

} // namespace

#define SNEAKY_T sneaky_prevent_std_containers::sneaky_t
#define INTERNAL_T sneaky_prevent_std_containers::internal_sneaky_t

namespace std {

/***
 * < IMPORTANT >
 ****************
 1. keep in mind we aren't really interested in the correctness of the subclass
 instantiation with respect to ctor arguments, we just want to get past
 compiler errors and flag failure on construction

 2. at least one other very similarly organized prevent-std-containers sneaky header
 file consists in the repo, and updating these when we switch C++ base line spec (11, 14, 17, ..)
 will be a real PiA.  Keep the template specializations in alphabetical order, so that
 when fixes or updates *do* occur, they can be easily merged to and fro different programming
 projects.  compare by *template name*, not header name.  So its:  array, basic_string, deque, ...
 */
	
/***
 * (basic_string)
 * turns out you can't make a basic_string from sneaky_t because sneaky_t
 * fails the compilers' "standard layout test", I suspect because it is polymorphic,
 * for string backing for make_heap has to fail at compile time and provide
 * a much uglier message to students --- then again students that are making strings
 * out of the sorting input vector elements might deserve this, because, well,
 * why else would they be doing this?
 *
 * we keep the below for posterity...
 */
//template<class Traits, class Allocator>
//struct basic_string<SNEAKY_T,Traits,Allocator> : public std::basic_string<INTERNAL_T,Traits,Allocator> {
//	typedef std::basic_string<INTERNAL_T>::size_type size_type;
//	basic_string() : std::basic_string<INTERNAL_T>() { SNEAKY_T::set_test_failure("basic_string"); }
//	explicit basic_string( const Allocator& ) : std::basic_string<INTERNAL_T>() { SNEAKY_T::set_test_failure("basic_string"); }
//	basic_string( size_type Size, const SNEAKY_T&, const Allocator& ) 
//		: std::basic_string<INTERNAL_T>(Size) { SNEAKY_T::set_test_failure("basic_string"); }
//	basic_string( const SNEAKY_T* s, se_type Size, const Allocator& ) 
//		: std::basic_string<INTERNAL_T>(Size) { SNEAKY_T::set_test_failure("basic_string"); }
//	basic_string( const SNEAKY_T* s, const Allocator& ) 
//		: std::basic_string<INTERNAL_T>() { SNEAKY_T::set_test_failure("basic_string"); }
//	basic_string( std::vector<SNEAKY_T>::iterator b, std::vector<SNEAKY_T>::iterator e, const Allocator& ) 
//		: std::basic_string<INTERNAL_T>(b, e) { SNEAKY_T::set_test_failure("basic_string"); }
//};

template<class Allocator>
struct deque<SNEAKY_T,Allocator> : public std::deque<INTERNAL_T> {
	typedef std::deque<INTERNAL_T>::size_type size_type;
	typedef std::deque<INTERNAL_T>::iterator iterator;
	deque() : std::deque<INTERNAL_T>() { SNEAKY_T::set_test_failure("deque"); }
	deque( size_type Size ) : std::deque<INTERNAL_T>(Size) { SNEAKY_T::set_test_failure("deque"); }
	explicit deque( const Allocator& ) : std::deque<INTERNAL_T>() { SNEAKY_T::set_test_failure("deque"); }
	deque( size_type Size, const SNEAKY_T&, const Allocator& ) 
		: std::deque<INTERNAL_T>(Size) { SNEAKY_T::set_test_failure("deque"); }
	deque( std::vector<SNEAKY_T>::iterator b, std::vector<SNEAKY_T>::iterator e ) 
		: std::deque<INTERNAL_T>(b, e) { SNEAKY_T::set_test_failure("deque"); }
//	iterator end()       { SNEAKY_T::set_test_failure("deque"); return this->std::deque<INTERNAL_T>::end(); }
//	iterator end() const { SNEAKY_T::set_test_failure("deque"); return this->std::deque<INTERNAL_T>::end(); }
};

template <class Container, class Compare>
struct priority_queue<SNEAKY_T,Container,Compare>: public std::priority_queue<INTERNAL_T> {
	typedef std::priority_queue<INTERNAL_T>::size_type size_type;
	priority_queue() : std::priority_queue<INTERNAL_T>() { SNEAKY_T::set_test_failure("priority_queue"); }
	explicit priority_queue( const Compare& ) 
		: std::priority_queue<INTERNAL_T>() { SNEAKY_T::set_test_failure("priority_queue"); }
#if __cplusplus <= 201100L
	explicit priority_queue( const Compare& = Compare(), const Container& = Container()) 
		: std::priority_queue<INTERNAL_T>() { SNEAKY_T::set_test_failure("priority_queue"); }
#else
	priority_queue( const Compare&, const Container& ) 
		: std::priority_queue<INTERNAL_T>() { SNEAKY_T::set_test_failure("priority_queue"); }
#endif
	priority_queue( const Compare&, Container&& ) 
		: std::priority_queue<INTERNAL_T>() { SNEAKY_T::set_test_failure("priority_queue"); }
	priority_queue( std::vector<SNEAKY_T>::iterator b, std::vector<SNEAKY_T>::iterator e, const Compare& ) 
		: std::priority_queue<INTERNAL_T>(b, e) { SNEAKY_T::set_test_failure("priority_queue"); }
	priority_queue( std::vector<SNEAKY_T>::iterator b, std::vector<SNEAKY_T>::iterator e, const Compare&, const Container& ) 
		: std::priority_queue<INTERNAL_T>(b, e) { SNEAKY_T::set_test_failure("priority_queue"); }
	priority_queue( std::vector<SNEAKY_T>::iterator b, std::vector<SNEAKY_T>::iterator e, const Compare&, Container& ) 
		: std::priority_queue<INTERNAL_T>(b, e) { SNEAKY_T::set_test_failure("priority_queue"); }
};

} // end namespace std
  
#undef SNEAKY_T
#undef INTERNAL_T
 
 
TEST(ProjectRequirements, PreventStdContainers) {
	using namespace std;
	using namespace sneaky_prevent_std_containers;
	/***
	 * This will not compile if a student is using a std:: container (array, string)
	 * as the backing store for a priority_queue or a string (both of which have random iterators)
	 ****
	 * Students:  if you're getting caught by this test it is most likely an issue
	 * with your sort.h implementation.    Double check the project write-up, we require
	 * that you implement generic k-way merge sort without heap sorting (priority_queue) 
	 * in your sort implementation
	 ***/
    // create a vector with specialized type 
	const size_t Z(512);
	vector<sneaky_t> data(Z);
	sorter(data, 3);
	// if it compiles, the test is passed
    ASSERT_TRUE(true);
}

#endif
