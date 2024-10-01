/*
 * sorter.h
 *
 * Declares a template function:
 *    
 *   sorter() : k-way merge sort.
 * 
 * 
 * You may add other functions if you wish, but this template function
 * is the only one that need to be exposed for the testing code.
 * 
 * Authors: C. Painter-Wakefield & Tolga Can
 */

#ifndef _SORTER_H
#define _SORTER_H

#include <string>
#include <cstddef>
#include <vector>

/***
 * DO NOT put unscoped 'using namespace std;' in header files!
 * Instead put them at the beginning of class or function definitions
 * (as demonstrated below).
 *
 * For more details, see the commentary at the top of
 *   tests/header-sans-using-namespace.h
 * in this project repo.
 */
template <class T>
void sorter(std::vector<T> &vec, int k)  
{
	using namespace std;

	// base case 
	if (vec.size() <= 1)
	{ 
		return;
	}

	// merge sort set up
		// partion vector into k - parts
		// create vector where smaller vectors will be stored 
	vector<vector<T>> vectorOfVectors;
	
		// create for loop that loops through the vector the 'K' amount of times
	for (int i = 0; i < k; i++) 
	{
		// create smaller vectors that will contain elements to be sorted
		vector<T> smallerVectors;
		vectorOfVectors.push_back(smallerVectors);	
	}
		// determing the amount of vectors needed
		// How the lab document states: performing integer division
				// will yield a truncated result leaving out elements 
					// ie n / k == 11 / 3 = 3 leaving out two elements
					// use modulo: 11 % 3 = 2, these are the vectors that
					// need additional elements
	//int remainder = vec.size() % k; 
	for (int i = 0; i < vec.size(); i++) 
	{
		int remainder = i % k; // might need to just change the remainder to i % k in the function call
		vectorOfVectors.at(remainder).push_back(vec.at(i));
	}
		// create for loop mergesorts input vector
		// recurvisley sort 
	for (int i = 0; i < k; i++)
	{
		sorter(vectorOfVectors.at(i),k);
	} 
	// Merge the sorted vectors
		// i iterates over origanl vector (vec)
		// track index for vec where will elements will be merged
	// loop until all elements are merged into vec

	for ( int i = vec.size() - 1; i >= 0; i-- )  // looping thru the back of a vector	
	{	
		T largestValue; // initially set to the maximum possible value of T
		int largestIndex; // index of the vector containing the smallest value
		bool firstVector = true;
		// Iterate over all sub-vectors
		for ( int j = 0; j < k; j++ ) 
		{
			if ( vectorOfVectors.at(j).size() == 0 ) 
			{ 
				continue;
			}
			// Check if the current vector is not empty
			if ( firstVector == true ) 
			{
					largestValue = vectorOfVectors.at(j).back();
					largestIndex = j;
					firstVector = false;
			}
			else 
			{
				if ( vectorOfVectors.at(j).back() > largestValue ) 
					{
						largestValue = vectorOfVectors.at(j).back();
						largestIndex = j;	
					}
			}

		}
		// Place the smallest element into the current position in vec
		vec.at(i) = vectorOfVectors.at(largestIndex).back();
		// Remove the element from the corresponding smaller vector
		vectorOfVectors.at(largestIndex).pop_back();
	}

}


#endif
