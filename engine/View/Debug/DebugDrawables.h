#pragma once
#include <Graphics/Graphics.h>
#include <glm/glm.hpp>

namespace GaemGizmo {

	//######################################
	//#				Base
	//######################################

	class DebugGizmo {
	public:
		DebugGizmo() {};
		virtual ~DebugGizmo() { };
		virtual void Render(glm::mat4 projection, glm::mat4 view, Shader* shader) = 0;

		void SetColor(glm::vec4 color);
		void SetPosition(glm::vec3 position);
	protected:
		glm::vec4 _color = { 1.0f,0.0f,0.0f,1.0f };
		glm::vec3 _position = {0.0f,0.0f,0.0f};
	};

	//######################################
	//#				LINE
	//######################################

	class Line : public DebugGizmo
	{
	public:
		Line();
		Line(std::vector<glm::vec3>& positions, glm::vec3 position);
		~Line();
		void SetLine(std::vector<glm::vec3>& positions);
		void Render(glm::mat4 projection, glm::mat4 view, Shader* shader);
		void RenderFront(glm::mat4 projection, glm::mat4 view, Shader* shader);
		void SetWidth(float width);

	private:
		
		//number of verts in line
		unsigned int _vertCount = 0;
		float _width = 5;
		VAO _vao;
	};

	//######################################
	//#				BOX
	//######################################

	class Box : public DebugGizmo
	{
	public:
		Box();
		Box(glm::vec3 position, glm::vec3 scale);
		~Box();

		void SetScale(glm::vec3 scale);
		void Render(glm::mat4 projection, glm::mat4 view, Shader* shader);

	private:
		void SetVertexBuffer();

		glm::vec3 _scale = {1,1,1};
		static VAO* _vao;
	};
}