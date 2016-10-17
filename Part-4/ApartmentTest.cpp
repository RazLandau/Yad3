#include <iostream>
#include "Apartment.h"
#include "MtmTst.h"

const int TEST_LENGTH1 = 3;
const int TEST_WIDTH1 = 4;
const int TEST_PRICE1 = 1;
const int TEST_TOTAL_AREA1 = 12;
const int TEST_LENGTH2 = 4;
const int TEST_WIDTH2 = 5;
const int TEST_PRICE2 = 2;
const int TEST_TOTAL_AREA2 = 20;
const int TEST_LENGTH3 = 4;
const int TEST_WIDTH3 = 4;
const int TEST_PRICE3 = 3;
const int TEST_TOTAL_AREA3 = 16;
const int TEST_LENGTH4 = 7;
const int TEST_WIDTH4 = 5;
const int TEST_PRICE4 = 4;
const int TEST_TOTAL_AREA4 = 35;
const int TEST_LENGTH5 = 3;
const int TEST_WIDTH5 = 4;
const int TEST_PRICE5 = 5;
const int TEST_LENGTH6 = 7;
const int TEST_WIDTH6 = 2;
const int TEST_PRICE6 = 6;
const int TOTAL_LENGTH = 7;
const int TOTAL_WIDTH = 4;
const int TOTAL_PRICE = 4;
const int TEST_BAD_LENGTH1 = 0;
const int TEST_BAD_WIDTH1 = 0;
const int TEST_BAD_PRICE1 = -1;
const int TEST_AREA1 = 28;
const int TEST_AREA2 = 63;
const int TEST_AREA3 = 83;
const int TOTAL_LENGTH1 = 7;
const int TOTAL_LENGTH2 = 7;
const int TOTAL_LENGTH3 = 11;
const int TOTAL_WIDTH1 = 4;
const int TOTAL_WIDTH2 = 9;
const int TOTAL_WIDTH3 = 9;
const int TOTAL_PRICE1 = 4;
const int TOTAL_PRICE2 = 8;
const int TOTAL_PRICE3 = 10;

static void deleteSquares(Apartment::SquareType** squares, int length) {
	for (int i = 0; i < length; ++i) {
		delete[] squares[i];
	}
	delete[] squares;
}

bool apartmentTestExample() {

	const int length = 3;
	const int width = 4;
	const int price = 1;
	Apartment::SquareType** squares = new Apartment::SquareType*[length];
	for (int i = 0; i < length; ++i) {
		squares[i] = new Apartment::SquareType[width];
		for (int j = 0; j < width; ++j) {
			squares[i][j] = Apartment::EMPTY;
		}
	}
	ASSERT_NO_THROW(Apartment(squares, length, width, price));
	const Apartment apt(squares, length, width, price);
	ASSERT_EQUALS(length, apt.getLength());
	Apartment apt2(apt);
	ASSERT_NO_THROW(apt2 += apt);
	ASSERT_EQUALS(true, apt < apt2);
	deleteSquares(squares, length);
	return true;
}

bool testApartmentConstructor() {

	Apartment::SquareType** squares = new Apartment::SquareType*[TEST_LENGTH1];
	for (int i = 0; i < TEST_LENGTH1; ++i) {
		squares[i] = new Apartment::SquareType[TEST_WIDTH1];
		for (int j = 0; j < TEST_WIDTH1; ++j) {
			squares[i][j] = Apartment::EMPTY;
		}
	}

	ASSERT_THROW(Apartment::IllegalArgException,
			Apartment(NULL, TEST_LENGTH1, TEST_WIDTH1, TEST_PRICE1));
	ASSERT_THROW(Apartment::IllegalArgException,
			Apartment(squares, TEST_BAD_LENGTH1, TEST_WIDTH1, TEST_PRICE1));
	ASSERT_THROW(Apartment::IllegalArgException,
			Apartment(squares, TEST_LENGTH1, TEST_BAD_WIDTH1, TEST_PRICE1));
	ASSERT_THROW(Apartment::IllegalArgException,
			Apartment(squares, TEST_LENGTH1, TEST_WIDTH1, TEST_BAD_PRICE1));

	ASSERT_NO_THROW(Apartment(squares, TEST_LENGTH1, TEST_WIDTH1, TEST_PRICE1));

	deleteSquares(squares, TEST_LENGTH1);
	return true;
}

