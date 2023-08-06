#include "GaemEngine.h"
#include <string.h>
#include <iostream>

GameEngine& GameEngine::Get() {

	static GameEngine e_instance;
	return e_instance;
}

void GameEngine::ExposeToLua(){

	luaManager.Expose_CPPClass<GameEngine>("GameEngine",
		sol::no_constructor,
		"Time", &GameEngine::Time,
		"DeltaTime", &GameEngine::DeltaTime,
		"Shutdown", &GameEngine::Shutdown,
		"IsSimRunning", &GameEngine::IsSimRunning,
		"SetSimulation", &GameEngine::SetSimulation,
		"scene", &GameEngine::scene,
		"SwitchScenes", &GameEngine::SwitchScenes,
		"SetWindowType", &GameEngine::SetWindowType,
		"SetWindowIcon", &GameEngine::SetWindowIcon,
		"SetWindowName", &GameEngine::SetWindowName
		);

	luaManager.Expose_CPPClass<ScriptableState>("ScriptableState",
		sol::constructors<ScriptableState(sol::function, sol::function, sol::function, sol::function)>(),
		sol::base_classes, sol::bases<State>()
		);

	luaManager.Expose_Engine();
	luaManager.Expose_CPPReference("engine",*this);
	luaManager.Expose_CPPReference("scene", *scene);
	luaManager.Expose_CPPReference("physics", scene->physics);
	luaManager.Expose_CPPReference("renderer", renderer);
	luaManager.Expose_CPPReference("GUI", guirenderer);
}

GameEngine::GameEngine()

{
	//init window and glfw.
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	window = glfwCreateWindow(wWidth, wHeight, "Engine", NULL, NULL);

	if (!window)
	{
		std::cout << "ERROR Could not initalize window." << std::endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(window);

	//glad required to access GL functions
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "ERROR: Could not load glad" << std::endl;
		return;
	}

	//scene camera settings
	scene = new Scene;
	scene->camera.aspectRatio = (float)wWidth / (float)wHeight;

	inputMngr.Init(window);
	renderer.Init(window);
	guirenderer.Init(window);
	aiManager.Init(scene);

	//callbacks
	glfwSetFramebufferSizeCallback(window, ResizeCallback);

	//expose to lua
	ExposeToLua();
	luaManager.RunInitMethod();

	//set light uniforms
	auto it = ResourceManager::Get().ShaderBegin();
	auto end = ResourceManager::Get().ShaderEnd();
	for (it; it != end; it++) {
		Renderer::SetLightUniforms(scene->lights, *it->second);
	}

}

GameEngine::~GameEngine() {
	//do some cleanup
}

//start main loop
void GameEngine::Run() {
	isRunning = true;
	//main loop
	while (!glfwWindowShouldClose(window))
	{
		// timer
		double currentFrameTime = glfwGetTime();
		deltaTime = currentFrameTime - previousFrameTime;
		previousFrameTime = currentFrameTime;
		//accumulator += deltaTime;
		
		inputMngr.KeyActions(deltaTime);

		if (simIsRunning) {
			scene->physics.StepPhysics(deltaTime);
			scene->physics.UpdateGameObjects(scene->gameObjects);
			aiManager.UpdateAgents(deltaTime);
		}
		else {
			deltaTime = 0.0f;
		}

		renderer.Draw(*scene, deltaTime);
		scene->physics.DrawDebug(&scene->camera, ResourceManager::Get().GetShader("physics"));
		luaManager.RunUpdateMethod(deltaTime);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	isRunning = false;

	//cleanup
	glfwDestroyWindow(window);
	glfwTerminate();
}

double GameEngine::Time()
{
	return glfwGetTime();
}

double GameEngine::DeltaTime() {
	return deltaTime;
}

void GameEngine::ResizeCallback(GLFWwindow* window, int width, int height) {

	Scene& s = *GameEngine::Get().scene;
	s.camera.aspectRatio = (float)width / (float)height;
	glViewport(0, 0, width, height);
	GameEngine::Get().renderer.Draw(s, GameEngine::Get().deltaTime);
}

void GameEngine::Shutdown()
{
	isRunning = false;
	glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void GameEngine::SetSimulation(bool isRun)
{
	simIsRunning = isRun;
}

bool GameEngine::IsSimRunning()
{
	return simIsRunning;
}

void GameEngine::SwitchScenes(Scene& nscene)
{
	SceneLoader loader;
	delete scene;
	scene = &nscene;
	luaManager.Expose_CPPReference("scene", nscene);
	luaManager.Expose_CPPReference("physics", nscene.physics);
	aiManager.Init(&nscene);
}

void GameEngine::SetWindowType(int type)
{
	GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();;
	const GLFWvidmode* videoMode = glfwGetVideoMode(primaryMonitor);;
	switch (type) {
	case 1: // Windowed Window
		glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_TRUE);
		glfwSetWindowAttrib(window, GLFW_RESIZABLE, GLFW_TRUE);
		glfwSetWindowMonitor(window, nullptr, 1, 1, videoMode->width, videoMode->height, 0);
		break;
	case 2: // Borderless Window
		glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_TRUE);
		glfwSetWindowAttrib(window, GLFW_RESIZABLE, GLFW_FALSE);
		glfwSetWindowMonitor(window, nullptr, 0, 0, videoMode->width, videoMode->height, 0);
		break;
	case 3: // Fullscreen Window
		glfwSetWindowMonitor(window, primaryMonitor, 0, 0, videoMode->width, videoMode->height, videoMode->refreshRate);
		break;
	}

}

void GameEngine::SetWindowIcon(std::string path)
{
	Texture tex(path.c_str());
	GLFWimage image;
	image.pixels = tex.GetImageData();
	image.width = tex.GetWidth();
	image.height = tex.GetHeight();
	glfwSetWindowIcon(window,1, &image);
}

void GameEngine::SetWindowName(std::string name)
{
	glfwSetWindowTitle(window,name.c_str());
}

