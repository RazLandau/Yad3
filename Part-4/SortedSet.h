#include <iostream>
#include <assert.h>

#ifndef SORTEDSET_H_
#define SORTEDSET_H_

template<class D, class Compare = std::less<D> >
class SortedSet;

/************************************NODE**************************************/
/* An aiding  class for class list (all the function and fields are private) */

/*
 * An aiding class that will function as the limb of the SortedSet.
 *
 * @field data - The data the Node holds
 * @field next - Next Node in SortedSet
 * @field previous - Previous Node in SortedSet
 */
template<class U>
class Node {

	U* data;
	Node<U>* next;
	Node<U>* previous;

	/**
	 * Node C'tor.
	 *
	 * @param data - The data the Node holds.
	 *
	 * @return
	 * 	A new allocated Node with a copy of given data.
	 * 	Next and Previous fields are set to NULL.
	 **/
	explicit Node(const U data) :
			data(new U(data)), next(NULL), previous(NULL) {
	}

	/**
	 * Node Copy-C'tor.
	 *
	 * @param node - The copied node.
	 *
	 * @return
	 * 	A new allocated Node with a copy of given Node's data.
	 * 	Next and Previous fields are set to NULL.
	 **/
	Node(const Node& node) :
			data(new U(*(node.data))), next(NULL), previous(NULL) {
	}

	/**
	 * Node D'tor.
	 *
	 * Frees all memory allocated for the given Node.
	 **/
	~Node() {
		delete data;
	}

	template<class D, class Compare> friend class SortedSet;
};

/********************************SORTED SET************************************/

/**
 * SortedSet Class.
 *
 * @field head - The first node of the SortedSet
 * @field tail - The last node of the SortedSet
 * @field Size - The size of the SortedSet
 * @field compare - Compare class for 2 elements in the SortedSet: Returns true
 * if element a < element b, false otherwise.
 **/
template<class D, class Compare >
class SortedSet {
	Node<D>* head; //
	Node<D>* tail; //
	int Size;
	Compare compare;

public:

	/**
	 * Inner class for iterating over the SortedSet.
	 *
	 * @field element - The Node in the SortedSet the iterator points to.
	 **/
	class iterator {

		Node<D>* element;

	public:

		/**
		 * Iterator C'tor.
		 *
		 * @param element - The Node to be pointed.
		 *
		 * @return
		 * 	A new allocated Iterator that point to element.
		 * 	Element field is set to NULL as default.
		 **/
		explicit iterator(const SortedSet<D, Compare>* sortedSet,
				Node<D>* element =
				NULL);

		/**
		 * Iterator Copy-C'tor.
		 *
		 * @param iterator - The copied iterator.
		 *
		 * @return
		 * 	A new allocated copy of given Iterator.
		 **/
		iterator(const iterator& iterator);

		/**
		 * Iterator D'tor.
		 *
		 * Frees all memory allocated for the given Iterator.
		 **/
		~iterator() {
		}

		/**
		 * Iterator Prefix operator (++Iterator).
		 *
		 * @return
		 * The iterator after advancement.
		 **/
		iterator& operator++();

		/**
		 * Iterator Postfix operator (Iterator++).
		 *
		 * @param int - Dummy argument.
		 *
		 * @return
		 * A copy of the iterator before advancement.
		 **/
		iterator operator++(int);

		/**
		 * Iterator Dereference opterator.
		 *
		 * @return
		 * The data of the Node the iterator points at, unchangeable.
		 *
		 **/
		const D& operator*() const;

		/**
		 * Iterator Placement operator.
		 *
		 * @param iterator- The iterator to place.
		 *
		 * @return
		 * The iterator after placement.
		 **/
		iterator& operator=(const iterator& iterator);

		/**
		 * Iterator Compare operator.
		 *
		 * @param iterator - The compared iterator.
		 *
		 * @return
		 * True - If iterators points to the same Node.
		 * False - Otherwise.
		 **/
		bool operator==(const iterator& iterator) const;

		/**
		 * Iterator Incompare operator.
		 *
		 * @param iterator - The compared iterator.
		 *
		 * @return
		 * Flase - If iterators points to the same Node.
		 * True - Otherwise.
		 **/
		bool operator!=(const iterator& iterator) const;

