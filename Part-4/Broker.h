#ifndef MTM4_BROKER_H
#define MTM4_BROKER_H

#include <map>
#include <list>
#include "SortedSet.h"
#include "Publisher.h"
#include "Subscriber.h"
#include "BrokerIfc.h"

/**
* This class inherits from class BrokerIfc. Its goal is to connect between
* publishers and subscribers, transferring messages between them and more.
* It holds a map of key Topics. Each topic has a value of a Sorted Set of
* clients (publishers and subscribers) who are subscribed or publishing to it.
**/

class Broker : public BrokerIfc {

	/**
	* This class is for the SortedSet class, for sorting the clients inside
	* the set.
	**/
    class CompareClients {
    public:
        bool operator()(const Client* c1, const Client* c2) const {
        	if ( (*(c1)).getId() < (*(c2)).getId() ) {
        		return true;
        	}
        	return false;
        }
    };

    /**
    * This function is called by a subscriber. It checks if a certain topic is
    * in the topics map. If it is not there, it is added. Then the subscriber
    * is added to the set of clients of said topic.
    **/
    virtual void subscribeToTopic(const Subscriber& sub, const Topic& t);
    /**
    * This function is called by a subscriber. If the supplied topic is not
    * in the topics map, does nothing. otherwise, remove the subscriber from
    * said topic's client set.
    **/
    virtual void unsubscribeToTopic(const Subscriber& sub, const Topic& t);
    /**
    * This function is called by a publisher. It checks if a certain topic is
    * in the topics map. If it is not there, it is added. Then the publisher is
    * added to the set of clients of said topic.
    **/
    virtual void publishTopic(const Publisher& pub, const Topic& t);
    /**
    * This function is called by a publisher. If the supplied topic is not in
    * the topics map, does nothing. Otherwise, removes the publisher from said
    * topic's client set.
    **/
    virtual void unpublishTopic(const Publisher& pub, const Topic& t);
    /**
    * This function receives a topic, a message, and a publisher kind of client.
    * it finds the size of that certain topic's client set and moves the
    * parameters on to publishMessage_aux.
    **/
    virtual void publishMessage(const Topic& t, const std::string& message,
    		const Client& sender) const;
    /**
    * This function receives a topic, a message, a publisher kind of client,
    * and the size of the topic's client set. It then sends the message (calls
    * each client's receiveMessage function) to the clients listed in the set.
    * Clients with lower priority get the messages first. If more than one
    * client have the same priority, the one with the smaller id gets the
    * message first.
    **/
    void publishMessage_aux(const Topic& t, const std::string& message,
    		const Client& sender, int size);

public:
    /**
    * This is a map where the key is a Topic (a string) and the value is a
    * sorted set of pointers to Clients. Each set of clients is made of clients
    * who either publish or are subscribed to that topic.
    **/
    std::map<Topic,SortedSet<const Client*,CompareClients>> topics;

    Broker() = default;

    /**
    * This function receives a list of topics and a string message. It searches
    * the topics map for topics that are in the list. It then sends a
    * maintenance message (by calling each client's function that receives a
    * maintenance message) to every client that is publishing or subscribed to
    * any one of the topics in the list. First by order of priority, and then
    * by id.
    */
    void sendMaintenanceMessageAny(std::list<Topic>& listOfTopics,
    		const std::string s);
    /**
    * This function is similar to the previous one. The difference is that it
    * only sends a maintenance message to clients that are publishing/
    * subscribed to every single topic in the given list.
    **/
    void sendMaintenanceMessageAll(std::list<Topic>& listOfTopics,
    		const std::string s);

};


#endif //MTM4_BROKER_H
