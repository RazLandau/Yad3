/*
 * realtor_test.c
 *
 *  Created on: 16 באפר 2016
 *      Author: Raz
 */

#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "test_utilities.h"
#include "realtor.h"

#define TEST_REALTOR_RANK1 (1000000*1 + TEST_PRICE1 + 100000*TEST_TOTAL_AREA1)


static bool testRealtorCreate() {
	RealtorResult realtor_result;
	Realtor realtor;

	realtor = realtorCreate(NULL, TEST_COMPANY_NAME1, TEST_PRICE1,
			&realtor_result); // NULL realtor
	ASSERT_TEST(!realtor);
	ASSERT_TEST(realtor_result == REALTOR_INVALID_PARAMETERS); // NULL company name
	realtor = realtorCreate(TEST_REALTOR_EMAIL1, NULL, TEST_PRICE1,
			&realtor_result); // NULL company name
	ASSERT_TEST(!realtor);
	ASSERT_TEST(realtor_result == REALTOR_INVALID_PARAMETERS);
	realtor = realtorCreate(TEST_REALTOR_EMAIL1, TEST_COMPANY_NAME1,
	TEST_BAD_TAX_PERCENTAGE1, &realtor_result); // bad tax percentage
	ASSERT_TEST(!realtor);
	ASSERT_TEST(realtor_result == REALTOR_INVALID_PARAMETERS);
	realtor = realtorCreate(TEST_REALTOR_EMAIL1, TEST_COMPANY_NAME1,
	TEST_BAD_TAX_PERCENTAGE2, &realtor_result); // bad tax percentage
	ASSERT_TEST(!realtor);
	ASSERT_TEST(realtor_result == REALTOR_INVALID_PARAMETERS);
	realtor = realtorCreate(TEST_BAD_EMAIL1, TEST_COMPANY_NAME1, TEST_PRICE1,
			&realtor_result); // bad email
	ASSERT_TEST(!realtor);
	ASSERT_TEST(realtor_result == REALTOR_INVALID_PARAMETERS);
	realtor = realtorCreate(TEST_BAD_EMAIL2, TEST_COMPANY_NAME1, TEST_PRICE1,
			&realtor_result); // bad email
	ASSERT_TEST(!realtor);
	ASSERT_TEST(realtor_result == REALTOR_INVALID_PARAMETERS);
	realtor = realtorCreate(TEST_REALTOR_EMAIL1, TEST_COMPANY_NAME1,
	TEST_PRICE1, &realtor_result); // SUCCESS!
	ASSERT_TEST(realtor);
	ASSERT_TEST(realtor_result == REALTOR_SUCCESS);

	realtorDestroy(NULL);
	realtorDestroy(realtor);

	return true;
}


static bool testRealtorCopy() {

	Realtor realtor_copy;
	RealtorResult realtor_result;

	Realtor realtor = realtorCreate(TEST_REALTOR_EMAIL1, TEST_COMPANY_NAME1,
	TEST_PRICE1, &realtor_result);
	assert(realtor_result == REALTOR_SUCCESS);
	realtorAddService(realtor, TEST_SERVICE_NAME1, 1);
	assert(realtorNumOfServices(realtor) == 1);

	realtor_copy = realtorCopy(NULL, &realtor_result); // NULL realtor
	ASSERT_TEST(!realtor_copy);
	ASSERT_TEST(realtor_result == REALTOR_INVALID_PARAMETERS);
	realtor_copy = realtorCopy(realtor, &realtor_result); // SUCCESS!
	ASSERT_TEST(realtor_copy);
	ASSERT_TEST(realtor_result == REALTOR_SUCCESS);
	ASSERT_TEST(realtorNumOfServices(realtor_copy) == 1);
	// changes in copy doesn't affect source:
	realtorRemoveService(realtor_copy, TEST_SERVICE_NAME1);
	ASSERT_TEST(realtorNumOfServices(realtor_copy) == 0);
	ASSERT_TEST(realtorNumOfServices(realtor) == 1);

	realtorDestroy(realtor);
	realtorDestroy(realtor_copy);

	return true;
}


