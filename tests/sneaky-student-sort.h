/***
 * sneaky-student-sort.h:  check that students have used their own sort 
 * implementation and not std::sort
 */

#include <iostream>
#include <algorithm>

#ifndef SORTING_STUDENT_SORT_H
#define SORTING_STUDENT_SORT_H

namespace sneaky_student_sort {
struct sneaky_t {
    sneaky_t() { _x = -1; }
    sneaky_t(int x) { _x = x; }
    bool operator<(const sneaky_t& b) const { return _x < b._x; }
    bool operator>(const sneaky_t& b) const { return _x > b._x; }
    bool operator==(const sneaky_t& b) const { return _x == b._x; }
    int _x;
	static void set_sort_failure( const char* const n ); 
	static bool get_sort_passed() { return sort_passed; }
	private:
	static bool sort_passed;
};

bool  sneaky_t::sort_passed = true;

void sneaky_t::set_sort_failure( const char* const n ) 
{ 
	/***
	 * No, this output is not buggy.  Turns out on at least one common CPP library distribution
	 * all four sorting routines lead to standard sort.
	 */
	std::cerr << std::endl;
	std::cerr << "Please don't use std::"<< n << " or it's variants to satisfy this project's sorting requirements." << std::endl;
	std::cerr << "Implement merge sort as specified in the project write-up." << std::endl << std::endl;
	sneaky_t::sort_passed = false; 
}

// so sneaky_ts can be printed in debug code (without this, students
// simply get build errors)
static inline std::ostream& operator<<( std::ostream& os, const sneaky_t& soo )
{
	return os << "sneaky_student_sort::sneaky_t(" << soo._x  << ")" ;
}

} // end namespace 

#define SNEAKY_T sneaky_student_sort::sneaky_t

