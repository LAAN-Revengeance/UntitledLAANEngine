dofile("resources/scripts/keybinds.lua")

--main init function, called once before update
function init()

	--set inputs
	input:SetMouseLock(false);
	input:BindKey("camF",KEY_W);
	input:BindKey("camB",KEY_S);
	input:BindKey("camU",KEY_SPACE);
	input:BindKey("camD",KEY_LEFT_SHIFT);
	input:BindKey("camL",KEY_A);
	input:BindKey("camR",KEY_D);
	input:BindKey("escape",KEY_ESCAPE);

	scene:GetCamera().position = vec3:new(0,2,0);

	print("init lua");
end

--main update function, called every frame
function update(deltaTime)
	
	keyInput(deltaTime)
	mouseMoveFunc(deltaTime)
end


function keyInput(dt)
	up = vec3:new(0,1,0);
	camSpeed = 5 * dt;
	local camera = scene:GetCamera();
	player = resources:GetGameObject("Player");
	print(player.name)
	if(input:GetKeyState("camF"))
	then
		player = player.position +  NormalizeVector(CrossVectors(up,camera.right)):multiply(camSpeed);
	end
	if(input:GetKeyState("camB"))
	then
		player.position = player.position -  NormalizeVector(CrossVectors(up,camera.right)):multiply(camSpeed);
			
	end
	if(input:GetKeyState("camL"))
	then
		player.position = player.position - camera.right:multiply(camSpeed);
	end
	if(input:GetKeyState("camR"))
	then
		player.position = player.position + camera.right:multiply(camSpeed);
	end

	if(input:GetKeyState("escape"))
	then
		--engine:Shutdown();
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