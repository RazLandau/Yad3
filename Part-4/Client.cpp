/*
 * Client.cpp
 *
 *  Created on: Jun 21, 2016
 *      Author: Lioz
 */


#include "Client.h"

int Client::nextId = 0;

int const Client::getPriority() const {
	return this->priority;
}

int const Client::getId() const {
	return this->id;
}

void Client::addTopic(const Topic& t) {
	clientTopics.insert(t);
}

void Client::removeTopic(const Topic& t) {
	clientTopics.erase(t);
}

std::ostream& Client::getSink() {
	return this->messagesSink;
}

void Client::receiveMaintenanceMessage(const std::string& s) const {
	messagesSink << "Client #" << getId() << " received maintenance message: "
			<< s << std::endl;
}
