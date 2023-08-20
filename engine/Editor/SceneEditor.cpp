#include "SceneEditor.h"

void SceneEditor::Run()
{
	//main loop
	while (!glfwWindowShouldClose(window))
	{
		// timer
		double currentFrameTime = glfwGetTime();
		deltaTime = currentFrameTime - previousFrameTime;
		previousFrameTime = currentFrameTime;

		Update(deltaTime);

		renderer.Draw(camera, *scene, deltaTime);
		Draw(deltaTime);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//cleanup
	glfwDestroyWindow(window);
	glfwTerminate();

}

SceneEditor::SceneEditor()
{
	camera.farPlane = 10000.0f;
	//init window and glfw.
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	int wWidth = mode->width;
	int wHeight = mode->height;

	window = glfwCreateWindow(wWidth, wHeight, "Engine", NULL, NULL);

	if (!window)
	{
		std::cout << "ERROR Could not initalize window." << std::endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(window);

	//scene camera settings
	scene = new Scene;
	scene->camera.aspectRatio = (float)wWidth / (float)wHeight;

	InputManager::Get().Init(window);
	GUIRenderer::Get().Init(window);
	renderer.Init(window);
	//aiManager.Init(scene);

	//callbacks
	glfwSetFramebufferSizeCallback(window, ResizeCallback);

	//expose to lua
	//ExposeToLua();
	//luaManager.RunInitMethod();

	//set light uniforms
	auto it = ResourceManager::Get().ShaderBegin();
	auto end = ResourceManager::Get().ShaderEnd();
	for (it; it != end; it++) {
		Renderer::SetLightUniforms(scene->lights, *it->second);
	}

	if (!scene)
		scene = new Scene;
}

SceneEditor::~SceneEditor()
{
}

SceneEditor& SceneEditor::Get()
{
	static SceneEditor s_instance;
	return s_instance;
}

void SceneEditor::Draw(double deltaTime)
{
	r.StartGUI();

	DrawInspector();
	DrawHeighrarchy();
	DrawMenu();
	ImGui::ShowDemoWindow();
	r.EndGUI();
}

void SceneEditor::Update(double deltaTime)
{
	CameraControl(deltaTime);
}

void SceneEditor::LoadSceneFromFile(const char* path)
{
	scene = &SceneLoader::LoadScene(path);
}

void SceneEditor::UseScene(Scene* nscene)
{
	if (nscene) {
		scene = nscene;
	}
}

void SceneEditor::ResizeCallback(GLFWwindow* window, int width, int height)
{
	SceneEditor& editor = SceneEditor::Get();
	editor.camera.aspectRatio = (float)width / (float)height;
	glViewport(0, 0, width, height);
	editor.renderer.Resize(width, height);
	editor.renderer.Draw(editor.camera, *editor.scene, editor.deltaTime);
}

void SceneEditor::DrawHeighrarchy()
{
	float align = 0.0;
	static ImGuiTreeNodeFlags baseFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

	r.StartWindow("Scene Objects", true, 0.2, 0.9, 0.0, 0.1);

	int i = 0;
	static int selectedNode = -1;


	ImGui::CollapsingHeader("Scene Objects", ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Leaf);
	ImGui::SeparatorText("GameObjects");
	for (auto& pair : scene->gameObjects)
	{
		ImGuiTreeNodeFlags tmpFlags = baseFlags;
		if (selectedNode == i) {
			tmpFlags |= ImGuiTreeNodeFlags_Selected;
		}

		bool nodeOpen = ImGui::TreeNodeEx(pair.second->name.c_str(), tmpFlags);
		if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
			inspectedObject = pair.second;
			selectedNode = i;
		}

		if (nodeOpen) {
			ImGui::Text("Children go here");
			ImGui::TreePop();
		}
		i++;
	}

	ImGui::SeparatorText("Skybox");
	if (!scene->skybox) {


		/*
		cm["right"] 
		cm["left"] =
		cm["top"] = 
		cm["bottom"]
		cm["front"] 
		cm["back"] =
		*/
		static char cmSides[6][128];

		ImGui::InputTextWithHint("##cmright",	"Face right",	cmSides[0], IM_ARRAYSIZE(cmSides[0]));
		ImGui::InputTextWithHint("##cmleft",	"Face left",	cmSides[1], IM_ARRAYSIZE(cmSides[1]));
		ImGui::InputTextWithHint("##cmtop",		"Face top",		cmSides[2], IM_ARRAYSIZE(cmSides[2]));
		ImGui::InputTextWithHint("##cmbottom",	"Face bottom",	cmSides[3], IM_ARRAYSIZE(cmSides[3]));
		ImGui::InputTextWithHint("##cmfront",	"Face front",	cmSides[4], IM_ARRAYSIZE(cmSides[4]));
		ImGui::InputTextWithHint("##cmback",	"Face back",	cmSides[5], IM_ARRAYSIZE(cmSides[5]));

		if (ImGui::Button("SetSkybox")) {
			ResourceManager& res = ResourceManager::Get();
			res.LoadCubemap(cmSides[0], cmSides[0], cmSides[1], cmSides[2], cmSides[3], cmSides[4], cmSides[5]);
			scene->SetSkybox(res.GetCubeMap(cmSides[0]));
		}
	}
	else {
		if (ImGui::Button("Remove Skybox")) {
			scene->skybox = nullptr;
		}
	}


	ImGui::SeparatorText("Lights");
	ResourceManager& res = ResourceManager::Get();
	//Ambient Light
	if (ImGui::ColorEdit3("Ambient Light", (float*)&scene->lights.ambient)) {
		for (auto& shader : res.shaders) {
			Renderer::Get().SetLightUniforms(scene->lights, *shader.second);
		}
	}

	//Directional Lights
	int deleteIndex = -1;
	for (int i = 0; i < scene->lights.direction.size(); ++i)
	{
		std::string dName = std::string("Dir Light##" + std::to_string(i));
		if(ImGui::TreeNodeEx(dName.c_str())) {
			if (ImGui::Button(std::string("Delete Light##" + std::to_string(i)).c_str())) {
				deleteIndex = i;
			}

			bool dColor = ImGui::ColorEdit3(("Diffuse##"+dName).c_str(), (float*)&scene->lights.direction[i].diffuse);
			bool dSpec = ImGui::ColorEdit3(("Specular##"+dName).c_str(), (float*)&scene->lights.direction[i].specular);
			bool dPos = ImGui::InputFloat3 (("Direction##"+dName).c_str(), (float*)&scene->lights.direction[i].direction);
			if (dColor || dPos || dSpec) {

				for (auto& shader : res.shaders) {
					Renderer::Get().SetLightUniforms(scene->lights, *shader.second);
				}
			};
			ImGui::TreePop();
		}
	}
	if (deleteIndex >= 0) {
		scene->lights.direction.erase(scene->lights.direction.begin() + deleteIndex);
		for (auto& shader : res.shaders) {
			Renderer::Get().SetLightUniforms(scene->lights, *shader.second);
		}
	}
	
	//point Lights
	deleteIndex = -1;
	for (int i = 0; i < scene->lights.point.size(); ++i)
	{
		std::string dName = std::string("Point Light##" + std::to_string(i));
		if (ImGui::TreeNodeEx(dName.c_str())) {
			if (ImGui::Button(std::string("Delete Light##" + std::to_string(i)).c_str())) {
				deleteIndex = i;
			}
		
			bool dColor = ImGui::ColorEdit3(("Diffuse##" + dName).c_str(), (float*)&scene->lights.point[i].diffuse);
			bool dSpec = ImGui::ColorEdit3(("Specular##" + dName).c_str(), (float*)&scene->lights.point[i].specular);
			bool dPos = ImGui::InputFloat3(("Position##" + dName).c_str(), (float*)&scene->lights.point[i].position);
			if (dColor || dSpec || dPos) {

				for (auto& shader : res.shaders) {
					Renderer::Get().SetLightUniforms(scene->lights, *shader.second);
				}
			};
			ImGui::TreePop();
		}
	}
	if (deleteIndex >= 0) {
		scene->lights.point.erase(scene->lights.point.begin() + deleteIndex);
		for (auto& shader : res.shaders) {
			Renderer::Get().SetLightUniforms(scene->lights, *shader.second);
		}
	}

	ImGui::End();

}

void SceneEditor::DrawInspector()
{
	
	r.StartWindow("Inspector",true,0.2,0.9,0.8,0.1);

	static ImGuiTreeNodeFlags baseFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Leaf;

	if (inspectedObject) {
		ImGui::CollapsingHeader(inspectedObject->name.c_str(), baseFlags);

		//TRANSFORM SETTINGS
		ImGui::SeparatorText("Transform");
		float tmpPosX = inspectedObject->position.x;
		float tmpPosY = inspectedObject->position.y;
		float tmpPosZ = inspectedObject->position.z;
		ImGui::Text("Position:");
		ImGui::DragFloat("x position", &tmpPosX, 0.1f);
		ImGui::DragFloat("y position", &tmpPosY, 0.1f);
		ImGui::DragFloat("z position", &tmpPosZ, 0.1f);

		if (tmpPosX != inspectedObject->position.x || tmpPosY != inspectedObject->position.y || tmpPosZ != inspectedObject->position.z) {
			inspectedObject->SetPosition({ tmpPosX, tmpPosY, tmpPosZ });
		}

		
		float tmpRotX = inspectedObject->rotation.x;
		float tmpRotY = inspectedObject->rotation.y;
		float tmpRotZ = inspectedObject->rotation.z;
		ImGui::Text("Rotation:");
		ImGui::DragFloat("x rotation", &tmpRotX, 0.1f);
		ImGui::DragFloat("y rotation", &tmpRotY, 0.1f);
		ImGui::DragFloat("z rotation", &tmpRotZ, 0.1f);

		if (tmpRotX != inspectedObject->rotation.x || tmpRotY != inspectedObject->rotation.y || tmpRotZ != inspectedObject->rotation.z) {
			inspectedObject->SetRotation({ tmpRotX, tmpRotY, tmpRotZ });
		}

		float tmpSclX = inspectedObject->scale.x;
		float tmpSclY = inspectedObject->scale.y;
		float tmpSclZ = inspectedObject->scale.z;
		ImGui::Text("Scale:");
		ImGui::DragFloat("x scale", &tmpSclX, 0.01f);
		ImGui::DragFloat("y scale", &tmpSclY, 0.01f);
		ImGui::DragFloat("z scale", &tmpSclZ, 0.01f);

		if (tmpSclX != inspectedObject->scale.x || tmpSclY != inspectedObject->scale.y || tmpSclZ != inspectedObject->scale.z) {
			inspectedObject->SetScale({ tmpSclX, tmpSclY, tmpSclZ });
		}

		//PHYSICS SETTINGS
		ImGui::SeparatorText("Physics");
	}
	else {
		ImGui::CollapsingHeader("No Object Selected", baseFlags);
	}

	ImGui::End();
}

void SceneEditor::DrawMenu()
{
	static bool showChangeWindow = false;
	static bool showDebug = false;
	static bool showOpenFile = false;
	static bool showSaveFile = false;
	r.StartWindow("Menu", true, 1.0, 0.1, 0.0, 0.0);
	if (ImGui::BeginMenuBar()) {
		
		if (ImGui::BeginMenu("File")) {
			
			if (ImGui::MenuItem("New")) { scene = new Scene; }
			if (ImGui::MenuItem("Save",NULL, &showSaveFile)) { }
			if (ImGui::MenuItem("Open",NULL,&showOpenFile)) { }
			if (ImGui::MenuItem("Exit")) { }
			
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit")) {

			ImGui::MenuItem("Window Title",NULL, &showChangeWindow);
			ImGui::MenuItem("Window Icon");

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help")) {

			ImGui::MenuItem("About");
			ImGui::MenuItem("Version");
			ImGui::MenuItem("Debug",NULL,&showDebug);

			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	float buttonWidth = 80;
	ImGui::SetCursorPosX((viewport->WorkSize.x/2) - (((buttonWidth + ImGui::GetStyle().ItemSpacing.x) * 3)/2));
	
	ImGui::Button("Play", { buttonWidth,20 });
	ImGui::SameLine();
	ImGui::Button("Pause", { buttonWidth,20 }); 
	ImGui::SameLine();
	if (ImGui::Button("FreeCam", { buttonWidth,20 })) { InputManager::Get().SetMouseLock(false); }

	r.EndWindow();

	DrawWindowSettings(&showChangeWindow);
	DrawDebug(&showDebug);
	DrawOpenFile(&showOpenFile);
	DrawSaveFile(&showSaveFile);
}

void SceneEditor::DrawWindowSettings(bool* showChangeWindow)
{
	if(!(*showChangeWindow))
		return

	ImGui::SetNextWindowSize({ 330,70 });
	ImGui::Begin("Window Settings", showChangeWindow);
	static char str0[128] = "";
	if (ImGui::InputTextWithHint("Window Title", "Window Name", str0, IM_ARRAYSIZE(str0))) {
		//TODO: SET window title in json file.
	}

	ImGui::End();
}

void SceneEditor::DrawDebug(bool* showDebug)
{
	if (!(*showDebug))
		return;

	ImGui::SetNextWindowSize({ 300,150 });
	ImGui::Begin("Debug", showDebug);
	
	double fps = Renderer::Get().GetFPS();
	ImGui::Text("Current FPS:");
	ImGui::SameLine();
	ImGui::Text(std::to_string(fps).c_str());

	static float values[1080] = {};
	static int values_offset = 0;
	
	static float phase = 0.0f;
	values[values_offset] = fps;
	values_offset = (values_offset + 1) % IM_ARRAYSIZE(values);
	phase += 0.10f * values_offset;
	{
		float average = 0.0f;
		for (int n = 0; n < IM_ARRAYSIZE(values); n++)
			average += values[n];
		average /= (float)IM_ARRAYSIZE(values);
		char overlay[32];
		sprintf(overlay, "avg %f", average);
		ImGui::PlotLines("##FPS", values, IM_ARRAYSIZE(values), values_offset, overlay, 0.0f, 200.0f, ImVec2(0, 80.0f));
	}


	ImGui::End();

}

void SceneEditor::DrawOpenFile(bool* showOpenFile)
{
	if (!(*showOpenFile))
		return;

	ImGui::SetNextWindowSize({ 300,150 });
	ImGui::Begin("Open Scene", showOpenFile);
	static char filePath[256] = "";
	if (ImGui::InputTextWithHint("##openfilePath", "FilePath", filePath, IM_ARRAYSIZE(filePath))) {
		
	}
	if (ImGui::Button("Select##openFile")) {
		scene = &SceneLoader::LoadScene(filePath);
	}
	
	ImGui::End();
}

void SceneEditor::DrawSaveFile(bool* showSaveFile)
{
	if (!(*showSaveFile))
		return;

	ImGui::SetNextWindowSize({ 300,150 });
	ImGui::Begin("Save Scene", showSaveFile);
	static char filePath[256] = "";
	if (ImGui::InputTextWithHint("##savefilePath", "FilePath", filePath, IM_ARRAYSIZE(filePath))) {

	}
	if (ImGui::Button("Save##saveFile")) {
		if(scene)
			SceneLoader::SaveScene(scene, filePath);
	}

	ImGui::End();
}

void SceneEditor::CameraControl(double deltaTime)
{
	InputManager& input = InputManager::Get();
	bool camlock = input.GetMouseLock();
	static bool toggleCamPress = false;
	if (input.GetKeyPressedDown(GLFW_KEY_ESCAPE) && !toggleCamPress) {
		toggleCamPress = true;
		camlock = !input.GetMouseLock();
		input.SetMouseLock(camlock);
	}
	else if (input.GetKeyPressedDown(GLFW_KEY_ESCAPE)) {
		toggleCamPress = true;
	}
	else {
		toggleCamPress = false;
	}
	if (camlock)
		return;



	float baseSpeed = 5;
	float camSpeed = baseSpeed;
	if (input.GetKeyPressed(GLFW_KEY_LEFT_SHIFT)) {
		camSpeed *= 10;
	}
	if (input.GetKeyPressed(GLFW_KEY_W)) {
		camera.position += glm::normalize(glm::cross({ 0,1,0 }, camera.right)) * camSpeed;
	}
	if (input.GetKeyPressed(GLFW_KEY_S)) {
		camera.position -= glm::normalize(glm::cross({ 0,1,0 }, camera.right)) * camSpeed;
	}
	if (input.GetKeyPressed(GLFW_KEY_A)) {
		camera.position -= camera.right * camSpeed;
	}
	if (input.GetKeyPressed(GLFW_KEY_D)) {
		camera.position += camera.right * camSpeed;
	}

	if (input.GetKeyPressed(GLFW_KEY_SPACE)) {
		camera.position += glm::vec3(0,1,0) * camSpeed;
	}
	if (input.GetKeyPressed(GLFW_KEY_LEFT_CONTROL)) {
		camera.position -= glm::vec3(0, 1, 0) * camSpeed;
	}




	//mouse controls
	float xPos = input.GetMouseX();
	float yPos = input.GetMouseY();

	float xoffset = (xPos - lastX);
	float yoffset = (lastY - yPos);

	lastX = xPos;
	lastY = yPos;

	xoffset = xoffset * -mouseSensitivity * deltaTime;
	yoffset = yoffset * -mouseSensitivity * deltaTime;

	camera.Yaw = camera.Yaw - xoffset;
	camera.Pitch = camera.Pitch - yoffset;

	camera.UpdateCameraVectors();

}
