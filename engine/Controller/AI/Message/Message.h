#pragma once
/**
*	@Struct Message
*	@brief Stores a message used by the dispatcher to communicate
*	between state machines
*
*	@author Andres Comeros-Ochtman
*	@version 1.0
*	@date 30/05/2023
*/
struct Message
{
		///Time this message should be sent
	double dispatchTime;
		///State Machine ID of sender
	int senderID;
		///State Machine ID of reciever
	int receiverID;
		///Type ID of message
	int msgType;
		///Arbitrary pointer to data, use the msgType to determine its type.
	void* info;
		///Constructor
	Message(double dispatch, int sender, int receiver, int type, void* infoPtr);

	//compare dispatch times
	bool operator< (const Message& m2)const;
	bool operator> (const Message& m2)const;
	bool operator==(const Message& m2)const;
};