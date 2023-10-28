#pragma once
#include <sol/sol.hpp>
#include <type_traits>
#include "LuaManager.h"
#include <Utils/DebugLogger.h>

template<class T, typename ...Args>
class LuaFunction
{
public:
	LuaFunction() {
		solFunc = sol::nil;
	}

	LuaFunction(const char* luaName, LuaManager* luaState) {
		solFunc = luaState->luaState[luaName];// GetFunction(luaName);
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

private:
	sol::function solFunc;

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
			DebugLogger::Log(GAEM_LOG, std::string("Lua function loaded: ") + luaName);
		}
		else {
			DebugLogger::Log(GAEM_ERROR, std::string("Could not load Lua function: ") + luaName);
		}
	}

	~LuaFunction() {

	}

	void Execute(Args ...args) {

		if (solFunc.valid() && solFunc != sol::nil) {
			solFunc(args...);
		}
	}

private:
	sol::function solFunc;
};

