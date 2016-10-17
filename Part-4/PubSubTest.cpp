#include <cstdlib>
#include "Broker.h"
#include "EncryptionSubscriber.h"
#include "EncryptionPublisher.h"
#include "MtmTst.h"
#include <list>

using namespace std;

bool pubSubTestExample() {
	Broker broker;
	stringstream ss;
	Publisher p1(0, broker, ss);
	Subscriber s1(1, broker, ss);
	Subscriber s2(0, broker, ss);
	EncryptionPublisher ep1(1, broker, static_cast<char>(0xFF), ss);
	EncryptionSubscriber es1(2, broker, static_cast<char>(0xFF), ss);

	p1.publishTopic("Cat Videos");
	p1.publishTopic("Dog Videos");
	ep1.publishTopic("Marianas Web");

	s1.subscribeToTopic("Cat Videos");
	s2.subscribeToTopic("Cat Videos");
	s2.subscribeToTopic("Dog Videos");
	es1.subscribeToTopic("Marianas Web");

	p1.sendMessage("This is a cat video", "Cat Videos");
	p1.sendMessage("This is a dog video", "Dog Videos");
	ep1.sendMessage("This is a big secret", "Marianas Web");

	stringstream expected;
	expected << "Topic: " << "Cat Videos" << ". Sender: #" << p1.getId()
			<< ". Receiver: #" << s2.getId() << ". Message: "
			<< "This is a cat video" << endl;
	expected << "Topic: " << "Cat Videos" << ". Sender: #" << p1.getId()
			<< ". Receiver: #" << s1.getId() << ". Message: "
			<< "This is a cat video" << endl;
	expected << "Topic: " << "Dog Videos" << ". Sender: #" << p1.getId()
			<< ". Receiver: #" << s2.getId() << ". Message: "
			<< "This is a dog video" << endl;
	expected << "Topic: " << "Marianas Web" << ". Sender: #" << ep1.getId()
			<< ". Receiver: #" << es1.getId() << ". Message: "
			<< "This is a big secret" << endl;

	ASSERT_EQUALS(expected.str(), ss.str());

	return true;
}

/******************************************************************************/
bool subscribeToTopicTest() {
	Broker broker;
	stringstream ss;

	Subscriber badass(0,broker,ss);
	badass.subscribeToTopic("Bruce Lee");
	badass.subscribeToTopic("Steven Segal");
	badass.subscribeToTopic("Muhammad Ali");
	badass.subscribeToTopic("Jackie Chan");

	ASSERT_EQUALS(0,badass.getPriority());
	ASSERT_EQUALS(5,badass.getId());
	/*Illegal Subscriber creation*/
	ASSERT_THROW(Client::IllegalPriority,(Subscriber(-5, broker, ss)));
	/*legal Subscriber creation*/
	ASSERT_NO_THROW(Subscriber(1,broker,ss));
	return true;
}

/*****************************************************************************/

bool unsubscribeToTopicTest() {
	Broker broker;
	stringstream ss;

	Subscriber moving(0,broker,ss);
	moving.subscribeToTopic("Tell Aviv");
	moving.subscribeToTopic("Jerusalem");
	moving.subscribeToTopic("Kiryat Shmona");
	moving.subscribeToTopic("Haifa");
	moving.subscribeToTopic("Eilat");
	moving.subscribeToTopic("Metula");

	moving.unsubscribeToTopic("Haifa");
	moving.unsubscribeToTopic("Eilat");

	/*unsubscribing from a topic we're not subscribed to*/
	ASSERT_THROW(Client::NonSubscribedTopic,moving.unsubscribeToTopic("Haifa"));
	ASSERT_THROW(Client::NonSubscribedTopic,moving.unsubscribeToTopic("Eilat"));
	return true;
}

/*****************************************************************************/

bool unsubscribeAllTest () {
	Broker broker;
	stringstream ss;

	Subscriber player(1,broker,ss);
	player.subscribeToTopic("organ");
	player.subscribeToTopic("piano");
	player.subscribeToTopic("guitar");
	player.subscribeToTopic("drums");
	player.subscribeToTopic("bass");
	player.unsubscribeAll();

	return true;
}

