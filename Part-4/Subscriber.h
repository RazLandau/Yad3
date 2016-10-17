#ifndef MTM4_SUBSCRIBER_H
#define MTM4_SUBSCRIBER_H


#include "Client.h"

/**
* This class inherits from the client class. A client of this class is able
* to subscribe to different topics and receive messages from the
* publishers that are publishing to them.
**/

class Subscriber : public Client {

public:

	Subscriber(int priority, BrokerIfc& broker,
	    		std::ostream& sink = std::cout) : Client(priority,
	    				broker, sink) {}

	/**
	* This function receives a topic. It then calls to the broker's
	* subscribeToTopic function with the topic and a reference to the client.
	* Eventually, we call the function addTopic from class client, to add the
	* topic to the Subscriber's set of topics.
	**/
	void subscribeToTopic(const Topic& t);
	/**
	* This function receives a topic. It first checks if the Subscriber is
	* subscribed to said topic. If it isn't, throws a NonSubscribedTopic type
	* error. If it does, calls the broker's unsubscribeToTopic function with the
	* topic and a reference to the client.
	* Eventually, we call the function removeTopic from class client, to remove
	* the topic from the Subscriber's set of topics.
	**/
	void unsubscribeToTopic(const Topic& t);

	/**
	* This function unsubscribes from every topic related to the Subscriber.It
	* goes over the client's topics set and for every topic, calls the broker's
	* unsubscribeToTopic function. In the end, the function clears the set of
	* topics.
	**/
	void unsubscribeAll();

	/**
	* This function overrides the above class' receiveMessage function. It
	* receives a message string and a topic, as well as a reference to a
	* Publisher. It first checks if the Subscriber is subscribed to said topic.
	* If not, throws a NonSubscribedTopic type error. If he does, it directs
	* the message to the messagesSink in a specific format.
	**/
	void receiveMessage(const std::string message, const Topic& t,
	    		const Client& publisher) override;

	/**
	* This empty function is here so the class won't be considered an abstract
	* class and we can manually create objects of its class.
	**/
    void sendMessage(const std::string message, const Topic& t) {}

};


#endif //MTM4_SUBSCRIBER_H
