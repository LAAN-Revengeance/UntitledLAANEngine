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
        cm["right"]     .append(it.second[0]);
        cm["left"]      .append(it.second[1]);
        cm["top"]       .append(it.second[2]);
        cm["bottom"]    .append(it.second[3]);
        cm["front"]     .append(it.second[4]);
        cm["back"]      .append(it.second[5]);

        cubemaps[it.first] = cm;
    }
    resources["cubemaps"] = cubemaps;

    Json::Value textures;
    for (auto& it : res.texturePaths)
    {
        Json::Value tex;
        tex["path"].append(it.second);
        textures[it.first] = tex;
    }
    resources["textures"] = textures;
    
    Json::Value shaders;
    for (auto& it : res.shadersPaths)
    {
        Json::Value sha;
    
        sha["vert"].append(it.second[0]);
        sha["frag"].append(it.second[1]);
        sha["geom"].append(it.second[2]);

        shaders[it.first] = sha;
    }
    resources["shaders"] = shaders;
    
    Json::Value models;
    for (auto& it : res.modelPaths)
    {
        Json::Value mod;
    
        DrawItem* drawItem = res.models.at(it.first);
    
        mod["path"] = it.second;
    
        if(drawItem->GetDiffuseTexture(0))
            mod["diff"].append(drawItem->GetDiffuseTexture(0)->name);
    
        if (drawItem->GetEmissionTexture(0))
            mod["emis"].append(drawItem->GetEmissionTexture(0)->name);
    
        if (drawItem->GetSpecularTexture(0))
            mod["spec"].append(drawItem->GetSpecularTexture(0)->name);
    
        models[it.first] = mod;
    }
    resources["models"] = models;

    root["resources"] = resources;

    //skybox
    if(scene->skybox)
        root["skybox"] = scene->skybox->name;
    
    //lights


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
