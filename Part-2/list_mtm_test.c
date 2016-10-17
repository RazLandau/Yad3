/*
 * list_mtm_test.c
 *
 *  Created on: 3 במאי 2016
 *      Author: Raz
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "../list_mtm/list_mtm.h"
#include "test_utilities.h"

enum {
	MIN_NUM, NUM1, NUM2, MAX_NUM
};

/**********************************LISTS***************************************/

/**
 * List of ints,
 * exactly as taught in tutorials (and appears in tutorial 5).
 * Everything that will succeed on a more complex ADT will succeed here,
 * and more importantly-
 * everything that will fail on a more complex ADT will fail here.
 * Functions that will be used by the list:
 */

static void freeInt(ListElement element) {
	free(element);
}
static ListElement copyInt(ListElement element) {
	int* newInt = malloc(sizeof(*newInt));
	if (newInt == NULL) {
		return NULL;
	}
	*newInt = *(int*) element;
	return newInt;
}
// Filter function (used by listFilter)
static bool greaterOrEqual(ListElement element, ListFilterKey key) {
	return *(int*) element >= *(int*) key;
}
// Compare function (used by listSort)
static int compareIntsBySize(ListElement num1, ListElement num2) {
	return *(int*) num1 - *(int*) num2;
}

/**********************************TESTS***************************************/

static bool testListCreate() {

	List list;
	assert(list != NULL);

	list = listCreate(copyInt, NULL); // NULL free function
	ASSERT_TEST(list == NULL);
	list = listCreate(NULL, freeInt); // NULL copy function
	ASSERT_TEST(list == NULL);
	list = listCreate(NULL, NULL); // NULL both
	ASSERT_TEST(list == NULL);
	list = listCreate(copyInt, freeInt); // SUCCESS!
	ASSERT_TEST(list != NULL);
	listDestroy(list);

	return true;
}

static bool testListCopy() {
	List list = listCreate(copyInt, freeInt);
	assert(list);

	List list_copy;
	list_copy = listCopy(NULL);
	ASSERT_TEST(!list_copy);
	list_copy = listCopy(list);
	ASSERT_TEST(list_copy);
	ASSERT_TEST(listGetSize(list) == listGetSize(list_copy));

	listDestroy(list);
	listDestroy(list_copy);

	return true;
}

static bool testListGetSize() {

	List list = listCreate(copyInt, freeInt);
	assert(list);

	int list_size;
	list_size = listGetSize(NULL); // NULL list
	ASSERT_TEST(list_size == -1);
	list_size = listGetSize(list); // SUCCESS! empty list
	ASSERT_TEST(list_size == 0);
	int num1 = NUM1;
	listInsertFirst(list, &num1);
//	assert(list_result == LIST_SUCCESS);
	list_size = listGetSize(list); // SUCCESS! non-empty list
	ASSERT_TEST(list_size == 1);

	listDestroy(list);
	return true;
}

static bool testListGetFirst() {

	List list = listCreate(copyInt, freeInt);
	assert(list);

	int* first_element;
	first_element = listGetFirst(NULL); // NULL list
	ASSERT_TEST(!first_element);
	first_element = listGetFirst(list); // empty list
	ASSERT_TEST(!first_element);
	int num1 = NUM1;
	listInsertFirst(list, &num1);
	assert(listGetSize(list) == 1);
	first_element = listGetFirst(list); // SUCCESS!
	ASSERT_TEST(*first_element == NUM1);

	listDestroy(list);
	return true;
}

