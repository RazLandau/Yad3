/*
 * yad3.c
 *
 *  Created on: 15 באפר 2016
 *      Author: Raz
 */

#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "yad3.h"

enum {
	CUSTOMER, REALTOR, SERVICE, APARTMENT
};

#define MIN_PURCHASES_SUM_FOR_SORT 1
#define MIN_APARTMENTS_FOR_SORT 1
#define OFFER_ACCEPTED (strcmp(choice, "accept") == 0)
#define COUNT_IS_VALID (count>0)

/**
 * The yad3 structure.
 */
struct yad3_t {
	Map realtors;
	Map customers;
	Set offers;
};

#define REALTORS (yad3->realtors)
#define CUSTOMERS (yad3->customers)
#define OFFERS (yad3->offers)

/****************************STATIC FUNCTIONS**********************************/

/* Map/Set/List */

/**
 * Converts given Map to List.
 *
 * @param
 * copyElement- Function pointer to be used for copying elements into
 * the list or when copying the list.
 * freeElement- Function pointer to be used for removing elements from
 *
 * @return
 * A newly allocated List containing newly allocated elements identical to Map.
 * NULL if map, copyElement or freeElement are NULL,
 * or in any case of memory allocation failure.
 */
List convertMap2List(Map map, CopyListElement copyElement,
		FreeListElement freeElement) {

	List list = listCreate(copyElement, freeElement);
	if (!list) {
		return NULL;
	}
	MAP_FOREACH(MapKeyElement,mapKeyElement,map)
	{
		MapDataElement mapDataElement = mapGet(map, mapKeyElement);
		ListResult result = listInsertLast(list, mapDataElement);
		if (result != LIST_SUCCESS) {
			listDestroy(list);
			return NULL;
		}
	}
	return list;
}

/**
 * Generic strcpy.
 *
 * @return
 * A newly allocated copy of the original string, in void* form.
 * NULL if the string is NULL or in any case of memory allocation failure.
 */
static void* genericStringCopy(void* string) {
	return (void*) (copyString((char*) string));
}

/**
 * Generic strcpy.
 *
 * @return
 * A newly allocated copy of the original string, in void* form.
 * NULL if the string is NULL or in any case of memory allocation failure.
 */
static void* genericStringCopy4Map(const void* string) {
	return genericStringCopy((void*) string);
}

/**
 * Generic strcmp.
 *
 * @return
 * 0- if elements are identical.
 * 1- if element1>element2.
 * -1- if element1<element2.
 *
 * This function asserts (string1 != NULL && string2 != NULL).
 */
static int genericStringCompare(void* string1, void* string2) {
	assert(string1 && string2);
	return strcmp((char*) string1, (char*) string2);
}

/**
 * Generic strcmp.
 *
 * @return
 * 0- if elements are identical.
 * 1- if element1>element2.
 * -1- if element1<element2.
 */
static int genericStringCompare4Map(const void* string1, const void* string2) {
	return genericStringCompare((void*) string1, (void*) string2);
}

/**
 * Generic free(char*).
 * Frees all memory allocated for the given element.
 * This function can receive NULL.
 */
void genericStringDestroy(void* string) {
	free((char*) string);
}

/**
 * Generic offerCopy
 *
 * @return
 * A newly allocated copy of the original offer, in void* form.
 * NULL if the offer is NULL or in any case of memory allocation failure.
 */
void* genericOfferCopy(void* offer) {
	OfferResult offer_result;
	return (void*) (offerCopy((Offer) offer, &offer_result));
}

/**
 * Generic offerDestroy.
 * Frees all memory allocated for the given offer.
 * This function can receive NULL.
 */
void genericOfferDestroy(void* offer) {
	offerDestroy((Offer) offer);
}

/**
 * Generic offerCompareByEmail.
 *
 * @return
 * true- if both offers' customer's and realtor's e-mails are identical
 * false- otherwise
 */
int genericOfferCompareByEmail(void* offer1, void* offer2) {
	return !offerCompareByEmail((Offer) offer1, (Offer) offer2);
}

/**
 * Generic customerCopy.
 *
 * @return
 * A newly allocated copy of the original customer, in void* form.
 * NULL if customer is NULL or in any case of memory allocation failure.
 */
void* genericCustomerCopy(void* customer) {
	CustomerResult customer_result;
	return (void*) (customerCopy((Customer) customer, &customer_result));
}

/**
 * Generic customerCopy.
 *
 * @return
 * A newly allocated copy of the original customer, in void* form.
 * NULL if customer is NULL or in any case of memory allocation failure.
 */
static void* genericCustomerCopy4Map(const void* customer) {
	return genericCustomerCopy((void*) customer);
}

/**
 * Generic customerDestroy.
 * Frees all memory allocated for the given customer.
 * This function can receive NULL.
 */
static void genericCustomerDestroy(void* customer) {
	customerDestroy((Customer) customer);
}

/**
 * Filters customers who spent less than key.
 *
 * @return
 * true- if customer has spent more than the number key points.
 * false- otherwise.
 */
static bool filterCustomersByPurchasesSum(void* customer, void* key) {
	return (customerGetPurchasesSum((Customer) customer) >= (*((int*) key)));
}

/**
 * Compares customers by lexicographical order of their e-mails.
 *
 * @return
 * 0- if e-mails are identical.
 * 1- if customer1's email > customer2's email.
 * -1- if customer1's email < customer2's email.
 */
static int sortCustomersByEmail(void* customer1, void* customer2) {
	char* customer1_email = customerGetEmail(customer1);
	char* customer2_email = customerGetEmail(customer2);
	int compare = strcmp(customer1_email, customer2_email);
	free(customer1_email);
	free(customer2_email);
	return compare;
}

/**
 * Sorts customer1 and customer2 by the amount money they have spent so far.
 * If equal, by lexicographical order of customers' e-mails.
 *
 * @return
 * 0 if customers are identical
 * 1 if customer1>customer2
 * -1 if customer1<customer2
 */
