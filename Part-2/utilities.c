/*
 * utilities.c
 *
 *  Created on: 15 באפר 2016
 *      Author: Raz
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "utilities.h"

char* copyString(char* string) {
	if(!string){
		return NULL;
	}
	int length = strlen(string) + 1;
	char* service_name_copy = malloc(sizeof(*string) * length);
	if (!service_name_copy) {
		return NULL;
	}
	strcpy(service_name_copy, string);
	return service_name_copy;
}

bool emailIsValid(char* email) {
	if(!email){
		return false;
	}
	int counter = 0;
	for (int i = 0; i < strlen(email); i++) {
		if (email[i] == '@') {
			counter++;
		}
	}
	return counter == 1;
}

