/*
 * yad3.h
 *
 *  Created on: 13 באפר 2016
 *      Author: Raz
 */

#ifndef YAD3_H_
#define YAD3_H_

#include "customer.h"
#include "realtor.h"
#include "offer.h"
#include "set.h"
#include "map.h"
#include "list.h"
#include "utilities.h"

typedef enum Yad3Result_t {
	YAD3_OUT_OF_MEMORY,
	YAD3_INVALID_PARAMETERS,
	YAD3_EMAIL_ALREADY_EXISTS,
	YAD3_EMAIL_DOES_NOT_EXIST,
	YAD3_EMAIL_WRONG_ACCOUNT_TYPE,
	YAD3_ALREADY_REQUESTED,
	YAD3_NOT_REQUESTED,
	YAD3_SERVICE_ALREADY_EXISTS,
	YAD3_SERVICE_DOES_NOT_EXIST,
	YAD3_SERVICE_FULL,
	YAD3_APARTMENT_ALREADY_EXISTS,
	YAD3_APARTMENT_DOES_NOT_EXIST,
	YAD3_PURCHASE_WRONG_PROPERTIES,
	YAD3_REQUEST_WRONG_PROPERTIES,
	YAD3_REQUEST_ILLOGICAL_PRICE,
	YAD3_SUCCESS
} Yad3Result;

typedef struct yad3_t* Yad3;

/**
 * Creates a new yad3.
 *
 * @return
 * 	A new allocated yad3.
 *  In case of a memory allocation failure - return NULL.
 **/
Yad3 yad3Create();

/**
 * Frees all memory allocated for the given realtor.
 * This function can receive NULL.
 */
void yad3Destroy(Yad3 yad3);

/*
 * Adds customer with given criteria to yad3.
 *
 * @return
 *	YAD3_INVALID_PARAMETERS- if:
 *	yad3 or customer_email are NULL,
 *	min_rooms, min_area or max_price isn't positive,
 *	or customer_email doesn't include exactly one '@'.
 *	YAD3_EMAIL_ALREADY_EXISTS- if a customer with the same email already exists
 *	in yad3.
 *	YAD3_OUT_OF_MEMORY- in any case of memory allocation failure.
 *	YAD3_SUCCESS- otherwise.
 */
Yad3Result yad3AddCustomer(Yad3 yad3, char* customer_email, int min_rooms,
		int min_area, int max_price);

/*
 * Removes a customer with given email from yad3.
 *
 * @return
 *	REALTOR_INVALID_PARAMETERS- if:
 *	yad3 or email are NULL,
 *	or email doesn't include exactly one '@'.
 *	YAD3_EMAIL_DOES_NOT_EXISTS- if a customer or realtor with the same email
 *	doesn't exists in yad3.
 *	YAD3_EMAIL_WRONG_ACCOUNT_TYPE- if a realtor with the same email exists
 *	in yad3.
 *	YAD3_SUCCESS- otherwise.
 */
Yad3Result yad3RemoveCustomer(Yad3 yad3, char* email);

/*
 * Adds realtor with given criteria to yad3.
 *
 * @return
 *	YAD3_INVALID_PARAMETERS- if:
 *	yad3, realtor_email or company_name are NULL,
 *	tax_percentage is lower than 1 or higher than 100,
 *	or customer_email doesn't include exactly one '@'.
 *	YAD3_EMAIL_ALREADY_EXISTS- if a realtor with the same email already exists
 *	in yad3.
 *	YAD3_OUT_OF_MEMORY- in any case of memory allocation failure.
 *	YAD3_SUCCESS- otherwise.
 */
Yad3Result yad3AddRealtor(Yad3 yad3, char* realtor_email, char* company_name,
		int tax_percentage);

/*
 * Removes a realtor with given email from yad3.
 *
 * @return
 *	REALTOR_INVALID_PARAMETERS- if:
 *	yad3 or email are NULL,
 *	or email doesn't include exactly one '@'.
 *	YAD3_EMAIL_DOES_NOT_EXISTS- if a customer or realtor with the same email
 *	doesn't exists in yad3.
 *	YAD3_EMAIL_WRONG_ACCOUNT_TYPE- if a customer with the same email exists
 *	in yad3.
 *	YAD3_SUCCESS- otherwise.
 */