/*****************************************************************************/
bool receiveMessageTest () {
	Broker broker;
	stringstream ss;

	Publisher executioner(2,broker,ss);
	Subscriber got(3,broker,ss);
	got.subscribeToTopic("Hodor");
	got.subscribeToTopic("Ned Stark");
	got.subscribeToTopic("Osha");
	got.subscribeToTopic("Ramsay");
	got.subscribeToTopic("Jon Snow");
	got.subscribeToTopic("Jeoffry Baratheon");

	got.receiveMessage("He's unfortunately dead","Hodor",executioner);
	got.receiveMessage("Beheaded","Ned Stark",executioner);
	got.receiveMessage("Stabbed","Osha",executioner);
	got.receiveMessage("Fed to the hounds","Ramsay",executioner);
	got.receiveMessage("Somehow he's alive again","Jon Snow",executioner);
	got.receiveMessage("Poisoned","Jeoffry Baratheon",executioner);
	//Not subscribed to topic - throw
	ASSERT_THROW(Client::NonSubscribedTopic,
			got.receiveMessage("Shot with an arrow","Rikkon",executioner));

	stringstream expected;
	expected << "Topic: " << "Hodor" << ". Sender: #" <<
			executioner.getId() << ". Receiver: #" << got.getId() <<
			". Message: " << "He's unfortunately dead" << endl;
	expected << "Topic: " << "Ned Stark" << ". Sender: #" <<
			executioner.getId() << ". Receiver: #" << got.getId() <<
			". Message: " << "Beheaded" << endl;
	expected << "Topic: " << "Osha" << ". Sender: #" <<
			executioner.getId() << ". Receiver: #" << got.getId() <<
			". Message: " << "Stabbed" << endl;
	expected << "Topic: " << "Ramsay" << ". Sender: #" <<
			executioner.getId() << ". Receiver: #" << got.getId() <<
			". Message: " << "Fed to the hounds" << endl;
	expected << "Topic: " << "Jon Snow" << ". Sender: #" <<
			executioner.getId() << ". Receiver: #" << got.getId() <<
			". Message: " << "Somehow he's alive again" << endl;
	expected << "Topic: " << "Jeoffry Baratheon" << ". Sender: #" <<
			executioner.getId() << ". Receiver: #" << got.getId() <<
			". Message: " << "Poisoned" << endl;

	ASSERT_EQUALS(expected.str(), ss.str());

	got.unsubscribeToTopic("Ned Stark");
	//not subscribed to topic anymore
	ASSERT_THROW(Client::NonSubscribedTopic,
		got.receiveMessage("We loved him the most", "Ned Stark", executioner));
	got.unsubscribeAll();
	//not subscribed to anything after unsubscribeAll
	ASSERT_THROW(Client::NonSubscribedTopic,
			got.receiveMessage("The red woman", "Jon Snow", executioner));
	ASSERT_THROW(Client::NonSubscribedTopic,
			got.receiveMessage("Hold the door!", "Hodor", executioner));
	ASSERT_THROW(Client::NonSubscribedTopic,
			got.receiveMessage("Good riddance", "Ramsay", executioner));
	return true;
}

/*****************************************************************************/

bool publishTopicTest() {
	Broker broker;
	stringstream ss;

	Publisher animals(2,broker,ss);

	ASSERT_THROW(Client::IllegalPriority,Publisher(-2,broker,ss));

	animals.publishTopic("Dogs");
	animals.publishTopic("Cats");
	animals.publishTopic("Horses");
	animals.publishTopic("Cows");
	animals.publishTopic("Pigs");

	return true;
}

/*****************************************************************************/

bool unpublishTopicTest() {
	Broker broker;
	stringstream ss;

	Publisher animals(2,broker,ss);

	animals.publishTopic("Dogs");
	animals.publishTopic("Cats");
	animals.publishTopic("Horses");
	animals.publishTopic("Cows");
	animals.publishTopic("Pigs");

	// Unpublish from topic its not publishing to
	ASSERT_THROW(Client::NonPublishedTopic, animals.unpublishTopic("Rabbits"));
	ASSERT_THROW(Client::NonPublishedTopic, animals.unpublishTopic("Mice"));
	ASSERT_THROW(Client::NonPublishedTopic, animals.unpublishTopic("Fish"));

	animals.unpublishTopic("Dogs");
	animals.unpublishTopic("Cats");
	animals.unpublishTopic("Horses");
	animals.unpublishTopic("Cows");
	animals.unpublishTopic("Pigs");
	//Unpublish to removed topic
	ASSERT_THROW(Client::NonPublishedTopic, animals.unpublishTopic("Pigs"));

	animals.publishTopic("Pigs");
	ASSERT_NO_THROW(animals.unpublishTopic("Pigs"));
	return true;
}