static bool testListInsertFirst() {
	List list = listCreate(copyInt, freeInt);
	assert(list);

	int num1 = NUM1;
	int num2 = NUM2;
	ListResult list_result;
	list_result = listInsertFirst(NULL, &num1); // NULL list
	ASSERT_TEST(list_result == LIST_NULL_ARGUMENT);
	list_result = listInsertFirst(list, &num1); // SUCCESS! empty list
	ASSERT_TEST(list_result == LIST_SUCCESS);
	ASSERT_TEST(listGetSize(list) == 1);
	ASSERT_TEST(*(int* )listGetFirst(list) == NUM1);
	list_result = listInsertFirst(list, &num2); // SUCCESS! non-empty list
	ASSERT_TEST(list_result == LIST_SUCCESS);
	ASSERT_TEST(listGetSize(list) == 2);
	ASSERT_TEST(*(int* )listGetFirst(list) == NUM2);

	listDestroy(list);
	return true;
}

static bool testListInsertBeforeCurrent() {
	List list = listCreate(copyInt, freeInt);
	assert(list);

	int num1 = NUM1;
	int num2 = NUM2;
	ListResult list_result;
	list_result = listInsertBeforeCurrent(NULL, &num1); // NULL list
	ASSERT_TEST(list_result == LIST_NULL_ARGUMENT);
	list_result = listInsertBeforeCurrent(list, &num1); // empty list
	ASSERT_TEST(list_result == LIST_INVALID_CURRENT);
	listInsertFirst(list, &num1);
	assert(listGetSize(list) == 1);
	listGetFirst(list);
	list_result = listInsertBeforeCurrent(list, &num1); // SUCCESS! iterator points to head
	ASSERT_TEST(list_result == LIST_SUCCESS);
	ASSERT_TEST(listGetSize(list) == 2);
	list_result = listInsertBeforeCurrent(list, &num2); // SUCCESS! regular insert
	ASSERT_TEST(list_result == LIST_SUCCESS);
	ASSERT_TEST(listGetSize(list) == 3);

	listDestroy(list);
	return true;
}

static bool testListClear() {

	List list = listCreate(copyInt, freeInt);
	assert(list);
	assert(listGetSize(list) == 0);

	int num1 = NUM1;
	ListResult list_result;
	list_result = listClear(NULL); // NULL list
	ASSERT_TEST(list_result == LIST_NULL_ARGUMENT);
	ASSERT_TEST(list);
	list_result = listClear(list); // SUCCESS! empty list
	ASSERT_TEST(list_result == LIST_SUCCESS);
	ASSERT_TEST(list);
	ASSERT_TEST(listGetSize(list) == 0);
	listInsertFirst(list, &num1);
	assert(listGetSize(list) == 1);
	list_result = listClear(list);
	ASSERT_TEST(list_result == LIST_SUCCESS); // SUCCESS!
	ASSERT_TEST(listGetSize(list) == 0);
	ASSERT_TEST(list);

	listDestroy(list);
	return true;
}

static bool testListRemoveCurrent() {
	List list = listCreate(copyInt, freeInt);
	assert(list);
	assert(listGetSize(list) == 0);

	int num1 = NUM1;
	int num2 = NUM2;
	ListResult list_result;

	list_result = listRemoveCurrent(NULL); // NULL list
	ASSERT_TEST(list_result == LIST_NULL_ARGUMENT);
	list_result = listRemoveCurrent(list); // empty list
	ASSERT_TEST(list_result == LIST_INVALID_CURRENT);
	listGetFirst(list);
	list_result = listRemoveCurrent(list); // empty list
	ASSERT_TEST(list_result == LIST_INVALID_CURRENT);
	ASSERT_TEST(listGetSize(list) == 0);
	listInsertFirst(list, &num1);
	assert(listGetSize(list) == 1);
	listGetFirst(list);
	list_result = listRemoveCurrent(list);
	ASSERT_TEST(list_result == LIST_SUCCESS); // SUCCESS! iterator points to head
	ASSERT_TEST(listGetSize(list) == 0);
	listInsertFirst(list, &num1);
	listInsertFirst(list, &num2);
	assert(listGetSize(list) == 2);
	listGetFirst(list);
	listGetNext(list);
	list_result = listRemoveCurrent(list);
	ASSERT_TEST(list_result == LIST_SUCCESS); // SUCCESS! regular remove
	ASSERT_TEST(listGetSize(list) == 1);

	listDestroy(list);
	return true;
}

