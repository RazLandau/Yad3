/*
 * offer.c
 *
 *  Created on: 19 באפר 2016
 *      Author: Raz
 */

#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#include "offer.h"

#define CUSTOMER_EMAIL (offer->customer_email)
#define REALTOR_EMAIL (offer->realtor_email)
#define SERVICE_NAME (offer->service_name)
#define APARTMENT_ID (offer->apartment_id)
#define NEW_PRICE (offer->new_price)

/**
 * The customer structure.
 */
struct offer_t {
	char* customer_email;
	char* realtor_email;
	char* service_name;
	int apartment_id, new_price;
};

Offer offerCreate(char* customer_email, char* realtor_email, char* service_name,
		int apartment_id, int new_price, OfferResult* offer_result) {
	if (!offer_result) {
		return NULL;
	}
	if (!emailIsValid(customer_email) || !emailIsValid(realtor_email) ||
	!service_name || !ID_IS_VALID || !NEW_PRICE_IS_VALID) {
		*offer_result = OFFER_INVALID_PARAMETERS;
		return NULL;
	}
	Offer offer = malloc(sizeof(*offer));
	char* new_customer_email = copyString(customer_email);
	char* new_realtor_email = copyString(realtor_email);
	char* new_service_name = copyString(service_name);
	if (!offer || !new_customer_email || !new_realtor_email
			|| !new_service_name) {
		free(offer);
		free(new_customer_email);
		free(new_realtor_email);
		free(new_service_name);
		*offer_result = OFFER_OUT_OF_MEMORY;
		return NULL;

	}
	CUSTOMER_EMAIL = new_customer_email;
	REALTOR_EMAIL = new_realtor_email;
	SERVICE_NAME = new_service_name;
	APARTMENT_ID = apartment_id;
	NEW_PRICE = new_price;
	*offer_result = OFFER_SUCCESS;
	return offer;
}

void offerDestroy(Offer offer) {
	if (offer) {
		free(CUSTOMER_EMAIL);
		free(REALTOR_EMAIL);
		free(SERVICE_NAME);
	}
	free(offer);
}

Offer offerCopy(Offer offer, OfferResult* offer_result) {
	if (!offer_result) {
		return NULL;
	}
	if (!offer) {
		*offer_result = OFFER_INVALID_PARAMETERS;
		return NULL;
	}
	Offer offer_copy = offerCreate(CUSTOMER_EMAIL, REALTOR_EMAIL, SERVICE_NAME,
	APARTMENT_ID, NEW_PRICE, offer_result);
	return offer_copy;
}

bool offerCompareByEmail(Offer offer1, Offer offer2) {
	assert(offer1 && offer2);
	return ((strcmp(offer1->customer_email, offer2->customer_email) == 0)
			&& (strcmp(offer1->realtor_email, offer2->realtor_email) == 0));
}

char* offerGetCustomerEmail(Offer offer) {
	assert(offer);
	return copyString(CUSTOMER_EMAIL);
}

char* offerGetRealtorEmail(Offer offer) {
	assert(offer);
	return copyString(REALTOR_EMAIL);
}

char* offerGetServiceName(Offer offer) {
	assert(offer);
	return copyString(SERVICE_NAME);
}

int offerGetApartmentId(Offer offer) {
	assert(offer);
	return APARTMENT_ID;
}

int offerGetPrice(Offer offer) {
	assert(offer);
	return NEW_PRICE;
}