Yad3Result yad3RemoveRealtor(Yad3 yad3, char* email);

/*
 * Adds an apartment service to chosen realtor in yad3.
 *
 * @param
 * new_price- new price offered for the apartment
 *
 * @return
 *	YAD3_INVALID_PARAMETERS- if:
 *	yad3, realtor_email or service_name are NULL,
 *	max_apartments isn't positive,
 *	or realtor_email doesn't include exactly one '@'.
 *	YAD3_EMAIL_DOES_NOT_EXISTS- if a realtor with the same email or a customer
 *	with the same email don't exist in yad3.
 *	YAD3_EMAIL_WRONG_ACCOUNT_TYPE- if a customer with the same email exists
 *	in yad3.
 *	YAD3_SERVICE_ALREADY_EXISTS- if realtor already has a service with
 *	the same service_name.
 *	YAD3_OUT_OF_MEMORY- in any case of memory allocation failure.
 *	YAD3_SUCCESS- otherwise.
 */
Yad3Result yad3AddServiceToRealtor(Yad3 yad3, char* realtor_email,
		char* service_name, int max_apartments);

/*
 * Removes an apartment service with given service_name from realtor with given
 * realtor_email in yad3.
 *
 * @return
 *	REALTOR_INVALID_PARAMETERS- if:
 *	yad3, realtor_email or service_name are NULL,
 *	or if email doesn't include exactly one '@'.
 *	YAD3_EMAIL_DOES_NOT_EXISTS- if a customer or realtor with the same email
 *	doesn't exists in yad3.
 *	YAD3_EMAIL_WRONG_ACCOUNT_TYPE- if a customer with the same email exists
 *	in yad3.
 *	YAD3_SERVICE_DOES_NOT_EXIST- if realtor doesn't have a service with
 *	the same service_name.
 *	YAD3_SUCCESS- otherwise.
 */
Yad3Result yad3RemoveServiceFromRealtor(Yad3 yad3, char* realtor_email,
		char* service_name);

/*
 * Adds an apartment to chosen realtor's service in yad3.
 *
 * @param
 * new_price- new price offered for the apartment
 *
 * @return
 *	YAD3_INVALID_PARAMETERS- if:
 *	yad3, realtor_email, service_name or matrix are NULL,
 *	width or height aren't positive,
 *	apartment_id is negative,
 *	realtor_email doesn't include exactly one '@',
 *	matrix doesn't contain only 'e' or 'w' or height*width != matrix's length.
 *	YAD3_EMAIL_DOES_NOT_EXISTS- if a realtor with the same email or a customer
 *	with the same email don't exist in yad3.
 *	YAD3_EMAIL_WRONG_ACCOUNT_TYPE- if a customer with the same email exists
 *	in yad3.
 *	YAD3_SERVICE_DOES_NOT_EXIST- if realtor doesn't have a service with
 *	the same service_name.
 *	YAD3_APARTMENT_DOES_NOT_EXIST- if realtor doesn't have an apartment with the
 *	same id in chosen service.
 *	YAD3_OUT_OF_MEMORY- in any case of memory allocation failure.
 *	YAD3_SUCCESS- otherwise.
 */
Yad3Result yad3AddApartmentToRealtor(Yad3 yad3, char* realtor_email,
		char* service_name, int apartment_id, int price, int width, int height,
		char* matrix);