int sortCustomersByPurchasesSum(void* customer1, void* customer2) {
	int compare;
	compare = customerGetPurchasesSum((Customer) customer2)
			- customerGetPurchasesSum((Customer) customer1);
	if (compare == 0) {
		compare = sortCustomersByEmail((Customer) customer1,
				(Customer) customer2);
	}
	return compare;
}

/**
 * Generic realtorCopy.
 *
 * @return
 * A newly allocated copy of the original realtor, in void* form.
 * NULL if realtor is NULL or in any case of memory allocation failure.
 */
void* genericRealtorCopy(void* realtor) {
	RealtorResult realtor_result;
	return (void*) (realtorCopy((Realtor) realtor, &realtor_result));
}

/**
 * Generic realtorCopy.
 *
 * @return
 * A newly allocated copy of the original realtor, in void* form.
 * NULL if realtor is NULL or in any case of memory allocation failure.
 */
void* genericRealtorCopy4Map(const void* realtor) {
	return genericRealtorCopy((void*) realtor);
}

/**
 * Generic realtorDestroy.
 *
 * Frees all memory allocated for the given realtor.
 * This function can receive NULL.
 */
void genericRealtorDestroy(void* realtor) {
	realtorDestroy((Realtor) realtor);
}

/**
 * Filters realtors by their relevance to given customer.
 *
 * @return
 * true- if realtor has at least 1 apartment that meets customer's demands.
 * false- otherwise.
 */
bool filterRealtorsByRelevanceToCustomer(void* realtor, void* customer) {
	int area = customerGetMinArea((Customer) customer);
	int rooms = customerGetMinRooms((Customer) customer);
	int max_price_after_tax = (customerGetMaxPrice(customer) * 100)
			/ (100 + realtorGetTaxPercentage(realtor));
	return realtorApartmentExists((Realtor) realtor, area, rooms,
			max_price_after_tax);
}

/**
 * Filters realtors by num of apartments they hold.
 *
 * @return
 * true- if realtor has at least min_num_of_apartments of apartments.
 * false- otherwise.
 */
bool filterRealtorsByNumOfApartments(void* realtor, void* num) {
	return (realtorNumOfApartments(realtor) >= (*(int*) num));
}

/**
 * Compares realtors by lexicographical order of their e-mails.
 *
 * @return
 * 0- if e-mails are identical.
 * 1- if realtor1's email > realtor2's email.
 * -1- if realtor1's email < realtor2's email.
 */
int sortRealtorsByEmail(void* realtor1, void* realtor2) {
	char* realtor1_email = realtorGetEmail((Realtor) realtor1);
	char* realtor2_email = realtorGetEmail((Realtor) realtor2);
	int compare = strcmp(realtor1_email, realtor2_email);
	free(realtor1_email);
	free(realtor2_email);
	return compare;
}

/**
 * Compares realtors by their significance.
 *
 * @return
 * 0- if realtors are of identical significance.
 * 1- if realtor2 is more significant than realtor1.
 * -1- if realtor1 is more significant than realtor2.
 */
int sortRealtorsBySignificance(void* realtor1, void* realtor2) {
	int compare;
	int rank1 = realtorGetRank((Realtor) realtor1);
	int rank2 = realtorGetRank((Realtor) realtor2);
	compare = rank2 - rank1;
	if (compare == 0) {
		compare = sortRealtorsByEmail(realtor1, realtor2);
	}
	return compare;
}

/* results transformations*/

/**
 * Converts MapResult to Yad3Result.
 *
 * @return
 * Yad3Result equivalent to given map_result
 */
static Yad3Result mapResult2Yad3Result(MapResult map_result) {
	Yad3Result yad3_result;
	switch (map_result) {
	case MAP_SUCCESS:
		yad3_result = YAD3_SUCCESS;
		break;
	case MAP_OUT_OF_MEMORY:
		yad3_result = YAD3_OUT_OF_MEMORY;
		break;
	case MAP_NULL_ARGUMENT:
		yad3_result = YAD3_INVALID_PARAMETERS;
		break;
	case MAP_ITEM_ALREADY_EXISTS:
		yad3_result = YAD3_EMAIL_ALREADY_EXISTS;
		break;
	case MAP_ITEM_DOES_NOT_EXIST:
		yad3_result = YAD3_EMAIL_DOES_NOT_EXIST;
	}
	return yad3_result;
}

/**
 * Converts offerResult to Yad3Result.
 *
 * @return
 * Yad3Result equivalent to given offer_result
 */
static Yad3Result offerResult2Yad3Result(OfferResult offer_result) {
	Yad3Result yad3_result;
	switch (offer_result) {
	case OFFER_SUCCESS:
		yad3_result = YAD3_SUCCESS;
		break;
	case OFFER_OUT_OF_MEMORY:
		yad3_result = YAD3_OUT_OF_MEMORY;
		break;
	case OFFER_INVALID_PARAMETERS:
		yad3_result = YAD3_INVALID_PARAMETERS;
	}
	return yad3_result;
}

/**
 * Converts customerResult to Yad3Result.
 *
 * @return
 * Yad3Result equivalent to given customer_result
 */
static Yad3Result customerResult2Yad3Result(CustomerResult customer_result) {
	Yad3Result yad3_result;
	switch (customer_result) {
	case CUSTOMER_SUCCESS:
		yad3_result = YAD3_SUCCESS;
		break;
	case CUSTOMER_OUT_OF_MEMORY:
		yad3_result = YAD3_OUT_OF_MEMORY;
		break;
	case CUSTOMER_INVALID_PARAMETERS:
		yad3_result = YAD3_INVALID_PARAMETERS;
		break;
	}
	return yad3_result;
}

/**
 * Converts realtorResult to Yad3Result.
 *
 * @return
 * Yad3Result equivalent to given realtor_result
 */
