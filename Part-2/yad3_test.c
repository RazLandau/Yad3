/*
 * yad3_test.c
 *
 *  Created on: 16 באפר 2016
 *      Author: Raz
 */

#include <stdlib.h>
#include <assert.h>

#include "test_utilities.h"
#include "yad3.h"

#define ACCEPT "accept"
#define DECLINE "decline"
#define BAD_CHOICE "bad_choice"


static bool testYad3Create() {
	Yad3 yad3 = yad3Create();
	ASSERT_TEST(yad3NumOfRealtors(yad3) == 0);
	ASSERT_TEST(yad3NumOfCustomers(yad3) == 0);
	ASSERT_TEST(yad3NumOfOffers(yad3) == 0);
	yad3Destroy(yad3);

	return true;
}


static bool testYad3AddCustomer() {
	Yad3 yad3 = yad3Create();
	yad3AddRealtor(yad3, TEST_REALTOR_EMAIL1, TEST_COMPANY_NAME1,
	TEST_TAX_PERCENTAGE1);
	assert(yad3NumOfCustomers(yad3) == 0);
	assert(yad3NumOfRealtors(yad3) == 1);
	assert(yad3NumOfOffers(yad3) == 0);

	Yad3Result yad3_result;
	yad3_result = yad3AddCustomer(yad3, TEST_CUSTOMER_EMAIL1, TEST_MIN_AREA1,
	TEST_MIN_ROOMS1, TEST_MAX_PRICE1); //SUCCES!
	ASSERT_TEST(yad3_result == YAD3_SUCCESS);
	ASSERT_TEST(yad3NumOfCustomers(yad3) == 1);
	yad3_result = yad3AddCustomer(NULL, TEST_CUSTOMER_EMAIL1, TEST_MIN_AREA1,
	TEST_MIN_ROOMS1, TEST_MAX_PRICE1);  //NULL yad3
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3AddCustomer(yad3, NULL, TEST_MIN_AREA1,
	TEST_MIN_ROOMS1, TEST_MAX_PRICE1); //NULL email
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS); // bad min area
	yad3_result = yad3AddCustomer(yad3, TEST_CUSTOMER_EMAIL1, TEST_BAD_MIN_AREA,
	TEST_MIN_ROOMS1, TEST_MAX_PRICE1);
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3AddCustomer(yad3, TEST_CUSTOMER_EMAIL1, TEST_MIN_AREA1,
	TEST_BAD_MIN_ROOMS, TEST_MAX_PRICE1); // bad min rooms
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3AddCustomer(yad3, TEST_CUSTOMER_EMAIL1, TEST_MIN_AREA1,
	TEST_MIN_ROOMS1, TEST_BAD_MAX_PRICE); // bad max price
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3AddCustomer(yad3, TEST_BAD_EMAIL1, TEST_MIN_AREA1,
	TEST_MIN_ROOMS1, TEST_MAX_PRICE1); // bad email
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3AddCustomer(yad3, TEST_BAD_EMAIL2, TEST_MIN_AREA1,
	TEST_MIN_ROOMS1, TEST_MAX_PRICE1); //bad email
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3AddCustomer(yad3, TEST_REALTOR_EMAIL1, TEST_MIN_AREA1,
	TEST_MIN_ROOMS1, TEST_MAX_PRICE1); // exists in realtors
	ASSERT_TEST(yad3_result == YAD3_EMAIL_ALREADY_EXISTS);
	yad3_result = yad3AddCustomer(yad3, TEST_CUSTOMER_EMAIL1, TEST_MIN_AREA1,
	TEST_MIN_ROOMS1, TEST_MAX_PRICE1); // exists in customers
	ASSERT_TEST(yad3_result == YAD3_EMAIL_ALREADY_EXISTS);
	ASSERT_TEST(yad3NumOfCustomers(yad3) == 1);

	yad3Destroy(yad3);
	return true;
}


static bool testYad3AddRealtor() {

	Yad3 yad3 = yad3Create();
	yad3AddCustomer(yad3, TEST_CUSTOMER_EMAIL1, TEST_MIN_AREA1, TEST_MIN_ROOMS1,
	TEST_MAX_PRICE1);
	assert(yad3NumOfRealtors(yad3) == 0);
	assert(yad3NumOfCustomers(yad3) == 1);
	assert(yad3NumOfOffers(yad3) == 0);

	Yad3Result yad3_result;
	yad3_result = yad3AddRealtor(yad3, TEST_REALTOR_EMAIL1, TEST_COMPANY_NAME1,
	TEST_TAX_PERCENTAGE1); // SUCCESS!
	ASSERT_TEST(yad3_result == YAD3_SUCCESS);
	ASSERT_TEST(yad3NumOfRealtors(yad3) == 1);
	yad3_result = yad3AddRealtor(NULL, TEST_REALTOR_EMAIL1, TEST_COMPANY_NAME1,
	TEST_TAX_PERCENTAGE1); // NULL yad3
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3AddRealtor(yad3, NULL, TEST_COMPANY_NAME1,
	TEST_TAX_PERCENTAGE1); // NULL email
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3AddRealtor(yad3, TEST_REALTOR_EMAIL1, NULL,
	TEST_TAX_PERCENTAGE1); // NULL email
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3AddRealtor(yad3, TEST_BAD_EMAIL1, TEST_COMPANY_NAME1,
	TEST_TAX_PERCENTAGE1); // bad email
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3AddRealtor(yad3, TEST_BAD_EMAIL2, TEST_COMPANY_NAME1,
	TEST_TAX_PERCENTAGE1); // bad email
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3AddRealtor(yad3, TEST_REALTOR_EMAIL1, TEST_COMPANY_NAME1,
	TEST_BAD_TAX_PERCENTAGE1); // bad tax percentage
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3AddRealtor(yad3, TEST_REALTOR_EMAIL1, TEST_COMPANY_NAME1,
	TEST_BAD_TAX_PERCENTAGE2); // bad tax percentage
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3AddRealtor(yad3, TEST_REALTOR_EMAIL1, TEST_COMPANY_NAME1,
	TEST_TAX_PERCENTAGE1); // exists in realtors
	ASSERT_TEST(yad3_result == YAD3_EMAIL_ALREADY_EXISTS);
	yad3_result = yad3AddRealtor(yad3, TEST_CUSTOMER_EMAIL1, TEST_COMPANY_NAME1,
	TEST_TAX_PERCENTAGE1); // exists in customers
	ASSERT_TEST(yad3_result == YAD3_EMAIL_ALREADY_EXISTS);

	yad3Destroy(yad3);

	return true;
}


