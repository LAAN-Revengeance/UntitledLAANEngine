#pragma once
#include<glad/glad.h>
#include<iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

class Shader{
public:
	Shader();
	Shader(const char* vertShader, const char* fragShader, const char* geomShader);
	~Shader();
	Shader(const Shader& rhs);

	void Use();

	//uniform modifiers:
	void SetUniform(const std::string &uName, float uValue);
	void SetUniform(const std::string& uName, std::vector<float> uValue);
	void SetUniform(const std::string& uName, float v1, float v2);
	void SetUniform(const std::string& uName, glm::vec2 uValue);
	void SetUniform(const std::string &uName, float v1, float v2, float v3);
	void SetUniform(const std::string &uName, glm::vec3 uValue);
	void SetUniform(const std::string &uName, float v1, float v2, float v3, float v4);
	void SetUniform(const std::string &uName, glm::vec4 uValue);
	void SetUniform(const std::string &uName, int uValue);
	void SetUniform(const std::string& uName, std::vector<int> uValue);
	void SetUniform(const std::string &uName, bool uValue);
	void SetUniform(const std::string &uName, glm::mat4 &uValue);
	
	std::string name;

private:

	unsigned int CreateVertexShader(const char* filePath);
	unsigned int CreateFragmentShader(const char* filePath);
	unsigned int CreateGeometryShader(const char* filePath);

	std::string ReadFile(const char* fileName);
	unsigned int ID;
};