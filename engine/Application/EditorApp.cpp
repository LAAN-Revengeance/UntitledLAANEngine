#include "EditorApp.h"

EditorApp::EditorApp() :
	window(1920,1080,"Application",&eventDispatcher),
	game(&window, &eventDispatcher, &timer),
	editor(&game, &logger)
{
}

EditorApp::~EditorApp()
{
}

void EditorApp::Run()
{	
	timer.Reset();
	while (!window.IsWindowClosed())
	{
		timer.Update();

		double deltaTime = timer.DeltaTime();
		window.PollEvents();

		game.Draw(deltaTime);
		editor.Draw(deltaTime);

		game.Update(deltaTime);
		editor.Update(deltaTime);
		
		window.SwapBuffers();
	}
}
