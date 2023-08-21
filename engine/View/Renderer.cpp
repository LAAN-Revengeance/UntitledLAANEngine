#include "Renderer.h"

Renderer::Renderer() {}

Renderer::~Renderer() {}

Renderer& Renderer::Get()
{
	static Renderer e_instance;
	return e_instance;
}

void Renderer::Init(GLFWwindow* window) {

	//glad required to access GL functions
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "ERROR: Could not load glad" << std::endl;
		return;
	}

	mainWindow = window;
	mainShader = Shader("resources/shaders/Default.vert", "resources/shaders/Default.frag", "");

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_MULTISAMPLE);

	int	wWidth, wHeight;
	glfwGetWindowSize(window, &wWidth, &wHeight);
	glViewport(0, 0, wWidth, wHeight);

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	////Setup Post Processing resources////
	//Frame Buffer Shader
	postProcessShader = Shader("resources/shaders/screen/screen.vert", "resources/shaders/screen/screen.frag", "");

	//Create a quad to render frame buffer to
	float quadVertices[] = {
		-1.0f,  1.0f,  0.0f,   0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f,   0.0f, 0.0f,
		 1.0f, -1.0f,  0.0f,   1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f,   0.0f, 1.0f,
		 1.0f, -1.0f,  0.0f,   1.0f, 0.0f,
		 1.0f,  1.0f,  0.0f,   1.0f, 1.0f,
	};

	unsigned int VBO;
	glGenVertexArrays(1, &screenQuad);
	glGenBuffers(1, &VBO);
	glBindVertexArray(screenQuad);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	Resize(wWidth, wHeight);
}

