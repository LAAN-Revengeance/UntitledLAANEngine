#pragma once
#include <Scene.h>
#include <Graphics/Graphics.h>
/**
*	@Class Renderer
*	@brief Used to draw scene to the screen
*
*	@author Andres Comeros-Ochtman
*	@version 1.0
*	@date 15/04/2023
*/
class Renderer
{
public:

		/**
		*	@brief Get the singleton instance of Renderer
		*	@return The singleton instance of Renderer
		*/
	static Renderer& Get();

		/**
		*	@brief initalize OpenGL
		*	@param window window being drawn
		*	@return void
		*/
	void Init(GLFWwindow* window);

		/**
		*	@brief Draw the scene
		*	@param scene scene to be drawn
		*	@return void
		*/
	void Draw(Camera& cam, Scene& scene, double deltaTime);

		/**
		*	@brief Set uniforms for All lights in a scene
		*	@param sLights lights to set uniforms for
		*	@param sShader shader to set uniforms for
		*	@return void
		*/
	static void SetLightUniforms(Lights& sLights, Shader& sShader = Renderer::Get().mainShader);

		/**
		*	@brief Set uniforms for all  point lights in a scene
		*	@param sLights lights to set uniforms for
		*	@param sShader shader to set uniforms for
		*	@return void
		*/
	static void SetPointLightUniforms(Lights& sLights, Shader& sShader = Renderer::Get().mainShader);

		/**
		*	@brief Set uniforms for all direction lights in a scene
		*	@param sLights lights to set uniforms for
		*	@param sShader shader to set uniforms for
		*	@return void
		*/
	static void SetDirectionLightUniforms(Lights& sLights, Shader& sShader = Renderer::Get().mainShader);

		/**
		*	@brief Set uniforms for all spot lights in a scene
		*	@param sLights lights to set uniforms for
		*	@param sShader shader to set uniforms for
		*	@return void
		*/
	static void SetSpotLightUniforms(Lights& sLights, Shader& sShader = Renderer::Get().mainShader);

		/**
		*	@brief Returns the default shader
		*	@return void
		*/
	Shader& GetShader();

		/**
		*	@brief Calculates current FPS and returns value
		*	@return average Frame per second
		*/
	double GetFPS();

	void ToggleWireFrame();

	void Resize(int width, int height);

		/*
		*   @brief Binds a material
		*   @return void
		*/
	void BindMaterial(Material& material, Shader* shader);
private:

		///Default constructor. Private becuase singleton
	Renderer();
		///Default Destructor.
	~Renderer();
		///Assignment operator. Private becuase singleton
	Renderer& operator =(const Renderer&) = delete;
		///Copy constructor. Private becuase singleton
	Renderer(const Renderer&) = delete;

		///Whether to draw in wireframe mode or not
	bool wireFrame = false;
		///main window
	GLFWwindow* mainWindow;
		///default shader
	Shader mainShader;

	int windowWidth, windowHeight;
		//frames since last fps check
	int numFrames = 0;
		//current frame rate
	double fps = 0.0;
		//time since last fps update
	double lastTime = 0.0;


	//quad frame buffer is rendered onto
	unsigned int screenQuad;

	//post processing shader
	Shader postProcessShader;

	//##-Shadow Data-##//
	//direction light fbo

	const unsigned int shadowWidth = 2048;
	const unsigned int shadowHeight = 2048;

	unsigned int dirFBO;//Directional light fbo
	unsigned int dirDepthMap;//directional light depth buffer(bound as texture so can be read in shader)

	Shader dirShadowShader;

	void InitShadowsMaps();
	void DrawShadowMaps(Camera &cam, Scene& scene);


	//##-Frame buffer data-##//
	//FRAME BUFFER
	unsigned int FBO;

	//RENDER BUFFER
	unsigned int RBO;

	//texture rendered to
	unsigned int textureColorBuff;

	friend class SceneEditor;
};