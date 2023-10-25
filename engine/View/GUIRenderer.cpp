#include <GUIRenderer.h>
#include <cassert>

bool GUIRenderer::isDrawing = false;

GUIRenderer::GUIRenderer(Window* window) : _window(window) {
	//set flags
	flags = 
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoTitleBar;
	Init(window);
}

GUIRenderer::~GUIRenderer() {
	Shutdown();
}

void GUIRenderer::Shutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void GUIRenderer::Init(Window* nwindow) {

	_window = nwindow;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& nio = ImGui::GetIO(); (void)nio;
	io = &ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();

	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 1.0f);

	ImGui_ImplGlfw_InitForOpenGL(_window->window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	
	//style
	ImGuiStyle* style = &ImGui::GetStyle();
	ImVec4* colors = style->Colors;

	colors[ImGuiCol_WindowBg] = { 0,0,0,1 };
	colors[ImGuiCol_Text] = { 0.8,0.8,0.8,1 };

	colors[ImGuiCol_Button] = { 0.1f, 0.1f, 0.1f, 1.0f };
	colors[ImGuiCol_ButtonHovered] = { 0.2f, 0.2f, 0.2f, 1.0f };
	colors[ImGuiCol_ButtonActive] = { 0.3f, 0.3f, 0.3f, 1.0f };
}

void GUIRenderer::SetFont(std::string path)
{
	io->Fonts->AddFontFromFileTTF(path.c_str(), 20.0f);
}

void GUIRenderer::SetGUIStyle(GUIStyle& guiStyle)
{
	//style
	ImGuiStyle* style = &ImGui::GetStyle();
	ImVec4* colors = style->Colors;
	GUIColor color_Text_Disabled = { guiStyle.color_Text.x / 2, guiStyle.color_Text.y / 2, guiStyle.color_Text.z / 2,1.0 };


	colors[ImGuiCol_Text]					= guiStyle.color_Text;
	colors[ImGuiCol_TextDisabled]			= color_Text_Disabled;
	colors[ImGuiCol_WindowBg]				= guiStyle.color_Background;
	colors[ImGuiCol_ChildBg]				= guiStyle.color_Background;
	colors[ImGuiCol_PopupBg]				= guiStyle.color_Background;
	colors[ImGuiCol_Border]					= guiStyle.color_Secondary;
	colors[ImGuiCol_BorderShadow]			= guiStyle.color_Background;
	colors[ImGuiCol_FrameBg]				= guiStyle.color_Secondary_Light;
	colors[ImGuiCol_FrameBgHovered]			= guiStyle.color_Primary;
	colors[ImGuiCol_FrameBgActive]			= guiStyle.color_Primary;
	colors[ImGuiCol_TitleBg]				= guiStyle.color_Primary;
	colors[ImGuiCol_TitleBgActive]			= guiStyle.color_Primary;
	colors[ImGuiCol_TitleBgCollapsed]		= guiStyle.color_Secondary;
	colors[ImGuiCol_MenuBarBg]				= guiStyle.color_Secondary;
	colors[ImGuiCol_ScrollbarBg]			= ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
	colors[ImGuiCol_ScrollbarGrab]			= ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered]	= ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive]	= ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	colors[ImGuiCol_CheckMark]				= ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_SliderGrab]				= ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
	colors[ImGuiCol_SliderGrabActive]		= ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_Button]					= ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
	colors[ImGuiCol_ButtonHovered]			= ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_ButtonActive]			= ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
	colors[ImGuiCol_Header]					= ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
	colors[ImGuiCol_HeaderHovered]			= ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
	colors[ImGuiCol_HeaderActive]			= ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_Separator]				= colors[ImGuiCol_Border];
	colors[ImGuiCol_SeparatorHovered]		= ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
	colors[ImGuiCol_SeparatorActive]		= ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
	colors[ImGuiCol_ResizeGrip]				= ImVec4(0.26f, 0.59f, 0.98f, 0.20f);
	colors[ImGuiCol_ResizeGripHovered]		= ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	colors[ImGuiCol_ResizeGripActive]		= ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	colors[ImGuiCol_Tab]					= ImLerp(colors[ImGuiCol_Header], colors[ImGuiCol_TitleBgActive], 0.80f);
	colors[ImGuiCol_TabHovered]				= colors[ImGuiCol_HeaderHovered];
	colors[ImGuiCol_TabActive]				= ImLerp(colors[ImGuiCol_HeaderActive], colors[ImGuiCol_TitleBgActive], 0.60f);
	colors[ImGuiCol_TabUnfocused]			= ImLerp(colors[ImGuiCol_Tab], colors[ImGuiCol_TitleBg], 0.80f);
	colors[ImGuiCol_TabUnfocusedActive]		= ImLerp(colors[ImGuiCol_TabActive], colors[ImGuiCol_TitleBg], 0.40f);
	colors[ImGuiCol_PlotLines]				= ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered]		= ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram]			= ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered]	= ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TableHeaderBg]			= ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
	colors[ImGuiCol_TableBorderStrong]		= ImVec4(0.31f, 0.31f, 0.35f, 1.00f);   // Prefer using Alpha=1.0 here
	colors[ImGuiCol_TableBorderLight]		= ImVec4(0.23f, 0.23f, 0.25f, 1.00f);   // Prefer using Alpha=1.0 here
	colors[ImGuiCol_TableRowBg]				= ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_TableRowBgAlt]			= ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
	colors[ImGuiCol_TextSelectedBg]			= ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	colors[ImGuiCol_DragDropTarget]			= ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight]			= ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight]	= ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg]		= ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg]		= ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

}

