/*
 * SortedSetTest.cpp
 *
 *  Created on: 6 2016
 *      Author: Raz
 */

#include "SortedSet.h"
#include "MtmTst.h"

/********************************INT STRUCT************************************/

struct Int {
	int i;

	Int(int i) :
			i(i) {
	}

	operator int() const {
		return i;
	}
};

class IntCompare {
public:
	bool operator()(const Int& i1, const Int& i2) const {
		return static_cast<int>(i1) < static_cast<int>(i2);
	}
};

/******************************ITERATOR TESTS**********************************/

bool iteratorConstructorTest() {

	SortedSet<Int, IntCompare> sortedSet;

	// NULL iterator
	SortedSet<Int, IntCompare>::iterator null_iterator(&sortedSet);

	// Regular iterator
	sortedSet.insert(1);
	sortedSet.insert(2);
	SortedSet<Int, IntCompare>::iterator iterator = sortedSet.begin();

	// Check data
	ASSERT_EQUALS(*iterator, 1);
	return true;
}

bool iteratorCopyConstructorTest() {

	// Construct a SortedSet of Ints
	SortedSet<Int, IntCompare> sortedSet;
	sortedSet.insert(1);
	sortedSet.insert(2);
	sortedSet.insert(3);

	SortedSet<Int, IntCompare>::iterator iterator = sortedSet.begin();

	// regular iterator copy:
	SortedSet<Int, IntCompare>::iterator copy_iterator(iterator);
	//check if both iterators are equal
	ASSERT_EQUALS(true, copy_iterator == iterator);
	//advance one and check it doesn't equal to the second
	copy_iterator++;
	ASSERT_EQUALS(false, copy_iterator == iterator);

	// iterator that copies itself
	SortedSet<Int, IntCompare>::iterator copy_iterator1(iterator);
	SortedSet<Int, IntCompare>::iterator copy_iterator2(copy_iterator1);
	ASSERT_EQUALS(true, copy_iterator1 == copy_iterator2);

	// null iterator copy:
	SortedSet<Int, IntCompare>::iterator null_iterator(&sortedSet);
	SortedSet<Int, IntCompare>::iterator copy_iterator3(null_iterator);
	//check if both iterators are equal
	ASSERT_EQUALS(true, copy_iterator3 == null_iterator);

	return true;
}

bool iteratorPrefixAndPostfixOperatorTest() {

	// NULL iterator
	SortedSet<Int, IntCompare> sortedSet;
	SortedSet<Int, IntCompare>::iterator null_iterator(&sortedSet);
	null_iterator++; //advance with Prefix
	++null_iterator; //advance with Postfix

	// Regular iterator
	sortedSet.insert(1);
	sortedSet.insert(2);
	sortedSet.insert(3);

	// Create 2 iterators:
	SortedSet<Int, IntCompare>::iterator iterator1 = sortedSet.begin();
	SortedSet<Int, IntCompare>::iterator iterator2 = ++iterator1;
	ASSERT_EQUALS(true, iterator1 == iterator2);
	iterator2 = iterator1++;
	ASSERT_EQUALS(false, iterator2 == iterator1);

	iterator1 = sortedSet.begin();
	iterator2 = sortedSet.begin();

	// Advance the iterator twice with Prefix
	iterator1++;
	iterator1++;

	// Advance the iterator twice with Postfix
	++iterator2;
	++iterator2;

	ASSERT_EQUALS(true, iterator1 == iterator2);

	return true;
}

bool iteratorDereferenceTest() {

	SortedSet<Int, IntCompare> sortedSet;

	//add elements:
	sortedSet.insert(1);
	sortedSet.insert(2);
	sortedSet.insert(3);

	// regular iterator dereference
	SortedSet<Int, IntCompare>::iterator iterator = sortedSet.begin();
	ASSERT_EQUALS(*iterator, 1);
	iterator++; //advance iterator
	ASSERT_EQUALS(*iterator, 2);
	iterator++; //advance iterator
	ASSERT_EQUALS(*iterator, 3);

//	// ASSERT: dereference to end
//	iterator = sortedSet.end();
//	*iterator;

	return true;
}

