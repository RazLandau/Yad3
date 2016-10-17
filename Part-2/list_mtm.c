/*
 * list_mtm.c
 *
 *  Created on: 25 באפר 2016
 *      Author: Raz
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include "../list_mtm/list_mtm.h"

/**********************************NODE****************************************/

/**
 * ADT which functions as the limb of List
 */
typedef struct node {
	ListElement data;
	struct node* next;
}*Node;

#define DATA node->data
#define NEXT node->next
#define COPY_DATA node_copy->data
#define NEW_DATA new_node->data
#define NEW_NEXT new_node->next

/**
 * Creates an empty Node.
 * @return
 * A new allocated Node.
 * NULL in case of a memory allocation failure.
 */
static Node nodeCreate() {
	Node new_node = malloc(sizeof(*new_node));
	if (!new_node) {
		return NULL;
	}
	NEW_DATA = NULL;
	NEW_NEXT = NULL;
	return new_node;
}

/**
 * Frees all memory allocated for the given Node using the freeFunction.
 *
 * @param node - the node to free
 * @param freeFunction - the function used to free the Node
 *
 * This function can receive NULL.
 */
static void nodeDestroy(Node node, FreeListElement freeFunction) {
	while (node) {
		freeFunction(DATA);
		Node to_delete = node;
		node = NEXT;
		free(to_delete);
	}
}

/**
 * Creates a newly allocated copy of a given Node.
 *
 *@param node  - the node copied
 *@param copyFunction - the function used for copying the node's data
 *@param freeFunction - the function need for creating a new Node.
 *
 * @return
 * A newly allocated copy of the original apartment.
 * NULL if the Node is NULL or in any case of memory allocation failure.
 */
static Node nodeCopy(Node node, CopyListElement copyFunction,
		FreeListElement freeFunction) {
	if (!node) {
		return NULL;
	}
	Node node_copy = nodeCreate();
	if (!node_copy) {
		return NULL;
	}
	if (!(DATA)) {
		return node_copy;
	}
	COPY_DATA = copyFunction(DATA);
	if (!COPY_DATA) {
		nodeCreate(node_copy, freeFunction);
		return NULL;
	}
	return node_copy;
}

/**
 * Sets the data of given node using copy function supplied if successful.
 *
 * @param - the node changed
 * @param data - the data to be set.
 * @param copyFunctio  - the function used for copying the data.
 */
static void nodeSetData(Node node, ListElement data,
		CopyListElement copyFunction) {
	if (!node || !data || !copyFunction) {
		return;
	}
	DATA = copyFunction(data);
}

/*********************************LIST*****************************************/

/**
 * The List structure.
 */
struct List_t {
	Node head;
	Node iterator;
	CopyListElement copy;
	FreeListElement free;
};

#define HEAD list->head
#define ITERATOR list->iterator
#define COPY_FUNCTION list->copy
#define FREE_FUNCTION list->free
#define COPY_HEAD list_copy->head
#define COPY_ITERATOR list_copy->iterator
#define NEW_HEAD new_list->head
#define NEW_ITERATOR new_list->iterator

/****************************STATIC FUNCTIONS**********************************/

/**
 * @return
 * -1 if a NULL pointer was sent.
 * Otherwise the number of element the iterator points.
 *
 * This function asserts (list != NULL).
 *
 */
int getIterator(List list) {
	assert(list);
	int list_size = listGetSize(list), pre_final_iterator = 0;
	if (list_size == -1) {
		return -1;
	}
	if (list_size == 0) {
		return 0;
	}
	Node node = ITERATOR;
	while (node) {
		pre_final_iterator++;
		node = NEXT;
	}
	pre_final_iterator--;
	if (pre_final_iterator == 0 && list_size == 1) {
		return 0;
	}
	return list_size - pre_final_iterator - 1;
}

/**
 * Makes list_copy's iterator points to what list's iterator points.
 * This function asserts (list != NULL && list_copy != NULL).
 */
static void copyIterator(List list, List list_copy) {
	assert(list && list_copy);
	int list_iterator = getIterator(list);
	Node node = COPY_HEAD;
	for (int i = 0; i < list_iterator; i++) {
		node = NEXT;
	}
	COPY_ITERATOR = node;
}

