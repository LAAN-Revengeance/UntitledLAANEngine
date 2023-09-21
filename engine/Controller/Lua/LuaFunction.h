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

	void VoidExecute(Args ...args) {

		if (solFunc.valid()) {
			solFunc(args...);
		}
	}


private:
	sol::function solFunc;
};


//template<typename Func, typename ...Args>
//inline void LuaManager::Expose_CPPFunction(const char* luaName, Func cppFunc, Args ...args)
//{
//	luaState.set_function(luaName, cppFunc, args);
//}