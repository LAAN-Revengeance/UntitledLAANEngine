#pragma once
#include <sol/sol.hpp>
#include <type_traits>
#include "LuaManager.h"
#include <Utils/DebugLogger.h>

/**
*	@Class LuaFunction
*	@brief stores a lua function extracted from a lua managers current lua state
*	and provides easy interfacing with it through c++
*
*	@author Andres Comeros-Ochtman
*	@version 1.0
*	@date 21/09/2023
*/
template<class T, typename ...Args>
class LuaFunction
{
public:
	LuaFunction() {
		solFunc = sol::nil;
	}

	LuaFunction(const char* luaName, LuaManager* luaState) {

		funcName = luaName;
		solFunc = luaState->luaState[luaName];
	}

	~LuaFunction() {

	}

	T Execute(Args ...args) {

		if (solFunc.valid()) {
			return (T)solFunc(args...);
		}
		T value{};
		return value;
	}

	std::string GetName() {
		return funcName;
	}

private:
	sol::function solFunc;
	std::string funcName = "";
};

// Template specialization for Execute when T is void
template<typename ...Args>
class LuaFunction<void, Args...>
{
public:
	LuaFunction() {
		solFunc = sol::nil;
	}

	LuaFunction(const char* luaName, LuaManager* luaState) {
		solFunc = luaState->luaState[luaName];
		if (solFunc.valid()) {
			funcName = luaName;
			DebugLogger::Log(GAEM_LOG, std::string("Lua function loaded: ") + luaName);
		}
		else {
			DebugLogger::Log(GAEM_ERROR, std::string("Could not load Lua function: ") + luaName);
			funcName = "";
		}
	}

	~LuaFunction() {

	}

	void Execute(Args ...args) {

		if (solFunc.valid() && solFunc != sol::nil) {
			solFunc(args...);
		}
	}

	std::string GetName() {
		return funcName;
	}

private:
	sol::function solFunc;
	std::string funcName = "";
};

