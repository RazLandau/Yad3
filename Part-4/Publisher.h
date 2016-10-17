#ifndef MTM4_PUBLISHER_H
#define MTM4_PUBLISHER_H


#include "Client.h"

/**
* This class inherits from the client class. A client of this class is able
* to publish in different topics and send messages to the subscribers that are
* subscribed to them.
**/

class Publisher : public Client {

public:

	Publisher(int priority, BrokerIfc& broker, std::ostream& sink = std::cout) :
		Client(priority, broker, sink) {}

	/**
	* This function receives a topic. It then calls to the broker's
	* PublishTopic function with the topic and a reference to the client.
	* Eventually, we call the function addTopic from class client, to add the
	* topic to the Publisher's set of topics.
	**/
	void publishTopic(const Topic& t);

	/**
	* This function receives a topic. It first checks if the Publisher is
	* publishing to said topic. If it isn't, throws a NonPublishingTopic type
	* error. If it does, calls the broker's unpublishTopic function with the
	* topic and a reference to the client.
	* Eventually, we call the function removeTopic from class client, to remove
	* the topic from the Publisher's set of topics.
	**/
	void unpublishTopic(const Topic& t);

	/**
	* This function unpublishes every topic related to the Publisher. It goes
	* over the client's topics set and for every topic, calls the broker's
	* unpublishTopic function. In the end, the function clears the set of
	* topics.
	**/
	void unpublishAll();

	/**
	* This function overrides the above class' sendMessage function. It
	* receives a message string and a topic. It first checks if the Publisher
	* is publishing to said topic. If not, throws a NonPublishedTopic type
	* error. If he does, it calls to the broker's publishMessage function with
	* said message, topic, and a reference to itself.
	**/
	virtual void sendMessage(const std::string message,const Topic& t) override;

	/**
	* This function receives a topic, an encrypted message, and a client
	* reference from EncryptionPublisher. It then calls the broker's
	* publishMessage function with said message, topic and a reference to
	* itself.
	**/
	void publishEncryptedMessage(const Topic& t, const std::string& message,
    		const Client& sender);

	/**
	* This empty function is here so the class won't be considered an abstract
	* class and we can manually create objects of its class.
	**/
	virtual void receiveMessage(const std::string message, const Topic& t,
	    		const Client& publisher) {}
};


#endif //MTM4_PUBLISHER_H
