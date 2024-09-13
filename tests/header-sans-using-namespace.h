#ifndef HEADER_SANS_USING_NAMESPACE_H
#define  HEADER_SANS_USING_NAMESPACE_H
/***
 * header-sans-using-namespace.h:  make sure student's haven't used
 *   using namespace std;
 * in their project .h files (in global scope).
 * 
 * 220 STAFF: incorporate this into a <Project>'s testing code with
 * a symlink alongside the project test.cpp file:
 *  $ ln -sf ../../../lib/tests/header-sans-using-namespace.h ./
 * and then putting
 *  #include "header-sans-using-namespace.h"
 * immediately _after_ any student project .h files are included in module.
 *
 * STUDENTS:  having 
 *   using namespace X;  // X=std, X=your_own_namespace, ...
 * in the "global scope" of C++ header files is a no, no.  And is
 * counter to best practices.
 *
 * While it won't always break your compile right off the bat, it
 * makes code non-portable and fragile (to see why, look how simple
 * it was to write a test case that that snaps poorly written 
 * code like a dried twig...).
 *
 * Some guidelines:
 *  - You can always put  using namespace  in C++ source files
 *  - You can put  using namespace  _in_side_of_other_namespaces_
 *    within C++ header files.
 *  - You can always put  using namespace  inside of function
 *    _definitions_, even inside of header files (think template<>
 *    parameterized functions or inline member functions).
 */

namespace header_sans_namespace {
	// declare some like named symbols we know exist in the stl
	
	typedef int iostream;
	typedef int vector;
	typedef int basic_string;
	typedef int pair;
}

TEST(ProjectRequirements,HeaderSansNamespace) {
	using namespace header_sans_namespace;
	// these work because their fully qualified names are used
	header_sans_namespace::iostream q = 0;
	header_sans_namespace::vector r = 1;
	header_sans_namespace::basic_string s = 2;
	header_sans_namespace::pair t = 3;

	// one of these is likely to fail if  using namespace std  has been
	// used in a student's header file.  why?  because the compiler
	// can't tell if we intend to use  header_sans_namespace::vector or std::vector.
	// 
	// you can detect these issues by spotting the oft used term "ambiguous" in the 
	// compiler error messages.
	//
	// (and no, of course, the solution is _not_ to remove the  using namespace
	// in this function, it is to fix your header file!)  See the STUDENTS
	// commentary at the top of this file for details
	iostream a = 4;	
	vector b = 5;	
	basic_string c = 5;	
	pair d = 6;
	
	/** avoid silly unused variable warnings */
	a = b = c = d = q = r = s = t;
	t = a;
	ASSERT_TRUE(true);
}

#endif