		friend class SortedSet;
	};

	/**
	 * SortedSet Default-C'tor.
	 *
	 * @return
	 * 	A new allocated SortedSet.
	 * 	Head and Tail fields are set to NULL and Size field is set to 0.
	 **/
	SortedSet();

	/**
	 * SortedSet Copy-C'tor.
	 *
	 * @param sortedSet - The copied SortedSet.
	 *
	 * @return
	 * 	A new allocated copy of given sortedSet.
	 **/
	SortedSet(const SortedSet& sortedSet);

	/**
	 * SortedSet D'tor.
	 *
	 * Frees all memory allocated for the given SortedSet.
	 **/
	~SortedSet();

	/**
	 * SortedSet Placement operator.
	 *
	 * @param other- The SortedSet to place.
	 *
	 * @return
	 * The SortedSet after placement.
	 **/
	SortedSet& operator=(const SortedSet& other);


	/**
	 * @return
	 * An Iterator to the head of the SortSet.
	 **/
	iterator begin() const;

	/**
	 * @return
	 * An Iterator to the end of the SortSet, the content of which is NULL.
	 **/
	iterator end() const;

	/**
	 * Insert an element to the SortedSet.
	 *
	 * @param data - The element to be inserted.
	 *
	 * @return
	 * True- If inserted successfully.
	 * False- If an identical element already exists in the SortedSet.
	 **/
	bool insert(const D& data);

	/**
	 * Remove an element from the SortedSet.
	 *
	 * @param data - The element to be removed.
	 *
	 * @return
	 * True- If removed successfully.
	 * False- If the element doesn't exist in the SortedSet.
	 **/
	bool remove(const D& data);

	/**
	 * @return
	 * The number of elements in the SortedSet.
	 **/
	int size() const;

	/**
	 * Finds an element in the sortedSet.
	 *
	 * @param data - The searched element.
	 *
	 * @return
	 * An iterator to the element in the SortedSet if exists.
	 * Otherwise, an iterator to the the end of the SortedSet.
	 *
	 **/
	iterator find(const D& data) const;
};

template<class D, class Compare>
SortedSet<D, Compare>::SortedSet() :
		head(NULL), tail(NULL), Size(0) {
}

template<class D, class Compare>
SortedSet<D, Compare>::SortedSet(const SortedSet& sortedSet) :
		head(NULL), tail(NULL), Size(0) {
	if (sortedSet.Size == 0) { //empty list
		return;
	}
	this->head = new Node<D>(*sortedSet.head);
	this->tail = this->head;
	this->Size++;
	Node<D>* next_head = this->head;
	Node<D>* original_set = sortedSet.head->next;
	while (this->Size < sortedSet.Size) {
		Node<D>* new_node = new Node<D>(*original_set);
		next_head->next = new_node;
		new_node->previous = next_head;
		this->Size++;
		next_head = next_head->next;
		original_set = original_set->next;
		this->tail = next_head;
	}
}

template<class D, class Compare>
SortedSet<D, Compare>::~SortedSet() {
	while (Size > 0) {
		Node<D>* next_head = head->next;
		delete head;
		Size--;
		head = next_head;
	}
}

template<class D, class Compare>
SortedSet<D, Compare>& SortedSet<D, Compare>::operator=(
		const SortedSet<D, Compare>& other) {
	if (this == &other) {
		return *this;
	}
	while (Size > 0) {
		Node<D>* next_head = head->next;
		delete head;
		Size--;
		head = next_head;
	}
	this->Size = 0;
	if (other.Size == 0) { //empty list
		this->head = NULL;
		this->tail = NULL;
		return *this;
	}
	this->head = new Node<D>(*other.head);
	this->Size++;
	Node<D> *next_head = this->head;
	Node<D> *original_set = other.head->next;
	while (this->Size < other.Size) {
		Node<D> *newNode = new Node<D>(*original_set);
		next_head->next = newNode;
		newNode->previous = next_head;
		this->tail = next_head;
		this->Size++;
		next_head = next_head->next;
		original_set = original_set->next;
	}
	return *this;
}

