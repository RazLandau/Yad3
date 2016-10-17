/*
 * utilities.h
 *
 *  Created on: 15 באפר 2016
 *      Author: Raz
 */

#ifndef UTILITIES_H_
#define UTILITIES_H_

#define TEST_REALTOR_EMAIL1 "realtor1@email"
#define TEST_REALTOR_EMAIL2 "realtor2@email"
#define TEST_REALTOR_EMAIL3 "realtor3@email"
#define TEST_CUSTOMER_EMAIL1 "customer1@email"
#define TEST_CUSTOMER_EMAIL2 "customer2@email"
#define TEST_COMPANY_NAME1 "company1"
#define TEST_COMPANY_NAME2 "company2"
#define TEST_COMPANY_NAME3 "company3"
#define TEST_SERVICE_NAME1 "service1"
#define TEST_APARTMENT1 "ewe"
#define TEST_WIDTH1 1
#define TEST_HEIGHT1 3
#define TEST_TOTAL_AREA1 2
#define TEST_NUM_OF_ROOMS1 2
#define TEST_APARTMENT_ID1 1000
#define TEST_APARTMENT_ID2 2000
#define TEST_APARTMENT_ID3 3000
#define TEST_PRICE1 100
#define TEST_MIN_AREA1 1
#define TEST_MIN_ROOMS1 1
#define TEST_MAX_PRICE1 1
#define TEST_OFFER1 ((((100+TEST_TAX_PERCENTAGE1)*TEST_PRICE1)/100)-1)
#define TEST_PURCHASE_PRICE1 (((100+TEST_TAX_PERCENTAGE1)*TEST_PRICE1)/100)
#define TEST_MAX_APARTMENTS1 1
#define TEST_BAD_APARTMENT_ID -1
#define TEST_BAD_PRICE1 0
#define TEST_BAD_PRICE2 (101)
#define TEST_BAD_WIDTH 0
#define TEST_BAD_HEIGHT 0
#define TEST_TAX_PERCENTAGE1 100
#define TEST_BAD_APARTMENT1 "ewa"
#define TEST_BAD_APARTMENT2 "ewee"
#define TEST_BAD_APARTMENT3 "ew"
#define TEST_BAD_TAX_PERCENTAGE1 0
#define TEST_BAD_TAX_PERCENTAGE2 101
#define TEST_BAD_EMAIL1 "no_at_symbol"
#define TEST_BAD_EMAIL2 "too_many_@@"
#define TEST_ABSENT_STRING "absent@string"
#define TEST_BAD_MIN_AREA 0
#define TEST_BAD_MIN_ROOMS 0
#define TEST_BAD_MAX_PRICE 0
#define TEST_ILLOGICAL_OFFER1 (TEST_OFFER1+1)
#define TEST_BAD_OFFER 0
#define TEST_ABSENT_APARTMENT_ID 9999
#define TEST_BAD_MAX_APARTMENTS 0
#define TEST_BAD_COUNT 0

#define MAX_APARTMENT_IS_VALID (max_apartments>0)
#define ID_IS_VALID (apartment_id>=0)
#define WIDTH_IS_VALID (width>=1)
#define HEIGHT_IS_VALID (height>=1)
#define PRICE_IS_VALID (price>0 && price%100==0)
#define MIN_AREA_IS_VALID (min_area>0)
#define TAX_IS_VALID (tax_percentage >= 1 && tax_percentage <= 100)
#define MAX_PRICE_IS_VALID (max_price > 0)
#define MIN_ROOMS_IS_VALID (min_rooms > 0)
#define NEW_PRICE_IS_VALID (new_price>0)

/**
* Creates a newly allocated copy of a given string.
*
* @return
* A newly allocated copy of the original string.
* NULL if the string is NULL or in any case of memory allocation failure.
*/
char* copyString(char* string);

/**
 * Checks if email is valid.
 *
 * @return
 * false- if email is NULL or if email doesn't contain exactly one '@'
 * true- otherwise
 */
bool emailIsValid(char* email);

#endif /* UTILITIES_H_ */
