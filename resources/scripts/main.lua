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

	print("init lua");
end

--main update function, called every frame
function update(deltaTime)
	
	print("update lua");
	--keyInput(deltaTime)
	--mouseMoveFunc(deltaTime)
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