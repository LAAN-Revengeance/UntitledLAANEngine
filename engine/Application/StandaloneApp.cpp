#include "StandaloneApp.h"

GaemApp::GaemApp(const char* path) :
	window(1920, 1080, "Application", &eventDispatcher),
	game(&window, &eventDispatcher, &timer)
{
	Project projectData = ProjectLoader::LoadProject(&game,path);
	game.scene = projectData.scene;
	window.SetName(projectData.windowName);

	for (auto shader = ResourceManager::Get().ShaderBegin(); shader != ResourceManager::Get().ShaderEnd(); ++shader) {
		game.renderer.SetLightUniforms(game.scene->lights, *(*shader).second);
	}
}

GaemApp::~GaemApp()
{
}

void GaemApp::Run()
{
	game.scene->InitFunction.Execute();
	logger.SetLogLevel(GAEM_ALL, false);

	timer.Reset();
	while (!window.IsWindowClosed())
	{
		timer.Update();

		double deltaTime = timer.DeltaTime();
		window.PollEvents();

		game.Draw(deltaTime);

		game.Update(deltaTime);

		window.SwapBuffers();
	}
}
