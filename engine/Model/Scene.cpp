#include "Scene.h"

Scene::Scene() {}

Scene::~Scene() {}

Scene& Scene::operator=(const Scene& other)
{
    // Check for self-assignment
    if (this == &other)
    {
        return *this;
    }

    // Copy all the member variables from the other scene
    camera = other.camera;
    skybox = other.skybox;
    lights = other.lights;
    //physics = other.physics;
    gameObjects = other.gameObjects;
    gameObjectsID = other.gameObjectsID;

    return *this;
}

void Scene::AddObject(GameObject& gameObject) {
	gameObjects.insert(std::pair<std::string, GameObject*>(gameObject.name, &gameObject));
	gameObjectsID.insert(std::pair<int, GameObject*>(gameObject.GetID(), &gameObject));
}

void Scene::AddLights(Lights& nLights) {
	lights = nLights;
}

void Scene::SetSkybox(CubeMap* nSkybox) {
	skybox = nSkybox;
}

Camera& Scene::GetCamera() {
	return camera;
}

Lights& Scene::GetLights() {
	return lights;
}

GameObject& Scene::GetGameObject(std::string objName) {
	return *gameObjects.at(objName);
}

GameObject* Scene::GetObjectByID(int objID)
{
    if (gameObjectsID.find(objID) != gameObjectsID.end()) {
        return gameObjectsID.at(objID);
    }
    return nullptr;
}

