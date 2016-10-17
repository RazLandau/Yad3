/*
 * apartment_service.c
 *
 *  Created on: Apr 5, 2016
 *      Author: Lioz
 */

#include "apartment_service.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

/*
 * Finds apartment with same given id in given service that hold given number of
 * apartments.
 * Doesn't check if service, number of apartments or id are valid.
 * @return
 * -1- if no such apartment is found.
 * the index of the identical apartment in the service- if found.
 */
static int findById(ApartmentService service, int numOfApartments, int id) {
	for (int i = numOfApartments - 1; i >= 0; i--) {
		if (service->idArray[i] == id) {
			return i;
		}
	}
	return -1;

}

/******************************************************************************/

/*
 * This function is a sorting method function for sorting the arrays of prices
 * and apartment areas in order to find their median. The order is from lowest
 * to highest number.
 */
static void bubble_sort(int* array, int arraySize) {
	for (int i = 0; i < arraySize - 1; i++) {
		for (int j = 0; j < arraySize - i - 1; j++) {
			if (array[j] > array[j + 1]) {
				int temp = array[j];
				array[j] = array[j + 1];
				array[j + 1] = temp;
			}
		}
	}
}

/******************************************************************************/

/*
 * This function receives an array of integers and its size and finds the
 * median out of all the numbers in the array.
 */
static int findMedian(int* array, int arraySize) {
	bubble_sort(array, arraySize);
	if (arraySize % 2 == 1) {
		return array[arraySize / 2];
	} else {
		return ((array[(arraySize / 2) - 1] + array[arraySize / 2]) / 2);
	}
}

/*****************************************************************************/

/*
 * Deletes the apartment in the index location in given service, and closes all
 * the gaps is the service after deletion.
 * Doesn't check if service or index are valid.
 */
void static serviceDeleteIndexApartment(ApartmentService service, int index) {
	apartmentDestroy(service->apartmentArray[index]);
	for (int i = index;
			i < service->maxApartments - 1 && service->apartmentArray[i] != NULL;
			i++) {
		service->apartmentArray[i] = service->apartmentArray[i + 1];
		service->idArray[i] = service->idArray[i + 1];
	}
	service->apartmentArray[service->maxApartments - 1] = NULL;
}

/*****************************************************************************/

ApartmentService serviceCreate(int maxNumOfApartments) {
	if (maxNumOfApartments <= 0) {
		return NULL;
	}
	int* newIdArray = malloc(sizeof(*newIdArray) * maxNumOfApartments);
	if (newIdArray == NULL) {
		return NULL;
	}
	Apartment* newApartmentArray = malloc(
			sizeof(*newApartmentArray) * maxNumOfApartments);
	if (newApartmentArray == NULL) {
		free(newIdArray);
		return NULL;
	}
	ApartmentService newService = malloc(sizeof(*newService));
	if (newService == NULL) {
		free(newApartmentArray);
		free(newIdArray);
		return NULL;
	}
	for (int i = 0; i < maxNumOfApartments; i++) {
		newApartmentArray[i] = NULL;
		newIdArray[i] = -1;
	}
	newService->apartmentArray = newApartmentArray;
	newService->idArray = newIdArray;
	newService->maxApartments = maxNumOfApartments;
	return newService;
}

/******************************************************************************/

int serviceNumberOfApatments(ApartmentService service) {
	assert(service != NULL);
	int counter = 0;
	for (int i = 0; i < service->maxApartments; i++) {
		if (service->apartmentArray[i] != NULL) {
			counter++;
		}
	}
	return counter;

}

/******************************************************************************/

ApartmentServiceResult serviceAddApartment(ApartmentService service,
		Apartment apartment, int id) {
	if (service == NULL || apartment == NULL) {
		return APARTMENT_SERVICE_NULL_ARG;
	}
	if (id < 0) {
		return APARTMENT_SERVICE_OUT_OF_BOUNDS;
	}
	int numOfApartments = serviceNumberOfApatments(service);
	if (service->maxApartments == numOfApartments) {
		return APARTMENT_SERVICE_FULL;
	}
	if (findById(service, numOfApartments, id) != -1) {
		return APARTMENT_SERVICE_ALREADY_EXISTS;
	}
	Apartment copy = apartmentCopy(apartment);
	if (copy == NULL) {
		return APARTMENT_SERVICE_OUT_OF_MEM;
	}
	service->apartmentArray[numOfApartments] = copy;
	service->idArray[numOfApartments] = id;
	return APARTMENT_SERVICE_SUCCESS;
}

/******************************************************************************/

ApartmentServiceResult serviceGetById(ApartmentService service, int id,
		Apartment* outApartment) {
	if (service == NULL || outApartment == NULL) {
		return APARTMENT_SERVICE_NULL_ARG;
	}
	if (id < 0) {
		return APARTMENT_SERVICE_OUT_OF_BOUNDS;
	}
	int numOfApartments = serviceNumberOfApatments(service);
	if (numOfApartments == 0) {
		return APARTMENT_SERVICE_EMPTY;
	}
	int i = findById(service, numOfApartments, id);
	if (i == -1) {
		return APARTMENT_SERVICE_NO_FIT;
	}
	Apartment copy = apartmentCopy(service->apartmentArray[i]);
	if (copy == NULL) {
		return APARTMENT_SERVICE_OUT_OF_MEM;
	}
	*outApartment = copy;
	return APARTMENT_SERVICE_SUCCESS;
}

/******************************************************************************/