bool iteratorPlacementTest() {

	SortedSet<Int, IntCompare> sortedSet;
	sortedSet.insert(1);
	sortedSet.insert(2);

	// null iterator
	SortedSet<Int, IntCompare>::iterator null_iterator1 = sortedSet.end();
	SortedSet<Int, IntCompare>::iterator null_iterator2 = null_iterator1;
	ASSERT_EQUALS(null_iterator1, null_iterator2);

	// self-placement
	SortedSet<Int, IntCompare>::iterator iterator = sortedSet.begin();
	iterator = sortedSet.begin();

	iterator++;
	SortedSet<Int, IntCompare>::iterator iterator2 = iterator;
	ASSERT_EQUALS(iterator2, iterator);

	return true;
}

bool iteratorComparingTest() {

	SortedSet<Int, IntCompare> sortedSet;
	sortedSet.insert(1);
	sortedSet.insert(2);

	//compare null iterator
	SortedSet<Int, IntCompare>::iterator null_iterator1 = sortedSet.end();
	SortedSet<Int, IntCompare>::iterator null_iterator2 = sortedSet.end();
	ASSERT_EQUALS(true, null_iterator1 == null_iterator2);
	ASSERT_EQUALS(false, null_iterator1 != null_iterator2);

	// compare null iterator to regular iterator
	SortedSet<Int, IntCompare>::iterator iterator = sortedSet.begin();
	ASSERT_EQUALS(true, null_iterator2 != iterator);
	ASSERT_EQUALS(false, null_iterator2 == iterator);

	// compare copy iterator
	SortedSet<Int, IntCompare>::iterator copy_iterator(iterator);
	ASSERT_EQUALS(true, copy_iterator == iterator);
	ASSERT_EQUALS(false, copy_iterator != iterator);

	//compare different iterators
	SortedSet<Int, IntCompare>::iterator iterator2 = sortedSet.begin();
	iterator++;
	ASSERT_EQUALS(false, iterator2 == iterator);
	ASSERT_EQUALS(true, iterator2 != iterator);

	return true;
}

/******************************ITERATOR TESTS**********************************/

bool sortedSetConstructorAndDestructortest() {

	SortedSet<Int, IntCompare> sortedSet1;

	return true;
}

bool sortedSetCopyConstructorTest() {

	SortedSet<Int, IntCompare> sortedSet1;

	//copy a empty SortedSet
	SortedSet<Int, IntCompare> sortedSet2(sortedSet1);
	ASSERT_EQUALS(sortedSet1.size(), sortedSet2.size());

	sortedSet1.insert(1); // add element

	//copy list with one element
	SortedSet<Int, IntCompare> sortedSet3(sortedSet1);
	// check both are equal
	ASSERT_EQUALS(sortedSet1.size(), sortedSet3.size());
	ASSERT_EQUALS(*sortedSet1.begin(), *sortedSet3.begin());

	// add 2 more elements:
	sortedSet1.insert(2); // sortedSet1: 1,2
	sortedSet1.insert(3); // sortedSet1: 1,2,3

	SortedSet<Int, IntCompare> sortedSet4(sortedSet1);
	// check the elements are equals
	ASSERT_EQUALS(sortedSet1.size(), sortedSet4.size());
	SortedSet<Int, IntCompare>::iterator iterator1 = sortedSet1.begin();
	SortedSet<Int, IntCompare>::iterator iterator2 = sortedSet4.begin();
	ASSERT_EQUALS(*iterator1, *iterator2);
	ASSERT_EQUALS(*(++iterator1), *(++iterator2));
	ASSERT_EQUALS(*(++iterator1), *(++iterator2));

	// add element to the old SortedSet
	sortedSet1.insert(0); // list1: 0,1,2,3   list4:1,2,3
	// check they are not equal
	ASSERT_EQUALS(false, sortedSet1.size() == sortedSet4.size());
	ASSERT_EQUALS(false, *sortedSet1.begin() == *sortedSet4.begin());

	return true;
}

