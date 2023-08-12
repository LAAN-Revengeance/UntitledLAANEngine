#include "MD2Reader.h"
#include "anorms.h"


int md2_model_t::ReadMD2Model(const char* filename)
{
    FILE* fp;
    int i;

    fopen_s(&fp,filename, "rb");

    if (!fp)
    {
        fprintf(stderr, "Error: couldn't open \"%s\"!\n", filename);
        return 0;
    }

    /* Read header */
    fread(&header, 1, sizeof(md2Header), fp);

    if ((header.ident != 844121161) ||
        (header.version != 8))
    {
        /* Error! */
        fprintf(stderr, "Error: bad version or identifier\n");
        fclose(fp);
        return 0;
    }

    /* Memory allocations */
    texcoords = (struct md2_texCoord_t*)
        malloc(sizeof(struct md2_texCoord_t) * header.num_st);
    triangles = (struct md2_triangle_t*)
        malloc(sizeof(struct md2_triangle_t) * header.num_tris);
    frames = (struct md2_frame_t*)
        malloc(sizeof(struct md2_frame_t) * header.num_frames);

    /* Read model data */
    fseek(fp, header.offset_st, SEEK_SET);
    fread(texcoords, sizeof(struct md2_texCoord_t),
        header.num_st, fp);

    fseek(fp,header.offset_tris, SEEK_SET);
    fread(triangles, sizeof(struct md2_triangle_t),
        header.num_tris, fp);

       
    /* Read frames */
    fseek(fp, header.offset_frames, SEEK_SET);
    for (i = 0; i < header.num_frames; ++i)
    {
        /* Memory allocation for vertices of this frame */
        frames[i].verts = (struct md2_vertex_t*)
            malloc(sizeof(struct md2_vertex_t) * header.num_vertices);

        /* Read frame data */
        fread(frames[i].scale, sizeof(vec3_t), 1, fp);
        fread(frames[i].translate, sizeof(vec3_t), 1, fp);
        fread(frames[i].name, sizeof(char), 16, fp);
        fread(frames[i].verts, sizeof(struct md2_vertex_t),
            header.num_vertices, fp);
    }

    fclose(fp);

    CreateModel(0);

    return 1;
}

void md2_model_t::Render(Camera* camera, Shader* shader, bool isElements = false, unsigned int primative = GL_TRIANGLES)
{
    if (!visible)
        return;

    BindMaterial(shader);

    glm::mat4 view = camera->GetView();
    glm::mat4 projection = camera->GetProjection();

    //Set camera position
	shader->SetUniform("cameraPos", camera->position);

    //basic postion matricies
    shader->SetUniform("view", view);
    shader->SetUniform("projection", projection);

    vao.Bind();

    glDrawArrays(GL_TRIANGLES, 0, header.num_tris * 3);

    shader->Use();
    vao.UnBind();
}

