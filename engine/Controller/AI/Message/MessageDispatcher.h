#pragma once
#include <set>
#include <Scene.h>
#include <Timer.h>
#include <AI/StateMachine.h>

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
		/**
		*	@brief Get the singleton instance
		*	@return void
		*/
	static Dispatcher& Get();

		/**
		*	@brief Sends a message
		*	@param delay how long to wait before sending the message
		*	@param sender sender ID
		*	@param reciver reciever ID
		*	@param type type ID
		*	@param data pointer to arbitrary data used in message
		*	@return void
		*/
	void SendMessage(double delay, int sender, int receiver, int type, void* data);

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

private:
		///messages with delays
	std::set<Message> msgQueue;
		///Scene messages are sent in
	Scene* scene = nullptr;

	//singleton
	Dispatcher();
	~Dispatcher();
	Dispatcher(const Dispatcher&) = delete;
	Dispatcher& operator = (const Dispatcher&) = delete;
};