template<class D, class Compare>
typename SortedSet<D, Compare>::iterator SortedSet<D, Compare>::begin() const {
	if (Size == 0) {
		// the list is empty
		return SortedSet::iterator(this); //return NULL iterator (end of list)
	}
	return SortedSet::iterator(this, this->head); // return iterator to the beginning
}

template<class D, class Compare>
typename SortedSet<D, Compare>::iterator SortedSet<D, Compare>::end() const {
	return SortedSet::iterator(this); //return NULL iterator (end of list)
}

template<class D, class Compare>
bool SortedSet<D, Compare>::insert(const D& data) {
	iterator iterator = (*this).find(data);
	if (iterator != (*this).end()) {
		return false;
	}
	Node<D>* new_node = new Node<D>(data); // construct new node
	if (Size == 0) { // empty list
		this->head = new_node;
		this->tail = new_node;
		Size++;
		return true;
	}
	Node<D>* ptr = this->head;
	if (compare(data, *(ptr->data))) {
		this->head->previous = new_node;
		new_node->next = this->head;
		this->head = new_node;
		Size++;
		return true;
	}
	while (ptr && compare(*(ptr->data), data)) {
		ptr = ptr->next;
	}
	if (!ptr) {
		this->tail->next = new_node;
		new_node->previous = this->tail;
		this->tail = new_node;
		Size++;
		return true;
	}
	new_node->next = ptr;
	new_node->previous = ptr->previous;
	ptr->previous->next = new_node;
	ptr->previous = new_node;
	Size++;
	return true;
}

template<class D, class Compare>
bool SortedSet<D, Compare>::remove(const D& data) {
	iterator iterator = (*this).find(data);
	if (iterator == (*this).end()) {
		return false;
	}
	if (iterator.element == head) {	//iterator points to the head of the list
		Node<D>* next_node = iterator.element->next;
		delete iterator.element;
		this->head = next_node;
		if (next_node != NULL) { //remove the last element
			next_node->previous = NULL;
		}
		Size--;
		return true;
	}
	if (iterator.element == tail) {	//iterator points to the last node
		Node<D>* prev_node = iterator.element->previous;
		delete iterator.element;
		tail = prev_node;
		prev_node->next = NULL;
		Size--;
		return true;
	}
	Node<D>* next_node = iterator.element->next;
	Node<D>* prev_node = iterator.element->previous;
	delete iterator.element;
	prev_node->next = next_node;
	next_node->previous = prev_node;
	Size--;
	return true;
}

template<class D, class Compare>
int SortedSet<D, Compare>::size() const {
	return Size;
}

template<class D, class Compare>
typename SortedSet<D, Compare>::iterator SortedSet<D, Compare>::find(
		const D& data) const {
	int size = this->Size;
	iterator iterator = (*this).begin(); // set iterator to head of the list
	while (size > 0) {
		if (!compare(*((iterator.element)->data), (data))
				&& !compare(data, *((iterator.element)->data))) {
			return iterator;
		}
		iterator++;
		size--;
	}
	iterator = (*this).end();
	return iterator;
}


/**
 *
 * SortedSet AND operator.
 *
 * @param sortedSet1, sortedSet2 - The sets to intersect.
 *
 * @return
 * A SortedSet containing all common element of the SortedSets.
 **/
template<class D, class Compare>
SortedSet<D, Compare> operator&(const SortedSet<D, Compare>& sortedSet1,
		const SortedSet<D, Compare>& sortedSet2) {
	SortedSet<D, Compare> intersection;
	int size1 = sortedSet1.size();
	typename SortedSet<D, Compare>::iterator iterator1 = sortedSet1.begin();
	while (size1 > 0) {
		if (sortedSet2.find(*iterator1) != sortedSet2.end()) {
			intersection.insert(*iterator1);
		}
		iterator1++;
		size1--;
	}
	int size2 = sortedSet2.size();
	typename SortedSet<D, Compare>::iterator iterator2 = sortedSet2.begin();
	while (size2 > 0) {
		if (sortedSet1.find(*iterator2) != sortedSet1.end()) {
			intersection.insert(*iterator2);
		}
		iterator2++;
		size2--;
	}
	return intersection;
}