static Yad3Result realtorResult2Yad3Result(RealtorResult realtor_result) {
	Yad3Result yad3_result;
	switch (realtor_result) {
	case REALTOR_SUCCESS:
		yad3_result = YAD3_SUCCESS;
		break;
	case REALTOR_OUT_OF_MEMORY:
		yad3_result = YAD3_OUT_OF_MEMORY;
		break;
	case REALTOR_INVALID_PARAMETERS:
		yad3_result = YAD3_INVALID_PARAMETERS;
		break;
	case REALTOR_SERVICE_ALREADY_EXISTS:
		yad3_result = YAD3_SERVICE_ALREADY_EXISTS;
		break;
	case REALTOR_SERVICE_DOES_NOT_EXIST:
		yad3_result = YAD3_SERVICE_DOES_NOT_EXIST;
		break;
	case REALTOR_SERVICE_FULL:
		yad3_result = YAD3_SERVICE_FULL;
		break;
	case REALTOR_APARTMENT_ALREADY_EXISTS:
		yad3_result = YAD3_APARTMENT_ALREADY_EXISTS;
		break;
	case REALTOR_APARTMENT_DOES_NOT_EXIST:
		yad3_result = YAD3_APARTMENT_DOES_NOT_EXIST;
	}
	return yad3_result;
}

/* input validation */

/**
 * Checks if matrix is valid.
 *
 * @return
 * true- if matrix contains only 'e' or 'w' and if width*height=matrix's length.
 * false- otherwise.
 */
static bool matrixIsValid(char* matrix, int width, int height) {
	assert(matrix && WIDTH_IS_VALID && HEIGHT_IS_VALID);
	int length = strlen(matrix);
	if (length != height * width) {
		return false;
	}
	for (int i = 0; i < length; i++) {
		if (matrix[i] != 'e' && matrix[i] != 'w') {
			return false;
		}
	}
	return true;
}

/**
 * Checks if choice is valid.
 *
 * @return
 * true- if choice is either 'accept' or 'decline'
 * false- otherwise.
 */
static bool choiceIsValid(char* choice) {
	return (strcmp(choice, "accept") == 0 || strcmp(choice, "decline") == 0);
}

/* email checks */

/**
 * Checks if email already exists in yad3's customers.
 *
 * @return
 * true- if a realtor with the email same exists in yad3.
 * false- otherwise.
 */
static bool emailExistsInRealtors(Yad3 yad3, char* email) {
	assert(yad3 && email && emailIsValid(email));
	return mapContains(REALTORS, email);
}

/**
 * Checks if email already exists in yad3's customers.
 *
 * @return
 * true- if a customer with the email same exists in yad3.
 * false- otherwise.
 */
static bool emailExistsInCustomers(Yad3 yad3, char* email) {
	assert(yad3 && email && emailIsValid(email));
	return mapContains(CUSTOMERS, email);
}

/**
 * Checks if email already exists in yad3.
 *
 * @return
 * true- if a realtor or customer with the same email exists in yad3.
 * false- otherwise.
 */
static bool emailAlreadyExists(Yad3 yad3, char* email) {
	assert(yad3 && email && emailIsValid(email));
	return (emailExistsInRealtors(yad3, email)
			|| emailExistsInCustomers(yad3, email));
}

/* remove offer */

/**
 *
 * Removes offer in Yad3 from customer/to realtor.
 *
 * @param email - customer's/realtor's email.
 * @param type- type to remove:
 * 0- customer
 * 1- realtor
 *
 *@return
 * YAD3_OUT_OF_MEMORY- in case of a memory allocation failure.
 * YAD3_SUCCESS- otherwise.
 *
 * This function asserts (yad3 != NULL && email != NULL).
 *
 */
Yad3Result removeOfferByEmail(Yad3 yad3, char* email, bool type) {
	assert(yad3 && email);
	SET_FOREACH(Offer,i,OFFERS)
	{
		Offer offer = setGetCurrent(OFFERS);
		assert(offer);
		char* offer_email;
		type ? (offer_email = offerGetRealtorEmail(offer)) : (offer_email =
						offerGetCustomerEmail(offer));
		if (!offer_email) {
			return YAD3_OUT_OF_MEMORY;
		}
		if (strcmp(offer_email, email) == 0) {
			setRemove(OFFERS, offer);
			offer = setGetFirst(OFFERS);
		}
		free(offer_email);
	}
	return YAD3_SUCCESS;
}

/**
 *
 * Removes all offers in Yad3 on apartments in given service.
 *
 * @param realtor_email - service's realtor.
 * @param service_name - service's name.
 *
 *@return
 * YAD3_OUT_OF_MEMORY- in case of a memory allocation failure.
 * YAD3_SUCCESS- otherwise.
 *
 * This function asserts (yad3 && realtor_email && service_name).
 *
 */
Yad3Result removeOfferByService(Yad3 yad3, char* realtor_email,
		char* service_name) {
	assert(yad3 && realtor_email && service_name);
	SET_FOREACH(Offer,i,OFFERS)
	{
		Offer offer = setGetCurrent(OFFERS);
		assert(offer);
		char* offer_service_name = offerGetServiceName(offer);
		char* offer_realtor_email = offerGetRealtorEmail(offer);
		if (!offer_service_name || !offer_realtor_email) {
			return YAD3_OUT_OF_MEMORY;
		}
		if (strcmp(offer_realtor_email, realtor_email) == 0
				&& strcmp(offer_service_name, service_name) == 0) {
			setRemove(OFFERS, offer);
			offer = setGetFirst(OFFERS);
		}
		free(offer_service_name);
		free(offer_realtor_email);
	}
	return YAD3_SUCCESS;
}

