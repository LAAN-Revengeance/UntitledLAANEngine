#include <GUIRenderer.h>

GUIRenderer::GUIRenderer() : window(nullptr) {
	//set flags
	flags = ImGuiWindowFlags_NoDecoration |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoTitleBar | 
		ImGuiWindowFlags_MenuBar;
}

GUIRenderer::~GUIRenderer() {
	Shutdown();
}

GUIRenderer& GUIRenderer::Get()
{
	static GUIRenderer g_instance;
	return g_instance;
}

void GUIRenderer::Shutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void GUIRenderer::Init(GLFWwindow* nwindow) {

	window = nwindow;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& nio = ImGui::GetIO(); (void)nio;
	io = &ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();

	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 1.0f);

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	//style
	ImGuiStyle* style = &ImGui::GetStyle();
	ImVec4* colors = style->Colors;

	colors[ImGuiCol_WindowBg] = { 0,0,0,1 };
	colors[ImGuiCol_Text] = {0.8,0.8,0.8,1};

	colors[ImGuiCol_Button] =			{0.1f, 0.1f, 0.1f, 1.0f};
	colors[ImGuiCol_ButtonHovered] =	{0.2f, 0.2f, 0.2f, 1.0f};
	colors[ImGuiCol_ButtonActive] =		{0.3f, 0.3f, 0.3f, 1.0f};
}

void GUIRenderer::SetFont(std::string path)
{
	io->Fonts->AddFontFromFileTTF(path.c_str(), 20.0f);
}

void GUIRenderer::StartGUI()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
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
	int wwidth, wheight;
	glfwGetWindowSize(window, &wwidth, &wheight);
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
	int wwidth, wheight;
	glfwGetWindowSize(window, &wwidth, &wheight);
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
