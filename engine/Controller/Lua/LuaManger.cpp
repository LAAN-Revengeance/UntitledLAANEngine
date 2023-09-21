#include "LuaManager.h"

LuaManager::LuaManager()
{
	luaState;
	luaState.open_libraries(sol::lib::base);
	luaState.open_libraries(sol::lib::math);
	luaState.open_libraries(sol::lib::os);
}

LuaManager::~LuaManager()
{
	luaState.collect_garbage();
}


void LuaManager::ClearLuaState()
{
	luaState.collect_garbage();
	luaState.stack_clear();

	luaState.open_libraries(sol::lib::base);
	luaState.open_libraries(sol::lib::math);
	luaState.open_libraries(sol::lib::os);
}


void LuaManager::LoadScript(const std::string& fileName) {
	try
	{
		luaState.script_file(fileName);
	}
	catch (const sol::error e)
	{
		std::cout << "ERROR: Could not load script File:\n" << e.what() << std::endl;
	}
}