/**
 *
 * Removes all offers on a specific apartment in Yad3.
 *
 * @param realtor_email - apartment's realtor.
 * @param service_name - apartment's service.
 * @param apartment_id - apartment's id.
 *
 *@return
 * YAD3_OUT_OF_MEMORY- in case of a memory allocation failure.
 * YAD3_SUCCESS- otherwise.
 *
 * This function asserts (yad3 != NULL && email != NULL).
 *
 */
Yad3Result removeOfferByApartment(Yad3 yad3, char* realtor_email,
		char* service_name, int apartment_id) {
	assert(yad3 && realtor_email && service_name);
	SET_FOREACH(Offer,i,OFFERS)
	{
		Offer offer = setGetCurrent(OFFERS);
		assert(offer);
		char* offer_service_name = offerGetServiceName(offer);
		char* offer_realtor_email = offerGetRealtorEmail(offer);
		if (!offer_service_name || !offer_realtor_email) {
			return YAD3_OUT_OF_MEMORY;
		}
		int offer_apartment_id = offerGetApartmentId(offer);
		if (strcmp(offer_realtor_email, realtor_email) == 0
				&& strcmp(offer_service_name, service_name) == 0
				&& apartment_id == offer_apartment_id) {
			setRemove(OFFERS, offer);
			offer = setGetFirst(OFFERS);
		}
		free(offer_service_name);
		free(offer_realtor_email);
	}
	return YAD3_SUCCESS;
}

/* customer and realtor merger functions  */

/**
 *
 * Adds customer/realtor with given email to yad3.
 *
 * @param
 * type- Type to add:
 * 0- Customer,
 * 1- Realtor.
 *
 *@return
 * YAD3_EMAIL_ALREADY_EXISTS- if customer/realtor with same email already
 * exists in yad3.
 * YAD3_OUT_OF_MEMORY- in case of a memory allocation failure.
 *
 * This function asserts assert(yad3 != NULL)
 */
static Yad3Result addCustomerOrRealtor(Yad3 yad3, char* email,
		MapDataElement element, bool type, Yad3Result yad3_result) {
	assert(yad3);
	if (yad3_result != YAD3_SUCCESS) {
		assert(!element);
		return yad3_result;
	}
	if (emailAlreadyExists(yad3, email)) {
		type ? realtorDestroy(element) : customerDestroy(element);
		return YAD3_EMAIL_ALREADY_EXISTS;
	}
	MapResult map_result;
	type ? (map_result = mapPut(REALTORS, email, element)) : (map_result =
					mapPut(
					CUSTOMERS, email, element));
	type ? realtorDestroy(element) : customerDestroy(element);
	return mapResult2Yad3Result(map_result);
}

/**
 *
 * Removes customer/realtor with given email from yad3.
 *
 * @param
 * type- Type to add:
 * 0- Customer,
 * 1- Realtor.
 *
 *@return
 * YAD3_EMAIL_DOES_NOT_EXIST- if the email doesn't exists in yad3.
 * YAD3_EMAIL_WRONG_ACCOUNT_TYPE- if a realtor/customer with the same id exists
 * in yad3, respectively.
 * YAD3_OUT_OF_MEMORY- in case of a memory allocation failure.
 * YAD3_SUCCESS- otherwise.
 *
 */
static Yad3Result removeCustomerOrRealtor(Yad3 yad3, char* email, bool type) {
	if (!yad3 || !emailIsValid(email)) {
		return YAD3_INVALID_PARAMETERS;
	}
	if (!emailAlreadyExists(yad3, email)) {
		return YAD3_EMAIL_DOES_NOT_EXIST;
	}
	bool incorrect_email;
	type ? (incorrect_email = emailExistsInCustomers(yad3, email)) : (incorrect_email =
					emailExistsInRealtors(yad3, email));
	if (incorrect_email) {
		return YAD3_EMAIL_WRONG_ACCOUNT_TYPE;
	}
	MapResult map_result;
	type ? (map_result = mapRemove(REALTORS, email)) : (map_result = mapRemove(
	CUSTOMERS, email));
	assert(map_result == MAP_SUCCESS);
	Yad3Result yad3_result;
	type ? (yad3_result = removeOfferByEmail(yad3, email, REALTOR)) : (yad3_result =
					removeOfferByEmail(yad3, email, CUSTOMER));
	return yad3_result;
}

/* get functions */

/**
 * Returns the customer/realtor with the same email in yad3.
 *
 * @param
 * type- The type of element to be returned:
 * 0- Customer
 * 1- Realtor
 *
 * @return
 * The customer/realtor in MapDataElement(void*) form.
 * NULL- if:
 * yad3 or email is NULL,
 * email doesn't contain exactly one '@',
 * email doesn't exist in yad3,
 * email belongs to realtor/customer in yad3, respectively.
 */
static MapDataElement getCustomerOrRealtor(Yad3 yad3, bool type, char* email,
		Yad3Result* yad3_result) {
	if (!yad3 || !emailIsValid(email)) {
		*yad3_result = YAD3_INVALID_PARAMETERS;
		return NULL;
	}
	if (!emailAlreadyExists(yad3, email)) {
		*yad3_result = YAD3_EMAIL_DOES_NOT_EXIST;
		return NULL;
	}
	bool correct_type;
	type ? (correct_type = emailExistsInRealtors(yad3, email)) : (correct_type =
					emailExistsInCustomers(yad3, email));
	if (!correct_type) {
		*yad3_result = YAD3_EMAIL_WRONG_ACCOUNT_TYPE;
		return NULL;
	}
	*yad3_result = YAD3_SUCCESS;
	MapDataElement map_data_element;
	type ? (map_data_element = mapGet(REALTORS, email)) : (map_data_element =
					mapGet(CUSTOMERS, email));
	assert(map_data_element);
	return map_data_element;
}