static bool testYad3AddOffer() {

	Yad3Result yad3_result;

	Yad3 yad3 = yad3Create();
	yad3AddCustomer(yad3, TEST_CUSTOMER_EMAIL1, 1000, 1000, 1);
	yad3AddCustomer(yad3, TEST_CUSTOMER_EMAIL2, 1, 1, 10000);
	yad3AddRealtor(yad3, TEST_REALTOR_EMAIL1, TEST_COMPANY_NAME1,
	TEST_TAX_PERCENTAGE1);
	yad3AddServiceToRealtor(yad3, TEST_REALTOR_EMAIL1, TEST_SERVICE_NAME1, 1);
	yad3AddApartmentToRealtor(yad3, TEST_REALTOR_EMAIL1, TEST_SERVICE_NAME1,
	TEST_APARTMENT_ID1, TEST_PRICE1, TEST_WIDTH1,
	TEST_HEIGHT1, TEST_APARTMENT1);
	assert(yad3NumOfRealtors(yad3) == 1);
	assert(yad3NumOfServices(yad3) == 1);
	assert(yad3NumOfApartments(yad3) == 1);
	assert(yad3NumOfOffers(yad3) == 0);

	yad3_result = yad3AddOffer(NULL, TEST_CUSTOMER_EMAIL1, TEST_REALTOR_EMAIL1,
	TEST_SERVICE_NAME1, TEST_APARTMENT_ID1, TEST_OFFER1); // NULL yad3
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3AddOffer(yad3, NULL, TEST_REALTOR_EMAIL1,
	TEST_SERVICE_NAME1, TEST_APARTMENT_ID1, TEST_OFFER1); // NULL customer email
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3AddOffer(yad3, TEST_CUSTOMER_EMAIL1, NULL,
	TEST_SERVICE_NAME1, TEST_APARTMENT_ID1, TEST_OFFER1); // NULL realtor email
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3AddOffer(yad3, TEST_CUSTOMER_EMAIL1, TEST_REALTOR_EMAIL1,
	NULL, TEST_APARTMENT_ID1, TEST_OFFER1); // NULL service name
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3AddOffer(yad3, TEST_BAD_EMAIL1, TEST_REALTOR_EMAIL1,
	TEST_SERVICE_NAME1, TEST_APARTMENT_ID1, TEST_OFFER1); // bad customer email
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3AddOffer(yad3, TEST_BAD_EMAIL2, TEST_REALTOR_EMAIL1,
	TEST_SERVICE_NAME1, TEST_APARTMENT_ID1, TEST_OFFER1); // bad customer email
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3AddOffer(yad3, TEST_CUSTOMER_EMAIL1, TEST_BAD_EMAIL1,
	TEST_SERVICE_NAME1, TEST_APARTMENT_ID1, TEST_OFFER1); // bad realtor email
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3AddOffer(yad3, TEST_CUSTOMER_EMAIL1, TEST_BAD_EMAIL2,
	TEST_SERVICE_NAME1, TEST_APARTMENT_ID1, TEST_OFFER1); // bad realtor email
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3AddOffer(yad3, TEST_CUSTOMER_EMAIL1, TEST_REALTOR_EMAIL1,
	TEST_SERVICE_NAME1, TEST_BAD_APARTMENT_ID, TEST_OFFER1); // bad id
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3AddOffer(yad3, TEST_CUSTOMER_EMAIL1, TEST_REALTOR_EMAIL1,
	TEST_SERVICE_NAME1, TEST_APARTMENT_ID1, TEST_BAD_OFFER); // bad offer
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3AddOffer(yad3, TEST_ABSENT_STRING, TEST_REALTOR_EMAIL1,
	TEST_SERVICE_NAME1, TEST_APARTMENT_ID1, TEST_OFFER1); // absent customer email
	ASSERT_TEST(yad3_result == YAD3_EMAIL_DOES_NOT_EXIST);
	yad3_result = yad3AddOffer(yad3, TEST_CUSTOMER_EMAIL1, TEST_ABSENT_STRING,
	TEST_SERVICE_NAME1, TEST_APARTMENT_ID1, TEST_OFFER1); // absent realtor email
	ASSERT_TEST(yad3_result == YAD3_EMAIL_DOES_NOT_EXIST);
	yad3_result = yad3AddOffer(yad3, TEST_REALTOR_EMAIL1, TEST_REALTOR_EMAIL1,
	TEST_SERVICE_NAME1, TEST_APARTMENT_ID1, TEST_OFFER1); // wrong customer email
	ASSERT_TEST(yad3_result == YAD3_EMAIL_WRONG_ACCOUNT_TYPE);
	yad3_result = yad3AddOffer(yad3, TEST_CUSTOMER_EMAIL1, TEST_CUSTOMER_EMAIL1,
	TEST_SERVICE_NAME1, TEST_APARTMENT_ID1, TEST_OFFER1); // wrong realtor email
	ASSERT_TEST(yad3_result == YAD3_EMAIL_WRONG_ACCOUNT_TYPE);
	yad3_result = yad3AddOffer(yad3, TEST_CUSTOMER_EMAIL1, TEST_REALTOR_EMAIL1,
	TEST_ABSENT_STRING, TEST_APARTMENT_ID1, TEST_OFFER1); // service doesn't exist
	ASSERT_TEST(yad3_result == YAD3_SERVICE_DOES_NOT_EXIST);
	yad3_result = yad3AddOffer(yad3, TEST_CUSTOMER_EMAIL1, TEST_REALTOR_EMAIL1,
	TEST_SERVICE_NAME1, TEST_ABSENT_APARTMENT_ID, TEST_OFFER1); // apartment doesn't exist
	ASSERT_TEST(yad3_result == YAD3_APARTMENT_DOES_NOT_EXIST);
	yad3_result = yad3AddOffer(yad3, TEST_CUSTOMER_EMAIL1, TEST_REALTOR_EMAIL1,
	TEST_SERVICE_NAME1, TEST_APARTMENT_ID1, TEST_OFFER1); // unfit apartment
	ASSERT_TEST(yad3_result == YAD3_REQUEST_WRONG_PROPERTIES);
	yad3_result = yad3AddOffer(yad3, TEST_CUSTOMER_EMAIL2, TEST_REALTOR_EMAIL1,
	TEST_SERVICE_NAME1, TEST_APARTMENT_ID1, TEST_ILLOGICAL_OFFER1); // low price
	ASSERT_TEST(yad3_result == YAD3_REQUEST_ILLOGICAL_PRICE);
	ASSERT_TEST(yad3NumOfOffers(yad3) == 0);
	yad3_result = yad3AddOffer(yad3, TEST_CUSTOMER_EMAIL2, TEST_REALTOR_EMAIL1,
	TEST_SERVICE_NAME1, TEST_APARTMENT_ID1, TEST_OFFER1); // SUCCESS!
	ASSERT_TEST(yad3_result == YAD3_SUCCESS);
	ASSERT_TEST(yad3NumOfOffers(yad3) == 1);
	yad3_result = yad3AddOffer(yad3, TEST_CUSTOMER_EMAIL2, TEST_REALTOR_EMAIL1,
	TEST_SERVICE_NAME1, TEST_APARTMENT_ID1, TEST_OFFER1); // offer exists
	ASSERT_TEST(yad3_result == YAD3_ALREADY_REQUESTED);
	ASSERT_TEST(yad3NumOfOffers(yad3) == 1);

	yad3Destroy(yad3);
	return true;
}


static bool testYad3RemoveCustomer() {

	Yad3 yad3 = yad3Create();
	yad3AddCustomer(yad3, TEST_CUSTOMER_EMAIL1, 1000, 1000, 1);
	yad3AddCustomer(yad3, TEST_CUSTOMER_EMAIL2, 1, 1, 10000);
	yad3AddRealtor(yad3, TEST_REALTOR_EMAIL1, TEST_COMPANY_NAME1,
	TEST_TAX_PERCENTAGE1);
	yad3AddServiceToRealtor(yad3, TEST_REALTOR_EMAIL1, TEST_SERVICE_NAME1, 1);
	yad3AddApartmentToRealtor(yad3, TEST_REALTOR_EMAIL1, TEST_SERVICE_NAME1,
	TEST_APARTMENT_ID1, TEST_PRICE1, TEST_WIDTH1,
	TEST_HEIGHT1, TEST_APARTMENT1);
	yad3AddOffer(yad3, TEST_CUSTOMER_EMAIL2, TEST_REALTOR_EMAIL1,
	TEST_SERVICE_NAME1, TEST_APARTMENT_ID1, TEST_OFFER1);
	assert(yad3NumOfCustomers(yad3) == 2);
	assert(yad3NumOfRealtors(yad3) == 1);
	assert(yad3NumOfServices(yad3) == 1);
	assert(yad3NumOfApartments(yad3) == 1);
	assert(yad3NumOfOffers(yad3) == 1);

	Yad3Result yad3_result;
	yad3_result = yad3RemoveCustomer(yad3, TEST_CUSTOMER_EMAIL2); // SUCCESS!
	ASSERT_TEST(yad3_result = YAD3_SUCCESS);
	ASSERT_TEST(yad3NumOfCustomers(yad3) == 1);
	ASSERT_TEST(yad3NumOfOffers(yad3) == 0);
	yad3_result = yad3RemoveCustomer(NULL, TEST_CUSTOMER_EMAIL2); // NULL yad3
	ASSERT_TEST(yad3_result = YAD3_INVALID_PARAMETERS);
	yad3_result = yad3RemoveCustomer(yad3, NULL); // NULL yad3
	ASSERT_TEST(yad3_result = YAD3_INVALID_PARAMETERS);
	yad3_result = yad3RemoveCustomer(yad3, TEST_BAD_EMAIL1); // bad email
	ASSERT_TEST(yad3_result = YAD3_INVALID_PARAMETERS);
	yad3_result = yad3RemoveCustomer(yad3, TEST_BAD_EMAIL2); // bad email
	ASSERT_TEST(yad3_result = YAD3_INVALID_PARAMETERS);
	yad3_result = yad3RemoveCustomer(yad3, TEST_ABSENT_STRING); // customer doesn't exit
	ASSERT_TEST(yad3_result = YAD3_EMAIL_DOES_NOT_EXIST);
	yad3_result = yad3RemoveCustomer(yad3, TEST_REALTOR_EMAIL1); // exists in realtors
	ASSERT_TEST(yad3_result = YAD3_EMAIL_WRONG_ACCOUNT_TYPE);

	yad3Destroy(yad3);

	return true;
}


