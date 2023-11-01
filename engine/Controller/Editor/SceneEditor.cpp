#include "SceneEditor.h"

SceneEditor::SceneEditor(GameEngine* nEngine, DebugLogger* logger):
	engine(nEngine),
	guirenderer(nEngine->guiRenderer),
	_logger(logger)
{
	engine->isRunning = false;
	UseScene(engine->scene);
	pathDebugLine.SetWidth(10);
	selectedNavNodeBox.SetScale({ 0.7, 0.7, 0.7 });
	selectedNavNodeBox.SetColor({0.0,1.0,1.0,0.8});
	selectedNavNodeBox.SetEnabled(false);
}

SceneEditor::~SceneEditor()
{
}

void SceneEditor::Update(double deltaTime)
{
	if (!engine->isRunning)
		CameraControl(deltaTime);

	CheckKeys();
}

void SceneEditor::Draw(double deltaTime)
{
	//if (engine->isRunning)
	//	return;

	if (isPhysicDebug)
		engine->scene->physicsWorld.DrawPhysicsWorld(*camera);

	if (isPathDebug) {
		engine->scene->pathManager.DrawDebug(camera->GetProjection(), camera->GetView(), ResourceManager::Get().GetShader("line"));
		pathDebugLine.RenderFront(camera->GetProjection(), camera->GetView(), ResourceManager::Get().GetShader("line"));
		selectedNavNodeBox.Render(camera->GetProjection(), camera->GetView(), ResourceManager::Get().GetShader("line"));
	}

	guirenderer.StartGUI();
	Draw3DWidget();
	DrawInspector();
	DrawHeighrarchy();
	DrawMenu();
	DrawResources();
	guirenderer.EndGUI();
}



void SceneEditor::SaveProject(const char* path)
{
	ProjectLoader::SaveProject(engine->scene,luaFilePath, windowName.c_str(),path);
}

void SceneEditor::LoadSceneFromFile(const char* path)
{
	inspectedObject = nullptr;
	lastObject = nullptr;

	Project nProject = ProjectLoader::LoadProject(engine,path);
	UseScene(nProject.scene);
	luaFilePath = nProject.luaPath;
	windowName = nProject.windowName;
	Window::GetActiveWindow()->SetName(windowName);

	saveFilePath = path;

	if (!engine->scene)
		return;
	for (auto& shader : ResourceManager::Get().shaders) {
		engine->renderer.SetLightUniforms(engine->scene->lights, *shader.second);
	}

	Json::Value root;
	std::ifstream jsonFile(path);
	jsonFile >> root;
	jsonFile.close();
	std::string luaMain = root["luaPath"].asString();

	//SetLuaFile(luaMain.c_str());

}

