#include "ResourceManager.h"


ResourceManager& ResourceManager::Get() {
	
	static ResourceManager r_instance;
	return r_instance;
}

Material ResourceManager::CreateMaterial(std::string diff, std::string spec, std::string emiss)
{
	Material nMaterial;

	if (diff.size() > 0)
		nMaterial.diffuseTexture.push_back(GetTexture(diff));
	if (spec.size() > 0)
		nMaterial.specularMap.push_back(GetTexture(spec));
	if (emiss.size() > 0)
		nMaterial.emissionMap.push_back(GetTexture(emiss));

	return nMaterial;
}

DrawItem& ResourceManager::GetDrawItemReference(std::string resName)
{
	return *models.at(resName);
}

void ResourceManager::StoreGameObject(GameObject* go)
{
	objects[go->name] = go;
}

void ResourceManager::ClearGameObjects()
{
	for (auto& it : objects) {
		if (!dynamic_cast<Terrain*>(it.second)) {
			delete it.second;
			objects.erase(it.first);
		}
	}
}

ResourceManager::ResourceManager(){

		//default shader
	LoadShader("default","resources/shaders/Default.vert", "resources/shaders/Default.frag", "");
		//default gizmo shader
	LoadShader("line", "resources/shaders/gizmo_debug/line.vert", "resources/shaders/gizmo_debug/line.frag", "");
		//default physics debug
	LoadShader("physics","resources/shaders/Physics_Debug/Physics.vert", "resources/shaders/Physics_Debug/Physics.frag", "");
		//default terrain shader
	LoadShader("terrain", "resources/shaders/Default.vert", "resources/shaders/terrain/Terrain.frag", "");
		//default Water shader
	LoadShader("water","resources/shaders/Water.vert", "resources/shaders/Water.frag", "");
		//default texture
	LoadTexture("default", "resources/textures/default.png");
}

ResourceManager::~ResourceManager(){}


GameObject& ResourceManager::CreateGameObject(std::string objectName, std::string modelName, std::string shaderName, Material material) {
	GameObject* gameObject = new GameObject();
	gameObject->name = objectName;
	gameObject->SetID(IDIndex);
	IDIndex++;

	gameObject->material = material;

	if(!modelName.empty())
		gameObject->model_data = GetModel(modelName);

	if (!shaderName.empty()) {
		gameObject->shader = GetShader(shaderName);
	}
	else if (shaders.find("default") != shaders.end()) {
		gameObject->shader = shaders.at("default");
	}

	if (objects.find(objectName) != objects.end()) {
		delete objects[objectName];
		objects[objectName] = gameObject;
	}
	else {
		objects.insert({ objectName, gameObject });
	}
	return *gameObject;
}

NPC_GameObject& ResourceManager::CreateNPC(std::string objectName, std::string modelName, std::string shaderName, Material material)
{
	NPC_GameObject* gameObject = new NPC_GameObject();
	gameObject->name = objectName;
	gameObject->SetID(IDIndex);
	IDIndex++;

	gameObject->material = material;

	if (!modelName.empty())
		gameObject->model_data = GetModel(modelName);

	if (!shaderName.empty()) {
		gameObject->shader = GetShader(shaderName);
	}
	else if (shaders.find("default") != shaders.end()) {
		gameObject->shader = shaders.at("default");
	}

	if (objects.find(objectName) != objects.end()) {
		delete objects[objectName];
		objects[objectName] = gameObject;
	}
	else {
		objects.insert({ objectName, gameObject });
	}
	return *gameObject;
}


Terrain& ResourceManager::CreateTerrain(std::string terrainName, std::string heightMapName, std::vector<std::string> layerTextures, std::string detailName, std::string specularName, std::string emissiveName, float texScale, float scaleX, float scaleY, float scaleZ) {
	
	Terrain* terrain = new Terrain(textures.at(heightMapName),scaleX,scaleY,scaleZ,texScale);
	
	if(textures.find(emissiveName) != textures.end())
		terrain->material.emissionMap.push_back(GetTexture(emissiveName));
	if(textures.find(specularName) != textures.end())
		terrain->material.specularMap.push_back(GetTexture(specularName));
	
	if (shaders.find("terrain") != shaders.end())
		terrain->shader = shaders.at("terrain");

	std::vector<Texture*> layers;
	for (int i = 0; i < layerTextures.size(); i++)
		layers.emplace_back(textures.at(layerTextures[i]));

	terrain->SetTextures(layers,textures.at(detailName));

	terrain->name = terrainName;
	terrain->model_data->name = terrainName;
	terrain->SetID(IDIndex);
	IDIndex++;

	models.insert({terrainName,terrain->model_data});

	objects.insert({ terrainName, terrain });
	return *terrain;
}

