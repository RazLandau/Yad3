/*
 * customer.h
 *
 *  Created on: Apr 19, 2016
 *      Author: Lioz
 */

#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include "mtm_ex2.h"
#include "utilities.h"

typedef struct customer_t* Customer;

typedef enum CustomerResult_t {
	CUSTOMER_SUCCESS,
	CUSTOMER_OUT_OF_MEMORY,
	CUSTOMER_INVALID_PARAMETERS
} CustomerResult;

/**
* Creates a new customer of type image:
*
* @param
* email- customer's email
* min_area- minimum apartment's area for customer
* min_rooms- minimum apartment's for customer
* max_price- maximum apartment's price for customer
* customer_result- pointer that holds the result/cause of failure
*
* @return
* 	A new allocated customer.
* 	If:
* 	email doesn't contain exactly one '@',
* 	min_area, min_rooms or max_price aren't positive,
* 	email or customer_result are NULL,
* 	or in case of a memory allocation failure - return NULL.
**/
Customer customerCreate(char* email, int min_area, int min_rooms, int max_price,
		CustomerResult* customer_result);

/**
* Frees all memory allocated for the given customer.
* This function can receive NULL.
*/
void customerDestroy(Customer customer);

/**
* creates a newly allocated copy of a given customer.
*
* @param
* customer_result- Pointer that holds the result/cause of failure
*
* @return
* A newly allocated copy of the original customer.
* NULL if customer is NULL or in any case of memory allocation failure.
*/
Customer customerCopy(Customer customer, CustomerResult* customer_result);

/**
* @return
* 	The minimum area for the customer.
*	This function asserts (customer != NULL).
*/
int customerGetMinArea(Customer customer);

/**
* @return
* 	The minimum rooms for the customer.
*	This function asserts (customer != NULL).
*/
int customerGetMinRooms(Customer customer);

/**
* @return
* 	The maximum price for the customer.
*	This function asserts (customer != NULL).
*/
int customerGetMaxPrice(Customer customer);

/**
* @return
* 	The total purchases sum of the customer.
*	This function asserts (customer != NULL).
*/
int customerGetPurchasesSum(Customer customer);

/**
* @return
* 	A copy of the customer's email
* 	NULL in case of a memory allocation failure.
*	This function asserts (customer != NULL).
*/
char* customerGetEmail (Customer customer);

/**
 * Adds purchase to customer's total sum of purchases
 * This function asserts (customer != NULL && purchase>0).
 */
void customerAddPurchase(Customer customer, int purchase);

/**
 * Prints customer details.
 *
 * @param outputChannel - File descriptor for the output channel
 */
void customerPrint(FILE* outputChannel, Customer customer);

#endif /* CUSTOMER_H_ */