void SceneEditor::UseScene(Scene* nscene)
{
	if (nscene) {
		engine->scene = nscene;
		camera = &nscene->camera;
	}
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


	if (ImGui::Button("Add Object", {100,0})) {
		
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
		engine->scene->AddObject(go);
	}

	ImGui::SameLine();

	if (ImGui::Button("Add NPC", { 100,0 })) {

		std::string name = "new NPC";
		std::string nName = name;
		int nSuffix = 1;
		while (res.objects.find(nName) != res.objects.end())
		{
			nName = name;
			nName.append(std::to_string(nSuffix));
			++nSuffix;
		}
		NPC_GameObject& go = res.CreateNPC(nName, "", "");
		go.SetPathManager(&engine->scene->pathManager);
		engine->scene->AddObject(go);
	}

	int j = 0;
	std::string delname = "";
	if(engine->scene)
	for (auto& pair : engine->scene->gameObjects)
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
		if (ImGui::IsMouseDoubleClicked(0) && ImGui::IsItemHovered() && inspectedObject && camera) {
			camera->position = { inspectedObject->position.x,inspectedObject->position.y,inspectedObject->position.z - 10 };
			camera->up = { 0,1,0 };
			camera->right = { 0.0, 0.0, -1.0 };
			camera->LookAt(inspectedObject->position);
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
				GameObject* go;
				
				if (dynamic_cast<NPC_GameObject*>(pair.second) != nullptr) {
					go = &res.CreateNPC(nName, "", "");
					*go = *pair.second;
				}
				else {
					go = &res.CreateGameObject(nName, "", "");
					*go = *pair.second;
				}
				
				go->name = nName;

				PhysicsBody* pbOld = pair.second->physicsBody;
				PhysicsBody* pbNew = engine->scene->physicsWorld.CreatePhysicsBody(go);
				
				if(pbOld)
				for (int i = 0; i < pbOld->GetNumColliders(); ++i)
				{
					PhysicsCollider oldCollider = pbOld->GetCollider(i);
					PhysicsCollider* newCollider = nullptr;
					switch (pbOld->GetCollider(i).GetType())
					{
					case COLLIDER_BOX:
						newCollider = engine->scene->physicsWorld.AddBoxCollider(*go->physicsBody,static_cast<BoxCollider*>(&oldCollider)->GetScale());
						break;
					case COLLIDER_SPHERE:
						newCollider = engine->scene->physicsWorld.AddSphereCollider(*go->physicsBody, static_cast<SphereCollider*>(&oldCollider)->GetRadius());
						break;
					case COLLIDER_CAPSULE:
						newCollider =engine->scene->physicsWorld.AddCapsuleCollider(*go->physicsBody, static_cast<CapsuleCollider*>(&oldCollider)->GetRadius(), static_cast<CapsuleCollider*>(&oldCollider)->GetHeight());
						break;
					default:
						break;
					}
					newCollider->SetMass(oldCollider.GetMass());
					pbNew->GetCollider(i).SetOffset(oldCollider.GetOffset());
					pbNew->GetCollider(i).SetRotation(oldCollider.GetRotation());
				}
				if (pair.second->physicsBody) {
					pbNew->SetPositionVec(pbOld->GetPosition());
					pbNew->SetRotation(pbOld->GetRotation());
					pbNew->SetMass(pbOld->GetMass());
					pbNew->dampeningLinear = pbOld->dampeningLinear;
					pbNew->dampeningAngular = pbOld->dampeningAngular;
					pbNew->bounce = pbOld->bounce;
					pbNew->CalcDerivedData();
				}

				engine->scene->AddObject(*go);
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
			engine->scene->physicsWorld.DeletePhysicsBody(delObj->physicsBody);

		res.DeleteGameObject(delname);
		engine->scene->gameObjects.erase(delname);
	}
		
	ImGui::SeparatorText("Skybox");
	if(engine->scene)
	if (!engine->scene->skybox) {

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
			engine->scene->SetSkybox(res.GetCubeMap(cmSides[0]));
		}
	}
	else {
		if (ImGui::Button("Remove Skybox")) {
			engine->scene->skybox = nullptr;
		}
	}

	ImGui::SeparatorText("Lights");
	//Ambient Light
	if(engine->scene)
	if (ImGui::ColorEdit3("Ambient Light", (float*)&engine->scene->lights.ambient)) {
		for (auto& shader : res.shaders) {
			engine->renderer.SetLightUniforms(engine->scene->lights, *shader.second);
		}
	}


	//Directional Lights
	int deleteIndex = -1;

	if (ImGui::Button("Add Direction Light")) {
		engine->scene->lights.AddDirectionLight({ 0,1,0 }, { 1,1,1 }, { 1,1,1 });

		for (auto& shader : ResourceManager::Get().shaders) {
			engine->renderer.SetLightUniforms(engine->scene->lights, *shader.second);
		}
	}
	if(engine->scene)
	for (int i = 0; i < engine->scene->lights.direction.size(); ++i)
	{
		std::string dName = std::string("Dir Light##" + std::to_string(i));
		if(ImGui::TreeNodeEx(dName.c_str())) {
			if (ImGui::Button(std::string("Delete Light##" + std::to_string(i)).c_str())) {
				deleteIndex = i;
			}

			bool dColor = ImGui::ColorEdit3(("Diffuse##"+dName).c_str(), (float*)&engine->scene->lights.direction[i].diffuse);
			bool dSpec = ImGui::ColorEdit3(("Specular##"+dName).c_str(), (float*)&engine->scene->lights.direction[i].specular);
			bool dPos = ImGui::InputFloat3 (("Direction##"+dName).c_str(), (float*)&engine->scene->lights.direction[i].direction);
			if (dColor || dPos || dSpec) {

				for (auto& shader : res.shaders) {
					engine->renderer.SetLightUniforms(engine->scene->lights, *shader.second);
				}
			};
		
			glm::vec3 lightDir = -(engine->scene->lights.direction[i].direction);
			if (ImGui::gizmo3D("##gizmo1", lightDir)) {
				engine->scene->lights.direction[i].direction = -(lightDir);
				for (auto& shader : res.shaders) {
					engine->renderer.SetLightUniforms(engine->scene->lights, *shader.second);
				}
			}

			ImGui::TreePop();
		}
	}
	if (deleteIndex >= 0) {
		engine->scene->lights.direction.erase(engine->scene->lights.direction.begin() + deleteIndex);
		for (auto& shader : res.shaders) {
			engine->renderer.SetLightUniforms(engine->scene->lights, *shader.second);
		}
	}
	
	//point Lights
	deleteIndex = -1;
	if(engine->scene)
	if (ImGui::Button("Add Point Light")) {
		engine->scene->lights.AddPointLight({ 0,0,0 }, { 1,1,1 }, {1,1,1}, 1.0, 0.007, 0.0002);
		for (auto& shader : ResourceManager::Get().shaders) {
			engine->renderer.SetLightUniforms(engine->scene->lights, *shader.second);
		}
	}

	if(engine->scene)
	for (int i = 0; i < engine->scene->lights.point.size(); ++i)
	{
		std::string dName = std::string("Point Light##" + std::to_string(i));
		if (ImGui::TreeNodeEx(dName.c_str())) {
			if (ImGui::Button(std::string("Delete Light##" + std::to_string(i)).c_str())) {
				deleteIndex = i;
			}
		
			bool dColor = ImGui::ColorEdit3(("Diffuse##" + dName).c_str(), (float*)&engine->scene->lights.point[i].diffuse);
			bool dSpec = ImGui::ColorEdit3(("Specular##" + dName).c_str(), (float*)&engine->scene->lights.point[i].specular);
			bool dPos = ImGui::InputFloat3(("Position##" + dName).c_str(), (float*)&engine->scene->lights.point[i].position);
			if (dColor || dSpec || dPos) {

				for (auto& shader : res.shaders) {
					engine->renderer.SetLightUniforms(engine->scene->lights, *shader.second);
				}
			};
			ImGui::TreePop();
		}
	}
	if (deleteIndex >= 0) {
		engine->scene->lights.point.erase(engine->scene->lights.point.begin() + deleteIndex);
		for (auto& shader : res.shaders) {
			engine->renderer.SetLightUniforms(engine->scene->lights, *shader.second);
		}
	}

	ImGui::End();

}

