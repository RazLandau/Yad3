/*
 * tmp.c
 *
 *  Created on: 7 באפר 2016
 *      Author: Raz
 */

#include "apartment.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#define MIN_PERCENT -100

//void printSquares(SquareType** squares, int length, int width) {
//	assert(squares != NULL);
//	for (int i = 0; i < length; i++) {
//		printf("\n");
//		for (int j = 0; j < width; j++) {
//			printf("%d", squares[i][j]);
//		}
//	}
//	printf("\n");
//}
//
//void printApartment(Apartment apartment) {
//	assert(apartment != NULL);
//	printSquares(apartment->squares, apartment->length, apartment->width);
//	printf("price is %d \n", apartmentGetPrice(apartment));
//	printf("length is %d \n", apartmentGetLength(apartment));
//	printf("width is %d \n", apartmentGetWidth(apartment));
//	printf("total area is %d \n", apartmentTotalArea(apartment));
//}

/*****************************************************************************/

/*
 * This is a recursive function that receives an apartment and a certain
 * square in that apartment, and maps the the entire room which it is in.
 * it eventually returns the total amount of empty squares in the room.
 */
static int mapTheRoom(Apartment apartment, int row, int col, int roomSpace) {
	assert(apartment != NULL);
	if (row < 0 || row >= apartment->length || col < 0
			|| col >= apartment->width
			|| apartment->squares[row][col] == WALL) {
		return roomSpace;
	}
	apartment->squares[row][col] = WALL;
	roomSpace++;
	roomSpace = mapTheRoom(apartment, row - 1, col, roomSpace);
	roomSpace = mapTheRoom(apartment, row, col + 1, roomSpace);
	roomSpace = mapTheRoom(apartment, row + 1, col, roomSpace);
	roomSpace = mapTheRoom(apartment, row, col - 1, roomSpace);
	return roomSpace;
}

/*****************************************************************************/

/*
 * Copied partial/full given matrix oldSquares to given matrix newSquares from
 * start point [i1][j1] to end point [i2][j2].
 * @return
 * false- in case of a memory allocation failure.
 * true- if copied successfully.
 */
static bool copySquares(SquareType** oldSquares, SquareType** newSquares,
		int i1, int j1, int i2, int j2) {
	for (int i = 0; i < i2 - i1; i++) {
		newSquares[i] = (SquareType*) malloc(sizeof(SquareType) * (j2 - j1));
		if (newSquares[i] == NULL) {
			for (int j = 0; j <= i; j++) {
				free(newSquares[j]);
			}
			return false;
		}
	}
	for (int i = 0; i < i2 - i1; i++) {
		for (int j = 0; j < j2 - j1; j++) {
			newSquares[i][j] = oldSquares[i + i1][j + j1];
		}
	}
	return true;
}

/*****************************************************************************/

/*
 * Checks if given 2 matrixes are identical.
 * Assumes length and width are identical.
 * @return
 * false- if different.
 * true- if identical.
 */
static bool squaresIsIdentical(SquareType** squares1, SquareType** squares2,
		int length, int width) {
	for (int i = 0; i < length; i++) {
		for (int j = 0; j < width; j++) {
			if (squares1[i][j] != squares2[i][j]) {
				return false;
			}
		}
	}
	return true;
}

/*****************************************************************************/

/*
 * Recursively checks if there is a path in given matrix with given length&width
 * from point [i][j] to point [row][col], with only UP, DOWN, RIGHT, LEFT valid.
 * Doesn't check if given end point is in the matrix.
 * Doesn't delete back the matrix to previous condition.
 * @return
 * false- if path is not found or if given start point isn't in the matrix.
 * true- if path is found.
 */
static bool isPath(SquareType** matrix, int length, int width, int i, int j,
		int row, int col) {
	if (i >= length || j >= width || i < 0 || j < 0) {
		return false;
	}
	if (matrix[i][j] == WALL) {
		return false;
	}
	if (i == row && j == col) {
		return true;
	}
	matrix[i][j] = WALL;
	return (isPath(matrix, length, width, i++, j, row, col)
			|| isPath(matrix, length, width, i--, j, row, col)
			|| isPath(matrix, length, width, i, j++, row, col)
			|| isPath(matrix, length, width, i, j--, row, col));
	return false;
}

/*****************************************************************************/

Apartment apartmentCreate(SquareType** squares, int length, int width,
		int price) {
//	printf("\n apartmentCreate \n");
	if (squares == NULL || length <= 0 || width <= 0 || price < 0) {
		return NULL;
	}
	Apartment newApartment = malloc(sizeof(*newApartment));
	if (newApartment == NULL) {
		return NULL;
	}
	SquareType** newSquares = malloc(sizeof(*newSquares) * length);
	if (newSquares == NULL) {
		free(newApartment);
		return NULL;
	}
	if (!copySquares(squares, newSquares, 0, 0, length, width)) {
		free(newSquares);
		free(newApartment);
		return NULL;
	}
	newApartment->squares = newSquares;
	newApartment->length = length;
	newApartment->width = width;
	newApartment->price = price;
	return newApartment;
}

