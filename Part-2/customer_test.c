/*
 * customer_test.c
 *
 *  Created on: Apr 19, 2016
 *      Author: Lioz
 */

#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "test_utilities.h"
#include "customer.h"

static bool testCustomerCreate() {
	CustomerResult customer_result;
	Customer customer;
	customer = customerCreate(TEST_CUSTOMER_EMAIL1, TEST_MIN_AREA1,
			TEST_MIN_ROOMS1, TEST_MAX_PRICE1, NULL); // NULL customer result
	ASSERT_TEST(!customer);
	customer = customerCreate(NULL, TEST_MIN_AREA1, TEST_MIN_ROOMS1,
	TEST_MAX_PRICE1, &customer_result); // NULL customer email
	ASSERT_TEST(!customer);
	ASSERT_TEST(customer_result == CUSTOMER_INVALID_PARAMETERS);
	customer = customerCreate(TEST_CUSTOMER_EMAIL1, TEST_BAD_MIN_AREA,
	TEST_MIN_ROOMS1, TEST_MAX_PRICE1, &customer_result); // bad min area
	ASSERT_TEST(!customer);
	ASSERT_TEST(customer_result == CUSTOMER_INVALID_PARAMETERS);
	customer = customerCreate(TEST_CUSTOMER_EMAIL1, TEST_MIN_AREA1,
	TEST_BAD_MIN_ROOMS, TEST_MAX_PRICE1, &customer_result); // bad min rooms
	ASSERT_TEST(!customer);
	ASSERT_TEST(customer_result == CUSTOMER_INVALID_PARAMETERS);
	customer = customerCreate(TEST_CUSTOMER_EMAIL1, TEST_MIN_AREA1,
	TEST_MIN_ROOMS1, TEST_BAD_MAX_PRICE, &customer_result); // bad max price
	ASSERT_TEST(!customer);
	ASSERT_TEST(customer_result == CUSTOMER_INVALID_PARAMETERS);
	customer = customerCreate(TEST_BAD_EMAIL1, TEST_MIN_AREA1,
	TEST_MIN_ROOMS1, TEST_MAX_PRICE1, &customer_result); // bad email
	ASSERT_TEST(!customer);
	ASSERT_TEST(customer_result == CUSTOMER_INVALID_PARAMETERS);
	customer = customerCreate(TEST_BAD_EMAIL2, TEST_MIN_AREA1,
	TEST_MIN_ROOMS1, TEST_MAX_PRICE1, &customer_result); // bad email
	ASSERT_TEST(!customer);
	ASSERT_TEST(customer_result == CUSTOMER_INVALID_PARAMETERS);
	customer = customerCreate(TEST_CUSTOMER_EMAIL1, TEST_MIN_AREA1,
	TEST_MIN_ROOMS1, TEST_MAX_PRICE1, &customer_result); // SUCCESS!
	ASSERT_TEST(customer);
	ASSERT_TEST(customer_result == CUSTOMER_SUCCESS);

	customerDestroy(customer);
	return true;
}

static bool testCustomerCopy() {

	CustomerResult customer_result;
	Customer customer = customerCreate(TEST_CUSTOMER_EMAIL1, TEST_MIN_AREA1,
	TEST_MIN_ROOMS1, TEST_MAX_PRICE1, &customer_result);
	assert(customer);
	assert(customer_result == CUSTOMER_SUCCESS);

	Customer customer_copy;
	customer_copy = customerCopy(customer, NULL); //NULL customer result
	ASSERT_TEST(!customer_copy);
	customer_copy = customerCopy(NULL, &customer_result); //NULL customer
	ASSERT_TEST(!customer_copy);
	ASSERT_TEST(customer_result == CUSTOMER_INVALID_PARAMETERS);
	customer_copy = customerCopy(customer, &customer_result); // SUCCESS!
	ASSERT_TEST(customer);
	ASSERT_TEST(customer_result == CUSTOMER_SUCCESS);

	customerDestroy(customer);
	customerDestroy(customer_copy);
	return true;
}

static bool testCustomerGettersAndCustomerAddPurchase() {

	CustomerResult customer_result;
	Customer customer = customerCreate(TEST_CUSTOMER_EMAIL1, TEST_MIN_AREA1,
	TEST_MIN_ROOMS1, TEST_MAX_PRICE1, &customer_result);
	assert(customer);
	assert(customer_result == CUSTOMER_SUCCESS);

	/* customerGetMinArea */
	ASSERT_TEST(customerGetMinArea(customer) == TEST_MIN_AREA1);

	/* customerGetMinRooms */
	ASSERT_TEST(customerGetMinRooms(customer) == TEST_MIN_ROOMS1);

	/* customerGetMaxPrice */
	ASSERT_TEST(customerGetMaxPrice(customer) == TEST_MAX_PRICE1);

	/* customerGetEmail */
	char* customer_email = customerGetEmail(customer);
	ASSERT_TEST(strcmp(customer_email, TEST_CUSTOMER_EMAIL1)==0);
	ASSERT_TEST(customerGetPurchasesSum(customer) == 0);

	/* customerAddPurchase */
	customerAddPurchase(customer, 100);
	ASSERT_TEST(customerGetPurchasesSum(customer) == 100);

	free(customer_email);
	customerDestroy(customer);

	return true;
}

int main() {
	RUN_TEST(testCustomerCreate);
	RUN_TEST(testCustomerCopy);
	RUN_TEST(testCustomerGettersAndCustomerAddPurchase);
	return 0;
}