void GUIRenderer::StartGUI()
{
	if (!_window) {
		Init(Window::GetActiveWindow());
	}
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();
	//ImGuizmo::SetImGuiContext(ImGui::GetCurrentContext());
}

void GUIRenderer::EndGUI()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUIRenderer::StartWindow(const std::string& wName, bool background, float width, float height, float posX, float posY)
{

	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	
	ImGui::SetNextWindowPos({(float)(viewport->WorkSize.x * posX),(float)(viewport->WorkSize.y * posY)});
	ImGui::SetNextWindowSize({ viewport->WorkSize.x * width,viewport->WorkSize.y * height });
	
	if (background) {
		ImGui::Begin(wName.c_str(), nullptr, flags);
	}
	else {
		ImGui::Begin(wName.c_str(), nullptr, flags | ImGuiWindowFlags_NoBackground);
	}
}

void GUIRenderer::EndWindow()
{
	ImGui::End();
}

void GUIRenderer::Text(const std::string label, float alignment)
{
	ImGuiStyle& style = ImGui::GetStyle();

	float size = ImGui::CalcTextSize(label.c_str()).x + style.FramePadding.x * 2.0f;
	float avail = ImGui::GetContentRegionAvail().x;

	float off = (avail - size) * alignment;
	if (off > 0.0f)
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
	//ImGui::PushItemWidth(5);
	ImGui::Text(label.c_str());
}

bool GUIRenderer::Button(const std::string label, float alignment, float width, float height)
{
	ImGuiStyle& style = ImGui::GetStyle();
	float total_width = 0.0f;
	total_width += style.ItemSpacing.x;

	total_width += width;
	AlignForWidth(total_width, alignment);

	return ImGui::Button(label.c_str(), ImVec2(total_width, height));
}

void GUIRenderer::Image(const std::string texture, float width, float height, float alignX, float alignY)
{
	int wwidth = _window->GetWidth();
	int wheight = _window->GetHeight();
	float size = ((wwidth < wheight) ? wwidth : wheight) / 2;
	float availX = ImGui::GetContentRegionAvail().x;
	float availY = ImGui::GetContentRegionAvail().y;
	float offX = (availX - width) * alignX;
	float offY = (availY - height) * alignY;

	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offX);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + offY);

	Texture* img = ResourceManager::Get().GetTexture(texture);
	ImGui::Image((void*)(intptr_t)img->ID, ImVec2(width, height));
}


bool GUIRenderer::ImageButton(const std::string texture, float width, float height, float alignX, float alignY)
{
	int wwidth = _window->GetWidth();
	int wheight = _window->GetHeight();
	float size = ((wwidth < wheight) ? wwidth : wheight) / 2;
	float availX = ImGui::GetContentRegionAvail().x;
	float availY = ImGui::GetContentRegionAvail().y;
	float offX = (availX - width) * alignX;
	float offY = (availY - height) * alignY;

	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offX);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + offY);

	Texture* img = ResourceManager::Get().GetTexture(texture);
	if (ImGui::ImageButton((void*)(intptr_t)img->ID, ImVec2(width, height))) {
		return true;
	}
	return false;
}

void GUIRenderer::Tab(float size)
{
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + size);
}

void GUIRenderer::SetCursorPosX(float pos)
{
	ImGui::SetCursorPosX(pos);
}

void GUIRenderer::SetCursorPosY(float pos)
{
	ImGui::SetCursorPosY(pos);
}

void GUIRenderer::AlignForWidth(float width, float alignment)
{
	ImGuiStyle& style = ImGui::GetStyle();
	float avail = ImGui::GetContentRegionAvail().x;
	float off = (avail - width) * alignment;
	if (off > 0.0f)
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
}

int GUIRenderer::GetWindowWidth()
{
	return _window->GetWidth();
}

int GUIRenderer::GetWindowHeight()
{
	return _window->GetHeight();
}