void SceneEditor::DrawInspector()
{
	guirenderer.StartWindow("Inspector",true,0.2,0.94,0.8,0.06);

	static ImGuiTreeNodeFlags baseFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Leaf;
	ResourceManager& res = ResourceManager::Get();
	
	if (ImGui::BeginTabBar("InspectorTabs", ImGuiTabBarFlags_None)) {
		if (ImGui::BeginTabItem("Inpector##inpectTab")) {
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
					cmObjName[0] = '\n';
				}
				ImGui::Dummy(ImVec2(0.0f, 10.0f));
				//TRANSFORM SETTINGS
				if (ImGui::CollapsingHeader("-- Transform --")) {
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

					float tmpRots[3];
					tmpRots[0] = glm::degrees(inspectedObject->GetRotationEuler().x);
					tmpRots[1] = glm::degrees(inspectedObject->GetRotationEuler().y);
					tmpRots[2] = glm::degrees(inspectedObject->GetRotationEuler().z);
					ImGui::Text("Rotation:");
					ImGui::DragFloat3("##rot", tmpRots);

					glm::quat nOrientation = inspectedObject->orientation;
					if (ImGui::gizmo3D("##gizmo2", nOrientation)) {
						inspectedObject->SetRotation(nOrientation);
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
					ImGui::Dummy(ImVec2(0.0f, 20.0f));
				}
				

				//Rendering SETTINGS
				if (ImGui::CollapsingHeader("-- Rendering --")) {
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
					ImGui::Dummy(ImVec2(0.0f, 20.0f));
				}
				

				//PHYSICS SETTINGS
				if (ImGui::CollapsingHeader("-- Physics --")) {
					if (inspectedObject->physicsBody)
						ImGui::Text((std::string("PhysicsBody Position: ") + std::to_string(inspectedObject->physicsBody->GetPosition().x) + " | " + std::to_string(inspectedObject->physicsBody->GetPosition().y) + " | " + std::to_string(inspectedObject->physicsBody->GetPosition().z)).c_str());
					if (inspectedObject->physicsBody)
						ImGui::Text((std::string("PhysicsBody Velocity: ") + std::to_string(inspectedObject->physicsBody->velocity.x) + " | " + std::to_string(inspectedObject->physicsBody->velocity.y) + " | " + std::to_string(inspectedObject->physicsBody->velocity.z)).c_str());

					if (inspectedObject->physicsBody)
						ImGui::Text((std::string("PhysicsBody Angular: ") + std::to_string(inspectedObject->physicsBody->angularVelocity.x) + " | " + std::to_string(inspectedObject->physicsBody->angularVelocity.y) + " | " + std::to_string(inspectedObject->physicsBody->angularVelocity.z)).c_str());
					if (inspectedObject->physicsBody)
						ImGui::Text((std::string("Total Mass: ") + std::to_string(inspectedObject->physicsBody->GetMass())).c_str());
					if (inspectedObject->physicsBody)
						ImGui::Text((std::string("Inverse Mass: ") + std::to_string(inspectedObject->physicsBody->GetInverseMass())).c_str());
					if (inspectedObject->physicsBody)
						ImGui::Text((std::string("ID: ") + std::to_string(inspectedObject->physicsBody->GetID())).c_str());

					if (inspectedObject->physicsBody)
						if (ImGui::RadioButton("Is Kinematic", inspectedObject->physicsBody->isKinematic))
						{
							inspectedObject->physicsBody->isKinematic = !inspectedObject->physicsBody->isKinematic;
							if (inspectedObject->physicsBody->isKinematic)
							{
								inspectedObject->physicsBody->SetVelocity(0.0f, 0.0f, 0.0f);
							}
						}

					if (inspectedObject->physicsBody)
						if (ImGui::RadioButton("Use Gravity", inspectedObject->physicsBody->useGravity))
						{
							inspectedObject->physicsBody->useGravity = !inspectedObject->physicsBody->useGravity;
							if (inspectedObject->physicsBody->useGravity)
							{
								inspectedObject->physicsBody->velocity.y = 0.0f;
							}
						}

					if (inspectedObject->physicsBody) {
						if (ImGui::Button("Set Mass Infinite"))
						{
							inspectedObject->physicsBody->SetMassInf();
						}
					}

					if (inspectedObject->physicsBody) {
						float nMass = inspectedObject->physicsBody->GetMass();
						if (ImGui::DragFloat("Mass##setmass", &nMass, 0.01f, 0.0f)) {
							inspectedObject->physicsBody->SetMass(nMass);
						}

						float nBounce = inspectedObject->physicsBody->GetBounce();
						if (ImGui::DragFloat("Bounce##setbounce", &nBounce, 0.01f, 0.0f, 1.0f)) {
							inspectedObject->physicsBody->SetBounce(nBounce);
						}

						float nlDamp = inspectedObject->physicsBody->GetLinearDampening();
						if (ImGui::DragFloat("LinearDampening##setLinDamp", &nlDamp, 0.01f, 0.0f, 1.0f)) {
							inspectedObject->physicsBody->SetLinearDampening(nlDamp);
						}

						float naDamp = inspectedObject->physicsBody->GetAngularDampening();
						if (ImGui::DragFloat("AngularDampening##setLinDamp", &naDamp, 0.01f, 0.0f, 1.0f)) {
							inspectedObject->physicsBody->SetAngularDampening(naDamp);
						}


					}


					//Box
					if (ImGui::Button("Add Box Collider##box")) {
						if (!inspectedObject->physicsBody)
								engine->scene->physicsWorld.CreatePhysicsBody(inspectedObject);
						engine->scene->physicsWorld.AddBoxCollider(*inspectedObject->physicsBody, { 1.0f,1.0f,1.0f });
						inspectedObject->physicsBody->CalcCenterOfMass();
					}

					//Sphere
					if (ImGui::Button("Add Sphere Collider##sphere")) {
						if (!inspectedObject->physicsBody)
							engine->scene->physicsWorld.CreatePhysicsBody(inspectedObject);
						engine->scene->physicsWorld.AddSphereCollider(*inspectedObject->physicsBody, 1.0f);
						inspectedObject->physicsBody->CalcCenterOfMass();
					}

					//Capsule
					if (ImGui::Button("Add Capsule Collider##capsule")) {
						if (!inspectedObject->physicsBody)
							engine->scene->physicsWorld.CreatePhysicsBody(inspectedObject);
						engine->scene->physicsWorld.AddCapsuleCollider(*inspectedObject->physicsBody, 1.0f, 2.0f);
						inspectedObject->physicsBody->CalcCenterOfMass();
					}

					static const char* colliderNames[4] = { "Box Collider","Sphere Collider", "Capsule Collider", "Terrain Collider" };
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
									pb->CalcCenterOfMass();
								}

								glm::vec3 nRotation = it.GetRotation();
								if (ImGui::DragFloat3((std::string("rotation") + nodeName).c_str(), &nRotation.x, 0.01f))
								{
									it.SetRotation(nRotation);
									pb->CalcCenterOfMass();
								}

								float nMass = it.GetMass();
								if (ImGui::DragFloat((std::string("Mass") + nodeName).c_str(), &nMass, 0.01f))
								{
									it.SetMass(nMass);
									pb->CalcCenterOfMass();
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
										if (nHeight > 0)
											static_cast<CapsuleCollider*>(&it)->SetHeight(nHeight);
									}
								}

								if (ImGui::Button((std::string("Delete") + nodeName).c_str()))
								{
									pb->DeleteCollider(i);
									inspectedObject->physicsBody->CalcCenterOfMass();
								}
								ImGui::TreePop();
							}
							++i;
						}
					}
					ImGui::Dummy(ImVec2(0.0f, 20.0f));
				}

				//affordance Settings
				if (ImGui::CollapsingHeader("-- Affordances --")) {
					
					AffordanceController* affordanceController = &inspectedObject->affordanceController;

					//pickup
					Affordance* affordancePickup = affordanceController->GetAffordance<AffordancePickup>();
					bool performPickup = affordancePickup->GetCanPerform();
					bool affordsPickup = affordancePickup->GetCanAfford();
					ImGui::Text("pickup");
					if (ImGui::Checkbox("Can Perform##pickup", &performPickup)) {
						affordancePickup->SetCanPerform(performPickup);
					}
					ImGui::SameLine();
					if (ImGui::Checkbox("Can Afford##pickup", &affordsPickup)) {
						affordancePickup->SetCanAfford(affordsPickup);
					}
					ImGui::Separator();

					//punch
					Affordance* affordancePunch = affordanceController->GetAffordance<AffordancePunch>();
					bool performPunch = affordancePunch->GetCanPerform();
					bool affordsPunch = affordancePunch->GetCanAfford();
					ImGui::Text("punch");
					if (ImGui::Checkbox("Can Perform##punch", &performPunch)) {
						affordancePunch->SetCanPerform(performPunch);
					}
					ImGui::SameLine();
					if (ImGui::Checkbox("Can Afford##punch", &affordsPunch)) {
						affordancePunch->SetCanAfford(affordsPunch);
					}
					ImGui::Separator();

					//slap
					Affordance* affordanceSlap = affordanceController->GetAffordance<AffordanceSlap>();
					bool performSlap = affordanceSlap->GetCanPerform();
					bool affordsSlap = affordanceSlap->GetCanAfford();
					ImGui::Text("slap");
					if (ImGui::Checkbox("Can Perform##slap", &performSlap)) {
						affordanceSlap->SetCanPerform(performSlap);
					}
					ImGui::SameLine();
					if (ImGui::Checkbox("Can Afford##slap", &affordsSlap)) {
						affordanceSlap->SetCanAfford(affordsSlap);
					}
					ImGui::Separator();

					//poke
					Affordance* affordancePoke = affordanceController->GetAffordance<AffordancePoke>();
					bool performPoke = affordancePoke->GetCanPerform();
					bool affordsPoke = affordancePoke->GetCanAfford();
					ImGui::Text("poke");
					if (ImGui::Checkbox("Can Perform##poke", &performPoke)) {
						affordancePoke->SetCanPerform(performPoke);
					}
					ImGui::SameLine();
					if (ImGui::Checkbox("Can Afford##poke", &affordsPoke)) {
						affordancePoke->SetCanAfford(affordsPoke);
					}
					ImGui::Separator();

					//sit
					Affordance* affordanceSit = affordanceController->GetAffordance<AffordanceSit>();
					bool performSit = affordanceSit->GetCanPerform();
					bool affordsSit = affordanceSit->GetCanAfford();
					ImGui::Text("sit");
					if (ImGui::Checkbox("Can Perform##sit", &performSit)) {
						affordanceSit->SetCanPerform(performSit);
					}
					ImGui::SameLine();
					if (ImGui::Checkbox("Can Afford##sit", &affordsSit)) {
						affordanceSit->SetCanAfford(affordsSit);
					}
					ImGui::Separator();

					//giveMoney
					Affordance* affordanceGiveMoney = affordanceController->GetAffordance<AffordanceGiveMoney>();
					bool performGiveMoney = affordanceGiveMoney->GetCanPerform();
					bool affordsGiveMoney = affordanceGiveMoney->GetCanAfford();
					ImGui::Text("giveMoney");
					if (ImGui::Checkbox("Can Perform##giveMoney", &performGiveMoney)) {
						affordanceGiveMoney->SetCanPerform(performGiveMoney);
					}
					ImGui::SameLine();
					if (ImGui::Checkbox("Can Afford##giveMoney", &affordsGiveMoney)) {
						affordanceGiveMoney->SetCanAfford(affordsGiveMoney);
					}
					ImGui::Separator();

					//compliment
					Affordance* affordanceCompliment = affordanceController->GetAffordance<AffordanceCompliment>();
					bool performCompliment = affordanceCompliment->GetCanPerform();
					bool affordsCompliment = affordanceCompliment->GetCanAfford();
					ImGui::Text("compliment");
					if (ImGui::Checkbox("Can Perform##compliment", &performCompliment)) {
						affordanceCompliment->SetCanPerform(performCompliment);
					}
					ImGui::SameLine();
					if (ImGui::Checkbox("Can Afford##compliment", &affordsCompliment)) {
						affordanceCompliment->SetCanAfford(affordsCompliment);
					}
					ImGui::Separator();

					//threaten
					Affordance* affordanceThreaten = affordanceController->GetAffordance<AffordanceThreaten>();
					bool performThreaten = affordanceThreaten->GetCanPerform();
					bool affordsThreaten = affordanceThreaten->GetCanAfford();
					ImGui::Text("threaten");
					if (ImGui::Checkbox("Can Perform##threaten", &performThreaten)) {
						affordanceThreaten->SetCanPerform(performThreaten);
					}
					ImGui::SameLine();
					if (ImGui::Checkbox("Can Afford##threaten", &affordsThreaten)) {
						affordanceThreaten->SetCanAfford(affordsThreaten);
					}
					ImGui::Separator();

					//generousOffer
					Affordance* affordanceGenerousOffer = affordanceController->GetAffordance<AffordanceGenerousOffer>();
					bool performGenerousOffer = affordanceGenerousOffer->GetCanPerform();
					bool affordsGenerousOffer = affordanceGenerousOffer->GetCanAfford();
					ImGui::Text("generousOffer");
					if (ImGui::Checkbox("Can Perform##generousOffer", &performGenerousOffer)) {
						affordanceGenerousOffer->SetCanPerform(performGenerousOffer);
					}
					ImGui::SameLine();
					if (ImGui::Checkbox("Can Afford##generousOffer", &affordsGenerousOffer)) {
						affordanceGenerousOffer->SetCanAfford(affordsGenerousOffer);
					}
					ImGui::Separator();
					ImGui::Dummy(ImVec2(0.0f, 20.0f));
				}


				if (dynamic_cast<NPC_GameObject*>(inspectedObject)) {
					DrawNPCInspector();
				}

				changeObject = false;
			}
			else {
				ImGui::CollapsingHeader("No Object Selected", baseFlags);
			}
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Navigation")) {
			
			GaemPathing::PathNodeManager* pathNodeManager = &engine->scene->pathManager;

			ImGui::SeparatorText("Test Path");

			static GaemPathing::PathNode* testStartNode = nullptr;
			static GaemPathing::PathNode* testEndNode = nullptr;


			static std::string startName = "";
			if (testStartNode) {
				startName = std::to_string(testStartNode->GetID());
			}
			else {
				startName = "";
			}

			static std::string endName = "";
			if (testEndNode) {
				endName = std::to_string(testEndNode->GetID());
			}
			else {
				endName = "";
			}
			ImGui::Text("Start Node:");
			if (ImGui::BeginCombo("##startNodeTest", startName.c_str()))
			{
				if (ImGui::Selectable("--None--"))
					testStartNode = nullptr;
				for (auto& node : pathNodeManager->GetNodes())
				{
					if (ImGui::Selectable(std::to_string(node->GetID()).c_str())) {
						testStartNode = node;
					}
				}
				ImGui::EndCombo();
			}

			ImGui::Text("End Node:");
			if (ImGui::BeginCombo("##endNodeTest", endName.c_str()))
			{
				if (ImGui::Selectable("--None--"))
					testEndNode = nullptr;
				for (auto& node : pathNodeManager->GetNodes())
				{
					if (ImGui::Selectable(std::to_string(node->GetID()).c_str())) {
						testEndNode = node;
					}
				}
				ImGui::EndCombo();
			}

			static std::string isValid = "";
			static ImVec4 isValidColor;

			if (ImGui::Button("Test Path"))
			{
				if (testStartNode && testEndNode) {
					std::vector<glm::vec3> path = GaemPathing::FindPathA_StarPositions(testStartNode, testEndNode, pathNodeManager->GetNodes());
					if (path.size() > 1) {

						isValid = "Path is valid";
						isValidColor = { 0.0f, 1.0f, 0.2f, 1.0f };
					}
					else {
						isValid = "Path is invalid";
						isValidColor = { 1.0f, 0.0f, 0.0f, 1.0f };
					}
					pathDebugLine.SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });
					pathDebugLine.SetLine(path);
				}
				else {
					isValid = "Missing start or end node";
					isValidColor = { 1.0f, 0.6f, 0.0f, 1.0f };
				
				}

			}
			ImGui::SameLine();
			if (ImGui::Button("Clear Test Path")) {
				std::vector<glm::vec3> path;
				pathDebugLine.SetLine(path);
				isValid = "";
			}


			ImGui::PushStyleColor(ImGuiCol_Text, isValidColor);
			ImGui::Text(isValid.c_str());
			ImGui::PopStyleColor();

			ImGui::SeparatorText("Create Node");
			static glm::vec3 nNodePos(0.0f);
			static float nNodeSize = 1.0f;
			static bool nIsObstacle = false;
			static float neighbourDist = pathNodeManager->GetMaxConnectionDist();

			if (ImGui::DragFloat("NeighbourDistance##nodeNDist", &neighbourDist, 0.2f, 0.0f, FLT_MAX)) {
				pathNodeManager->SetMaxConnectionDist(neighbourDist);
			}
			ImGui::DragFloat3("position##nnodePos",&nNodePos.x);
			ImGui::DragFloat("size##nnodescale", &nNodeSize);
			ImGui::Checkbox("Obstacle##nnodeObstacle", &nIsObstacle);
			if (ImGui::Button("Add Node##nnodeAdd")) 
			{
				pathNodeManager->AddNode(nNodePos, nIsObstacle)->SetSize({ nNodeSize ,nNodeSize, nNodeSize});
			}

			if (ImGui::Button("Update Nodes##updateNodes")) {
				pathNodeManager->UpdateNodes();
			}

			ImGui::SeparatorText("Nodes");
			int i = 0;
			GaemPathing::PathNode* delNode = nullptr;
			static GaemPathing::PathNode* selectedNode = nullptr;

			if(pathNodeManager->GetNodes().empty())
				selectedNavNodeBox.SetEnabled(false);

			for (auto& node : pathNodeManager->GetNodes()) {
				
				std::string nodeID = std::string("##node") + std::to_string(i);

				bool selectedButton = false;

				if (selectedNode)
					if (node->GetID() == selectedNode->GetID())
						selectedButton = true;

				if (selectedButton) {
					ImGui::PushStyleColor(ImGuiCol_Button, { 0.0,1.0,1.0,1.0 });
					ImGui::PushStyleColor(ImGuiCol_Text, { 0.0,0.0,0.0,1.0 });
				}
				if (ImGui::Button(std::to_string(node->GetID()).c_str(), {30,20}))
				{
					selectedNode = node;

					selectedNavNodeBox.SetEnabled(true);
					selectedNavNodeBox.SetPosition(node->GetPosition());
					selectedNavNodeBox.SetScale(node->GetSize() * 1.1f);
				}
				if (selectedButton) {
					ImGui::PopStyleColor();
					ImGui::PopStyleColor();
				}
				if (ImGui::IsItemHovered())
				{
					ImGui::BeginTooltip();
					ImGui::Text("Node ID");
					ImGui::EndTooltip();
				}

				ImGui::SameLine();
				
				glm::vec3 cPos = node->GetPosition();
				ImGui::PushItemWidth(160);
				if (ImGui::DragFloat3(nodeID.c_str(), &cPos.x)) {
					node->SetPosition(cPos);
					selectedNavNodeBox.SetEnabled(true);
					selectedNavNodeBox.SetPosition(node->GetPosition());
					selectedNavNodeBox.SetScale(node->GetSize() * 1.1f);
					selectedNode = node;
				}
				ImGui::PopItemWidth();

				if (ImGui::IsItemHovered())
				{
					ImGui::BeginTooltip();
					ImGui::Text("node position");
					ImGui::EndTooltip();
				}

				ImGui::SameLine();

				float cScale = node->GetSize().x;
				ImGui::PushItemWidth(60);
				if (ImGui::DragFloat((nodeID + "scaleSet").c_str(), &cScale, 0.1f, 0.0f, FLT_MAX)) {
					node->SetSize({ cScale ,cScale ,cScale });
				}
				ImGui::PopItemWidth();

				if (ImGui::IsItemHovered())
				{
					ImGui::BeginTooltip();
					ImGui::Text("node scale");
					ImGui::EndTooltip();
				}

				ImGui::SameLine();
				bool cObstacle = node->GetObstacle();
				if (ImGui::Checkbox(nodeID.c_str(), &cObstacle)) {
					node->SetObstacle(cObstacle);
				}

				if (ImGui::IsItemHovered())
				{
					ImGui::BeginTooltip();
					ImGui::Text("set obstacle");
					ImGui::EndTooltip();
				}

				ImGui::SameLine();
				if (ImGui::Button(("Delete" + nodeID).c_str())) {
					delNode = node;
				}

				i++;
			}

			if (delNode) {
				pathNodeManager->DeleteNode(delNode);
			}

			ImGui::EndTabItem();
		}


		if (ImGui::BeginTabItem("Debug")) {
			
			DrawDebug();


			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}



	
	guirenderer.EndWindow();
}