static bool testYad3RemoveRealtor() {

	Yad3 yad3 = yad3Create();
	yad3AddCustomer(yad3, TEST_CUSTOMER_EMAIL1, 1000, 1000, 1);
	yad3AddCustomer(yad3, TEST_CUSTOMER_EMAIL2, 1, 1, 10000);
	yad3AddRealtor(yad3, TEST_REALTOR_EMAIL1, TEST_COMPANY_NAME1,
	TEST_TAX_PERCENTAGE1);
	yad3AddServiceToRealtor(yad3, TEST_REALTOR_EMAIL1, TEST_SERVICE_NAME1, 1);
	yad3AddApartmentToRealtor(yad3, TEST_REALTOR_EMAIL1, TEST_SERVICE_NAME1,
	TEST_APARTMENT_ID1, TEST_PRICE1, TEST_WIDTH1,
	TEST_HEIGHT1, TEST_APARTMENT1);
	yad3AddOffer(yad3, TEST_CUSTOMER_EMAIL2, TEST_REALTOR_EMAIL1,
	TEST_SERVICE_NAME1, TEST_APARTMENT_ID1,
	TEST_OFFER1);
	assert(yad3NumOfCustomers(yad3) == 2);
	assert(yad3NumOfRealtors(yad3) == 1);
	assert(yad3NumOfServices(yad3) == 1);
	assert(yad3NumOfApartments(yad3) == 1);
	assert(yad3NumOfOffers(yad3) == 1);

	Yad3Result yad3_result;
	yad3_result = yad3RemoveRealtor(yad3, TEST_REALTOR_EMAIL1); // SUCCESS!
	ASSERT_TEST(yad3_result == YAD3_SUCCESS);
	ASSERT_TEST(yad3NumOfRealtors(yad3) == 0);
	ASSERT_TEST(yad3NumOfOffers(yad3) == 0);
	yad3_result = yad3RemoveRealtor(NULL, TEST_REALTOR_EMAIL1); // NULL yad3
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3RemoveRealtor(yad3, NULL); // NULL yad3
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3RemoveRealtor(yad3, TEST_BAD_EMAIL1); // bad email
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3RemoveRealtor(yad3, TEST_BAD_EMAIL2); // bad email
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3RemoveRealtor(yad3, "realtor1@email"); // realtor doesn't exist
	ASSERT_TEST(yad3_result == YAD3_EMAIL_DOES_NOT_EXIST);
	yad3_result = yad3RemoveRealtor(yad3, "customer1@email"); // exists in customers
	ASSERT_TEST(yad3_result == YAD3_EMAIL_WRONG_ACCOUNT_TYPE);

	yad3Destroy(yad3);

	return true;
}


static bool testYad3AddServiceToRealtor() {

	Yad3 yad3 = yad3Create();
	yad3AddCustomer(yad3, TEST_CUSTOMER_EMAIL1, TEST_MIN_AREA1, TEST_MIN_ROOMS1,
	TEST_MAX_PRICE1);
	yad3AddRealtor(yad3, TEST_REALTOR_EMAIL1, TEST_COMPANY_NAME1,
	TEST_TAX_PERCENTAGE1);
	assert(yad3NumOfRealtors(yad3) == 1);
	assert(yad3NumOfCustomers(yad3) == 1);
	assert(yad3NumOfServices(yad3) == 0);

	Yad3Result yad3_result;
	yad3_result = yad3AddServiceToRealtor(yad3, TEST_REALTOR_EMAIL1,
	TEST_SERVICE_NAME1, TEST_MAX_APARTMENTS1); // SUCCESS!
	ASSERT_TEST(yad3_result == YAD3_SUCCESS);
	ASSERT_TEST(yad3NumOfServices(yad3) == 1);
	yad3_result = yad3AddServiceToRealtor(NULL, TEST_REALTOR_EMAIL1,
	TEST_SERVICE_NAME1, TEST_MAX_APARTMENTS1); // NULL yad3
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3AddServiceToRealtor(yad3, NULL,
	TEST_SERVICE_NAME1, TEST_MAX_APARTMENTS1); // NULL email
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3AddServiceToRealtor(yad3, TEST_REALTOR_EMAIL1,
	NULL, TEST_MAX_APARTMENTS1); // NULL service name
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3AddServiceToRealtor(yad3, TEST_BAD_EMAIL1,
	TEST_SERVICE_NAME1, TEST_MAX_APARTMENTS1); // bad email
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3AddServiceToRealtor(yad3, TEST_BAD_EMAIL2,
	TEST_SERVICE_NAME1, TEST_MAX_APARTMENTS1); // bad email
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3AddServiceToRealtor(yad3, TEST_REALTOR_EMAIL1,
	TEST_SERVICE_NAME1, TEST_BAD_MAX_APARTMENTS); // bad max apartments
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3AddServiceToRealtor(yad3, TEST_ABSENT_STRING,
	TEST_SERVICE_NAME1, TEST_MAX_APARTMENTS1); // email doesn't exist
	ASSERT_TEST(yad3_result == YAD3_EMAIL_DOES_NOT_EXIST);
	yad3_result = yad3AddServiceToRealtor(yad3, TEST_CUSTOMER_EMAIL1,
	TEST_SERVICE_NAME1, TEST_MAX_APARTMENTS1); // wrong email
	yad3_result = yad3AddServiceToRealtor(yad3, TEST_REALTOR_EMAIL1,
	TEST_SERVICE_NAME1, TEST_MAX_APARTMENTS1); // service exists

	yad3Destroy(yad3);

	return true;
}


static bool testYad3RemoveServiceFromRealtor() {

	Yad3 yad3 = yad3Create();
	yad3AddCustomer(yad3, TEST_CUSTOMER_EMAIL1, 1000, 1000, 1);
	yad3AddCustomer(yad3, TEST_CUSTOMER_EMAIL2, 1, 1, 10000);
	yad3AddRealtor(yad3, TEST_REALTOR_EMAIL1, TEST_COMPANY_NAME1,
	TEST_TAX_PERCENTAGE1);
	yad3AddServiceToRealtor(yad3, TEST_REALTOR_EMAIL1, TEST_SERVICE_NAME1,
	TEST_MAX_APARTMENTS1);
	yad3AddApartmentToRealtor(yad3, TEST_REALTOR_EMAIL1, TEST_SERVICE_NAME1,
	TEST_APARTMENT_ID1, TEST_PRICE1, TEST_WIDTH1,
	TEST_HEIGHT1, TEST_APARTMENT1);
	yad3AddOffer(yad3, TEST_CUSTOMER_EMAIL2, TEST_REALTOR_EMAIL1,
	TEST_SERVICE_NAME1, TEST_APARTMENT_ID1,
	TEST_OFFER1);
	assert(yad3NumOfCustomers(yad3) == 2);
	assert(yad3NumOfRealtors(yad3) == 1);
	assert(yad3NumOfServices(yad3) == 1);
	assert(yad3NumOfApartments(yad3) == 1);
	assert(yad3NumOfOffers(yad3) == 1);

	Yad3Result yad3_result;
	yad3_result = yad3RemoveServiceFromRealtor(yad3, TEST_REALTOR_EMAIL1,
	TEST_SERVICE_NAME1); // SUCCESS!
	ASSERT_TEST(yad3_result == YAD3_SUCCESS);
	ASSERT_TEST(yad3NumOfServices(yad3) == 0);
	ASSERT_TEST(yad3NumOfOffers(yad3) == 0);
	yad3_result = yad3RemoveServiceFromRealtor(NULL, TEST_REALTOR_EMAIL1,
	TEST_SERVICE_NAME1); // NULL yad3
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3RemoveServiceFromRealtor(yad3, NULL, TEST_SERVICE_NAME1); // NULL email
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3RemoveServiceFromRealtor(yad3, TEST_REALTOR_EMAIL1, NULL); // NULL service name
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3RemoveServiceFromRealtor(yad3, TEST_BAD_EMAIL1,
	TEST_SERVICE_NAME1); // bad email
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3RemoveServiceFromRealtor(yad3, TEST_BAD_EMAIL2,
	TEST_SERVICE_NAME1); // bad email
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3RemoveServiceFromRealtor(yad3, TEST_ABSENT_STRING,
	TEST_SERVICE_NAME1); // email doesn't exit
	ASSERT_TEST(yad3_result == YAD3_EMAIL_DOES_NOT_EXIST);
	yad3_result = yad3RemoveServiceFromRealtor(yad3, TEST_CUSTOMER_EMAIL1,
	TEST_SERVICE_NAME1); // wrong email
	ASSERT_TEST(yad3_result == YAD3_EMAIL_WRONG_ACCOUNT_TYPE);
	yad3_result = yad3RemoveServiceFromRealtor(yad3, TEST_REALTOR_EMAIL1,
	TEST_SERVICE_NAME1); // service doesn't exit
	ASSERT_TEST(yad3NumOfServices(yad3) == 0);

	yad3Destroy(yad3);

	return true;
}


