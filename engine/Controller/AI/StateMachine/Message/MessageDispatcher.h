#pragma once
#include <set>
#include <Scene.h>
#include <Timer.h>
#include <AI/StateMachine/StateMachine.h>

/**
*	@Class Dispatcher
*	@brief Stores sent messages and sends them to the correct reciever
*	when their dispatch time is reached
*
*	@author Andres Comeros-Ochtman
*	@version 1.0
*	@date 30/05/2023
*/
class Dispatcher
{
public:


	Dispatcher(Scene* nScene,Timer* nTimer);
	~Dispatcher();
	Dispatcher(const Dispatcher&) = delete;
	Dispatcher& operator = (const Dispatcher&) = delete;
		/**
		*	@brief Sends a message
		*	@param delay how long to wait before sending the message
		*	@param sender sender ID
		*	@param reciver reciever ID
		*	@param type type ID
		*	@param data pointer to arbitrary data used in message
		*	@return void
		*/
	void SendMessage(double delay, std::string& sender, std::string&  receiver, int type, void* data);

		/**
		*	@brief Sends all messages that are ready to be sent
		*	@return void
		*/
	void SendMsgQueue();

		/**
		*	@brief Sends set the scene messages will be sent in
		*	@brief nScene new scene messages will be sent in
		*	@return void
		*/
	void SetScene(Scene* nScene);
		
		///Return scene pointer
	Scene* GetScene();
private:
		///messages with delays
	std::set<Message> msgQueue;
		///Scene messages are sent in
	Scene* scene = nullptr;
	Timer* timer;

};