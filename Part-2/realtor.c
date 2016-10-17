/*
 * realtor.c
 *
 *  Created on: 15 באפר 2016
 *      Author: Raz
 */

#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#include "realtor.h"

enum {
	PRICE, AREA
};

#define EMAIL (realtor->email)
#define COMPANY_NAME (realtor->company_name)
#define TAX_PERCENTAGE (realtor->tax_percentage)
#define SERVICES (realtor->services)

#define ZERO_APARTMENTS (realtorNumOfApartments(realtor) == 0)

/**
 * The customer structure.
 */
struct realtor_t {
	char* email;
	char* company_name;
	int tax_percentage;
	Map services;
};

/****************************STATIC FUNCTIONS**********************************/

 /* Services Map */

/**
* Generic strcpy.
*
* @return
* A newly allocated copy of the original string, in void* form.
* NULL if the string is NULL or in any case of memory allocation failure.
*/
 static void* genericStringCopy4Map(const void* string) {
	return (void*) (copyString((char*) string));
}

/*
 * Generic strcmp.
 *
 * @return
 * 0- if elements are identical.
 * 1- if element1>element2.
 * -1- if element1<element2.
 */
 static int genericStringCompare4Map(const void* string1,
		const void* string2) {
	return strcmp((char*) string1, (char*) string2);
}

/*
 * Generic free(char*).
 * Frees all memory allocated for the given element.
 * This function can receive NULL.
 */
 static void genericStringDestroy(void* string) {
	free(string);
}

/**
 * Generic serviceCopy
 *
 * @return
 * A newly allocated copy of given service.
 * NULL if NULL service was sent or in any case of memory allocation failure.
 */
static void* genericServiceCopy4Map(const void* service) {
	return (void*) (serviceCopy((ApartmentService) service));
}

/**
 * Generic serviceDestroy.
 * Frees all memory allocated for the given apartment.
 * This function can receive NULL.
 */
static void genericServiceDestroy(void* service) {
	serviceDestroy((ApartmentService) service);
}

/* Results Conversions */

/**
 * Converts MapResult to RealtorResult.
 *
 * @return
 * RealtorResult equivalent to given MapResult
 */
static RealtorResult mapResult2RealtorResult(MapResult map_result) {
	RealtorResult realtor_result;
	switch (map_result) {
	case MAP_NULL_ARGUMENT:
		realtor_result = REALTOR_INVALID_PARAMETERS;
		break;
	case MAP_ITEM_ALREADY_EXISTS:
		realtor_result = REALTOR_SERVICE_ALREADY_EXISTS;
		break;
	case MAP_OUT_OF_MEMORY:
		realtor_result = REALTOR_OUT_OF_MEMORY;
		break;
	case MAP_ITEM_DOES_NOT_EXIST:
		realtor_result = REALTOR_SERVICE_DOES_NOT_EXIST;
		break;
	case MAP_SUCCESS:
		realtor_result = REALTOR_SUCCESS;
	}
	return realtor_result;
}

/**
 * Convert ServiceResult to RealtorResult.
 *
 * @return
 * RealtorResult equivalent to given ApartmentServiceResult
 */
static RealtorResult apartmentServiceResult2RealtorResult(
		ApartmentServiceResult apartment_service_result) {
	RealtorResult realtor_result;
	switch (apartment_service_result) {
	case APARTMENT_SERVICE_OUT_OF_BOUNDS:
	case APARTMENT_SERVICE_NULL_ARG:
		realtor_result = REALTOR_INVALID_PARAMETERS;
		break;
	case APARTMENT_SERVICE_OUT_OF_MEM:
		realtor_result = REALTOR_OUT_OF_MEMORY;
		break;
	case APARTMENT_SERVICE_FULL:
		realtor_result = REALTOR_SERVICE_FULL;
		break;
	case APARTMENT_SERVICE_ALREADY_EXISTS:
		realtor_result = REALTOR_APARTMENT_ALREADY_EXISTS;
		break;
	case APARTMENT_SERVICE_EMPTY:
	case APARTMENT_SERVICE_NO_FIT:
		realtor_result = REALTOR_APARTMENT_DOES_NOT_EXIST;
		break;
	case APARTMENT_SERVICE_SUCCESS:
		realtor_result = REALTOR_SUCCESS;
	}
	return realtor_result;
}

/* Create apartment from string */

/**
 * Frees all memory allocated for the given squares.
 * This function can receive NULL.
 * This function asserts (height > 0).
 */
static void freeSquares(SquareType** squares, int height) {
	assert(HEIGHT_IS_VALID);
	if (squares != NULL) {
		for (int i = 0; i < height; i++) {
			free(squares[i]);
		}
	}
	free(squares);
}

/**
 * Checks if matrix represent a valid apartment.
 *
 * @return
 * true- if matrix contains only 'e' and 'w' &
 * if width * height = matrix's length.
 * false- otherwise.
 * this function asserts (matrix != NULL && width > 0 && height > 0).
 */