static bool testListFilter() {

	List list = listCreate(copyInt, freeInt);
	assert(list);

	List filtered_list;
	assert(filtered_list);
	int key;

	filtered_list = listFilter(NULL, greaterOrEqual, &key); // NULL list
	ASSERT_TEST(!filtered_list);
	filtered_list = listFilter(list, NULL, &key); // NULL
	ASSERT_TEST(!filtered_list);
	filtered_list = listFilter(list, greaterOrEqual, &key); // empty list
	ASSERT_TEST(filtered_list);
	ASSERT_TEST(listGetSize(filtered_list) == 0);
	listDestroy(filtered_list);
	int num1 = NUM1, num2 = NUM2;
	listInsertLast(list, &num1);
	listInsertLast(list, &num2);
	assert(listGetSize(list) == 2);
	key = MAX_NUM;
	filtered_list = listFilter(list, greaterOrEqual, &key); // SUCCESS!
	ASSERT_TEST(listGetSize(filtered_list) == 0);
	listDestroy(filtered_list);
	key = MAX_NUM - 1;
	filtered_list = listFilter(list, greaterOrEqual, &key); // SUCCESS!
	ASSERT_TEST(listGetSize(filtered_list) == 1);
	listDestroy(filtered_list);
	key = MIN_NUM;
	filtered_list = listFilter(list, greaterOrEqual, &key); // SUCCESS!
	ASSERT_TEST(listGetSize(filtered_list) == 2);
	// check identical order:
	ASSERT_TEST(*(int* )listGetFirst(filtered_list) == NUM1);
	ASSERT_TEST(*(int* )listGetNext(filtered_list) == NUM2);

	listDestroy(list);
	listDestroy(filtered_list);

	return true;
}

static bool testListInsertLast() {

	List list = listCreate(copyInt, freeInt);
	assert(list);
	assert(listGetSize(list) == 0);

	ListResult list_result;
	int num1 = NUM1;

	list_result = listInsertLast(NULL, &num1); // NULL list
	ASSERT_TEST(list_result == LIST_NULL_ARGUMENT);
	ASSERT_TEST(listGetSize(list) == 0);
	list_result = listInsertLast(list, &num1); // SUCCESS! empty list
	ASSERT_TEST(list_result == LIST_SUCCESS);
	ASSERT_TEST(listGetSize(list) == 1);
	listGetFirst(list);
	list_result = listInsertLast(list, &num1); // SUCCESS! iterator point to end
	ASSERT_TEST(list_result == LIST_SUCCESS);
	ASSERT_TEST(listGetSize(list) == 2);
	list_result = listInsertLast(list, &num1); // SUCCESS! regular insert
	ASSERT_TEST(list_result == LIST_SUCCESS);
	ASSERT_TEST(listGetSize(list) == 3);

	listDestroy(list);
	return true;
}

static bool testListGetNext() {

	List list = listCreate(copyInt, freeInt);
	assert(list);
	assert(listGetSize(list) == 0);

	int* next;
	next = listGetNext(NULL); // NULL list
	ASSERT_TEST(!next);
	next = listGetNext(list); // invalid iterator state
	ASSERT_TEST(!next);
	int num1 = NUM1, num2 = NUM2;
	listInsertLast(list, &num1);
	listInsertLast(list, &num2);
	assert(listGetSize(list) == 2);
	listGetFirst(list);
	next = listGetNext(list); // SUCCESS!
	ASSERT_TEST(*next == NUM2);
	next = listGetNext(list); // end of list
	ASSERT_TEST(!next);

	listDestroy(list);
	return true;

}

static bool testListGetCurrent() {

	List list = listCreate(copyInt, freeInt);
	assert(list);
	assert(listGetSize(list) == 0);

	int* current;
	current = listGetCurrent(NULL); // NULL list
	ASSERT_TEST(!current);
	current = listGetCurrent(list); // invalid iterator state
	ASSERT_TEST(!current);
	int num1 = NUM1;
	listInsertLast(list, &num1);
	assert(listGetSize(list) == 1);
	listGetFirst(list);
	current = listGetCurrent(list); // SUCCESS!
	ASSERT_TEST(*current == NUM1);

	listDestroy(list);
	return true;
}

