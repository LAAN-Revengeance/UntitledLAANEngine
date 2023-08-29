#include "SceneLoader.h"

SceneLoader::SceneLoader()
{

}

SceneLoader::~SceneLoader()
{

}

void SceneLoader::SaveScene(Scene* scene, const std::string outName)
{
    if (!scene)
        return;
    std::ofstream out(outName);

    Json::Value root;
    Json::Value objects;
    Json::Value resources;

    //save resouce paths
    ResourceManager& res = ResourceManager::Get();

    Json::Value cubemaps;
    for (auto& it : res.cubemapPaths)
    {
        Json::Value cm;
        cm["name"   ]   = it.first;
        cm["right"  ]  = it.second[0];
        cm["left"   ]   = it.second[1];
        cm["top"    ]    = it.second[2];
        cm["bottom" ] = it.second[3];
        cm["front"  ]  = it.second[4];
        cm["back"   ]   = it.second[5];

        cubemaps.append(cm);
    }
    resources["cubemaps"] = cubemaps;

    Json::Value textures;
    for (auto& it : res.texturePaths)
    {
        Json::Value tex;
        tex["path"] = it.second;
        tex["name"] = it.first;
        textures.append(tex);
    }
    resources["textures"] = textures;
    
    Json::Value shaders;
    for (auto& it : res.shadersPaths)
    {
        Json::Value sha;
        
        sha["name"] = it.first;
        sha["vert"] = it.second[0];
        sha["frag"] = it.second[1];
        sha["geom"] = it.second[2];

        shaders.append(sha);
    }
    resources["shaders"] = shaders;
    
    Json::Value models;
    for (auto& it : res.modelPaths)
    {
        Json::Value mod;
    
        DrawItem* drawItem = res.models.at(it.first);
    
        mod["path"] = it.second;
        mod["name"] = it.first;
    
        std::string type = "";

        if (dynamic_cast<Mesh*>(drawItem))
            type = "mesh";

        if (dynamic_cast<md2_model_t*>(drawItem)) {
            type = "md2";

            Json::Value animations;
            for (auto& it : dynamic_cast<md2_model_t*>(drawItem)->animations)
            {
                Json::Value anim;
                anim["name"] = it.first.c_str();
                anim["start"] = it.second.start;
                anim["end"] = it.second.end;
                anim["speed"] = it.second.speed;

                animations.append(anim);
            }
            mod["animations"] = animations;

        }

        mod["type"] = type;


        models.append(mod);
    }
    resources["models"] = models;

    root["resources"] = resources;

    //skybox
    if(scene->skybox)
        root["skybox"] = scene->skybox->name;
    
    //lights
    Json::Value Lighting;
    Json::Value AmibientLight;
    Json::Value DirLights;
    Json::Value PointLights;

    AmibientLight.append(scene->lights.ambient.x);
    AmibientLight.append(scene->lights.ambient.y);
    AmibientLight.append(scene->lights.ambient.z);

    for (auto& it : scene->lights.direction) {
        Json::Value dirLight;

        dirLight["direction"].append(it.direction.x);
        dirLight["direction"].append(it.direction.y);
        dirLight["direction"].append(it.direction.z);

        dirLight["diffuse"].append(it.diffuse.x);
        dirLight["diffuse"].append(it.diffuse.y);
        dirLight["diffuse"].append(it.diffuse.z);

        dirLight["specular"].append(it.specular.x);
        dirLight["specular"].append(it.specular.y);
        dirLight["specular"].append(it.specular.z);

        DirLights.append(dirLight);
    }

    for (auto& it : scene->lights.point) {
        Json::Value pntLight;

        pntLight["position"].append(it.position.x);
        pntLight["position"].append(it.position.y);
        pntLight["position"].append(it.position.z);

        pntLight["diffuse"].append(it.diffuse.x);
        pntLight["diffuse"].append(it.diffuse.y);
        pntLight["diffuse"].append(it.diffuse.z);

        pntLight["specular"].append(it.specular.x);
        pntLight["specular"].append(it.specular.y);
        pntLight["specular"].append(it.specular.z);

        pntLight["constant"] = it.constant;
        pntLight["linear"] = it.linear;
        pntLight["quadratic"] = it.quadratic;

        PointLights.append(pntLight);
    }

    Lighting["Ambient"] = AmibientLight;
    Lighting["DirectionLights"] = DirLights;
    Lighting["PointLights"] = PointLights;
    root["Lighting"] = Lighting;

    //serialize game objects
    for (auto& it : scene->gameObjects)
    {
        Json::Value jobj = ObjectToJson(it.second);
        objects.append(jobj);
    }
    root["objects"] = objects;

    Json::StreamWriterBuilder builder;
    const std::string json_file = Json::writeString(builder, root);
    out << json_file << std::endl;
    out.close();

}

