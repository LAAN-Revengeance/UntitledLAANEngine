#pragma once
#include <sol/sol.hpp>
#include <type_traits>


template<class T, typename ...Args>
class LuaFunction
{
	class LuaManager;
public:
	LuaFunction() {
		solFunc = sol::nil;
	}

	LuaFunction(const char* luaName, LuaManager* luaState) {
		solFunc = luaState->GetFunction(luaName);
	}

	~LuaFunction() {

	}

	T Execute(Args ...args) {

		if (solFunc.valid()) {
			return (T)solFunc(args...);
		}
		T value;
		return value;
	}

private:
	sol::function solFunc;
};

// Template specialization for Execute when T is void
template<typename ...Args>
class LuaFunction<void, Args...>
{
	class LuaManager;
public:
	LuaFunction() {
		solFunc = sol::nil;
	}

	LuaFunction(const char* luaName, LuaManager* luaState) {
		solFunc = luaState->GetFunction(luaName);
	}

	~LuaFunction() {

	}

	void Execute(Args ...args) {

		if (solFunc.valid()) {
			solFunc(args...);
		}
	}

private:
	sol::function solFunc;
};