void SceneEditor::DrawMenu()
{
	static bool showChangeWindow = false;
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
				delete engine->scene;
				UseScene(new Scene);
				engine->scene->physicsWorld.ResetPhysicsWorld();
				saveFilePath[0] = '\0';
				luaFilePath = ("resources/scripts/main.lua");
				inspectedObject = nullptr;
				lastObject = nullptr;
			}
			if (ImGui::MenuItem("Save", "Ctrl+S")) {
				if (saveFilePath.size() < 1)
				{
					std::string savePath = FileOpener::OpenFileDialogue(SAVE_FILE);
					if (engine->scene && !savePath.empty()) {
						SaveProject(savePath.c_str());
						saveFilePath = savePath;
					}
				}
				else if(engine->scene){
					SaveProject(saveFilePath.c_str());
				}
			}
			if (ImGui::MenuItem("Save As",NULL)) {
				std::string savePath = FileOpener::OpenFileDialogue(SAVE_FILE);
				if (engine->scene) {
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
					SetLuaFile(FilterFilePath(lpath).c_str());
				}
				else {
					DebugLogger::Log(GAEM_ERROR,"Could not load lua file" + lpath);
				}

			}
			ImGui::MenuItem("Window Title",NULL, &showChangeWindow);
			ImGui::MenuItem("Window Icon");

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View")) {
			ImGui::MenuItem("(Toggle Debug Rendering)", NULL, false, false);
			ImGui::MenuItem("Debug Physics", NULL, &isPhysicDebug);
			ImGui::MenuItem("Debug Pathfinding", NULL, &isPathDebug);
			ImGui::MenuItem("Debug Transform Gizmo", NULL, &isShowWidget);

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

			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	float buttonWidth = 80;
	ImGui::SetCursorPosX((viewport->WorkSize.x/2) - (((buttonWidth + ImGui::GetStyle().ItemSpacing.x) * 3)/2));
	

	static ImVec4 baseCol = ImGui::GetStyle().Colors[ImGuiCol_Button];
	static ImVec4 selectedCol = ImGui::GetStyle().Colors[ImGuiCol_Header];
	static ImVec4 pButtonCol = baseCol;

	if (engine->isRunning)
		pButtonCol = selectedCol;
	else
		pButtonCol = baseCol;

	ImGui::PushStyleColor(ImGuiCol_Button,pButtonCol);
	if (ImGui::Button("Play", { buttonWidth,20 })) {
		engine->scene->InitFunction.Execute();
		engine->isRunning = !engine->isRunning;
		input.SetMouseLock(true);
	}
	ImGui::PopStyleColor();


	ImGui::SameLine();
	if (ImGui::Button("Pause", { buttonWidth,20 })) { engine->isRunning = false; }
	ImGui::SameLine();
	if (ImGui::Button("FreeCam", { buttonWidth,20 })) { isFreecam = true; }

	guirenderer.EndWindow();

	DrawWindowSettings(&showChangeWindow);
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
							delteItem = it.first;
						}
						ImGui::NextColumn();
					}
					if (delteItem.compare("") != 0) {
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

				//soundEngine.AddSound(audioName, audioPath);
			}

			int colCount = (viewport->Size.x * windowWidth) / (resourceWidth + (style.ItemSpacing.x * 2));
			ImGui::Columns(colCount, "texCols", false);

			Texture* shaderIcon = res.GetTexture("default");

			////show all audio loaded
			//std::vector<std::string> audioNames = soundEngine.GetAudioNames();
			//
			//for (int i = 0; i < audioNames.size(); i++)
			//{
			//	ImGui::Image((void*)(intptr_t)shaderIcon->ID, ImVec2(resourceWidth, resourceWidth));
			//	ImGui::Text(audioNames[i].c_str());
			//	ImGui::NextColumn();
			//}
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
		Window::GetActiveWindow()->SetName(windowName);
	}

	ImGui::End();
}