Scene& SceneLoader::LoadScene(const char* inName)
{
    Scene* scene = new Scene();

    std::ifstream file(inName);
    Json::Reader reader;
    Json::Value sceneJSON;
    reader.parse(file, sceneJSON);

    ResourceManager& res = ResourceManager::Get();
    //load resources
    Json::Value jCubemaps = sceneJSON["resources"]["cubemaps"];
    Json::Value jTextures = sceneJSON["resources"]["textures"];
    Json::Value jShaders  = sceneJSON["resources"]["shaders"];
    Json::Value jModels   = sceneJSON["resources"]["models"];

    
    for (int i = 0; i < jCubemaps.size(); i++)
    {
        std::string name =      jCubemaps[i]["name"].asString();
        std::string right =     jCubemaps[i]["right"].asString();
        std::string left =      jCubemaps[i]["left"].asString();
        std::string top =       jCubemaps[i]["top"].asString();
        std::string bottom =    jCubemaps[i]["bottom"].asString();
        std::string front =     jCubemaps[i]["front"].asString();
        std::string back =      jCubemaps[i]["back"].asString();
   
        if(res.cubemaps.find(name) == res.cubemaps.end())
            res.LoadCubemap(name, right, left, top, bottom, front, back);
    }

    for (int i = 0; i < jTextures.size(); i++)
    {
        std::string name = jTextures[i]["name"].asString();
        std::string path = jTextures[i]["path"].asString();

        if (res.textures.find(name) == res.textures.end())
            res.LoadTexture(name,path);
    }

    for (int i = 0; i < jShaders.size(); i++)
    {
        std::string name = jShaders[i]["name"].asString();
        std::string vert = jShaders[i]["vert"].asString();
        std::string frag = jShaders[i]["frag"].asString();
        std::string geom = jShaders[i]["geom"].asString();
        if (res.shaders.find(name) == res.shaders.end())
            res.LoadShader(name, vert, frag, geom);
    }

    for (int i = 0; i < jModels.size(); i++)
    {
        std::string name = jModels[i]["name"].asString();
        std::string path = jModels[i]["path"].asString();
        std::string diff = jModels[i]["diff"].asString();
        std::string spec = jModels[i]["spec"].asString();
        std::string emis = jModels[i]["emis"].asString();
        std::string type = jModels[i]["type"].asString();

        if (res.models.find(name) != res.models.end())
            continue;

        if (type.compare("mesh") == 0) {
            res.LoadModel(name,path);
        }
        else if(type.compare("md2") == 0) {
            res.LoadAnimatedModel(name, path);
            md2_model_t* md2Model = dynamic_cast<md2_model_t*>(res.models.at(name));

            Json::Value animations = jModels[i]["animations"];
            for (int j = 0; j < animations.size(); j++)
            {
                md2Model->SetAnimation(animations[j]["name"].asString(), animations[j]["start"].asInt(), animations[j]["end"].asInt(), animations[j]["speed"].asFloat());
            }
        }
    }

    //load lighting data
    Json::Value DirLights =   sceneJSON["Lighting"]["DirectionLights"];
    Json::Value PntLights =       sceneJSON["Lighting"]["PointLights"];
    Json::Value AmbientLight = sceneJSON["Lighting"]["Ambient"];
    scene->lights.ambient = { AmbientLight[0].asFloat(),AmbientLight[1].asFloat() ,AmbientLight[2].asFloat() };

    for (int i = 0; i < DirLights.size(); i++)
    {
        glm::vec3 dir = { DirLights[i]["direction"][0].asFloat(),DirLights[i]["direction"][1].asFloat(), DirLights[i]["direction"][2].asFloat()};
        glm::vec3 dif = { DirLights[i]["diffuse"][0].asFloat(),DirLights[i]["diffuse"][1].asFloat(), DirLights[i]["diffuse"][2].asFloat()};
        glm::vec3 spe = { DirLights[i]["specular"][0].asFloat(),DirLights[i]["specular"][1].asFloat(), DirLights[i]["specular"][2].asFloat()};
        scene->lights.AddDirectionLight(dir,dif,spe);
    }

    for (int i = 0; i < PntLights.size(); i++)
    {
        glm::vec3 pos = { PntLights[i]["position"][0].asFloat(),PntLights[i]["position"][1].asFloat(), PntLights[i]["position"][2].asFloat() };
        glm::vec3 dif = { PntLights[i]["diffuse"][0].asFloat(),PntLights[i]["diffuse"][1].asFloat(), PntLights[i]["diffuse"][2].asFloat() };
        glm::vec3 spe = { PntLights[i]["specular"][0].asFloat(),PntLights[i]["specular"][1].asFloat(), PntLights[i]["specular"][2].asFloat() };

        float constant = PntLights[i]["constant"].asFloat();
        float linear = PntLights[i]["linear"].asFloat();
        float quadratic = PntLights[i]["quadratic"].asFloat();

        scene->lights.AddPointLight(pos,dif,spe,constant,linear,quadratic);
    }

    //populate scene
    scene->skybox = res.GetCubeMap(sceneJSON["skybox"].asString());
    Json::Value objects = sceneJSON["objects"];
    for (unsigned int i = 0; i < objects.size(); i++)
    {
        Json::Value jobj = objects[i];
     
        GameObject* go = nullptr;
    
        if (jobj["type"].asString() == "terrain") {
          
            go = res.GetGameObject(jobj["name"].asString());
        }
        else {
            go = &res.CreateGameObject(objects[i]["name"].asString(), objects[i]["model"].asString(), objects[i]["shader"].asString());
        }
        
        //transform properties
        go->name = jobj["name"].asString();
        go->position.x = jobj["position"][0].asFloat();
        go->position.y = jobj["position"][1].asFloat();
        go->position.z = jobj["position"][2].asFloat();
    
        go->scale.x = jobj["scale"][0].asFloat();
        go->scale.y = jobj["scale"][1].asFloat();
        go->scale.z = jobj["scale"][2].asFloat();
    
        go->rotation.x = jobj["rotation"][0].asFloat();
        go->rotation.y = jobj["rotation"][1].asFloat();
        go->rotation.z = jobj["rotation"][2].asFloat();
        
        //material properties
        if(jobj.isMember("diff"))
            go->material.diffuseTexture.push_back(res.GetTexture(jobj["diff"].asString()));
        if (jobj.isMember("spec"))
            go->material.specularMap.push_back(res.GetTexture(jobj["spec"].asString()));
        if (jobj.isMember("emis"))
            go->material.emissionMap.push_back(res.GetTexture(jobj["emis"].asString()));

        res.StoreGameObject(go);
        scene->AddObject(*go);
    }

    return *scene;
}

