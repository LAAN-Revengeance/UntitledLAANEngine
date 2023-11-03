#include "LuaGameBridge.h"
#include <AnimatedDrawItem.h>
#include <MD2/MD2Reader.h>
#include <AI/AIManager.h>
#include <AI/StateMachine/States/ScriptableState.h>

void LuaGameBridge::ExposeEngine(LuaManager* luaManager)
{
	luaManager->ClearLuaState();
	//expose vec3
	luaManager->Expose_CPPClass<glm::vec3>("vec3",
		sol::constructors<glm::vec3(), glm::vec3(float, float, float)>(),
		sol::meta_function::addition, [](const glm::vec3& a, const glm::vec3& b) {return a + b; },
		sol::meta_function::subtraction, [](const glm::vec3& a, const glm::vec3& b) {return a - b; },
		"multiply", [](const glm::vec3& a, const float& b) {return a * b; },
		"x", &glm::vec3::x,
		"y", &glm::vec3::y,
		"z", &glm::vec3::z,
		"length", &glm::vec3::length
	);


	luaManager->Expose_CPPClass<glm::quat>("quat",
		sol::constructors<glm::quat(), glm::quat(float, float, float,float)>(),
		"x", &glm::quat::x,
		"y", &glm::quat::y,
		"z", &glm::quat::z,
		"w", &glm::quat::w
	);

	luaManager->luaState["Length"] = sol::overload(
		[](const glm::vec3& a) {return glm::length(a); }
	);

	luaManager->luaState["NormalizeVector"] = sol::overload(
		[](const glm::vec3& a) {

			return glm::normalize(a);

		}
	);

	luaManager->luaState["AddVectors"] = sol::overload(
		[](const glm::vec3& a, const glm::vec3& b) {return a + b; }
	);

	luaManager->luaState["CrossVectors"] = sol::overload(
		[](const glm::vec3& a, const glm::vec3& b) {return glm::cross(a, b); }
	);

	luaManager->luaState["DotVectors"] = sol::overload(
		[](const glm::vec3& a, const glm::vec3& b) {return glm::dot(a, b); }
	);

	luaManager->luaState["CloseWindow"] = sol::overload([](bool shouldClose) { Window::GetActiveWindow()->CloseWindow(); });

	//expose vec2
	luaManager->Expose_CPPClass<glm::vec2>("vec2",
		sol::constructors<glm::vec2(), glm::vec2(float, float)>(),
		"x", &glm::vec2::x,
		"y", &glm::vec2::y
	);


	//expose animation struct
	luaManager->Expose_CPPClass<animation>("animation",
		sol::constructors<animation(), animation(int, int, int)>(),
		"start", &animation::start,
		"end", &animation::end,
		"speed", &animation::speed
	);


	//expose texture
	luaManager->Expose_CPPClass<Texture>("Texture",
		sol::no_constructor
	);

	//expose draw item
	luaManager->Expose_CPPClass<AnimatedDrawItem>("AnimatedDrawItem",
		sol::no_constructor,
		sol::base_classes, sol::bases<DrawItem>(),
		"SetAnimationSpeed", &AnimatedDrawItem::SetAnimationSpeed,
		"SetAnimation", &AnimatedDrawItem::SetAnimation,
		"Animate", &AnimatedDrawItem::Animate
		);


	//expose draw item
	luaManager->Expose_CPPClass<md2_model_t>("MD2",
		sol::no_constructor,
		sol::base_classes, sol::bases<AnimatedDrawItem>()
	);
	

	//expose game object
	luaManager->Expose_CPPClass<GameObject>("GameObject",
		sol::constructors<GameObject()>(),
		"name", &GameObject::name,
		"position", &GameObject::position,
		"scale", &GameObject::scale,
		"orientation", &GameObject::orientation,
		"SetPosition", &GameObject::SetPosition,
		"SetRotation", &GameObject::SetRotation,
		"SetRotationEuler", &GameObject::SetRotationEuler,
		"GetDrawItem", &GameObject::GetDrawItem,
		"LookAt", &GameObject::LookAt,
		"GetID", &GameObject::GetID,
		"physicsBody", &GameObject::physicsBody,
		"affordances", &GameObject::affordanceController,
		"GetForward", &GameObject::GetForwardVec,
		"Rotate", &GameObject::Rotate,
		"GetAnimation", &GameObject::GetAnimationItem,
		"stateMachine", &GameObject::stateMachine
	);

	//expose terrain
	luaManager->Expose_CPPClass<Terrain>("Terrain",
		sol::constructors<Terrain(), Terrain(std::string&, float, float, float, float)>(),
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

	//expose emotion
	luaManager->Expose_CPPClass<Emotion>("Emotion",
		sol::constructors<Emotion()>(),
		"emotion", &Emotion::emotion,
		"emotionStrength", &Emotion::emotionStrength,
		"reactionStrength", &Emotion::reactionStrength
	);

	//expose NPC
	luaManager->Expose_CPPClass<NPC>("NPC",
		sol::constructors<NPC()>(),
		sol::base_classes, sol::bases<GameObject>(),
		"MoveToPoint", &NPC::MoveToPoint,
		"SetMoveSpeed", &NPC::SetMoveSpeed,
		"CancelPath", &NPC::CancelPath,
		"FindClosestNode", &NPC::FindClosestNode,
		"FindFurthestNode", &NPC::FindFurthestNode,
		"FindRandomNode", &NPC::FindRandomNode,
		"GetIsMoving", &NPC::GetIsMoving,

		"GetEmotion", &NPC::GetEmotion,
		"GetLastInterracted", &NPC::GetLastInterracted,
		"GetPersonality", &NPC::GetPersonality
	);

	//expose resource manager class
	luaManager->Expose_CPPClass<ResourceManager>("ResourceManager",
		sol::no_constructor,
		"Get", &ResourceManager::Get,

		"CreateGameObject", &ResourceManager::CreateGameObject,
		"CreateTerrain", &ResourceManager::CreateTerrain,
		"LoadTexture", &ResourceManager::LoadTexture,
		"LoadModel", &ResourceManager::LoadModel,
		"LoadAnimatedModel", &ResourceManager::LoadAnimatedModel,
		"LoadShader", &ResourceManager::LoadShader,
		"LoadCubemap", &ResourceManager::LoadCubemap,
		"CreateMaterial", &ResourceManager::CreateMaterial,

		"GetCubeMap", &ResourceManager::GetCubeMap,
		"GetGameObject", &ResourceManager::GetGameObject,
		"GetModel", &ResourceManager::GetModel,
		"GetShader", &ResourceManager::GetShader,
		"GetTexture", &ResourceManager::GetTexture,


		"GetModel", &ResourceManager::GetDrawItemReference
	);
	//expose resource manager singleton
	luaManager->luaState["resources"] = &ResourceManager::Get();

	//expose skybox
	luaManager->Expose_CPPClass<CubeMap>("CubeMap",
		sol::constructors<CubeMap, CubeMap(std::vector<std::string>&)>()
	);

	//expose scene object
	luaManager->Expose_CPPClass<Scene>("Scene",
		sol::constructors<Scene()>(),
		"AddObject", &Scene::AddObject,
		"AddLights", &Scene::AddLights,
		"SetSkybox", &Scene::SetSkybox,
		"GetCamera", &Scene::GetCamera,
		"GetLights", &Scene::GetLights,
		"GetObject", &Scene::GetGameObject
	);

	//expose camera
	luaManager->Expose_CPPClass<Camera>("Camera",
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
		"yaw", &Camera::Yaw,
		"pitch", &Camera::Pitch,
		"UpdateCameraVectors", &Camera::UpdateCameraVectors
	);

	//expose lights
	luaManager->Expose_CPPClass<PointLight>("PointLight",
		sol::constructors<PointLight, PointLight(glm::vec3, glm::vec3, glm::vec3, float, float, float)>(),
		"position", &PointLight::position,
		"diffuse", &PointLight::diffuse,
		"quadratic", &PointLight::quadratic,
		"linear", &PointLight::linear,
		"constant", &PointLight::constant
	);

	luaManager->Expose_CPPClass<SpotLight>("SpotLight",
		sol::constructors<SpotLight, SpotLight(glm::vec3, glm::vec3, float, float, glm::vec3, glm::vec3, float, float, float)>(),
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
	luaManager->Expose_CPPClass<DirectionLight>("DirectionLight",
		sol::constructors<DirectionLight, DirectionLight(glm::vec3, glm::vec3, glm::vec3)>(),
		"direction", &DirectionLight::direction,
		"diffuse", &DirectionLight::diffuse,
		"specular", &DirectionLight::specular
	);
	luaManager->Expose_CPPClass<Lights>("Lights",
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
	luaManager->Expose_CPPClass<InputManager>("InputManager",
		sol::no_constructor,
		"Get", &InputManager::Get,
		"BindKey", &InputManager::BindKey,
		"GetKeyState", &InputManager::GetKeyState,
		"SetMouseLock", &InputManager::SetMouseLock,
		"GetMouseX", &InputManager::GetMouseX,
		"GetMouseY", &InputManager::GetMouseY,
		"GetScrollOffset", &InputManager::GetScrollOffset,
		"GetMouseLock", &InputManager::GetMouseLock,
		"GetKeyDown", &InputManager::GetKeyPressedDown
	);
	luaManager->luaState["input"] = &InputManager::Get();


	//expose physics
	luaManager->Expose_CPPClass<PhysicsManager>("PhysicsManager",
		sol::no_constructor,
		"CreatePhysicsBody", &PhysicsManager::CreatePhysicsBody,
		"AddSphereCollider", &PhysicsManager::AddSphereCollider,
		"AddBoxCollider", &PhysicsManager::AddBoxCollider,
		"AddCapsuleCollider", &PhysicsManager::AddCapsuleCollider,
		"GetPhysicsBody", &PhysicsManager::GetPhysicsBody,
		"Raycast",&PhysicsManager::Raycast,
		"RaycastNPC",&PhysicsManager::RaycastNPC
	);
	

	luaManager->Expose_CPPClass<PhysicsBody>("PhysicsBody",
		sol::no_constructor,
		"ApplyForce", &PhysicsBody::ApplyForce,
		"ApplyForceImpulse", &PhysicsBody::ApplyForceImpulse,
		"ApplyTorque", &PhysicsBody::ApplyTorque,
		"GetPosition", &PhysicsBody::GetPosition,
		"GetRotation", &PhysicsBody::GetRotation,
		"SetPosition", &PhysicsBody::SetPosition,
		"SetRotation", &PhysicsBody::SetRotation,
		"SetVelocity", &PhysicsBody::SetVelocity,
		"SetMass", &PhysicsBody::SetMass,
		"SetGravity", &PhysicsBody::SetGravity,
		"Kinematic", &PhysicsBody::isKinematic,
		"GetID", &PhysicsBody::GetID
	);


	//expose GUI
	luaManager->Expose_CPPClass<GUIRenderer>("GUIRenderer",
		sol::no_constructor,
		"StartGUI", &GUIRenderer::StartGUI,
		"EndGUI", &GUIRenderer::EndGUI,
		"StartWindow", &GUIRenderer::StartWindow,
		"EndEndWindow", &GUIRenderer::EndWindow,
		"Text", &GUIRenderer::Text,
		"Button", &GUIRenderer::Button,
		"Image", &GUIRenderer::Image,
		"ImageButton", &GUIRenderer::ImageButton,
		"Tab", &GUIRenderer::Tab,
		"SetCursorPosX", &GUIRenderer::SetCursorPosX,
		"SetCursorPosY", &GUIRenderer::SetCursorPosY,
		"SetFont", &GUIRenderer::SetFont,
		"GetWindowWidth", &GUIRenderer::GetWindowWidth,
		"GetWindowHeight", &GUIRenderer::GetWindowHeight
	);

	luaManager->Expose_CPPClass<Renderer>("Renderer",
		sol::no_constructor,
		"ToggleWireFrame", &Renderer::ToggleWireFrame,
		"GetFPS", &Renderer::GetFPS
	);

	luaManager->Expose_CPPClass<StateMachine>("StateMachine",
		sol::no_constructor,
		"ChangeState", &StateMachine::ChangeState,
		"ChangeGlobalState", &StateMachine::ChangeGlobalState,
		"RevertState", &StateMachine::RevertState
		);

	luaManager->Expose_CPPClass<State>("State",
		sol::no_constructor,
		"Enter", &State::Enter,
		"Exit", &State::Exit,
		"Update", &State::Update,
		"ProcessMessage", &State::ProcessMessage
		);

	luaManager->Expose_CPPClass<AIManager>("AIManager",
		sol::no_constructor,
		"AddState", &AIManager::AddState,
		"GetState", &AIManager::GetState,
		"SendMessage", &AIManager::SendMessage
		);

	luaManager->Expose_CPPClass<Message>("Message",
		sol::no_constructor,
		"dispatchTime", &Message::dispatchTime,
		"msgType", &Message::msgType,
		"receiverID", &Message::receiverID,
		"senderID", &Message::senderID
		);		

	luaManager->Expose_CPPClass<ScriptableState>("ScriptableState",
		sol::constructors<ScriptableState(sol::function, sol::function, sol::function, sol::function)>(),
		sol::base_classes, sol::bases<State>()
		);

	//expose the sound engine
	luaManager->Expose_CPPClass<SoundEngine>("SoundEngine",
		sol::no_constructor,
		"playSoundFromFile", &SoundEngine::PlaySoundFromFile,
		"playSound", &SoundEngine::PlaySound,
		"playLoop", &SoundEngine::PlayLoop,
		"addSound", &SoundEngine::AddSound,
		"playSoundAtPosition", &SoundEngine::PlaySoundAtPosition,
		"playLoopAtPosition", &SoundEngine::PlayLoopAtPosition,
		"setUserPosition", &SoundEngine::SetUserPosition
	);
	luaManager->luaState["Sound"] = &SoundEngine::Get();

	//expose affordances
	luaManager->Expose_CPPClass<Affordance>("Affordance",
		sol::no_constructor,
		"Activate", &Affordance::Activate,
		"Deactivate", &Affordance::Deactivate,
		"SetCanAfford", &Affordance::SetCanAfford,
		"GetType", &Affordance::GetType,
		"GetIsActive", &Affordance::GetIsActive
	);

	luaManager->Expose_CPPClass<AffordanceController>("AffordanceController",
		sol::no_constructor,
		"GetAffordance", &AffordanceController::GetAffordanceString,
		"RemoveAffordance", &AffordanceController::RemoveAffordanceString
	);

}
