#include "ProjectLoader.h"
#include <Lua/LuaGameBridge.h>
#include <AI/Affordance/Global_Affordances.h>

void ProjectLoader::SaveProject(Scene* scene, const std::string luaFile, const std::string windowName, const std::string outName)
{
    SceneLoader::SaveScene(scene, outName);

    std::ifstream file(outName);
    Json::Reader reader;
    Json::Value sceneJSON;
    reader.parse(file, sceneJSON);

    sceneJSON["windowName"] = windowName;
    sceneJSON["luaPath"] = luaFile;

    std::ofstream out(outName);
    Json::StreamWriterBuilder builder;
    const std::string json_file = Json::writeString(builder, sceneJSON);
    out << json_file << std::endl;
    out.close();
}

Project ProjectLoader::LoadProject(GameEngine* engine, const char* inName)
{
    Project project;

    project.scene = &SceneLoader::LoadScene(inName);

    //get title and luapath
    std::ifstream file(inName);
    Json::Reader reader;
    Json::Value saveJSON;
    reader.parse(file, saveJSON);

    project.windowName = saveJSON["windowName"].asString();
    project.luaPath = saveJSON["luaPath"].asString();

    //lua
    LuaGameBridge::ExposeEngine(&project.scene->luaState);
    project.scene->luaState.Expose_CPPReference("scene", *project.scene);
    project.scene->luaState.Expose_CPPReference("GUI", engine->guiRenderer);
    project.scene->luaState.Expose_CPPReference("physics", project.scene->physicsWorld);

    project.scene->luaState.LoadScript(project.luaPath);
    project.scene->UpdateFunction = project.scene->luaState.GetFunction<void, double>("update");
    project.scene->InitFunction = project.scene->luaState.GetFunction<void>("init");

    //npc update fim
    Json::Value objects = saveJSON["objects"];

    for (unsigned int i = 0; i < objects.size(); i++) {
        std::string objName = objects[i]["name"].asString();
        if (dynamic_cast<NPC*>(project.scene->gameObjects.at(objName)))
        {
            std::string funcName = objects[i]["updateFunc"].asString();
            if (funcName != "") {
                dynamic_cast<NPC*>(project.scene->gameObjects.at(objName))->SetUpdateFunction(
                    project.scene->luaState.GetFunction<void, NPC&, float>(funcName.c_str())
                );
            }
        }

    }

    //make sure it hall all affordances
    for (auto& object : project.scene->gameObjects)
    {
        object.second->affordanceController.AddAffordance<AffordancePickup>();
        object.second->affordanceController.AddAffordance<AffordancePoke>();
        object.second->affordanceController.AddAffordance<AffordancePunch>();
        object.second->affordanceController.AddAffordance<AffordanceSlap>();
        object.second->affordanceController.AddAffordance<AffordanceSit>();
        object.second->affordanceController.AddAffordance<AffordanceGiveMoney>();
        object.second->affordanceController.AddAffordance<AffordanceCompliment>();
        object.second->affordanceController.AddAffordance<AffordanceThreaten>();
        object.second->affordanceController.AddAffordance<AffordanceGenerousOffer>();
    }

    return project;
}

ProjectLoader::ProjectLoader()
{

}

ProjectLoader::~ProjectLoader()
{

}