/**
 * Insert's newly allocated copies of a list's nodes to list_copy
 *
 * @param list - the list copied from
 * @param list_copy - the list copied to
 *
 * This function asserts (list != NULL && list_copy != NULL).
 */
static bool copyNodes(List list, List list_copy) {
	assert(list && list_copy);
	Node node = HEAD;
	Node new_node = COPY_HEAD;
	nodeSetData(new_node, HEAD->data, COPY_FUNCTION);
	node = NEXT;
	int list_size = listGetSize(list) - 1;
	for (int i = 0; i < list_size; i++) {
		NEW_NEXT = nodeCopy(node, COPY_FUNCTION, FREE_FUNCTION);
		if (!NEW_NEXT) {
			listDestroy(list_copy);
			return false;
		}
		new_node = NEW_NEXT;
		node = NEXT;
	}
	return true;
}

/**
 * Adds a new head node to an empty list.
 *
 * @param list The list for which to add an element in its start
 * @param element The element to insert. A copy of the element will be
 * inserted as supplied by the copying function which is stored in the list
 * @return
 * LIST_OUT_OF_MEMORY if an allocation failed (Meaning the function for copying
 * an element failed)
 * LIST_SUCCESS the element has been inserted successfully
 *
 * This function asserts (list != NULL && element != NULL).
 */
static ListResult insertHead(List list, ListElement element) {
	assert(list && element);
	nodeSetData(HEAD, element, COPY_FUNCTION);
	if (!HEAD->data) {
		return LIST_OUT_OF_MEMORY;
	}
	return LIST_SUCCESS;
}

/**
 * Adds a new head element to a non-empty list.
 * @param list The list for which to add an element in its start
 * @param element The element to insert. A copy of the element will be
 * inserted as supplied by the copying function which is stored in the list
 * @return
 * LIST_OUT_OF_MEMORY if an allocation failed (Meaning the function for copying
 * an element failed)
 * LIST_SUCCESS the element has been inserted successfully
 *
 * This function asserts (list != NULL && element != NULL).
 */
static ListResult insertBeforeHead(List list, ListElement element) {
	assert(list && element);
	Node new_head = nodeCreate(COPY_FUNCTION, FREE_FUNCTION);
	if (!new_head) {
		return LIST_OUT_OF_MEMORY;
	}
	nodeSetData(new_head, element, COPY_FUNCTION);
	if (!new_head->data) {
		nodeDestroy(new_head, FREE_FUNCTION);
		return LIST_OUT_OF_MEMORY;
	}
	new_head->next = HEAD;
	HEAD = new_head;
	return LIST_SUCCESS;
}

/**
 * Removes the head of the list using the stored freeing function.
 *
 * @param list The list for which the current element will be removed
 *
 * This function asserts (list != NULL).
 */
static void removeHead(List list) {
	if (listGetSize(list) == 1) {
		FREE_FUNCTION(HEAD->data);
		HEAD->data = NULL;
		ITERATOR = NULL;
		return;
	}
	Node new_head = HEAD->next;
	FREE_FUNCTION(HEAD->data);
	free(HEAD);
	HEAD = new_head;
	ITERATOR = NULL;
}

/*****************************************************************************/

List listCreate(CopyListElement copyElement, FreeListElement freeElement) {
	if (!copyElement || !freeElement) {
		return NULL;
	}
	List list = malloc(sizeof(*list));
	if (!list) {
		return NULL;
	}
	HEAD = nodeCreate();
	if (!HEAD) {
		listDestroy(list);
		return NULL;
	}
	COPY_FUNCTION = copyElement;
	FREE_FUNCTION = freeElement;
	ITERATOR = NULL;
	return list;
}

void listDestroy(List list) {
	if (list) {
		nodeDestroy(HEAD, FREE_FUNCTION);
		ITERATOR = NULL;
		FREE_FUNCTION = NULL;
		COPY_FUNCTION = NULL;
	}
	free(list);
}

