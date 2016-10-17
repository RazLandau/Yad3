/*
 * Apartment.cpp
 *
 *  Created on: Jun 8, 2016
 *      Author: Lioz
 */

#include <iostream>
#include "Apartment.h"

Apartment::Apartment(SquareType** squares, int length, int width, int price) :
		squares(NULL), length(0), width(0), price(0) {
	if (price < 0 || length <= 0 || width <= 0 || squares == NULL) {
		throw IllegalArgException();
	}
	this->squares = new SquareType*[length];
	for (int i = 0; i < length; ++i) {
		this->squares[i] = new SquareType[width];
		for (int j = 0; j < width; ++j) {
			this->squares[i][j] = squares[i][j];
		}
	}
	this->length = length;
	this->width = width;
	this->price = price;
}

Apartment::Apartment(const Apartment& apartment) :
		squares(new SquareType*[apartment.length]), length(apartment.length), width(
				apartment.width), price(apartment.price) {
	for (int i = 0; i < length; i++) {
		this->squares[i] = new SquareType[width];
		for (int j = 0; j < width; j++) {
			squares[i][j] = apartment.squares[i][j];

		}
	}
}

int Apartment::getPrice() const {
	return price;
}

int Apartment::getLength() const {
	return length;
}

int Apartment::getWidth() const {
	return width;
}

int Apartment::getTotalArea() const {
	int counter = 0;
	for (int i = 0; i < length; i++) {
		for (int j = 0; j < width; j++) {
			if (squares[i][j] == EMPTY) {
				counter++;
			}
		}
	}
	return counter;
}

Apartment::~Apartment() {
	for (int i = 0; i < length; i++) {
		delete[] squares[i];
	}
	delete[] squares;
}

Apartment& Apartment::operator=(const Apartment& apartment) {
	if (this == &apartment) {
		return *this;
	}
	for (int i = 0; i < length; i++) {
		delete[] squares[i];
	}
	delete[] squares;
	length = apartment.length;
	width = apartment.width;
	price = apartment.price;
	SquareType** temp = new SquareType*[apartment.length];
	for (int i = 0; i < length; ++i) {
		temp[i] = new SquareType[apartment.width];
		for (int j = 0; j < width; ++j) {
			temp[i][j] = apartment.squares[i][j];
		}
	}
	squares = temp;
	return *this;
}

Apartment& Apartment::operator+=(const Apartment& apartment) {
	price += apartment.price;
	const int old_length = length;
	SquareType** newSquares;
	if (width == apartment.width) {
		newSquares = MergeSameWidth(*this, apartment);
		length += apartment.length;
	} else if (length == apartment.length) {
		newSquares = MergeSameLength(*this, apartment);
		width += apartment.width;
	} else {
		if (width > apartment.width) {
			newSquares = MergeApt1Wider(*this, apartment);
		} else {
			newSquares = MergeApt2Wider(*this, apartment);
			width = apartment.width;
		}
		length += apartment.length;
	}
	SquareType** ptr;
	ptr = this->squares;
	this->squares = newSquares;
	for (int i = 0; i < old_length; i++) {
		delete[] ptr[i];
	}
	delete[] ptr;
	return *this;
}

Apartment operator+(const Apartment& apartment1, const Apartment& apartment2) {
	return (Apartment(apartment1) += apartment2);
}

Apartment::SquareType** Apartment::MergeSameWidth(const Apartment& apt1,
		const Apartment& apt2) {
	const int old_length = apt1.length;
	const int length = apt1.length + apt2.length;
	SquareType** newSquares = new SquareType*[length];
	for (int i = 0; i < length; i++) {
		newSquares[i] = new SquareType[apt1.width];
	}
	for (int i = 0; i < old_length; i++) {
		for (int j = 0; j < apt1.width; j++) {
			newSquares[i][j] = apt1.squares[i][j];
		}
	}
	for (int i = old_length; i < length; i++) {
		for (int j = 0; j < apt1.width; j++) {
			newSquares[i][j] = apt2.squares[(i - old_length)][j];
		}
	}
	return newSquares;
}

Apartment::SquareType** Apartment::MergeSameLength(const Apartment& apt1,
		const Apartment& apt2) {
	const int old_width = apt1.width;
	const int new_width = apt1.width + apt2.width;
	SquareType** newSquares = new SquareType*[new_width];
	for (int i = 0; i < apt1.length; i++) {
		newSquares[i] = new SquareType[new_width];
	}
	for (int i = 0; i < apt1.length; i++) {
		for (int j = 0; j < old_width; j++) {
			newSquares[i][j] = apt1.squares[i][j];
		}
		for (int j = old_width; j < new_width; j++) {
			newSquares[i][j] = apt2.squares[i][(j - old_width)];
		}
	}
	return newSquares;
}

Apartment::SquareType** Apartment::MergeApt1Wider(const Apartment& apt1,
		const Apartment& apt2) {
	const int old_length = apt1.length;
	const int new_length = apt1.length + apt2.length;
	SquareType** newSquares = new SquareType*[new_length];
	for (int i = 0; i < new_length; i++) {
		newSquares[i] = new SquareType[apt1.width];
	}
	for (int i = 0; i < old_length; i++) {
		for (int j = 0; j < apt1.width; j++) {
			newSquares[i][j] = apt1.squares[i][j];
		}
	}
	for (int i = old_length; i < new_length; i++) {
		for (int j = 0; j < apt2.width; j++) {
			newSquares[i][j] = apt2.squares[(i - old_length)][j];
		}
		for (int j = apt2.width; j < apt1.width; j++) {
			newSquares[i][j] = WALL;
		}
	}
	return newSquares;
}

Apartment::SquareType** Apartment::MergeApt2Wider(const Apartment& apt1,
		const Apartment& apt2) {
	const int old_length = apt1.length;
	const int new_length = apt1.length + apt2.length;
	SquareType** newSquares = new SquareType*[new_length];
	for (int i = 0; i < new_length; i++) {
		newSquares[i] = new SquareType[apt2.width];
	}
	for (int i = 0; i < old_length; i++) {
		for (int j = 0; j < apt1.width; j++) {
			newSquares[i][j] = apt1.squares[i][j];
		}
		for (int j = apt1.width; j < apt2.width; j++) {
			newSquares[i][j] = WALL;
		}
	}
	for (int i = old_length; i < new_length; i++) {
		for (int j = 0; j < apt2.width; j++) {
			newSquares[i][j] = apt2.squares[(i - old_length)][j];
		}
	}
	return newSquares;
}

bool operator<(const Apartment& apartment1, const Apartment& apartment2) {
	const int area1 = apartment1.getTotalArea(), area2 =
			apartment2.getTotalArea(), price1 = apartment1.getPrice(), price2 =
			apartment2.getPrice();
	if (price1 * area2 == price2 * area1) {
		return price1 < price2;
	} else {
		return (price1 * area2) < (price2 * area1);
	}
}

Apartment::SquareType& Apartment::operator()(int row, int col) {
	if (row < 0 || row >= length || col < 0 || col >= width) {
		throw OutOfApartmentBoundsException();
	}
	return squares[row][col];
}

const Apartment::SquareType& Apartment::operator()(int row, int col) const {
	if (row < 0 || row >= length || col < 0 || col >= width) {
		throw OutOfApartmentBoundsException();
	}
	Apartment::SquareType& square = squares[row][col];
	return square;
}