static bool testRealtorAddService() {

	RealtorResult realtor_result;

	Realtor realtor = realtorCreate(TEST_REALTOR_EMAIL1, TEST_COMPANY_NAME1,
	TEST_PRICE1, &realtor_result);
	assert(realtor);
	assert(realtorNumOfServices(realtor) == 0);

	realtor_result = realtorAddService(NULL, TEST_SERVICE_NAME1, 1); // NULL realtor
	ASSERT_TEST(realtor_result == REALTOR_INVALID_PARAMETERS);
	realtor_result = realtorAddService(realtor, NULL, 1); // NULL service name
	ASSERT_TEST(realtor_result == REALTOR_INVALID_PARAMETERS);
	realtor_result = realtorAddService(realtor, TEST_SERVICE_NAME1, 0); // bad tax percentage
	ASSERT_TEST(realtor_result == REALTOR_INVALID_PARAMETERS);
	ASSERT_TEST(realtorNumOfServices(realtor) == 0);
	realtor_result = realtorAddService(realtor, TEST_SERVICE_NAME1, 1); // SUCCESS!
	ASSERT_TEST(realtor_result == REALTOR_SUCCESS);
	ASSERT_TEST(realtorNumOfServices(realtor) == 1);
	realtor_result = realtorAddService(realtor, TEST_SERVICE_NAME1, 1); // existed service
	ASSERT_TEST(realtor_result == REALTOR_SERVICE_ALREADY_EXISTS);
	ASSERT_TEST(realtorNumOfServices(realtor) == 1);

	realtorDestroy(realtor);

	return true;
}


static bool testRealtorRemoveService() {

	RealtorResult realtor_result;

	Realtor realtor = realtorCreate(TEST_REALTOR_EMAIL1, TEST_COMPANY_NAME1,
	TEST_PRICE1, &realtor_result);
	assert(realtor_result == REALTOR_SUCCESS);
	realtor_result = realtorAddService(realtor, TEST_SERVICE_NAME1, 1);
	assert(realtorNumOfServices(realtor) == 1);

	realtor_result = realtorRemoveService(NULL, TEST_SERVICE_NAME1); // NULL realtor
	ASSERT_TEST(realtor_result == REALTOR_INVALID_PARAMETERS);
	realtor_result = realtorRemoveService(realtor, NULL); // NULL service name
	ASSERT_TEST(realtor_result == REALTOR_INVALID_PARAMETERS);
	realtor_result = realtorRemoveService(realtor, TEST_ABSENT_STRING); // service doesn't exit
	ASSERT_TEST(realtor_result == REALTOR_SERVICE_DOES_NOT_EXIST);
	ASSERT_TEST(realtorNumOfServices(realtor) == 1);
	realtor_result = realtorRemoveService(realtor, TEST_SERVICE_NAME1); // SUCCESS!
	ASSERT_TEST(realtor_result == REALTOR_SUCCESS);
	ASSERT_TEST(realtorNumOfServices(realtor) == 0);

	realtorDestroy(realtor);

	return true;

}


