/*
 * yad3_main.c
 *
 *  Created on: Apr 20, 2016
 *      Author: Lioz
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "mtm_ex2.h"
#include "utilities.h"
#include "yad3.h"

/**
* This function closes the files used as output and input channels, as long as
* the user supplied files to be used as input and/or output channels.
**/

static void closeChannels(FILE** input, FILE** output) {
	if (*input != stdin && *input != NULL) {
		fclose(*input);
	}
	if (*output != stdout && *output != NULL) {
		fclose(*output);
	}
}

/**
* This function concludes weither the command line arguments used to start the
* program are valid.
*
* Returns false if:
* 	Number of arguments is wrong (cannot be even or more than 5).
* 	2nd or 4th arguments are not an input (-i) or output (-o) flag.
* 	Either input/output flags is used twice in the command line.
*
* Returns true otherwise.
**/

static bool isInputValid(int argc, char** argv) {
	if (argc > 5 || argc % 2 == 0) {
		return false;
	}
	for (int i = 1; i < argc; i += 2) {
		if (argv[i][0] != '-') {
			return false;
		} else if (argv[i][1] != 'i' && argv[i][1] != 'o') {
			return false;
		}
	}
	if (argc == 5 && argv[1][1] == argv[3][1]) {
		return false;
	}
	return true;
}

/**
* This function interprets the command line arguments and opens files to be
* used as input and output channels, if supplied by the user.
*
* Returns false if:
* 	Opening one of the files fails.
*
* Otherwise returns true.
**/

static bool setInputAndOutput(int argc, char** argv, FILE** input,
		FILE** output) {
	if (argc > 1) {
		if (argv[1][1] == 'i') {
			*input = fopen(argv[2], "r");
		} else {
			*output = fopen(argv[2], "w");
		}
	}
	if (argc > 3) {
		if (argv[3][1] == 'i') {
			*input = fopen(argv[4], "r");
		} else {
			*output = fopen(argv[4], "w");
		}
	}
	if (*input == NULL || *output == NULL) {
		return false;
	}
	return true;
}

/**
* Since all results supplied to the main function are of type Yad3Result,
* translation to the matching MTM error is needed.
*
* Returns the specific MTM error that matches the Yad3 error received.
**/

static Yad3Result yad3Err2MtmErr(Yad3Result result) {
	if (result == YAD3_OUT_OF_MEMORY) {
		return MTM_OUT_OF_MEMORY;
	} else if (result == YAD3_INVALID_PARAMETERS) {
		return MTM_INVALID_PARAMETERS;
	} else if (result == YAD3_EMAIL_ALREADY_EXISTS) {
		return MTM_EMAIL_ALREADY_EXISTS;
	} else if (result == YAD3_EMAIL_DOES_NOT_EXIST) {
		return MTM_EMAIL_DOES_NOT_EXIST;
	} else if (result == YAD3_EMAIL_WRONG_ACCOUNT_TYPE) {
		return MTM_EMAIL_WRONG_ACCOUNT_TYPE;
	} else if (result == YAD3_ALREADY_REQUESTED) {
		return MTM_ALREADY_REQUESTED;
	} else if (result == YAD3_NOT_REQUESTED) {
		return MTM_NOT_REQUESTED;
	} else if (result == YAD3_SERVICE_ALREADY_EXISTS) {
		return MTM_APARTMENT_SERVICE_ALREADY_EXISTS;
	} else if (result == YAD3_SERVICE_DOES_NOT_EXIST) {
		return MTM_APARTMENT_SERVICE_DOES_NOT_EXIST;
	} else if (result == YAD3_SERVICE_FULL) {
		return MTM_APARTMENT_SERVICE_FULL;
	} else if (result == YAD3_APARTMENT_ALREADY_EXISTS) {
		return MTM_APARTMENT_ALREADY_EXISTS;
	} else if (result == YAD3_APARTMENT_DOES_NOT_EXIST) {
		return MTM_APARTMENT_DOES_NOT_EXIST;
	} else if (result == YAD3_PURCHASE_WRONG_PROPERTIES) {
		return MTM_PURCHASE_WRONG_PROPERTIES;
	} else if (result == YAD3_REQUEST_WRONG_PROPERTIES) {
		return MTM_REQUEST_WRONG_PROPERTIES;
	} else {
		return MTM_REQUEST_ILLOGICAL_PRICE;
	}
}

