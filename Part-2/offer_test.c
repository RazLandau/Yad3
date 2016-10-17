/*
 * offer_test.c
 *
 *  Created on: 20 באפר 2016
 *      Author: Raz
 */

#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "test_utilities.h"
#include "offer.h"


static bool testOfferCreate() {

	OfferResult offer_result;
	Offer offer;
	offer = offerCreate(TEST_CUSTOMER_EMAIL1, TEST_REALTOR_EMAIL1,
	TEST_SERVICE_NAME1, TEST_APARTMENT_ID1, TEST_PRICE1, NULL); // NULL offer result
	ASSERT_TEST(!offer);
	offer = offerCreate(NULL, TEST_REALTOR_EMAIL1,
	TEST_SERVICE_NAME1, TEST_APARTMENT_ID1, TEST_PRICE1, &offer_result);
	ASSERT_TEST(!offer); // NULL email
	ASSERT_TEST(offer_result == OFFER_INVALID_PARAMETERS);
	offer = offerCreate(TEST_CUSTOMER_EMAIL1, NULL,
	TEST_SERVICE_NAME1, TEST_APARTMENT_ID1, TEST_PRICE1, &offer_result);
	ASSERT_TEST(!offer); // NULL realtor email
	ASSERT_TEST(offer_result == OFFER_INVALID_PARAMETERS);
	offer = offerCreate(TEST_CUSTOMER_EMAIL1, TEST_REALTOR_EMAIL1,
	NULL, TEST_APARTMENT_ID1, TEST_PRICE1, &offer_result); // NULL service name
	ASSERT_TEST(!offer);
	ASSERT_TEST(offer_result == OFFER_INVALID_PARAMETERS);
	offer = offerCreate(TEST_BAD_EMAIL1, TEST_REALTOR_EMAIL1,
	TEST_SERVICE_NAME1, TEST_APARTMENT_ID1, TEST_PRICE1, &offer_result); // bad customer email
	ASSERT_TEST(!offer);
	ASSERT_TEST(offer_result == OFFER_INVALID_PARAMETERS);
	offer = offerCreate(TEST_BAD_EMAIL2, TEST_REALTOR_EMAIL1,
	TEST_SERVICE_NAME1, TEST_APARTMENT_ID1, TEST_PRICE1, &offer_result); // bad customer email
	ASSERT_TEST(!offer);
	ASSERT_TEST(offer_result == OFFER_INVALID_PARAMETERS);
	offer = offerCreate(TEST_CUSTOMER_EMAIL1, TEST_BAD_EMAIL1,
	TEST_SERVICE_NAME1, TEST_APARTMENT_ID1, TEST_PRICE1, &offer_result); // bad realtor email
	ASSERT_TEST(!offer);
	ASSERT_TEST(offer_result == OFFER_INVALID_PARAMETERS);
	offer = offerCreate(TEST_CUSTOMER_EMAIL1, TEST_BAD_EMAIL2,
	TEST_SERVICE_NAME1, TEST_APARTMENT_ID1, TEST_PRICE1, &offer_result); // bad realtor email
	ASSERT_TEST(!offer);
	ASSERT_TEST(offer_result == OFFER_INVALID_PARAMETERS);
	offer = offerCreate(TEST_CUSTOMER_EMAIL1, TEST_REALTOR_EMAIL1,
	TEST_SERVICE_NAME1, TEST_BAD_APARTMENT_ID, TEST_PRICE1, &offer_result); // bad id
	ASSERT_TEST(!offer);
	ASSERT_TEST(offer_result == OFFER_INVALID_PARAMETERS);
	offer = offerCreate(TEST_CUSTOMER_EMAIL1, TEST_REALTOR_EMAIL1,
	TEST_SERVICE_NAME1, TEST_APARTMENT_ID1, TEST_BAD_OFFER, &offer_result); // bad price
	ASSERT_TEST(!offer);
	ASSERT_TEST(offer_result == OFFER_INVALID_PARAMETERS);
	offer = offerCreate(TEST_CUSTOMER_EMAIL1, TEST_REALTOR_EMAIL1,
	TEST_SERVICE_NAME1, TEST_APARTMENT_ID1, TEST_PRICE1, &offer_result); // SUCCESS!
	ASSERT_TEST(offer);
	ASSERT_TEST(offer_result == OFFER_SUCCESS);

	offerDestroy(offer);
	return true;

}