/**
 * Returns the customer with the same email in yad3.
 *
 * @param
 * yad3_result- Pointer that hold the result/cause of failure.
 *
 * @return
 * The realtor.
 * NULL if
 * yad3 or customer_email are NULL,
 * email doesn't contain exactly one '@',
 * the email doesn't exist in yad3,
 * if the email belongs to a realtor in yad3 respectively.
 */
static Customer getCustomer(Yad3 yad3, char* customer_email,
		Yad3Result* yad3_result) {
	return getCustomerOrRealtor(yad3, CUSTOMER, customer_email, yad3_result);
}

/**
 * Returns the realtor with the same email in yad3.
 *
 * @param
 * yad3_result- Pointer that hold the result/cause of failure.
 *
 * @return
 * The realtor.
 * NULL if
 * yad3 or customer_email are NULL,
 * email doesn't contain exactly one '@',
 * the email doesn't exist in yad3,
 * if the email belongs to a customer in yad3 respectively.
 */
static Realtor getRealtor(Yad3 yad3, char* realtor_email,
		Yad3Result* yad3_result) {
	return getCustomerOrRealtor(yad3, REALTOR, realtor_email, yad3_result);
}

/**
 * Returns the offer in yad3 with the same customer's and realtor's e-mails.
 *
 * @return
 *   NULL if an offer as requested doesn't exist in yad3.
 *   The current offer in case of success
 */
static Offer getOffer(Yad3 yad3, char* realtor_email, char* customer_email) {
	SET_FOREACH(Offer,i,OFFERS)
	{
		Offer offer = setGetCurrent(OFFERS);
		char* offer_realtor_email = offerGetRealtorEmail(offer);
		char* offer_customer_email = offerGetCustomerEmail(offer);
		if (strcmp(offer_realtor_email, realtor_email) == 0
				&& strcmp(offer_customer_email, customer_email) == 0) {
			free(offer_realtor_email);
			free(offer_customer_email);
			return offer;
		}
		free(offer_realtor_email);
		free(offer_customer_email);
	}
	return NULL;
}

/* offer and purchase */

/**
 * Calculates apartment's price after realtor's tax.
 *
 * @return
 * The apartment's price including the realtor's tax percentage.
 * This function asserts (customer != NULL && apartment != NULL)
 */
static int apartmentPricePlusTax(Realtor realtor, Apartment apartment) {
	assert(realtor && apartment);
	int price = apartmentGetPrice(apartment)
			+ ((apartmentGetPrice(apartment) * realtorGetTaxPercentage(realtor))
					/ 100);
	return price;
}

/**
 * Checks if apartment is valid for offer.
 *
 * @return
 * true- if apartment meets given customer criteria.
 * false- otherwise.
 * This function asserts (customer != NULL && apartment != NULL)
 */
static bool apartmentIsValidForOffer(Customer customer, Apartment apartment) {
	assert(customer && apartment);
	return (apartmentTotalArea(apartment) >= customerGetMinArea(customer)
			&& apartmentNumOfRooms(apartment) >= customerGetMinRooms(customer));
}

/**
 * Check if offer's new price is valid.
 *
 * @return
 * true- if offer's new price meets customer's demands.
 * false- otherwise.
 */
static bool offerNewPriceIsValid(int new_price, Customer customer) {
	return new_price <= customerGetMaxPrice(customer);
}

/**
 * Checks if offer's new price is logical.
 *
 * @return
 * true- if new_price is lower than what the customer would have paid on the
 * apartment in normal purchase.
 * false- otherwise.
 * This function asserts (realtor != NULL && apartment != NULL)
 */
static bool offerNewPriceIsLogical(Realtor realtor, Apartment apartment,
		int new_price) {
	assert(realtor && apartment);
	return (new_price < apartmentPricePlusTax(realtor, apartment));
}

/**
 * Removes the accepted offer's apartment from realtor and adds the sum of the
 * transaction to customer.
 * This function asserts
 * (yad3!=NULL && offer!=NULL && realtor_email!= NULL && choice=='accepted')
 */
static Yad3Result executeOffer(Yad3 yad3, Offer offer, char* realtor_email,
		char* customer_email, char* choice) {
	assert(yad3 && offer && realtor_email && customer_email);
	assert(OFFER_ACCEPTED);
	Customer customer = mapGet(CUSTOMERS, customer_email);
	customerAddPurchase(customer, offerGetPrice(offer));
	char* offer_service_name = offerGetServiceName(offer);
	if (!offer_service_name) {
		return YAD3_OUT_OF_MEMORY;
	}
	int id = offerGetApartmentId(offer);
	Yad3Result yad3_result = yad3RemoveApartmentFromRealtor(yad3, realtor_email,
			offer_service_name, id);
	free(offer_service_name);
	return yad3_result;
}

/**
 * Checks if given apartment meets customer's demands.
 *
 * @return
 * true- if apartment meets given customer criteria.
 * false- otherwise.
 * This function asserts (customer!=NULL && apartment!=NULL && realtor!=NULL)
 */
static bool apartmentIsValidForPurchase(Customer customer, Apartment apartment,
		Realtor realtor) {
	assert(customer && apartment && realtor);
	return (apartmentIsValidForOffer(customer, apartment)
			&& apartmentPricePlusTax(realtor, apartment)
					<= customerGetMaxPrice(customer));
}

/* sort */

/**
 * Creates a new allocated List of types sorted by criteria.
 *
 * @param
 * type- List's type:
 * 0- Customer
 * 1- Realtor
 * criteria- The criteria to sort by:
 * 0- most paying customers
 * 1- realtor's signifigance
 * 2- relevant realtors to customer
 * filterElement- function to filter list by.
 * compareElement- function to sort list by.
 *
 * @return
 * A newly allocated sorted List.
 * In case of a memory allocation failure - return NULL.
 *
 * This function asserts (yad3 && filterElement && compareElement && key).
 */
