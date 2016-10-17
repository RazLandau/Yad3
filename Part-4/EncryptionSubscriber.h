//
// Created by izikgo on 5/23/2016.
//

#ifndef MTM4_ENCRYPTIONSUBSCRIBER_H
#define MTM4_ENCRYPTIONSUBSCRIBER_H

/**
* This class inherits the regular Subscriber class. It is almost identical to
* the Subscriber class, the difference being this class holds a char variable
* that is used as an encryption_key, as this subscriber decrypts every message
* he receives, using that encryption_key.
**/

#include "Subscriber.h"

class EncryptionSubscriber : public Subscriber {

	//This variable is used as an encryption key when sending out messages.
	char encryption_key;
public:
	EncryptionSubscriber(int priority, BrokerIfc& broker, char key,
	    		std::ostream& sink = std::cout) : Subscriber(priority, broker,
	    				sink), encryption_key(key){}

/**
* This function overrides the receiveMessage functions from above classes.
* It receives a message string, a topic and a publisher.
* First, it checks if the Subscriber is subscribed to that topic. If he isn't,
* throws a NonSubscribedTopic error.
* It then decrypts the message using the encryption key and the XOR operator
* on each and every char in the message.
* Eventually it sends the message into the messagesSink in a specific format.
**/
	void receiveMessage(const std::string message, const Topic& t,
			const Client& publisher);
};


#endif //MTM4_ENCRYPTIONSUBSCRIBER_H
