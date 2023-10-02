#pragma once
#include <imGuIZMO.quat/imGuIZMOquat.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <Graphics/Graphics.h>
#include <ResourceManager.h>
#include <Renderer.h>
#include <map>
#include <string>
#include <ImGuizmo.h>
#include <Window.h>

using GUIColor = ImVec4;

struct GUIStyle {

	GUIColor color_Background;
	GUIColor color_Text;

	GUIColor color_Primary;
	GUIColor color_Secondary;
	GUIColor color_Secondary_Light;
};

/**
*	@Class GUIRenderer
*	@brief Used to draw gui elements to the screen
*
*	@author Andres Comeros-Ochtman
*	@version 1.0
*	@date 15/04/2023
*/
class GUIRenderer
{
public:
		///Default constructor. Private becuase singleton
	GUIRenderer(Window* window);
		///Default Destructor.
	~GUIRenderer();
		/**
		*	@brief free up GUI resources
		*	@return void
		*/
	void Shutdown();

		/**
		*	@brief initalize gui
		*	@param nwindow window being rendered to
		*	@return void
		*/
	void Init(Window* nwindow);

		/**
		*	@brief set font to a ttf font
		*	@param path path to font
		*	@return void
		*/
	void SetFont(std::string path);

	void SetGUIStyle(GUIStyle& nStyle);

	//lua programmable GUI//

		/**
		*	@brief Starts a GUI to be rendered
		*	@return void
		*/
	void StartGUI();

		/**
		*	@brief ends GUI rendering
		*	@return void
		*/
	void EndGUI();

		/**
		*	@brief Start rendering a window
		*	@param background were or not render the window
		*	@param width ratio width of window
		*	@param height ratio height of window
		*	@param posX ratio position on X axis
		*	@param posY ratio position on Y axis
		*	@return void
		*/
	void StartWindow(const std::string& wName, bool background, float width, float height, float posX, float posY);

		/**
		*	@brief ends rendering a window
		*	@return void
		*/
	void EndWindow();

		/**
		*	@brief create text on gui
		*	@param label name of the text
		*   @param aligment type of the text
		*	@return void
		*/
	void Text(const std::string label, float alignment);
		/**
		*	@brief create button on gui
		*	@param label name of the button
		*   @param aligment type of the button
		*   @param width type of the button
		*   @param height type of the button
		*	@return bool
		*/
	bool Button(const std::string label, float alignment, float width, float height);
		/**
		*	@brief create Image on gui
		*	@param texture file location
		*   @param width type of the image
		*   @param height type of the image
		*   @param alignX type of the image
		*   @param alignY type of the image
		*	@return void
		*/
	void Image(const std::string texture, float width, float height, float alignX, float alignY);
		/**
		*	@brief create ImageButton on gui
		*	@param texture file location
		*   @param width type of the ImageButton
		*   @param height type of the ImageButton
		*   @param alignX type of the ImageButton
		*   @param alignY type of the ImageButton
		*	@return bool
		*/
	bool ImageButton(const std::string texture, float width, float height, float alignX, float alignY);
		/**
		*	@brief create Tab on gui
		*	@param size of the tab
		*	@return bool
		*/
	void Tab(float size);
		/**
		*	@brief set cursor pos x on the image
		*	@param pos in 2D space
		*	@return void
		*/
	void SetCursorPosX(float pos);
		/**
		*	@brief set cursor pos y on the image
		*	@param pos in 2D space
		*	@return void
		*/
	void SetCursorPosY(float pos);

		/**
		*	@brief get the amount of pixels accross the window is
		*	@return the windows width in pixels
		*/
	int GetWindowWidth();

		/**
		*	@brief get the amount of pixels up the window is
		*	@return the windows height in pixels
		*/
	int GetWindowHeight();
private:

		//io pointer for imgui
	ImGuiIO* io;

		/**
		*	@brief set width aligment
		*	@param width in pixels
		*	@param the alignment type
		*	@return void
		*/
	void AlignForWidth(float width, float alignment);
		
		///imgui settings for windos
	ImGuiWindowFlags flags;
		///window being drawn to
	Window* _window = nullptr;
};