/*****************************************************************************/

int apartmentGetPrice(Apartment apartment) {
//	printf("\n apartmentGetPrice \n");
	assert(apartment != NULL);
	return apartment->price;
}

/*****************************************************************************/

int apartmentGetWidth(Apartment apartment) {
//	printf("\n apartmentGetWidth \n");
	assert(apartment != NULL);
	return apartment->width;
}

/*****************************************************************************/

int apartmentGetLength(Apartment apartment) {
//	printf("\n apartmentGetLength \n");
	assert(apartment != NULL);
	return apartment->length;
}

/*****************************************************************************/

int apartmentTotalArea(Apartment apartment) {
//	printf("\n apartmentTotalArea \n");
	assert(apartment != NULL);
	int counter = 0;
	for (int i = 0; i < apartment->length; i++) {
		for (int j = 0; j < apartment->width; j++) {
			if (apartment->squares[i][j] == EMPTY) {
				counter++;
			}
		}
	}
	return counter;
}

/*****************************************************************************/

ApartmentResult apartmentChangePrice(Apartment apartment, int percent) {
//	printf("\n apartmentChangePrice \n");
	if (apartment == NULL) {
		return APARTMENT_NULL_ARG;
	}
	if (percent < MIN_PERCENT) {
		return APARTMENT_PRICE_NOT_IN_RANGE;
	}
	int oldPrice = apartment->price;
	int newPrice = oldPrice + (int) (oldPrice * (percent / 100.0));
	apartment->price = newPrice;
	return APARTMENT_SUCCESS;
}

/*****************************************************************************/

bool apartmentIsIdentical(Apartment apartment1, Apartment apartment2) {
//	printf("\n apartmentIsIdentical \n");
	if (apartment1 == NULL && apartment2 == NULL) {
		return true;
	}
	if (apartment1 == NULL || apartment2 == NULL) {
		return false;
	}
	int price1 = apartment1->price, price2 = apartment2->price, length1 =
			apartment1->length, length2 = apartment2->length, width1 =
			apartment1->width, width2 = apartment2->width;
	if (price1 != price2 || length1 != length2 || width1 != width2) {
		return false;
	}
	return squaresIsIdentical(apartment1->squares, apartment2->squares, length1,
			width2);
}

/*****************************************************************************/

ApartmentResult apartmentGetSquare(Apartment apartment, int row, int col,
		SquareType* outValue) {
//	printf("\napartmentGetSquare\n");

	if (apartment == NULL || outValue == NULL) {
		return APARTMENT_NULL_ARG;
	}
	if (row < 0 || row >= apartment->length || col < 0
			|| col >= apartment->width) {
		return APARTMENT_OUT_OF_BOUNDS;
	}
	*outValue = apartment->squares[row][col];
	return APARTMENT_SUCCESS;
}

/*****************************************************************************/

ApartmentResult apartmentSetSquare(Apartment apartment, int row, int col,
		SquareType value) {
//	printf("\n apartmentSetSquare \n");

	if (apartment == NULL) {
		return APARTMENT_NULL_ARG;
	}
	if (row < 0 || row >= apartment->length || col < 0
			|| col >= apartment->width) {
		return APARTMENT_OUT_OF_BOUNDS;
	}
	if (apartment->squares[row][col] == value) {
		return APARTMENT_OLD_VALUE;
	}
	apartment->squares[row][col] = value;
	return APARTMENT_SUCCESS;
}

/*****************************************************************************/

Apartment apartmentCopy(Apartment apartment) {
//	printf("\n apartmentCopy \n");

	if (apartment == NULL) {
		return NULL;
	}
	Apartment copy = apartmentCreate(apartment->squares, apartment->length,
			apartment->width, apartment->price);
	return copy;
}

/*****************************************************************************/