List listCopy(List list) {
	if (!list) {
		return NULL;
	}
	List list_copy = listCreate(COPY_FUNCTION, FREE_FUNCTION);
	if (!list_copy) {
		return NULL;
	}
	if (!HEAD || !HEAD->data) {
		return list_copy;

	}
	if (!copyNodes(list, list_copy)) {
		return NULL;
	}
	if (ITERATOR) {
		copyIterator(list, list_copy);
	}
	return list_copy;
}

int listGetSize(List list) {
	if (!list) {
		return -1;
	}
	if (!HEAD || !HEAD->data) {
		return 0;
	}
	Node node = HEAD;
	int count = 0;
	while (node) {
		count++;
		node = NEXT;
	}
	return count;
}

ListElement listGetFirst(List list) {
	if (!list || !HEAD || !HEAD->data) {
		return NULL;
	}
	ITERATOR = HEAD;
	return (HEAD->data);
}

ListResult listInsertFirst(List list, ListElement element) {
	assert(element);
	if (!list || !HEAD) {
		return LIST_NULL_ARGUMENT;
	}
	if (!HEAD->data) {
		return insertHead(list, element);
	}
	Node head = HEAD;
	Node new_head = nodeCreate();
	if (!new_head) {
		list->head = head;
		head = NULL;
		new_head = NULL;
		return LIST_OUT_OF_MEMORY;
	}
	nodeSetData(new_head, element, COPY_FUNCTION);
	if (!new_head->data) {
		nodeDestroy(new_head, FREE_FUNCTION);
		HEAD = head;
		new_head = NULL;
		return LIST_OUT_OF_MEMORY;
	}
	HEAD = new_head;
	HEAD->next = head;
	return LIST_SUCCESS;
}

ListResult listInsertBeforeCurrent(List list, ListElement element) {
	assert(element);
	if (!list || !HEAD) {
		return LIST_NULL_ARGUMENT;
	}
	if (!ITERATOR || !ITERATOR->data) {
		return LIST_INVALID_CURRENT;
	}
	int list_iterator = getIterator(list);
	if (list_iterator >= listGetSize(list)) {
		return LIST_INVALID_CURRENT;
	}
	if (list_iterator == 0) {
		return insertBeforeHead(list, element);
	}
	Node list_end = ITERATOR;
	Node node = HEAD;
	for (int i = 0; i < list_iterator - 1; i++) {
		node = NEXT;
	}
	NEXT = nodeCreate();
	if (!NEXT) {
		NEXT = list_end;
		return LIST_OUT_OF_MEMORY;
	}
	nodeSetData(NEXT, element, COPY_FUNCTION);
	if (!NEXT->data) {
		nodeDestroy(NEXT, FREE_FUNCTION);
		NEXT = list_end;
		return LIST_OUT_OF_MEMORY;
	}
	NEXT->next = list_end;
	return LIST_SUCCESS;
}

ListResult listInsertLast(List list, ListElement element) {
	assert(element);
	if (!list || !HEAD) {
		return LIST_NULL_ARGUMENT;
	}
	if (!HEAD->data) {
		return insertHead(list, element);
	}
	Node node = HEAD;
	while (NEXT) {
		node = NEXT;
	}
	Node last_element = nodeCreate();
	if (!last_element) {
		return LIST_OUT_OF_MEMORY;
	}
	nodeSetData(last_element, element, COPY_FUNCTION);
	if (!last_element->data) {
		nodeDestroy(last_element, FREE_FUNCTION);
		return LIST_OUT_OF_MEMORY;
	}
	NEXT = last_element;
	return LIST_SUCCESS;
}

ListResult listClear(List list) {
	if (!list) {
		return LIST_NULL_ARGUMENT;
	}
	if (!HEAD || !HEAD->data) {
		return LIST_SUCCESS;
	}
	nodeDestroy(HEAD->next, FREE_FUNCTION);
	FREE_FUNCTION(HEAD->data);
	HEAD->data = NULL;
	HEAD->next = NULL;

	ITERATOR = NULL;
	return LIST_SUCCESS;
}