int main(int argc, char** argv) {
	FILE *input = stdin, *output = stdout;
	if (!isInputValid(argc, argv)) {
		mtmPrintErrorMessage(stderr, MTM_INVALID_COMMAND_LINE_PARAMETERS);
		return 0;
	}
	if (!setInputAndOutput(argc, argv, &input, &output)) {
		mtmPrintErrorMessage(stderr, MTM_CANNOT_OPEN_FILE);
		closeChannels(&input, &output);
		return 0;
	}
	Yad3 yad3 = yad3Create();
	if (yad3 == NULL) {
		mtmPrintErrorMessage(stderr, MTM_OUT_OF_MEMORY);
		closeChannels(&input, &output);
		return 0;
	}
	Yad3Result result = YAD3_SUCCESS;
	char delim[6] = " \n\t\0\r";
	char* command_line = calloc(MAX_LEN + 1, sizeof(char));
	if (command_line == NULL) {
		mtmPrintErrorMessage(stderr, MTM_OUT_OF_MEMORY);
		closeChannels(&input, &output);
		yad3Destroy(yad3);
		return 0;
	}
	char *command_ptr, *word;
	while ((command_ptr = fgets(command_line, MAX_LEN + 1, input)) != NULL) {
		word = strtok(command_line, delim);
		if (word == NULL || word[0] == '\n' || word[0] == '#') {
			continue;
		} else if (!strcmp(word, "realtor")) {
			word = strtok(NULL, delim);
			if (!strcmp(word, "add")) {
				char* email = strtok(NULL, delim);
				char* company = strtok(NULL, delim);
				int tax = atoi(strtok(NULL, delim));
				result = yad3AddRealtor(yad3, email, company, tax);
			} else if (!strcmp(word, "remove")) {
				char* email = strtok(NULL, delim);
				result = yad3RemoveRealtor(yad3, email);
			} else if (!strcmp(word, "add_apartment_service")) {
				char* email = strtok(NULL, delim);
				char* service = strtok(NULL, delim);
				int max_apartments = atoi(strtok(NULL, delim));
				result = yad3AddServiceToRealtor(yad3, email, service,
						max_apartments);
			} else if (!strcmp(word, "remove_apartment_service")) {
				char* email = strtok(NULL, delim);
				char* service = strtok(NULL, delim);
				result = yad3RemoveServiceFromRealtor(yad3, email, service);
			} else if (!strcmp(word, "add_apartment")) {
				char* email = strtok(NULL, delim);
				char* service = strtok(NULL, delim);
				int id = atoi(strtok(NULL, delim));
				int price = atoi(strtok(NULL, delim));
				int width = atoi(strtok(NULL, delim));
				int height = atoi(strtok(NULL, delim));
				char* matrix = strtok(NULL, delim);
				result = yad3AddApartmentToRealtor(yad3, email, service, id,
						price, width, height, matrix);
			} else if (!strcmp(word, "remove_apartment")) {
				char* email = strtok(NULL, delim);
				char* service = strtok(NULL, delim);
				int id = atoi(strtok(NULL, delim));
				result = yad3RemoveApartmentFromRealtor(yad3, email, service,
						id);
			} else if (!strcmp(word, "respond_to_offer")) {
				char* email = strtok(NULL, delim);
				char* customer_email = strtok(NULL, delim);
				char* choice = strtok(NULL, delim);
				result = yad3RealtoRespondToOffer(yad3, email, customer_email,
						choice);
			}
		} else if (!strcmp(word, "customer")) {
			word = strtok(NULL, delim);
			if (!strcmp(word, "add")) {
				char* email = strtok(NULL, delim);
				int min_area = atoi(strtok(NULL, delim));
				int min_rooms = atoi(strtok(NULL, delim));
				int max_price = atoi(strtok(NULL, delim));
				result = yad3AddCustomer(yad3, email, min_area, min_rooms,
						max_price);
			} else if (!strcmp(word, "remove")) {
				char* email = strtok(NULL, delim);
				result = yad3RemoveCustomer(yad3, email);
			} else if (!strcmp(word, "purchase")) {
				char* email = strtok(NULL, delim);
				char* realtor_email = strtok(NULL, delim);
				char* service = strtok(NULL, delim);
				int id = atoi(strtok(NULL, delim));
				result = yad3CustomerPurchase(yad3, email, realtor_email,
						service, id);
			} else if (!strcmp(word, "make_offer")) {
				char* email = strtok(NULL, delim);
				char* realtor_email = strtok(NULL, delim);
				char* service = strtok(NULL, delim);
				int id = atoi(strtok(NULL, delim));
				int new_price = atoi(strtok(NULL, delim));
				result = yad3AddOffer(yad3, email, realtor_email, service, id,
						new_price);
			}
		} else if (!strcmp(word, "report")) {
			word = strtok(NULL, delim);
			if (!strcmp(word, "relevant_realtors")) {
				char* email = strtok(NULL, delim);
				result = yad3ReportRelevantRealtors(yad3, email, output);
			} else if (!strcmp(word, "most_paying_customers")) {
				int count = atoi(strtok(NULL, delim));
				result = yad3ReportMostPayingCustomers(yad3, count, output);
			} else if (!strcmp(word, "significant_realtors")) {
				int count = atoi(strtok(NULL, delim));
				result = yad3ReportSignificantRealtors(yad3, count, output);
			}
		}
		if (result != YAD3_SUCCESS) {
			mtmPrintErrorMessage(stderr, yad3Err2MtmErr(result));
		}
		if (result == YAD3_OUT_OF_MEMORY) {
			break;
		}
	}
	closeChannels(&input, &output);
	yad3Destroy(yad3);
	free(command_line);
	return 0;
}
