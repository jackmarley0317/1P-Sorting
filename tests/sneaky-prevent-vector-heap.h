/***
 * sneaky-prevent-vector-heap: make sure students don't use heap_sort
 * or its ilk
 */

#include <iostream>
#include <vector>
#include <algorithm>


#ifndef SNEAKY_PREVENT_VECTOR_HEAP_H
#define SNEAKY_PREVENT_VECTOR_HEAP_H

namespace sneaky_prevent_vector_heap {
struct sneaky_t {
    sneaky_t() { _x = -1; }
    sneaky_t(int x) { _x = x; }
    bool operator<(const sneaky_t& b) const { return _x < b._x; }
    bool operator>(const sneaky_t& b) const { return _x > b._x; }
    bool operator==(const sneaky_t& b) const { return _x == b._x; }
    int _x;
	static void set_vector_heap_failure(const char* const n);
	static bool get_vector_heap_passed() { return vector_heap_sort_passed; }
	private: 
	static bool vector_heap_sort_passed;
};

bool  sneaky_t::vector_heap_sort_passed = true;

void sneaky_t::set_vector_heap_failure(const char* const n)
{ 
	if( !vector_heap_sort_passed ) return;
	std::cerr << std::endl;
	std::cerr << "Please don't use std::" << n << " or it's variants to satisfy this project's sorting requirements." << std::endl;
	std::cerr << "Implement merge sort as specified in the project write-up." << std::endl << std::endl;
	vector_heap_sort_passed = false; 
}

// so sneaky_ts can be printed in debug code (without this, students
// simply get build errors)
static inline std::ostream& operator<<( std::ostream& os, const sneaky_t& soo )
{
	return os << "st(" << soo._x  << ")" ;
}

} // end namespace

#define SNEAKY_T sneaky_prevent_vector_heap::sneaky_t

#define MAKEHEAP(ce,funcname) template<> \
				ce void funcname( std::vector<SNEAKY_T>::iterator, vector<SNEAKY_T>::iterator ) \
				{ SNEAKY_T::set_vector_heap_failure( #funcname ); }

// https://www.fluentcpp.com/2017/08/15/function-templates-partial-specialization-cpp/
#define MAKEHEAPCmp(ce,funcname) \
	 	template<> \
		ce void funcname( vector<SNEAKY_T>::iterator, vector<SNEAKY_T>::iterator, \
				greater<SNEAKY_T> ) \
		{ SNEAKY_T::set_vector_heap_failure( #funcname ); } \
	 	template<> \
		ce void funcname( vector<SNEAKY_T>::iterator, vector<SNEAKY_T>::iterator, \
				less<SNEAKY_T> ) \
		{ SNEAKY_T::set_vector_heap_failure( #funcname ); } \
	 	template<> \
		ce void funcname( vector<SNEAKY_T>::iterator, vector<SNEAKY_T>::iterator, \
				bool (*)(const SNEAKY_T&, const SNEAKY_T& ) ) \
		{ SNEAKY_T::set_vector_heap_failure( #funcname ); }

#define RANGEHEAPsent(ce,funcname) template< class Sentinel, class Cmp, class Proj> \
	                    ce Sentinel funcname( std::vector<SNEAKY_T>::iterator, Sentinel s, Cmp c = {}, Proj p = {} )\
						{ SNEAKY_T::set_vector_heap_failure( #funcname ); }

#define RANGEHEAPrang(ce,funcname) template< class Cmp, class Proj> \
	                    ce std::ranges::borrowed_iterator_t<std::vector<SNEAKY_T>::range> \
							funcname( std::vector<SNEAKY_T>::range&&, Cmp, Proj )\
							{ SNEAKY_T::set_vector_heap_failure( #funcname ); }



#if __cplusplus < 202000L
// until C++ 20
#define SN_CONSTEXPR
#else
// since C++ 20
#define SN_CONSTEXPR // constexpr
#endif

namespace std {
/***
 * template specializations
 * we don't need to provide constexpr (return value) specializations for sort because
 * since we invoke it from the SneakyStudentSort TEST we know the arguments are
 * not themselves constexpr.
 */

MAKEHEAP(SN_CONSTEXPR,make_heap)
MAKEHEAPCmp(SN_CONSTEXPR,make_heap)
MAKEHEAP(SN_CONSTEXPR,push_heap)
MAKEHEAPCmp(SN_CONSTEXPR,push_heap)

//namespace ranges {
//RANGEHEAPsent(SN_CONSTEXPR,make_heap)
//#if __cplusplus >= 202000L && 0
//RANGEHEAPrang(SN_CONSTEXPR,make_heap)
//#endif
//} // end namespace ranges

} // namespace
 
#undef SN_CONSTEXPR
#undef MAKEHEAP
#undef MAKEHEAPCmp
#undef RANGEHEAPrang
#undef RANGEHEAPsent

TEST(ProjectRequirements, SneakyVectorHeap) {
	using namespace std;
	using namespace sneaky_prevent_vector_heap;

	vector<sneaky_t> sorted(LARGE_SET_SIZE);
	// use a large vector because students may have hand coded
	// easier algorithms (bubble, insertion, for small vectors)
    for (int i = LARGE_SET_SIZE-1; i>=0; --i) {
        sorted.push_back(sneaky_t(i));
    }
	// the flag is a static variable that students *might* be able to modify with a lot of trickery
	// so we need to not only be sure it remains true but also that the out of order vector
	// is in fact now ordered.
    sorter(sorted, 3);
	ASSERT_TRUE(sneaky_t::get_vector_heap_passed());
    ASSERT_TRUE(is_sorted(sorted.begin(), sorted.end()));
}

#endif
