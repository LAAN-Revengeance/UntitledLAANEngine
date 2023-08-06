#include "MessageDispatcher.h"

Dispatcher& Dispatcher::Get()
{
	static Dispatcher d_instance;
	return d_instance;
}

void Dispatcher::SendMessage(double delay, int sender, int receiver, int type, void* data)
{
	if (!scene) {
		std::cerr << "No Scene assigned in message dispatcher!" << std::endl;
		return;
	}

	GameObject* nReceiver = scene->GetObjectByID(receiver);

	if (!nReceiver && receiver != -1) {
		std::cerr << "Could not send message!" << std::endl;
		return;
	}

	Message message(0.0,sender,receiver,type,data);

	if (delay < 0.1) {
		if (receiver == -1) {
			for (auto& it : scene->gameObjects) {
				it.second->stateMachine.ReceiveMessage(&message);
			}
		}
		else {
			nReceiver->stateMachine.ReceiveMessage(&message);
		}
	}
	else {
		message.dispatchTime = delay + Timer::Get().Time();
		msgQueue.insert(message);
	}
}

void Dispatcher::SendMsgQueue()
{
	double time = Timer::Get().Time();
	while (!msgQueue.empty() && msgQueue.begin()->dispatchTime < time)
	{
		if (msgQueue.begin()->receiverID == -1) {
			for (auto& it : scene->gameObjects) {
				it.second->stateMachine.ReceiveMessage(&*msgQueue.begin());
			}
		}
		else {
			GameObject* rec = scene->GetObjectByID(msgQueue.begin()->receiverID);
			if (rec)
				rec->stateMachine.ReceiveMessage(&*msgQueue.begin());
			
		}
		msgQueue.erase(msgQueue.begin());
	}
}

void Dispatcher::SetScene(Scene* nScene)
{
	scene = nScene;
}

Dispatcher::Dispatcher()
{

}

Dispatcher::~Dispatcher()
{

}
