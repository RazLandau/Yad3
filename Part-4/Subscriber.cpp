/*
 * Subscriber.cpp
 *
 *  Created on: Jun 21, 2016
 *      Author: Lioz
 */

#include "Subscriber.h"

void Subscriber::subscribeToTopic(const Topic& t) {
	broker.subscribeToTopic(*this,t);
	addTopic(t);
}

void Subscriber::unsubscribeToTopic(const Topic& t) {
	if ( clientTopics.find(t) == clientTopics.end() ) {
		NonSubscribedTopic error;
		throw error;
	}
	broker.unsubscribeToTopic(*this,t);
	removeTopic(t);
}

void Subscriber::unsubscribeAll() {
	for (Topic t : clientTopics) {
		broker.unsubscribeToTopic(*this,t);
	}
	clientTopics.clear();
}

void Subscriber::receiveMessage(const std::string message,const Topic& t,
	    		const Client& publisher) {
	if ( clientTopics.find(t) == clientTopics.end() ) {
		NonSubscribedTopic error;
		throw error;
	}
	getSink() << "Topic: " << t << ". Sender: #" << publisher.getId() <<
			". Receiver: #" << getId() << ". Message: " << message <<
			std::endl;
}

