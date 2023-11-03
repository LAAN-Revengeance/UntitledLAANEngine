#include "LuaManager.h"
#include <queue>
#include <iostream>
#include <fstream>
#include <string>
#include <regex>

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
		DebugLogger::Log(GAEM_ERROR, std::string("Could not load script File:") + e.what());
	}
}

std::vector<std::string> LuaManager::GetFunctionNames(const std::string& fileName)
{
    std::vector<std::string> functionNames;

    std::ifstream file(fileName);

    if (!file.is_open()) {
        return functionNames;
    }

    //use regex to find all basic functions, does not cover all cases but shoudl get basic functions
    std::regex function_regex(R"(\bfunction\s+(\w+))");

    std::string line;
    while (std::getline(file, line)) {
        std::smatch matches;

        if (std::regex_search(line, matches, function_regex)) {
            if (matches.size() >= 2) {
                std::ssub_match sub_match = matches[1];
                std::string function_name = sub_match.str();
                functionNames.push_back(function_name);
            }
        }
    }

    file.close();

	return functionNames;
}