std::vector<std::string> SceneLoader::GetSaves(const std::string path)
{
    std::vector<std::string> fileNames;
    struct stat info;
    if (stat(path.c_str(), &info) != 0)
        return fileNames;

    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        if (entry.is_regular_file()) {
            fileNames.push_back(entry.path().filename().string());
        }
    }
    return fileNames;
}

Json::Value SceneLoader::ObjectToJson(GameObject* obj)
{
    Json::Value jobj;

    jobj["name"] = obj->name;
    jobj["position"].append(obj->position.x);
    jobj["position"].append(obj->position.y);
    jobj["position"].append(obj->position.z);

    jobj["scale"].append(obj->scale.x);
    jobj["scale"].append(obj->scale.y);
    jobj["scale"].append(obj->scale.z);

    jobj["rotation"].append(obj->rotation.x);
    jobj["rotation"].append(obj->rotation.y);
    jobj["rotation"].append(obj->rotation.z);

    //pointer
    if (obj->model_data)
        jobj["model"] = obj->model_data->name;

    if (obj->shader)
        jobj["shader"] = obj->shader->name;
    
    //material
    if(!obj->material.diffuseTexture.empty())
        jobj["diff"] = obj->material.diffuseTexture[0]->name;
    if (!obj->material.specularMap.empty())
        jobj["spec"] = obj->material.specularMap[0]->name;
    if (!obj->material.emissionMap.empty())
        jobj["emis"] = obj->material.emissionMap[0]->name;

    /*
    //rigidbody
    Json::Value rb;

    rb["contact_listen"] = obj->rigidBody.GetIsContactListen();
    rb["mass"] = obj->rigidBody.GetMass();
    rb["mod"] = obj->rigidBody.GetModType();
    rb["damp_linear"] = obj->rigidBody.GetDampeningLinear();
    rb["damp_angle"] = obj->rigidBody.GetDampeningAngle();

    rb["mass_center"].append(obj->rigidBody.GetCenterOfMass().x);
    rb["mass_center"].append(obj->rigidBody.GetCenterOfMass().y);
    rb["mass_center"].append(obj->rigidBody.GetCenterOfMass().z);

    rb["axis_linear_factor"].append(obj->rigidBody.GetAxisLinearFactor().x);
    rb["axis_linear_factor"].append(obj->rigidBody.GetAxisLinearFactor().y);
    rb["axis_linear_factor"].append(obj->rigidBody.GetAxisLinearFactor().z);

    rb["axis_angle_factor"].append(obj->rigidBody.GetAxisAngleFactor().x);
    rb["axis_angle_factor"].append(obj->rigidBody.GetAxisAngleFactor().y);
    rb["axis_angle_factor"].append(obj->rigidBody.GetAxisAngleFactor().z);

    rb["linear_velocity"].append(obj->rigidBody.GetLinearVelocty().x);
    rb["linear_velocity"].append(obj->rigidBody.GetLinearVelocty().y);
    rb["linear_velocity"].append(obj->rigidBody.GetLinearVelocty().z);

    rb["angular_velocity"].append(obj->rigidBody.GetAngularVelocity().x);
    rb["angular_velocity"].append(obj->rigidBody.GetAngularVelocity().y);
    rb["angular_velocity"].append(obj->rigidBody.GetAngularVelocity().z);

    Json::Value rbcollider;
    PhysicsCollider* collider = obj->rigidBody.GetCollider();

    rbcollider["type"] = collider->GetType();
    if (collider->GetType() != COLLIDER_INVALID)
    {
        rbcollider["mass"] = collider->mass;
        rbcollider["bounce"] = collider->bounce;
        rbcollider["friction"] = collider->friction;
        rbcollider["offset"].append(collider->offset.x);
        rbcollider["offset"].append(collider->offset.y);
        rbcollider["offset"].append(collider->offset.z);

        rbcollider["rotation"].append(collider->rotation.x);
        rbcollider["rotation"].append(collider->rotation.y);
        rbcollider["rotation"].append(collider->rotation.z);
    }

    switch (collider->GetType())
    {
    case COLLIDER_BOX:
        rb["scale"].append(static_cast<BoxCollider*>(collider)->scale.x);
        rb["scale"].append(static_cast<BoxCollider*>(collider)->scale.y);
        rb["scale"].append(static_cast<BoxCollider*>(collider)->scale.z);
        break;
    case COLLIDER_SPHERE:
        rb["radius"] = static_cast<SphereCollider*>(collider)->radius;
        break;
    case COLLIDER_CAPSULE:
        rb["radius"] = static_cast<CapsuleCollider*>(collider)->radius;
        rb["height"] = static_cast<CapsuleCollider*>(collider)->height;
        break;
    case COLLIDER_TERRAIN:
        rb["rows"] = static_cast<TerrainCollider*>(collider)->rows;
        rb["columns"] = static_cast<TerrainCollider*>(collider)->columns;
        rb["min"] = static_cast<TerrainCollider*>(collider)->min;
        rb["max"] = static_cast<TerrainCollider*>(collider)->max;
        rb["heights"] = static_cast<TerrainCollider*>(collider)->heights;
        break;
    case COLLIDER_INVALID:
        break;
    default:
        break;
    }
    rb["collider"] = rbcollider;
    jobj["rigidbody"] = rb;
    //end rigidbody
    */
    //state machine info
    AIManager& ai = AIManager::Get();

    if (obj->stateMachine.GetState())
        jobj["state"] = ai.GetStateKey(obj->stateMachine.GetState());

    if (obj->stateMachine.GetPreviousState())
        jobj["previous_state"] = ai.GetStateKey(obj->stateMachine.GetPreviousState());

    if (obj->stateMachine.GetGlobalState())
        jobj["global_state"] = ai.GetStateKey(obj->stateMachine.GetGlobalState());

    if (dynamic_cast<Terrain*>(obj)) {
        Terrain* ter = dynamic_cast<Terrain*>(obj);
        jobj["type"] = "terrain";
        jobj["scaleX"] = ter->scaleX;
        jobj["scaleY"] = ter->scaleY;
        jobj["scaleZ"] = ter->scaleZ;
        jobj["terrain_size"] = ter->GetSize();
        jobj["texture_scale"] = ter->GetSize();

        if(ter->GetHeightTexture())
            jobj["height_texture"] = ter->GetHeightTexture()->name;
    }
    else {
        jobj["type"] = "base";
    }
    return jobj;
}

Json::Value SceneLoader::LightsToJson(Lights* lights)
{
    return Json::Value();
}