static bool testOfferCopy() {

	OfferResult offer_result;
	Offer offer = offerCreate(TEST_CUSTOMER_EMAIL1, TEST_REALTOR_EMAIL1,
			TEST_SERVICE_NAME1, TEST_APARTMENT_ID1, TEST_PRICE1, &offer_result);
	assert(offer);
	assert(offer_result == OFFER_SUCCESS);

	Offer offer_copy;
	offer_copy = offerCopy(offer, NULL); // NULL offer result
	ASSERT_TEST(!offer_copy);
	offer_copy = offerCopy(NULL, &offer_result); // NULL offer
	ASSERT_TEST(!offer_copy);
	ASSERT_TEST(offer_result == OFFER_INVALID_PARAMETERS);
	offer_copy = offerCopy(offer, &offer_result); // SUCCESS!
	ASSERT_TEST(offer_copy != NULL);
	ASSERT_TEST(offer_result == OFFER_SUCCESS);

	offerDestroy(offer);
	offerDestroy(offer_copy);
	return true;
}


static bool testOfferCompareByEmail() {
	OfferResult offer_result;
	Offer offer3 = offerCreate(TEST_CUSTOMER_EMAIL1, TEST_REALTOR_EMAIL1,
	TEST_SERVICE_NAME1,
	TEST_APARTMENT_ID1, TEST_PRICE1, &offer_result);
	assert(offer_result == OFFER_SUCCESS);
	Offer offer4 = offerCreate(TEST_CUSTOMER_EMAIL1, TEST_REALTOR_EMAIL1,
	TEST_SERVICE_NAME1, TEST_APARTMENT_ID1, TEST_PRICE1, &offer_result);
	assert(offer_result == OFFER_SUCCESS);
	Offer offer5 = offerCreate(TEST_CUSTOMER_EMAIL2, TEST_REALTOR_EMAIL1,
	TEST_SERVICE_NAME1, TEST_APARTMENT_ID1, TEST_PRICE1, &offer_result);
	assert(offer_result == OFFER_SUCCESS);
	Offer offer6 = offerCreate(TEST_CUSTOMER_EMAIL2, TEST_REALTOR_EMAIL2,
	TEST_SERVICE_NAME1, TEST_APARTMENT_ID1, TEST_PRICE1, &offer_result);
	assert(offer_result == OFFER_SUCCESS);

	ASSERT_TEST(offerCompareByEmail(offer3, offer4)); // same
	ASSERT_TEST(!offerCompareByEmail(offer3, offer5)); // different customer email
	ASSERT_TEST(!offerCompareByEmail(offer3, offer6)); // different realtor email
	ASSERT_TEST(!offerCompareByEmail(offer4, offer5)); // both different
	assert(offer_result == OFFER_SUCCESS);

	offerDestroy(offer3);
	offerDestroy(offer4);
	offerDestroy(offer5);
	offerDestroy(offer6);
	return true;
}


static bool testOfferGetters(){
	OfferResult offer_result;
	Offer offer = offerCreate(TEST_CUSTOMER_EMAIL1, TEST_REALTOR_EMAIL1,
			TEST_SERVICE_NAME1, TEST_APARTMENT_ID1, TEST_PRICE1, &offer_result);
	assert(offer);
	assert(offer_result == OFFER_SUCCESS);

	/* offerGetCustomerEmail */
	char* customer_email = offerGetCustomerEmail(offer);
	ASSERT_TEST(strcmp(customer_email, TEST_CUSTOMER_EMAIL1)==0);
	free(customer_email);

	/* offerGetRealtorEmail */
	char* realtor_email = offerGetRealtorEmail(offer);
	ASSERT_TEST(strcmp(realtor_email, TEST_REALTOR_EMAIL1)==0);
	free(realtor_email);

	/* offerGetServiceName */
	char* service_name = offerGetServiceName(offer);
	ASSERT_TEST(strcmp(service_name, TEST_SERVICE_NAME1)==0);
	free(service_name);

	/* offerGetApartmentId */
	ASSERT_TEST(offerGetApartmentId(offer) == TEST_APARTMENT_ID1);

	/* offerGetPrice */
	ASSERT_TEST(offerGetPrice(offer) == TEST_PRICE1);

	offerDestroy(offer);

	return true;
}


int main() {
	RUN_TEST(testOfferCreate);
	RUN_TEST(testOfferCopy);
	RUN_TEST(testOfferCompareByEmail);
	RUN_TEST(testOfferGetters);

	return 0;
}