bool sortedSetPlacementTest() {

	SortedSet<Int, IntCompare> sortedSet1;
	sortedSet1.insert(1);
	sortedSet1.insert(2);

//	sortedSet = sortedSet; 	// self-placement

	SortedSet<Int, IntCompare> sortedSet2;
	sortedSet2.insert(3);
	sortedSet2.insert(4);
	sortedSet2.insert(5);

	ASSERT_NOT_EQUALS(*sortedSet1.begin(), *sortedSet2.begin());
	ASSERT_NOT_EQUALS(sortedSet1.size(), sortedSet2.size());

	sortedSet1 = sortedSet2;

	ASSERT_EQUALS(*sortedSet1.begin(), *sortedSet2.begin());
	ASSERT_EQUALS(sortedSet1.size(), sortedSet2.size());

	// Immunity to changes:

	sortedSet2.remove(3);
	ASSERT_NOT_EQUALS(*sortedSet1.begin(), *sortedSet2.begin());
	ASSERT_NOT_EQUALS(sortedSet1.size(), sortedSet2.size());

	return true;
}

bool sortedSetIteratorBeginAndEndTest() {

	SortedSet<Int, IntCompare> sortedSet;

	//a empty SortedSet
	ASSERT_EQUALS(sortedSet.begin(), sortedSet.end());

	//add element
	sortedSet.insert(1);
	// check the iterator to the beginning is different from the
	//iterator to the end
	ASSERT_EQUALS(true, sortedSet.begin() != sortedSet.end());

	// check the data of the iterator to the beginning
	ASSERT_EQUALS(1, *sortedSet.begin());

	return true;
}

bool sortedSetInsertTest() {

	SortedSet<Int, IntCompare> sortedSet1;

	// empty SortedSet
	ASSERT_EQUALS(true, sortedSet1.insert(1));	//sortedSet1: 1
	ASSERT_EQUALS(1, *sortedSet1.begin());
	ASSERT_EQUALS(1, sortedSet1.size());
	ASSERT_EQUALS(true, ++sortedSet1.begin() == sortedSet1.end());

	// add existing data
	ASSERT_EQUALS(false, sortedSet1.insert(1));	//sortedSet1: 1
	ASSERT_EQUALS(1, *sortedSet1.begin());
	ASSERT_EQUALS(1, sortedSet1.size());
	ASSERT_EQUALS(true, ++sortedSet1.begin() == sortedSet1.end());

	// add to tail
	ASSERT_EQUALS(true, sortedSet1.insert(2));	//sortedSet1: 1,2
	ASSERT_EQUALS(2, sortedSet1.size());
	SortedSet<Int, IntCompare>::iterator iterator = ++sortedSet1.begin();
	ASSERT_EQUALS(*iterator, 2);
	iterator++;
	ASSERT_EQUALS(iterator, sortedSet1.end());

	// add to head
	ASSERT_EQUALS(true, sortedSet1.insert(-1));	//sortedSet1: -1,1,2
	ASSERT_EQUALS(-1, *sortedSet1.begin());
	ASSERT_EQUALS(3, sortedSet1.size());
	iterator = ++sortedSet1.begin();
	ASSERT_EQUALS(*iterator, 1);

	// add to middle
	ASSERT_EQUALS(true, sortedSet1.insert(0));	//sortedSet1: -1,0,1,2
	ASSERT_EQUALS(-1, *sortedSet1.begin());
	ASSERT_EQUALS(4, sortedSet1.size());
	iterator = ++sortedSet1.begin();
	ASSERT_EQUALS(*iterator, 0);
	iterator++;
	ASSERT_EQUALS(*iterator, 1);

	// immunity to outside changes
	SortedSet<Int, IntCompare> sortedSet2;
	int a = 1;
	sortedSet2.insert(a);
	ASSERT_EQUALS(true, *sortedSet2.begin() == a);
	a = 2;
	ASSERT_EQUALS(false, *sortedSet2.begin() == a);
	return true;
}

