/*
 * EncryptionPublisher.cpp
 *
 *  Created on: Jun 23, 2016
 *      Author: Lioz
 */

#include "EncryptionPublisher.h"

void EncryptionPublisher::sendMessage(const std::string message,
		const Topic& t) {
	if ( clientTopics.find(t) == clientTopics.end() ) {
		NonPublishedTopic error;
		throw error;
	}
	std::string encrypted_message = message;
	for (unsigned int i=0 ; i<message.size() ; i++) {
		encrypted_message[i] = message[i]^encryption_key;
	}
	publishEncryptedMessage(t,encrypted_message,*this);
}
