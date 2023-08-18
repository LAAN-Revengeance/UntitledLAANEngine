#include "SceneEditor.h"



SceneEditor::SceneEditor()
{
	camera.farPlane = 10000.0f;
}

SceneEditor::~SceneEditor()
{
}

SceneEditor& SceneEditor::Get()
{
	static SceneEditor s_instance;
	return s_instance;
}

void SceneEditor::Draw()
{
	r.StartGUI();

	DrawInspector();
	DrawHeighrarchy();
	ImGui::ShowDemoWindow();
	r.EndGUI();
}

void SceneEditor::Update(double deltaTime)
{
	CameraControl(deltaTime);
}

void SceneEditor::UseScene(Scene* nscene)
{
	if (nscene) {
		scene = nscene;
	}
}

void SceneEditor::DrawHeighrarchy()
{
	float align = 0.0;
	static ImGuiTreeNodeFlags baseFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

	r.StartWindow("Scene Objects", true, 0.2, 0.8, 0.0, 0.0);

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
	ImGui::SeparatorText("Lights");
	ResourceManager& res = ResourceManager::Get();
	int deleteIndex = -1;
	for (int i = 0; i < scene->lights.direction.size(); ++i)
	{
		if (ImGui::Button(std::string("x##" + std::to_string(i)).c_str())) {
			deleteIndex = i;
		}
		ImGui::SameLine();

		if (ImGui::ColorEdit3(std::string("Dir Light##" + std::to_string(i)).c_str(), (float*)&scene->lights.direction[i].diffuse)) {
			
			for (auto& shader : res.shaders) {
				Renderer::Get().SetLightUniforms(scene->lights, *shader.second);
			}
		};
	}
	if (deleteIndex >= 0) {
		scene->lights.direction.erase(scene->lights.direction.begin() + deleteIndex);
	}

	ImGui::End();

}

void SceneEditor::DrawInspector()
{
	
	r.StartWindow("Inspector",true,0.2,0.8,0.8,0.0);

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

void SceneEditor::CameraControl(double deltaTime)
{
	static bool toggleCamPress = false;
	InputManager& input = InputManager::Get();
	bool lock = input.GetMouseLock();

	if (input.GetKeyPressedDown(GLFW_KEY_ESCAPE) && !toggleCamPress) {
		toggleCamPress = true;
		lock = !input.GetMouseLock();
		input.SetMouseLock(lock);
	}
	else if (input.GetKeyPressedDown(GLFW_KEY_ESCAPE)) {
		toggleCamPress = true;
	}
	else {
		toggleCamPress = false;
	}
	if (lock)
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