bool testApartmentDestructor() {

	Apartment::SquareType** squares = new Apartment::SquareType*[TEST_LENGTH1];
	for (int i = 0; i < TEST_LENGTH1; ++i) {
		squares[i] = new Apartment::SquareType[TEST_WIDTH1];
		for (int j = 0; j < TEST_WIDTH1; ++j) {
			squares[i][j] = Apartment::EMPTY;
		}
	}
	Apartment apt(squares, TEST_LENGTH1, TEST_WIDTH1, TEST_PRICE1);

	deleteSquares(squares, TEST_LENGTH1);
	return true;
}

bool testApartmentGetters() {

	Apartment::SquareType** squares = new Apartment::SquareType*[TEST_LENGTH1];
	for (int i = 0; i < TEST_LENGTH1; ++i) {
		squares[i] = new Apartment::SquareType[TEST_WIDTH1];
		for (int j = 0; j < TEST_WIDTH1; ++j) {
			squares[i][j] = Apartment::EMPTY;
		}
	}
	Apartment apt(squares, TEST_LENGTH1, TEST_WIDTH1, TEST_PRICE1);
	ASSERT_EQUALS(TEST_LENGTH1, apt.getLength());
	ASSERT_EQUALS(TEST_WIDTH1, apt.getWidth());
	ASSERT_EQUALS(TEST_PRICE1, apt.getPrice());
	ASSERT_EQUALS(TEST_TOTAL_AREA1, apt.getTotalArea());

	deleteSquares(squares, TEST_LENGTH1);
	return true;
}

bool testApartmentCopyConstructor() {

	Apartment::SquareType** squares = new Apartment::SquareType*[TEST_LENGTH1];
	for (int i = 0; i < TEST_LENGTH1; ++i) {
		squares[i] = new Apartment::SquareType[TEST_WIDTH1];
		for (int j = 0; j < TEST_WIDTH1; ++j) {
			squares[i][j] = Apartment::EMPTY;
		}
	}
	Apartment apt(squares, TEST_LENGTH1, TEST_WIDTH1, TEST_PRICE1);

	ASSERT_NO_THROW(Apartment(apt));
	const Apartment apt2(apt);
	ASSERT_EQUALS(apt2.getLength(), apt.getLength());
	ASSERT_EQUALS(apt2.getWidth(), apt.getWidth());
	ASSERT_EQUALS(apt2.getPrice(), apt.getPrice());
	ASSERT_EQUALS(apt2.getTotalArea(), apt.getTotalArea());

	deleteSquares(squares, TEST_LENGTH1);
	return true;
}

bool testApartmentAssignmentOperator() {
	Apartment::SquareType** squares1 = new Apartment::SquareType*[TEST_LENGTH1];
	for (int i = 0; i < TEST_LENGTH1; ++i) {
		squares1[i] = new Apartment::SquareType[TEST_WIDTH1];
		for (int j = 0; j < TEST_WIDTH1; ++j) {
			squares1[i][j] = Apartment::EMPTY;
		}
	}
	Apartment apt1(squares1, TEST_LENGTH1, TEST_WIDTH1, TEST_PRICE1);

	// apt1 = apt1;	// Self-assignment
	ASSERT_EQUALS(TEST_LENGTH1, apt1.getLength());
	ASSERT_EQUALS(TEST_WIDTH1, apt1.getWidth());
	ASSERT_EQUALS(TEST_PRICE1, apt1.getPrice());

	Apartment::SquareType** squares2 = new Apartment::SquareType*[TEST_LENGTH2];
	for (int i = 0; i < TEST_LENGTH2; ++i) {
		squares2[i] = new Apartment::SquareType[TEST_WIDTH2];
		for (int j = 0; j < TEST_WIDTH2; ++j) {
			squares2[i][j] = Apartment::EMPTY;
		}
	}
	Apartment apt2(squares2, TEST_LENGTH2, TEST_WIDTH2, TEST_PRICE2);

	apt2 = apt1;
	ASSERT_EQUALS(TEST_LENGTH1, apt2.getLength());
	ASSERT_EQUALS(TEST_WIDTH1, apt2.getWidth());
	ASSERT_EQUALS(TEST_PRICE1, apt2.getPrice());

	deleteSquares(squares1, TEST_LENGTH1);
	deleteSquares(squares2, TEST_LENGTH2);
	return true;
}

