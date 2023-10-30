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

    return *this;
}

void Scene::AddObject(GameObject& gameObject) {
	gameObjects.insert(std::pair<std::string, GameObject*>(gameObject.name, &gameObject));
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


