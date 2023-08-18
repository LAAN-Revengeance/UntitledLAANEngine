#include "SceneEditor.h"



SceneEditor::SceneEditor()
{
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

	r.EndGUI();
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

	ImGui::Begin("Scene Objects");

	int i = 0;
	static int selectedNode = -1;

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
	ImGui::ShowDemoWindow();
	ImGui::End();

}

void SceneEditor::DrawInspector()
{
	ImGui::Begin("Inspector");

	static ImGuiTreeNodeFlags baseFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Leaf;

	if (inspectedObject) {
		ImGui::CollapsingHeader(inspectedObject->name.c_str(), baseFlags);

		//TRANSFORM SETTINGS
		ImGui::SeparatorText("Transform");
		float tmpX = inspectedObject->position.x;
		float tmpY = inspectedObject->position.y;
		float tmpZ = inspectedObject->position.z;
		ImGui::DragFloat("x position", &tmpX, 0.1f);
		ImGui::DragFloat("y position", &tmpY, 0.1f);
		ImGui::DragFloat("z position", &tmpZ, 0.1f);

		if (tmpX != inspectedObject->position.x || tmpY != inspectedObject->position.y || tmpZ != inspectedObject->position.z) {
			inspectedObject->SetPosition({ tmpX, tmpY, tmpZ });
		}

		//PHYSICS SETTINGS
		ImGui::SeparatorText("Physics");
	}

	ImGui::End();
}