static bool matrixIsValid(char* matrix, int width, int height) {
	assert(matrix && WIDTH_IS_VALID && HEIGHT_IS_VALID);
	int length = strlen(matrix);
	if (length != height * width) {
		return false;
	}
	for (int i = 0; i < length; i++) {
		if (matrix[i] != 'e' && matrix[i] != 'w') {
			return false;
		}
	}
	return true;
}

/**
 * Creates a matrix a squares from string.
 *
 * @return
 * A newly allocated squares.
 * NULL in any case of memory allocation failure.
 * this function asserts (matrixIsValid && width > 0 && height > 0).
 *
 */
static SquareType** createSquaresFromString(char* matrix, int width, int height) {
	assert(matrixIsValid(matrix, width, height));
	assert(WIDTH_IS_VALID && HEIGHT_IS_VALID);
	SquareType** squares = malloc(sizeof(*squares) * height);
	if (!squares) {
		return NULL;
	}
	for (int i = 0; i < height; i++) {
		squares[i] = malloc(sizeof(**squares) * width);
		if (!squares[i]) {
			freeSquares(squares, i);
			return NULL;
		}
	}
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			matrix[i * width + j] == 'e' ?
					(squares[i][j] = EMPTY) : (squares[i][j] = WALL);
		}
	}
	return squares;
}

/**
 * Creates a new apartment of type image:
 * The structure is given by matrix, and the dimensions are width/height.
 * The price of the apartment is given by price.
 *
 * @return
 * 	A new allocated apartment.
 * 	NULL in case of a memory allocation failure.
 * 	This function asserts (matrix != NULL && width > 0 && height > 0)
 */
static Apartment createApartmentFromMatrix(char* matrix, int width, int height,
		int price) {
	assert(matrix && WIDTH_IS_VALID && HEIGHT_IS_VALID);
	SquareType** squares = createSquaresFromString(matrix, width, height);
	Apartment apartment = apartmentCreate(squares, height, width, price);
	freeSquares(squares, height);
	return apartment;
}

/* Service */

/**
 *
 * Checks if realtor already has a service with similiar name.
 *
 * @return
 * true- if realtor already has a service with same name.
 * false- otherwise.
 */
static bool serviceAlreadyExists(Realtor realtor, char* service_name) {
	return mapContains(SERVICES, service_name);
}

/* rank */

/**
 *
 * Calculates the average of price/area medians for all of realtor's services.
 *
 * @param
 * type- Median to calculate:
 * 0- Price,
 * 1- Area.
 *
 * @return
 * The average of price/area medians, respectively.
 *
 * This function asserts (realtor != NULL)
 */
static int realtorAveragePriceOrAreaMedian(Realtor realtor, bool type) {
	assert(realtor);
	if (ZERO_APARTMENTS) {
		return 0;
	}
	int sum_of_medians = 0;
	int num_of_valid_services = 0;
	MAP_FOREACH(char*,i,SERVICES)
	{
		ApartmentService service = mapGet(SERVICES, i);
		if (serviceNumberOfApatments(service) >= 1) {
			int median;
			type ? serviceAreaMedian(service, &median) : servicePriceMedian(
							service, &median);
			sum_of_medians += median;
			num_of_valid_services++;
		}
	}
	int average = sum_of_medians / num_of_valid_services;
	return average;
}

/******************************************************************************/

Realtor realtorCreate(char* email, char* company_name, int tax_percentage,
		RealtorResult* realtor_result) {
	if (!realtor_result) {
		return NULL;
	}
	if (!emailIsValid(email) || !company_name || !TAX_IS_VALID) {
		*realtor_result = REALTOR_INVALID_PARAMETERS;
		return NULL;
	}
	Realtor realtor = malloc(sizeof(*realtor));
	char* new_email = copyString(email);
	char* new_company_name = copyString(company_name);
	Map services = mapCreate(genericServiceCopy4Map,
			genericStringCopy4Map, genericServiceDestroy,
			genericStringDestroy, genericStringCompare4Map);
	if (!realtor || !new_email || !new_company_name || !services) {
		free(services);
		free(new_company_name);
		free(new_email);
		free(realtor);
		return NULL;
	}
	EMAIL = new_email;
	COMPANY_NAME = new_company_name;
	TAX_PERCENTAGE = tax_percentage;
	SERVICES = services;
	*realtor_result = REALTOR_SUCCESS;
	return realtor;
}

void realtorDestroy(Realtor realtor) {
	if (realtor) {
		free(EMAIL);
		free(COMPANY_NAME);
		mapDestroy(SERVICES);
	}
	free(realtor);
}

Realtor realtorCopy(Realtor realtor, RealtorResult* realtor_result) {
	if (!realtor_result) {
		return NULL;
	}
	if (!realtor) {
		*realtor_result = REALTOR_INVALID_PARAMETERS;
		return NULL;
	}
	Realtor new_realtor = realtorCreate(EMAIL, COMPANY_NAME, TAX_PERCENTAGE,
			realtor_result);
	Map new_services = mapCopy(SERVICES);
	if (!new_realtor || !new_services) {
		*realtor_result = REALTOR_OUT_OF_MEMORY;
		return NULL;
	}
	assert(*realtor_result == REALTOR_SUCCESS);
	mapDestroy(new_realtor->services);
	new_realtor->services = new_services;
	return new_realtor;
}

