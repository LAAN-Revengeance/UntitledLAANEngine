#include "Application.h"

Application::Application() :
	window(500,500,"Application"),
	game(&window),
	editor(&game)
{
	
}

Application::~Application()
{
}

void Application::Run()
{	
	timer.Reset();
	while (!window.IsWindowClosed())
	{
		timer.Update();
		window.PollEvents();

		double deltaTime = timer.DeltaTime();
		game.Update(deltaTime);
		editor.Update(deltaTime);

		game.Draw(deltaTime);
		editor.Draw(deltaTime);

		window.SwapBuffers();
	}
}