/*****************************************************************************/

bool unpublishAllTest() {
	Broker broker;
	stringstream ss;

	Publisher color(5, broker, ss);
	color.publishTopic("blue");
	color.publishTopic("black");
	color.publishTopic("green");
	color.publishTopic("red");
	color.publishTopic("yellow");

	color.unpublishAll();

	ASSERT_THROW(Client::NonPublishedTopic,(color.unpublishTopic("blue")));
	return true;
}

/*****************************************************************************/

bool sendMessageTest() {
	Broker broker;
	stringstream ss;
	stringstream expected;

	Publisher mail(10, broker, ss);
	mail.publishTopic("Delivery");
	mail.sendMessage("Nobody should see this", "Delivery");
	ASSERT_EQUALS(expected.str(), ss.str());
	Subscriber customer1(5, broker, ss);
	Subscriber customer2(2, broker, ss);
	Subscriber customer3(4, broker, ss);
	Subscriber customer4(3, broker, ss);
	Subscriber customer5(5, broker, ss);
	Subscriber customer6(5, broker, ss);
	customer1.subscribeToTopic("Delivery");
	customer3.subscribeToTopic("Delivery");
	customer2.subscribeToTopic("Delivery");
	customer4.subscribeToTopic("Delivery");
	customer5.subscribeToTopic("Delivery");
	mail.sendMessage("Your package is here!","Delivery");

	expected << "Topic: " << "Delivery" << ". Sender: #" << mail.getId()
			<< ". Receiver: #" << customer2.getId() << ". Message: "
			<< "Your package is here!" << endl;

	expected << "Topic: " << "Delivery" << ". Sender: #" << mail.getId()
			<< ". Receiver: #" << customer4.getId() << ". Message: "
			<< "Your package is here!" << endl;

	expected << "Topic: " << "Delivery" << ". Sender: #" << mail.getId()
			<< ". Receiver: #" << customer3.getId() << ". Message: "
			<< "Your package is here!" << endl;
	expected << "Topic: " << "Delivery" << ". Sender: #" << mail.getId()
			<< ". Receiver: #" << customer1.getId() << ". Message: "
			<< "Your package is here!" << endl;
	expected << "Topic: " << "Delivery" << ". Sender: #" << mail.getId()
			<< ". Receiver: #" << customer5.getId() << ". Message: "
			<< "Your package is here!" << endl;

	ASSERT_EQUALS(expected.str(), ss.str());

	mail.unpublishTopic("Delivery");
	ASSERT_THROW(Client::NonPublishedTopic,
			mail.sendMessage("Spam mail", "Delivery"));

	mail.publishTopic("Special Delivery");
	mail.sendMessage("No taxes", "Special Delivery");

	ASSERT_EQUALS(expected.str(), ss.str());
	return true;
}

/*****************************************************************************/