static bool testYad3AddApartmentToRealtor() {

	Yad3 yad3 = yad3Create();
	yad3AddCustomer(yad3, TEST_CUSTOMER_EMAIL1, TEST_MIN_AREA1, TEST_MIN_ROOMS1,
	TEST_MAX_PRICE1);
	yad3AddRealtor(yad3, TEST_REALTOR_EMAIL1, TEST_COMPANY_NAME1,
	TEST_TAX_PERCENTAGE1);
	yad3AddServiceToRealtor(yad3, TEST_REALTOR_EMAIL1, TEST_SERVICE_NAME1, 2);
	assert(yad3NumOfCustomers(yad3) == 1);
	assert(yad3NumOfRealtors(yad3) == 1);
	assert(yad3NumOfServices(yad3) == 1);
	assert(yad3NumOfApartments(yad3) == 0);

	Yad3Result yad3_result;
	yad3_result = yad3AddApartmentToRealtor(yad3, TEST_REALTOR_EMAIL1,
	TEST_SERVICE_NAME1, TEST_APARTMENT_ID1, TEST_PRICE1, TEST_WIDTH1,
	TEST_HEIGHT1, TEST_APARTMENT1); // SUCCESS!
	ASSERT_TEST(yad3_result == YAD3_SUCCESS);
	ASSERT_TEST(yad3NumOfApartments(yad3) == 1);
	yad3_result = yad3AddApartmentToRealtor(NULL, TEST_REALTOR_EMAIL1,
	TEST_SERVICE_NAME1, TEST_APARTMENT_ID1, TEST_PRICE1, TEST_WIDTH1,
	TEST_HEIGHT1, TEST_APARTMENT1); // NULL yad3
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3AddApartmentToRealtor(yad3, NULL,
	TEST_SERVICE_NAME1, TEST_APARTMENT_ID1, TEST_PRICE1, TEST_WIDTH1,
	TEST_HEIGHT1, TEST_APARTMENT1); // NULL realtor email
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3AddApartmentToRealtor(yad3, TEST_REALTOR_EMAIL1,
	NULL, TEST_APARTMENT_ID1, TEST_PRICE1, TEST_WIDTH1,
	TEST_HEIGHT1, TEST_APARTMENT1); // NULL service name
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3AddApartmentToRealtor(yad3, TEST_BAD_EMAIL1,
	TEST_SERVICE_NAME1, TEST_APARTMENT_ID1, TEST_PRICE1, TEST_WIDTH1,
	TEST_HEIGHT1, TEST_APARTMENT1); // bad email
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3AddApartmentToRealtor(yad3, TEST_BAD_EMAIL2,
	TEST_SERVICE_NAME1, TEST_APARTMENT_ID1, TEST_PRICE1, TEST_WIDTH1,
	TEST_HEIGHT1, TEST_APARTMENT1); // bad email
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3AddApartmentToRealtor(yad3, TEST_BAD_EMAIL2,
	TEST_SERVICE_NAME1, TEST_BAD_APARTMENT_ID, TEST_PRICE1, TEST_WIDTH1,
	TEST_HEIGHT1, TEST_APARTMENT1); // bad id
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3AddApartmentToRealtor(yad3, TEST_REALTOR_EMAIL1,
	TEST_SERVICE_NAME1, TEST_APARTMENT_ID1, TEST_BAD_PRICE1, TEST_WIDTH1,
	TEST_HEIGHT1, TEST_APARTMENT1); // bad price
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3AddApartmentToRealtor(yad3, TEST_REALTOR_EMAIL1,
	TEST_SERVICE_NAME1, TEST_APARTMENT_ID1, TEST_BAD_PRICE2, TEST_WIDTH1,
	TEST_HEIGHT1, TEST_APARTMENT1); // bad price
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3AddApartmentToRealtor(yad3, TEST_REALTOR_EMAIL1,
	TEST_SERVICE_NAME1, TEST_APARTMENT_ID1, TEST_PRICE1, TEST_BAD_WIDTH,
	TEST_HEIGHT1, TEST_APARTMENT1); // bad width
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3AddApartmentToRealtor(yad3, TEST_REALTOR_EMAIL1,
	TEST_SERVICE_NAME1, TEST_APARTMENT_ID1, TEST_PRICE1, TEST_WIDTH1,
	TEST_BAD_HEIGHT, TEST_APARTMENT1); // bad height
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3AddApartmentToRealtor(yad3, TEST_REALTOR_EMAIL1,
	TEST_SERVICE_NAME1, TEST_APARTMENT_ID1, TEST_PRICE1, TEST_WIDTH1,
	TEST_HEIGHT1, TEST_BAD_APARTMENT1); //  bad apartment
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3AddApartmentToRealtor(yad3, TEST_REALTOR_EMAIL1,
	TEST_SERVICE_NAME1, TEST_APARTMENT_ID1, TEST_PRICE1, TEST_WIDTH1,
	TEST_HEIGHT1, TEST_BAD_APARTMENT2); //  bad apartment
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3AddApartmentToRealtor(yad3, TEST_REALTOR_EMAIL1,
	TEST_SERVICE_NAME1, TEST_APARTMENT_ID1, TEST_PRICE1, TEST_WIDTH1,
	TEST_HEIGHT1, TEST_BAD_APARTMENT3); //  bad apartment
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3AddApartmentToRealtor(yad3, TEST_ABSENT_STRING,
	TEST_SERVICE_NAME1, TEST_APARTMENT_ID1, TEST_PRICE1, TEST_WIDTH1,
	TEST_HEIGHT1, TEST_APARTMENT1); // email doesn't exist
	ASSERT_TEST(yad3_result == YAD3_EMAIL_DOES_NOT_EXIST);
	yad3_result = yad3AddApartmentToRealtor(yad3, TEST_CUSTOMER_EMAIL1,
	TEST_SERVICE_NAME1, TEST_APARTMENT_ID1, TEST_PRICE1, TEST_WIDTH1,
	TEST_HEIGHT1, TEST_APARTMENT1); // email exists in customers
	ASSERT_TEST(yad3_result == YAD3_EMAIL_WRONG_ACCOUNT_TYPE);
	yad3_result = yad3AddApartmentToRealtor(yad3, TEST_REALTOR_EMAIL1,
	TEST_ABSENT_STRING, TEST_APARTMENT_ID1, TEST_PRICE1, TEST_WIDTH1,
	TEST_HEIGHT1, TEST_APARTMENT1); // service doesn't exist
	ASSERT_TEST(yad3_result == YAD3_SERVICE_DOES_NOT_EXIST);
	yad3_result = yad3AddApartmentToRealtor(yad3, TEST_REALTOR_EMAIL1,
	TEST_SERVICE_NAME1, TEST_APARTMENT_ID1, TEST_PRICE1, TEST_WIDTH1,
	TEST_HEIGHT1, TEST_APARTMENT1); // apartment already exists
	ASSERT_TEST(yad3_result == YAD3_APARTMENT_ALREADY_EXISTS);
	ASSERT_TEST(yad3NumOfApartments(yad3) == 1);
	yad3AddApartmentToRealtor(yad3, TEST_REALTOR_EMAIL1,
	TEST_SERVICE_NAME1, TEST_APARTMENT_ID2, TEST_PRICE1, TEST_WIDTH1,
	TEST_HEIGHT1, TEST_APARTMENT1);
	assert(yad3NumOfApartments(yad3) == 2);
	yad3_result = yad3AddApartmentToRealtor(yad3, TEST_REALTOR_EMAIL1,
	TEST_SERVICE_NAME1, TEST_APARTMENT_ID1, TEST_PRICE1, TEST_WIDTH1,
	TEST_HEIGHT1, TEST_APARTMENT1); // service full
	ASSERT_TEST(yad3_result == YAD3_SERVICE_FULL);
	ASSERT_TEST(yad3NumOfApartments(yad3) == 2);

	yad3Destroy(yad3);

	return true;
}