static List createSortedTypeListByCriteria(Yad3 yad3, bool type,
		FilterListElement filterElement, CompareListElements compareElement,
		void* key) {
	assert(yad3 && filterElement && compareElement && key);
	List list;
	type ? (list = convertMap2List(REALTORS, genericRealtorCopy,
			genericRealtorDestroy)) :
			(list = convertMap2List(CUSTOMERS, genericCustomerCopy,
					genericCustomerDestroy));
	if (!list) {
		return NULL;
	}
	List filtered_list;
	filtered_list = listFilter(list, filterElement, key);
	listDestroy(list);
	if (!filtered_list) {
		return NULL;
	}
	ListResult list_result;
	list_result = listSort(filtered_list, compareElement);
	if (list_result != LIST_SUCCESS) {
		listDestroy(filtered_list);
		return NULL;
	}
	return filtered_list;
}

/**
 * Creates a new allocated List of customers who spent any money in yad3,
 * sorted by the amount of money they have spent by descending order.
 * If equal, by lexicographical order of e-mails.
 *
 * @return
 * A newly allocated sorted List.
 * In case of a memory allocation failure - return NULL.
 *
 * This function asserts (yad3 != NULL)
 */
static List createSortedListOfMostPayingCustomers(Yad3 yad3) {
	assert(yad3);
	int key = MIN_PURCHASES_SUM_FOR_SORT;
	return createSortedTypeListByCriteria(yad3, CUSTOMER,
			filterCustomersByPurchasesSum, sortCustomersByPurchasesSum, &key);
}

/**
 * Creates a new allocated List of realtor with 1 or more apartments in yad3,
 * sorted by significance in descending order.
 * In case of equal significance- by lexicographical order of e-mails.
 * @return
 * A newly allocated sorted List.
 * In case of a memory allocation failure - return NULL.
 *
 * This function asserts (yad3 != NULL)
 */
static List createSortedListOfSignificantRealtors(Yad3 yad3) {
	assert(yad3);
	int key = MIN_APARTMENTS_FOR_SORT;
	return createSortedTypeListByCriteria(yad3, REALTOR,
			filterRealtorsByNumOfApartments, sortRealtorsBySignificance, &key);
}

/**
 * Creates a new allocated List of realtors who has at least one apartment that
 * meets the given customer, identified by customer_email, demands, sorted by
 * lexicographical order of e-mails.
 *
 * @return
 * A newly allocated sorted List.
 * In case of a memory allocation failure - return NULL.
 *
 * This function asserts (yad3 && customer_email && customer_email is valid)
 */
static List createSortedListOfRelevantRealtors(Yad3 yad3, char* customer_email) {
	assert(yad3 && emailIsValid(customer_email));
	Customer key = mapGet(CUSTOMERS, customer_email);
	assert(key);
	return createSortedTypeListByCriteria(yad3, REALTOR,
			filterRealtorsByRelevanceToCustomer, sortRealtorsByEmail, key);
}

/* print */

/**
 * Prints a List of types from 1st element to element in count place.
 *
 * @param outputChannel - File descriptor for the output channel
 *
 * This function asserts (list != NULL && outputChannel != NULL).
 */
static void printListToCount(List list, int count, FILE* outputChannel,
bool type) {
	assert(list && outputChannel);
	int counter = 0;
	LIST_FOREACH(ListElement,i,list)
	{
		if (counter == count || counter == listGetSize(list)) {
			break;
		}
		ListElement element = listGetCurrent(list);
		type ? realtorPrint(outputChannel, element) : customerPrint(
						outputChannel, element);
		counter++;
	}
}

/* Create */

/**
 * Creates a new empty map of Realtors:
 * Key = is the realtor's email.
 * Data = Realtor.
 *
 * @return
 * 	A new allocated map of Realtors.
 *  In case of a memory allocation failure - return NULL.
 **/
static Map createRealtorsMap() {
	return mapCreate(genericRealtorCopy4Map, genericStringCopy4Map,
			genericRealtorDestroy, genericStringDestroy,
			genericStringCompare4Map);
}

/**
 * Creates a new map of Customers:
 * Key = is the customer's email.
 * Data = Customer.
 *
 * @return
 * 	A new allocated empty map of Customers.
 *  In case of a memory allocation failure - return NULL.
 **/
static Map createCustomersMap() {
	return mapCreate(genericCustomerCopy4Map, genericStringCopy4Map,
			genericCustomerDestroy, genericStringDestroy,
			genericStringCompare4Map);
}

/**
 * Creates a new empty set of Offers.
 * @param genericOfferCompareByEmail - the compare function. The comparison is
 * by both offer's customer's email and realtor's email.
 *
 * @return
 * 	A new allocated set of Offers.
 *  In case of a memory allocation failure - return NULL.
 **/
static Set createOffersSet() {
	return setCreate(genericOfferCopy, genericOfferDestroy,
			genericOfferCompareByEmail);
}

/******************************************************************************/

Yad3 yad3Create() {
	Yad3 yad3 = malloc(sizeof(*yad3));
	Map realtors = createRealtorsMap();
	Map customers = createCustomersMap();
	Set offers = createOffersSet();
	if (!yad3 || !realtors || !customers || !offers) {
		free(yad3);
		mapDestroy(realtors);
		mapDestroy(customers);
		setDestroy(offers);
		return NULL;
	}
	REALTORS = realtors;
	CUSTOMERS = customers;
	OFFERS = offers;
	return yad3;
}

void yad3Destroy(Yad3 yad3) {
	if (yad3) {
		mapDestroy(REALTORS);
		mapDestroy(CUSTOMERS);
		setDestroy(OFFERS);
	}
	free(yad3);
}

Yad3Result yad3AddCustomer(Yad3 yad3, char* email, int min_rooms, int min_area,
		int max_price) {
	if (!yad3) {
		return YAD3_INVALID_PARAMETERS;
	}
	CustomerResult customer_result;
	Customer customer = customerCreate(email, min_rooms, min_area, max_price,
			&customer_result);
	Yad3Result yad3_result = customerResult2Yad3Result(customer_result);
	return addCustomerOrRealtor(yad3, email, customer, CUSTOMER, yad3_result);
}

