/*
 * customer.c
 *
 *  Created on: Apr 19, 2016
 *      Author: Lioz
 */

#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "customer.h"

#define EMAIL (customer->email)
#define MIN_AREA (customer->min_area)
#define MIN_ROOMS (customer->min_rooms)
#define MAX_PRICE (customer->max_price)
#define PURCHASES_SUM (customer->purchases_sum)

/**
 * The customer structure.
 */
struct customer_t {
	char* email;
	int min_area, min_rooms, max_price, purchases_sum;
};

Customer customerCreate(char* email, int min_area, int min_rooms, int max_price,
		CustomerResult* customer_result) {
	if (!customer_result) {
		return NULL;
	}
	if (!emailIsValid(email) || !MIN_AREA_IS_VALID || !MIN_ROOMS_IS_VALID
			|| !MAX_PRICE_IS_VALID) {
		*customer_result = CUSTOMER_INVALID_PARAMETERS;
		return NULL;
	}
	Customer customer = malloc(sizeof(*customer));
	char* new_email = copyString(email);
	if (!customer || !new_email) {
		*customer_result = CUSTOMER_OUT_OF_MEMORY;
		free(customer);
		free(new_email);
		return NULL;
	}
	EMAIL = new_email;
	MAX_PRICE = max_price;
	MIN_AREA = min_area;
	MIN_ROOMS = min_rooms;
	PURCHASES_SUM = 0;
	*customer_result = CUSTOMER_SUCCESS;
	return customer;
}

void customerDestroy(Customer customer) {
	if (customer) {
		free(EMAIL);
	}
	free(customer);
}

Customer customerCopy(Customer customer, CustomerResult* customer_result) {
	if (!customer_result) {
		return NULL;
	}
	if (!customer) {
		*customer_result = CUSTOMER_INVALID_PARAMETERS;
		return NULL;
	}
	Customer new_customer = customerCreate(EMAIL,
			MIN_AREA, MIN_ROOMS, MAX_PRICE, customer_result);
	if (new_customer) {
		assert(*customer_result == CUSTOMER_SUCCESS);
		new_customer->purchases_sum = PURCHASES_SUM;
	}
	return new_customer;
}

int customerGetMinArea(Customer customer){
	assert(customer);
	return MIN_AREA;
}

int customerGetMinRooms(Customer customer){
	assert(customer);
	return MIN_ROOMS;
}

int customerGetMaxPrice(Customer customer){
	assert(customer);
	return MAX_PRICE;
}

int customerGetPurchasesSum(Customer customer){
	assert(customer);
	return PURCHASES_SUM;
}

char* customerGetEmail (Customer customer){
	assert(customer);
	return copyString(EMAIL);
}

void customerAddPurchase(Customer customer, int purchase){
	assert(customer);
	PURCHASES_SUM+=purchase;
}

void customerPrint(FILE* outputChannel, Customer customer) {
	assert(outputChannel && customer);
	mtmPrintCustomer(outputChannel, EMAIL, PURCHASES_SUM);
}

int customerCompareByEmail(Customer customer1,
		Customer customer2) {
	char* customer1_email = customer1->email;
	char* customer2_email = customer2->email;
	int compare = strcmp(customer1_email, customer2_email);
	return compare;
}