ListResult listRemoveCurrent(List list) {
	if (!list) {
		return LIST_NULL_ARGUMENT;
	}
	if (!ITERATOR) {
		return LIST_INVALID_CURRENT;
	}
	if (listGetSize(list) == 0) {
		return LIST_SUCCESS;
	}
	int list_iterator = getIterator(list);
	if (list_iterator >= listGetSize(list)) {
		return LIST_INVALID_CURRENT;
	}
	if (list_iterator == 0) {
		removeHead(list);
		return LIST_SUCCESS;
	}
	Node pre_iterator = HEAD;
	for (int i = 0; i < list_iterator - 1; i++) {
		pre_iterator = pre_iterator->next;
	}
	Node end = ITERATOR->next;
	FREE_FUNCTION(ITERATOR->data);
	ITERATOR->next = NULL;
	free(ITERATOR);
	pre_iterator->next = end;
	ITERATOR = NULL;
	return LIST_SUCCESS;
}

ListElement listGetNext(List list) {
	if (!list || !HEAD || !HEAD->data || !ITERATOR) {
		return NULL;
	}
	if (!ITERATOR->next) {
		ITERATOR = NULL;
		return NULL;
	}
	ITERATOR = ITERATOR->next;
	return ITERATOR->data;
}

List listFilter(List list, FilterListElement filterElement, ListFilterKey key) {
	assert(key);
	if (!list || !filterElement) {
		return NULL;
	}
	List new_list = listCreate(COPY_FUNCTION, FREE_FUNCTION);
	if (!new_list) {
		return NULL;
	}
	int list_size = listGetSize(list);
	Node node = HEAD;
	for (int i = 0; i < list_size; i++) {
		if (!filterElement(DATA, key)) {
			node = NEXT;
			continue;
		}
		if (listInsertLast(new_list, DATA) == LIST_OUT_OF_MEMORY) {
			return NULL;
		}
		node = NEXT;
	}
	NEW_ITERATOR = NEW_HEAD;
	return new_list;
}

ListElement listGetCurrent(List list) {
	if (!list || !HEAD || !HEAD->data || !ITERATOR) {
		return NULL;
	}
	if (!ITERATOR->data) {
		ITERATOR = NULL;
		return NULL;
	}
	return (ITERATOR->data);
}

ListResult listInsertAfterCurrent(List list, ListElement element) {
	assert(element);
	if (!list || !HEAD) {
		return LIST_NULL_ARGUMENT;
	}
	if (!ITERATOR || !ITERATOR->data) {
		return LIST_INVALID_CURRENT;
	}
	int list_iterator = getIterator(list);
	if (list_iterator >= listGetSize(list)) {
		return LIST_INVALID_CURRENT;
	}
	Node node = nodeCreate();
	if (!node) {
		return LIST_OUT_OF_MEMORY;
	}
	nodeSetData(node, element, COPY_FUNCTION);
	if (!DATA) {
		nodeDestroy(node, FREE_FUNCTION);
		return LIST_OUT_OF_MEMORY;
	}
	Node post_iterator = ITERATOR->next;
	ITERATOR->next = node;
	NEXT = post_iterator;
	return LIST_SUCCESS;
}

ListResult listSort(List list, CompareListElements compareElement) {
	if (!list || !compareElement) {
		return LIST_NULL_ARGUMENT;
	}
	Node temp_node = nodeCreate();
	if (!temp_node) {
		return LIST_OUT_OF_MEMORY;
	}
	for (Node ptr1 = HEAD; ptr1; ptr1 = ptr1->next) {
		for (Node ptr2 = ptr1->next; ptr2; ptr2 = ptr2->next) {
			if (compareElement(ptr1->data, ptr2->data) > 0) {
				FREE_FUNCTION(temp_node->data);
				nodeSetData(temp_node, ptr1->data, COPY_FUNCTION);
				FREE_FUNCTION(ptr1->data);
				nodeSetData(ptr1, ptr2->data, COPY_FUNCTION);
				FREE_FUNCTION(ptr2->data);
				nodeSetData(ptr2, temp_node->data, COPY_FUNCTION);
			}
		}
	}
	nodeDestroy(temp_node, FREE_FUNCTION);
	return LIST_SUCCESS;
}