Yad3Result yad3RemoveCustomer(Yad3 yad3, char* email) {
	return removeCustomerOrRealtor(yad3, email, CUSTOMER);
}

Yad3Result yad3AddRealtor(Yad3 yad3, char* email, char* company_name,
		int tax_percentage) {
	if (!yad3) {
		return YAD3_INVALID_PARAMETERS;
	}
	RealtorResult realtor_result;
	Realtor realtor = realtorCreate(email, company_name, tax_percentage,
			&realtor_result);
	Yad3Result yad3_result = realtorResult2Yad3Result(realtor_result);
	return addCustomerOrRealtor(yad3, email, realtor, REALTOR, yad3_result);
}

Yad3Result yad3RemoveRealtor(Yad3 yad3, char* email) {
	return removeCustomerOrRealtor(yad3, email, REALTOR);
}

Yad3Result yad3AddServiceToRealtor(Yad3 yad3, char* email, char* service_name,
		int max_apartments) {
	if (!yad3 || !service_name || !MAX_APARTMENT_IS_VALID) {
		return YAD3_INVALID_PARAMETERS;
	}
	Yad3Result yad3_result;
	Realtor realtor = getRealtor(yad3, email, &yad3_result);
	if (!realtor) {
		return yad3_result;
	}
	RealtorResult realtor_result = realtorAddService(realtor, service_name,
			max_apartments);
	return realtorResult2Yad3Result(realtor_result);
}

Yad3Result yad3RemoveServiceFromRealtor(Yad3 yad3, char* realtor_email,
		char* service_name) {
	if (!yad3 || !service_name) {
		return YAD3_INVALID_PARAMETERS;
	}
	Yad3Result yad3_result;
	Realtor realtor = getRealtor(yad3, realtor_email, &yad3_result);
	if (!realtor) {
		return yad3_result;
	}
	RealtorResult realtor_result = realtorRemoveService(realtor, service_name);
	if (realtor_result == REALTOR_SUCCESS) {
		Yad3Result yad3_result = removeOfferByService(yad3, realtor_email,
				service_name);
		return yad3_result;
	}
	return realtorResult2Yad3Result(realtor_result);
}

Yad3Result yad3AddApartmentToRealtor(Yad3 yad3, char* realtor_email,
		char* service_name, int apartment_id, int price, int width, int height,
		char* matrix) {
	if (!yad3 || !service_name || !matrix || !ID_IS_VALID || !PRICE_IS_VALID
			|| !WIDTH_IS_VALID || !HEIGHT_IS_VALID
			|| !matrixIsValid(matrix, width, height)) {
		return YAD3_INVALID_PARAMETERS;
	}
	Yad3Result yad3_result;
	Realtor realtor = getRealtor(yad3, realtor_email, &yad3_result);
	if (!realtor) {
		return yad3_result;
	}
	RealtorResult realtor_result = realtorAddApartment(realtor, service_name,
			apartment_id, price, width, height, matrix);
	return realtorResult2Yad3Result(realtor_result);
}

Yad3Result yad3RemoveApartmentFromRealtor(Yad3 yad3, char* realtor_email,
		char* service_name, int apartment_id) {
	if (!yad3 || !service_name || !ID_IS_VALID) {
		return YAD3_INVALID_PARAMETERS;
	}
	Yad3Result yad3_result;
	Realtor realtor = getRealtor(yad3, realtor_email, &yad3_result);
	if (!realtor) {
		return yad3_result;
	}
	RealtorResult realtor_result = realtorRemoveApartment(realtor, service_name,
			apartment_id);
	if (realtor_result != REALTOR_SUCCESS) {
		return realtorResult2Yad3Result(realtor_result);
	}
	yad3_result = removeOfferByApartment(yad3, realtor_email, service_name,
			apartment_id);
	return yad3_result;

}

Yad3Result yad3AddOffer(Yad3 yad3, char* customer_email, char* realtor_email,
		char* service_name, int apartment_id, int new_price) {
	if (!yad3) {
		return YAD3_INVALID_PARAMETERS;
	}
	OfferResult offer_result;
	Offer offer = offerCreate(customer_email, realtor_email, service_name,
			apartment_id, new_price, &offer_result);
	if (!offer) {
		offerDestroy(offer);
		return offerResult2Yad3Result(offer_result);
	}
	Yad3Result yad3_result;
	Customer customer = getCustomer(yad3, customer_email, &yad3_result);
	if (!customer) {
		offerDestroy(offer);
		return yad3_result;
	}
	Realtor realtor = getRealtor(yad3, realtor_email, &yad3_result);
	if (!realtor) {
		offerDestroy(offer);
		return yad3_result;
	}
	RealtorResult realtor_result;
	if (setIsIn(OFFERS, offer)) {
		offerDestroy(offer);
		return YAD3_ALREADY_REQUESTED;
	}
	Apartment apartment = realtorGetApartmentById(realtor, service_name,
			apartment_id, &realtor_result);
	if (!apartment) {
		offerDestroy(offer);
		return realtorResult2Yad3Result(realtor_result);
	}
	if (!offerNewPriceIsValid(new_price, customer)
			|| !apartmentIsValidForOffer(customer, apartment)) {
		apartmentDestroy(apartment);
		offerDestroy(offer);
		return YAD3_REQUEST_WRONG_PROPERTIES;
	}
	if (!offerNewPriceIsLogical(realtor, apartment, new_price)) {
		apartmentDestroy(apartment);
		offerDestroy(offer);
		return YAD3_REQUEST_ILLOGICAL_PRICE;
	}
	setAdd(OFFERS, offer);
	offerDestroy(offer);
	apartmentDestroy(apartment);
	return YAD3_SUCCESS;
}