namespace std { 
/***
 * template specializations
 * we don't need to provide constexpr (return value) specializations for sort because
 * since we invoke it from the SneakyStudentSort TEST we know the arguments are
 * not themselves constexpr.
 */

#define SNEAKYFAIL(n)  SNEAKY_T::set_sort_failure(n)

#define SNORT(ce,funcname) template<> \
	                    ce void funcname( vector<SNEAKY_T>::iterator, \
								vector<SNEAKY_T>::iterator ) \
						{ SNEAKYFAIL( #funcname ); }

#define SNORTExc(funcname) template<class ExcPolicy> \
	                    void funcname( ExcPolicy&&, vector<SNEAKY_T>::iterator, \
								vector<SNEAKY_T>::iterator ) \
						{ SNEAKYFAIL( #funcname ); }

#define SNORTCmp(ce,funcname) template<class Compare> \
	                    ce void funcname( vector<SNEAKY_T>::iterator, \
								vector<SNEAKY_T>::iterator, Compare ) \
						{ SNEAKYFAIL( #funcname ); }

#define SNORTExcCmp(funcname) template<class ExcPolicy, class Compare> \
	                    void funcname( ExcPolicy&&, vector<SNEAKY_T>::iterator, \
								vector<SNEAKY_T>::iterator, Compare ) \
						{ SNEAKYFAIL( #funcname ); }

/*** std::sort, std::partial_sort and partial_copy_sort ***/
#if __cplusplus < 202000L
// until C++ 20
#define SN_CONSTEXPR
#else
// since C++ 20
#define SN_CONSTEXPR // constexpr
#endif

SNORT(SN_CONSTEXPR,sort)
SNORTCmp(SN_CONSTEXPR,sort)

template<>
SN_CONSTEXPR void partial_sort( vector<SNEAKY_T>::iterator,
		vector<SNEAKY_T>::iterator, vector<SNEAKY_T>::iterator )
{ SNEAKYFAIL( "partial_sort" ); }

template<class Compare>
SN_CONSTEXPR void partial_sort( vector<SNEAKY_T>::iterator,
		vector<SNEAKY_T>::iterator, vector<SNEAKY_T>::iterator, Compare )
{ SNEAKYFAIL( "partial_sort" ); }

template< class InputIt >
SN_CONSTEXPR vector<SNEAKY_T>::iterator partial_sort_copy( InputIt, 
		vector<SNEAKY_T>::iterator r, vector<SNEAKY_T>::iterator,
		vector<SNEAKY_T>::iterator, vector<SNEAKY_T>::iterator )
{ SNEAKYFAIL( "partial_copy_sort" ); return r; }

template< class InputIt, class Compare >
SN_CONSTEXPR vector<SNEAKY_T>::iterator partial_sort_copy( InputIt, 
		vector<SNEAKY_T>::iterator r, vector<SNEAKY_T>::iterator,
		vector<SNEAKY_T>::iterator, vector<SNEAKY_T>::iterator, Compare )
{ SNEAKYFAIL( "partial_copy_sort" ); return r; }


#if __cplusplus >= 201700L
// since C++ 17
SNORTExc(sort)
SNORTExcCmp(sort)

template< class ExcPolicy>
SN_CONSTEXPR void partial_sort( ExcPolicy&&, vector<SNEAKY_T>::iterator,
		vector<SNEAKY_T>::iterator, vector<SNEAKY_T>::iterator )
{ SNEAKYFAIL("partial_sort"); }

template< class ExcPolicy, class Compare>
SN_CONSTEXPR void partial_sort( ExcPolicy&&, vector<SNEAKY_T>::iterator,
		vector<SNEAKY_T>::iterator, vector<SNEAKY_T>::iterator, Compare )
{ SNEAKYFAIL("partial_sort"); }


template< class ExcPolicy, class ForwardIt>
vector<SNEAKY_T>::iterator partial_sort_copy( ExcPolicy&& policy, 
		ForwardIt first, ForwardIt last,
		vector<SNEAKY_T>::iterator r, vector<SNEAKY_T>::iterator )
{ SNEAKYFAIL("partial_copy_sort"); return r; }

template< class ExcPolicy, class ForwardIt, class Compare>
vector<SNEAKY_T>::iterator partial_sort_copy( ExcPolicy&& policy, 
		ForwardIt first, ForwardIt last,
		vector<SNEAKY_T>::iterator r, vector<SNEAKY_T>::iterator, Compare )
{ SNEAKYFAIL("partial_copy_sort"); return r; }

#endif

/*** std::stable_sort partitions on C++26 ***/
#if __cplusplus < 202600L
// until C++ 26
#define SN_CONSTEXPR
#else
// since C++ 26
#define SN_CONSTEXPR //constexpr
#endif

SNORT(SN_CONSTEXPR,stable_sort)
SNORTCmp(SN_CONSTEXPR,stable_sort)

#if __cplusplus >= 201700L

// since C++ 17
SNORTExc(stable_sort)
SNORTExcCmp(stable_sort)

#endif
#undef SNORT
#undef SNORTCmp
#undef SNORTExc
#undef SNORTExcCmp
#undef SN_CONSTEXPR
#undef SNEAKYFAIL
#undef SNEAKY_T

} // namespace
 
TEST(ProjectRequirements, SneakyStudentSort) {
	using namespace std;
	using namespace sneaky_student_sort;
	vector<sneaky_t> sorted(LARGE_SET_SIZE);
	// use a large vector because students may have hand coded
	// easier algorithms (bubble, insertion, for small vectors)
    for (int i = 0; i < LARGE_SET_SIZE; i++) {
        sorted.push_back(sneaky_t(i));
    }
	vector<sneaky_t> reversed( sorted );
	reverse( reversed.begin(), reversed.end() );
	// the flag is a static variable that students *might* be able to modify with a lot of trickery
	// so we need to not only be sure it remains true but also that the out of order vector
	// is in fact now ordered.
    sorter(reversed, 3);
    ASSERT_TRUE(sneaky_t::get_sort_passed());
    ASSERT_TRUE(is_sorted(reversed.begin(), reversed.end()));
}

#endif
