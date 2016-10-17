/*
 * offer.h
 *
 *  Created on: 19 באפר 2016
 *      Author: Raz
 */

#ifndef OFFER_H_
#define OFFER_H_

#include "utilities.h"

typedef struct offer_t* Offer;

typedef enum OfferResult_t {
	OFFER_SUCCESS, OFFER_OUT_OF_MEMORY, OFFER_INVALID_PARAMETERS
} OfferResult;

/**
 * Creates a new offer from a customer to a realtor on a specific apartment.
 *
 * @param
 * customer_email- The customer identifier.
 * realtor_email- The realtor identifier.
 * service_name- The name of the service the apartment is in.
 * apartment_id- The id of the apartment.
 * new_price- The new price offered.
 * offer_result- Pointer that holds the result/cause of failure.
 *
 * @return
 * 	A new allocated offer.
 * 	If:
 * 	price is negative,
 * 	customer_email, realtor_name, service_name or offer_result are NULL,
 * 	or in case of a memory allocation failure - return NULL.
 **/
Offer offerCreate(char* customer_email, char* realtor_email, char* service_name,
		int apartment_id, int new_price, OfferResult* offer_result);

/**
* Frees all memory allocated for the given offer.
* This function can receive NULL.
*/
void offerDestroy(Offer offer);

/**
* Creates a newly allocated copy of a given offer.
* offer_result holds the result of the creation/cause of failure.
*
* @return
* A newly allocated copy of the original offer.
* NULL if the offer is NULL or in any case of memory allocation failure.
*/
Offer offerCopy(Offer offer, OfferResult* offer_result);

/**
 * @return
 * true- if both offers' customer's and realtor's emails are identical
 * false- otherwise
 */
bool offerCompareByEmail(Offer offer1, Offer offer2);

/**
* @return
* 	A copy of the offer's customer email.
* 	NULL in any case of memory allocation failure.
*	This function asserts (offer != NULL).
*/
char* offerGetCustomerEmail(Offer offer);

/**
* @return
* 	A copy of the offer's realtor email.
* 	NULL in any case of memory allocation failure.
*	This function asserts (offer != NULL).
*/
char* offerGetRealtorEmail(Offer offer);

/**
* @return
* 	A copy of the offered apartment's service's name.
* 	NULL in any case of memory allocation failure.
*	This function asserts (offer != NULL).
*/
char* offerGetServiceName(Offer offer);

/**
* @return
* 	The offered apartment's id.
*	This function asserts (offer != NULL).
*/
int offerGetApartmentId(Offer offer);

/**
* @return
* 	The sum of the offer.
*	This function asserts (offer != NULL).
*/
int offerGetPrice(Offer offer);

#endif /* OFFER_H_ */
