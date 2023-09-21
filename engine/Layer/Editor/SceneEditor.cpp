#include "SceneEditor.h"

void SceneEditor::Run(const char* filePath)
{
	/*
	if (std::strlen(filePath) > 0) {
		LoadSceneFromFile(filePath);
	}

	//main loop
	while (!glfwWindowShouldClose(window))
	{
		// timer
		double currentFrameTime = glfwGetTime();
		deltaTime = currentFrameTime - previousFrameTime;
		previousFrameTime = currentFrameTime;

		Update(deltaTime);

		//inputMngr.KeyActions(deltaTime);
		if (isRunning) {
			for (auto& it : scene->gameObjects) {
				it.second->Update(deltaTime);
			}
			
			renderer.RenderScene(scene->camera, *scene, deltaTime);
		}
		else {
			renderer.RenderScene(camera, *scene, deltaTime);
			if (isPhysicDebug)
				physicsManager.DrawPhysicsWorld(camera);
		}

		if (!isRunning)
			Draw(deltaTime);

		if (isRunning) {
			luaManager.RunUpdateMethod(deltaTime);
			physicsManager.Update(deltaTime);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//cleanup
	glfwDestroyWindow(window);
	glfwTerminate();

	*/
}

SceneEditor::SceneEditor()
{
	//camera.farPlane = 10000.0f;
	////init window and glfw.
	//glfwInit();
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_SAMPLES, 4);
	//
	//const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	//int wWidth = mode->width;
	//int wHeight = mode->height;
	//
	//window = glfwCreateWindow(wWidth, wHeight, "Engine", NULL, NULL);
	//
	//if (!window)
	//{
	//	std::cout << "ERROR Could not initalize window." << std::endl;
	//	glfwTerminate();
	//	return;
	//}
	//glfwMakeContextCurrent(window);
	//
	////scene camera settings
	//scene = new Scene;
	//scene->camera.aspectRatio = (float)wWidth / (float)wHeight;
	//
	//InputManager::Get().Init(window);
	//GUIRenderer::Get().Init(window);
	//renderer.Init(window);
	////aiManager.Init(scene);
	//
	////callbacks
	//glfwSetFramebufferSizeCallback(window, ResizeCallback);
	//
	////expose to lua
	////ExposeToLua();
	////luaManager.RunInitMethod();
	//
	////set light uniforms
	//auto it = ResourceManager::Get().ShaderBegin();
	//auto end = ResourceManager::Get().ShaderEnd();
	//for (it; it != end; it++) {
	//	Renderer::SetLightUniforms(scene->lights, *it->second);
	//}

	if (!scene)
		scene = new Scene;
}

SceneEditor::~SceneEditor()
{
}

void SceneEditor::OnAttatch()
{
	window = Window::GetActiveWindow();
}

void SceneEditor::OnDetatch()
{

}

void SceneEditor::OnUpdate(double deltaTime)
{
	if (!isRunning)
		CameraControl(deltaTime);
	soundEngine.SetUserPosition(camera.position);
	CheckKeys();
}

void SceneEditor::OnDraw(double deltaTime)
{
	guirenderer.StartGUI();
	if (!isRunning)
		Draw3DWidget();
	DrawInspector();
	DrawHeighrarchy();
	DrawMenu();
	DrawResources();
	guirenderer.EndGUI();
}



void SceneEditor::SaveProject(const char* path)
{
	ProjectLoader::SaveProject(scene,luaFilePath, windowName.c_str(),path);
}

void SceneEditor::LoadSceneFromFile(const char* path)
{
	inspectedObject = nullptr;
	lastObject = nullptr;

	Project nProject = ProjectLoader::LoadProject(path);
	scene = nProject.scene;// &SceneLoader::LoadScene(path);
	luaFilePath = nProject.luaPath;
	windowName = nProject.windowName;

	saveFilePath = path;

	if (!scene)
		return;
	for (auto& shader : ResourceManager::Get().shaders) {
		//Renderer::Get().SetLightUniforms(scene->lights, *shader.second);
	}

	Json::Value root;
	std::ifstream jsonFile(path);
	jsonFile >> root;
	jsonFile.close();
	std::string luaMain = root["luaPath"].asString();
	SetLuaFile(luaMain.c_str());

}

void SceneEditor::UseScene(Scene* nscene)
{
	if (nscene) {
		scene = nscene;
	}
}