bool testApartmentFunctionCallOperator() {

	Apartment::SquareType** squares = new Apartment::SquareType*[TEST_LENGTH1];
	for (int i = 0; i < TEST_LENGTH1; ++i) {
		squares[i] = new Apartment::SquareType[TEST_WIDTH1];
		for (int j = 0; j < TEST_WIDTH1; ++j) {
			squares[i][j] = Apartment::EMPTY;
		}
	}
	Apartment apt(squares, TEST_LENGTH1, TEST_WIDTH1, TEST_PRICE1);

	ASSERT_THROW(Apartment::OutOfApartmentBoundsException, apt(-1, 0));
	ASSERT_THROW(Apartment::OutOfApartmentBoundsException, apt(0, -1));
	ASSERT_THROW(Apartment::OutOfApartmentBoundsException,
			apt(TEST_LENGTH1, 0));
	ASSERT_THROW(Apartment::OutOfApartmentBoundsException, apt(0, TEST_WIDTH1));

	for (int i = 0; i < TEST_LENGTH1; ++i) {
		for (int j = 0; j < TEST_WIDTH1; ++j) {
			ASSERT_NO_THROW(apt(i, j));
			ASSERT_EQUALS(Apartment::EMPTY, apt(i, j));
			apt(i, j) = Apartment::WALL;	// Check changes
			ASSERT_EQUALS(Apartment::WALL, apt(i, j));
		}
	}
	ASSERT_EQUALS(0, apt.getTotalArea());

	deleteSquares(squares, TEST_LENGTH1);
	return true;
}

bool testApartmentConstFunctionCallOperator() {

	Apartment::SquareType** squares = new Apartment::SquareType*[TEST_LENGTH1];
	for (int i = 0; i < TEST_LENGTH1; ++i) {
		squares[i] = new Apartment::SquareType[TEST_WIDTH1];
		for (int j = 0; j < TEST_WIDTH1; ++j) {
			squares[i][j] = Apartment::EMPTY;
		}
	}
	const Apartment apt(squares, TEST_LENGTH1, TEST_WIDTH1, TEST_PRICE1);

	ASSERT_THROW(Apartment::OutOfApartmentBoundsException, apt(-1, 0));
	ASSERT_THROW(Apartment::OutOfApartmentBoundsException, apt(0, -1));
	ASSERT_THROW(Apartment::OutOfApartmentBoundsException,
			apt(TEST_LENGTH1, 0));
	ASSERT_THROW(Apartment::OutOfApartmentBoundsException, apt(0, TEST_WIDTH1));

	for (int i = 0; i < TEST_LENGTH1; ++i) {
		for (int j = 0; j < TEST_WIDTH1; ++j) {
			ASSERT_NO_THROW(apt(i, j));
			ASSERT_EQUALS(Apartment::EMPTY, apt(i, j));
		}
	}

	deleteSquares(squares, TEST_LENGTH1);
	return true;
}