/*
 * Removes an apartment with given apartment_id from realtor's service in yad3.
 *
 * @return
 *	REALTOR_INVALID_PARAMETERS- if:
 *	yad3, realtor_email or service_name are NULL,
 *	apartment_id is negative,
 *	or if email doesn't include exactly one '@'.
 *	YAD3_EMAIL_DOES_NOT_EXISTS- if a customer or realtor with the same email
 *	doesn't exists in yad3.
 *	YAD3_EMAIL_WRONG_ACCOUNT_TYPE- if a customer with the same email exists
 *	in yad3.
 *	YAD3_SERVICE_DOES_NOT_EXIST- if realtor doesn't have a service with
 *	the same service_name.
 *	YAD3_APARTMENT_DOES_NOT_EXIST- if realtor doesn't have an apartment with
 *	the same apartment_id.
 *	YAD3_OUT_OF_MEMORY- in any case of memory allocation failure.
 *	YAD3_SUCCESS- otherwise.
 */
Yad3Result yad3RemoveApartmentFromRealtor(Yad3 yad3, char* realtor_email,
		char* service_name, int apartment_id);

/*
 * Adds to yad3 an price offer from a customer to a realtor on realtor's
 * apartment with the same id in chosen service name.
 *
 * @param
 * new_price- new price offered for the apartment
 *
 * @return
 *	YAD3_INVALID_PARAMETERS- if:
 *	yad3, customer_email, realtor_name or service_name are NULL,
 *	apartment id is negative,
 *	new price isn't positive,
 *	or customer_email or realtor_email doesn't include exactly one '@'.
 *	YAD3_EMAIL_DOES_NOT_EXISTS- if a realtor with the same email or a customer
 *	with the same email don't exist in yad3.
 *	YAD3_EMAIL_WRONG_ACCOUNT_TYPE- if customer_email belongs to a realtor or
 *	realtor_email belongs to a customer.
 *	YAD3_ALREADY_REQUESTED- if an offer from that customer to that realtor
 *	already exists.
 *	in yad3.
 *	YAD3_SERVICE_DOES_NOT_EXIST- if realtor doesn't have a service with
 *	the same service_name.
 *	YAD3_APARTMENT_DOES_NOT_EXIST- if realtor doesn't have an apartment with
 *	the same apartment_id.
 *	YAD3_REQUEST_WRONG_PROPERTIES- if the apartment chosen doesn't fit the
 *	customer's criteria.
 *	YAD3_REQUEST_ILLOGICAL_PRICE- if the offer's new_price is higher than what
 *	the customer would have payed in a normal purchase.
 *	YAD3_OUT_OF_MEMORY- in any case of memory allocation failure.
 *	YAD3_SUCCESS- otherwise.
 */
Yad3Result yad3AddOffer(Yad3 yad3, char* customer_email, char* realtor_email,
		char* service_name, int apartment_id, int new_price);

/*
 * Executes the response of a realtor to an exisiting offer.
 *
 * @param
 * choice- Pointer the string representation of realtor's choice.
 * if:
 * *choice='accept'- the apartment is sold.
 * *choice='decline- nothing happens.
 * The offer is removed in both cases.
 *
 * @return
 *	YAD3_INVALID_PARAMETERS- if:
 *	yad3, customer_email, realtor_name or choice are NULL,
 *	customer_email or realtor_email doesn't contain exactly one '@',
 *	or if choice hold a string other than 'accept' or 'decline'.
 *	YAD3_EMAIL_DOES_NOT_EXISTS- if a realtor with the same email or a customer
 *	with the same email don't exist in yad3.
 *	YAD3_EMAIL_WRONG_ACCOUNT_TYPE- if customer_email belongs to a realtor or
 *	realtor_email belongs to a customer.
 *	YAD3_OUT_OF_MEMORY- in any case of memory allocation failure.
 * YAD3_SUCCESS- otherwise.
 */
Yad3Result yad3RealtoRespondToOffer(Yad3 yad3, char* realtor_email,
		char* customer_email, char* choice);

