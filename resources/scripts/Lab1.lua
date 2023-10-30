dofile("resources/scripts/keybinds.lua")

--main init function, called once before update
function init()

	--set inputs
	input:SetMouseLock(false);
	input:BindKey("Fire",KEY_SPACE);

	--set inputs
	input:SetMouseLock(false);
	input:BindKey("camF",KEY_W);
	input:BindKey("camB",KEY_S);
	input:BindKey("camU",KEY_SPACE);
	input:BindKey("camD",KEY_LEFT_SHIFT);
	input:BindKey("camL",KEY_A);
	input:BindKey("camR",KEY_D);
	input:BindKey("pickUp", KEY_E);
	input:BindKey("escape",KEY_ESCAPE);

	scene:GetCamera().position = vec3:new(0,0.85,0);

	--scene:GetCamera().position = vec3:new(0,0,-3);
	material = resources:CreateMaterial("Grass","","");
	
	print("init lua");
end

--main update function, called every frame
function update(deltaTime)
	keyInput(deltaTime)
	mouseMoveFunc(deltaTime)
end

numThings = 0;
spread = 20;
upforce = 70;
keyPressed = false;

function keyInput(dt)
	up = vec3:new(0,1,0);
	camSpeed = 5 * dt;
	local camera = scene:GetCamera();

	physics:Raycast(camera.position,camera.front,5);

	--print(player.name)
	if(input:GetKeyState("camF"))
	then
		camera.position = (camera.position +  NormalizeVector(CrossVectors(up,camera.right)):multiply(camSpeed));
	end
	if(input:GetKeyState("camB"))
	then
		camera.position = (camera.position -  NormalizeVector(CrossVectors(up,camera.right)):multiply(camSpeed));
	end
	if(input:GetKeyState("camL"))
	then
		camera.position = (camera.position - camera.right:multiply(camSpeed));
	end
	if(input:GetKeyState("camR"))
	then
		camera.position = (camera.position + camera.right:multiply(camSpeed));
	end
	if(input:GetKeyState("pickUp"))
	then
		if(not keyPressed)
		then
			if not(physics:Raycast(camera.position,camera.front,5) == nil)
			then
				test = physics:Raycast(camera.position,camera.front,5);
			end
		end
	end
	--fireBall()
	--camera.position = camera.position + vec3:new(0,2,0);
end

isDown = false;
function fireBall()

	local camera = scene:GetCamera();
	if(input:GetKeyState("Fire"))
	then
		if(not isDown)
		then
			--GameObject& ResourceManager::CreateGameObject(stz`d::string objectName, std::string modelName, std::string shaderName)
			nName = ("object" .. numThings);
			ball = resources:CreateGameObject(nName, "Sphere", "default",material);
		
			ball.scale = vec3:new(0.1,0.1,0.1);
	
			ball.physicsBody = physics:CreatePhysicsBody();
			
			physics:AddSphereCollider(ball.physicsBody,0.1);
			--physics:AddBoxCollider(ball.physicsBody,vec3:new(0.1,0.1,0.1));
			ball.physicsBody:SetMass(0.1);
			ball.physicsBody:SetGravity(false);
			
			ball:SetPosition(camera.position);
			
			forward = camera.front:multiply(2.0);

			ball.physicsBody:ApplyForceImpulse(forward.x,forward.y,forward.z);
			scene:AddObject(ball);
			numThings = numThings + 1;
			--print(nName);
		end
		isDown = true;
	else
		isDown = false;
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

	camera:UpdateCameraVectors();
end

function testFunction(go)
	local camera = scene:GetCamera();
	go:LookAt(camera.position);
end
