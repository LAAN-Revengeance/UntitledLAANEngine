#include "DrawItem.h"


#include "DrawItem.h"

void DrawItem::Render(Camera* camera, Shader* shader, bool isElements, unsigned int primative)
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

Texture* DrawItem::GetDiffuseTexture(int index)  { return material.diffuseTexture[index]; }
Texture* DrawItem::GetSpecularTexture(int index) { return material.specularMap	 [index]; }
Texture* DrawItem::GetEmissionTexture(int index) { return material.emissionMap	 [index]; }

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
	unsigned int curTexture = GL_TEXTURE0;
	int curIndex = 0;
	int diff = 0;
	int spec = 0;
	int emis = 0;

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