static bool testrealtorAddApartemt() {

	RealtorResult realtor_result;

	Realtor realtor = realtorCreate(TEST_REALTOR_EMAIL1, TEST_COMPANY_NAME1,
	TEST_PRICE1, &realtor_result);
	assert(realtor_result == REALTOR_SUCCESS);
	realtor_result = realtorAddService(realtor, TEST_SERVICE_NAME1, 2);
	assert(realtorNumOfServices(realtor) == 1);
	assert(realtorNumOfApartments(realtor) == 0);

	realtor_result = realtorAddApartment(realtor, TEST_SERVICE_NAME1,
	TEST_APARTMENT_ID1, TEST_PRICE1, TEST_WIDTH1, TEST_HEIGHT1,
	TEST_APARTMENT1); // SUCCESS!
	ASSERT_TEST(realtor_result == REALTOR_SUCCESS);
	ASSERT_TEST(realtorNumOfApartments(realtor) == 1);
	realtor_result = realtorAddApartment(NULL, TEST_SERVICE_NAME1,
	TEST_APARTMENT_ID1, TEST_PRICE1, TEST_WIDTH1, TEST_HEIGHT1,
	TEST_APARTMENT1); // NULL realtor
	ASSERT_TEST(realtor_result == REALTOR_INVALID_PARAMETERS);
	realtor_result = realtorAddApartment(realtor, NULL, TEST_APARTMENT_ID1,
	TEST_PRICE1,
	TEST_WIDTH1, TEST_HEIGHT1, TEST_APARTMENT1); // NULL service name
	ASSERT_TEST(realtor_result == REALTOR_INVALID_PARAMETERS);
	realtor_result = realtorAddApartment(realtor, TEST_SERVICE_NAME1,
	TEST_APARTMENT_ID1,
	TEST_PRICE1, TEST_WIDTH1, TEST_HEIGHT1, NULL); // NULL matrix
	ASSERT_TEST(realtor_result == REALTOR_INVALID_PARAMETERS);
	realtor_result = realtorAddApartment(realtor, TEST_SERVICE_NAME1,
	TEST_BAD_APARTMENT_ID, TEST_PRICE1, TEST_WIDTH1, TEST_HEIGHT1, NULL); // bad apartment id
	ASSERT_TEST(realtor_result == REALTOR_INVALID_PARAMETERS);
	realtor_result = realtorAddApartment(realtor, TEST_SERVICE_NAME1,
	TEST_APARTMENT_ID1,
	TEST_BAD_PRICE1, TEST_WIDTH1, TEST_HEIGHT1, TEST_APARTMENT1); // bad price
	ASSERT_TEST(realtor_result == REALTOR_INVALID_PARAMETERS);
	realtor_result = realtorAddApartment(realtor, TEST_SERVICE_NAME1,
	TEST_APARTMENT_ID1,
	TEST_BAD_PRICE2, TEST_WIDTH1, TEST_HEIGHT1, TEST_APARTMENT1); // bad price
	ASSERT_TEST(realtor_result == REALTOR_INVALID_PARAMETERS);
	realtor_result = realtorAddApartment(realtor, TEST_SERVICE_NAME1,
	TEST_APARTMENT_ID1,
	TEST_PRICE1, TEST_BAD_WIDTH, TEST_HEIGHT1, TEST_APARTMENT1); // bad width
	ASSERT_TEST(realtor_result == REALTOR_INVALID_PARAMETERS);
	realtor_result = realtorAddApartment(realtor, TEST_SERVICE_NAME1,
	TEST_APARTMENT_ID1,
	TEST_PRICE1, TEST_WIDTH1, TEST_BAD_HEIGHT, TEST_APARTMENT1); // bad height
	ASSERT_TEST(realtor_result == REALTOR_INVALID_PARAMETERS);
	realtor_result = realtorAddApartment(realtor, TEST_SERVICE_NAME1,
	TEST_APARTMENT_ID1,
	TEST_PRICE1, TEST_WIDTH1, TEST_HEIGHT1, TEST_BAD_APARTMENT1); // bad apartment
	ASSERT_TEST(realtor_result == REALTOR_INVALID_PARAMETERS);
	realtor_result = realtorAddApartment(realtor, TEST_SERVICE_NAME1,
	TEST_APARTMENT_ID1,
	TEST_PRICE1, TEST_WIDTH1, TEST_HEIGHT1, TEST_BAD_APARTMENT2); // bad apartment
	ASSERT_TEST(realtor_result == REALTOR_INVALID_PARAMETERS);
	realtor_result = realtorAddApartment(realtor, TEST_SERVICE_NAME1,
	TEST_APARTMENT_ID1,
	TEST_PRICE1, TEST_WIDTH1, TEST_HEIGHT1, TEST_BAD_APARTMENT3); // bad apartment
	ASSERT_TEST(realtor_result == REALTOR_INVALID_PARAMETERS);
	realtor_result = realtorAddApartment(realtor, TEST_SERVICE_NAME1,
	TEST_APARTMENT_ID1,
	TEST_PRICE1, TEST_WIDTH1, TEST_HEIGHT1, TEST_APARTMENT1); // existed ID
	ASSERT_TEST(realtor_result == REALTOR_APARTMENT_ALREADY_EXISTS);
	ASSERT_TEST(realtorNumOfApartments(realtor) == 1);
	realtorAddApartment(realtor, TEST_SERVICE_NAME1, TEST_APARTMENT_ID2,
	TEST_PRICE1, TEST_WIDTH1,
	TEST_HEIGHT1, TEST_APARTMENT1);
	assert(realtorNumOfApartments(realtor) == 2);
	realtor_result = realtorAddApartment(realtor, TEST_SERVICE_NAME1,
	TEST_APARTMENT_ID1,
	TEST_PRICE1, TEST_WIDTH1, TEST_HEIGHT1, TEST_APARTMENT1); // service full

	realtorDestroy(realtor);

	return true;
}


