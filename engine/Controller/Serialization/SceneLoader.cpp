#include "SceneLoader.h"

SceneLoader::SceneLoader()
{

}

SceneLoader::~SceneLoader()
{

}

void SceneLoader::SaveScene(Scene* scene, const std::string outName)
{

    struct stat info;
    std::ofstream out(outName);

    Json::Value root;
    Json::Value objects;

    //skybox
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

Scene& SceneLoader::LoadScene(const std::string inName)
{
    Scene* scene = new Scene();

    std::ifstream file(inName);
    Json::Reader reader;
    Json::Value sceneJSON;
    reader.parse(file, sceneJSON);

    ResourceManager& res = ResourceManager::Get();

    scene->skybox = res.GetCubeMap(sceneJSON["skybox"].asString());

    Json::Value objects = sceneJSON["objects"];
    for (int i = 0; i < objects.size(); i++)
    {
        Json::Value jobj = objects[i];
     
        GameObject* go = nullptr;

        //type specific data
        if(jobj["type"].asString() == "npc"){
            go = &res.CreateNPCObject(objects[i]["name"].asString(), objects[i]["model"].asString(), objects[i]["shader"].asString());
            for (auto& member : jobj["data"].getMemberNames())
            {
                dynamic_cast<NPC*>(go)->AddData(member, jobj["data"][member].asFloat());
            }
        }
        else if (jobj["type"].asString() == "terrain") {
          
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

        //state information
        AIManager& ai = AIManager::Get();
        if(!jobj["state"].empty())
            go->stateMachine.ChangeState(*ai.GetState(jobj["state"].asString()));
        if (!jobj["previous_state"].empty())
            go->stateMachine.ChangePreviousState(*ai.GetState(jobj["previous_state"].asString()));
        if (!jobj["global_state"].empty())
            go->stateMachine.ChangeGlobalState(*ai.GetState(jobj["global_state"].asString()));

        //physics properties


        Json::Value rb = jobj["rigidbody"];
        Json::Value rbcollider = rb["collider"];
        
        scene->physics.AddRigidBody(*go, rb["mod"].asInt());

        if(rb["contact_listen"].asBool())
            go->rigidBody.ToggleContactListenState();

        go->rigidBody.SetMass(rb["mass"].asFloat());
        go->rigidBody.SetDampeningLinear(rb["damp_linear"].asFloat());
        go->rigidBody.SetDampeningAngle(rb["damp_angle"].asFloat());

        go->rigidBody.SetCenterOfMass    ({ rb["mass_center"][0].asFloat() ,rb["mass_center"][1].asFloat() ,rb["mass_center"][2].asFloat() });
        
        go->rigidBody.SetAxisLinearFactor( rb["axis_linear_factor"][0].asFloat() ,rb["axis_linear_factor"][1].asFloat() ,rb["axis_linear_factor"][2].asFloat() );

        go->rigidBody.SetAxisAngleFactor(rb["axis_angle_factor"][0].asFloat(), rb["axis_angle_factor"][1].asFloat(), rb["axis_angle_factor"][2].asFloat());

        if (rbcollider["type"].asInt() != COLLIDER_INVALID)
        {
            float mass = rbcollider["mass"].asFloat();
            float bounce = rbcollider["bounce"].asFloat();
            float friction = rbcollider["friction"].asFloat();
            glm::vec3 offset(rbcollider["offset"][0].asFloat(), rbcollider["offset"][1].asFloat(), rbcollider["offset"][2].asFloat());
            glm::vec3 rotation(rbcollider["rotation"][0].asFloat(), rbcollider["rotation"][1].asFloat(), rbcollider["rotation"][2].asFloat());

            float radius;
            float height;
            int rows;
            int columns;
            float min;
            float max;
            switch (rbcollider["type"].asInt())
            {
            case COLLIDER_BOX:
                glm::vec3 scale(rb["scale"][0].asFloat(), rb["scale"][1].asFloat(), rb["scale"][2].asFloat());
                scene->physics.AddRigidBodyColliderBox(*go,scale,offset,mass,bounce ,friction);
                break;
            case COLLIDER_SPHERE:
                radius = rb["radius"].asFloat();
                scene->physics.AddRigidBodyColliderSphere(*go,radius,offset,mass,bounce,friction);
                break;
            case COLLIDER_CAPSULE:
                radius = rb["radius"].asFloat();
                height = rb["height"].asFloat();
                scene->physics.AddRigidBodyColliderCapsule(*go,radius,height,offset,rotation,mass,bounce,friction);
                break;
            case COLLIDER_TERRAIN:
                rows = rb["rows"].asInt();
                columns = rb["columns"].asInt();
                min = rb["min"].asFloat();
                max = rb["max"].asFloat();
                if(rb["heights"].asBool())
                    scene->physics.AddRigidBodyColliderHeightMap(*static_cast<Terrain*>(go));
                
                break;
            case COLLIDER_INVALID:
                break;
            default:
                break;
            }
            go->rigidBody.SetLinearVelocity(rb["linear_velocity"][0].asFloat(), rb["linear_velocity"][1].asFloat(), rb["linear_velocity"][2].asFloat());
            go->rigidBody.SetAngularVelocity(rb["angular_velocity"][0].asFloat(), rb["angular_velocity"][1].asFloat(), rb["angular_velocity"][2].asFloat());
            go->SetRotation({ jobj["rotation"][0].asFloat(), jobj["rotation"][1].asFloat(), jobj["rotation"][2].asFloat() });

        }
        //end phyiscs

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

    //state machine info
    AIManager& ai = AIManager::Get();

    if (obj->stateMachine.GetState())
        jobj["state"] = ai.GetStateKey(obj->stateMachine.GetState());

    if (obj->stateMachine.GetPreviousState())
        jobj["previous_state"] = ai.GetStateKey(obj->stateMachine.GetPreviousState());

    if (obj->stateMachine.GetGlobalState())
        jobj["global_state"] = ai.GetStateKey(obj->stateMachine.GetGlobalState());

    //idendify obj type.
    if (dynamic_cast<NPC*>(obj)) {
        NPC* npc = dynamic_cast<NPC*>(obj);
        jobj["type"] = "npc";
        
        Json::Value data;
        for (auto& it : npc->data) {
            data[it.first] = it.second;
        }
        jobj["data"] = data;
    }
    else if (dynamic_cast<Terrain*>(obj)) {
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