static bool testYad3RemoveApartmentFromRealtor() {

	Yad3Result yad3_result;

	Yad3 yad3 = yad3Create();
	yad3AddCustomer(yad3, TEST_CUSTOMER_EMAIL1, 1000, 1000, 1);
	yad3AddCustomer(yad3, TEST_CUSTOMER_EMAIL2, 1, 1, 10000);
	yad3AddRealtor(yad3, TEST_REALTOR_EMAIL1, TEST_COMPANY_NAME1,
	TEST_TAX_PERCENTAGE1);
	yad3AddServiceToRealtor(yad3, TEST_REALTOR_EMAIL1, TEST_SERVICE_NAME1, 1);
	yad3AddApartmentToRealtor(yad3, TEST_REALTOR_EMAIL1, TEST_SERVICE_NAME1,
	TEST_APARTMENT_ID1, TEST_PRICE1, TEST_WIDTH1,
	TEST_HEIGHT1, TEST_APARTMENT1);
	yad3AddOffer(yad3, TEST_CUSTOMER_EMAIL2, TEST_REALTOR_EMAIL1,
	TEST_SERVICE_NAME1, TEST_APARTMENT_ID1,
	TEST_OFFER1);
	assert(yad3NumOfCustomers(yad3) == 2);
	assert(yad3NumOfRealtors(yad3) == 1);
	assert(yad3NumOfServices(yad3) == 1);
	assert(yad3NumOfApartments(yad3) == 1);
	assert(yad3NumOfOffers(yad3) == 1);

	yad3_result = yad3RemoveApartmentFromRealtor(yad3, TEST_REALTOR_EMAIL1,
	TEST_SERVICE_NAME1, TEST_APARTMENT_ID1); // SUCCESS!
	ASSERT_TEST(yad3_result == YAD3_SUCCESS);
	ASSERT_TEST(yad3NumOfApartments(yad3) == 0);
	ASSERT_TEST(yad3NumOfOffers(yad3) == 0);
	yad3_result = yad3RemoveApartmentFromRealtor(NULL, TEST_REALTOR_EMAIL1,
	TEST_SERVICE_NAME1, TEST_APARTMENT_ID1); // NULL yad3
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3RemoveApartmentFromRealtor(yad3, NULL, TEST_SERVICE_NAME1,
	TEST_APARTMENT_ID1); // NULL email
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3RemoveApartmentFromRealtor(yad3, TEST_REALTOR_EMAIL1,
	NULL, TEST_APARTMENT_ID1); // NULL service name
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3RemoveApartmentFromRealtor(yad3, TEST_BAD_EMAIL1,
	TEST_SERVICE_NAME1, TEST_APARTMENT_ID1); // bad email
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3RemoveApartmentFromRealtor(yad3, TEST_BAD_EMAIL2,
	TEST_SERVICE_NAME1, TEST_APARTMENT_ID1); // bad email
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3RemoveApartmentFromRealtor(yad3, TEST_REALTOR_EMAIL1,
	TEST_SERVICE_NAME1, TEST_BAD_APARTMENT_ID); // bad id
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3RemoveApartmentFromRealtor(yad3, TEST_ABSENT_STRING,
	TEST_SERVICE_NAME1, TEST_APARTMENT_ID1); // email doesn't exist
	ASSERT_TEST(yad3_result == YAD3_EMAIL_DOES_NOT_EXIST);
	yad3_result = yad3RemoveApartmentFromRealtor(yad3, TEST_CUSTOMER_EMAIL1,
	TEST_SERVICE_NAME1, TEST_APARTMENT_ID1); // wrong email
	ASSERT_TEST(yad3_result == YAD3_EMAIL_WRONG_ACCOUNT_TYPE);
	yad3_result = yad3RemoveApartmentFromRealtor(yad3, TEST_REALTOR_EMAIL1,
	TEST_ABSENT_STRING, TEST_APARTMENT_ID1); // service doesn't exist
	ASSERT_TEST(yad3_result == YAD3_SERVICE_DOES_NOT_EXIST);
	yad3_result = yad3RemoveApartmentFromRealtor(yad3, TEST_REALTOR_EMAIL1,
	TEST_SERVICE_NAME1, TEST_APARTMENT_ID1); // apartment doesn't exist
	ASSERT_TEST(yad3_result == YAD3_APARTMENT_DOES_NOT_EXIST);
	ASSERT_TEST(yad3NumOfApartments(yad3) == 0);

	yad3Destroy(yad3);

	return true;
}