//may want to make several "draw queues" to seperate shaders and opacity
void Renderer::Draw(Camera& cam, Scene& scene, double deltaTime) {

	//Bind pre-post processing frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	//fps counter
	double currentFrameTime = glfwGetTime();
	double differenceTime = currentFrameTime - lastTime;
	numFrames++;
	if (differenceTime >= 1.0 / 30.0)
	{
		fps = (1 / differenceTime) * numFrames;
		lastTime = currentFrameTime;
		numFrames = 0;
	}

	//Set render mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (wireFrame)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//caclulate camera view frustum planes
	cam.CreateViewFrustum();
	Frustum& camFrustum = cam.frustum;

	//draw skybox
	if (scene.skybox)
		scene.skybox->Render(&cam);

	for (auto& it : scene.gameObjects) {
		if (it.second) {

			//if not in view, dont render
			if (!it.second->CheckInFrustum(camFrustum))
				continue;
      
			GameObject* obj = it.second;
			//set special uniforms
			obj->SetUniforms();
			obj->shader->SetUniform("wireframe", wireFrame);
			obj->shader->SetUniform("cubemap", 10);
			obj->shader->SetUniform("_Time", (float) glfwGetTime());
			//set model matrix uniforms
			glm::mat4 modelMat(1.0f);
			modelMat = glm::scale(modelMat, obj->scale);
			modelMat = glm::translate(modelMat, obj->position);
		
			//pitch roll and yaw rotationss
			modelMat = glm::rotate(modelMat, glm::radians(obj->rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
			modelMat = glm::rotate(modelMat, glm::radians(obj->rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
			modelMat = glm::rotate(modelMat, glm::radians(obj->rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));

			if (obj->shader) {
				obj->shader->SetUniform("model", modelMat);
				obj->model_data->Render(&cam, obj->shader, true, GL_TRIANGLES);
			}
			else {
				mainShader.SetUniform("model", modelMat);
				obj->model_data->Render(&cam, &mainShader, true, GL_TRIANGLES);
			}

			obj->model_data->Update(deltaTime);
		}
	}

	//Bind the default frame buffer and draw scene with post processing shader
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	postProcessShader.Use();
	glBindVertexArray(screenQuad);
	glDisable(GL_DEPTH_TEST);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureColorBuff);
	glEnable(GL_TEXTURE_2D);
	glDrawArrays(GL_TRIANGLES, 0, 6);

}

void Renderer::SetLightUniforms(Lights& sLights, Shader& sShader) {

	sShader.SetUniform("ambient_Light", sLights.ambient);

	SetPointLightUniforms(sLights, sShader);
	SetDirectionLightUniforms(sLights, sShader);
	SetPointLightUniforms(sLights, sShader);

}

void Renderer::SetPointLightUniforms(Lights& sLights, Shader& sShader) {

	sShader.SetUniform("numPointLights", (int)sLights.point.size());

	for (int i = 0; i < (int)sLights.point.size(); i++)
	{
		char buffer[32];

		sprintf_s(buffer, "pointLights[%i].position", i);
		sShader.SetUniform(buffer, sLights.point[i].position);

		sprintf_s(buffer, "pointLights[%i].diffuse", i);
		sShader.SetUniform(buffer, sLights.point[i].diffuse);

		sprintf_s(buffer, "pointLights[%i].specular", i);
		sShader.SetUniform(buffer, sLights.point[i].specular);

		sprintf_s(buffer, "pointLights[%i].constant", i);
		sShader.SetUniform(buffer, sLights.point[i].constant);

		sprintf_s(buffer, "pointLights[%i].linear", i);
		sShader.SetUniform(buffer, sLights.point[i].linear);

		sprintf_s(buffer, "pointLights[%i].quadratic", i);
		sShader.SetUniform(buffer, sLights.point[i].quadratic);
	}

}

void Renderer::SetDirectionLightUniforms(Lights& sLights, Shader& sShader) {


	sShader.SetUniform("numDirectionLights", (int)sLights.direction.size());

	for (int i = 0; i < sLights.direction.size(); i++)
	{
		char buffer[32];

		sprintf_s(buffer, "directionLights[%i].direction", i);
		sShader.SetUniform(buffer, sLights.direction[i].direction);

		sprintf_s(buffer, "directionLights[%i].diffuse", i);
		sShader.SetUniform(buffer, sLights.direction[i].diffuse);

		sprintf_s(buffer, "directionLights[%i].specular", i);
		sShader.SetUniform(buffer, sLights.direction[i].specular);
	}
}

void Renderer::SetSpotLightUniforms(Lights& sLights, Shader& sShader) {

	sShader.SetUniform("numSpotLights", (int)sLights.spot.size());

	for (int i = 0; i < sLights.spot.size(); i++)
	{
		char buffer[32];

		sprintf_s(buffer, "spotLights[%i].position", i);
		sShader.SetUniform(buffer, sLights.spot[i].position);

		sprintf_s(buffer, "spotLights[%i].direction", i);
		sShader.SetUniform(buffer, sLights.spot[i].direction);

		sprintf_s(buffer, "spotLights[%i].maxAngle", i);
		sShader.SetUniform(buffer, sLights.spot[i].maxAngle);

		sprintf_s(buffer, "spotLights[%i].featherAngle", i);
		sShader.SetUniform(buffer, sLights.spot[i].featherAngle);

		sprintf_s(buffer, "spotLights[%i].diffuse", i);
		sShader.SetUniform(buffer, sLights.spot[i].diffuse);

		sprintf_s(buffer, "spotLights[%i].specular", i);
		sShader.SetUniform(buffer, sLights.spot[i].specular);

		sprintf_s(buffer, "spotLights[%i].constant", i);
		sShader.SetUniform(buffer, sLights.spot[i].constant);

		sprintf_s(buffer, "spotLights[%i].linear", i);
		sShader.SetUniform(buffer, sLights.spot[i].linear);

		sprintf_s(buffer, "spotLights[%i].quadratic", i);
		sShader.SetUniform(buffer, sLights.spot[i].quadratic);
	}

}

Shader& Renderer::GetShader() {
	return mainShader;
}

void Renderer::ToggleWireFrame()
{
	wireFrame = !wireFrame;
}

void Renderer::Resize(int width, int height)
{
	//delete buffers
	glDeleteTextures(1, &textureColorBuff);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteFramebuffers(1, &FBO);

	//create buffers
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	//create frame buffer texture
	glGenTextures(1, &textureColorBuff);
	glBindTexture(GL_TEXTURE_2D, textureColorBuff);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	//attatch texture to frame buffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuff, 0);

	//rbo for depth
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

	//done setting up frame buffer so unbind
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

double Renderer::GetFPS()
{
	return fps;
}