/*
 * Publisher.cpp
 *
 *  Created on: Jun 21, 2016
 *      Author: Lioz
 */

#include "Publisher.h"

void Publisher::publishTopic(const Topic& t) {
	broker.publishTopic(*this,t);
	addTopic(t);
}

void Publisher::unpublishTopic(const Topic& t) {
	if ( clientTopics.find(t) == clientTopics.end() ) {
		NonPublishedTopic error;
		throw error;
	}
	broker.unpublishTopic(*this,t);
	removeTopic(t);
}

void Publisher::unpublishAll() {
	for (Topic t : clientTopics) {
		broker.unpublishTopic(*this,t);
	}
	clientTopics.clear();
}

void Publisher::sendMessage(const std::string message,const Topic& t) {
	if ( clientTopics.find(t) == clientTopics.end() ) {
		NonPublishedTopic error;
		throw error;
	}
	broker.publishMessage(t,message,*this);
}

void Publisher::publishEncryptedMessage(const Topic& t,
		const std::string& message, const Client& sender) {
	broker.publishMessage(t,message,sender);
}