/**
 *
 * SortedSet AND operator.
 *
 * @param sortedSet1, sortedSet2 - The sets to unite.
 *
 * @return
 * A SortedSet containing all elements in either one of the
 * SortedSets.
 **/
template<class D, class Compare>
SortedSet<D, Compare> operator|(const SortedSet<D, Compare>& sortedSet1,
		const SortedSet<D, Compare>& sortedSet2) {
	SortedSet<D, Compare> unison;
	int size = sortedSet1.size();
	typename SortedSet<D, Compare>::iterator iterator = sortedSet1.begin();
	while (size > 0) {
		unison.insert(*iterator);
		iterator++;
		size--;
	}
	size = sortedSet2.size();
	iterator = sortedSet2.begin();
	while (size > 0) {
		unison.insert(*iterator);
		iterator++;
		size--;
	}
	return unison;
}

/**
 *
 * SortedSet MINUS operator.
 *
 * @param sortedSet1 - The set to substract from.
 * @param sortedSet2 - The set to substract.
 *
 * @return
 * A SortedSet containing all elements that are in sortedSet1
 * but not in sortedSet2.
 **/
template<class D, class Compare>
SortedSet<D, Compare> operator-(const SortedSet<D, Compare>& sortedSet1,
		const SortedSet<D, Compare>& sortedSet2) {
	SortedSet<D, Compare> unison;
	int size = sortedSet1.size();
	typename SortedSet<D, Compare>::iterator iterator = sortedSet1.begin();
	while (size > 0) {
		if (sortedSet2.find(*iterator) == sortedSet2.end()) {
			unison.insert(*iterator);
		}
		iterator++;
		size--;
	}
	return unison;
}

/**
 *
 * SortedSet XOR operator.
 *
 * @param sortedSet1, sortedSet2 - The sets to unite.
 *
 * @return
 * A SortedSet containing all elements that are in in one of the sortedSets
 * but not in the other.
 **/
template<class D, class Compare>
SortedSet<D, Compare> operator^(const SortedSet<D, Compare>& sortedSet1,
		const SortedSet<D, Compare>& sortedSet2) {
	SortedSet<D, Compare> unison = sortedSet1 | sortedSet2;
	SortedSet<D, Compare> intersection = sortedSet1 & sortedSet2;
	return unison - intersection;
}

/**********************************ITERATOR************************************/

template<class D, class Compare>
SortedSet<D, Compare>::iterator::iterator(
		const SortedSet<D, Compare>* sortedSet, Node<D>* element) :
		element(element) {
}

template<class D, class Compare>
SortedSet<D, Compare>::iterator::iterator(const iterator& iterator) :
		element(iterator.element) {
}

template<class D, class Compare>
typename SortedSet<D, Compare>::iterator&
SortedSet<D, Compare>::iterator::operator++() {
	if (!this->element) {
		// the iterator points to the end
		return (*this); // return the iterator without a change
	}
	//advance the iterator
	this->element = (this->element)->next;
	return (*this); // return reference to the iterator after the change
}

template<class D, class Compare>
typename SortedSet<D, Compare>::iterator SortedSet<D, Compare>::iterator::operator++(
		int) {
	iterator copy_iterator(*this); //make a copy for result
	++(*this); // advance the iterator
	return copy_iterator; // return the old data
}

template<class D, class Compare>
const D& SortedSet<D, Compare>::iterator::operator*() const {
	assert(this->element != NULL);
	return *((this->element)->data);
}

template<class D, class Compare>
typename SortedSet<D, Compare>::iterator&
SortedSet<D, Compare>::iterator::operator=(const iterator& iterator) {
	this->element = iterator.element;
	return (*this);
}

template<class D, class Compare>
bool SortedSet<D, Compare>::iterator::operator==(
		const iterator& iterator) const {
	return (this->element == iterator.element);
}

template<class D, class Compare>
bool SortedSet<D, Compare>::iterator::operator!=(
		const iterator& iterator) const {
	return !(iterator == *this);
}

#endif /* SORTEDSET_H_ */