static bool testRealtorRemoveApartemt() {

	RealtorResult realtor_result;

	Realtor realtor = realtorCreate(TEST_REALTOR_EMAIL1, TEST_COMPANY_NAME1,
	TEST_PRICE1, &realtor_result);
	assert(realtor_result == REALTOR_SUCCESS);
	realtorAddService(realtor, TEST_SERVICE_NAME1, 1);
	assert(realtorNumOfServices(realtor) == 1);
	realtorAddApartment(realtor, TEST_SERVICE_NAME1, TEST_APARTMENT_ID1,
	TEST_PRICE1, TEST_WIDTH1,
	TEST_HEIGHT1, TEST_APARTMENT1);
	assert(realtorNumOfApartments(realtor) == 1);

	realtor_result = realtorRemoveApartment(realtor, TEST_SERVICE_NAME1,
	TEST_APARTMENT_ID1); // SUCCESS!
	ASSERT_TEST(realtor_result == REALTOR_SUCCESS);
	realtor_result = realtorRemoveApartment(NULL, TEST_SERVICE_NAME1,
	TEST_APARTMENT_ID1); // NULL realtor
	ASSERT_TEST(realtor_result == REALTOR_INVALID_PARAMETERS);
	realtor_result = realtorRemoveApartment(realtor, NULL, TEST_APARTMENT_ID1); // NULL service name
	ASSERT_TEST(realtor_result == REALTOR_INVALID_PARAMETERS);
	realtor_result = realtorRemoveApartment(realtor, TEST_SERVICE_NAME1,
	TEST_BAD_APARTMENT_ID); // bad id
	ASSERT_TEST(realtor_result == REALTOR_INVALID_PARAMETERS);
	realtor_result = realtorRemoveApartment(realtor, TEST_ABSENT_STRING,
	TEST_APARTMENT_ID1); // service doesn't exist
	ASSERT_TEST(realtor_result == REALTOR_SERVICE_DOES_NOT_EXIST);
	realtor_result = realtorRemoveApartment(realtor, TEST_SERVICE_NAME1,
	TEST_APARTMENT_ID1); // apartment doesn't exit
	ASSERT_TEST(realtor_result == REALTOR_APARTMENT_DOES_NOT_EXIST);

	realtorDestroy(realtor);

	return true;
}


static bool testRealtorGetApartmentById() {

	RealtorResult realtor_result;
	Apartment apartment;

	Realtor realtor = realtorCreate(TEST_REALTOR_EMAIL1, TEST_COMPANY_NAME1,
	TEST_PRICE1, &realtor_result);
	assert(realtor_result == REALTOR_SUCCESS);
	realtor_result = realtorAddService(realtor, TEST_SERVICE_NAME1, 1);
	assert(realtor_result == REALTOR_SUCCESS);
	assert(realtorNumOfServices(realtor) == 1);
	realtor_result = realtorAddApartment(realtor, TEST_SERVICE_NAME1,
	TEST_APARTMENT_ID1, TEST_PRICE1, TEST_WIDTH1, TEST_HEIGHT1,
	TEST_APARTMENT1);
	assert(realtor_result == REALTOR_SUCCESS);
	assert(realtorNumOfApartments(realtor) == 1);

	apartment = realtorGetApartmentById(realtor, TEST_SERVICE_NAME1,
	TEST_APARTMENT_ID1,
	NULL); // NULL realtor result
	ASSERT_TEST(!apartment);
	apartment = realtorGetApartmentById(NULL, TEST_SERVICE_NAME1,
	TEST_APARTMENT_ID1, &realtor_result); // NULL realtor
	ASSERT_TEST(!apartment);
	ASSERT_TEST(realtor_result == REALTOR_INVALID_PARAMETERS);
	apartment = realtorGetApartmentById(realtor, NULL, TEST_APARTMENT_ID1,
			&realtor_result); // NULL service name
	ASSERT_TEST(!apartment);
	ASSERT_TEST(realtor_result == REALTOR_INVALID_PARAMETERS);
	apartment = realtorGetApartmentById(realtor, TEST_SERVICE_NAME1,
	TEST_BAD_APARTMENT_ID, &realtor_result); // bad id
	ASSERT_TEST(!apartment);
	ASSERT_TEST(realtor_result == REALTOR_INVALID_PARAMETERS);
	apartment = realtorGetApartmentById(realtor, TEST_ABSENT_STRING,
	TEST_APARTMENT_ID1, &realtor_result); // service doesn't exist
	ASSERT_TEST(!apartment);
	ASSERT_TEST(realtor_result == REALTOR_SERVICE_DOES_NOT_EXIST);
	apartment = realtorGetApartmentById(realtor, TEST_SERVICE_NAME1,
	TEST_APARTMENT_ID2, &realtor_result); // apartment doesn't exist
	ASSERT_TEST(!apartment);
	ASSERT_TEST(realtor_result == REALTOR_APARTMENT_DOES_NOT_EXIST);
	apartment = realtorGetApartmentById(realtor, TEST_SERVICE_NAME1,
	TEST_APARTMENT_ID1, &realtor_result); // SUCCESS!
	ASSERT_TEST(apartment);
	ASSERT_TEST(realtor_result == REALTOR_SUCCESS);
	ASSERT_TEST(apartmentGetPrice(apartment) == 100);
	ASSERT_TEST(apartmentTotalArea(apartment) == 2);
	ASSERT_TEST(apartmentNumOfRooms(apartment) == 2);

	apartmentDestroy(apartment);
	realtorDestroy(realtor);

	return true;
}