bool sortedSetRemoveTest() {

	SortedSet<Int, IntCompare> sortedSet;

	//empty SortedSet
	ASSERT_EQUALS(false, sortedSet.remove(1));

	//add elements
	sortedSet.insert(1);
	sortedSet.insert(2);
	sortedSet.insert(3); //sortedSet: 1,2,3

	// non-existing data
	ASSERT_EQUALS(false, sortedSet.remove(4)); //sortedSet: 1,2,3
	ASSERT_EQUALS(3, sortedSet.size());
	ASSERT_EQUALS(1, *sortedSet.begin());

	// remove head
	ASSERT_EQUALS(true, sortedSet.remove(1));	//sortedSet: 2,3
	ASSERT_EQUALS(2, sortedSet.size());
	ASSERT_EQUALS(2, *sortedSet.begin());

	// remove tail
	ASSERT_EQUALS(true, sortedSet.remove(3));	//sortedSet: 2
	ASSERT_EQUALS(1, sortedSet.size());
	ASSERT_EQUALS(2, *sortedSet.begin());

	sortedSet.insert(1);
	sortedSet.insert(3);
	sortedSet.insert(4);
	sortedSet.insert(5); //sortedSet: 1,2,3,4,5

	//remove 3 elements
	ASSERT_EQUALS(true, sortedSet.remove(2)); //sortedSet: 1,3,4,5
	ASSERT_EQUALS(true, sortedSet.remove(3)); //sortedSet: 1,4,5
	ASSERT_EQUALS(true, sortedSet.remove(4)); //sortedSet: 1,5
	ASSERT_EQUALS(2, sortedSet.size());
	ASSERT_EQUALS(1, *sortedSet.begin());
	ASSERT_EQUALS(5, *(++sortedSet.begin()));

	return true;
}

bool sortedSetGetSizeTest() {
	SortedSet<Int, IntCompare> sortedSet;

	//empty SortedSet
	ASSERT_EQUALS(0, sortedSet.size());

	//SortedSet with one element
	ASSERT_EQUALS(true, sortedSet.insert(1));	//sortedSet: 1
	ASSERT_EQUALS(1, sortedSet.size());

	//SortedSet with many elements
	ASSERT_EQUALS(true, sortedSet.insert(2));	//sortedSet: 1,2
	ASSERT_EQUALS(true, sortedSet.insert(3));	//sortedSet: 1,2,3
	ASSERT_EQUALS(true, sortedSet.insert(4));	//sortedSet: 1,2,3,4
	ASSERT_EQUALS(true, sortedSet.insert(5));	//sortedSet: 1,2,3,4,5
	ASSERT_EQUALS(true, sortedSet.insert(6));	//sortedSet: 1,2,3,4,5,6
	ASSERT_EQUALS(6, sortedSet.size());

	sortedSet.remove(1);
	sortedSet.remove(3);
	sortedSet.remove(5);
	ASSERT_EQUALS(3, sortedSet.size());

	return true;
}

bool sortedSetFindTest() {

	SortedSet<Int, IntCompare> sortedSet;

	//empty SortedSet
	SortedSet<Int, IntCompare>::iterator iterator = sortedSet.find(1);
	ASSERT_EQUALS(sortedSet.end(), iterator);

	//add elements:
	sortedSet.insert(1);
	sortedSet.insert(2);
	sortedSet.insert(3);
	sortedSet.insert(4); //sortedSet: 1,2,3,4

	//there is an element that is equal to 1- first element
	iterator = sortedSet.find(1);
	ASSERT_EQUALS(1, *iterator);

	//there is an element that is equal to 4 - last element
	iterator = sortedSet.find(4);
	ASSERT_EQUALS(4, *iterator);

	//there is an element that is equal to 3 - middle element
	iterator = sortedSet.find(3);
	ASSERT_EQUALS(3, *iterator);

	//there isn't an element that is equal to 5
	iterator = sortedSet.find(5);
	ASSERT_EQUALS(sortedSet.end(), iterator);

	return true;
}