Yad3Result yad3RealtoRespondToOffer(Yad3 yad3, char* realtor_email,
		char* customer_email, char* choice) {
	if (!yad3 || !emailIsValid(realtor_email) || !emailIsValid(customer_email)
			|| !choice || !choiceIsValid(choice)) {
		return YAD3_INVALID_PARAMETERS;
	}
	if (!emailAlreadyExists(yad3, customer_email)
			|| !emailAlreadyExists(yad3, realtor_email)) {
		return YAD3_EMAIL_DOES_NOT_EXIST;
	}
	if (emailExistsInCustomers(yad3, realtor_email)
			|| emailExistsInRealtors(yad3, customer_email)) {
		return YAD3_EMAIL_WRONG_ACCOUNT_TYPE;
	}
	Offer offer = getOffer(yad3, realtor_email, customer_email);
	if (!offer) {
		return YAD3_NOT_REQUESTED;
	}
	if (OFFER_ACCEPTED) {
		Yad3Result yad3_result = executeOffer(yad3, offer, realtor_email,
				customer_email, choice);
		return yad3_result;
	}
	setRemove(OFFERS, offer);
	return YAD3_SUCCESS;
}

Yad3Result yad3CustomerPurchase(Yad3 yad3, char* customer_email,
		char* realtor_email, char* service_name, int apartment_id) {
	if (!yad3 || !service_name || !apartment_id || !ID_IS_VALID) {
		return YAD3_INVALID_PARAMETERS;
	}
	Yad3Result yad3_result;
	Customer customer = getCustomer(yad3, customer_email, &yad3_result);
	if (!customer) {
		return yad3_result;
	}
	Realtor realtor = getRealtor(yad3, realtor_email, &yad3_result);
	if (!realtor) {
		return yad3_result;
	}
	RealtorResult realtor_result;
	Apartment apartment = realtorGetApartmentById(realtor, service_name,
			apartment_id, &realtor_result);
	if (!apartment) {
		apartmentDestroy(apartment);
		return realtorResult2Yad3Result(realtor_result);
	}
	assert(realtor_result == REALTOR_SUCCESS);
	if (!apartmentIsValidForPurchase(customer, apartment, realtor)) {
		apartmentDestroy(apartment);
		return YAD3_PURCHASE_WRONG_PROPERTIES;
	}
	int purchase_price = apartmentPricePlusTax(realtor, apartment);
	customerAddPurchase(customer, purchase_price);
	yad3_result = yad3RemoveApartmentFromRealtor(yad3, realtor_email,
			service_name, apartment_id);
	apartmentDestroy(apartment);
	return yad3_result;
}

int yad3NumOfCustomers(Yad3 yad3) {
	assert(yad3);
	return mapGetSize(CUSTOMERS);
}

int yad3NumOfRealtors(Yad3 yad3) {
	assert(yad3);
	return mapGetSize(REALTORS);
}

int yad3NumOfServices(Yad3 yad3) {
	assert(yad3);
	int counter = 0;
	MAP_FOREACH(Realtor,i,REALTORS)
	{
		Realtor realtor = mapGet(REALTORS, i);
		counter += realtorNumOfServices(realtor);
	}
	return counter;
}

int yad3NumOfApartments(Yad3 yad3) {
	assert(yad3);
	int counter = 0;
	MAP_FOREACH(Realtor,i,REALTORS)
	{
		Realtor realtor = mapGet(REALTORS, i);
		counter += realtorNumOfApartments(realtor);
	}
	return counter;
}

int yad3NumOfOffers(Yad3 yad3) {
	assert(yad3);
	return setGetSize(OFFERS);
}

int yad3PurchasesSumTotal(Yad3 yad3) {
	assert(yad3);
	int counter = 0;
	MAP_FOREACH(Customer,i,CUSTOMERS)
	{
		Customer customer = mapGet(CUSTOMERS, i);
		counter += customerGetPurchasesSum(customer);
	}
	return counter;
}

Yad3Result yad3ReportMostPayingCustomers(Yad3 yad3, int count,
		FILE* outputChannel) {
	if (!yad3 || !COUNT_IS_VALID || !outputChannel) {
		return YAD3_INVALID_PARAMETERS;
	}
	List list = createSortedListOfMostPayingCustomers(yad3);
	if (!list) {
		return YAD3_OUT_OF_MEMORY;
	}
	printListToCount(list, count, outputChannel, CUSTOMER);
	listDestroy(list);
	return YAD3_SUCCESS;
}

Yad3Result yad3ReportRelevantRealtors(Yad3 yad3, char* customer_email,
		FILE* outputChannel) {
	if (!yad3 || !emailIsValid(customer_email) || !outputChannel) {
		return YAD3_INVALID_PARAMETERS;
	}
	if (!emailAlreadyExists(yad3, customer_email)) {
		return YAD3_EMAIL_DOES_NOT_EXIST;
	}
	if (!emailExistsInCustomers(yad3, customer_email)) {
		return YAD3_EMAIL_WRONG_ACCOUNT_TYPE;
	}
	List list = createSortedListOfRelevantRealtors(yad3, customer_email);
	if (!list) {
		return YAD3_OUT_OF_MEMORY;
	}
	printListToCount(list, listGetSize(list), outputChannel, REALTOR);
	listDestroy(list);
	return YAD3_SUCCESS;
}

Yad3Result yad3ReportSignificantRealtors(Yad3 yad3, int count,
		FILE* outputChannel) {
	if (!yad3 || !COUNT_IS_VALID || !outputChannel) {
		return YAD3_INVALID_PARAMETERS;
	}
	List list = createSortedListOfSignificantRealtors(yad3);
	if (!list) {
		return YAD3_OUT_OF_MEMORY;
	}
	printListToCount(list, count, outputChannel, REALTOR);
	listDestroy(list);
	return YAD3_SUCCESS;
}
