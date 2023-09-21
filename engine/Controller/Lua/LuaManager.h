#include <sol/sol.hpp>
#include <Scene.h>
#include <GameObject.h>
#include <Physics/PhysicsManager.h>
#include <ResourceManager.h>
#include <InputManager.h>
#include <GUIRenderer.h>
#include <Renderer.h>
#include <SoundEngine.h>
#include <Serialization/SceneLoader.h>

template<class T, typename... Args>
class LuaFunction;

/**
*	@Class LuaManager
*	@brief Provided an interface to expose data, classes and functions
*	to lua. Also provides ability to expose engine API to the lua state
*
*	@author Andres Comeros-Ochtman
*	@version 1.0
*	@date 15/04/2023
* 
*	@author Andres Comeros-Ochtman
*	@version 2.0
*	@date 21/09/2023
*/
class LuaManager
{
public:

		/**
		*	Default Constructor. Creates the main lua state and
		*	opens all required lua libraries.
		*/
	LuaManager();

		/**
		*	Destructor. Frees LuaState data
		*/
	~LuaManager();

		/**
		*	@brief Restarts the luaState
		*	@return void
		*/
	void ClearLuaState();

		/**
		*	@brief Loads a lua scrpit file into the main luastate 
		*	@param fileName file path of lua script
		*	@return void
		*/
	void LoadScript(const std::string& fileName);

		/**
		*	@brief Attempts to collect a lua function from the main luastate
		*	@param luaName name of the function being extracted
		*	@return sol function extracted
		*/
	template<class T, typename... Args>
	LuaFunction<T, Args...> GetFunction(const char* luaName);

		/**
		*	@brief extract variable from the lua state
		*	@param luaName name of varaible to extract
		*	@return variable from the luastate
		*/
	template<typename T>
	T GetData(const char* luaName);

		/**
		*	@brief expose data from cpp to luastate
		*	@param luaName name of the varaible in the lua state
		*	@param cppData data being passed to lua
		*	@return void
		*/
	template<typename T>
	void Expose_CPPVariable(const char* luaName, T cppData);

		/**
		*	@brief Expose a reference to data/object from cpp to lua
		*	@param luaName name of data in the lua state
		*	@param cppData data being referenced
		*	@return void
		*/
	template<typename T>
	void Expose_CPPReference(const char* luaName, T& cppData);

		/**
		*	@brief Expose a cpp function to the lua state
		*	@param luaName name of the function in the lua state
		*	@param cppFunc the cpp function 
		*	@param args arguments to send to sol
		*	@return void
		*/
	template<typename Func, typename... Args>
	void Expose_CPPFunction(const char* luaName, Func cppFunc, Args... args);

		/**
		*	@brief Expose a cpp class to the lua state
		*	@param luaName name of the class in the lua state
		*	@param args arguments to send to sol
		*	@return void
		*/
	template<typename Class, typename... Args>
	void Expose_CPPClass(const char* luaName, Args... args);

private:
		///Main lua state
	sol::state luaState;

	friend class LuaGameBridge;
};

template<class T, typename... Args>
class LuaFunction
{
public:
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

template<class T, typename ...Args>
inline LuaFunction<T, Args...> LuaManager::GetFunction(const char* luaName)
{
	sol::function func = luaState[luaName];
	if (func.valid()) {
		return func;
	}
	else {
		std::cout << "ERROR: Could not retrieve function: " << luaName << std::endl;
		return sol::nil;
	}

	return LuaFunction<T, Args...>(luaName);
}

template<typename T>
inline T LuaManager::GetData(const char* luaName)
{
	try {
		return luaState.get<T>(luaName);
	}
	catch (const sol::error& e) {
		std::cout << "ERROR: could not get data: " << e.what() << std::endl;
		return T();
	}
}

template<typename T>
inline void LuaManager::Expose_CPPVariable(const char* luaName, T cppData)
{
	luaState.set(luaName, cppData);
}

template<typename T>
inline void LuaManager::Expose_CPPReference(const char* luaName, T& cppData)
{
	luaState[luaName] = &cppData;
}

template<typename Func, typename ...Args>
inline void LuaManager::Expose_CPPFunction(const char* luaName, Func cppFunc, Args ...args)
{
	luaState.set_function(luaName, cppFunc, args);
}

template<typename Class, typename ...Args>
inline void LuaManager::Expose_CPPClass(const char* luaName, Args ...args)
{
	luaState.new_usertype<Class>(luaName, args...);
}