bool sortedSetAndOperatorTest() {

	SortedSet<Int, IntCompare> sortedSet1;

	SortedSet<Int, IntCompare> sortedSet2;

	//empty sortedSets
	SortedSet<Int, IntCompare> sortedSet3 = sortedSet1 & sortedSet2;
	ASSERT_EQUALS(0, sortedSet3.size());

	//add elements to sortedSet1:
	sortedSet1.insert(1);
	sortedSet1.insert(2);
	sortedSet1.insert(4);
	sortedSet1.insert(6); //sortedSet1: 1,2,4,6

	//empty sortedSet2
	SortedSet<Int, IntCompare> sortedSet4 = sortedSet1 & sortedSet2;
	ASSERT_EQUALS(0, sortedSet4.size());

	//add elements:
	sortedSet2.insert(1);
	sortedSet2.insert(3);
	sortedSet2.insert(5);
	sortedSet2.insert(7); //sortedSet2: 1,3,5,7

	SortedSet<Int, IntCompare> sortedSet5 = sortedSet1 & sortedSet2;

	ASSERT_EQUALS(1, sortedSet5.size());
	ASSERT_EQUALS(1, *sortedSet5.begin());

	return true;
}

bool sortedSetOrOperatorTest() {

	SortedSet<Int, IntCompare> sortedSet1;

	SortedSet<Int, IntCompare> sortedSet2;

	//empty sortedSets
	SortedSet<Int, IntCompare> sortedSet3 = sortedSet1 | sortedSet2;
	ASSERT_EQUALS(0, sortedSet3.size());

	//add elements to sortedSet1:
	sortedSet1.insert(1);
	sortedSet1.insert(2);
	sortedSet1.insert(4);
	sortedSet1.insert(6); //sortedSet1: 1,2,4,6

	//empty sortedSet2
	SortedSet<Int, IntCompare> sortedSet4 = sortedSet1 | sortedSet2;
	ASSERT_EQUALS(4, sortedSet4.size());

	//add elements:
	sortedSet2.insert(1);
	sortedSet2.insert(3);
	sortedSet2.insert(5);
	sortedSet2.insert(7); //sortedSet2: 1,3,5,7

	SortedSet<Int, IntCompare> sortedSet5 = sortedSet1 | sortedSet2;

	ASSERT_EQUALS(7, sortedSet5.size());
	int i = 1;
	SortedSet<Int, IntCompare>::iterator iterator = sortedSet5.begin();
	while (i <= 7) {
		ASSERT_EQUALS(i, *iterator);
		i++;
		iterator++;
	}

	return true;
}

bool sortedSetSubtractionOperatorTest() {

	SortedSet<Int, IntCompare> sortedSet1;

	SortedSet<Int, IntCompare> sortedSet2;

	//empty sortedSets
	SortedSet<Int, IntCompare> sortedSet3 = sortedSet1 - sortedSet2;
	ASSERT_EQUALS(0, sortedSet3.size());

	//add elements to sortedSet1:
	sortedSet1.insert(1);
	sortedSet1.insert(2);
	sortedSet1.insert(4);
	sortedSet1.insert(6); //sortedSet1: 1,2,4,6

	//empty sortedSet2
	SortedSet<Int, IntCompare> sortedSet4 = sortedSet1 - sortedSet2;
	ASSERT_EQUALS(4, sortedSet4.size());

	//add elements:
	sortedSet2.insert(1);
	sortedSet2.insert(3);
	sortedSet2.insert(5);
	sortedSet2.insert(7); //sortedSet2: 1,3,5,7

	SortedSet<Int, IntCompare> sortedSet5 = sortedSet1 - sortedSet2;

	ASSERT_EQUALS(3, sortedSet5.size());
	int i = 2;
	SortedSet<Int, IntCompare>::iterator iterator = sortedSet5.begin();
	while (i <= 6) {
		ASSERT_EQUALS(i, *iterator);
		i += 2;
		iterator++;
	}

	return true;
}