bool sendMaintenanceMessageAnyTest() {
	Broker broker;
	stringstream ss;
	stringstream expected;
	//publishers
	Publisher Mccdonalds(7, broker, ss);
	Publisher Burger_king(9, broker, ss);
	Publisher Zesty(1, broker, ss);
	Publisher Japanika(8, broker, ss);
	Publisher Max_Brenner(2, broker, ss);
	//subscribers
	Subscriber customer1(5, broker, ss);
	Subscriber customer2(9, broker, ss);
	Subscriber customer3(3, broker, ss);
	Subscriber customer4(7, broker, ss);
	Subscriber customer5(4, broker, ss);

	//creating topics list
	std::list<Topic> topics;
	topics.insert(topics.begin(), "Desserts");
	topics.insert(topics.begin(), "Main courses");
	topics.insert(topics.begin(), "Appetizers");
	topics.insert(topics.begin(), "Brunches");
	topics.insert(topics.begin(), "Salads");
	topics.insert(topics.begin(), "Midnight snacks");

	broker.sendMaintenanceMessageAny(topics, "Everybody is HUNGRY!");
	ASSERT_EQUALS(expected.str(), ss.str());

	customer5.subscribeToTopic("Junk Food");

	customer1.subscribeToTopic("Appetizers");
	customer2.subscribeToTopic("Salads");
	customer3.subscribeToTopic("Salads");
	customer4.subscribeToTopic("Salads");
	Mccdonalds.publishTopic("Midnight snacks");
	Burger_king.publishTopic("Brunches");
	Zesty.publishTopic("Appetizers");
	Japanika.publishTopic("Main courses");
	Max_Brenner.publishTopic("Desserts");


	broker.sendMaintenanceMessageAny(topics, "Sushi Rolls on the house!");

	expected << "Client #" << Zesty.getId() << " received maintenance message: "
			<< "Sushi Rolls on the house!" << endl;
	expected << "Client #" << Max_Brenner.getId()
			<< " received maintenance message: " << "Sushi Rolls on the house!"
			<< endl;
	expected << "Client #" << customer3.getId()
			<< " received maintenance message: " << "Sushi Rolls on the house!"
			<< endl;
	expected << "Client #" << customer1.getId()
			<< " received maintenance message: " << "Sushi Rolls on the house!"
			<< endl;
	expected << "Client #" << Mccdonalds.getId()
			<< " received maintenance message: " << "Sushi Rolls on the house!"
			<< endl;
	expected << "Client #" << customer4.getId()
			<< " received maintenance message: " << "Sushi Rolls on the house!"
			<< endl;
	expected << "Client #" << Japanika.getId()
			<< " received maintenance message: " << "Sushi Rolls on the house!"
			<< endl;
	expected << "Client #" << Burger_king.getId()
			<< " received maintenance message: " << "Sushi Rolls on the house!"
			<< endl;
	expected << "Client #" << customer2.getId()
			<< " received maintenance message: " << "Sushi Rolls on the house!"
			<< endl;

ASSERT_EQUALS(expected.str(),ss.str());
	return true;
}

/****************************************************************************/

bool sendMaintenanceMessageAllTest() {
	Broker broker;
	stringstream ss;
	stringstream expected;
	//publishers
	Publisher Mccdonalds(7, broker, ss);
	Publisher Burger_king(9, broker, ss);
	Publisher Zesty(1, broker, ss);
	Publisher Japanika(8, broker, ss);
	Publisher Max_Brenner(2, broker, ss);
	//subscribers
	Subscriber customer1(5, broker, ss);
	Subscriber customer2(9, broker, ss);
	Subscriber customer3(3, broker, ss);
	Subscriber customer4(7, broker, ss);
	Subscriber customer5(4, broker, ss);

	//creating topics list
	std::list<Topic> topics;
	topics.insert(topics.begin(), "Desserts");
	topics.insert(topics.begin(), "Main courses");
	topics.insert(topics.begin(), "Salads");

	customer5.subscribeToTopic("Salads");
	customer5.subscribeToTopic("Desserts");

	customer1.subscribeToTopic("Salads");
	customer2.subscribeToTopic("Salads");
	customer3.subscribeToTopic("Salads");
	customer4.subscribeToTopic("Salads");
	Mccdonalds.publishTopic("Salads");
	Burger_king.publishTopic("Salads");
	Zesty.publishTopic("Salads");
	Japanika.publishTopic("Salads");
	Max_Brenner.publishTopic("Salads");

	customer1.subscribeToTopic("Main courses");
	customer2.subscribeToTopic("Main courses");
	customer3.subscribeToTopic("Main courses");
	customer4.subscribeToTopic("Main courses");
	Mccdonalds.publishTopic("Main courses");
	Burger_king.publishTopic("Main courses");
	Zesty.publishTopic("Main courses");
	Japanika.publishTopic("Main courses");
	Max_Brenner.publishTopic("Main courses");

	customer1.subscribeToTopic("Desserts");
	customer2.subscribeToTopic("Desserts");
	customer3.subscribeToTopic("Desserts");
	customer4.subscribeToTopic("Desserts");
	Mccdonalds.publishTopic("Desserts");
	Burger_king.publishTopic("Desserts");
	Zesty.publishTopic("Desserts");
	Japanika.publishTopic("Desserts");
	Max_Brenner.publishTopic("Desserts");

	broker.sendMaintenanceMessageAll(topics, "Sushi Rolls on the house!");

	expected << "Client #" << Zesty.getId() << " received maintenance message: "
			<< "Sushi Rolls on the house!" << endl;
	expected << "Client #" << Max_Brenner.getId()
			<< " received maintenance message: " << "Sushi Rolls on the house!"
			<< endl;
	expected << "Client #" << customer3.getId()
			<< " received maintenance message: " << "Sushi Rolls on the house!"
			<< endl;
	expected << "Client #" << customer1.getId()
			<< " received maintenance message: " << "Sushi Rolls on the house!"
			<< endl;
	expected << "Client #" << Mccdonalds.getId()
			<< " received maintenance message: " << "Sushi Rolls on the house!"
			<< endl;
	expected << "Client #" << customer4.getId()
			<< " received maintenance message: " << "Sushi Rolls on the house!"
			<< endl;
	expected << "Client #" << Japanika.getId()
			<< " received maintenance message: " << "Sushi Rolls on the house!"
			<< endl;
	expected << "Client #" << Burger_king.getId()
			<< " received maintenance message: " << "Sushi Rolls on the house!"
			<< endl;
	expected << "Client #" << customer2.getId()
			<< " received maintenance message: " << "Sushi Rolls on the house!"
			<< endl;

	ASSERT_EQUALS(expected.str(), ss.str());

	topics.insert(topics.begin(), "Sushi");
	broker.sendMaintenanceMessageAll(topics, "Sushi Rolls on the house!");
	ASSERT_EQUALS(expected.str(), ss.str());



	return true;
}

