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

	player = resources:GetGameObject("player");
	--scene:GetCamera().position = vec3:new(0,2,0);
	
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

	camera.position = player.position + vec3:new(0,1,0);

	lookvec = CrossVectors(camera.right, vec3:new(0,1,0));

	player:LookAt(player.position + lookvec);

	if(input:GetKeyDown(KEY_E))
	then
		if(player.affordances:GetAffordance("sit"):GetIsActive())
		then
			player.affordances:GetAffordance("sit"):Deactivate();
		else
			object = physics:Raycast(camera.position,camera.front,3);
			if(not(object == nil))
			then
				player.affordances:GetAffordance("sit"):Activate(object);
			end
		end
	end

	if(input:GetKeyDown(KEY_P))
	then
		if(player.affordances:GetAffordance("punch"):GetIsActive())
		then
			player.affordances:GetAffordance("punch"):Deactivate();
		else
			object = physics:Raycast(camera.position,camera.front,3);
			if(not(object == nil))
			then
				player.affordances:GetAffordance("punch"):Activate(object);
			end
		end
	end
	
	if(input:GetKeyDown(KEY_I))
	then
		if(player.affordances:GetAffordance("poke"):GetIsActive())
		then
			player.affordances:GetAffordance("poke"):Deactivate();
		else
			object = physics:Raycast(camera.position,camera.front,3);
			if(not(object == nil))
			then
				player.affordances:GetAffordance("poke"):Activate(object);
			end
		end
	end

	if(input:GetKeyDown(KEY_O))
	then
		if(player.affordances:GetAffordance("slap"):GetIsActive())
		then
			player.affordances:GetAffordance("slap"):Deactivate();
		else
			object = physics:Raycast(camera.position,camera.front,3);
			if(not(object == nil))
			then
				player.affordances:GetAffordance("slap"):Activate(object);
			end
		end
	end

end

lastX = input:GetMouseX();
lastY = input:GetMouseY();
mouseSensitivity = 0.1

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

	camera.yaw =  camera.yaw - xoffset
	camera.pitch = camera.pitch - yoffset

	if (camera.pitch > 89.0)
	then
		camera.pitch = 89.0
	end
	if (camera.pitch < -89.0)
	then
		camera.pitch = -89.0
	end

	camera:UpdateCameraVectors();
end