#ifndef MTM4_APARTMENT_H
#define MTM4_APARTMENT_H

#include <exception>

class Apartment
{
public:
    class ApartmentException : public std::exception {};
    class IllegalArgException : public ApartmentException {};
    class OutOfApartmentBoundsException : public ApartmentException {};

    enum SquareType {EMPTY, WALL, NUM_SQUARE_TYPES};

	/**
	* Creates a new apartment:
	* The structure is given by squares, and the dimensions are width/height.
	* The price of the apartment is given by price.
	*
	* @return
	* 	A new allocated apartment.
	* 	if width or height are not positive, price is negative, squares is NULL
	* 	or in case of a memory allocation failure - return NULL.
	**/
	Apartment(SquareType** squares, int length, int width, int price);

	/**
	* Frees all memory allocated for the given apartment.
	* This function can receive NULL.
	*/
	~Apartment();

	/**
	* Creates a newly allocated copy of a given apartment.
	*
	* @return
	* A newly allocated copy of the original apartment.
	* NULL if the apartment is NULL or in any case of memory allocation failure.
	*/
	Apartment(const Apartment& apartment);
	/**
	* @return
	* 	the price of the apartment.
	*/
	int getPrice() const;

	/**
	* @return
	* 	the length of the apartment.
	*/
	 int getLength() const;

	/**
	* @return
	* 	the width of the apartment.
	*/
	int getWidth() const;

	/**
	* @return
	* 	the number of empty squares in the apartment.
	*/
	int getTotalArea() const;

	/**
	 * Placement operator.
	 *
	 * @param apartment - The apartment to be placed.
	 *
	 * @return
	 * The apartment, identical to given apartment.
	 */
	Apartment& operator=(const Apartment& apartment);

	/**
	 * Dereference operator.
	 *
	 * @param row - Square's row in Apartment.
	 * @param col- Square's column in Apartment.
	 *
	 * @return
	 * OutOfApartmentBoundsException if row or col are negative, row >=
	 * Apartment's length or col >= Apartment's width.
	 * The content of given square in Apartment, changable.
	 */
	SquareType& operator()(int row, int col);

	/**
	 * Const Dereference operator.
	 *
	 * @param row - Square's row in Apartment.
	 * @param col- Square's column in Apartment.
	 *
	 * @return
	 * OutOfApartmentBoundsException if row or col are negative, row >=
	 * Apartment's length or col >= Apartment's width.
	 * The content of given square in Apartment, unchangable.
	 */
	const SquareType& operator()(int row, int col) const;

	/**
	* Sums two apartments. The new squares array will be made the following way:
	* 1. If their widths are the same, the 2nd array will be "pasted"
	*    below the 1st's.
	* 2. If their lenghts are the same, the 2nd array will be "pasted" to the
	*    right of the 1st's.
	* 3. If neither of the above happens, the 2nd array will be "pasted" below
	*    the 1st's, and all remaining squares will be walls, to complete a
	*    rectangle.
	*
	* The new price will be the sum of the 2 prices, and the length and width
	* are changed accordingly.
	*/
    Apartment& operator+=(const Apartment& apartment);

private:
    SquareType** squares; // Aprtment's structure
    int length; // Apartment's length
    int width; // Apartment's width
	int price; // Apartment's price

    /**
    * The following functions are used by operator += for creating new squares
    * from the two combined arrays of the two apartments.
    *
    * MergeSameWidth - for if the two have the same width
    * MergeSameLength - for if the two have the same length
    * MergeApt1Wider - for if the two have different sizes but apartment 1 is
    * 				   wider
    * MergeApt2Wider - for if the two have different sizes but apartment 2 is
    * 				   wider
    */

    SquareType** MergeSameWidth(const Apartment& apt1, const Apartment& apt2);

    SquareType** MergeSameLength(const Apartment& apt1, const Apartment& apt2);

    SquareType** MergeApt1Wider(const Apartment& apt1, const Apartment& apt2);

    SquareType** MergeApt2Wider(const Apartment& apt1, const Apartment& apt2);
};

/*****************************
 * External operator function
 ****************************/

/**
* Sums two apartments. The new squares array will be made the following way:
* 1. If their widths are the same, the 2nd array will be "pasted"
*    below the 1st's.
* 2. If their lenghts are the same, the 2nd array will be "pasted" to the
*    right of the 1st's.
* 3. If neither of the above happens, the 2nd array will be "pasted" below
*    the 1st's, and all remaining squares will be walls, to complete a
*    rectangle.
*
* The new price will be the sum of the 2 prices, and the length and width
* are changed accordingly.
*/
Apartment operator+(const Apartment& apartment1, const Apartment& apartment2);

/**
* The following operator defines the meaning of an apartment being "less"
* than another apartment by comparing the ratio between an apartment's
* price and its total area.
* if both apartments have the same exact ratio, then the prices are
* compared.
*/
bool operator<(const Apartment& apartment1, const Apartment& apartment2);

#endif //MTM4_APARTMENT_H
