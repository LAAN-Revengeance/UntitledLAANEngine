#include "Terrain.h"

Terrain::Terrain() : terrainSize(0)
{
	heightArray = new std::vector<float>;
}

Terrain::Terrain(const std::string& nHeightMap, float xScale, float yScale, float zScale, float texScale) {
	textureScale = texScale;
	heightArray = new std::vector<float>;
	scaleX = xScale;
	scaleY = yScale;
	scaleZ = zScale;
	LoadHeightMap(nHeightMap);
}

Terrain::Terrain(Texture* nHeightMap, float xScale, float yScale, float zScale, float texScale) {
	textureScale = texScale;
	heightArray = new std::vector<float>;
	scaleX = xScale;
	scaleY = yScale;
	scaleZ = zScale;
	LoadHeightMap(nHeightMap);
}

Terrain::Terrain(int size, float xScale, float zScale, float texScale)
{
	textureScale = texScale;
	heightArray = new std::vector<float>;
	for (int y = 0; y < size; y++)
	{
		for (int x = 0; x < size; x++)
		{
			(*heightArray).push_back(0.0f);
		}
	}

	terrainSize = size;
	scaleX = xScale;
	scaleY = 0;
	scaleZ = zScale;
	GenerateModel();
}

Terrain::Terrain(std::vector<float> nHeights) {
	LoadHeightMap(nHeights);
}

Terrain::~Terrain()
{
}

float Terrain::GetHeight(float x, float z) {
	
	x = (x / scaleX + (((float)terrainSize - 1.0f) / 2.0f)) * scaleX;
	z = (z / scaleZ + (((float)terrainSize - 1.0f) / 2.0f)) * scaleZ;

	if (x >= (terrainSize - 1.0f) * scaleX || z >= (terrainSize - 1.0f) * scaleZ || x <= 0.0f || z <= 0.0f)
		return 0;

	int xDown = (int)(z / scaleX);
	int zDown = (int)(x / scaleZ);

	int xUp = xDown + 1;
	int zUp = zDown + 1;

	float triZ0 = ((*heightArray)[(xDown * terrainSize) + zDown]);
	float triZ1 = ((*heightArray)[(xUp * terrainSize) + zDown]);
	float triZ2 = ((*heightArray)[(xDown * terrainSize) + zUp]);
	float triZ3 = ((*heightArray)[(xUp * terrainSize) + zUp]);

	float height = 0.0f;
	float sqX = (z / scaleX) - xDown;
	float sqZ = (x / scaleZ) - zDown;
	if ((sqX + sqZ) < 1)
	{
		height = triZ0;
		height += (triZ1 - triZ0) * sqX;
		height += (triZ2 - triZ0) * sqZ;
	}
	else
	{
		height = triZ3;
		height += (triZ1 - triZ3) * (1.0f - sqZ);
		height += (triZ2 - triZ3) * (1.0f - sqX);
	}
	return height - ((maxHeight + minHeight) / 2.0f);
}

glm::vec3 Terrain::GetNormal(float x, float z)
{
	x = (x / scaleX + (((float)terrainSize - 1) / 2.0f)) * scaleX;
	z = (z / scaleZ + (((float)terrainSize - 1) / 2.0f)) * scaleZ;

	if (x >= (terrainSize - 1) * scaleX || z >= (terrainSize - 1) * scaleZ || x < 0 || z < 0)
		return glm::vec3(0,1,0);

	int xDown = (int)(z / scaleX);
	int zDown = (int)(x / scaleZ);

	int xUp = xDown + 1;
	int zUp = zDown + 1;

	float h00 = ((*heightArray)[(xDown * terrainSize) + zDown]);
	float h01 = ((*heightArray)[(xUp * terrainSize) + zDown]);
	float h10 = ((*heightArray)[(xDown * terrainSize) + zUp]);
	float h11 = ((*heightArray)[(xUp * terrainSize) + zUp]);

	// Calculate the vectors from (x, z) to the four corners of the quad.
	glm::vec3 v00(x, h00, z);
	glm::vec3 v01(x + 1, h01, z);
	glm::vec3 v10(x, h10, z + 1);
	glm::vec3 v11(x + 1, h11, z + 1);

	//Calculate the normal vectors for the two triangles in the quad.
	glm::vec3 n1 = glm::normalize(glm::cross(v10 - v00, v01 - v00));
	glm::vec3 n2 = glm::normalize(glm::cross(v01 - v11, v10 - v11));

	//Normalize the resulting vector
	glm::vec3 normal = glm::normalize(n1 + n2);

	return normal;
}