bool testApartmentOperatorPlusEqual() {

	Apartment::SquareType** squares1 = new Apartment::SquareType*[TEST_LENGTH1];
	for (int i = 0; i < TEST_LENGTH1; ++i) {
		squares1[i] = new Apartment::SquareType[TEST_WIDTH1];
		for (int j = 0; j < TEST_WIDTH1; ++j) {
			squares1[i][j] = Apartment::EMPTY;
		}
	}
	Apartment apt1(squares1, TEST_LENGTH1, TEST_WIDTH1, TEST_PRICE1);
	Apartment::SquareType** squares2 = new Apartment::SquareType*[TEST_LENGTH2];
	for (int i = 0; i < TEST_LENGTH2; ++i) {
		squares2[i] = new Apartment::SquareType[TEST_WIDTH2];
		for (int j = 0; j < TEST_WIDTH2; ++j) {
			squares2[i][j] = Apartment::EMPTY;
		}
	}
	Apartment apt2(squares2, TEST_LENGTH2, TEST_WIDTH2, TEST_PRICE2);
	Apartment::SquareType** squares3 = new Apartment::SquareType*[TEST_LENGTH3];
	for (int i = 0; i < TEST_LENGTH3; ++i) {
		squares3[i] = new Apartment::SquareType[TEST_WIDTH3];
		for (int j = 0; j < TEST_WIDTH3; ++j) {
			squares3[i][j] = Apartment::EMPTY;
		}
	}
	Apartment apt3(squares3, TEST_LENGTH3, TEST_WIDTH3, TEST_PRICE3);
	Apartment::SquareType** squares4 = new Apartment::SquareType*[TEST_LENGTH4];
	for (int i = 0; i < TEST_LENGTH4; ++i) {
		squares4[i] = new Apartment::SquareType[TEST_WIDTH4];
		for (int j = 0; j < TEST_WIDTH4; ++j) {
			squares4[i][j] = Apartment::EMPTY;
		}
	}
	Apartment apt4(squares4, TEST_LENGTH4, TEST_WIDTH4, TEST_PRICE4);
	//Test same width apartments
	apt1 += apt3;
	ASSERT_EQUALS(TOTAL_LENGTH1, apt1.getLength());
	ASSERT_EQUALS(TOTAL_WIDTH1, apt1.getWidth());
	ASSERT_EQUALS(TOTAL_PRICE1, apt1.getPrice());
	ASSERT_EQUALS(TEST_AREA1, apt1.getTotalArea());
	//Test same length apartments
	apt4 += apt1;
	ASSERT_EQUALS(TOTAL_LENGTH2, apt4.getLength());
	ASSERT_EQUALS(TOTAL_WIDTH2, apt4.getWidth());
	ASSERT_EQUALS(TOTAL_PRICE2, apt4.getPrice());
	ASSERT_EQUALS(TEST_AREA2, apt4.getTotalArea());
	//Test completely different apartments
	apt2 += apt4;
	ASSERT_EQUALS(TOTAL_LENGTH3, apt2.getLength());
	ASSERT_EQUALS(TOTAL_WIDTH3, apt2.getWidth());
	ASSERT_EQUALS(TOTAL_PRICE3, apt2.getPrice());
	ASSERT_EQUALS(TEST_AREA3, apt2.getTotalArea());
	deleteSquares(squares1, TEST_LENGTH1);
	deleteSquares(squares2, TEST_LENGTH2);
	deleteSquares(squares3, TEST_LENGTH3);
	deleteSquares(squares4, TEST_LENGTH4);
	return true;
}

bool testApartmentLessThanOperator() {

	Apartment::SquareType** squares1 = new Apartment::SquareType*[TEST_LENGTH1];
	for (int i = 0; i < TEST_LENGTH1; ++i) {
		squares1[i] = new Apartment::SquareType[TEST_WIDTH1];
		for (int j = 0; j < TEST_WIDTH1; ++j) {
			squares1[i][j] = Apartment::EMPTY;
		}
	}
	Apartment apt1(squares1, TEST_LENGTH1, TEST_WIDTH1, TEST_PRICE1);
	Apartment::SquareType** squares2 = new Apartment::SquareType*[TEST_LENGTH2];
	for (int i = 0; i < TEST_LENGTH2; ++i) {
		squares2[i] = new Apartment::SquareType[TEST_WIDTH2];
		for (int j = 0; j < TEST_WIDTH2; ++j) {
			squares2[i][j] = Apartment::EMPTY;
		}
	}
	Apartment apt2(squares2, TEST_LENGTH2, TEST_WIDTH2, TEST_PRICE2);
	Apartment::SquareType** squares5 = new Apartment::SquareType*[TEST_LENGTH5];
	for (int i = 0; i < TEST_LENGTH5; ++i) {
		squares5[i] = new Apartment::SquareType[TEST_WIDTH5];
		for (int j = 0; j < TEST_WIDTH5; ++j) {
			squares5[i][j] = Apartment::WALL;
		}
	}
	Apartment apt5(squares5, TEST_LENGTH5, TEST_WIDTH5, TEST_PRICE5);
	Apartment::SquareType** squares6 = new Apartment::SquareType*[TEST_LENGTH6];
	for (int i = 0; i < TEST_LENGTH6; ++i) {
		squares6[i] = new Apartment::SquareType[TEST_WIDTH6];
		for (int j = 0; j < TEST_WIDTH6; ++j) {
			squares6[i][j] = Apartment::WALL;
		}
	}
	Apartment apt6(squares6, TEST_LENGTH6, TEST_WIDTH6, TEST_PRICE6);
	//Testing operator
	ASSERT_EQUALS(true, (apt1 < apt2));
	ASSERT_EQUALS(false, (apt2 < apt1));
	//Testing operator with 1 apartment with 0 total area
	ASSERT_EQUALS(true, (apt1 < apt5));
	ASSERT_EQUALS(false, (apt5 < apt1));
	//Testing operator with 2 apartments with 0 total areas
	ASSERT_EQUALS(false, (apt6 < apt5));
	ASSERT_EQUALS(true, (apt5 < apt6));
	deleteSquares(squares1, TEST_LENGTH1);
	deleteSquares(squares2, TEST_LENGTH2);
	deleteSquares(squares5, TEST_LENGTH5);
	deleteSquares(squares6, TEST_LENGTH6);
	return true;
}