RealtorResult realtorAddService(Realtor realtor, char* service_name,
		int max_apartments) {
	if (!realtor || !service_name || !MAX_APARTMENT_IS_VALID) {
		return REALTOR_INVALID_PARAMETERS;
	}
	if (serviceAlreadyExists(realtor, service_name)) {
		return REALTOR_SERVICE_ALREADY_EXISTS;
	}
	ApartmentService service = serviceCreate(max_apartments);
	if (!service) {
		return REALTOR_OUT_OF_MEMORY;
	}
	MapResult map_result = mapPut(SERVICES, service_name, service);
	serviceDestroy(service);
	return mapResult2RealtorResult(map_result);
}

RealtorResult realtorRemoveService(Realtor realtor, char* service_name) {
	if (!realtor || !service_name) {
		return REALTOR_INVALID_PARAMETERS;
	}
	return mapResult2RealtorResult(mapRemove(SERVICES, service_name));
}

RealtorResult realtorAddApartment(Realtor realtor, char* service_name,
		int apartment_id, int price, int width, int height, char* matrix) {
	if (!realtor || !service_name || !matrix || !ID_IS_VALID || !PRICE_IS_VALID
			|| !WIDTH_IS_VALID || !HEIGHT_IS_VALID
			|| !matrixIsValid(matrix, width, height)) {
		return REALTOR_INVALID_PARAMETERS;
	}
	ApartmentService service = mapGet(SERVICES, service_name);
	if (!service) {
		return REALTOR_SERVICE_DOES_NOT_EXIST;
	}
	Apartment apartment = createApartmentFromMatrix(matrix, width, height,
			price);
	if (!apartment) {
		return REALTOR_OUT_OF_MEMORY;
	}
	ApartmentServiceResult service_result = serviceAddApartment(service,
			apartment, apartment_id);
	apartmentDestroy(apartment);
	return apartmentServiceResult2RealtorResult(service_result);
}

RealtorResult realtorRemoveApartment(Realtor realtor, char* service_name,
		int apartment_id) {
	if (!realtor || !service_name || !ID_IS_VALID) {
		return REALTOR_INVALID_PARAMETERS;
	}
	ApartmentService service = mapGet(SERVICES, service_name);
	if (!service) {
		return REALTOR_SERVICE_DOES_NOT_EXIST;
	}
	ApartmentServiceResult service_result = serviceDeleteById(service,
			apartment_id);
	return apartmentServiceResult2RealtorResult(service_result);
}

Apartment realtorGetApartmentById(Realtor realtor, char* service_name,
		int apartment_id, RealtorResult* realtor_result) {
	if (!realtor_result) {
		return NULL;
	}
	if (!realtor || !service_name || !ID_IS_VALID) {
		*realtor_result = REALTOR_INVALID_PARAMETERS;
		return NULL;
	}
	ApartmentService service = mapGet(SERVICES, service_name);
	if (!service) {
		*realtor_result = REALTOR_SERVICE_DOES_NOT_EXIST;
		return NULL;
	}
	Apartment apartment = NULL;
	ApartmentServiceResult service_result = serviceGetById(service,
			apartment_id, &apartment);
	*realtor_result = apartmentServiceResult2RealtorResult(service_result);
	return apartment;
}

bool realtorApartmentExists(Realtor realtor, int area, int rooms, int price) {
	assert(realtor);
	MAP_FOREACH(char*,i,SERVICES)
	{
		ApartmentService service = mapGet(SERVICES, i);
		Apartment apartment = NULL;
		serviceSearch(service, area, rooms, price, &apartment);
		if (apartment) {
			apartmentDestroy(apartment);
			return true;
		}
	}
	return false;
}

int realtorNumOfServices(Realtor realtor) {
	assert(realtor);
	return mapGetSize(SERVICES);
}

int realtorNumOfApartments(Realtor realtor) {
	assert(realtor);
	int counter = 0;
	MAP_FOREACH(char*,i,SERVICES)
	{
		ApartmentService service = mapGet(SERVICES, i);
		counter += serviceNumberOfApatments(service);
	}
	return counter;
}

char* realtorGetEmail(Realtor realtor) {
	assert(realtor);
	return copyString(EMAIL);
}

int realtorGetTaxPercentage(Realtor realtor) {
	assert(realtor);
	return TAX_PERCENTAGE;
}

int realtorGetRank(Realtor realtor) {
	assert(realtor);
	if (ZERO_APARTMENTS) {
		return 0;
	}
	int c = realtorNumOfApartments(realtor);
	int p = realtorAveragePriceOrAreaMedian(realtor, PRICE);
	int a = realtorAveragePriceOrAreaMedian(realtor, AREA);
	int rank = (1000000 * c) + p + (100000 * a);
	return rank;
}

void realtorPrint(FILE* outputChannel, Realtor realtor) {
	assert(realtor && outputChannel);
	mtmPrintRealtor(outputChannel, EMAIL, COMPANY_NAME);
}