int Terrain::GetSize() {
	return terrainSize;
}

float Terrain::GetMaxHeight() { return maxHeight; }
float Terrain::GetMinHeight() { return minHeight; }

std::vector<float>* Terrain::GetHeightArray()
{
	return heightArray;
}

void Terrain::SetTextures(std::vector<Texture*> textures, Texture* detailMap) {

	if (textures.size() > MAX_TERRAIN_TEXTURES) { 
		std::cout << "ERROR: Max terrain textures is: " << MAX_TERRAIN_TEXTURES << std::endl; 
		return;
	}
		
	int i;
	for (i = 0; i < textures.size(); i++) {
		model_data->SetDiffuseTexture(textures[i]);
		tUnits.push_back(i);
		tCount++;
	}

	model_data->SetDiffuseTexture(detailMap);
	tUnits.push_back(i);

	//need to use all texture units in shader or GLSL will only use one texture
	for (i = tCount; i < MAX_TERRAIN_TEXTURES; i++)
		tUnits.push_back(0);

	SetUniforms();
}

void Terrain::SetMaterailTextures(std::vector<Texture*> textures) {

	if (textures.size() > MAX_TERRAIN_TEXTURES) {
		std::cout << "ERROR: Max terrain textures is: " << MAX_TERRAIN_TEXTURES << std::endl;
		return;
	}

	int i;
	for (i = 0; i < textures.size(); i++) {
		model_data->SetDiffuseTexture(textures[i]);
		tUnits.push_back(i);
		tCount++;
	}

	//need to use all texture units in shader or GLSL will only use one texture
	for (i = tCount; i < MAX_TERRAIN_TEXTURES; i++)
		tUnits.push_back(0);

	SetUniforms();
}



void Terrain::SetTextureHeights(std::vector<float> heights) {

	std::vector<float> nHeights;

	nHeights.push_back(heights[0]);
	for (int i = 0; i < heights.size(); i++)
	{
		nHeights.push_back(heights[i]);
	}

	shader->SetUniform("heights", nHeights);
	shader->SetUniform("heightCount", (int)nHeights.size());
}


void Terrain::SetTextureScale(float nScale) {
	if (nScale <= 0)
		return;

	textureScale = nScale;

	if ((*heightArray).size() > 0)
		GenerateModel();

}

void Terrain::SetHeightTexture(Texture* nHeightTex)
{
	heightTexture = nHeightTex;
}

void Terrain::SetUniforms()
{
	if (shader) {
		shader->SetUniform("textures", tUnits);
		shader->SetUniform("textureCount", tCount);
	}
}

void Terrain::LoadHeightMap(const std::string& fileName) {

	heightTexture = new Texture(fileName.c_str());
	CreateHeightArray();
	GenerateModel();
}

void Terrain::LoadHeightMap(Texture* nHeightMap) {
	
	heightTexture = nHeightMap;
	CreateHeightArray();
	GenerateModel();
}

void Terrain::LoadHeightMap(std::vector<float> nHeights) {
	*heightArray = nHeights;
	terrainSize = (int)sqrt(nHeights.size());
	GenerateModel();
}

Texture* Terrain::GetHeightTexture()
{
	return heightTexture;
}