void md2_model_t::CreateModel(int n)
{
    int i, j;
    GLfloat s, t;
    struct md2_frame_t* pframe;
    struct md2_vertex_t* pvert;

    std::vector<pos_normal_vert>vertbuff;
    std::vector<glm::vec2>uvbuff;

    pos_normal_vert nVert;
    glm::vec2 nUV;

    maxBounds = 0.0f;

    for (i = 0; i < header.num_tris; ++i)
    {
        /* Draw each vertex */
        for (j = 0; j < 3; ++j)
        {
            pframe = &frames[n];
            pvert = &pframe->verts[triangles[i].vertex[j]];

            /* Compute texture coordinates */
            s = (GLfloat)texcoords[triangles[i].st[j]].s / header.skinwidth;
            t = (GLfloat)texcoords[triangles[i].st[j]].t / header.skinheight;

            /* Pass texture coordinates to OpenGL */
            nUV = { s,1.0 - t };

            /* Normal vector */
            nVert.normal = { anorms_table[pvert->normalIndex][0],anorms_table[pvert->normalIndex][1],anorms_table[pvert->normalIndex][2] };

            /* Calculate vertex real position */
            nVert.vertex.x = (pframe->scale[0] * pvert->v[0]) + pframe->translate[0];
            nVert.vertex.z = (pframe->scale[1] * pvert->v[1]) + pframe->translate[1];
            nVert.vertex.y = (pframe->scale[2] * pvert->v[2]) + pframe->translate[2];

            if (maxBounds < glm::length(nVert.vertex))
                maxBounds = glm::length(nVert.vertex);

            vertbuff.push_back(nVert);
            uvbuff.push_back(nUV);
        }
    }

    VAO nVAO;

    VBO   uvVBO(&uvbuff[0].x, sizeof(float) * header.num_tris * 3 * 2);
    VBO vertVBO(&vertbuff[0].vertex.x, sizeof(float) * header.num_tris * 3 * 6);

    //add normal and vertex vbo attributes
    nVAO.AddAttribute(vertVBO, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
    nVAO.AddAttribute(vertVBO, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    //add UV vbo attributes
    nVAO.AddAttribute(uvVBO, 2, 2, GL_FLOAT, 2 * sizeof(float), (void*)0);

    vertvbo.Delete();
    uvvbo.Delete();
    vao.Delete();

    vertvbo = vertVBO;
    uvvbo = uvVBO;
    vao = nVAO;

    prevFrame = n;
    curInterpolation = 0.0f;
    
    //create array of current verts in use.
    delete[] currentVerts;
    currentVerts = (pos_normal_vert*)malloc(sizeof(pos_normal_vert) * vertbuff.size());
    std::copy(vertbuff.begin(), vertbuff.end(), currentVerts);
}

void md2_model_t::PrintHeader()
{
    header.Print();
}

void md2_model_t::SetAnimationSpeed(float speed)
{
    animSpeed = speed;
}

float md2_model_t::lerp(float start, float end, float factor)
{
    return start + factor * (end - start);
}


md2_model_t::md2_model_t()
{
}

md2_model_t::md2_model_t(const char* md2File)
{
    ReadMD2Model(md2File);
}

md2_model_t::md2_model_t(md2_model_t& other) :
    DrawItem(other), // call base class copy constructor
    animations(other.animations),
    header(other.header),
    texcoords(other.texcoords),
    triangles(other.triangles),
    frames(other.frames),
    animSpeed(other.animSpeed),
    prevFrame(other.prevFrame),
    curInterpolation(other.curInterpolation),
    startFrame(other.startFrame),
    endFrame(other.endFrame)
{
    CreateModel(0);
}

void md2_model_t::Update(float deltaTime)
{
    if (startFrame == endFrame)
        return;

    //increment interpolation and frame
    curInterpolation += deltaTime * animSpeed;
    if (curInterpolation >= 1) {
        curInterpolation = 0.0f;
        prevFrame++;
        if (prevFrame >= header.num_frames - 1 || prevFrame > endFrame)
            prevFrame = startFrame;
    }

    int i,j;
    struct md2_frame_t* pframe;
    struct md2_frame_t* nframe;
    struct md2_vertex_t* pvert;
    struct md2_vertex_t* nvert;

    if (prevFrame >= endFrame) {
        pframe = &frames[prevFrame];
        nframe = &frames[startFrame];
    }
    else {
        pframe = &frames[prevFrame];
        nframe = &frames[prevFrame + 1];
    }

    for (i = 0; i < header.num_tris; i++)
    {
        for (j = 0; j < 3; j++)
        {
            //get last and current frame pointers
            pvert = &pframe->verts[triangles[i].vertex[j]];
            nvert = &nframe->verts[triangles[i].vertex[j]];

            //interpolate normals
            currentVerts[(i * 3)+j].normal = {
                lerp(anorms_table[pvert->normalIndex][0], anorms_table[nvert->normalIndex][0], curInterpolation),
                lerp(anorms_table[pvert->normalIndex][1], anorms_table[nvert->normalIndex][1], curInterpolation),
                lerp(anorms_table[pvert->normalIndex][2], anorms_table[nvert->normalIndex][2], curInterpolation)
            };

            //interpolate and scale verts
            currentVerts[(i * 3)+j].vertex.x = lerp((pframe->scale[0] * (float)pvert->v[0]) + pframe->translate[0], (nframe->scale[0] * (float)nvert->v[0]) + nframe->translate[0], curInterpolation);
            currentVerts[(i * 3)+j].vertex.z = lerp((pframe->scale[1] * (float)pvert->v[1]) + pframe->translate[1], (nframe->scale[1] * (float)nvert->v[1]) + nframe->translate[1], curInterpolation);
            currentVerts[(i * 3)+j].vertex.y = lerp((pframe->scale[2] * (float)pvert->v[2]) + pframe->translate[2], (nframe->scale[2] * (float)nvert->v[2]) + nframe->translate[2], curInterpolation);
        }
    }
    
    //sub out the normal and positional vertex buffer
    vertvbo.SubData(0,sizeof(pos_normal_vert) * header.num_tris * 3,currentVerts);
}

void md2_model_t::SetAnimation(const std::string& animName, int start, int end, float speed)
{
    if (start >= 0 && start < header.num_frames) {
        if (end >= start && end < header.num_frames) {
            animations.insert({ animName, animation(start,end,speed) });
        }
    }
}

void md2_model_t::Animate(const std::string& animname)
{
    if (animations.find(animname) == animations.end()) {
        visible = false;
        return;
    }
    visible = true;
    animation& anim = animations.at(animname);

    if (anim.start != startFrame || anim.end != endFrame) {
        prevFrame = anim.start;
        startFrame = anim.start;
        endFrame = anim.end;
        curInterpolation = 0;
    }
    animSpeed = anim.speed;
}

void md2Header::Print()
{
   std::cout << "ident        : " << ident << std::endl;
   std::cout << "version      : " << version << std::endl;
   std::cout << "skinwidth    : " << skinwidth << std::endl;
   std::cout << "skinheight   : " << skinheight << std::endl;
   std::cout << "framesize    : " << framesize << std::endl;
   std::cout << "num_skins    : " << num_skins << std::endl;
   std::cout << "num_vertices : " << num_vertices << std::endl;
   std::cout << "num_st       : " << num_st << std::endl;
   std::cout << "num_tris     : " << num_tris << std::endl;
   std::cout << "num_glcmds   : " << num_glcmds << std::endl;
   std::cout << "num_frames   : " << num_frames << std::endl;
   std::cout << "offset_skins : " << offset_skins << std::endl;
   std::cout << "offset_st    : " << offset_st << std::endl;
   std::cout << "offset_tris  : " << offset_tris << std::endl;
   std::cout << "offset_frames: " << offset_frames << std::endl;
   std::cout << "offset_glcmds: " << offset_glcmds << std::endl;
   std::cout << "offset_end   : " << offset_end    << std::endl;

}
