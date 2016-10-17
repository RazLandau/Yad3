#ifndef MTM4_BROKERIFC_H
#define MTM4_BROKERIFC_H

#include <string>

typedef std::string Topic;

// Forward Declarations
class Client;
class Subscriber;
class Publisher;

class BrokerIfc {
public:
    BrokerIfc() = default;

    BrokerIfc (const BrokerIfc& broker) = default;

    BrokerIfc& operator=(const BrokerIfc& broker) = default;

    virtual ~BrokerIfc() {}

private:
    friend class Subscriber;
    friend class Publisher;

    virtual void subscribeToTopic(const Subscriber& sub, const Topic& t) = 0;

    virtual void unsubscribeToTopic(const Subscriber& sub, const Topic& t) = 0;

    virtual void publishTopic(const Publisher& pub, const Topic& t) = 0;

    virtual void unpublishTopic(const Publisher& pub, const Topic& t) = 0;

    virtual void publishMessage(const Topic& t, const std::string& message, const Client& sender) const = 0;
};


#endif //MTM4_BROKERIFC_H
