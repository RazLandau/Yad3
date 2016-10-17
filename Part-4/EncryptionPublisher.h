//
// Created by izikgo on 5/23/2016.
//

#ifndef MTM4_ENCRYPTIONPUBLISHER_H
#define MTM4_ENCRYPTIONPUBLISHER_H

/**
* This class inherits the regular Publisher class. It is almost identical to
* the Publisher class, the difference being this class holds a char variable
* that is used as an encryption_key, as this publisher only sends out
* encrypted messages.
**/

#include "Publisher.h"

class EncryptionPublisher : public Publisher {

	//This variable is used as an encryption key when sending out messages.
	char encryption_key;
public:
	EncryptionPublisher(int priority, BrokerIfc& broker, char key,
	    		std::ostream& sink = std::cout) : Publisher(priority, broker,
	    				sink), encryption_key(key){}

/**
* This function overrides the sendMessage functions from above classes.
* It receives a message string and a topic.
* First, it checks if the publisher is publishing to that topic. If he doesn't,
* throws a NonPublishedTopic error.
* It then encrypts the message using the encryption key and the XOR operator
* on each and every char in the message.
* Eventually it calls publishEncrtypedMessage which is a function from class
* Publisher.
**/
void sendMessage(const std::string message, const Topic& t);

//This empty function is here so this class won't be considered an abstract
//class, so we can manually create an EncryptionPublisher.
virtual void receiveMessage(const std::string message, const Topic& t,
    		const Client& publisher) {}

};



#endif //MTM4_ENCRYPTIONPUBLISHER_H
