#include "OBJReader.h"

OBJData *ReadObjFile(const char* fileName) {
	
	std::vector<glm::vec3>  vertCoords;
	std::vector<glm::vec3>  vertNorms;
	std::vector<glm::vec2>  texCoords;
	std::vector<glm::ivec3> indexes;

	float maxDist = 0.0f;

	OBJData* objData = new OBJData();
	std::unordered_map<glm::ivec3, int> elementMap;//track whats in indexes, stops needing linear search

	//read in data to vectors
	std::fstream file;
	file.open(fileName, std::ios::in);
	if (!file) {
		std::cout << "ERROR: COULD NOT READ FILE: " << fileName << std::endl;
		return objData;
	}

	int indexNum = 0;
	while (file.good()) {

		char dt[1024];

		file >> dt;
		//read in the specific type of data on that line
		if (strcmp(dt, "v") == 0) {
			glm::vec3 data;
			file >> data.x >> data.y >> data.z;
			vertCoords.push_back(data);

			float ndist = glm::length(data);
			if (ndist > maxDist)
				maxDist = ndist;

		}
		else if (strcmp(dt, "vt") == 0) {
			glm::vec2 data;
			file >> data.x >> data.y;
			texCoords.push_back(data);
		}
		else if (strcmp(dt, "vn") == 0) {
			glm::vec3 data;
			file >> data.x >> data.y >> data.z;
			vertNorms.push_back(data);
		}
		else if (strcmp(dt, "f") == 0) {
			glm::ivec3 data;
			char c;

			for (int i = 0; i < 3; i++) {
				file >> data.x >> c >> data.y >> c >> data.z;

				if (elementMap.find(data) != elementMap.end())//if vert already exists
				{
					objData->elements.push_back(elementMap.at(data));
				}
				else {
					elementMap.insert(std::pair<glm::ivec3, int>(data, indexNum));
					objData->elements.push_back(indexNum);
					indexes.push_back(data);
					indexNum++;
				}
			}
		}

	}

	for (size_t i = 0; i < indexes.size(); i++)
	{
		vertex nVert;
		nVert.vertex = vertCoords[indexes[i].x - 1];
		nVert.texCoord = texCoords[indexes[i].y - 1];
		nVert.normal = vertNorms[indexes[i].z - 1];

		objData->vertexData.push_back(nVert);
	}
	
	objData->boundingShpere = maxDist;
	return objData;
}