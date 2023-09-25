#include "Application.h"

Application::Application() :
	window(1920,1080,"Application",&eventDispatcher),
	game(&window, &eventDispatcher),
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

		double deltaTime = timer.DeltaTime();

		game.Draw(deltaTime);
		editor.Draw(deltaTime);

		game.Update(deltaTime);
		editor.Update(deltaTime);
		
		window.SwapBuffers();
		window.PollEvents();
	}
}
