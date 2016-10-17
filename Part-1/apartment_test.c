/*
 * tmp.c
 *
 *  Created on: 7 באפר 2016
 *      Author: Raz
 */

#include "aux_macros.h"
#include "apartment.h"

#include <stdlib.h>
#include <assert.h>

bool apartment_combo_test() {
	bool final = true;

	/* check apartmentCreate */
	SquareType** squares = malloc(sizeof(*squares) * 5);
	for (int i = 0; i < 5; i++) {
		squares[i] = malloc(sizeof(SquareType) * 3);
	}
	squares[0][0] = EMPTY;
	squares[0][1] = WALL;
	squares[0][2] = EMPTY;
	squares[1][0] = EMPTY;
	squares[1][1] = WALL;
	squares[1][2] = EMPTY;
	squares[2][0] = WALL;
	squares[2][1] = WALL;
	squares[2][2] = WALL;
	squares[3][0] = EMPTY;
	squares[3][1] = WALL;
	squares[3][2] = EMPTY;
	squares[4][0] = EMPTY;
	squares[4][1] = WALL;
	squares[4][2] = EMPTY;

	TEST_EQUALS(final, NULL, apartmentCreate(NULL, 1, 1, 1)); // null squares
	TEST_EQUALS(final, NULL, apartmentCreate(squares, 0, 1, 1)); // length<=0
	TEST_EQUALS(final, NULL, apartmentCreate(squares, 1, 0, 1)); // width<=0
	TEST_EQUALS(final, NULL, apartmentCreate(squares, 1, 1, -1)); // price<0
	Apartment apartment = apartmentCreate(squares, 5, 3, 35); //SUCCESS!

	/* check apartmentGetLength + apartmentGetWidth + apartmentTotalArea */
	TEST_EQUALS(final, 5, apartmentGetLength(apartment));
	TEST_EQUALS(final, 3, apartmentGetWidth(apartment));
	TEST_EQUALS(final, 35, apartmentGetPrice(apartment));
	TEST_EQUALS(final, 8, apartmentTotalArea(apartment));

	/* check apartmentNumOfRooms */
	TEST_EQUALS(final, 4, apartmentNumOfRooms(apartment));

	/* check apartmentGetSquare */
	SquareType squareVal;
	TEST_EQUALS(final, APARTMENT_NULL_ARG,
			apartmentGetSquare(NULL,0,0,&squareVal)); //NULL apartment
	TEST_EQUALS(final, APARTMENT_NULL_ARG,
			apartmentGetSquare(apartment,0,0,NULL)); //NULL pointer
	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS,
			apartmentGetSquare(apartment, -2, 0, &squareVal)); //out of bounds
	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS,
			apartmentGetSquare(apartment, 3, 6, &squareVal)); //out of bounds
	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS,
			apartmentGetSquare(apartment, 2, -2, &squareVal)); //out of bounds
	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS,
			apartmentGetSquare(apartment, 10, 3, &squareVal)); //out of bounds
	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS,
			apartmentGetSquare(apartment, -2, 9, &squareVal)); //out of bounds
	TEST_EQUALS(final, APARTMENT_SUCCESS,
			apartmentGetSquare(apartment, 0, 1, &squareVal));
	TEST_EQUALS(final, WALL, squareVal); // check result

	/* check apartmentSetSquare */
	SquareType squareValResult;
	TEST_EQUALS(final, APARTMENT_NULL_ARG,
			apartmentSetSquare(NULL,0,0,squareVal)); //NULL apartment
	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS,
			apartmentSetSquare(apartment, -2, 0, squareVal)); //out of bounds
	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS,
			apartmentSetSquare(apartment, 3, 6, squareVal)); //out of bounds
	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS,
			apartmentSetSquare(apartment, 2, -2, squareVal)); //out of bounds
	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS,
			apartmentSetSquare(apartment, 10, 3, squareVal)); //out of bounds
	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS,
			apartmentSetSquare(apartment, -2, 9, squareVal)); //out of bounds
	TEST_EQUALS(final, APARTMENT_OLD_VALUE,
			apartmentSetSquare(apartment, 1, 1, squareVal)); // same value
	TEST_EQUALS(final, APARTMENT_SUCCESS,
			apartmentSetSquare(apartment, 0, 0, squareVal)); // set WALL
	apartmentGetSquare(apartment, 0, 0, &squareValResult);
	TEST_EQUALS(final, WALL, squareValResult); // check result
	squareVal = EMPTY;
	TEST_EQUALS(final, APARTMENT_SUCCESS,
			apartmentSetSquare(apartment, 0, 0, squareVal)); // set EMPTY
	apartmentGetSquare(apartment, 0, 0, &squareValResult);
	TEST_EQUALS(final, EMPTY, squareValResult); // check result

	/* check apartmentRoomArea */
	int area;
	TEST_EQUALS(final, APARTMENT_NULL_ARG, apartmentRoomArea(NULL,0,0,&area)); //NULL apartment
	TEST_EQUALS(final, APARTMENT_NULL_ARG,
			apartmentRoomArea(apartment,0,0,NULL)); //NULL pointer
	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS,
			apartmentRoomArea(apartment, -2, 0, &area)); //out of bounds
	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS,
			apartmentRoomArea(apartment, 3, 6, &area)); //out of bounds
	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS,
			apartmentRoomArea(apartment, 2, -2, &area)); //out of bounds
	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS,
			apartmentRoomArea(apartment, 10, 3, &area)); //out of bounds
	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS,
			apartmentRoomArea(apartment, -2, 9, &area)); //out of bounds
	TEST_EQUALS(final, APARTMENT_NO_ROOM,
			apartmentRoomArea(apartment, 2, 1, &area)); //square is a WALL
	TEST_EQUALS(final, APARTMENT_SUCCESS,
			apartmentRoomArea(apartment, 3, 0, &area));
	TEST_EQUALS(final, 2, area); // check result

	/* check apartmentIsSameRoom */
	bool result;
	TEST_EQUALS(final, APARTMENT_NULL_ARG,
			apartmentIsSameRoom(NULL, 0, 0, 0, 0, &result)); // null apartment
	TEST_EQUALS(final, APARTMENT_NULL_ARG,
			apartmentIsSameRoom(apartment, 0, 0, 0, 0, NULL)); // null pointer
	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS,
			apartmentIsSameRoom(apartment, -1, 0, 0, 0, &result)); // row1<0
	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS,
			apartmentIsSameRoom(apartment, 1, -1, 0, 0, &result)); // col1<0
	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS,
			apartmentIsSameRoom(apartment, 1, 1, -1, 0, &result)); // row2<0
	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS,
			apartmentIsSameRoom(apartment, 1, 1, 1, -1, &result)); //col2<0
	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS,
			apartmentIsSameRoom(apartment, 5, 0, 0, 0, &result)); // row1>=length
	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS,
			apartmentIsSameRoom(apartment, 1, 3, 0, 0, &result)); // col1>=width
	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS,
			apartmentIsSameRoom(apartment, 1, 1, 5, 0, &result)); // row2>=length
	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS,
			apartmentIsSameRoom(apartment, 1, 1, 1, 3, &result)); // col2>=width
	TEST_EQUALS(final, APARTMENT_NO_ROOM,
			apartmentIsSameRoom(apartment, 1, 1, 0, 0, &result)); // square1=wall
	TEST_EQUALS(final, APARTMENT_NO_ROOM,
			apartmentIsSameRoom(apartment, 0, 0, 1, 1, &result)); // square2=wall
	TEST_EQUALS(final, APARTMENT_SUCCESS,
			apartmentIsSameRoom(apartment, 0, 0, 1, 0, &result)); // same room
	TEST_EQUALS(final, true, result); // check result
	TEST_EQUALS(final, APARTMENT_SUCCESS,
			apartmentIsSameRoom(apartment, 4, 2, 0, 0, &result)); // not same room
	TEST_EQUALS(final, false, result); // check result

	/* check apartmentSplit */
	Apartment split1, split2, split3, split4;
	TEST_EQUALS(final, APARTMENT_NULL_ARG,
			apartmentSplit(NULL, false, 1, &split1, &split2)); // null apartment
	TEST_EQUALS(final, APARTMENT_NULL_ARG,
			apartmentSplit(apartment, false, 1, NULL, &split2)); // null pointer1
	TEST_EQUALS(final, APARTMENT_NULL_ARG,
			apartmentSplit(apartment, false, 1, &split1, NULL)); // null pointer2
	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS,
			apartmentSplit(apartment, true, -1, &split1, &split2)); // index row<0
	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS,
			apartmentSplit(apartment, true, 5, &split1, &split2)); // index row>=length
	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS,
			apartmentSplit(apartment, false, -1, &split1, &split2)); // index col<0
	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS,
			apartmentSplit(apartment, false, 3, &split1, &split2)); // index col>=width
	TEST_EQUALS(final, APARTMENT_BAD_SPLIT,
			apartmentSplit(apartment, true, 0, &split1, &split2)); // index row=0
	TEST_EQUALS(final, APARTMENT_BAD_SPLIT,
			apartmentSplit(apartment, false, 0, &split1, &split2)); // index col=0
	TEST_EQUALS(final, APARTMENT_BAD_SPLIT,
			apartmentSplit(apartment, true, 4, &split1, &split2)); // index row=length-1
	TEST_EQUALS(final, APARTMENT_BAD_SPLIT,
			apartmentSplit(apartment, false, 2, &split1, &split2)); // index col=width-1
	TEST_EQUALS(final, APARTMENT_BAD_SPLIT,
			apartmentSplit(apartment, true, 1, &split1, &split2)); // not all walls
	TEST_EQUALS(final, APARTMENT_SUCCESS,
			apartmentSplit(apartment, true, 2, &split1, &split2)); // split by row
	TEST_EQUALS(final, APARTMENT_SUCCESS,
			apartmentSplit(apartment, false, 1, &split3, &split4));
	// price modification check:
	TEST_EQUALS(final, 21, apartmentGetPrice(split1));
	TEST_EQUALS(final, 21, apartmentGetPrice(split2));
	TEST_EQUALS(final, 23, apartmentGetPrice(split3));
	TEST_EQUALS(final, 23, apartmentGetPrice(split4));

	/* check apartmentChangePrice + apartmentGetPrice */
	TEST_EQUALS(final, APARTMENT_NULL_ARG, apartmentChangePrice(NULL, 10)); // null apartment
	TEST_EQUALS(final, APARTMENT_PRICE_NOT_IN_RANGE,
			apartmentChangePrice(apartment, -101)); // percent < -100
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentChangePrice(apartment, -50));
	TEST_EQUALS(final, 18, apartmentGetPrice(apartment)); // check result

	/* check apartmentCopy+apartmentIsIdentical */
	assert(apartmentCopy(NULL) == NULL); // null apartment
	Apartment copy = apartmentCopy(apartment);
	TEST_EQUALS(final, true, apartmentIsIdentical(NULL, NULL)); // 2 nulls
	TEST_EQUALS(final, false, apartmentIsIdentical(apartment, NULL)); // 1 null
	TEST_EQUALS(final, false, apartmentIsIdentical(apartment, split1)); // different
	TEST_EQUALS(final, false, apartmentIsIdentical(apartment, split3)); // different
	TEST_EQUALS(final, false, apartmentIsIdentical(split1, split3)); // different
	TEST_EQUALS(final, true, apartmentIsIdentical(apartment, copy)); // identical
	TEST_EQUALS(final, true, apartmentIsIdentical(split1, split2)); // identical
	TEST_EQUALS(final, true, apartmentIsIdentical(split3, split4)); // identical

	/* check apartmentDestroy */
	apartmentDestroy(NULL); // null apartment
	apartmentDestroy(apartment);
	apartmentDestroy(copy);
	apartmentDestroy(split1);
	apartmentDestroy(split2);
	apartmentDestroy(split3);
	apartmentDestroy(split4);

	for (int i = 0; i < 5; i++) {
		free(squares[i]);
	}
	free(squares);

	return final;
}

int main() {

//	setvbuf(stdout, NULL, _IONBF, 0);
//	setvbuf(stderr, NULL, _IONBF, 0);

	RUN_TEST(apartment_combo_test);
	return true;
}

