#include "Renderer.h"

void Renderer::BindMaterial(Material& material, Shader* shader)
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

Renderer::Renderer(Window* window)
{
	Init(window);
}

Renderer::~Renderer() {}

void Renderer::InitShadowsMaps()
{
	dirFBO;

	glGenFramebuffers(1, &dirFBO);

	//generate shadow map textures
	glGenTextures(1, &dirDepthMap);
	glBindTexture(GL_TEXTURE_2D, dirDepthMap);
	glTexImage2D(GL_TEXTURE_2D,0, GL_DEPTH_COMPONENT,shadowWidth,shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderCol[] = {1.0f,1.0f,1.0f,1.0f};
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderCol);
	glBindTexture(GL_TEXTURE_2D, 0);

	//bind the shadow map to our fbo as its depth component
	glBindFramebuffer(GL_FRAMEBUFFER, dirFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, dirDepthMap, 0);
	//need to explicitly tell openGL we arent using a color buffer.
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::DrawShadowMaps(Camera& cam, Scene& scene)
{
	//get the current frame buffer so it can be rebound after
	static int currentFBO = 0;
	glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &currentFBO);

	glViewport(0, 0, shadowWidth, shadowHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, dirFBO);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);
	//glCullFace(GL_FRONT);

	if (scene.GetLights().direction.size() < 1) {

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, dirDepthMap);
		glEnable(GL_TEXTURE_2D);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, windowWidth, windowHeight);
		glCullFace(GL_BACK);
		return;
	}

	//projection
	float near_plane = 5.0f, far_plane = 30.0f, box_size = 15.0f;
	glm::mat4 lightProjection = glm::ortho(-box_size, box_size, -box_size, box_size, near_plane, far_plane);

	//view
	glm::vec3 lightCenter = cam.position + ((glm::cross({0,1,0}, cam.right)) * (far_plane / 2.0f));
	glm::vec3 lightDir = lightCenter + (scene.GetLights().direction[0].direction) * (far_plane/2.0f);

	glm::mat4 lightView = glm::lookAt(lightDir,lightCenter,glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 lightSpaceMatrix = lightProjection * lightView;

	dirShadowShader.SetUniform("lightSpaceMatrix", lightSpaceMatrix);
	

	glm::mat4 proj = cam.GetProjection();
	glm::mat4 view = cam.GetView();
	
	for (auto& it : scene.gameObjects) {
		if (it.second) {
			GameObject* obj = it.second;
			if (!obj->isCastShadow)
				continue;
			glm::mat4 modelMat(1.0f);
			modelMat = glm::translate(modelMat, obj->position);
			modelMat = glm::rotate(modelMat, glm::radians(obj->rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
			modelMat = glm::rotate(modelMat, glm::radians(obj->rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
			modelMat = glm::rotate(modelMat, glm::radians(obj->rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
			modelMat = glm::scale(modelMat, obj->scale);

			dirShadowShader.SetUniform("model", modelMat);
			if (obj->shader) {
				obj->shader->SetUniform("lightSpaceMatrix", lightSpaceMatrix);
				obj->shader->SetUniform("shadowMap", 0);
			}
			else {
				mainShader.SetUniform("lightSpaceMatrix", lightSpaceMatrix);
				mainShader.SetUniform("shadowMap", 0);
			}
			
			if(obj->model_data)
			obj->model_data->Render(proj, view, &dirShadowShader, true, GL_TRIANGLES);

		}
	}

	//bind the shadow map to texture unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, dirDepthMap);
	glEnable(GL_TEXTURE_2D);

	glBindFramebuffer(GL_FRAMEBUFFER, currentFBO);
	glViewport(0, 0, windowWidth, windowHeight);
	glCullFace(GL_BACK);
}

void Renderer::Init(Window* window) {
	//glad required to access GL functions
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "ERROR: Could not load glad" << std::endl;
		return;
	}

	mainWindow = window;
	mainShader = Shader("resources/shaders/Default.vert", "resources/shaders/Default.frag", "");
	postProcessShader = Shader("resources/shaders/screen/screen.vert", "resources/shaders/screen/screen.frag", "");
	dirShadowShader = Shader("resources/shaders/shadow/shadow.vert", "resources/shaders/shadow/shadow.frag", "");

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_MULTISAMPLE);

	windowWidth = window->GetWidth();
	windowHeight = window->GetHeight();

	glViewport(0, 0, windowWidth, windowHeight);

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	////Setup Post Processing resources////
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	//create frame buffer texture
	glGenTextures(1, &textureColorBuff);
	glBindTexture(GL_TEXTURE_2D, textureColorBuff);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowWidth, windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	//attatch texture to frame buffer as its color buffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuff, 0);

	//create an rbo for the depth buffer for our fbo
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, windowWidth, windowHeight);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	//attatch rbo as depth buffer to fbo
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "ERROR: Framebuffer could not be created." << std::endl;
	}
	//done setting up frame buffer so unbind
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

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

	InitShadowsMaps();
	Resize(windowWidth, windowHeight);
}

void Renderer::UsePostProcessingBuffer()
{
	//Bind pre-post processing frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
}

void Renderer::RenderPostProcessingBuffer()
{
	//Bind the default frame buffer and draw scene with post processing shader
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	postProcessShader.Use();
	glBindVertexArray(screenQuad);
	glDisable(GL_DEPTH_TEST);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureColorBuff);
	glEnable(GL_TEXTURE_2D);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

//may want to make several "draw queues" to seperate shaders and opacity
void Renderer::RenderScene(Camera& cam, Scene& scene, double deltaTime) {
	
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	//render shadow maps
	DrawShadowMaps(cam, scene);

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


	glm::mat4 view = cam.GetView();
	glm::mat4 proj = cam.GetProjection();
	for (auto& it : scene.gameObjects) {
		if (it.second) {

			//if not in view, dont render
			if (!it.second->CheckInFrustum(camFrustum))
				continue;
      
			GameObject* obj = it.second;

			//set materials

			//set special uniforms
			obj->SetUniforms();
			obj->shader->SetUniform("wireframe", wireFrame);
			obj->shader->SetUniform("cubemap", 10);
			obj->shader->SetUniform("_Time", (float) glfwGetTime());
			//set model matrix uniforms
			glm::mat4 modelMat(1.0f);
			modelMat = glm::translate(modelMat, obj->position);
			
			//pitch roll and yaw rotationss
			modelMat = glm::rotate(modelMat, glm::radians(obj->rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
			modelMat = glm::rotate(modelMat, glm::radians(obj->rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
			modelMat = glm::rotate(modelMat, glm::radians(obj->rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));

			modelMat = glm::scale(modelMat, obj->scale);
			if (obj->shader) {
				BindMaterial(obj->material,obj->shader);
				obj->shader->SetUniform("model", modelMat);
				obj->model_data->Render(proj, view, obj->shader, true, GL_TRIANGLES);
			}
			else {
				BindMaterial(obj->material, &mainShader);
				mainShader.SetUniform("model", modelMat);
				obj->model_data->Render(proj, view, &mainShader, true, GL_TRIANGLES);
			}

			obj->model_data->Update(deltaTime);
		}
	}

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
	windowWidth = width;
	windowHeight = height;

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