void SceneEditor::DrawDebug()
{
	double fps = engine->renderer.GetFPS();
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
	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	ImGui::Text("Console Output:");


	bool bErr = DebugLogger::GetLogLevel(GAEM_ERROR);
	bool bLog = DebugLogger::GetLogLevel(GAEM_LOG);
	bool bDbg = DebugLogger::GetLogLevel(GAEM_DEBUG);
	bool bWrn = DebugLogger::GetLogLevel(GAEM_WARNING);

	static bool lockScroll = true;
	
	if (ImGui::Checkbox("Log Errors ", &bErr)) { DebugLogger::SetLogLevel(GAEM_ERROR, bErr); } ImGui::SameLine();
	if (ImGui::Checkbox("Log Message", &bLog)) { DebugLogger::SetLogLevel(GAEM_LOG,bLog); }
	if (ImGui::Checkbox("Log Debug  ", &bDbg)) { DebugLogger::SetLogLevel(GAEM_DEBUG, bDbg); }ImGui::SameLine();
	if (ImGui::Checkbox("Log Warning", &bWrn)) { DebugLogger::SetLogLevel(GAEM_WARNING, bWrn); }
	ImGui::Checkbox("Auto Scroll", &lockScroll); ImGui::SameLine();
	if (ImGui::Button("Clear##clearConsole")) { _logger->Clear(); }

	ImGui::BeginChild("##ConsoleOutputWindow", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);
	std::string consoleOutput = _logger->streamCapture.GetBuffer();
	ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + ImGui::GetContentRegionAvail().x);
	ImGui::TextUnformatted(consoleOutput.c_str());
	ImGui::PopTextWrapPos();
	if(lockScroll)
		ImGui::SetScrollHereY(1.0f);
	ImGui::EndChild();
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
		if(engine->scene)
			SceneLoader::SaveScene(engine->scene, saveFilePath);
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
	
	if(isShowWidget)
	if (inspectedObject) {
		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist();
		ImGuizmo::SetRect(0, 0, viewport->WorkSize.x, viewport->WorkSize.y);

		glm::mat4 inspectTrans = inspectedObject->GetTransformMatrix();

		if(camera)
		if (ImGuizmo::Manipulate(glm::value_ptr(camera->GetView()), glm::value_ptr(camera->GetProjection()),
			ImGuizmo::OPERATION::TRANSLATE | ImGuizmo::OPERATION::ROTATE, ImGuizmo::MODE::LOCAL, glm::value_ptr(inspectTrans))) {
			glm::vec3 position = glm::vec3(inspectTrans[3]);
			glm::quat rotation = glm::quat(inspectTrans);
			inspectedObject->SetPosition(position);
			inspectedObject->SetRotation(inspectTrans);
		}
	
	}

	guirenderer.EndWindow();
}

