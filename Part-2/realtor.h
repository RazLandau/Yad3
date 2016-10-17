/*
 * realtor.h
 *
 *  Created on: 15 באפר 2016
 *      Author: Raz
 */

#ifndef REALTOR_H_
#define REALTOR_H_

#include "utilities.h"
#include "apartment_service.h"
#include "map.h"
#include "mtm_ex2.h"

typedef struct realtor_t* Realtor;

typedef enum RealtorResult_t {
	REALTOR_OUT_OF_MEMORY,
	REALTOR_INVALID_PARAMETERS,
	REALTOR_SERVICE_ALREADY_EXISTS,
	REALTOR_SERVICE_DOES_NOT_EXIST,
	REALTOR_SERVICE_FULL,
	REALTOR_APARTMENT_ALREADY_EXISTS,
	REALTOR_APARTMENT_DOES_NOT_EXIST,
	REALTOR_SUCCESS,
} RealtorResult;

/**
 * Creates a new customer of type image:
 *
 * @param
 * email- realtor's email
 * company_name- realtor's company's name
 * tax_percentage- added tax for sales
 * realtor_result- pointer that holds the result/cause of failure
 *
 * @return
 * 	A new allocated realtor.
 * 	If email doesn't contain exactly one '@',
 * 	tax_percentage is higher than 100 or lower than 1,
 * 	email, company_name or realtor_result are NULL
 * 	or in case of a memory allocation failure - return NULL.
 **/
Realtor realtorCreate(char* email, char* company_name, int tax_percentage,
		RealtorResult* realtor_result);

/**
 * Frees all memory allocated for the given realtor.
 * This function can receive NULL.
 */
void realtorDestroy(Realtor realtor);

/**
 * Creates a newly allocated copy of a given realtor.
 *
 * @param
 * realtor_result- Pointer that holds the result/cause of failure.
 *
 * @return
 * A newly allocated copy of the original realtor.
 * NULL if the customer is NULL or in any case of memory allocation failure.
 */
Realtor realtorCopy(Realtor realtor, RealtorResult* realtor_result);

/**
 * Adds a service to given realtor.
 *
 * @return
 *	REALTOR_INVALID_PARAMETERS- if realtor or service_name are NULL or if
 *	max_apartments isn't positive.
 *	REALTOR_SERVICE_ALREADY_EXISTS- if realtor already has a service with the
 *	same name.
 *	REALTOR_OUT_OF_MEMORY- in any case of memory allocation failure.
 *	REALTOR_SUCCESS- otherwise.
 */
RealtorResult realtorAddService(Realtor realtor, char* service_name,
		int max_apartments);

/**
 * Removes a service from given realtor.
 *
 * @return
 *	REALTOR_INVALID_PARAMETERS- if realtor or service_name are NULL.
 *	REALTOR_SERVICE_DOESN'T_EXISTS- if realtor doesn't have a service with the
 *	same name.
 *	REALTOR_SUCCESS- otherwise.
 */
RealtorResult realtorRemoveService(Realtor realtor, char* service_name);

/**
 * Adds an apartment to chosen service of given realtor.
 *
 * @return
 *	REALTOR_INVALID_PARAMETERS- if:
 *	realtor, service_name or matrix are NULL
 *	width or height aren't positive,
 *	apartment_id is negative,
 *	price isn't positive or price % 100 != 0,
 *	matrix contains something other than 'e' or 'w',
 *	or height * width != matrix's length.
 *	REALTOR_SERVICE_DOESN'T_EXISTS- if realtor doesn't have a service with the
 *	same service_name.
 *	REALTOR_SERVICE_FULL- if chosen service is full.
 *	REALTOR_APARTMENT_DOESN'T_EXISTS- if an apartment with the same id doesn't
 *	exists in chosen service.
 *	REALTOR_OUT_OF_MEMORY- in any case of memory allocation failure.
 *	REALTOR_SUCCESS- otherwise.
 */
RealtorResult realtorAddApartment(Realtor realtor, char* service_name,
		int apartment_id, int price, int width, int height, char* matrix);

/**
 * Removes an apartment from chosen service of given realtor.
 *
 * @return
 *	REALTOR_INVALID_PARAMETERS- if:
 *	realtor, service_name or matrix are NULL
 *	apartment_id is negative,
 *	REALTOR_SERVICE_DOESN'T_EXISTS- if realtor doesn't have a service with the
 *	same service_name.
 *	REALTOR_APARTMENT_DOESN'T_EXIST- if an apartment with the same id doesn't
 *	exist in chosen service.
 *	REALTOR_SUCCESS- otherwise.
 */
RealtorResult realtorRemoveApartment(Realtor realtor, char* service_name,
		int apartment_id);

/**
 * Creates a newly allocated copy of a given the apartment with the same id in
 * realtor's service with the same service_name, if exists.
 *
 * @param
 * realtor_result- Pointer that hold the result/cause of failure.
 *
 * @return
 * A newly allocated copy of the original apartment.
 * NULL if:
 * realtor, service_name or realtor_result are NULL,
 * realtor doesn't have a service with the same name,
 * realtor doesn't have an apartment with the same id in that service,
 * or in any case of memory allocation failure.
 */
Apartment realtorGetApartmentById(Realtor realtor, char* service_name,
		int apartment_id, RealtorResult* realtor_result);

/**
 * Checks if realtor has an apartment with at least as much area, at least as
 * many rooms and that costs less or equal to price.
 *
 * @return
 * true- if an such an apartment exists.
 * false- otherwise.
 *
 * this function asserts (realtor != NULL).
 */
bool realtorApartmentExists(Realtor realtor, int area, int rooms, int price);

/**
* @return
* 	The number of realtor's services.
*	This function asserts (realtor != NULL).
*/
int realtorNumOfServices(Realtor realtor);

/**
* @return
* 	The number of realtor's apartments.
*	This function asserts (realtor != NULL).
*/
int realtorNumOfApartments(Realtor realtor);

/**
* @return
* 	A copy of realtor's email
* 	NULL in any case of memory allocation failure.
*	This function asserts (realtor != NULL).
*/
char* realtorGetEmail(Realtor realtor);

/**
* @return
* 	The realtor's tax percentage.
*	This function asserts (realtor != NULL).
*/
int realtorGetTaxPercentage(Realtor realtor);

/**
* @return
* 	The realtor's rank.
* 	The rank is calculated by:
* 	1,000,000 * C + P + 100,000 * A
* 	Where:
* 	C = The number of realtor's apartments.
* 	P = The average of the median prices for all realtor's services
* 	(only for services with 1 or more apartments).
* 	A = The average of the median areas for all realtor's services.
* 	(only for services with 1 or more apartments).
*
*	This function asserts (realtor != NULL).
*/
int realtorGetRank(Realtor realtor);

/**
 * Prints realtor details.
 *
 * @param outputChannel - File descriptor for the output channel
 */
void realtorPrint(FILE* outputChannel, Realtor realtor);

#endif /* REALTOR_H_ */