void SceneEditor::ResizeCallback(GLFWwindow* window, int width, int height)
{
	//if (width <= 0 || height <= 0)
	//	return;
	//SceneEditor& editor = SceneEditor::Get();
	//editor.camera.aspectRatio = (float)width / (float)height;
	//glViewport(0, 0, width, height);
	//editor.renderer.Resize(width, height);
	//editor.renderer.RenderScene(editor.camera, *editor.scene, editor.deltaTime);
}

void SceneEditor::DrawHeighrarchy()
{
	float align = 0.0;
	static ImGuiTreeNodeFlags baseFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

	guirenderer.StartWindow("Scene Objects", true, 0.2, 0.94, 0.0, 0.06);

	ResourceManager& res = ResourceManager::Get();
	
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

	int j = 0;
	std::string delname = "";
	for (auto& pair : scene->gameObjects)
	{
		ImGuiTreeNodeFlags tmpFlags = baseFlags;
		if (selectedNode == j) {
			tmpFlags |= ImGuiTreeNodeFlags_Selected;
		}

		bool nodeOpen = ImGui::TreeNodeEx(pair.second->name.c_str(), tmpFlags);
		if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
			inspectedObject = pair.second;
			selectedNode = j;
		}
		if (ImGui::IsMouseDoubleClicked(0) && ImGui::IsItemHovered() && inspectedObject) {
			camera.position = { inspectedObject->position.x,inspectedObject->position.y,inspectedObject->position.z - 10 };
			camera.up = { 0,1,0 };
			camera.right = { 0.0, 0.0, -1.0 };
			camera.LookAt(inspectedObject->position);
		}

		if (nodeOpen) {
			
			if (ImGui::Button("Delete##deleteObject")) {
				delname = pair.first;			}

			if (ImGui::Button((std::string("Duplicate##") + (pair.second->name)).c_str()))
			{
				std::string name = pair.first;
				std::string nName = name;
				int nSuffix = 1;
				while (res.objects.find(nName) != res.objects.end())
				{
					nName = name;
					nName.append(std::to_string(nSuffix));
					++nSuffix;
				}
				GameObject& go = res.CreateGameObject(nName, "", "");
				go = *pair.second;
				go.name = nName;

				go.physicsBody = scene->physicsWorld.CreatePhysicsBody();
				
				if(pair.second->physicsBody)
				for (int i = 0; i < pair.second->physicsBody->GetNumColliders(); ++i)
				{
					PhysicsCollider nCollider = pair.second->physicsBody->GetCollider(i);
					switch (pair.second->physicsBody->GetCollider(i).GetType())
					{
					case COLLIDER_BOX:
						scene->physicsWorld.AddBoxCollider(*go.physicsBody,static_cast<BoxCollider*>(&nCollider)->GetScale());
						break;
					case COLLIDER_SPHERE:
						scene->physicsWorld.AddSphereCollider(*go.physicsBody, static_cast<SphereCollider*>(&nCollider)->GetRadius());
						break;
					case COLLIDER_CAPSULE:
						scene->physicsWorld.AddCapsuleCollider(*go.physicsBody, static_cast<CapsuleCollider*>(&nCollider)->GetRadius(), static_cast<CapsuleCollider*>(&nCollider)->GetHeight());
						break;
					default:
						break;
					}
					go.physicsBody->GetCollider(i).SetOffset(nCollider.GetOffset());
					go.physicsBody->GetCollider(i).SetRotation(nCollider.GetRotation());

					
				}

				scene->AddObject(go);
			}
			ImGui::TreePop();
		}
		j++;
	}
	if (!delname.empty()) {
		inspectedObject = nullptr;
		lastObject = nullptr;

		GameObject* delObj = res.GetGameObject(delname);

		//delete physics
		if(delObj->physicsBody != nullptr)
			scene->physicsWorld.DeletePhysicsBody(delObj->physicsBody);

		res.DeleteGameObject(delname);
		scene->gameObjects.erase(delname);
	}
		
	ImGui::SeparatorText("Skybox");
	if (!scene->skybox) {

		static char cmSides[6][128];
		std::string path;
		ImGui::InputTextWithHint("##cmright",	"Face right",	cmSides[0], IM_ARRAYSIZE(cmSides[0])); ImGui::SameLine(); if(ImGui::Button("Open File##sbRt")){ 
			path = FileOpener::OpenFileDialogue();
			if (path.size() >= 1) {strcpy(cmSides[0], path.c_str());}
		}
		ImGui::InputTextWithHint("##cmleft",	"Face left",	cmSides[1], IM_ARRAYSIZE(cmSides[1])); ImGui::SameLine(); if(ImGui::Button("Open File##sbLf")){
			path = FileOpener::OpenFileDialogue();
			if (path.size() >= 1) { strcpy(cmSides[1], path.c_str()); }
		}
		ImGui::InputTextWithHint("##cmtop",		"Face top",		cmSides[2], IM_ARRAYSIZE(cmSides[2])); ImGui::SameLine(); if(ImGui::Button("Open File##sbTp")){
			path = FileOpener::OpenFileDialogue();
			if (path.size() >= 1) { strcpy(cmSides[2], path.c_str()); }
		}
		ImGui::InputTextWithHint("##cmbottom",	"Face bottom",	cmSides[3], IM_ARRAYSIZE(cmSides[3])); ImGui::SameLine(); if(ImGui::Button("Open File##sbBt")){
			path = FileOpener::OpenFileDialogue();
			if (path.size() >= 1) { strcpy(cmSides[3], path.c_str()); }
		}
		ImGui::InputTextWithHint("##cmfront",	"Face front",	cmSides[4], IM_ARRAYSIZE(cmSides[4])); ImGui::SameLine(); if(ImGui::Button("Open File##sbFt")){
			path = FileOpener::OpenFileDialogue();
			if (path.size() >= 1) { strcpy(cmSides[4], path.c_str()); }
		}
		ImGui::InputTextWithHint("##cmback",	"Face back",	cmSides[5], IM_ARRAYSIZE(cmSides[5])); ImGui::SameLine(); if(ImGui::Button("Open File##sbBk")){
			path = FileOpener::OpenFileDialogue();
			if (path.size() >= 1) { strcpy(cmSides[5], path.c_str()); }
		}

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
			//Renderer::Get().SetLightUniforms(scene->lights, *shader.second);
		}
	}


	//Directional Lights
	int deleteIndex = -1;

	if (ImGui::Button("Add Direction Light")) {
		scene->lights.AddDirectionLight({ 0,1,0 }, { 1,1,1 }, { 1,1,1 });

		for (auto& shader : ResourceManager::Get().shaders) {
			//Renderer::Get().SetLightUniforms(scene->lights, *shader.second);
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
					//Renderer::Get().SetLightUniforms(scene->lights, *shader.second);
				}
			};
		
			glm::vec3 lightDir = -(scene->lights.direction[i].direction);
			if (ImGui::gizmo3D("##gizmo1", lightDir)) {
				scene->lights.direction[i].direction = -(lightDir);
				for (auto& shader : res.shaders) {
					//Renderer::Get().SetLightUniforms(scene->lights, *shader.second);
				}
			}

			ImGui::TreePop();
		}
	}
	if (deleteIndex >= 0) {
		scene->lights.direction.erase(scene->lights.direction.begin() + deleteIndex);
		for (auto& shader : res.shaders) {
			//Renderer::Get().SetLightUniforms(scene->lights, *shader.second);
		}
	}
	
	//point Lights
	deleteIndex = -1;

	if (ImGui::Button("Add Point Light")) {
		scene->lights.AddPointLight({ 0,0,0 }, { 1,1,1 }, {1,1,1}, 1.0, 0.007, 0.0002);
		for (auto& shader : ResourceManager::Get().shaders) {
			//Renderer::Get().SetLightUniforms(scene->lights, *shader.second);
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
					//Renderer::Get().SetLightUniforms(scene->lights, *shader.second);
				}
			};
			ImGui::TreePop();
		}
	}
	if (deleteIndex >= 0) {
		scene->lights.point.erase(scene->lights.point.begin() + deleteIndex);
		for (auto& shader : res.shaders) {
			//Renderer::Get().SetLightUniforms(scene->lights, *shader.second);
		}
	}

	ImGui::End();

}