bool testApartmentOperatorPlus() {

	Apartment::SquareType** squares1 = new Apartment::SquareType*[TEST_LENGTH1];
	for (int i = 0; i < TEST_LENGTH1; ++i) {
		squares1[i] = new Apartment::SquareType[TEST_WIDTH1];
		for (int j = 0; j < TEST_WIDTH1; ++j) {
			squares1[i][j] = Apartment::EMPTY;
		}
	}
	Apartment apt1(squares1, TEST_LENGTH1, TEST_WIDTH1, TEST_PRICE1);
	Apartment::SquareType** squares2 = new Apartment::SquareType*[TEST_LENGTH2];
	for (int i = 0; i < TEST_LENGTH2; ++i) {
		squares2[i] = new Apartment::SquareType[TEST_WIDTH2];
		for (int j = 0; j < TEST_WIDTH2; ++j) {
			squares2[i][j] = Apartment::EMPTY;
		}
	}
	Apartment apt2(squares2, TEST_LENGTH2, TEST_WIDTH2, TEST_PRICE2);
	Apartment::SquareType** squares3 = new Apartment::SquareType*[TEST_LENGTH3];
	for (int i = 0; i < TEST_LENGTH3; ++i) {
		squares3[i] = new Apartment::SquareType[TEST_WIDTH3];
		for (int j = 0; j < TEST_WIDTH3; ++j) {
			squares3[i][j] = Apartment::EMPTY;
		}
	}
	Apartment apt3(squares3, TEST_LENGTH3, TEST_WIDTH3, TEST_PRICE3);
	Apartment::SquareType** squares4 = new Apartment::SquareType*[TEST_LENGTH4];
	for (int i = 0; i < TEST_LENGTH4; ++i) {
		squares4[i] = new Apartment::SquareType[TEST_WIDTH4];
		for (int j = 0; j < TEST_WIDTH4; ++j) {
			squares4[i][j] = Apartment::EMPTY;
		}
	}
	Apartment apt4(squares4, TEST_LENGTH4, TEST_WIDTH4, TEST_PRICE4);
	//Test same width apartments
	Apartment testApartment = apt1 + apt3;
	ASSERT_EQUALS(TOTAL_LENGTH1, testApartment.getLength());
	ASSERT_EQUALS(TOTAL_WIDTH1, testApartment.getWidth());
	ASSERT_EQUALS(TOTAL_PRICE1, testApartment.getPrice());
	ASSERT_EQUALS(TEST_AREA1, testApartment.getTotalArea());
	//Test same length apartments
	testApartment = testApartment + apt4;
	ASSERT_EQUALS(TOTAL_LENGTH2, testApartment.getLength());
	ASSERT_EQUALS(TOTAL_WIDTH2, testApartment.getWidth());
	ASSERT_EQUALS(TOTAL_PRICE2, testApartment.getPrice());
	ASSERT_EQUALS(TEST_AREA2, testApartment.getTotalArea());
	//Test completely different apartments
	testApartment = testApartment + apt2;
	ASSERT_EQUALS(TOTAL_LENGTH3, testApartment.getLength());
	ASSERT_EQUALS(TOTAL_WIDTH3, testApartment.getWidth());
	ASSERT_EQUALS(TOTAL_PRICE3, testApartment.getPrice());
	ASSERT_EQUALS(TEST_AREA3, testApartment.getTotalArea());
	deleteSquares(squares1, TEST_LENGTH1);
	deleteSquares(squares2, TEST_LENGTH2);
	deleteSquares(squares3, TEST_LENGTH3);
	deleteSquares(squares4, TEST_LENGTH4);
	return true;
}

bool apartmentTest() {
	RUN_TEST(apartmentTestExample);
	RUN_TEST(testApartmentConstructor);
	RUN_TEST(testApartmentDestructor);
	RUN_TEST(testApartmentGetters);
	RUN_TEST(testApartmentCopyConstructor);
	RUN_TEST(testApartmentAssignmentOperator);
	RUN_TEST(testApartmentFunctionCallOperator);
	RUN_TEST(testApartmentConstFunctionCallOperator);
	RUN_TEST(testApartmentOperatorPlusEqual);
	RUN_TEST(testApartmentLessThanOperator);
	RUN_TEST(testApartmentOperatorPlus);
	return true;
}
