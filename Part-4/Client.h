#ifndef MTM4_CLIENT_H
#define MTM4_CLIENT_H


#include <exception>
#include <string>
#include <set>
#include <iostream>
#include "BrokerIfc.h"

/**
* This class is a general client class, holding the basic functions for either
* deriving class: Publisher and Subscriber. The class has a static int variable
* that is used for giving every created client a different id of his own. Each
* client also has his own priority. Each client also holds his own set of
* topics, documenting all the topics he publisher to/is subscribed to.
**/

class Client {

	/**
	* This defines the compare function for the set of topics.
	**/
	class CompareTopics {
	    public:
	        bool operator()(Topic c1, Topic c2) const {
	        	if ( c1.compare(c2) < 0 ) {
	        		return true;
	        	}
	        	return false;
	        }
	};
	//This static parameter is used for giving each client its own id
	static int nextId;
	//Is initialized to nextId everytime a client is created.
    int const id;
    //Determines the priority in receiving messages from a broker
    int const priority;
    //Messages the client receives are being sent to this ostream variable
    std::ostream& messagesSink;

protected:

    BrokerIfc& broker;

    //A set of topics, documenting the topics relevant to the client
    std::set<Topic,CompareTopics,std::allocator<Topic>> clientTopics;

public:
    class ClientException : public std::exception {};
    class IllegalPriority : public ClientException {};
    class NonSubscribedTopic : public ClientException {};
    class NonPublishedTopic : public ClientException {};

    Client(int priority, BrokerIfc& broker, std::ostream& sink = std::cout) :
    	 id(nextId), priority(priority), messagesSink(sink), broker(broker) {
    	if (priority < 0) {
    		IllegalPriority error;
    		throw error;
    	}
    	nextId++;
    }

    Client(const Client& c) = default;

    Client& operator=(const Client&) = default;

    virtual ~Client() {};

    //This function returns an integer: the client's priority variable.
    int const getPriority() const;

    //This function returns an integer: the client's id variable.
    int const getId() const;

    //This function receives a topic, and adds that topic to the client's list
    //of topics.
    void addTopic(const Topic& t);

    //This function receives a topic, and removes that topic from the client's
    //list of topics.
    void removeTopic(const Topic& t);

    /**
    * This function receives a message string and directs that message in a
    * certain format into the messagesSink.
    **/
    void receiveMaintenanceMessage(const std::string& s) const;

    //This function returns a reference to the client's ostream sink variable
    std::ostream& getSink();

    //This is a pure virtual function for the use of a publisher
    virtual void sendMessage(const std::string message, const Topic& t) = 0;

    //This is a pure virtual function for the use of a subscriber
    virtual void receiveMessage(const std::string message, const Topic& t,
    		const Client& publisher) = 0;
};

#endif //MTM4_CLIENT_H
