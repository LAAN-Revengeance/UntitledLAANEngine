#include "CubeMap.h"

static float cubeVerts[] = {
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};

CubeMap::CubeMap() {
    
    VBO nVBO(cubeVerts,sizeof(cubeVerts));
    vao.AddAttribute(nVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);

    Shader nShader("resources/shaders/Cube_map.vert","resources/shaders/Cube_map.frag","");
    shader = nShader;
    ID = 0;
    height = 0;
    width = 0;
    numColorChannels = 0;
}

CubeMap::CubeMap(std::vector<std::string>& nTextures) : CubeMap() {

    SetTextures(nTextures);
}

void CubeMap::Render(Camera* camera) {

    if (ID == 0)
        return;

    glm::mat4 projection = camera->GetProjection();

    //wacky mat4 to mat 3 to mat 4 to stop transformations
    glm::mat4 view = glm::mat4(glm::mat3(camera->GetView()));

    shader.SetUniform("projection",projection);
    shader.SetUniform("view", view);
    shader.SetUniform("cubemap", 0);

    glDepthFunc(GL_LEQUAL);
    shader.Use();
    vao.Bind();

    glActiveTexture(GL_TEXTURE10);
    glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glDepthFunc(GL_LESS);
}

void CubeMap::SetTextures(std::vector<std::string>& nTextures) {
    
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, ID);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    stbi_set_flip_vertically_on_load(false);
    for (int i = 0; i < 6; i++)
    {
        unsigned char* data = stbi_load(nTextures[i].c_str(), &width, &height, &numColorChannels, 0);
        if (data)
        {
            switch (numColorChannels) {
            case 1:
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
                break;
            case 2:
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RG, width, height, 0, GL_RG, GL_UNSIGNED_BYTE, data);
                break;
            case 3:
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                break;
            case 4:
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                break;
            }
            stbi_image_free(data);
        }
        else
        {
            std::cout << "FAILED TO LOAD IMAGE: " << nTextures[i] << std::endl;
            stbi_image_free(data);
        }
    }
    
    
}