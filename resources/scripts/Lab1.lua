dofile("resources/scripts/keybinds.lua")

--main init function, called once before update
function init()

	--set inputs
	input:SetMouseLock(false);
	input:BindKey("Fire",KEY_SPACE);
	input:BindKey("camF",KEY_W);
	input:BindKey("camB",KEY_S);
	input:BindKey("camU",KEY_LEFT_SHIFT);
	input:BindKey("camD",KEY_LEFT_CONTROL);
	input:BindKey("camL",KEY_A);
	input:BindKey("camR",KEY_D);
	input:BindKey("one",KEY_1);
	input:BindKey("two",KEY_2);
	input:BindKey("three",KEY_3);
	input:BindKey("four",KEY_4);
	input:BindKey("five",KEY_5);
	input:BindKey("escape",KEY_ESCAPE);
	input:BindKey("rapidfire",KEY_F);

	material = resources:CreateMaterial("Grass","","");
	player = resources:GetGameObject("player");
	
	print("init lua");
end

--main update function, called every frame
function update(deltaTime)
	keyInput(deltaTime)
	mouseMoveFunc(deltaTime)
end

numThings = 0;

isDown = false;
isDown2 = false;
function keyInput(dt)
	up = vec3:new(0,1,0);
	camSpeed = 5 * dt;
	camX = scene:GetCamera().position.x;
	camY = scene:GetCamera().position.y;
	camZ = scene:GetCamera().position.z;

	frontX = scene:GetCamera().front.x;
	frontY = scene:GetCamera().front.y;
	frontZ = scene:GetCamera().front.z;

	local camera = scene:GetCamera();
	--print(player.name)
	if(input:GetKeyState("camF"))
	then
		player:SetPosition(player.position +  NormalizeVector(CrossVectors(up,camera.right)):multiply(camSpeed));
	end
	if(input:GetKeyState("camB"))
	then
		player:SetPosition(player.position -  NormalizeVector(CrossVectors(up,camera.right)):multiply(camSpeed));
	end
	if(input:GetKeyState("camL"))
	then
		player:SetPosition(player.position - camera.right:multiply(camSpeed));
	end
	if(input:GetKeyState("camR"))
	then
		player:SetPosition(player.position + camera.right:multiply(camSpeed));
	end
	if(input:GetKeyState("camU"))
	then
		player:SetPosition(player.position + vec3:new(0, 0.1, 0));
	end
	if(input:GetKeyState("camD"))
	then
		player:SetPosition(player.position  - vec3:new(0, 0.1, 0));
	end
	
	if(input:GetKeyState("two"))
	then
		frontX = frontX * 2;
		frontY = frontY * 2;
		frontZ = frontZ * 2;
	end
	if(input:GetKeyState("three"))
	then
		frontX = frontX * 3;
		frontY = frontY * 3;
		frontZ = frontZ * 3;
	end
	if(input:GetKeyState("four"))
	then
		frontX = frontX * 4;
		frontY = frontY * 4;
		frontZ = frontZ * 4;
	end
	if(input:GetKeyState("five"))
	then
		frontX = frontX * 5;
		frontY = frontY * 5;
		frontZ = frontZ * 5;
	end

	camera.position = player.position;
	
	if(input:GetKeyState("Fire"))
	then
		if(not isDown)
		then
			--GameObject& ResourceManager::CreateGameObject(std::string objectName, std::string modelName, std::string shaderName)
			nName = ("object" .. numThings);
			ball = resources:CreateGameObject(nName, "Sphere", "default",material);

			ball.scale = vec3:new(0.1,0.1,0.1);

			ball.physicsBody = physics:CreatePhysicsBody();
			ball.physicsBody:SetPosition(vec3:new(camX, camY, camZ));
			
			physics:AddSphereCollider(ball.physicsBody,0.1);
			ball.physicsBody:SetMass(0.1);
			ball.physicsBody:SetGravity(false);

			ball.physicsBody:SetVelocity(ball.position + vec3:new(frontX, frontY, frontZ));

			scene:AddObject(ball);
			numThings = numThings + 1;
			--print(nName);
		end
		isDown = true;
	else
		isDown = false;
	end
	if(input:GetKeyState("rapidfire"))
	then
		--GameObject& ResourceManager::CreateGameObject(std::string objectName, std::string modelName, std::string shaderName)
		nName = ("object" .. numThings);
		ball = resources:CreateGameObject(nName, "Sphere", "default",material);
			
		ball.scale = vec3:new(0.1,0.1,0.1);

		ball.physicsBody = physics:CreatePhysicsBody();
		ball.physicsBody:SetPosition(player.position);
			
		physics:AddSphereCollider(ball.physicsBody,0.1);
		ball.physicsBody:SetMass(0.1);
		ball.physicsBody:SetGravity(true);

		ball.physicsBody:SetVelocity(vec3:new(frontX * 5, frontY * 5, frontZ * 5));

		scene:AddObject(ball);
		numThings = numThings + 1;
		--print(nName);
	end
	if(input:GetKeyState("one"))
	then
		if(not isDown2)
		then
			--GameObject& ResourceManager::CreateGameObject(std::string objectName, std::string modelName, std::string shaderName)
			nName = ("object" .. numThings);
			ball = resources:CreateGameObject(nName, "Sphere", "default",material);

			ball.scale = vec3:new(0.1,0.1,0.1);

			ball.physicsBody = physics:CreatePhysicsBody();
			--ball.physicsBody:SetPosition(vec3:new(0, 0, 0));
			
			physics:AddSphereCollider(ball.physicsBody,0.1);
			ball.physicsBody:SetMass(0.1);
			ball.physicsBody:SetGravity(false);

			ball.physicsBody:SetVelocity(vec3:new(-4, 0, 0));

			scene:AddObject(ball);
			numThings = numThings + 1;
			--print(nName);
		end
		isDown2 = true;
	else
		isDown2 = false;
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

	if (camera.Pitch > 89.0)
	then
		camera.Pitch = 89.0
	end
	if (camera.Pitch < -89.0)
	then
		camera.Pitch = -89.0
	end

	camera:UpdateCameraVectors();
end
