
dofile("resources/scripts/keybinds.lua")
dofile("resources/scripts/load_resources.lua")

--main init function, called once before update
function init()

	-- set window properties
	engine:SetWindowIcon("resources/textures/icon.png");
	engine:SetWindowName("InitalZ");
	GUI:SetFont("resources/fonts/Excluded-z8XrX.ttf");

	--set inputs
	input:SetMouseLock(false);
	input:BindKey("camF",KEY_W);
	input:BindKey("camB",KEY_S);
	input:BindKey("camU",KEY_SPACE);
	input:BindKey("camD",KEY_LEFT_SHIFT);
	input:BindKey("camL",KEY_A);
	input:BindKey("camR",KEY_D);


	--load resources
	load_resources();
	
	--load scene
	terrain = resources:CreateTerrain("Terrain","heightMap",{"dirt","grass","rock"},"detailMap","detailMap","", 100 , 12,0.8,12);
	terrain:GetDrawItem():SetShine(20);
	terrain:SetTextureHeights({-30,-5,40});
	scene:AddObject(terrain);
	
	--setup camera 
	camera = scene:GetCamera();
	camera.farPlane = 10000;
	--scene:SetSkybox(resources:GetCubeMap("skybox"));

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