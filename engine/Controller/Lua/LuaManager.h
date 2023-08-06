#include <sol/sol.hpp>
#include <Scene.h>
#include <GameObject.h>
#include <ResourceManager.h>
#include <InputManager.h>
#include <GUIRenderer.h>
#include <Renderer.h>
#include <AI/States/State.h>
#include <SoundEngine.h>
#include <Serialization/SceneLoader.h>
#include <AI/States/global_states.h>

/**
*	@Class LuaManager
*	@brief Provided an interface to expose data, classes and functions
*	to lua. Also provides ability to expose engine API to the lua state
*
*	@author Andres Comeros-Ochtman
*	@version 1.0
*	@date 15/04/2023
*/
class LuaManager
{
public:
		/**
		*	@brief Get the singleton instance
		*	@return the singleton instance
		*/
	static LuaManager& Get();
		/**
		*	@brief Runs the init() function extracted from lua
		*	@return void
		*/
	void RunInitMethod();

		/**
		*	@brief Runs the update(deltaTime) method extracted from lua
		*	@param dt deltaTime value used by the lua update function
		*	@return void
		*/
	void RunUpdateMethod(double dt);

		/**
		*	@brief Exposes The Gaem Engine API to the lua state
		*	@return void
		*/
	void Expose_Engine();

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
	sol::function GetFunction(const char* luaName);

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
		/**
		*	Default Constructor. Creates the main lua state and
		*	opens all required lua libraries.
		*/
	LuaManager();
		/**
		*	Destructor. Frees LuaState data
		*/
	~LuaManager();
	LuaManager(const LuaManager&) = delete;
	LuaManager& operator = (const LuaManager&) = delete;

		///Main lua state
	sol::state luaState;
		///Update function extracted from main.lua
	sol::function update;
		///Init function extracted from main.lua
	sol::function init;
};

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