void Terrain::CreateHeightArray() {

	
	terrainSize = 0;

	if (!heightTexture)
		return;
	if(!(*heightArray).empty())
		(*heightArray).clear();

	int tWidth = heightTexture->GetWidth();
	int tHeight = heightTexture->GetHeight();

	if (tWidth != tHeight)
		return;

	terrainSize = tWidth;


	for (int y = 0; y < tHeight; y++)
	{
		for (int x = 0; x < tWidth; x++)
		{
			float height = scaleY * (heightTexture->GetPixelValue(x, y, 0));
			(*heightArray).push_back(height);
			if (height > maxHeight)
				maxHeight = height;
			if (height < minHeight)
				minHeight = height;
		}
	}
}

void Terrain::GenerateModel() {
	if ((*heightArray).empty()) {
		model_data = new Model();
		return;
	}

	std::vector<vertex> vertexData;
	std::vector<glm::uvec3> elementsIndexes;
	std::vector<glm::vec3> faceNorms;

	vertexData.reserve(terrainSize * terrainSize);
	elementsIndexes.reserve((terrainSize - 1) * (terrainSize - 1) * 2);
	faceNorms.reserve((terrainSize - 1) * (terrainSize - 1) * 2);

	float heightOffset = ((maxHeight + minHeight)/2) + 1;
	float xzOffset = (terrainSize - 1) / 2.0f;
	//create vert data
	float texCoordScaleX = (float)textureScale / (float)terrainSize;
	float texCoordScaleY = (float)textureScale / (float)terrainSize;
	
	for (int y = 0; y < terrainSize; y++)
	{
		for (int x = 0; x < terrainSize; x++)
		{
			vertex nVert;
			nVert.normal = { 0,0,0 };
			nVert.texCoord = { x * texCoordScaleX, y * texCoordScaleY };
			nVert.vertex.x = (x - xzOffset) * scaleX;
			nVert.vertex.z = (y - xzOffset) * scaleZ;
			nVert.vertex.y = ((*heightArray)[x + (y * terrainSize)]) - heightOffset;

			vertexData.emplace_back(nVert);
		}
	}

	//create vert indexes for EBO
	for (int y = 0; y < terrainSize - 1; y++)
	{
		for (int x = 0; x < terrainSize - 1; x++)
		{
			glm::uvec3 nIndex;

			nIndex.x = (y * terrainSize) + x + terrainSize;
			nIndex.y = (y * terrainSize) + x + 1;
			nIndex.z = (y * terrainSize) + x;
			elementsIndexes.emplace_back(nIndex);

			nIndex.x = (y * terrainSize) + x + terrainSize;
			nIndex.y = (y * terrainSize) + x + terrainSize + 1;
			nIndex.z =  (y * terrainSize) + x + 1;
			elementsIndexes.emplace_back(nIndex);
		}
	}

	//create norms for faces
	for (int i = 0; i < elementsIndexes.size(); i++)
	{
		glm::vec3 nNorm;
		glm::vec3 e1 = vertexData[elementsIndexes[i].x].vertex - vertexData[elementsIndexes[i].y].vertex;
		glm::vec3 e2 = vertexData[elementsIndexes[i].x].vertex - vertexData[elementsIndexes[i].z].vertex;

		nNorm = glm::cross(e1, e2);
		faceNorms.emplace_back(nNorm);
	}

	//assign vertecies norms
	for (int i = 0; i < elementsIndexes.size(); i++)
	{
		glm::ivec3 curFace = elementsIndexes[i];
		vertexData[curFace.x].normal += faceNorms[i];
		vertexData[curFace.y].normal += faceNorms[i];
		vertexData[curFace.z].normal += faceNorms[i];
	}
	for (int i = 0; i < vertexData.size(); i++)
	{
		vertexData[i].normal = glm::normalize(vertexData[i].normal);
	}

	if ((Model*)model_data) {
		((Model*)model_data)->FreeData();
	}
	else {
		model_data = new Model();
	}
	model_data->maxBounds = (terrainSize / 2.0f) * scaleX;
	((Model*)model_data)->SetVertexData(&vertexData[0].vertex.x, vertexData.size(), &elementsIndexes[0].x, elementsIndexes.size() * 3);
}	