/*
 * Purchase specific realtor's apartment identified by service_name and
 * apartment_id for customer
 *
 * @return
 *	YAD3_INVALID_PARAMETERS- if:
 *	yad3, customer_email, realtor_name or service_name are NULL,
 *	customer_email or realtor_email doesn't contain exactly one '@',
 *	or if apartment_id is negative.
 *	YAD3_EMAIL_DOES_NOT_EXISTS- if a realtor with the same email or a customer
 *	with the same email don't exist in yad3.
 *	YAD3_EMAIL_WRONG_ACCOUNT_TYPE- if customer_email belongs to a realtor or
 *	realtor_email belongs to a customer.
 *	YAD3_SERVICE_DOES_NOT_EXIST- if realtor doesn't have a service with
 *	the same service_name.
 *	YAD3_APARTMENT_DOES_NOT_EXIST- if realtor doesn't have an apartment with
 *	the same apartment_id.
 *	YAD3_PURCHASE_WRONG_PROPERTIES- if the apartment chosen doesn't fit the
 *	customer's criteria.
 *	YAD3_OUT_OF_MEMORY- in any case of memory allocation failure.
 *	YAD3_SUCCESS- otherwise.
 */
Yad3Result yad3CustomerPurchase(Yad3 yad3, char* customer_email,
		char* realtor_email, char* service_name, int apartment_id);

/*
 * @return the number of offers currently in yad3.
 * This function asserts (yad3 != NULL).
 */
int yad3NumOfOffers(Yad3 yad3);

/*
 * @return the number of customers currently in yad3.
 * This function asserts (yad3 != NULL).
 */
int yad3NumOfCustomers(Yad3 yad3);

/*
 * @return the number of realtors currently in yad3.
 * This function asserts (yad3 != NULL).
 */
int yad3NumOfRealtors(Yad3 yad3);

/*
 * @return the number of services currently in yad3.
 * This function asserts (yad3 != NULL).
 */
int yad3NumOfServices(Yad3 yad3);

/*
 * @return the number of apartments currently in yad3.
 * This function asserts (yad3 != NULL).
 */
int yad3NumOfApartments(Yad3 yad3);

/*
 * @return the total money spent by customers in yad3.
 * This function asserts (yad3 != NULL).
 */
int yad3PurchasesSumTotal(Yad3 yad3);

/*
 * Prints count number most paying customers in yad3 by descending order.
 * If equal, by lexicographical order of e-mails.
 * Customers who haven't spent any money at all will not be printed.
 *
 * @param
 * outputChannel - File descriptor for the output channel
 *
 * @return
 * YAD3_INVALID_PARAMETERS- if yad3 or outputChannel are NULL or if count isn't
 * positive.
 * YAD3_OUT_OF_MEMORY- in any case of memory allocation failure.
 * YAD3_SUCCESS- otherwise.
 */
Yad3Result yad3ReportMostPayingCustomers(Yad3 yad3, int count,
		FILE* outputChannel);

/*
 * Prints by lexicographical order of e-mails count number of realtors in yad3
 * that has an apartments that meets the given customer, identified by
 * customer_email, demands.
 *
 * @param
 * outputChannel - File descriptor for the output channel
 *
 * @return
 * YAD3_INVALID_PARAMETERS- if yad3, customer_email pr outputChannel are NULL,
 * or if customer_email doesn't include exactly one '@'.
 * YAD3_EMAIL_DOES_NOT_EXIST- if email doesn't exist in yad3.
 * YAD3_EMAIL_WRONG_ACCOUNT_TYPE- if a realtor with customer_email aleady exists.
 * YAD3_OUT_OF_MEMORY- in any case of memory allocation failure.
 * YAD3_SUCCESS- otherwise.
 */
Yad3Result yad3ReportRelevantRealtors(Yad3 yad3, char* customer_email,
		FILE* outputChannel);

/*
 * Prints count number of most significant realtors in yad3 by descending order.
 * In case of equal significance- by lexicographical order of e-mails.
 * Realtors without any apartments will not be printed.
 *
 * @param
 * outputChannel - File descriptor for the output channel
 *
 * @return
 * YAD3_INVALID_PARAMETERS- if yad3 or outputChannel are NULL or if count isn't
 * positive.
 * YAD3_OUT_OF_MEMORY- in any case of memory allocation failure.
 * YAD3_SUCCESS- otherwise.
 */
Yad3Result yad3ReportSignificantRealtors(Yad3 yad3, int count,
		FILE* outputChannel);

#endif /* YAD3_H_ */
