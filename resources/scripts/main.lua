dofile("resources/scripts/keybinds.lua")

--main init function, called once before update
function init()

	-- set window properties
	--engine:SetWindowIcon("resources/textures/icon.png");
	--engine:SetWindowName("InitalZ");

	--set inputs
	input:SetMouseLock(false);
	input:BindKey("camF",KEY_W);
	input:BindKey("camB",KEY_S);
	input:BindKey("camU",KEY_SPACE);
	input:BindKey("camD",KEY_LEFT_SHIFT);
	input:BindKey("camL",KEY_A);
	input:BindKey("camR",KEY_D);
	input:BindKey("escape",KEY_ESCAPE);


	--load resources
	resources:LoadTexture("grass","resources/textures/terrain/grass.png");
	resources:LoadTexture("rock","resources/textures/terrain/rock.png");
	resources:LoadTexture("dirt","resources/textures/terrain/dirt.png");
	resources:LoadTexture("heightMap","resources/textures/heightmap/heightmap512_rivers.png");
	resources:LoadTexture("detailMap","resources/textures/terrain/detail.png");

	resources:LoadTexture("arcade","resources/models/untitled2022/Arcade.png");
	resources:LoadModel("arcade","resources/models/untitled2022/Arcade.obj","arcade","","");

	
	resources:LoadCubemap("skybox",
		"resources/textures/skybox/Synthwave3/Right.png",
		"resources/textures/skybox/Synthwave3/Left.png",
		"resources/textures/skybox/Synthwave3/Top.png",
		"resources/textures/skybox/Synthwave3/Bottom.png",
		"resources/textures/skybox/Synthwave3/Front.png",
		"resources/textures/skybox/Synthwave3/Back.png"
	);
	
	--load scene
	terrain = resources:CreateTerrain("Terrain","heightMap",{"dirt","grass","rock"},"detailMap","detailMap","", 200 , 12,2,12);
	terrain:GetDrawItem():SetShine(20);
	terrain:SetTextureHeights({-30,-5,40});
	scene:AddObject(terrain);
	lighting = scene:GetLights();
	lighting:SetAmbient(0.3,0.3,0.3);
	lighting:AddDirectionLight(NormalizeVector(vec3.new(0,0.5,-1)),vec3.new( 0.0,0.67,0.8),vec3.new(0,1,1));

	--add arcade with sphere collider
	arcade = resources:CreateGameObject("arcade","arcade","");
	scene:AddObject(arcade);
	arcadeCollider = physics:AddPhysicsBody(arcade);
	physics:AddSphereCollider(arcadeCollider,1.0);

	--setup camera 
	camera = scene:GetCamera();
	camera.farPlane = 10000;
	camera.position.x = 0;
	camera.position.y = 0;
	camera.position.z = 0;

	--setup skybox
	scene:SetSkybox(resources:GetCubeMap("skybox"));

end

--main update function, called every frame
function update(deltaTime)
	keyInput(deltaTime)
	mouseMoveFunc(deltaTime)
end


function keyInput(dt)
	up = vec3:new(0,1,0);
	camSpeed = 100 * dt;
	if(input:GetKeyState("camF"))
	then
		camera.position = camera.position +  NormalizeVector(CrossVectors(up,camera.right)):multiply(camSpeed);
	end
	if(input:GetKeyState("camB"))
	then
		camera.position = camera.position -  NormalizeVector(CrossVectors(up,camera.right)):multiply(camSpeed);
			
	end
	if(input:GetKeyState("camU"))
	then
		camera.position = camera.position + up:multiply(camSpeed);
			
	end
	if(input:GetKeyState("camD"))
	then
		camera.position = camera.position - up:multiply(camSpeed);
			
	end
	if(input:GetKeyState("camL"))
	then
		camera.position = camera.position - camera.right:multiply(camSpeed);
	end
	if(input:GetKeyState("camR"))
	then
		camera.position = camera.position + camera.right:multiply(camSpeed);
	end

	if(input:GetKeyState("escape"))
	then
		engine:Shutdown();
	end

end

lastX = input:GetMouseX();
lastY = input:GetMouseY();
mouseSensitivity = 0.1
Distance = 10
moveSpeed = 100


function mouseMoveFunc(dt)

	local camera = scene:GetCamera();
	xPos = input:GetMouseX();
	yPos = input:GetMouseY();
	
	xoffset = (xPos - lastX)
	yoffset = (lastY - yPos)

	lastX = xPos
	lastY = yPos

	xoffset = xoffset * -mouseSensitivity
	yoffset = yoffset * -mouseSensitivity


	camera.Yaw =  camera.Yaw - xoffset
	camera.Pitch = camera.Pitch - yoffset

	--CalaulateCamPos();
	camera:UpdateCameraVectors();
end