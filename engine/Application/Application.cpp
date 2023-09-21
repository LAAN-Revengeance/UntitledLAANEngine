#include "Application.h"

Application::Application() :
	window(500,500,"Application"),
	game(&window)
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
		//editor.Update();

		game.Draw(deltaTime);
		//editor.Draw();
		
	}

	window.SwapBuffers();
}