static bool testYad3CustomerPurchase() {

	Yad3 yad3 = yad3Create();
	yad3AddCustomer(yad3, TEST_CUSTOMER_EMAIL1, 1000, 1000, 1);
	yad3AddCustomer(yad3, TEST_CUSTOMER_EMAIL2, 1, 1, 10000);
	yad3AddRealtor(yad3, TEST_REALTOR_EMAIL1, TEST_COMPANY_NAME1,
	TEST_TAX_PERCENTAGE1);
	yad3AddServiceToRealtor(yad3, TEST_REALTOR_EMAIL1, TEST_SERVICE_NAME1, 1);
	yad3AddApartmentToRealtor(yad3, TEST_REALTOR_EMAIL1, TEST_SERVICE_NAME1,
	TEST_APARTMENT_ID1, TEST_PRICE1, TEST_WIDTH1,
	TEST_HEIGHT1, TEST_APARTMENT1);
	yad3AddOffer(yad3, TEST_CUSTOMER_EMAIL2, TEST_REALTOR_EMAIL1,
	TEST_SERVICE_NAME1, TEST_APARTMENT_ID1, TEST_OFFER1);
	assert(yad3NumOfCustomers(yad3) == 2);
	assert(yad3NumOfRealtors(yad3) == 1);
	assert(yad3NumOfServices(yad3) == 1);
	assert(yad3NumOfApartments(yad3) == 1);
	assert(yad3NumOfOffers(yad3) == 1);

	Yad3Result yad3_result;
	yad3_result = yad3CustomerPurchase(NULL, TEST_CUSTOMER_EMAIL1,
	TEST_REALTOR_EMAIL1, TEST_SERVICE_NAME1, TEST_APARTMENT_ID1); // NULL yad3
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3CustomerPurchase(yad3, NULL, TEST_REALTOR_EMAIL1,
	TEST_SERVICE_NAME1, TEST_APARTMENT_ID1); // NULL customer email
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3CustomerPurchase(yad3, TEST_CUSTOMER_EMAIL1,
	NULL, TEST_SERVICE_NAME1, TEST_APARTMENT_ID1); // NULL realtor email
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3CustomerPurchase(yad3, TEST_CUSTOMER_EMAIL1,
	TEST_REALTOR_EMAIL1, NULL, TEST_APARTMENT_ID1); // NULL service name
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3CustomerPurchase(yad3, TEST_BAD_EMAIL1,
	TEST_REALTOR_EMAIL1, TEST_SERVICE_NAME1, TEST_APARTMENT_ID1); // bad customer email
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3CustomerPurchase(yad3, TEST_BAD_EMAIL2,
	TEST_REALTOR_EMAIL1, TEST_SERVICE_NAME1, TEST_APARTMENT_ID1); // bad realtor email
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3CustomerPurchase(yad3, TEST_CUSTOMER_EMAIL1,
	TEST_BAD_EMAIL1, TEST_SERVICE_NAME1, TEST_APARTMENT_ID1); // bad customer email
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3CustomerPurchase(yad3, TEST_CUSTOMER_EMAIL1,
	TEST_BAD_EMAIL2, TEST_SERVICE_NAME1, TEST_APARTMENT_ID1); // bad customer email
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3CustomerPurchase(yad3, TEST_CUSTOMER_EMAIL1,
	TEST_REALTOR_EMAIL1, TEST_SERVICE_NAME1, TEST_BAD_APARTMENT_ID); // bad id
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3CustomerPurchase(yad3, TEST_ABSENT_STRING,
	TEST_REALTOR_EMAIL1, TEST_SERVICE_NAME1, TEST_APARTMENT_ID1); // customer doesn't exist
	ASSERT_TEST(yad3_result == YAD3_EMAIL_DOES_NOT_EXIST);
	yad3_result = yad3CustomerPurchase(yad3, TEST_CUSTOMER_EMAIL1,
	TEST_ABSENT_STRING, TEST_SERVICE_NAME1, TEST_APARTMENT_ID1); // realtor doesn't exist
	ASSERT_TEST(yad3_result == YAD3_EMAIL_DOES_NOT_EXIST);
	yad3_result = yad3CustomerPurchase(yad3, TEST_REALTOR_EMAIL1,
	TEST_REALTOR_EMAIL1, TEST_SERVICE_NAME1, TEST_APARTMENT_ID1); // wrong customer email
	ASSERT_TEST(yad3_result == YAD3_EMAIL_WRONG_ACCOUNT_TYPE);
	yad3_result = yad3CustomerPurchase(yad3, TEST_CUSTOMER_EMAIL1,
	TEST_CUSTOMER_EMAIL1, TEST_SERVICE_NAME1, TEST_APARTMENT_ID1); // wrong realtor email
	ASSERT_TEST(yad3_result == YAD3_EMAIL_WRONG_ACCOUNT_TYPE);
	yad3_result = yad3CustomerPurchase(yad3, TEST_CUSTOMER_EMAIL1,
	TEST_REALTOR_EMAIL1, TEST_ABSENT_STRING, TEST_APARTMENT_ID1); // service doesn't exist
	ASSERT_TEST(yad3_result == YAD3_SERVICE_DOES_NOT_EXIST);
	yad3_result = yad3CustomerPurchase(yad3, TEST_CUSTOMER_EMAIL1,
	TEST_REALTOR_EMAIL1, TEST_SERVICE_NAME1, TEST_ABSENT_APARTMENT_ID); // apartment doesn't exist
	ASSERT_TEST(yad3_result == YAD3_APARTMENT_DOES_NOT_EXIST);
	yad3_result = yad3CustomerPurchase(yad3, TEST_CUSTOMER_EMAIL1,
	TEST_REALTOR_EMAIL1, TEST_SERVICE_NAME1, TEST_APARTMENT_ID1); // bad apartment
	ASSERT_TEST(yad3_result == YAD3_PURCHASE_WRONG_PROPERTIES);
	ASSERT_TEST(yad3NumOfApartments(yad3) == 1);
	yad3_result = yad3CustomerPurchase(yad3, TEST_CUSTOMER_EMAIL2,
	TEST_REALTOR_EMAIL1, TEST_SERVICE_NAME1, TEST_APARTMENT_ID1); // SUCCESS!
	ASSERT_TEST(yad3_result == YAD3_SUCCESS);
	ASSERT_TEST(yad3NumOfApartments(yad3) == 0);
	ASSERT_TEST(yad3NumOfOffers(yad3) == 0);
	int total_purchases = yad3PurchasesSumTotal(yad3);
	ASSERT_TEST(total_purchases == TEST_PURCHASE_PRICE1);

	yad3Destroy(yad3);
	return true;
}


static bool testYad3RealtoRespondToOffer() {
	Yad3Result yad3_result;

	Yad3 yad3 = yad3Create();
	yad3AddCustomer(yad3, TEST_CUSTOMER_EMAIL1, 1000, 1000, 1);
	yad3AddCustomer(yad3, TEST_CUSTOMER_EMAIL2, 1, 1, 10000);
	yad3AddRealtor(yad3, TEST_REALTOR_EMAIL1, TEST_COMPANY_NAME1, 100);
	yad3AddServiceToRealtor(yad3, TEST_REALTOR_EMAIL1, TEST_SERVICE_NAME1, 1);
	yad3AddApartmentToRealtor(yad3, TEST_REALTOR_EMAIL1, TEST_SERVICE_NAME1,
	TEST_APARTMENT_ID1, TEST_PRICE1, TEST_WIDTH1, TEST_HEIGHT1,
	TEST_APARTMENT1);
	yad3_result = yad3AddOffer(yad3, TEST_CUSTOMER_EMAIL2, TEST_REALTOR_EMAIL1,
	TEST_SERVICE_NAME1, TEST_APARTMENT_ID1, 199);
	assert(yad3_result == YAD3_SUCCESS);
	assert(yad3NumOfRealtors(yad3) == 1);
	assert(yad3NumOfServices(yad3) == 1);
	assert(yad3NumOfApartments(yad3) == 1);
	assert(yad3NumOfOffers(yad3) == 1);

	yad3_result = yad3RealtoRespondToOffer(NULL, TEST_REALTOR_EMAIL1,
	TEST_CUSTOMER_EMAIL2, ACCEPT); // NULL yad3
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3RealtoRespondToOffer(yad3, NULL, TEST_CUSTOMER_EMAIL2,
	ACCEPT); // null realtor email
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3RealtoRespondToOffer(yad3, TEST_REALTOR_EMAIL1,
	NULL, ACCEPT); // NULL customer email
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3RealtoRespondToOffer(yad3, TEST_REALTOR_EMAIL1,
	TEST_CUSTOMER_EMAIL2, NULL); // NULL choice
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3RealtoRespondToOffer(yad3, TEST_BAD_EMAIL1,
	TEST_CUSTOMER_EMAIL2, ACCEPT); // bad realtor email
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3RealtoRespondToOffer(yad3, TEST_BAD_EMAIL2,
	TEST_CUSTOMER_EMAIL2, ACCEPT); // bad realtor email
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3RealtoRespondToOffer(yad3, TEST_REALTOR_EMAIL1,
	TEST_BAD_EMAIL1, ACCEPT); // bad customer email
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3RealtoRespondToOffer(yad3, TEST_REALTOR_EMAIL1,
	TEST_BAD_EMAIL2, ACCEPT); // bad customer email
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3RealtoRespondToOffer(yad3, TEST_REALTOR_EMAIL1,
	TEST_CUSTOMER_EMAIL2, BAD_CHOICE); // bad choice
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3RealtoRespondToOffer(yad3, TEST_ABSENT_STRING,
	TEST_CUSTOMER_EMAIL2, ACCEPT); // absent realtor email
	ASSERT_TEST(yad3_result == YAD3_EMAIL_DOES_NOT_EXIST);
	yad3_result = yad3RealtoRespondToOffer(yad3, TEST_REALTOR_EMAIL1,
	TEST_ABSENT_STRING, ACCEPT); // absent customer email
	ASSERT_TEST(yad3_result == YAD3_EMAIL_DOES_NOT_EXIST);
	yad3_result = yad3RealtoRespondToOffer(yad3, TEST_CUSTOMER_EMAIL2,
	TEST_CUSTOMER_EMAIL2, ACCEPT); // wrong email realtor
	ASSERT_TEST(yad3_result == YAD3_EMAIL_WRONG_ACCOUNT_TYPE);
	yad3_result = yad3RealtoRespondToOffer(yad3, TEST_REALTOR_EMAIL1,
	TEST_REALTOR_EMAIL1, ACCEPT); // wrong customer email
	ASSERT_TEST(yad3_result == YAD3_EMAIL_WRONG_ACCOUNT_TYPE);
	yad3_result = yad3RealtoRespondToOffer(yad3, TEST_REALTOR_EMAIL1,
	TEST_CUSTOMER_EMAIL1, ACCEPT); // no request
	ASSERT_TEST(yad3_result == YAD3_NOT_REQUESTED);
	ASSERT_TEST(yad3NumOfOffers(yad3) == 1);
	ASSERT_TEST(yad3NumOfApartments(yad3) == 1);
	yad3_result = yad3RealtoRespondToOffer(yad3, TEST_REALTOR_EMAIL1,
	TEST_CUSTOMER_EMAIL2, DECLINE); // SUCCESS! declined
	ASSERT_TEST(yad3_result == YAD3_SUCCESS);
	ASSERT_TEST(yad3NumOfOffers(yad3) == 0);
	ASSERT_TEST(yad3NumOfApartments(yad3) == 1);
	ASSERT_TEST(yad3PurchasesSumTotal(yad3) == 0);
	yad3AddOffer(yad3, TEST_CUSTOMER_EMAIL2, TEST_REALTOR_EMAIL1,
	TEST_SERVICE_NAME1, TEST_APARTMENT_ID1, TEST_OFFER1);
	assert(yad3NumOfOffers(yad3) == 1);
	yad3_result = yad3RealtoRespondToOffer(yad3, TEST_REALTOR_EMAIL1,
	TEST_CUSTOMER_EMAIL2, ACCEPT); // SUCCESS! accepted
	ASSERT_TEST(yad3_result == YAD3_SUCCESS);
	ASSERT_TEST(yad3NumOfOffers(yad3) == 0);
	ASSERT_TEST(yad3NumOfApartments(yad3) == 0);
	ASSERT_TEST(yad3PurchasesSumTotal(yad3) == 199);

	yad3Destroy(yad3);
	return true;
}


