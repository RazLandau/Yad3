/*
 * Broker.cpp
 *
 *  Created on: Jun 21, 2016
 *      Author: Lioz
 */

#include "Broker.h"


void Broker::subscribeToTopic(const Subscriber& sub, const Topic& t) {
	if (topics.find(t) == topics.end()) {
		topics[t];
	}
	topics[t].insert(&sub);
}

void Broker::unsubscribeToTopic(const Subscriber& sub, const Topic& t) {
	if (topics.find(t) == topics.end()) {
		return;
	}
	topics[t].remove(&sub);
}

void Broker::publishTopic(const Publisher& pub, const Topic& t) {
	if (topics.find(t) == topics.end()) {
		topics[t];
	}
	topics[t].insert(&pub);
}

void Broker::unpublishTopic(const Publisher& pub, const Topic& t) {
	if (topics.find(t) == topics.end()) {
		return;
	}
	topics[t].remove(&pub);
}

void Broker::publishMessage(const Topic& t, const std::string& message,
		const Client& sender) const{
	const int size = topics.at(t).size();
	const_cast<Broker&>(*this).publishMessage_aux(t,message,sender,size);
}

void Broker::publishMessage_aux(const Topic& t, const std::string& message,
   		const Client& sender, int size) {
	int counter = 0;
	for (int priority=0 ; counter < size ; priority++) {
		for ( const Client* client : topics.at(t) ) {
			if ( client->getPriority() == priority ) {
				const_cast<Client*>(client)->receiveMessage(message,t,sender);
				counter++;
			}
		}
	}
}

void Broker::sendMaintenanceMessageAny(std::list<Topic>& listOfTopics,
		const std::string s) {
	SortedSet<const Client*,CompareClients> clientsSet;
	bool higher_priority = true;
	int priority = 0;
	for (Topic t : listOfTopics) {
		for (const Client* client : topics[t]) {
			clientsSet.insert(client);
		}
	}
	while (higher_priority == true) {
		higher_priority = false;
		for(const Client* client : clientsSet) {
			if ( client->getPriority() == priority ) {
				client->receiveMaintenanceMessage(s);
			}
			if ( client->getPriority() > priority ){
				higher_priority = true;
			}
		}
		priority++;
	}
}

void Broker::sendMaintenanceMessageAll(std::list<Topic>& listOfTopics,
		const std::string s) {
	bool subscribedToAll, higher_priority = true;
	int priority = 0;
	Topic first_topic = listOfTopics.front();
	while (higher_priority == true) {
		higher_priority = false;
		for (const Client* client : topics[first_topic]) {
			if (client->getPriority() > priority) {
				higher_priority = true;
			} else if ( client->getPriority() == priority ) {
				subscribedToAll = true;
				for (const Topic t : listOfTopics) {
					if ( topics[t].find(client) == topics[t].end() ) {
						subscribedToAll = false;
						break;
					}
				}
				if (subscribedToAll == true) {
					const_cast<Client*>(client)->receiveMaintenanceMessage(s);
				}
			}
		}
		priority++;
	}
}
