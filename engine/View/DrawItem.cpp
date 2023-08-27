#include "DrawItem.h"


#include "DrawItem.h"

void DrawItem::Render(glm::mat4 projection, glm::mat4 view, Shader* shader, bool isElements, unsigned int primative)
{
}

void DrawItem::Update(float deltaTime)
{
}

void DrawItem::SetDiffuseTexture(Texture* nTexture) {
	material.diffuseTexture.push_back(nTexture);
}
void DrawItem::SetSpecularTexture(Texture* nTexture) {
	material.specularMap.push_back(nTexture);
}
void DrawItem::SetEmissionTexture(Texture* nTexture) {
	material.emissionMap.push_back(nTexture);
}

void DrawItem::SetDiffuseTexture(const char* fileName) {
	material.diffuseTexture.push_back(new Texture(fileName));
}

void DrawItem::SetSpecularTexture(const char* fileName) {
	material.specularMap.push_back(new Texture(fileName));
}

void DrawItem::SetEmissionTexture(const char* fileName) {
	material.emissionMap.push_back(new Texture(fileName));
}

Texture* DrawItem::GetDiffuseTexture(int index)  { 
	if (material.diffuseTexture.size() <= 0)
		return nullptr;
	return material.diffuseTexture[index]; 
}
Texture* DrawItem::GetSpecularTexture(int index) {
	if (material.specularMap.size() <= 0)
		return nullptr;
	return material.specularMap	 [index];
}
Texture* DrawItem::GetEmissionTexture(int index) {
	if (material.emissionMap.size() <= 0)
		return nullptr;
	return material.emissionMap	 [index];
}

void DrawItem::SetDiffuseTexture(int index, Texture* nTexture)
{
	if (material.diffuseTexture.empty())
		material.diffuseTexture.push_back(nTexture);
	if (!nTexture || material.diffuseTexture.size() <= index || index < 0)
		return;
	material.diffuseTexture[index] = nTexture;
}

void DrawItem::SetSpecularTexture(int index, Texture* nTexture)
{
	if (material.specularMap.empty())
		material.specularMap.push_back(nTexture);
	if (!nTexture || material.specularMap.size() <= index || index < 0)
		return;
	material.specularMap[index] = nTexture;
}

void DrawItem::SetEmissionTexture(int index, Texture* nTexture)
{
	if (material.emissionMap.empty())
		material.emissionMap.push_back(nTexture);
	if (!nTexture || material.emissionMap.size() <= index || index < 0)
		return;
	material.emissionMap[index] = nTexture;
}

void DrawItem::ResetDiffuseTexture()
{
	material.diffuseTexture.clear();
}

void DrawItem::ResetSpecularTexture()
{
	material.specularMap.clear();
}

void DrawItem::ResetEmissionTexture()
{
	material.emissionMap.clear();
}

void DrawItem::SetShine(float nShine)
{
	material.shine = nShine;
}

void DrawItem::SetAnimation(const std::string& animName, int start, int end, float speed)
{
}

void DrawItem::Animate(const std::string& animation)
{
}

void DrawItem::SetAnimationSpeed(float speed)
{
}

void DrawItem::BindMaterial(Shader* shader)
{
	unsigned int curTexture = GL_TEXTURE1;
	int curIndex = 1;
	int diff = 1;
	int spec = 1;
	int emis = 1;

	diff = curIndex;
	if (!material.diffuseTexture.empty()) {

		for (int i = 0; i < material.diffuseTexture.size(); i++)
		{
			material.diffuseTexture[i]->Bind(curTexture);
			curIndex++;
			curTexture++;
		}
	}
	else {
		glActiveTexture(curTexture);
		glBindTexture(GL_TEXTURE_2D, 0);
		curIndex++;
		curTexture++;
	}

	spec = curIndex;
	if (!material.specularMap.empty()) {

		for (int i = 0; i < material.specularMap.size(); i++)
		{
			material.specularMap[i]->Bind(curTexture);
			curIndex++;
			curTexture++;
		}
	}
	else {
		glActiveTexture(curTexture);
		glBindTexture(GL_TEXTURE_2D, 0);
		curIndex++;
		curTexture++;
	}

	emis = curIndex;
	if (!material.emissionMap.empty()) {

		for (int i = 0; i < material.emissionMap.size(); i++)
		{
			material.emissionMap[i]->Bind(curTexture);
			curIndex++;
			curTexture++;
		}
	}
	else {
		glActiveTexture(curTexture);
		glBindTexture(GL_TEXTURE_2D, 0);
		curIndex++;
		curTexture++;
	}

	//Set texure unit numbers
	shader->SetUniform("material.diffuseTexture", diff);
	shader->SetUniform("material.specularMap", spec);
	shader->SetUniform("material.emissionMap", emis);
	shader->SetUniform("material.alpha", material.shine);
}