void SceneEditor::DrawInspector()
{
	guirenderer.StartWindow("Inspector",true,0.2,0.94,0.8,0.06);

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
			//res.objects.find(inspectedObject->name);
			res.objects.erase(inspectedObject->name);
			inspectedObject->name = cmObjName;
			res.StoreGameObject(inspectedObject);
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

		if (ImGui::RadioButton("Cast Shadows", inspectedObject->isCastShadow))
		{
			inspectedObject->isCastShadow = !inspectedObject->isCastShadow;
		}
			
	
		selectedShader = "";
		if (inspectedObject->shader)
			selectedShader = inspectedObject->shader->name;
		selectedMesh = "";
		if (inspectedObject->model_data)
			selectedMesh = inspectedObject->model_data->name;
		

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
			if (ImGui::Selectable("--None--"))
				inspectedObject->model_data = nullptr;
			for (auto it : res.models)
			{
				if (ImGui::Selectable(it.first.c_str())) {
					selectedMesh = it.first;
					inspectedObject->model_data = it.second;
				}
			}
			ImGui::EndCombo();
		}
		ImGui::Text("Material:");
		std::string diffName = "";
		if (!inspectedObject->material.diffuseTexture.empty())
			diffName = inspectedObject->material.diffuseTexture[0]->name;
		if (ImGui::BeginCombo("Diffuse##dinspectedModel", diffName.c_str()))
		{
			if (ImGui::Selectable("--None--"))
				inspectedObject->material.diffuseTexture.clear();
			for (auto it : res.textures)
			{
				if (ImGui::Selectable(it.first.c_str())) {
					if (inspectedObject->material.diffuseTexture.empty()) {
						inspectedObject->material.diffuseTexture.push_back(it.second);
					}
					else {
						inspectedObject->material.diffuseTexture[0] = it.second;
					}
				}
			}
			ImGui::EndCombo();
		}
		std::string specName = "";
		if (!inspectedObject->material.specularMap.empty())
			specName = inspectedObject->material.specularMap[0]->name;
		if (ImGui::BeginCombo("Specular##dinspectedModel", specName.c_str()))
		{
			if (ImGui::Selectable("--None--"))
				inspectedObject->material.specularMap.clear();
			for (auto it : res.textures)
			{
				if (ImGui::Selectable(it.first.c_str())) {
					if (inspectedObject->material.specularMap.empty()) {
						inspectedObject->material.specularMap.push_back(it.second);
					}
					else {
						inspectedObject->material.specularMap[0] = it.second;
					}
				}
			}
			ImGui::EndCombo();
		}

		std::string emissName = "";
		if (!inspectedObject->material.emissionMap.empty())
			emissName = inspectedObject->material.emissionMap[0]->name;
		if (ImGui::BeginCombo("Emissive##dinspectedModel", emissName.c_str()))
		{
			if (ImGui::Selectable("--None--"))
				inspectedObject->material.emissionMap.clear();
			for (auto it : res.textures)
			{
				if (ImGui::Selectable(it.first.c_str())) {
					if (inspectedObject->material.emissionMap.empty()) {
						inspectedObject->material.emissionMap.push_back(it.second);
					}
					else {
						inspectedObject->material.emissionMap[0] = it.second;
					}
				}
			}
			ImGui::EndCombo();
		}
		ImGui::DragFloat("Shine##materialShine", &inspectedObject->material.shine);


		//PHYSICS SETTINGS
		ImGui::SeparatorText("Physics");
		if(inspectedObject->physicsBody)
		if (ImGui::RadioButton("Is Kinematic", inspectedObject->physicsBody->isKinematic))
		{
			inspectedObject->physicsBody->isKinematic = !inspectedObject->physicsBody->isKinematic;
		}
		//Box
		if (ImGui::Button("Add Box Collider##box")){
			if (!inspectedObject->physicsBody)
				inspectedObject->physicsBody = scene->physicsWorld.CreatePhysicsBody();
			scene->physicsWorld.AddBoxCollider(*inspectedObject->physicsBody, {1.0f,1.0f,1.0f});
		}

		//Sphere
		if (ImGui::Button("Add Sphere Collider##sphere")) {
			if (!inspectedObject->physicsBody)
				inspectedObject->physicsBody = scene->physicsWorld.CreatePhysicsBody();
			scene->physicsWorld.AddSphereCollider(*inspectedObject->physicsBody, 1.0f);
		}

		//Capsule
		if (ImGui::Button("Add Capsule Collider##capsule")) {
			if (!inspectedObject->physicsBody)
				inspectedObject->physicsBody = scene->physicsWorld.CreatePhysicsBody();
			scene->physicsWorld.AddCapsuleCollider(*inspectedObject->physicsBody, 1.0f,2.0f);
		}

		static const char* colliderNames[4] = {"Box Collider","Sphere Collider", "Capsule Collider", "Terrain Collider"};
		if (inspectedObject->physicsBody) {

			PhysicsBody* pb = inspectedObject->physicsBody;
			unsigned int i = 0;
			for (auto& it : pb->colliders)
			{

				std::string nodeName = std::string("##") + std::to_string(i);
				if (ImGui::TreeNodeEx((std::string(colliderNames[it.GetType() - 1]) + nodeName).c_str())) {
					glm::vec3 nOffset = it.GetOffset();
					if (ImGui::DragFloat3((std::string("position") + nodeName).c_str(), &nOffset.x, 0.01f))
					{
						it.SetOffset(nOffset);
					}

					glm::vec3 nRotation = it.GetRotation();
					if (ImGui::DragFloat3((std::string("rotation") + nodeName).c_str(), &nRotation.x, 0.01f))
					{
						it.SetRotation(nRotation);
					}
					
					if (it.GetType() == COLLIDER_BOX) {

						glm::vec3 nScale = static_cast<BoxCollider*>(&it)->GetScale();
						if (ImGui::DragFloat3((std::string("scale") + nodeName).c_str(), &nScale.x, 0.01f))
						{
							static_cast<BoxCollider*>(&it)->SetScale(nScale);
						}
					}

					if (it.GetType() == COLLIDER_SPHERE) {

						float nRadius = static_cast<SphereCollider*>(&it)->GetRadius();
						if (ImGui::DragFloat((std::string("Radius") + nodeName).c_str(), &nRadius, 0.01f))
						{
							if (nRadius > 0)
							static_cast<SphereCollider*>(&it)->SetRadius(nRadius);
						}
					}

					if (it.GetType() == COLLIDER_CAPSULE) {

						float nRadius = static_cast<CapsuleCollider*>(&it)->GetRadius();
						float nHeight = static_cast<CapsuleCollider*>(&it)->GetHeight();
						if (ImGui::DragFloat((std::string("Radius") + nodeName).c_str(), &nRadius, 0.01f))
						{
							if (nRadius > 0)
							static_cast<CapsuleCollider*>(&it)->SetRadius(nRadius);
						}
						if (ImGui::DragFloat((std::string("Height") + nodeName).c_str(), &nHeight, 0.01f))
						{
							if(nHeight > 0)
							static_cast<CapsuleCollider*>(&it)->SetHeight(nHeight);
						}
					}

					if (ImGui::Button((std::string("Delete") + nodeName).c_str()))
					{
						pb->DeleteCollider(i);
					}
					ImGui::TreePop();
				}
				++i;
			}
		}
		
		//std::string dName = std::string("Dir Light##" + std::to_string(i));
	
		//AUDIO SETTINGS
		/*
		* 
		* Another WIP, rather than playing a sound, this will
		* add a sound to the object that can be played through
		* scripting
		* 
		ImGui::SeparatorText("Audio");

		std::vector<std::string> audioNames = soundEngine.GetAudioNames();

		ImGui::Text("Add Audio");
		if (ImGui::BeginCombo("##audio", " "))
		{
			for (int i = 0; i < audioNames.size(); i++)
			{
				if (ImGui::Selectable(audioNames[i].c_str())) {
					soundEngine.PlayLoopAtPosition(audioNames[i], inspectedObject->position);
				}
			}
			ImGui::EndCombo();
		}

		*/

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
	float width = 1.0; float height = 0.06; float posY = 0.0; float posX = 0.0;
	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	InputManager& input = InputManager::Get();

	ImGui::SetNextWindowPos({ (float)(viewport->WorkSize.x * posX),(float)(viewport->WorkSize.y * posY) });
	ImGui::SetNextWindowSize({ viewport->WorkSize.x * width,viewport->WorkSize.y * height });
	ImGui::Begin("Menu", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings |ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar);

	if (ImGui::BeginMenuBar()) {
		
		if (ImGui::BeginMenu("File")) {
			
			if (ImGui::MenuItem("New")) { 
				delete scene;
				scene = new Scene;
				scene->physicsWorld.ResetPhysicsWorld();
				saveFilePath[0] = '\0';
				luaFilePath = ("resources/scripts/main.lua");
				inspectedObject = nullptr;
				lastObject = nullptr;
			}
			if (ImGui::MenuItem("Save", "Ctrl+S")) {
				if (saveFilePath.size() < 1)
				{
					std::string savePath = FileOpener::OpenFileDialogue(SAVE_FILE);
					if (scene && !savePath.empty()) {
						SaveProject(savePath.c_str());
						saveFilePath = savePath;
					}
				}
				else if(scene){
					SaveProject(saveFilePath.c_str());
				}
			}
			if (ImGui::MenuItem("Save As",NULL)) {
				std::string savePath = FileOpener::OpenFileDialogue(SAVE_FILE);
				if (scene) {
					SaveProject(savePath.c_str());
					saveFilePath = savePath;
				}
			}
			if (ImGui::MenuItem("Open",NULL)) {
				
				std::string filePath = FileOpener::OpenFileDialogue();
				if (filePath.size() >= 1) {
					LoadSceneFromFile(filePath.c_str());
					
				}
			}
			
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit")) {

			if (ImGui::MenuItem("Change LUA file")) {
				
				std::string lpath = FileOpener::OpenFileDialogue(OPEN_FILE);
				if (!lpath.empty()) {
					SetLuaFile(lpath.c_str());
				}
				else {
					std::cout << "ERROR: Could not load lua file " << lpath << std::endl;
				}

			}
			ImGui::MenuItem("Window Title",NULL, &showChangeWindow);
			ImGui::MenuItem("Window Icon");

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help")) {

			if (ImGui::BeginMenu("About")) {

				ImGui::SeparatorText("About");
				ImGui::Text("Developed By");
				ImGui::Text("Team LAAN: Revengeance:");
				ImGui::Text("- Andres Comeros-Ochtman");
				ImGui::Text("- Nathan Choo");
				ImGui::Text("- Aidan O'Connor");
				ImGui::Text("- Lochlyn Edward");
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Version")) {
				auto ver = "Engine Version: " + std::to_string(LAAN_ENGINE_VERSION);
				ImGui::Text(ver.c_str());

				ImGui::EndMenu();
			}
			ImGui::MenuItem("Debug",NULL,&showDebug);

			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	float buttonWidth = 80;
	ImGui::SetCursorPosX((viewport->WorkSize.x/2) - (((buttonWidth + ImGui::GetStyle().ItemSpacing.x) * 3)/2));
	

	static ImVec4 baseCol = ImGui::GetStyle().Colors[ImGuiCol_Button];
	static ImVec4 selectedCol = ImGui::GetStyle().Colors[ImGuiCol_Header];
	static ImVec4 pButtonCol = baseCol;

	if (isRunning)
		pButtonCol = selectedCol;
	else
		pButtonCol = baseCol;

	ImGui::PushStyleColor(ImGuiCol_Button,pButtonCol);
	if (ImGui::Button("Play", { buttonWidth,20 })) { 
		isRunning = !isRunning;
		//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		input.SetMouseLock(false);
	}
	ImGui::PopStyleColor();


	ImGui::SameLine();
	if (ImGui::Button("Pause", { buttonWidth,20 })) { isRunning = false; }
	ImGui::SameLine();
	if (ImGui::Button("FreeCam", { buttonWidth,20 })) { InputManager::Get().SetMouseLock(false); }

	guirenderer.EndWindow();

	DrawDebug(&showDebug);
	DrawWindowSettings(&showChangeWindow);
	//DrawOpenFile(&showOpenFile);
	DrawSaveFile(&showSaveFile);
}

void SceneEditor::DrawResources()
{
	float windowWidth = 0.6;
	float resourceWidth = 64;
	guirenderer.StartWindow("Resource Inspector", true, windowWidth, 0.3, 0.2, 0.7);
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
			static char texturePath[512] = "";
			static char textureName[512] = "";
			ImGui::InputTextWithHint("##texturefilePathInput", "File Path", texturePath, IM_ARRAYSIZE(texturePath)); 
			ImGui::SameLine();
			if (ImGui::Button("Open File##openTextureFile"))
			{
				std::string tPath = FileOpener::OpenFileDialogue();

				tPath = FilterFilePath(tPath);

				if (tPath.size() >= 1) {
					strcpy(texturePath, tPath.c_str());
				}

			}
			ImGui::InputTextWithHint("##textureNameInput", "Texture Name", textureName, IM_ARRAYSIZE(textureName));

			if (ImGui::Button("Add Texture"))
			{
				res.LoadTexture(textureName,texturePath);
				texturePath[0] = '\0';
				textureName[0] = '\0';
			}
	
			int colCount = (viewport->Size.x * windowWidth) / (resourceWidth + (style.ItemSpacing.x * 2));
			if (colCount <= 0)
				colCount = 1;
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
			if (colCount <= 0)
				colCount = 1;
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
			ImGui::Columns(3, "texCols", false);
	
			ImGui::Text("Model File and Name:");
			static char modelName[512] = "";
			static char modelPath[512] = "";
			ImGui::InputTextWithHint("##modelPath", "File path" , modelPath, IM_ARRAYSIZE(modelPath));
			ImGui::SameLine();
			if (ImGui::Button("Open File##openTextureFile"))
			{
				std::string mPath = FileOpener::OpenFileDialogue();

				mPath = FilterFilePath(mPath);

				if (mPath.size() >= 1) {
					strcpy(modelPath, mPath.c_str());
				}

			}
			ImGui::InputTextWithHint("##modelName", "Model Name", modelName, IM_ARRAYSIZE(modelName));

			if (ImGui::Button("Add Model")) {
				std::string::size_type idx = std::string(modelPath).rfind('.');
				std::string extension = std::string(modelPath).substr(idx + 1);

				if(extension.compare("obj") == 0){
					res.LoadModel(modelName, modelPath);
				}
				else if (extension.compare("md2") == 0) {
					res.LoadAnimatedModel(modelName, modelPath);
				}
			}

			Texture* shaderIcon = res.GetTexture("default");
			ImGui::NextColumn();

			ImGui::BeginChild("##modelsCol");

			static DrawItem* inspectedModel = nullptr;
			int colCount = ((viewport->Size.x * windowWidth) / 3) / (resourceWidth + (style.ItemSpacing.x * 3));
			if (colCount <= 0)
				colCount = 1;
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
		ImGui::Columns(1);

		//AUDIO TAB
		if (ImGui::BeginTabItem("Audio"))
		{
			ImGui::Columns(3, "texCols", false);

			ImGui::Text("Audio File and Name:");
			static char audioName[512] = "";
			static char audioPath[512] = "";
			ImGui::InputTextWithHint("##audioPath", "File path", audioPath, IM_ARRAYSIZE(audioPath));
			ImGui::SameLine();
			if (ImGui::Button("Open File##openTextureFile"))
			{
				std::string aPath = FileOpener::OpenFileDialogue();
				if (aPath.size() >= 1) {
					strcpy(audioPath, aPath.c_str());
				}

			}
			ImGui::InputTextWithHint("##audioName", "Audio Name", audioName, IM_ARRAYSIZE(audioName));

			if (ImGui::Button("Add Audio")) {
				std::string::size_type idx = std::string(audioPath).rfind('.');
				std::string extension = std::string(audioPath).substr(idx + 1);

				soundEngine.AddSound(audioName, audioPath);
			}

			int colCount = (viewport->Size.x * windowWidth) / (resourceWidth + (style.ItemSpacing.x * 2));
			ImGui::Columns(colCount, "texCols", false);

			Texture* shaderIcon = res.GetTexture("default");

			//show all audio loaded
			std::vector<std::string> audioNames = soundEngine.GetAudioNames();

			for (int i = 0; i < audioNames.size(); i++)
			{
				ImGui::Image((void*)(intptr_t)shaderIcon->ID, ImVec2(resourceWidth, resourceWidth));
				ImGui::Text(audioNames[i].c_str());
				ImGui::NextColumn();
			}
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}

	guirenderer.EndWindow();
}

void SceneEditor::DrawWindowSettings(bool* showChangeWindow)
{
	if(!(*showChangeWindow))
		return

	ImGui::SetNextWindowSize({ 500,70 });
	ImGui::Begin("Set Window Name", showChangeWindow);
	static char str0[128] = "";
	if (ImGui::InputTextWithHint("Window Title", "Window Name", str0, IM_ARRAYSIZE(str0))) {
		
	}
	if (ImGui::Button("Set Window Title##420"))
	{
		windowName = str0;
		window->SetName(windowName);
	}

	ImGui::End();
}

void SceneEditor::DrawDebug(bool* showDebug)
{
	if (!(*showDebug))
		return;

	ImGui::SetNextWindowSize({ 300,200 });
	ImGui::Begin("Debug", showDebug);
	
	double fps = 0;// Renderer::Get().GetFPS();
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

	if (ImGui::Button("Toggle Physics Debug"))
	{
		isPhysicDebug = !isPhysicDebug;
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
	static char saveFileBuf[256];
	if (ImGui::InputTextWithHint("##savefilePath", "FilePath", saveFileBuf, IM_ARRAYSIZE(saveFileBuf))) {
		saveFilePath = saveFileBuf;
	}
	if (ImGui::Button("Save##saveFile")) {
		if(scene)
			SceneLoader::SaveScene(scene, saveFilePath);
	}

	ImGui::End();
}

void SceneEditor::Draw3DWidget()
{
	if (!ImGui::GetCurrentContext())
		return;
	static float matrixTranslation[16] = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoTitleBar | 
		ImGuiWindowFlags_NoBackground | 
		ImGuiWindowFlags_NoBringToFrontOnFocus;

	float posX = 0.2; float posY = 0.06; float width = 0.6; float height = 0.64;
	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos({ (float)(viewport->WorkSize.x * posX),(float)(viewport->WorkSize.y * posY) });
	ImGui::SetNextWindowSize({ viewport->WorkSize.x * width,viewport->WorkSize.y * height });

	ImGui::Begin("transformWidget", nullptr, flags);
	
	if (inspectedObject) {
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

	guirenderer.EndWindow();
}

void SceneEditor::CameraControl(double deltaTime)
{
	InputManager& input = InputManager::Get();
	isFreecam = input.GetMouseLock();
	static bool toggleCamPress = false;
	if (input.GetKeyPressedDown(GLFW_KEY_ESCAPE) && !toggleCamPress) {
		toggleCamPress = true;
		isFreecam = !input.GetMouseLock();
		input.SetMouseLock(isFreecam);
		lastX = input.GetMouseX();
		lastY = input.GetMouseY();
	}
	else if (input.GetKeyPressedDown(GLFW_KEY_ESCAPE)) {
		toggleCamPress = true;
	}
	else {
		toggleCamPress = false;
	}
	if (isFreecam)
		return;

	float baseSpeed = 0.2;
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

	if (camera.Pitch > 85.0f)
		camera.Pitch = 85.0f;
	if (camera.Pitch < -85.0f)
		camera.Pitch = -85.0f;

	camera.UpdateCameraVectors();

}

void SceneEditor::CheckKeys()
{
	InputManager& input = InputManager::Get();

	static bool saveDown = false;

	bool savePressed = input.GetKeyPressedDown(GLFW_KEY_LEFT_CONTROL) && input.GetKeyPressedDown(GLFW_KEY_S);

	if (saveDown) {
		if (!savePressed) {
			saveDown = false;
		}
	}

	if (savePressed) {
		if(!saveDown)
			if (saveFilePath.size() > 0 && scene) {
				SaveProject(saveFilePath.c_str());
			}
			else if(scene){
				saveFilePath = "untitled_Save.json";
				SaveProject(saveFilePath.c_str());
			}
		saveDown = true;
	}

	if (isRunning && input.GetKeyPressedDown(GLFW_KEY_F5)) {
		isRunning = false;
	}

}

void SceneEditor::SetLuaFile(std::string nluaFile)
{
	//luaFilePath = (nluaFile);
	//
	//luaManager.SetLuaFile(nluaFile.c_str());
	//luaManager.Expose_CPPReference("scene", *scene);
	//luaManager.Expose_CPPReference("GUI", guirenderer);
	//luaManager.Expose_CPPReference("resources", ResourceManager::Get());
	//
	//luaManager.RunInitMethod();
}

std::string SceneEditor::FilterFilePath(std::string filePath)
{
	std::string temp;
	int flag = 0;

	for (int i = 0; i < filePath.size(); i++)
	{
		if (flag == 0)
		{
			if (filePath[i] == '\\')
			{
				if (temp == "\\resources")
				{
					flag = 1;
				}
				else
				{
					temp.clear();
				}
			}
		}
		else if (flag == 1)
		{
			temp = temp.substr(1, temp.size() - 1);
			flag = 2;
		}

		temp = temp + filePath[i];
	}

	return temp;
}