void ResourceManager::LoadTexture(std::string resName, std::string fileName) {
	try
	{
		Texture* nTex = new Texture(fileName.c_str());
		if (!nTex->GetImageData()) {
			delete nTex;
			return;
		}
		nTex->name = resName;
		textures.emplace(resName, nTex);
		texturePaths.emplace(resName, fileName);
	}
	catch (const std::exception&)
	{
		std::cout << "Error: Could not create: " << resName << std::endl;
	}
}

void ResourceManager::LoadAnimatedModel(std::string resName, std::string fileName)
{
	try
	{
		md2_model_t* model = new md2_model_t(fileName.c_str());


		//model
		model->name = resName;
		models.emplace(resName, model);
		modelPaths.emplace(resName, fileName);
	}
	catch (const std::exception&)
	{
		std::cout << "Error: Could not create: " << resName << std::endl;
	}
}

void ResourceManager::LoadModel(std::string resName, std::string fileName) {
	try
	{
		Mesh* model = new Mesh(fileName.c_str());
		model->name = resName;

		//model
		models.emplace(resName, model);
		modelPaths.emplace(resName, fileName);
	}
	catch (const std::exception&)
	{
		std::cout << "Error: Could not create: " << resName << std::endl;
	}
}

void ResourceManager::LoadShader(std::string resName, std::string vertPath, std::string fragPath, std::string geomPath) {
	
	Shader* nshader;

	try
	{
		nshader = new Shader(vertPath.c_str(), fragPath.c_str(), geomPath.c_str());
		if (!nshader->GetIsValid()) {
			delete nshader;
			return;
		}

	}
	catch (const std::exception&)
	{
		std::cout << "Error: Could not create: " << resName << std::endl;
		return;
	}
	nshader->name = resName;
	shaders.emplace(resName, nshader);
	std::array<std::string, 3> paths = { vertPath,fragPath,geomPath };
	shadersPaths.emplace(resName, paths);
}

void ResourceManager::LoadCubemap(std::string resName, std::string right, std::string left, std::string top, std::string bottom, std::string front, std::string back) {
	try
	{
		std::vector<std::string> sides({right,left,top,bottom,front,back});

		CubeMap* nCubemap = new CubeMap(sides);
		nCubemap->name = resName;
		cubemaps.emplace(resName, nCubemap);
		std::array<std::string,6> paths = {right, left, top, bottom, front, back};
		cubemapPaths.emplace(resName, paths);
	}
	catch (const std::exception&)
	{
		std::cout << "Error: Could not create: " << resName << std::endl;
	}
}

Texture* ResourceManager::GetTexture(std::string resName) {
	Texture* texture = nullptr;
	try
	{
		texture = textures.at(resName);
	}
	catch (const std::exception& e)
	{
		std::cout << "ERROR: Texture: '" << resName << "' does not exist: " << e.what() << std::endl;
		texture = textures.at("default");
	}
	return texture;
}

DrawItem* ResourceManager::GetModel(std::string resName) {
	DrawItem* model = nullptr;
	try
	{
		//animated model is a copy.//both copies have the same model data but hold different animation frame states
		//so model data is shared but they are animated seperatley.
		if (dynamic_cast<md2_model_t*>(models.at(resName)) != nullptr) {
			model = new md2_model_t(*dynamic_cast<md2_model_t*>(models.at(resName)));
		}else{
			model = models.at(resName);
		}
	}
	catch (const std::exception& e)
	{
		std::cout << "ERROR: Model: '" << resName << "' does not exist: " << e.what() << std::endl;
		model = nullptr;
	}
	return model;
}

Shader* ResourceManager::GetShader(std::string resName) {
	Shader* shader = nullptr;
	try
	{
		shader = shaders.at(resName);
	}
	catch (const std::exception& e)
	{
		std::cout << "ERROR: Shader: '" << resName << "' does not exist: " << e.what() << std::endl;
		shader = nullptr;
	}
	return shader;
}

CubeMap* ResourceManager::GetCubeMap(std::string resName) {
	CubeMap* cubemap = nullptr;
	try
	{
		cubemap = cubemaps.at(resName);
	}
	catch (const std::exception& e)
	{
		std::cout << "ERROR: Cubemap: '" << resName << "' does not exist: " << e.what() << std::endl;
		cubemap = nullptr;
	}
	return cubemap;

}

GameObject* ResourceManager::GetGameObject(std::string resName)
{
	GameObject* gameObject = nullptr;
	try
	{
		gameObject = objects.at(resName);
	}
	catch (const std::exception& e)
	{
		std::cout << "ERROR: gameObject: '" << resName << "' does not exist: " << e.what() << std::endl;
		gameObject = nullptr;
	}
	return gameObject;
}

void ResourceManager::DeleteGameObject(std::string goName)
{
	delete objects.at(goName);
	objects.at(goName) = nullptr;
	objects.erase(goName);
}
