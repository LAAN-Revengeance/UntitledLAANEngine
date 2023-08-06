#include "LuaManager.h"

LuaManager::LuaManager()
{
	luaState;
	luaState.open_libraries(sol::lib::base);
	luaState.open_libraries(sol::lib::math);
	luaState.open_libraries(sol::lib::os);
}

LuaManager::~LuaManager()
{
	luaState.collect_garbage();
}

LuaManager& LuaManager::Get()
{
	static LuaManager l_instance;
	return l_instance;
}

void LuaManager::RunInitMethod() {
	if (init.valid())
		init();
}

void LuaManager::RunUpdateMethod(double dt) {
	if (update.valid()) 
		update(dt);
}


void LuaManager::Expose_Engine() {
	//expose vec3
	Expose_CPPClass<glm::vec3>("vec3",
		sol::constructors<glm::vec3(), glm::vec3(float,float,float)>(),
		sol::meta_function::addition, [](const glm::vec3& a, const glm::vec3& b) {return a + b; },
		sol::meta_function::subtraction, [](const glm::vec3& a, const glm::vec3& b) {return a - b; },
		"multiply", [](const glm::vec3& a, const float& b) {return a * b; },
		"x", &glm::vec3::x,
		"y", &glm::vec3::y,
		"z", &glm::vec3::z,
		"length",&glm::vec3::length
		);

	luaState["Length"] = sol::overload(
		[](const glm::vec3& a) {return glm::length(a); }
	);		

	luaState["NormalizeVector"] = sol::overload(
		[](const glm::vec3& a) {
			
			return glm::normalize(a); 

		}
	);

	luaState["AddVectors"] = sol::overload(
		[](const glm::vec3& a, const glm::vec3& b) {return a + b; }
	);

	luaState["CrossVectors"] = sol::overload(
		[](const glm::vec3& a, const glm::vec3& b) {return glm::cross(a,b); }
	);

	luaState["DotVectors"] = sol::overload(
		[](const glm::vec3& a, const glm::vec3& b) {return glm::dot(a, b); }
	);

	luaState["CloseWindow"] = sol::overload([](bool shouldClose) {glfwSetWindowShouldClose(InputManager::Get().GetWindow(), shouldClose); });

	//expose vec2
	Expose_CPPClass<glm::vec2>("vec2",
		sol::constructors<glm::vec2(), glm::vec2(float, float)>(),
		"x", &glm::vec2::x,
		"y", &glm::vec2::y
		);


	//expose aniation struct
	Expose_CPPClass<animation>("animation",
		sol::constructors<animation(), animation(int,int,int)>(),
		"start",&animation::start,
		"end",&animation::end,
		"speed",&animation::speed
	);


	//expose draw item
	Expose_CPPClass<Texture>("Texture",
		sol::no_constructor
		);

	//expose draw item
	Expose_CPPClass<DrawItem>("DrawItem",
		sol::no_constructor,
		"SetAnimationSpeed", &DrawItem::SetAnimationSpeed,
		"SetAnimation", &DrawItem::SetAnimation,
		"Animate", &DrawItem::Animate,
		"SetShine", &DrawItem::SetShine,
		"SetEmissionTexture", sol::overload(
			static_cast<void (DrawItem::*)(Texture*)>(&DrawItem::SetEmissionTexture),
			static_cast<void (DrawItem::*)(const char*)>(&DrawItem::SetEmissionTexture))
		);

	//expose game object
	Expose_CPPClass<GameObject>("GameObject",
		sol::constructors<GameObject()>(),
		"name", &GameObject::name,
		"position", &GameObject::position,
		"rotation", &GameObject::rotation,
		"scale", &GameObject::scale,
		"rigidBody", &GameObject::rigidBody,
		"stateMachine", &GameObject::stateMachine,
		"SetPosition", &GameObject::SetPosition,
		"SetRotation", &GameObject::SetRotation,
		"GetDrawItem", &GameObject::GetDrawItem,
		"LookAt", &GameObject::LookAt,
		"GetForwardVec", &GameObject::GetForwardVec,
		"GetRightVec", &GameObject::GetRightVec,
		"GetUpVec", &GameObject::GetUpVec,
		"GetID", &GameObject::GetID
		);

	//expose terrain
	Expose_CPPClass<Terrain>("Terrain",
		sol::constructors<Terrain(), Terrain(std::string&, float, float, float,float)>(),
		sol::base_classes, sol::bases<GameObject>(),
		"GetHeight", &Terrain::GetHeight,
		"GetNormal", &Terrain::GetNormal,
		"SetTextures", &Terrain::SetTextures,
		"SetTextureHeights", &Terrain::SetTextureHeights,
		"GetSize", &Terrain::GetSize,
		"SetTextureScale", &Terrain::SetTextureScale,
		"scaleX", &Terrain::scaleX,
		"scaleY", &Terrain::scaleY,
		"scaleZ", &Terrain::scaleZ
		);

	//expose NPC
	Expose_CPPClass<NPC>("NPC",
		sol::constructors<NPC()>(),
		sol::base_classes, sol::bases<GameObject>(),
		"AddData", &NPC::AddData,
		"GetData", &NPC::GetData,
		"HasData", &NPC::HasData,
		"MoveTo2D", &NPC::MoveTo2D,
		"MoveTo3D", &NPC::MoveTo3D,
		"IsTargeting", &NPC::IsTargeting,
		"StopMoving", &NPC::StopMoving
		);

	//expose resource manager class
	Expose_CPPClass<ResourceManager>("ResourceManager",
		sol::no_constructor,
		"Get", &ResourceManager::Get,

		"CreateGameObject", &ResourceManager::CreateGameObject,
		"CreateNPCObject", &ResourceManager::CreateNPCObject,
		"CreateTerrain", &ResourceManager::CreateTerrain,
		"CreateWater", &ResourceManager::CreateWater,
		"LoadTexture", &ResourceManager::LoadTexture,
		"LoadModel", &ResourceManager::LoadModel,
		"LoadAnimatedModel", &ResourceManager::LoadAnimatedModel,
		"LoadShader", &ResourceManager::LoadShader,
		"LoadCubemap", &ResourceManager::LoadCubemap,

		"GetCubeMap", &ResourceManager::GetCubeMap,
		"GetGameObject", &ResourceManager::GetGameObject,
		"GetModel", &ResourceManager::GetModel,
		"GetShader", &ResourceManager::GetShader,
		"GetTexture", &ResourceManager::GetTexture,


		"GetModel", &ResourceManager::GetDrawItemReference
		);
	//expose resource manager singleton
	luaState["resources"] = &ResourceManager::Get();

	//expose skybox
	Expose_CPPClass<CubeMap>("CubeMap",
		sol::constructors<CubeMap, CubeMap(std::vector<std::string>&)>()
		);

	//expose scene object
	Expose_CPPClass<Scene>("Scene",
		sol::constructors<Scene()>(),
		"AddObject", &Scene::AddObject,
		"AddLights", &Scene::AddLights,
		"SetSkybox", &Scene::SetSkybox,
		"GetCamera", &Scene::GetCamera,
		"GetLights", &Scene::GetLights,
		"GetObject", &Scene::GetObject,
		"GetNPC", &Scene::GetNPC
		);

	//expose camera
	Expose_CPPClass<Camera>("Camera",
		sol::constructors<Camera()>(),
		"LookAt", &Camera::LookAt,
		"position", &Camera::position,
		"rotation", &Camera::rotation,
		"aspectRatio", &Camera::aspectRatio,
		"FOV", &Camera::FOV,
		"nearPlane", &Camera::nearPlane,
		"farPlane", &Camera::farPlane,
		"front", &Camera::front,
		"right", &Camera::right,
		"up", &Camera::up,
		"Yaw", &Camera::Yaw,
		"Pitch", &Camera::Pitch,
		"UpdateCameraVectors", &Camera::UpdateCameraVectors
		);

	//expose lights
	Expose_CPPClass<PointLight>("PointLight",
		sol::constructors<PointLight, PointLight(glm::vec3, glm::vec3, glm::vec3,float,float,float)>(),
		"position", &PointLight::position,
		"diffuse", &PointLight::diffuse,
		"quadratic", &PointLight::quadratic,
		"linear", &PointLight::linear,
		"constant", &PointLight::constant
		);

	Expose_CPPClass<SpotLight>("SpotLight",
		sol::constructors<SpotLight, SpotLight(glm::vec3, glm::vec3,float,float, glm::vec3, glm::vec3,float,float,float)>(),
		"position", &SpotLight::position,
		"direction", &SpotLight::direction,
		"maxAngle", &SpotLight::maxAngle,
		"featherAngle", &SpotLight::featherAngle,
		"diffuse", &SpotLight::diffuse,
		"specular", &SpotLight::specular,
		"constant", &SpotLight::constant,
		"linear", &SpotLight::linear,
		"quadratic", &SpotLight::quadratic
		);
	Expose_CPPClass<DirectionLight>("DirectionLight",
		sol::constructors<DirectionLight, DirectionLight(glm::vec3, glm::vec3, glm::vec3)>(),
		"direction", &DirectionLight::direction,
		"diffuse", &DirectionLight::diffuse,
		"specular", &DirectionLight::specular
		);
	Expose_CPPClass<Lights>("Lights",
		sol::constructors<Lights>(),
		"ambient", &Lights::ambient,
		"point", &Lights::point,
		"spot", &Lights::spot,
		"direction", &Lights::direction,
		"SetAmbient", &Lights::SetAmbient,
		"AddDirectionLight", &Lights::AddDirectionLight,
		"AddPointLight", &Lights::AddPointLight,
		"AddSpotLight", &Lights::AddSpotLight
		);

	//expose input manager
	Expose_CPPClass<InputManager>("InputManager",
		sol::no_constructor,
		"Get", &InputManager::Get,
		"BindKey", &InputManager::BindKey,
		"GetKeyState", &InputManager::GetKeyState,
		"SetMouseLock", &InputManager::SetMouseLock,
		"GetMouseX", &InputManager::GetMouseX,
		"GetMouseY", &InputManager::GetMouseY,
		"GetScrollOffset", &InputManager::GetScrollOffset,
		"GetMouseLock", &InputManager::GetMouseLock
		);
	luaState["input"] = &InputManager::Get();

	//expose physics
	Expose_CPPClass<Physics>("PhysicsManager",
		sol::no_constructor,
		"AddRigidBody", &Physics::AddRigidBody,
		"AddRigidBodyColliderBox", &Physics::AddRigidBodyColliderBox,
		"AddRigidBodyColliderSphere", &Physics::AddRigidBodyColliderSphere,
		"AddRigidBodyColliderCapsule", &Physics::AddRigidBodyColliderCapsule,
		"AddRigidBodyColliderHeightMap", &Physics::AddRigidBodyColliderHeightMap,
		"SetTimeStep", &Physics::SetTimeStep,
		"ToggleDebugDisplay", &Physics::ToggleDebugDisplay
		);

	Expose_CPPClass<Rigidbody>("RigidBody",
		sol::no_constructor,
		"ApplyForce", &Rigidbody::ApplyForce,
		"ApplyForceLocal", &Rigidbody::ApplyForceLocal,
		"ApplyTorqueLocal", &Rigidbody::ApplyTorqueLocal,
		"SetPosition", &Rigidbody::SetPosition,
		"GetPosition", &Rigidbody::GetPosition,
		"ModType", &Rigidbody::ModType,
		"SetUseGravity", &Rigidbody::SetUseGravity,
		"SetAxisLinearFactor", &Rigidbody::SetAxisLinearFactor,
		"SetAxisAngleFactor", &Rigidbody::SetAxisAngleFactor,
		"SetMass", &Rigidbody::SetMass,
		"SetCenterOfMass", &Rigidbody::SetCenterOfMass,
		"SetDampeningAngle", &Rigidbody::SetDampeningAngle,
		"SetDampeningLinear", &Rigidbody::SetDampeningLinear,
		"ToggleColliderListener", &Rigidbody::ToggleContactListenState,
		"GetIsContact", &Rigidbody::GetIsContact,
		"GetLinearVelocity", &Rigidbody::GetLinearVelocty,
		"GetAngularVelocity", &Rigidbody::GetAngularVelocity,
		"SetLinearVelocity", &Rigidbody::SetLinearVelocity,
		"SetAngularVelocity", &Rigidbody::SetAngularVelocity
		);

	Expose_CPPClass<GUIRenderer>("GUIRenderer",
		sol::no_constructor,
		"Start", &GUIRenderer::Start,
		"End", &GUIRenderer::End,
		"Text", &GUIRenderer::Text,
		"Button", &GUIRenderer::Button,
		"Image", &GUIRenderer::Image,
		"ImageButton", &GUIRenderer::ImageButton,
		"Tab", &GUIRenderer::Tab,
		"SetCursorPosX", &GUIRenderer::SetCursorPosX,
		"SetCursorPosY", &GUIRenderer::SetCursorPosY,
		"SetFont", &GUIRenderer::SetFont
		);

	Expose_CPPClass<Renderer>("Renderer",
		sol::no_constructor,
		"ToggleWireFrame", &Renderer::ToggleWireFrame,
		"GetFPS", &Renderer::GetFPS
		);

	Expose_CPPClass<StateMachine>("StateMachine",
		sol::no_constructor,
		"ChangeState", &StateMachine::ChangeState,
		"ChangeGlobalState", &StateMachine::ChangeGlobalState,
		"RevertState", &StateMachine::RevertState
		);

	Expose_CPPClass<State>("State",
		sol::no_constructor,
		"Enter", &State::Enter,
		"Exit", &State::Exit,
		"Update", &State::Update,
		"ProcessMessage", &State::ProcessMessage
		);

	//expose the sound engine
	Expose_CPPClass<SoundEngine>("SoundEngine",
		sol::no_constructor,
		"addSound", &SoundEngine::addSound,
		"playSound", &SoundEngine::playSound,
		"addMusic", &SoundEngine::addMusic,
		"SetVolume", &SoundEngine::setVolume,
		"SetMusicVolume", &SoundEngine::setMusicVolume,
		"setListenerPos", &SoundEngine::setListenerPos,
		"setMusicPos", &SoundEngine::setMusicPos,
		"toggleMusic", &SoundEngine::toggleMusic,
		"playSoundLoop", &SoundEngine::playSoundLoop,
		"stopSoundLoop", &SoundEngine::stopSoundLoop
		);
	luaState["Sound"] = &SoundEngine::Get();

	//add generic built in states
	State* state_wander = new State_Wander;
	AIManager::Get().AddState("state_wander", state_wander);
	Expose_CPPReference("state_wander", *state_wander);

	State* state_chase = new State_Chase;
	AIManager::Get().AddState("state_chase", state_chase);
	Expose_CPPReference("state_chase", *state_chase);

	State* state_pursuit = new State_Pursuit;
	AIManager::Get().AddState("state_pursuit", state_pursuit);
	Expose_CPPReference("state_pursuit", *state_pursuit);

	State* state_flee = new State_Flee;
	AIManager::Get().AddState("state_flee", state_flee);
	Expose_CPPReference("state_flee", *state_flee);

	State* state_evade = new State_Evade;
	AIManager::Get().AddState("state_evade", state_evade);
	Expose_CPPReference("state_evade", *state_evade);

	State* state_patrol = new State_Patrol;
	AIManager::Get().AddState("state_patrol", state_patrol);
	Expose_CPPReference("state_patrol", *state_patrol);
	
	Expose_CPPClass<AIManager>("AIManager",
		sol::no_constructor,
		"AddState", &AIManager::AddState,
		"GetState", &AIManager::GetState,
		"SendMessage", &AIManager::SendMessage
		);
	Expose_CPPReference("aimanager", AIManager::Get());

	Expose_CPPClass<Message>("Message",
		sol::no_constructor,
		"dispatchTime", &Message::dispatchTime,
		"msgType", &Message::msgType,
		"receiverID", &Message::receiverID,
		"senderID", &Message::senderID
		);

	Expose_CPPClass<SceneLoader>("SceneLoader",
		sol::no_constructor,
		"LoadScene", &SceneLoader::LoadScene,
		"SaveScene", &SceneLoader::SaveScene,
		"GetSaves", &SceneLoader::GetSaves
		);



	static SceneLoader loader;
	Expose_CPPReference("loader",loader);

	LoadScript("resources/scripts/main.lua");
	update = GetFunction("update");
	init = GetFunction("init");
}


void LuaManager::LoadScript(const std::string& fileName) {
	try
	{
		luaState.script_file(fileName);
	}
	catch (const sol::error e)
	{
		std::cout << "ERROR: Could not load Loading Script File!" << e.what() << std::endl;
	}
}

sol::function LuaManager::GetFunction(const char* luaName) {

	sol::function func = luaState[luaName];
	if (func.valid()) {
		return func;
	}
	else {
		std::cout << "ERROR: Could not retrieve function: " << luaName << std::endl;
		return sol::nil;
	}

}