static bool testYad3ReportMostPayingCustomers() {
	Yad3Result yad3_result;

	Yad3 yad3 = yad3Create();
	yad3AddCustomer(yad3, TEST_CUSTOMER_EMAIL2, 1, 1, 10000);
	yad3AddCustomer(yad3, TEST_CUSTOMER_EMAIL1, 1, 1, 10000);
	yad3AddRealtor(yad3, TEST_REALTOR_EMAIL1, TEST_COMPANY_NAME1,
	TEST_TAX_PERCENTAGE1);
	yad3AddServiceToRealtor(yad3, TEST_REALTOR_EMAIL1, TEST_SERVICE_NAME1, 3);
	yad3AddApartmentToRealtor(yad3, TEST_REALTOR_EMAIL1, TEST_SERVICE_NAME1,
	TEST_APARTMENT_ID1, TEST_PRICE1, TEST_WIDTH1,
	TEST_HEIGHT1, TEST_APARTMENT1);
	yad3AddApartmentToRealtor(yad3, TEST_REALTOR_EMAIL1, TEST_SERVICE_NAME1,
	TEST_APARTMENT_ID1, TEST_PRICE1, TEST_WIDTH1,
	TEST_HEIGHT1, TEST_APARTMENT1);
	yad3AddApartmentToRealtor(yad3, TEST_REALTOR_EMAIL1, TEST_SERVICE_NAME1,
	TEST_APARTMENT_ID2, TEST_PRICE1, TEST_WIDTH1,
	TEST_HEIGHT1, TEST_APARTMENT1);
	yad3AddApartmentToRealtor(yad3, TEST_REALTOR_EMAIL1, TEST_SERVICE_NAME1,
	TEST_APARTMENT_ID3, TEST_PRICE1, TEST_WIDTH1,
	TEST_HEIGHT1, TEST_APARTMENT1);

	assert(yad3NumOfCustomers(yad3) == 2);
	assert(yad3NumOfRealtors(yad3) == 1);
	assert(yad3NumOfServices(yad3) == 1);
	assert(yad3NumOfApartments(yad3) == 3);
	assert(yad3NumOfOffers(yad3) == 0);

	printf("\n");
	yad3_result = yad3ReportMostPayingCustomers(NULL, 2, stdout); // NULL yad3
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3ReportMostPayingCustomers(yad3, 2, NULL); // NULL output channel
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3ReportMostPayingCustomers(yad3, TEST_BAD_COUNT, NULL); // bad count
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3ReportMostPayingCustomers(yad3, 2, stdout);
	ASSERT_TEST(yad3_result == YAD3_SUCCESS); // SUCCESS! 0 customer
	yad3CustomerPurchase(yad3, TEST_CUSTOMER_EMAIL2,
	TEST_REALTOR_EMAIL1, TEST_SERVICE_NAME1, TEST_APARTMENT_ID1);
	assert(yad3NumOfApartments(yad3) == 2);
	yad3_result = yad3ReportMostPayingCustomers(yad3, 2, stdout);
	ASSERT_TEST(yad3_result == YAD3_SUCCESS); // SUCCESS! 1 customer
	yad3CustomerPurchase(yad3, TEST_CUSTOMER_EMAIL1,
	TEST_REALTOR_EMAIL1, TEST_SERVICE_NAME1, TEST_APARTMENT_ID2);
	assert(yad3NumOfApartments(yad3) == 1);
	yad3_result = yad3ReportMostPayingCustomers(yad3, 2, stdout); // SUCCES! 2 even customers
	ASSERT_TEST(yad3_result == YAD3_SUCCESS);
	yad3CustomerPurchase(yad3, TEST_CUSTOMER_EMAIL2, TEST_REALTOR_EMAIL1,
	TEST_SERVICE_NAME1, TEST_APARTMENT_ID3);
	assert(yad3NumOfApartments(yad3) == 0);
	yad3_result = yad3ReportMostPayingCustomers(yad3, 2, stdout); // SUCCESS! 2 uneven customers
	ASSERT_TEST(yad3_result == YAD3_SUCCESS);
	yad3_result = yad3ReportMostPayingCustomers(yad3, 1, stdout); // SUCCESS! count<customers
	ASSERT_TEST(yad3_result == YAD3_SUCCESS);
	yad3_result = yad3ReportMostPayingCustomers(yad3, 3, stdout); // SUCCESS! count>customers
	ASSERT_TEST(yad3_result == YAD3_SUCCESS);

	yad3Destroy(yad3);
	return true;
}


static bool testYad3ReportRelevantRealtors() {

	Yad3 yad3 = yad3Create();
	yad3AddCustomer(yad3, TEST_CUSTOMER_EMAIL1, 1, 1, 10000);
	yad3AddCustomer(yad3, TEST_CUSTOMER_EMAIL2, 10000, 10000, 1);
	yad3AddRealtor(yad3, TEST_REALTOR_EMAIL3, TEST_COMPANY_NAME3,
	TEST_TAX_PERCENTAGE1);
	yad3AddServiceToRealtor(yad3, TEST_REALTOR_EMAIL3, TEST_SERVICE_NAME1, 3);
	yad3AddApartmentToRealtor(yad3, TEST_REALTOR_EMAIL3, TEST_SERVICE_NAME1,
	TEST_APARTMENT_ID1, 100000, TEST_WIDTH1,
	TEST_HEIGHT1, TEST_APARTMENT1);
	yad3AddRealtor(yad3, TEST_REALTOR_EMAIL2, TEST_COMPANY_NAME2,
	TEST_TAX_PERCENTAGE1);
	yad3AddServiceToRealtor(yad3, TEST_REALTOR_EMAIL2, TEST_SERVICE_NAME1, 3);
	yad3AddApartmentToRealtor(yad3, TEST_REALTOR_EMAIL2, TEST_SERVICE_NAME1,
	TEST_APARTMENT_ID1, TEST_PRICE1, TEST_WIDTH1,
	TEST_HEIGHT1, TEST_APARTMENT1);
	yad3AddRealtor(yad3, TEST_REALTOR_EMAIL1, TEST_COMPANY_NAME1,
	TEST_TAX_PERCENTAGE1);
	yad3AddServiceToRealtor(yad3, TEST_REALTOR_EMAIL1, TEST_SERVICE_NAME1, 3);
	yad3AddApartmentToRealtor(yad3, TEST_REALTOR_EMAIL1, TEST_SERVICE_NAME1,
	TEST_APARTMENT_ID1, TEST_PRICE1, TEST_WIDTH1,
	TEST_HEIGHT1, TEST_APARTMENT1);
	assert(yad3NumOfRealtors(yad3) == 3);
	assert(yad3NumOfServices(yad3) == 3);
	assert(yad3NumOfApartments(yad3) == 3);
	assert(yad3NumOfOffers(yad3) == 0);

	Yad3Result yad3_result;
	printf("\n");
	yad3_result = yad3ReportRelevantRealtors(NULL, TEST_CUSTOMER_EMAIL1,
	stdout); // NULL yad3
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3ReportRelevantRealtors(yad3, NULL,
	stdout); // NULL yad3
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3ReportRelevantRealtors(yad3, TEST_CUSTOMER_EMAIL2,
	stdout); // SUCCESS! 0 realtors
	ASSERT_TEST(yad3_result == YAD3_SUCCESS);
	yad3_result = yad3ReportRelevantRealtors(yad3, TEST_CUSTOMER_EMAIL1,
	stdout); // SUCCESS! 2/3 realtors
	ASSERT_TEST(yad3_result == YAD3_SUCCESS);

	yad3Destroy(yad3);
	return true;
}


