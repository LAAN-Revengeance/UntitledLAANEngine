#include "Shader.h"

Shader::Shader() {
	ID = 0;
}

Shader::Shader(const char *vertShader, const char *fragShader, const char* geomShader) {
	
	unsigned int vertID = 0;
	unsigned int fragID = 0;
	unsigned int geomID = 0;

	//create shader program
	int success;
	ID = glCreateProgram();

	if (strlen(vertShader) > 0) {
		vertID = CreateVertexShader(vertShader);
		glAttachShader(ID, vertID);
	}
		
	if (strlen(fragShader) > 0) {
		unsigned int fragID = CreateFragmentShader(fragShader); 
		glAttachShader(ID, fragID);
	}
	
	if (strlen(geomShader) > 0) {
		unsigned int geomID = CreateGeometryShader(geomShader);
		glAttachShader(ID, geomID);
	}

	glLinkProgram(ID);

	//check if link is a success
	char infoLog[512];
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) 
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "FAILED TO LINK SHADER PROGRAM:\n" << infoLog << std::endl;
	}

	//already linked so can delete
	if (vertShader)
		glDeleteShader(vertID);
	if (fragShader)
		glDeleteShader(fragID);
	if (geomShader)
		glDeleteShader(geomID);
}

Shader::~Shader() {
	if (ID != 0)
		glDeleteShader(ID);
}

Shader::Shader(const Shader& rhs) {
	ID = rhs.ID;
}

void Shader::Use() {
	glUseProgram(ID);
}

//--UNIFORM FUNCTIONS
//--float
void Shader::SetUniform(const std::string& uName, float uValue){
	Use();
	glUniform1f(glGetUniformLocation(ID,uName.c_str()),uValue);
}
void Shader::SetUniform(const std::string& uName, std::vector<float> uValue) {
	Use();
	glUniform1fv(glGetUniformLocation(ID, uName.c_str()), 10, &uValue[0]);
}
//--vec2
void Shader::SetUniform(const std::string& uName, float v1, float v2) {
	Use();
	glUniform2f(glGetUniformLocation(ID, uName.c_str()), v1, v2);
}
void Shader::SetUniform(const std::string& uName, glm::vec2 uValue) {
	Use();
	glUniform2f(glGetUniformLocation(ID, uName.c_str()), uValue.x, uValue.y);
}
//--vec3
void Shader::SetUniform(const std::string& uName, float v1, float v2, float v3) {
	Use();
	glUniform3f(glGetUniformLocation(ID, uName.c_str()), v1,v2,v3);
}
void Shader::SetUniform(const std::string& uName, glm::vec3 uValue) {
	Use();
	glUniform3f(glGetUniformLocation(ID, uName.c_str()),uValue.x,uValue.y,uValue.z);
}
//--vec4
void Shader::SetUniform(const std::string& uName, float v1, float v2, float v3, float v4) {
	Use();
	glUniform4f(glGetUniformLocation(ID, uName.c_str()), v1, v2, v3,v4);
}
void Shader::SetUniform(const std::string& uName, glm::vec4 uValue) {
	Use();
	glUniform4f(glGetUniformLocation(ID, uName.c_str()), uValue.x, uValue.y, uValue.z,uValue.w);
}
//--integer
void Shader::SetUniform(const std::string& uName, int uValue){
	Use();
	glUniform1i(glGetUniformLocation(ID, uName.c_str()), uValue);
}
void Shader::SetUniform(const std::string& uName, std::vector<int> uValue) {
	Use();
	glUniform1iv(glGetUniformLocation(ID, uName.c_str()), 10, &uValue[0]);
}
//--boolean
void Shader::SetUniform(const std::string& uName, bool uValue){
	Use();
	glUniform1i(glGetUniformLocation(ID, uName.c_str()), uValue);
}
//--glm::mat4
void Shader::SetUniform(const std::string& uName, glm::mat4 &uValue) {
	Use();
	glUniformMatrix4fv(glGetUniformLocation(ID, uName.c_str()), 1,GL_FALSE,glm::value_ptr(uValue));
}


std::string Shader::ReadFile(const char* fileName) {
	
	std::ifstream fileStream;
	std::stringstream buffer;
	std::string shaderString;


	fileStream.open(fileName);
	buffer << fileStream.rdbuf();
	shaderString = buffer.str();

	if(!fileStream)
		std::cout << "SHADER FILE READ ERROR!: COULD NOT READ: " << fileName << std::endl;
	

	return shaderString;
}

unsigned int Shader::CreateVertexShader(const char* filePath)
{
	std::string vString = ReadFile(filePath);
	const char* vertSource = vString.c_str();

	unsigned int vertID;
	vertID = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertID, 1, &vertSource, NULL);
	glCompileShader(vertID);

	//check if compilation is a sucess
	int success;
	char infoLog[512];
	glGetShaderiv(vertID, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertID, 512, NULL, infoLog);
		std::cout << "FAILED TO COMPILE VERTEX SHADER:\n" << infoLog << std::endl;
		return 0;
	}
	return vertID;
}

unsigned int Shader::CreateFragmentShader(const char* filePath) 
{
	std::string fString = ReadFile(filePath);
	const char* fragSource = fString.c_str();

	unsigned int fragID;
	fragID = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragID, 1, &fragSource, NULL);
	glCompileShader(fragID);

	//check if compilation is a sucess
	int success;
	char infoLog[512];

	glGetShaderiv(fragID, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(fragID, 512, NULL, infoLog);
		std::cout << "FAILED TO COMPILE FRAGMENT SHADER:\n" << infoLog << std::endl;
		return 0;
	}
	return fragID;
}

unsigned int Shader::CreateGeometryShader(const char* filePath) 
{
	std::string gString = ReadFile(filePath);
	const char* geoSource = gString.c_str();

	unsigned int geoID;
	geoID = glCreateShader(GL_GEOMETRY_SHADER);

	glShaderSource(geoID, 1, &geoSource, NULL);
	glCompileShader(geoID);

	//check if compilation is a sucess
	int success;
	char infoLog[512];

	glGetShaderiv(geoID, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(geoID, 512, NULL, infoLog);
		std::cout << "FAILED TO COMPILE GEOMETRY SHADER:\n" << infoLog << std::endl;
		return 0;
	}
	return geoID;
}