/****************************************************************************/

bool EncryptionTest() {
	Broker broker;
	stringstream ss;
	stringstream expected;

	EncryptionPublisher Cafe_cafe(1, broker, static_cast<char>(0xFF), ss);
	EncryptionPublisher Mandarin(2, broker, static_cast<char>(0xFF), ss);
	EncryptionSubscriber customer1(3, broker, static_cast<char>(0xFF), ss);
	EncryptionSubscriber customer2(2, broker, static_cast<char>(0xFF), ss);
	EncryptionSubscriber customer3(4, broker, static_cast<char>(0xFF), ss);
	EncryptionSubscriber customer4(2, broker, static_cast<char>(0xFF), ss);
	EncryptionSubscriber customer5(2, broker, static_cast<char>(0xFF), ss);

	Cafe_cafe.publishTopic("Afternoon Sale");
	Mandarin.publishTopic("Free Ice-coffee");

	customer1.subscribeToTopic("Afternoon Sale");
	customer2.subscribeToTopic("Afternoon Sale");
	customer3.subscribeToTopic("Afternoon Sale");
	customer4.subscribeToTopic("Afternoon Sale");
	customer5.subscribeToTopic("Afternoon Sale");

	Cafe_cafe.sendMessage("All pasta meals for 20% off!", "Afternoon Sale");

	expected << "Topic: " << "Afternoon Sale" << ". Sender: #"
			<< Cafe_cafe.getId() << ". Receiver: #" << customer2.getId()
			<< ". Message: " << "All pasta meals for 20% off!" << endl;

	expected << "Topic: " << "Afternoon Sale" << ". Sender: #"
			<< Cafe_cafe.getId() << ". Receiver: #" << customer4.getId()
			<< ". Message: " << "All pasta meals for 20% off!" << endl;

	expected << "Topic: " << "Afternoon Sale" << ". Sender: #"
			<< Cafe_cafe.getId() << ". Receiver: #" << customer5.getId()
			<< ". Message: " << "All pasta meals for 20% off!" << endl;
	expected << "Topic: " << "Afternoon Sale" << ". Sender: #"
			<< Cafe_cafe.getId() << ". Receiver: #" << customer1.getId()
			<< ". Message: " << "All pasta meals for 20% off!" << endl;
	expected << "Topic: " << "Afternoon Sale" << ". Sender: #"
			<< Cafe_cafe.getId() << ". Receiver: #" << customer3.getId()
			<< ". Message: " << "All pasta meals for 20% off!" << endl;

	ASSERT_EQUALS(expected.str(), ss.str());
	return true;
}

/*****************************************************************************/

bool pubSubTest() {
	RUN_TEST(pubSubTestExample);
	//Publisher tests
	RUN_TEST(subscribeToTopicTest);
	RUN_TEST(unsubscribeToTopicTest);
	RUN_TEST(unsubscribeAllTest);
	RUN_TEST(receiveMessageTest);
	//Subscriber tests
	RUN_TEST(publishTopicTest);
	RUN_TEST(unpublishTopicTest);
	RUN_TEST(unpublishAllTest);
	RUN_TEST(sendMessageTest);
	//Broker tests
	RUN_TEST(sendMaintenanceMessageAnyTest);
	RUN_TEST(sendMaintenanceMessageAllTest);
	//Encryption test
	RUN_TEST(EncryptionTest);

	return true;
}