bool sortedSetXorOperatorTest() {

	SortedSet<Int, IntCompare> sortedSet1;

	SortedSet<Int, IntCompare> sortedSet2;

	//empty sortedSets
	SortedSet<Int, IntCompare> sortedSet3 = sortedSet1 ^ sortedSet2;
	ASSERT_EQUALS(0, sortedSet3.size());

	//add elements to sortedSet1:
	sortedSet1.insert(1);
	sortedSet1.insert(2);
	sortedSet1.insert(4);
	sortedSet1.insert(6); //sortedSet1: 1,2,4,6

	//empty sortedSet2
	SortedSet<Int, IntCompare> sortedSet4 = sortedSet1 ^ sortedSet2;
	ASSERT_EQUALS(4, sortedSet4.size());

	//add elements:
	sortedSet2.insert(1);
	sortedSet2.insert(3);
	sortedSet2.insert(5);
	sortedSet2.insert(7); //sortedSet2: 1,3,5,7

	SortedSet<Int, IntCompare> sortedSet5 = sortedSet1 ^ sortedSet2;

	ASSERT_EQUALS(6, sortedSet5.size());
	ASSERT_EQUALS(sortedSet5.end(), sortedSet5.find(1));
	int i = 2;
	SortedSet<Int, IntCompare>::iterator iterator = sortedSet5.begin();
	while (i <= 7) {
		ASSERT_EQUALS(i, *iterator);
		i++;
		iterator++;
	}

	return true;
}

bool sortedSetTestExample() {

	const int numElements = 10000;

	ASSERT_NO_THROW((SortedSet<Int, IntCompare>()));
	SortedSet<Int, IntCompare> s;
	for (int i = numElements; i > 0; --i) {
		ASSERT_EQUALS(numElements - i, s.size());
		ASSERT_EQUALS(true, s.insert(i));
	}
	for (int i = 2; i <= numElements; i += 2) {
		ASSERT_EQUALS(true, s.remove(i));
		ASSERT_EQUALS(numElements - i / 2, s.size());
	}

	int i = 1;
	for (SortedSet<Int, IntCompare>::iterator it = s.begin(); it != s.end();
			++it) {
		ASSERT_EQUALS(i, *it);
		i += 2;
	}

	return true;
}

bool sortedSetTest() {

	RUN_TEST(iteratorConstructorTest);
	RUN_TEST(iteratorCopyConstructorTest);
	RUN_TEST(iteratorPrefixAndPostfixOperatorTest);
	RUN_TEST(iteratorDereferenceTest);
	RUN_TEST(iteratorPlacementTest);
	RUN_TEST(iteratorComparingTest);

	RUN_TEST(sortedSetConstructorAndDestructortest);
	RUN_TEST(sortedSetCopyConstructorTest);
	RUN_TEST(sortedSetPlacementTest);
	RUN_TEST(sortedSetIteratorBeginAndEndTest);
	RUN_TEST(sortedSetInsertTest);
	RUN_TEST(sortedSetRemoveTest);
	RUN_TEST(sortedSetGetSizeTest);
	RUN_TEST(sortedSetFindTest);
	RUN_TEST(sortedSetAndOperatorTest);
	RUN_TEST(sortedSetOrOperatorTest);
	RUN_TEST(sortedSetSubtractionOperatorTest);
	RUN_TEST(sortedSetXorOperatorTest);
	RUN_TEST(sortedSetTestExample);

	return true;
}