static bool testRealtorApartmentExists() {

	RealtorResult realtor_result;
	Realtor realtor = realtorCreate(TEST_REALTOR_EMAIL1, TEST_COMPANY_NAME1,
	TEST_PRICE1, &realtor_result);
	assert(realtor_result == REALTOR_SUCCESS);
	realtor_result = realtorAddService(realtor, TEST_SERVICE_NAME1, 1);
	assert(realtor_result == REALTOR_SUCCESS);
	assert(realtorNumOfServices(realtor) == 1);
	realtor_result = realtorAddApartment(realtor, TEST_SERVICE_NAME1,
	TEST_APARTMENT_ID1, TEST_PRICE1, TEST_WIDTH1, TEST_HEIGHT1,
	TEST_APARTMENT1);
	assert(realtor_result == REALTOR_SUCCESS);
	assert(realtorNumOfApartments(realtor) == 1);

	bool result;
	result = realtorApartmentExists(realtor, TEST_TOTAL_AREA1 + 1,
	TEST_NUM_OF_ROOMS1, TEST_PRICE1); // total area too high
	ASSERT_TEST(result == false);
	result = realtorApartmentExists(realtor, TEST_TOTAL_AREA1,
	TEST_NUM_OF_ROOMS1 + 1, TEST_PRICE1); // num of rooms too high
	ASSERT_TEST(result == false);
	result = realtorApartmentExists(realtor, TEST_TOTAL_AREA1,
	TEST_NUM_OF_ROOMS1, TEST_PRICE1 - 1); // price too low
	ASSERT_TEST(result == false);
	result = realtorApartmentExists(realtor, TEST_TOTAL_AREA1,
			TEST_NUM_OF_ROOMS1, TEST_PRICE1); // SUCCESS!
	ASSERT_TEST(result == true);

	realtorDestroy(realtor);
	return true;
}


static bool testRealtorGetters() {

	RealtorResult realtor_result;
	Realtor realtor = realtorCreate(TEST_REALTOR_EMAIL1, TEST_COMPANY_NAME1,
	TEST_PRICE1, &realtor_result);
	assert(realtor_result == REALTOR_SUCCESS);

	/* realtorGetEmail */
	char* realtor_email = realtorGetEmail(realtor);
	ASSERT_TEST(strcmp(realtor_email, TEST_REALTOR_EMAIL1) == 0);
	free(realtor_email);

	/* realtorGetTaxPercentage */
	ASSERT_TEST(realtorGetTaxPercentage(realtor) == TEST_TAX_PERCENTAGE1);

	/* realtorNumOfServices */
	ASSERT_TEST(realtorNumOfServices(realtor) == 0);
	realtor_result = realtorAddService(realtor, TEST_SERVICE_NAME1, 1);
	assert(realtor_result == REALTOR_SUCCESS);
	ASSERT_TEST(realtorNumOfServices(realtor) == 1);

	/* realtorNumOfApartments */
	ASSERT_TEST(realtorNumOfApartments(realtor) == 0);
	realtor_result = realtorAddApartment(realtor, TEST_SERVICE_NAME1,
	TEST_APARTMENT_ID1, TEST_PRICE1, TEST_WIDTH1, TEST_HEIGHT1,
	TEST_APARTMENT1);
	assert(realtor_result == REALTOR_SUCCESS);
	ASSERT_TEST(realtorNumOfApartments(realtor) == 1);

	/* realtorGetRank */
	ASSERT_TEST(realtorGetRank(realtor) == TEST_REALTOR_RANK1);
	realtor_result = realtorRemoveApartment(realtor, TEST_SERVICE_NAME1,
	TEST_APARTMENT_ID1);
	assert(realtor_result == REALTOR_SUCCESS);
	ASSERT_TEST(realtorGetRank(realtor) == 0);

	realtorDestroy(realtor);
	return true;
}


int main() {
	RUN_TEST(testRealtorCreate);
	RUN_TEST(testRealtorCopy);
	RUN_TEST(testRealtorAddService);
	RUN_TEST(testRealtorRemoveService);
	RUN_TEST(testrealtorAddApartemt);
	RUN_TEST(testRealtorRemoveApartemt);
	RUN_TEST(testRealtorGetApartmentById);
	RUN_TEST(testRealtorApartmentExists);
	RUN_TEST(testRealtorGetters);
	return 0;
}

