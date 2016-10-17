/*
 * apartment_service_test.c
 *
 *  Created on: 6 באפר 2016
 *      Author: Raz
 */

#include "aux_macros.h"
#include "apartment_service.h"

#include <stdlib.h>


bool service_test_combo() {

//	setvbuf(stdout, NULL, _IONBF, 0);
//	setvbuf(stderr, NULL, _IONBF, 0);
	bool final = true;

	SquareType** squares1 = malloc(sizeof(*squares1) * 5);
	for (int i = 0; i < 5; i++) {
		squares1[i] = malloc(sizeof(SquareType) * 3);
	}
	squares1[0][0] = EMPTY;
	squares1[0][1] = EMPTY;
	squares1[0][2] = EMPTY;
	squares1[1][0] = EMPTY;
	squares1[1][1] = EMPTY;
	squares1[1][2] = EMPTY;
	squares1[2][0] = WALL;
	squares1[2][1] = WALL;
	squares1[2][2] = WALL;
	squares1[3][0] = EMPTY;
	squares1[3][1] = EMPTY;
	squares1[3][2] = EMPTY;
	squares1[4][0] = EMPTY;
	squares1[4][1] = EMPTY;
	squares1[4][2] = EMPTY;
	Apartment apartment1 = apartmentCreate(squares1, 5, 3, 35);

	SquareType** squares2 = malloc(sizeof(*squares2) * 5);
	for (int i = 0; i < 5; i++) {
		squares2[i] = malloc(sizeof(SquareType) * 3);
	}
	squares2[0][0] = EMPTY;
	squares2[0][1] = WALL;
	squares2[0][2] = EMPTY;
	squares2[1][0] = EMPTY;
	squares2[1][1] = WALL;
	squares2[1][2] = EMPTY;
	squares2[2][0] = EMPTY;
	squares2[2][1] = WALL;
	squares2[2][2] = EMPTY;
	squares2[3][0] = EMPTY;
	squares2[3][1] = WALL;
	squares2[3][2] = EMPTY;
	squares2[4][0] = EMPTY;
	squares2[4][1] = WALL;
	squares2[4][2] = EMPTY;
	Apartment apartment2 = apartmentCreate(squares2, 5, 3, 35);

	SquareType** squares3 = malloc(sizeof(*squares3) * 5);
	for (int i = 0; i < 5; i++) {
		squares3[i] = malloc(sizeof(SquareType) * 3);
	}
	squares3[0][0] = EMPTY;
	squares3[0][1] = WALL;
	squares3[0][2] = EMPTY;
	squares3[1][0] = EMPTY;
	squares3[1][1] = WALL;
	squares3[1][2] = EMPTY;
	squares3[2][0] = WALL;
	squares3[2][1] = WALL;
	squares3[2][2] = WALL;
	squares3[3][0] = EMPTY;
	squares3[3][1] = WALL;
	squares3[3][2] = EMPTY;
	squares3[4][0] = EMPTY;
	squares3[4][1] = WALL;
	squares3[4][2] = EMPTY;
	Apartment apartment3 = apartmentCreate(squares3, 5, 3, 35);

	/* check serviceCreate */
	ApartmentService service = serviceCreate(0); // maxNumOfApartments<=0
	service = serviceCreate(3);

//  ApartmentService service2 = serviceCopy(service);



	int intResult = -1;
	int* resultPtr;
	resultPtr = &intResult;
	TEST_EQUALS(final, APARTMENT_SERVICE_NULL_ARG,
              serviceAreaMedian(NULL, resultPtr)); //null service
	TEST_EQUALS(final, APARTMENT_SERVICE_NULL_ARG,
              serviceAreaMedian(service, NULL)); //null pointer
	TEST_EQUALS(final, APARTMENT_SERVICE_NULL_ARG,
              servicePriceMedian(NULL, resultPtr)); //null service
	TEST_EQUALS(final, APARTMENT_SERVICE_NULL_ARG,
              servicePriceMedian(service, NULL)); //null pointer

	/* check serviceAddApartment */
	TEST_EQUALS(final, APARTMENT_SERVICE_NULL_ARG,
				serviceAddApartment(NULL, apartment1, 1000)); // null service
	TEST_EQUALS(final, APARTMENT_SERVICE_NULL_ARG,
				serviceAddApartment(service, NULL, 1000)); // null apartment
	TEST_EQUALS(final, APARTMENT_SERVICE_OUT_OF_BOUNDS,
					serviceAddApartment(service, apartment1, -1)); // ID<0
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS,
			serviceAddApartment(service, apartment1, 1000)); // SUCCESS
	TEST_EQUALS(final, APARTMENT_SERVICE_ALREADY_EXISTS,
				serviceAddApartment(service, apartment1, 1000)); // existed ID
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS,
				serviceAddApartment(service, apartment2, 2000)); // SUCCESS
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS,
				serviceAddApartment(service, apartment3, 3000)); // SUCCESS
	TEST_EQUALS(final, APARTMENT_SERVICE_FULL,
					serviceAddApartment(service, apartment1, 1000)); // service full

    TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS,
                servicePriceMedian(service, resultPtr));
    TEST_EQUALS(final, 2000, intResult);
    TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS,
                serviceAreaMedian(service, resultPtr));
    TEST_EQUALS(final, 10, intResult);

	/* check serviceDeleteApartment */
	TEST_EQUALS(final, APARTMENT_SERVICE_NULL_ARG,
				serviceDeleteApartment(NULL, apartment1)); // null service
	TEST_EQUALS(final, APARTMENT_SERVICE_NULL_ARG,
				serviceDeleteApartment(service, NULL)); // null apartment
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS,
			serviceDeleteApartment(service, apartment1)); // SUCCESS
	TEST_EQUALS(final, APARTMENT_SERVICE_NO_FIT,
				serviceDeleteApartment(service, apartment1)); // no fit
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS,
				serviceDeleteApartment(service, apartment2)); // SUCCESS
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS,
				serviceDeleteApartment(service, apartment3)); // SUCCESS
	TEST_EQUALS(final, APARTMENT_SERVICE_EMPTY,
				serviceDeleteApartment(service, apartment1)); // service empty


	TEST_EQUALS(final, APARTMENT_SERVICE_EMPTY,
              servicePriceMedian(service, &intResult)); // no apartments
    TEST_EQUALS(final, APARTMENT_SERVICE_EMPTY,
              serviceAreaMedian(service, &intResult)); // no apartments
	//TEST_EQUALS(final, 100, intResult);

	/* check serviceSearch */
	Apartment* nullPointer = NULL;
	Apartment apartmentRes;
	TEST_EQUALS(final, APARTMENT_SERVICE_NULL_ARG,
					serviceSearch(NULL, 50, 50, 50, &apartmentRes)); // null service
	TEST_EQUALS(final, APARTMENT_SERVICE_NULL_ARG,
						serviceSearch(service, 50, 50, 50, nullPointer)); // null pointer
	TEST_EQUALS(final, APARTMENT_SERVICE_OUT_OF_BOUNDS,
				serviceSearch(service, -1, 50, 50, &apartmentRes)); // area<0
	TEST_EQUALS(final, APARTMENT_SERVICE_OUT_OF_BOUNDS,
					serviceSearch(service, 50, -1, 50, &apartmentRes)); // rooms<0
	TEST_EQUALS(final, APARTMENT_SERVICE_OUT_OF_BOUNDS,
						serviceSearch(service, 50, 50, -1, &apartmentRes)); // price<0
	TEST_EQUALS(final, APARTMENT_SERVICE_EMPTY,
			serviceSearch(service, 50, 50, 50, &apartmentRes)); // empty service
	serviceAddApartment(service, apartment1, 1000);
	TEST_EQUALS(final, APARTMENT_SERVICE_NO_FIT,
				serviceSearch(service, 13, 2, 35, &apartmentRes)); // area to high
	TEST_EQUALS(final, APARTMENT_SERVICE_NO_FIT,
						serviceSearch(service, 12, 3, 35, &apartmentRes)); // rooms too high
	TEST_EQUALS(final, APARTMENT_SERVICE_NO_FIT,
					serviceSearch(service, 12, 3, 34, &apartmentRes)); // price too low
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS,
					serviceSearch(service, 12, 2, 35, &apartmentRes)); // just right..

	/* check serviceNumberOfApartments */
	TEST_EQUALS(final, 1, serviceNumberOfApatments(service));

	/* check serviceGetById */
	Apartment apartmentRes2;
	TEST_EQUALS(final, APARTMENT_SERVICE_NULL_ARG,
					serviceGetById(NULL, 1000, &apartmentRes2)); // null service
	TEST_EQUALS(final, APARTMENT_SERVICE_NULL_ARG,
						serviceGetById(service, 1000, nullPointer)); // null pointer
	TEST_EQUALS(final, APARTMENT_SERVICE_OUT_OF_BOUNDS,
				serviceGetById(service, -1, &apartmentRes2)); // ID<0
	TEST_EQUALS(final, APARTMENT_SERVICE_NO_FIT,
			serviceGetById(service, 2000, &apartmentRes2)); // no fit
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS,
				serviceGetById(service, 1000, &apartmentRes2)); // SUCCESS
	serviceDeleteApartment(service, apartment1);
	TEST_EQUALS(final, APARTMENT_SERVICE_EMPTY,
					serviceGetById(service, 1000, &apartmentRes2)); // empty service

	/* check serviceDeleteById */
	TEST_EQUALS(final, APARTMENT_SERVICE_NULL_ARG,
					serviceDeleteById(NULL, 1000)); // null service
	TEST_EQUALS(final, APARTMENT_SERVICE_OUT_OF_BOUNDS,
				serviceDeleteById(service, -1)); // ID<0
	TEST_EQUALS(final, APARTMENT_SERVICE_EMPTY,
				serviceDeleteById(service, 1000)); // empty service
	serviceAddApartment(service, apartment1, 1000);
	TEST_EQUALS(final, APARTMENT_SERVICE_NO_FIT,
				serviceDeleteById(service, 2000)); // no fit
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS,
			serviceDeleteById(service, 1000)); // SUCCESS!


	/* check serviceDestroy */
	serviceDestroy(NULL); // null service
	serviceDestroy(service);
	apartmentDestroy(apartmentRes);
	apartmentDestroy(apartmentRes2);
	apartmentDestroy(apartment1);
	apartmentDestroy(apartment2);
	apartmentDestroy(apartment3);
	for (int i=0; i<5; i++){
		free(squares1[i]);
		free(squares2[i]);
		free(squares3[i]);
	}
	free(squares1);
	free(squares2);
	free(squares3);
//  serviceDestroy(service2);

	return final;
}

int main() {
	RUN_TEST(service_test_combo);
	return 0;
}

