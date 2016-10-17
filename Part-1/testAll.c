/*
 * testAll.c
 *
 *  Created on: Apr 5, 2016
 *      Author: mohamad
 */


#include "aux_macros.h"
#include "apartment_service.h"

#include <stdlib.h>

bool service_test_combo() {
	bool final = true;
	SquareType square[3] = { EMPTY, EMPTY, EMPTY};
	SquareType square2[3] = { EMPTY, EMPTY, EMPTY};
	SquareType square3[3] = { EMPTY, EMPTY, EMPTY};
	SquareType square4[3] = { EMPTY, EMPTY, EMPTY};


		SquareType* squares1[4] = { square, square2, square3, square4 };
	    squares1[2][2] = WALL;
		squares1[2][1] = WALL;
		squares1[2][0] = WALL;
		squares1[3][1] = WALL;
		squares1[0][2] = WALL;
		squares1[1][1] = WALL;
	TEST_EQUALS(final, NULL, apartmentCreate(NULL,2,1,10));
	TEST_EQUALS(final, NULL, apartmentCreate(squares1, -2, 1, 20));
	TEST_EQUALS(final, NULL, apartmentCreate(squares1, 2, 0, 30));
	TEST_EQUALS(final, NULL, apartmentCreate(squares1, 2, 5, -50));


	bool out1;
	Apartment apartment1 = apartmentCreate(squares1, 4, 3, 150);
	TEST_EQUALS(final, APARTMENT_NULL_ARG,
			apartmentIsSameRoom(NULL,5,3,2,1,&out1));
	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS,
			apartmentIsSameRoom(apartment1, 5, 3, 2, 1, &out1));
	TEST_EQUALS(final, APARTMENT_NO_ROOM,
			apartmentIsSameRoom(apartment1, 0, 0, 2, 1, &out1));
	TEST_EQUALS(final, APARTMENT_SUCCESS,
			apartmentIsSameRoom(apartment1, 0, 0, 1, 2, &out1));

	TEST_EQUALS(final, false, out1);
	TEST_EQUALS(final, APARTMENT_SUCCESS,
			apartmentIsSameRoom(apartment1, 0, 0, 1, 0, &out1));
	TEST_EQUALS(final, 6, apartmentTotalArea(apartment1));
	int num1;
	TEST_EQUALS(final, APARTMENT_NULL_ARG, apartmentRoomArea(NULL,1,0,&num1));
	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS,
			apartmentRoomArea(apartment1, 1, -1, &num1));
	TEST_EQUALS(final, APARTMENT_NO_ROOM,
			apartmentRoomArea(apartment1, 0, 2, &num1));
	TEST_EQUALS(final, APARTMENT_SUCCESS,
			apartmentRoomArea(apartment1, 1, 2, &num1));
	TEST_EQUALS(final, 1, num1);

	TEST_EQUALS(final, APARTMENT_SUCCESS,
			apartmentRoomArea(apartment1, 0, 0, &num1));
	TEST_EQUALS(final, 3, num1);

	Apartment first;
	Apartment second;

	out1 = true;
	TEST_EQUALS(final, APARTMENT_NULL_ARG,
			apartmentSplit(NULL,out1,2,&first,&second));
	TEST_EQUALS(final, APARTMENT_BAD_SPLIT,
			apartmentSplit(apartment1, out1, 1, &first, &second));
	TEST_EQUALS(final, APARTMENT_SUCCESS,
			apartmentSplit(apartment1, out1, 2, &first, &second));
	out1 = true;

	TEST_EQUALS(final, APARTMENT_SUCCESS,
			apartmentIsSameRoom(second, 0, 0, 0, 2, &out1));

	TEST_EQUALS(final, false, out1);
	out1 = false;
	TEST_EQUALS(final, APARTMENT_SUCCESS,
			apartmentIsSameRoom(first, 0, 0, 0, 1, &out1));
	TEST_EQUALS(final, true, out1);
	TEST_EQUALS(final, 2, apartmentNumOfRooms(first));
	TEST_EQUALS(final, 2, apartmentNumOfRooms(second));
	TEST_EQUALS(final, 4, apartmentNumOfRooms(apartment1));
	TEST_EQUALS(final, true, apartmentIsIdentical(NULL,NULL));
	TEST_EQUALS(final, false, apartmentIsIdentical(first,NULL));
	TEST_EQUALS(final, true, apartmentIsIdentical(second, second));
	TEST_EQUALS(final, NULL, apartmentCopy(NULL));

	Apartment firstcopy = apartmentCopy(first);
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentSetSquare(first, 0, 1, WALL));
	TEST_EQUALS(final, false, apartmentIsIdentical(first, firstcopy));
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentChangePrice(first, 50));
	TEST_DIFFERENT(final, apartmentGetPrice(first),
			apartmentGetPrice(firstcopy));
	///////////////////////////////////////////////////////////////////////////////////////////////
	TEST_EQUALS(final, NULL, serviceCreate(0));
	ApartmentService service = serviceCreate(3);
	TEST_EQUALS(final, APARTMENT_SERVICE_NULL_ARG,
			serviceAddApartment(NULL,NULL,-1));
	TEST_EQUALS(final, APARTMENT_SERVICE_NULL_ARG,
			serviceAddApartment(service,NULL,-1));
	TEST_EQUALS(final, APARTMENT_SERVICE_OUT_OF_BOUNDS,
			serviceAddApartment(service, apartment1, -1));
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS,
			serviceAddApartment(service, apartment1, 5));
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS,
			serviceAddApartment(service, first, 3));
	TEST_EQUALS(final, APARTMENT_SERVICE_ALREADY_EXISTS,
			serviceAddApartment(service, first, 5));
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS,
			serviceAddApartment(service, second, 2));
	TEST_EQUALS(final, APARTMENT_SERVICE_FULL,
			serviceAddApartment(service, firstcopy, 5));
	TEST_EQUALS(final, 3, serviceNumberOfApatments(service));

	TEST_EQUALS(final, APARTMENT_SERVICE_NO_FIT,
			serviceDeleteApartment(service, firstcopy));
	TEST_EQUALS(final, 3, serviceNumberOfApatments(service));
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS, serviceDeleteById(service, 5));
	TEST_EQUALS(final, 2, serviceNumberOfApatments(service));
	serviceDestroy(service);
	apartmentDestroy(apartment1);
	apartmentDestroy(first);
	apartmentDestroy(second);
	apartmentDestroy(firstcopy);

	return final;
}

int main() {
	RUN_TEST(service_test_combo);
	// TODO: add more tests:
	// RUN_TEST(...);
	return 0;
}
