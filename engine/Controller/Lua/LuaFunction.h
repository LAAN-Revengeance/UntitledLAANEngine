#pragma once
#include <sol/sol.hpp>



template<class T, typename... Args>
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

	T Run(Args... args) {
		if (solFunc.valid()) {
			return solFunc(args);
		}
		return T;//sus
	}

private:
	sol::function solFunc;
};