static bool testYad3ReportSignificantRealtors() {

	Yad3 yad3 = yad3Create();
	yad3AddRealtor(yad3, TEST_REALTOR_EMAIL3, TEST_COMPANY_NAME3,
	TEST_TAX_PERCENTAGE1);
	yad3AddServiceToRealtor(yad3, TEST_REALTOR_EMAIL3, TEST_SERVICE_NAME1, 3);
	yad3AddApartmentToRealtor(yad3, TEST_REALTOR_EMAIL3, TEST_SERVICE_NAME1,
	TEST_APARTMENT_ID1, 100000, TEST_WIDTH1,
	TEST_HEIGHT1, TEST_APARTMENT1);
	yad3AddRealtor(yad3, TEST_REALTOR_EMAIL2, TEST_COMPANY_NAME2,
	TEST_TAX_PERCENTAGE1);
	yad3AddServiceToRealtor(yad3, TEST_REALTOR_EMAIL2, TEST_SERVICE_NAME1, 3);
	yad3AddApartmentToRealtor(yad3, TEST_REALTOR_EMAIL2, TEST_SERVICE_NAME1,
	TEST_APARTMENT_ID1, TEST_PRICE1, TEST_WIDTH1,
	TEST_HEIGHT1, TEST_APARTMENT1);
	yad3AddRealtor(yad3, TEST_REALTOR_EMAIL1, TEST_COMPANY_NAME1,
	TEST_TAX_PERCENTAGE1);
	yad3AddServiceToRealtor(yad3, TEST_REALTOR_EMAIL1, TEST_SERVICE_NAME1, 3);
	yad3AddApartmentToRealtor(yad3, TEST_REALTOR_EMAIL1, TEST_SERVICE_NAME1,
	TEST_APARTMENT_ID1, TEST_PRICE1, TEST_WIDTH1,
	TEST_HEIGHT1, TEST_APARTMENT1);
	assert(yad3NumOfRealtors(yad3) == 3);
	assert(yad3NumOfServices(yad3) == 3);
	assert(yad3NumOfApartments(yad3) == 3);
	assert(yad3NumOfCustomers(yad3) == 0);
	assert(yad3NumOfOffers(yad3) == 0);

	printf("\n");
	Yad3Result yad3_result;
	yad3_result = yad3ReportSignificantRealtors(NULL, 3, stdout); // NULL yad3
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3ReportSignificantRealtors(yad3, 3, NULL); // NULL output channel
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3ReportSignificantRealtors(yad3, TEST_BAD_COUNT, stdout); // bad count
	ASSERT_TEST(yad3_result == YAD3_INVALID_PARAMETERS);
	yad3_result = yad3ReportSignificantRealtors(yad3, 3, stdout); // SUCCESS!
	ASSERT_TEST(yad3_result == YAD3_SUCCESS);
	yad3_result = yad3ReportSignificantRealtors(yad3, 2, stdout); // SUCCESS! count<realtors
	ASSERT_TEST(yad3_result == YAD3_SUCCESS);
	yad3_result = yad3ReportSignificantRealtors(yad3, 4, stdout); // SUCCESS! count>realtors
	ASSERT_TEST(yad3_result == YAD3_SUCCESS);

	yad3Destroy(yad3);
	return true;
}


static bool testYad3Getters() {
	Yad3 yad3 = yad3Create();

	Yad3Result yad3_result;

	/* yad3NumOfCustomers */
	ASSERT_TEST(yad3NumOfCustomers(yad3) == 0);
	yad3_result = yad3AddCustomer(yad3, TEST_CUSTOMER_EMAIL2, 1, 1, 1000);
	assert(yad3_result == YAD3_SUCCESS);
	ASSERT_TEST(yad3NumOfCustomers(yad3) == 1);

	/* yad3NumOfRealtors */
	ASSERT_TEST(yad3NumOfRealtors(yad3) == 0);
	yad3_result = yad3AddRealtor(yad3, TEST_REALTOR_EMAIL1, TEST_COMPANY_NAME1,
	TEST_TAX_PERCENTAGE1);
	assert(yad3_result == YAD3_SUCCESS);
	ASSERT_TEST(yad3NumOfRealtors(yad3) == 1);

	/* yad3NumOfServices */
	ASSERT_TEST(yad3NumOfServices(yad3) == 0);
	yad3_result = yad3AddServiceToRealtor(yad3, TEST_REALTOR_EMAIL1,
	TEST_SERVICE_NAME1, 2);
	assert(yad3_result == YAD3_SUCCESS);
	ASSERT_TEST(yad3NumOfServices(yad3) == 1);

	/* yad3NumOfApartments */
	ASSERT_TEST(yad3NumOfApartments(yad3) == 0);
	yad3_result = yad3AddApartmentToRealtor(yad3, TEST_REALTOR_EMAIL1,
	TEST_SERVICE_NAME1, TEST_APARTMENT_ID1, TEST_PRICE1, TEST_WIDTH1,
	TEST_HEIGHT1, TEST_APARTMENT1);
	assert(yad3_result == YAD3_SUCCESS);
	ASSERT_TEST(yad3NumOfApartments(yad3) == 1);

	/* yad3NumOfOffers */
	ASSERT_TEST(yad3NumOfOffers(yad3) == 0);
	yad3_result = yad3AddOffer(yad3, TEST_CUSTOMER_EMAIL2, TEST_REALTOR_EMAIL1,
	TEST_SERVICE_NAME1, TEST_APARTMENT_ID1, TEST_OFFER1);
	assert(yad3_result == YAD3_SUCCESS);
	ASSERT_TEST(yad3NumOfOffers(yad3) == 1);

	/* yad3PurchasesSumTotal */
	ASSERT_TEST(yad3PurchasesSumTotal(yad3) == 0);
	yad3_result = yad3RealtoRespondToOffer(yad3, TEST_REALTOR_EMAIL1,
	TEST_CUSTOMER_EMAIL2, ACCEPT);
	assert(yad3_result == YAD3_SUCCESS);
	ASSERT_TEST(yad3PurchasesSumTotal(yad3) == TEST_OFFER1);

	yad3Destroy(yad3);
	return true;
}


int main() {
	RUN_TEST(testYad3Create);
	RUN_TEST(testYad3AddCustomer);
	RUN_TEST(testYad3AddRealtor);
	RUN_TEST(testYad3AddOffer);
	RUN_TEST(testYad3RemoveCustomer);
	RUN_TEST(testYad3RemoveRealtor);
	RUN_TEST(testYad3AddServiceToRealtor);
	RUN_TEST(testYad3RemoveServiceFromRealtor);
	RUN_TEST(testYad3AddApartmentToRealtor);
	RUN_TEST(testYad3RemoveApartmentFromRealtor);
	RUN_TEST(testYad3CustomerPurchase);
	RUN_TEST(testYad3RealtoRespondToOffer);
	RUN_TEST(testYad3ReportMostPayingCustomers);
	RUN_TEST(testYad3ReportRelevantRealtors);
	RUN_TEST(testYad3ReportSignificantRealtors);
	RUN_TEST(testYad3Getters);
	return 0;
}