ApartmentResult apartmentSplit(Apartment apartment, bool splitByRow, int index,
		Apartment* first, Apartment* second) {
//	printf("\n apartmentSplit \n");
	if (apartment == NULL || first == NULL || second == NULL) {
		return APARTMENT_NULL_ARG;
	}
	int length = apartment->length, width = apartment->width, price =
			apartment->price;
	SquareType** squares = apartment->squares;
	if (index < 0 || index >= (splitByRow ? length : width)) {
		return APARTMENT_OUT_OF_BOUNDS;
	}
	if (index == 0 || index == ((splitByRow ? length : width) - 1)) {
		return APARTMENT_BAD_SPLIT;
	}
	for (int i = 0; i < (splitByRow ? width : length); i++) {
		if (squares[(splitByRow ? index : i)][(splitByRow ? i : index)] != 1) {
			return APARTMENT_BAD_SPLIT;
		}
	}
	SquareType** squares1 = malloc(
			sizeof(*squares1) * (splitByRow ? index : length));
	if (!copySquares(squares, squares1, 0, 0, (splitByRow ? index : length),
			(splitByRow ? width : index))) {
		free(squares1);
		return APARTMENT_OUT_OF_MEM;
	}
	int price1 = (price) * (index + 1) / (splitByRow ? length : width);
	Apartment apartment1 = apartmentCreate(squares1,
			(splitByRow ? index : length), (splitByRow ? width : index),
			price1);
	for (int i = 0; i < (splitByRow ? index : length); i++) {
		free(squares1[i]);
	}
	free(squares1);
	if (apartment1 == NULL) {
		return APARTMENT_OUT_OF_MEM;
	}
	SquareType** squares2 = malloc(
			sizeof(*squares2) * (splitByRow ? length - index - 1 : length));
	if (squares2 == NULL) {
		apartmentDestroy(apartment1);
		return APARTMENT_OUT_OF_MEM;
	}
	if (!copySquares(squares, squares2, (splitByRow ? index + 1 : 0),
			(splitByRow ? 0 : index + 1), length, width)) {
		free(squares2);
		apartmentDestroy(apartment1);
	}
	int price2 = (price) * ((splitByRow ? length : width) - index)
			/ (splitByRow ? length : width);
	Apartment apartment2 = apartmentCreate(squares2,
			(splitByRow ? length - index - 1 : length),
			(splitByRow ? width : width - index - 1), price2);
	for (int i = 0; i < (splitByRow ? length - index - 1 : length); i++) {
		free(squares2[i]);
	}
	free(squares2);
	if (apartment2 == NULL) {
		apartmentDestroy(apartment1);
	}
	*first = apartment1;
	*second = apartment2;
	return APARTMENT_SUCCESS;

}

/*****************************************************************************/

ApartmentResult apartmentIsSameRoom(Apartment apartment, int row1, int col1,
		int row2, int col2, bool* outResult) {
//	printf("\n apartmentIsSameRoom \n");
	if (apartment == NULL || outResult == NULL) {
		return APARTMENT_NULL_ARG;
	}
//	printf("\n%d%d,%d%d", row1, row2, col1, col2);
//	printApartment(apartment);
	int length = apartment->length, width = apartment->width;
	if (row1 >= length || row1 < 0 || col1 >= width || col1 < 0
			|| row2 >= length || row2 < 0 || col2 >= width || col2 < 0) {
		return APARTMENT_OUT_OF_BOUNDS;
	}
	if (apartment->squares[row1][col1] == WALL
			|| apartment->squares[row2][col2] == WALL) {
		return APARTMENT_NO_ROOM;
	}
	SquareType** tempSquares = malloc(sizeof(*tempSquares) * length);
	if (!copySquares(apartment->squares, tempSquares, 0, 0, length, width)) {
		free(tempSquares);
		return APARTMENT_OUT_OF_MEM;
	}
	if (isPath(tempSquares, length, width, row1, col1, row2, col2) == false) {
		*outResult = false;
	} else {
		*outResult = true;
	}
	for (int i = 0; i < length; i++) {
		free(tempSquares[i]);
	}
	free(tempSquares);
	return APARTMENT_SUCCESS;
}

/*****************************************************************************/

ApartmentResult apartmentRoomArea(Apartment apartment, int row, int col,
		int* outArea) {
//	printf("\n apartmentRoomArea \n");
	if (apartment == NULL || outArea == NULL) {
		return APARTMENT_NULL_ARG;
	}
	if (row < 0 || row >= apartment->length || col < 0
			|| col >= apartment->width) {
		return APARTMENT_OUT_OF_BOUNDS;
	}
	if (apartment->squares[row][col] == WALL) {
		return APARTMENT_NO_ROOM;
	}
	Apartment tempApartment = apartmentCopy(apartment);
	if (tempApartment == NULL) {
		return APARTMENT_OUT_OF_MEM;
	}
	int roomSpace = 0;
	*outArea = mapTheRoom(tempApartment, row, col, roomSpace);
	apartmentDestroy(tempApartment);
	return APARTMENT_SUCCESS;
}

/*****************************************************************************/

int apartmentNumOfRooms(Apartment apartment) {
//	printf("\n apartmentNumOfRooms \n");
	assert(apartment != NULL);
	Apartment tempApartment = apartmentCopy(apartment);
	if (tempApartment == NULL) {
		return -1;
	}
	int numberOfRooms = 0;
	for (int i = 0; i < tempApartment->length; i++) {
		for (int j = 0; j < tempApartment->width; j++) {
			if (tempApartment->squares[i][j] == EMPTY
					&& mapTheRoom(tempApartment, i, j, 0)) {
				numberOfRooms++;
			}
		}
	}
	apartmentDestroy(tempApartment);
	return numberOfRooms;
}

/*****************************************************************************/

void apartmentDestroy(Apartment apartment) {
//	printf("\n apartmentDestroy \n");
	if (apartment != NULL) {
		for (int i = 0; i < apartment->length; i++) {
			free(apartment->squares[i]);
		}
		free(apartment->squares);
		free(apartment);
	}
}

