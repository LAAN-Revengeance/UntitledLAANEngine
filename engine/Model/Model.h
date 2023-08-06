#pragma once
#include <Graphics/Graphics.h>

#include "Camera.h"
#include "Material.h"
#include <iostream>
#include "OBJReader.h"
#include <lighting.h>
#include <DrawItem.h>

/**
*	@Class Model
*	@brief Stores model and texture data for rendering objects in OpenGL.
*   Provides a draw method to render this model to openGL's current buffer.
*
*	@author Andres Comeros-Ochtman
*	@version 2.0
*	@date 15/04/2023
*/
class Model : public DrawItem {
public:
        /**
        *	@brief default constructor
        */
	Model();
        /*
        *   @brief default destructor, ensures textures and model data is freed
        */
	~Model();
        /**
        *	@brief constructor with inital obj file
        *   @param fileName path to file
        */
	Model(const char* fileName);

        /**
        *	@brief Constructor with instance matrix for instanced rendering.
        *   @param fileName path to .obj file
        *   @param nMatrix vector of transforms for instances of this model
        */
    Model(const char* fileName, std::vector<glm::mat4> nMatrix);

        /*
        *   @brief sets the model data based on obj file
        *   @param fileName file path to obj file
        *   @return bool - if read was a success
        */
	void ReadOBJ(const char *fileName);

        /*
        *   @brief Sets vertex data manually, use if data uses an EBO
        *   @param nVertexData pointer to array of verticies for VBO
        *   @param numData size of vertex array
        *   @param vertIndexes  pointer to array of indicies for the EBO
        *   @param numIndex size of vertIndexes
        *   @return void
        */
	void SetVertexData(float* nVertexData, int numData,unsigned int *vertIndexes,int numIndex);

        /*
        *   @brief Sets vertex data manually, use if data does not use an EBO
        *   @param nVertexData pointer to array of verticies for VBO
        *   @param numData size of vertex array
        *   @return void
        */
    void SetVertexData(float* nVertexData, int numData);

        /*
        *   @brief Sets vertex data if model represents a wireframe object with a color properties
        *   @param nVertexData vertex array
        *   @param numData amount of vertecies in nVertexData
        *   @return void
        */
    void SetDebugVertexData(float* nVertexData, int numData);

        /*
        *   @brief Sets Index data manually, use if data uses an EBO
        *   @param vertIndexes  pointer to array of indicies for the EBO
        *   @param numIndex size of vertIndexes
        *   @return void
        */
    void SetVertexElements(unsigned int* vertIndexes, int numIndex);
        /*
        *   @brief draw this model using specified camera and shader
        *   @param camera camera data to modify vertecies by in shader
        *   @param shader shader program used to render the geometry
        *   @param isElements whether an EBO is used to render this object
        *   @param primative which openGL primative to draw using the vertecies
        *   @return void
        */
	void Render(Camera* camera, Shader* shader, bool isElements, unsigned int primative);

        /*
        *   @brief set instance data
        *   @param nMatrix all transforms for each instance
        *   @return void
        */
    void SetInstanceMatrix(std::vector<glm::mat4> nMatrix);
        
    OBJData* GetModelData();
    std::vector<glm::mat4>* getInstanceMatrix();
    
    void FreeData();
    void FreeEBO();
private:
	int vertCount;
	int elementCount;
	VAO vao;
    VBO vbo;
    VBO ivbo;
    EBO ebo;

    OBJData* modelData;

    std::vector<glm::mat4> instanceMatrixes;
    int instanceCount;
};