ApartmentServiceResult serviceDeleteById(ApartmentService service, int id) {
	if (service == NULL) {
		return APARTMENT_SERVICE_NULL_ARG;
	}
	if (id < 0) {
		return APARTMENT_SERVICE_OUT_OF_BOUNDS;
	}
	int numOfApartments = serviceNumberOfApatments(service);
	if (numOfApartments == 0) {
		return APARTMENT_SERVICE_EMPTY;
	}
	int i = findById(service, numOfApartments, id);
	if (i == -1) {
		return APARTMENT_SERVICE_NO_FIT;
	}
	serviceDeleteIndexApartment(service, i);
	return APARTMENT_SERVICE_SUCCESS;
}

/******************************************************************************/

ApartmentServiceResult serviceSearch(ApartmentService service, int area,
		int rooms, int price, Apartment* outApartment) {
	if (service == NULL || outApartment == NULL) {
		return APARTMENT_SERVICE_NULL_ARG;
	}
	if (area < 0 || rooms < 0 || price < 0) {
		return APARTMENT_SERVICE_OUT_OF_BOUNDS;
	}
	int numOfApartments = serviceNumberOfApatments(service);
	if (numOfApartments == 0) {
		return APARTMENT_SERVICE_EMPTY;
	}
	int checkedPrice, checkedArea, checkedRooms;
	for (int i = numOfApartments - 1; i >= 0; i--) {
		checkedPrice = apartmentGetPrice(service->apartmentArray[i]);
		checkedArea = apartmentTotalArea(service->apartmentArray[i]);
		checkedRooms = apartmentNumOfRooms(service->apartmentArray[i]);
		if (checkedPrice <= price && checkedArea >= area
				&& checkedRooms >= rooms) {
			Apartment copy = apartmentCopy(service->apartmentArray[i]);
			if (copy == NULL) {
				return APARTMENT_SERVICE_OUT_OF_MEM;
			}
			*outApartment = copy;
			return APARTMENT_SERVICE_SUCCESS;
		}
	}
	return APARTMENT_SERVICE_NO_FIT;
}

/******************************************************************************/

ApartmentServiceResult serviceDeleteApartment(ApartmentService service,
		Apartment apartment) {
	if (service == NULL || apartment == NULL) {
		return APARTMENT_SERVICE_NULL_ARG;
	}
	int numOfApartments = serviceNumberOfApatments(service);
	if (numOfApartments == 0) {
		return APARTMENT_SERVICE_EMPTY;
	}
	for (int i = numOfApartments - 1; i >= 0; i--) {
		if (apartmentIsIdentical(service->apartmentArray[i], apartment)) {
			serviceDeleteIndexApartment(service, i);
			return APARTMENT_SERVICE_SUCCESS;

		}
	}
	return APARTMENT_SERVICE_NO_FIT;
}

/******************************************************************************/

ApartmentServiceResult servicePriceMedian(ApartmentService service,
		int* outResult) {
	if (service == NULL || outResult == NULL || service->apartmentArray == NULL) {
		return APARTMENT_SERVICE_NULL_ARG;
	}
	int numberOfApartments = serviceNumberOfApatments(service);
	if (numberOfApartments == 0) {
		return APARTMENT_SERVICE_EMPTY;
	}
	int* prices = malloc(sizeof(int) * numberOfApartments);
	if (prices == NULL) {
		return APARTMENT_SERVICE_OUT_OF_MEM;
	}
	for (int i = 0; i < numberOfApartments; i++) {
		prices[i] = apartmentGetPrice(service->apartmentArray[i]);
	}
	*outResult = findMedian(prices, numberOfApartments);
	free(prices);
	return APARTMENT_SERVICE_SUCCESS;
}

/******************************************************************************/

ApartmentServiceResult serviceAreaMedian(ApartmentService service,
		int* outResult) {
	if (service == NULL || outResult == NULL || service->apartmentArray == NULL) {
		return APARTMENT_SERVICE_NULL_ARG;
	}
	int numberOfApartments = serviceNumberOfApatments(service);
	if (numberOfApartments == 0) {
		return APARTMENT_SERVICE_EMPTY;
	}
	int* areas = malloc(sizeof(int) * numberOfApartments);
	if (areas == NULL) {
		return APARTMENT_SERVICE_OUT_OF_MEM;
	}
	for (int i = 0; i < numberOfApartments; i++) {
		areas[i] = apartmentTotalArea(service->apartmentArray[i]);
	}
	*outResult = findMedian(areas, numberOfApartments);
	free(areas);
	return APARTMENT_SERVICE_SUCCESS;
}

/******************************************************************************/

ApartmentService serviceCopy(ApartmentService service) {
	if (service == NULL) {
		return NULL;
	}
	ApartmentService copy = serviceCreate(service->maxApartments);
	if (copy == NULL) {
		return NULL;
	}
	for (int i = 0; i < serviceNumberOfApatments(service); i++) {
		serviceAddApartment(copy, service->apartmentArray[i],
				service->idArray[i]);
		copy->idArray[i] = service->idArray[i];
		if (copy->apartmentArray[i] == NULL) {
			copy->maxApartments = i;
			serviceDestroy(copy);
			return NULL;
		}
	}
	return copy;
}

/******************************************************************************/

void serviceDestroy(ApartmentService service) {
	if (service != NULL) {
		for (int i = 0; i < service->maxApartments; i++) {
			apartmentDestroy(service->apartmentArray[i]);
		}
		free(service->apartmentArray);
		free(service->idArray);
	}
	free(service);
}