void SceneEditor::DrawNPCInspector()
{
	if (!inspectedObject)return;
	NPC_GameObject* inspectedNPC = static_cast<NPC_GameObject*>(inspectedObject);
	if (!inspectedNPC) return;


	if (ImGui::CollapsingHeader("-- Lua Function --")) {


		std::vector<std::string> funcs = LuaManager::GetFunctionNames(luaFilePath);
		std::string funcName = inspectedNPC->GetUpdateFunction().GetName();

		ImGui::Text("Object Update function:");
		if (ImGui::BeginCombo("##functionSelector", funcName.c_str()))
		{
			if (ImGui::Selectable("--None--")) {
				inspectedNPC->SetUpdateFunction(LuaFunction<void, NPC_GameObject&, float>());
			}

			for (auto& funcName : funcs)
			{
				if (ImGui::Selectable(funcName.c_str())) {
					inspectedNPC->SetUpdateFunction(LuaFunction<void, NPC_GameObject&, float>(funcName.c_str(), &engine->scene->luaState));
				}
			}
			ImGui::EndCombo();
		}
		ImGui::Dummy(ImVec2(0.0f, 20.0f));
	}

	//NPC Path Finding settings
	if (ImGui::CollapsingHeader("-- Path Finding --")) {
		
		ImGui::SeparatorText("Move settings");
		float tmpSpeed = inspectedNPC->GetMoveSpeed();
		ImGui::Text("Move Speed:");
		if (ImGui::DragFloat("##npcmvSpeed", &tmpSpeed,0.1f,0.0f, FLT_MAX)) {
			inspectedNPC->SetMoveSpeed(tmpSpeed);
		}

		bool tmpisMoving = inspectedNPC->GetIsMoving();
		if (ImGui::Checkbox("Pathing active",&tmpisMoving)) {
			inspectedNPC->SetIsMoving(tmpisMoving);
		}

		ImGui::SeparatorText("Path Selection");


		GaemPathing::PathNodeManager* pathNodeManager = &engine->scene->pathManager;
		std::string targetNodeName = "node ";

		if (inspectedNPC->GetTargetNode()) {
			targetNodeName += std::to_string(inspectedNPC->GetTargetNode()->GetID());
		}
			 

		ImGui::Text("Target Node:");
		if (ImGui::BeginCombo("##targetNode", targetNodeName.c_str()))
		{
			if (ImGui::Selectable("--None--"))
				inspectedNPC->CancelPath();
			for (auto& node : pathNodeManager->GetNodes())
			{
				if (ImGui::Selectable(std::to_string(node->GetID()).c_str())) {
					inspectedNPC->MoveToPoint(node);
				}
			}
			ImGui::EndCombo();
		}

		ImGui::Text("Current Node:");
		std::string currentNodeName = "node: ";
		if (inspectedNPC->GetCurrentNode()) {
			ImGui::Text((currentNodeName + std::to_string(inspectedNPC->GetCurrentNode()->GetID())).c_str());
		}
		
		ImGui::Text("Next Node:");
		std::string nextNodeName = "node: ";
		if (inspectedNPC->GetNextNode()) {
			ImGui::Text((nextNodeName + std::to_string(inspectedNPC->GetNextNode()->GetID())).c_str());
		}

		ImGui::Dummy(ImVec2(0.0f, 20.0f));
	}

	//NPC Emotion Settings
	if (ImGui::CollapsingHeader("-- Emotion --")) {
		Personality personality = inspectedNPC->GetPersonality();
		float openness = personality.GetOpenness();
		float conscientiousness = personality.GetConscientiousness();
		float extraversion = personality.GetExtraversion();
		float agreeableness = personality.GetAgreeableness();
		float neuroticsim = personality.GetNeuroticism();

		ImGui::DragFloat("Openness", &openness, 2.0f, 0.0f, 120.0f);
		ImGui::DragFloat("Conscientiousness", &conscientiousness, 2.0f, 0.0f, 120.0f);
		ImGui::DragFloat("Extraversion", &extraversion, 2.0f, 0.0f, 120.0f);
		ImGui::DragFloat("Agreeableness", &agreeableness, 2.0f, 0.0f, 120.0f);
		ImGui::DragFloat("Neuroticsim", &neuroticsim, 2.0f, 0.0f, 120.0f);

		personality.SetOpenness(openness);
		personality.SetConscientiousness(conscientiousness);
		personality.SetExtraversion(extraversion);
		personality.SetAgreeableness(agreeableness);
		personality.SetNeuroticism(neuroticsim);

		inspectedNPC->SetPersonality(personality);
	}
}

