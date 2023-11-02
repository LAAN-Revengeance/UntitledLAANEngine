#include "MessageDispatcher.h"


void Dispatcher::SendMessage(double delay, std::string& sender, std::string& receiver, int type, void* data)
{
	if (!scene) {
		std::cerr << "No Scene assigned in message dispatcher!" << std::endl;
		return;
	}

	GameObject* nReceiver = &scene->GetGameObject(receiver);

	if (!nReceiver && receiver != "") {
		std::cerr << "Could not send message!" << std::endl;
		return;
	}

	Message message(0.0,sender,receiver,type,data);

	if (delay < 0.1) {
		if (receiver == "") {
			for (auto& it : scene->gameObjects) {
				it.second->stateMachine.ReceiveMessage(&message);
			}
		}
		else {
			nReceiver->stateMachine.ReceiveMessage(&message);
		}
	}
	else {
		message.dispatchTime = delay + timer->Time();
		msgQueue.insert(message);
	}
}

void Dispatcher::SendMsgQueue()
{
	double time = timer->Time();
	while (!msgQueue.empty() && msgQueue.begin()->dispatchTime < time)
	{
		if (msgQueue.begin()->receiverID == "") {
			for (auto& it : scene->gameObjects) {
				it.second->stateMachine.ReceiveMessage(&*msgQueue.begin());
			}
		}
		else {
			GameObject* rec = &scene->GetGameObject(msgQueue.begin()->receiverID);
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

Scene* Dispatcher::GetScene()
{
	return scene;
}

Dispatcher::Dispatcher(Scene* nScene, Timer* nTimer)
{
	scene = nScene;
	timer = nTimer;
}

Dispatcher::~Dispatcher()
{

}