static bool testListInsertAfterCurrent() {

	List list = listCreate(copyInt, freeInt);
	assert(list);
	assert(listGetSize(list) == 0);

	int num1 = NUM1, num2 = NUM2;
	ListResult list_result;
	list_result = listInsertAfterCurrent(NULL, &num1); // NULL list
	ASSERT_TEST(list_result = LIST_NULL_ARGUMENT);
	list_result = listInsertAfterCurrent(list, &num1); // invalid iterator (empty list)
	ASSERT_TEST(list_result = LIST_INVALID_CURRENT);
	ASSERT_TEST(listGetSize(list) == 0);
	listInsertFirst(list, &num1);
	listInsertLast(list, &num2);
	assert(listGetSize(list) == 2);
	listGetFirst(list);
	list_result = listInsertAfterCurrent(list, &num2); // SUCCESS! regular insert
	ASSERT_TEST(list_result == LIST_SUCCESS);
	ASSERT_TEST(listGetSize(list) == 3);
	ASSERT_TEST(*(int* )listGetNext(list) == num2);
	listGetNext(list);
	list_result = listInsertAfterCurrent(list, &num2); // SUCCESS! iterator points to last
	ASSERT_TEST(list_result == LIST_SUCCESS);
	ASSERT_TEST(listGetSize(list) == 4);
	ASSERT_TEST(*(int* )listGetNext(list) == num2);
	listGetNext(list);
	list_result = listInsertAfterCurrent(list, &num1); // invalid iterator (list end)
	ASSERT_TEST(list_result = LIST_INVALID_CURRENT);

	listDestroy(list);
	return true;
}

static bool testListSort() {

	List list = listCreate(copyInt, freeInt);
	assert(list);
	assert(listGetSize(list) == 0);

	ListResult list_result;
	list_result = listSort(NULL, compareIntsBySize); // NULL list
	ASSERT_TEST(list_result == LIST_NULL_ARGUMENT);
	list_result = listSort(list, NULL); // NULL compare function
	ASSERT_TEST(list_result == LIST_NULL_ARGUMENT);
	list_result = listSort(list, compareIntsBySize); // SUCCESS! empty list
	ASSERT_TEST(list_result == LIST_SUCCESS);
	ASSERT_TEST(listGetSize(list) == 0);
	int num1 = NUM1, num2 = NUM2;
	listInsertFirst(list, &num2);
	listInsertLast(list, &num1);
	assert(listGetSize(list) == 2);
	assert(
			*(int* )listGetFirst(list) == NUM2
					&& *(int* )listGetNext(list) == NUM1);
	list_result = listSort(list, compareIntsBySize); // SUCCESS! non-empty list
	ASSERT_TEST(list_result == LIST_SUCCESS);
	ASSERT_TEST(listGetSize(list) == 2);
	ASSERT_TEST(
			*(int* )listGetFirst(list) == NUM1
					&& *(int* )listGetNext(list) == NUM2);

	listDestroy(list);
	return true;
}

int main() {
	RUN_TEST(testListCreate);
	RUN_TEST(testListCopy);
	RUN_TEST(testListGetSize);
	RUN_TEST(testListGetFirst);
	RUN_TEST(testListInsertFirst);
	RUN_TEST(testListInsertBeforeCurrent);
	RUN_TEST(testListClear);
	RUN_TEST(testListRemoveCurrent);
	RUN_TEST(testListFilter);
	RUN_TEST(testListInsertLast);
	RUN_TEST(testListGetNext);
	RUN_TEST(testListGetCurrent);
	RUN_TEST(testListInsertAfterCurrent);
	RUN_TEST(testListSort);
	return 0;
}


