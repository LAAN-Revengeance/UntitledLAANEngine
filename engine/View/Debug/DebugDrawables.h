#pragma once
#include <Graphics/Graphics.h>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace GaemGizmo {

	class Line
	{
	public:
		Line();
		Line(std::vector<glm::vec3>& positions);
		~Line();

		void SetLine(std::vector<glm::vec3>& positions);
		void Render(glm::mat4 projection, glm::mat4 view, Shader* shader);

	private:
		
		//number of verts in line
		unsigned int _lineSize = 0;
		VAO _vao;

	};


}