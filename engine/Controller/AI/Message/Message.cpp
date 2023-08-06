#include "Message.h"
#include <math.h>

Message::Message(double dispatch, int sender, int receiver, int type, void* infoPtr):
	dispatchTime(dispatch),
	senderID(sender),
	receiverID(receiver),
	msgType(type),
	info(infoPtr)
{
}

bool Message::operator<(const Message& m2)const
{
	if (*this == m2)
		return false;
	return dispatchTime < m2.dispatchTime;
}

bool Message::operator>(const Message& m2)const
{
	if (*this == m2)
		return false;
	return dispatchTime > m2.dispatchTime;
}

bool Message::operator==(const Message& m2)const
{
	return fabs(dispatchTime - m2.dispatchTime) < 0.05 &&
		senderID == m2.senderID &&
		receiverID == m2.receiverID &&
		msgType == m2.msgType;
}
