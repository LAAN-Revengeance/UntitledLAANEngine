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
	Draw3DWidget();
	DrawInspector();
	DrawHeighrarchy();
	DrawMenu();
	DrawResources();
	

	//ImGui::ShowDemoWindow();
	r.EndGUI();
}

void SceneEditor::Update(double deltaTime)
{
	CameraControl(deltaTime);
}

void SceneEditor::LoadSceneFromFile(const char* path)
{
	inspectedObject = nullptr;
	lastObject = nullptr;
	scene = &SceneLoader::LoadScene(path);
		
	for (auto& shader : ResourceManager::Get().shaders) {
		Renderer::Get().SetLightUniforms(scene->lights, *shader.second);
	}
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

	r.StartWindow("Scene Objects", true, 0.2, 0.94, 0.0, 0.06);

	ResourceManager& res = ResourceManager::Get();
	int i = 0;
	static int selectedNode = -1;

	ImGui::CollapsingHeader("Scene Objects", ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Leaf);
	ImGui::SeparatorText("GameObjects");

	if (ImGui::Button("Add Object")) {
		
		std::string name = "new object";
		std::string nName = name;
		int nSuffix = 1;
		while (res.objects.find(nName) != res.objects.end())
		{ 
			nName = name;
			nName.append(std::to_string(nSuffix));
			++nSuffix;
		}
		GameObject& go = res.CreateGameObject(nName,"","");
		scene->AddObject(go);
	}

	for (auto& pair : scene->gameObjects)
	{
		ImGuiTreeNodeFlags tmpFlags = baseFlags;
		if (selectedNode == i) {
			tmpFlags |= ImGuiTreeNodeFlags_Selected;
		}

		bool nodeOpen = ImGui::TreeNodeEx(pair.second->name.c_str(), tmpFlags);
		if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
			inspectedObject = pair.second;
			
			camera.position = { inspectedObject->position.x,inspectedObject->position.y,inspectedObject->position.z - 10};
			camera.up = { 0,1,0 };
			camera.right = { 0.0, 0.0, -1.0 };
			camera.LookAt(inspectedObject->position);

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

		static char cmSides[6][128];

		ImGui::InputTextWithHint("##cmright",	"Face right",	cmSides[0], IM_ARRAYSIZE(cmSides[0]));
		ImGui::InputTextWithHint("##cmleft",	"Face left",	cmSides[1], IM_ARRAYSIZE(cmSides[1]));
		ImGui::InputTextWithHint("##cmtop",		"Face top",		cmSides[2], IM_ARRAYSIZE(cmSides[2]));
		ImGui::InputTextWithHint("##cmbottom",	"Face bottom",	cmSides[3], IM_ARRAYSIZE(cmSides[3]));
		ImGui::InputTextWithHint("##cmfront",	"Face front",	cmSides[4], IM_ARRAYSIZE(cmSides[4]));
		ImGui::InputTextWithHint("##cmback",	"Face back",	cmSides[5], IM_ARRAYSIZE(cmSides[5]));

		if (ImGui::Button("SetSkybox")) {
			
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
	//Ambient Light
	if (ImGui::ColorEdit3("Ambient Light", (float*)&scene->lights.ambient)) {
		for (auto& shader : res.shaders) {
			Renderer::Get().SetLightUniforms(scene->lights, *shader.second);
		}
	}

	//Directional Lights
	int deleteIndex = -1;

	if (ImGui::Button("Add Direction Light")) {
		scene->lights.AddDirectionLight({ 0,1,0 }, { 1,1,1 }, { 1,1,1 });

		for (auto& shader : ResourceManager::Get().shaders) {
			Renderer::Get().SetLightUniforms(scene->lights, *shader.second);
		}
	}

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

	if (ImGui::Button("Add Point Light")) {
		scene->lights.AddPointLight({ 0,0,0 }, { 1,1,1 }, {1,1,1}, 1.0, 0.007, 0.0002);
		for (auto& shader : ResourceManager::Get().shaders) {
			Renderer::Get().SetLightUniforms(scene->lights, *shader.second);
		}
	}

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
	r.StartWindow("Inspector",true,0.2,0.94,0.8,0.06);

	static ImGuiTreeNodeFlags baseFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Leaf;
	ResourceManager& res = ResourceManager::Get();
	

	if (inspectedObject) {
		
		if (!lastObject)
			lastObject = inspectedObject;

		static bool changeObject = false;
		if (strcmp(inspectedObject->name.c_str(), lastObject->name.c_str()) != 0) {
			lastObject = inspectedObject;
			changeObject = true;
		}

		ImGui::CollapsingHeader(inspectedObject->name.c_str(), baseFlags);

		//NAME SETTINGS
		static char cmObjName[128];
		ImGui::InputTextWithHint("##changeObjectName", "Object Name", cmObjName, IM_ARRAYSIZE(cmObjName));
		ImGui::SameLine();
		if (ImGui::Button("Change Name") && strlen(cmObjName) > 0) {
			inspectedObject->name = cmObjName;
		}

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

		//Rendering SETTINGS
		ImGui::SeparatorText("Rendering");
		static std::string selectedShader;
		static std::string selectedMesh;

		if (changeObject) {
			selectedShader = "";
			if (inspectedObject->shader)
				selectedShader = inspectedObject->shader->name;
			selectedMesh = "";
			if (inspectedObject->model_data)
				selectedMesh = inspectedObject->model_data->name;
		}

		ImGui::Text("Shader:");
		if (ImGui::BeginCombo("##objectShader", selectedShader.c_str()))
		{
			for (auto it : res.shaders)
			{
				if (ImGui::Selectable(it.first.c_str())) {
					selectedShader = it.first;
					inspectedObject->shader = it.second;
				}
			}
			ImGui::EndCombo();
		}
		ImGui::Text("Mesh:");
		
		if (ImGui::BeginCombo("##modelMesh", selectedMesh.c_str()))
		{
			for (auto it : res.models)
			{
				if (ImGui::Selectable(it.first.c_str())) {
					selectedMesh = it.first;
					inspectedObject->model_data = it.second;
				}
			}
			ImGui::EndCombo();
		}


		//PHYSICS SETTINGS
		ImGui::SeparatorText("Physics");


		changeObject = false;
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
	//r.StartWindow("Menu", true, 1.0, 0.06, 0.0, 0.0);
	float width = 1.0; float height = 0.06; float posY = 0.0; float posX = 0.0;
	const ImGuiViewport* viewport = ImGui::GetMainViewport();

	ImGui::SetNextWindowPos({ (float)(viewport->WorkSize.x * posX),(float)(viewport->WorkSize.y * posY) });
	ImGui::SetNextWindowSize({ viewport->WorkSize.x * width,viewport->WorkSize.y * height });
	ImGui::Begin("Menu", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings |ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar);

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

void SceneEditor::DrawResources()
{
	float windowWidth = 0.6;
	float resourceWidth = 64;
	r.StartWindow("Resource Inspector", true, windowWidth, 0.3, 0.2, 0.7);
	ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
	ResourceManager& res = ResourceManager::Get();

	if (ImGui::BeginTabBar("resourceTabs", tab_bar_flags))
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGuiStyle& style = ImGui::GetStyle();
	
		//TEXTURES TAB
		if (ImGui::BeginTabItem("Textures"))
		{
			//Add textures
			static char texturePath[256] = "";
			static char textureName[256] = "";
			ImGui::InputTextWithHint("##texturefilePathInput", "File Path", texturePath, IM_ARRAYSIZE(texturePath));
			ImGui::InputTextWithHint("##textureNameInput", "Texture Name", textureName, IM_ARRAYSIZE(textureName));

			if (ImGui::Button("Add Texture"))
			{
				res.LoadTexture(textureName,texturePath);
				texturePath[0] = '\0';
				textureName[0] = '\0';
			}
	
			int colCount = (viewport->Size.x * windowWidth) / (resourceWidth + (style.ItemSpacing.x * 2));
			ImGui::Columns(colCount, "texCols", false);

			//show all textures loaded
			for (auto it : res.textures)
			{
				ImGui::Image((void*)(intptr_t)it.second->ID, ImVec2(resourceWidth, resourceWidth));
				ImGui::Text(it.first.c_str());
				ImGui::NextColumn();
			}
			ImGui::EndTabItem();
		}
		ImGui::Columns(1);

		//SHADERS TAB
		if (ImGui::BeginTabItem("Shaders"))
		{
			//Add shaders
			static char shaderPathVert[256] = "";
			static char shaderPathFrag[256] = "";
			static char shaderPathGeom[256] = "";
			static char shaderName[256] = "";

			ImGui::PushItemWidth((viewport->Size.x * windowWidth)/3);
			ImGui::InputTextWithHint("##shaderPathVert", "Vertex Shader Path", shaderPathVert, IM_ARRAYSIZE(shaderPathVert));	ImGui::SameLine();
			ImGui::InputTextWithHint("##shaderPathFrag", "Fragment Shader Path", shaderPathFrag, IM_ARRAYSIZE(shaderPathFrag));	ImGui::SameLine();
			ImGui::InputTextWithHint("##shaderPathGeom", "Geometry Shader Path", shaderPathGeom, IM_ARRAYSIZE(shaderPathGeom));
			ImGui::InputTextWithHint("##shaderName",	 "Shader Name", shaderName,	 IM_ARRAYSIZE(shaderName));
			ImGui::PopItemWidth();
			if (ImGui::Button("Add Shader"))
			{
				res.LoadShader(shaderName, shaderPathVert, shaderPathFrag, shaderPathGeom);
				shaderPathVert[0] = '\0';
				shaderPathFrag[0] = '\0';
				shaderPathGeom[0] = '\0';
				shaderName[0] = '\0';
			}

			int colCount = (viewport->Size.x * windowWidth) / (resourceWidth + (style.ItemSpacing.x * 2));
			ImGui::Columns(colCount, "texCols", false);

			Texture* shaderIcon = res.GetTexture("default");

			//show all shaders loaded
			for (auto it : res.shaders)
			{
				ImGui::Image((void*)(intptr_t)shaderIcon->ID, ImVec2(resourceWidth, resourceWidth));
				ImGui::Text(it.first.c_str());
				ImGui::NextColumn();
			}

			ImGui::EndTabItem();
		}
		ImGui::Columns(1);

		//MODEL TAB
		if (ImGui::BeginTabItem("3D Models"))
		{
			ImGui::Columns(3, "texCols", true);
			//textures
			ImGui::Text("Material:");
			ImGui::PushItemWidth((viewport->Size.x * windowWidth) / 4);
			static std::string difTexPreview;
			if (ImGui::BeginCombo("Diffuse##modelTexture", difTexPreview.c_str()))
			{
				if (ImGui::Selectable("--None--"))
					difTexPreview = "";
				for (auto  it : res.textures)
				{
					if (ImGui::Selectable(it.first.c_str())) {
						difTexPreview = it.first;
					}
				}
				ImGui::EndCombo();
			}
			static std::string specTexPreview;
			if (ImGui::BeginCombo("Specular##modelTexture", specTexPreview.c_str()))
			{
				if (ImGui::Selectable("--None--"))
					specTexPreview = "";
				for (auto it : res.textures)
				{
					if (ImGui::Selectable(it.first.c_str())) {
						specTexPreview = it.first;
					}
				}
				ImGui::EndCombo();
			}
			static std::string emisTexPreview;
			if (ImGui::BeginCombo("Emissive##modelTexture", emisTexPreview.c_str()))
			{
				if (ImGui::Selectable("--None--"))
					emisTexPreview = "";
				for (auto it : res.textures)
				{
					if (ImGui::Selectable(it.first.c_str())) {
						emisTexPreview = it.first;
					}
				}
				ImGui::EndCombo();
			}


			ImGui::Text("Model File and Name:");
			static char modelName[256] = "";
			static char modelPath[256] = "";
			ImGui::InputTextWithHint("##modelPath", "File path" , modelPath, IM_ARRAYSIZE(modelPath));
			ImGui::InputTextWithHint("##modelName", "Model Name", modelName, IM_ARRAYSIZE(modelName));

			if (ImGui::Button("Add Model")) {
				std::string::size_type idx = std::string(modelPath).rfind('.');
				std::string extension = std::string(modelPath).substr(idx + 1);

				if(extension.compare("obj") == 0){
					res.LoadModel(modelName, modelPath, difTexPreview, emisTexPreview, specTexPreview);
				}
				else if (extension.compare("md2") == 0) {
					res.LoadAnimatedModel(modelName, modelPath, difTexPreview, emisTexPreview, specTexPreview);
				}
			}

			Texture* shaderIcon = res.GetTexture("default");
			ImGui::NextColumn();

			ImGui::BeginChild("##modelsCol");

			static DrawItem* inspectedModel = nullptr;
			int colCount = ((viewport->Size.x * windowWidth) / 3) / (resourceWidth + (style.ItemSpacing.x * 3));
			ImGui::Columns(colCount, "modCols", false);
			for (auto it : res.models)
			{
				if (ImGui::ImageButton(std::string("##"+ it.first).c_str(), (void*)(intptr_t)shaderIcon->ID, ImVec2(resourceWidth, resourceWidth))) {
					inspectedModel = it.second;
				}
				ImGui::Text(it.first.c_str());
				ImGui::NextColumn();
			}
			ImGui::Columns(1);
			ImGui::EndChild();
			ImGui::NextColumn();

			ImGui::BeginChild("##inspectmodelsCol");

			if (inspectedModel) {
				ImGui::Text(inspectedModel->name.c_str());

				std::string diffName = "";
				if(inspectedModel->GetDiffuseTexture(0))
					diffName = inspectedModel->GetDiffuseTexture(0)->name;
				if (ImGui::BeginCombo("Diffuse##dinspectedModel", diffName.c_str()))
				{
					if (ImGui::Selectable("--None--"))
						inspectedModel->ResetDiffuseTexture();
					for (auto it : res.textures)
					{
						if (ImGui::Selectable(it.first.c_str())) {
							inspectedModel->SetDiffuseTexture(0,it.second);
						}
					}
					ImGui::EndCombo();
				}
				std::string specName = "";
				if (inspectedModel->GetSpecularTexture(0))
					specName = inspectedModel->GetSpecularTexture(0)->name;
				if (ImGui::BeginCombo("Specular##sinspectedModel", specName.c_str()))
				{
					if (ImGui::Selectable("--None--"))
						inspectedModel->ResetSpecularTexture();
					for (auto it : res.textures)
					{
						if (ImGui::Selectable(it.first.c_str())) {
							inspectedModel->SetSpecularTexture(0, it.second);
						}
					}
					ImGui::EndCombo();
				}
				std::string EmissName = "";
				if (inspectedModel->GetEmissionTexture(0))
					EmissName = inspectedModel->GetEmissionTexture(0)->name;
				if (ImGui::BeginCombo("Emission##einspectedModel", EmissName.c_str()))
				{
					if (ImGui::Selectable("--None--"))
						inspectedModel->ResetEmissionTexture();
					for (auto it : res.textures)
					{
						if (ImGui::Selectable(it.first.c_str())) {
							inspectedModel->SetEmissionTexture(0, it.second);
						}
					}
					ImGui::EndCombo();
				}

				if (dynamic_cast<md2_model_t*>(inspectedModel)) {
					md2_model_t* inspectedMD2 = dynamic_cast<md2_model_t*>(inspectedModel);
					
					ImGui::SeparatorText("Animaiton Data");
					static char nAnimName[256];
					static int startAnim = 0;
					static int endAnim = 0;
					static float animSpeed = 5;
					ImGui::InputTextWithHint("##md2Name", "Animation Name", nAnimName, IM_ARRAYSIZE(nAnimName));
					ImGui::Text("Start/End Frame:");
					int colCount = (viewport->Size.x * windowWidth) / (resourceWidth + (style.ItemSpacing.x * 2));
					float colWidth = ((viewport->Size.x * windowWidth) / 3);
					ImGui::PushItemWidth((colWidth / 4) - style.ItemSpacing.x);
					ImGui::DragInt("##Start Frame", &startAnim, 1, 0, inspectedMD2->header.num_frames); ImGui::SameLine();
					ImGui::DragInt("##End Frame", &endAnim, 1,0,inspectedMD2->header.num_frames); ImGui::SameLine();
					ImGui::DragFloat("##Frame Speed", &animSpeed, 0.1); ImGui::SameLine();

					if (ImGui::Button("Add")) {
						inspectedMD2->SetAnimation(nAnimName,startAnim,endAnim,animSpeed);
					}

					ImGui::PopItemWidth();

					
		
					ImGui::PushItemWidth((colWidth / 5) - style.ItemSpacing.x);
					ImGui::Text("Current Animations:");
					ImGui::Columns(5, "saved anims", false);

					ImGui::Text("name"); ImGui::NextColumn();
					ImGui::Text("start"); ImGui::NextColumn();
					ImGui::Text("end"); ImGui::NextColumn();
					ImGui::Text("speed"); ImGui::NextColumn();
					ImGui::NextColumn();

					std::string delteItem = "";
					for (auto& it : inspectedMD2->animations) {

						ImGui::Text(it.first.c_str());
						ImGui::NextColumn();
						ImGui::DragInt  (std::string("##strAnim"+it.first).c_str(), &it.second.start  , 1, 0, inspectedMD2->header.num_frames); ImGui::NextColumn();
						ImGui::DragInt  (std::string("##endAnim"+it.first).c_str(), &it.second.end    , 1, 0, inspectedMD2->header.num_frames); ImGui::NextColumn();
						ImGui::DragFloat(std::string("##spdAnim"+it.first).c_str(), &it.second.speed  , 1, 0, inspectedMD2->header.num_frames); ImGui::NextColumn();
						if (ImGui::Button((std::string("Delete##") + it.first).c_str())) {
							std::cout << delteItem << std::endl;
							delteItem = it.first;
						}
						ImGui::NextColumn();
					}
					if (delteItem.compare("") != 0) {
						std::cout << delteItem << std::endl;
						inspectedMD2->animations.erase(delteItem);
					}

					ImGui::PopItemWidth();
					ImGui::Columns(1);
				}
			}
			ImGui::EndChild();

			ImGui::Columns(1);
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}

	r.EndWindow();
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
		LoadSceneFromFile(filePath);
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

void SceneEditor::Draw3DWidget()
{
	static float matrixTranslation[16] = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};


	const ImGuiViewport* viewport = ImGui::GetMainViewport();

	r.StartWindow("transformWidget", false, 0.6, 0.64, 0.2, 0.06);

	
	if (inspectedObject) {
		//ImGui::SeparatorText("deezr");
		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist();
		ImGuizmo::SetRect(0, 0, viewport->WorkSize.x, viewport->WorkSize.y);

		glm::mat4 inspectTrans = inspectedObject->GetTransformMatrix();

		if (ImGuizmo::Manipulate(glm::value_ptr(camera.GetView()), glm::value_ptr(camera.GetProjection()),
			ImGuizmo::OPERATION::TRANSLATE, ImGuizmo::MODE::LOCAL, glm::value_ptr(inspectTrans))) {
			glm::vec3 position = glm::vec3(inspectTrans[3]);
			inspectedObject->SetPosition(position);
		}
	
	}

	r.EndWindow();

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



	float baseSpeed = 0.5;
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