void SceneEditor::CameraControl(double deltaTime)
{
	InputManager& input = InputManager::Get();

	static bool toggleCamPress = true;//If escape key is down

	if (input.GetKeyPressedDown(GLFW_KEY_ESCAPE) && !toggleCamPress) {
		toggleCamPress = true;
		isFreecam = !isFreecam;
		lastX = input.GetMouseX();
		lastY = input.GetMouseY();
		
	}
	else if (input.GetKeyPressedDown(GLFW_KEY_ESCAPE)) {
		toggleCamPress = true;
	}
	else {
		toggleCamPress = false;
	}

	if (!isFreecam || !camera) {
		input.SetMouseLock(false);
		return;
	}
	input.SetMouseLock(true);
	

	float baseSpeed = 0.05;
	float camSpeed = baseSpeed;
	if (input.GetKeyPressed(GLFW_KEY_LEFT_SHIFT)) {
		camSpeed *= 10;
	}
	if (input.GetKeyPressed(GLFW_KEY_W)) {
		camera->position += glm::normalize(glm::cross({ 0,1,0 }, camera->right)) * camSpeed;
	}
	if (input.GetKeyPressed(GLFW_KEY_S)) {
		camera->position -= glm::normalize(glm::cross({ 0,1,0 }, camera->right)) * camSpeed;
	}
	if (input.GetKeyPressed(GLFW_KEY_A)) {
		camera->position -= camera->right * camSpeed;
	}
	if (input.GetKeyPressed(GLFW_KEY_D)) {
		camera->position += camera->right * camSpeed;
	}

	if (input.GetKeyPressed(GLFW_KEY_SPACE)) {
		camera->position += glm::vec3(0,1,0) * camSpeed;
	}
	if (input.GetKeyPressed(GLFW_KEY_LEFT_CONTROL)) {
		camera->position -= glm::vec3(0, 1, 0) * camSpeed;
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

	camera->Yaw = camera->Yaw - xoffset;
	camera->Pitch = camera->Pitch - yoffset;

	if (camera->Pitch > 85.0f)
		camera->Pitch = 85.0f;
	if (camera->Pitch < -85.0f)
		camera->Pitch = -85.0f;

	camera->UpdateCameraVectors();

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
			if (saveFilePath.size() > 0 && engine->scene) {
				SaveProject(saveFilePath.c_str());
			}
			else if(engine->scene){
				saveFilePath = "untitled_Save.json";
				SaveProject(saveFilePath.c_str());
			}
		saveDown = true;
	}

	if (engine->isRunning && input.GetKeyPressedDown(GLFW_KEY_F5)) {
		engine->isRunning = false;
		input.SetMouseLock(false);
	}

}

void SceneEditor::SetLuaFile(std::string nluaFile)
{
	luaFilePath = nluaFile;
	
	//lua
	LuaGameBridge::ExposeEngine(&engine->scene->luaState);
	engine->scene->luaState.Expose_CPPReference("scene", *engine->scene);
	engine->scene->luaState.Expose_CPPReference("GUI", engine->guiRenderer);
	engine->scene->luaState.Expose_CPPReference("physics", engine->scene->physicsWorld);

	engine->scene->luaState.LoadScript(nluaFile);
	engine->scene->UpdateFunction = engine->scene->luaState.GetFunction<void, double>("update");
	engine->scene->InitFunction = engine->scene->luaState.GetFunction<void>("init");
	
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