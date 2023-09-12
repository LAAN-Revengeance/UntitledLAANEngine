#include "ProjectLoader.h"

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

Project ProjectLoader::LoadProject(const char* inName)
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

    return project;
}

ProjectLoader::ProjectLoader()
{
}

ProjectLoader::~ProjectLoader()
{
}