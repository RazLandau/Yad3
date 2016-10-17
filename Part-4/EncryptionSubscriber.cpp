/*
 * EncryptionSubscriber.cpp
 *
 *  Created on: Jun 23, 2016
 *      Author: Lioz
 */

#include "EncryptionSubscriber.h"

void EncryptionSubscriber::receiveMessage(const std::string message,
		const Topic& t, const Client& publisher) {
	if ( clientTopics.find(t) == clientTopics.end() ) {
		NonSubscribedTopic error;
		throw error;
	}
	std::string decrypted_message = message;
	for (unsigned int i=0 ; i<message.size() ; i++) {
		decrypted_message[i] = message[i]^encryption_key;
	}
	getSink() << "Topic: " << t << ". Sender: #" << publisher.getId() <<
			". Receiver: #" << getId() << ". Message: " << decrypted_message <<
			std::endl;
}
