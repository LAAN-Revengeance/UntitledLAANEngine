
dofile("resources/scripts/keybinds.lua")


--This File Sets up all the player input events

input:BindKey("forward",KEY_W);
input:BindKey("backward",KEY_S);
input:BindKey("left",KEY_A);
input:BindKey("right",KEY_D);
input:BindKey("escape",KEY_ESCAPE);
input:BindKey("drift",KEY_SPACE);
input:BindKey("boost",KEY_LEFT_SHIFT);
input:BindKey("debug",KEY_K);
input:BindKey("toggleControlMenu",KEY_M);

lastX = input:GetMouseX();
lastY = input:GetMouseY();
mouseSensitivity = 0.1
Distance = 10
moveSpeed = 100


function MouseMoveFunc(dt)
	local camera = scene:GetCamera();
	xPos = input:GetMouseX();
	yPos = input:GetMouseY();
	
	xoffset = (xPos - lastX)
	yoffset = (lastY - yPos)

	lastX = xPos
	lastY = yPos

	xoffset = xoffset * mouseSensitivity
	yoffset = yoffset * mouseSensitivity

	if(not input:GetMouseLock())
	then
		camera.Yaw =  camera.Yaw - xoffset
		camera.Pitch = camera.Pitch - yoffset

		if (camera.Pitch < -89.0)
		then
			camera.Pitch = -89.0;
		end
		
		if (camera.Pitch > 0.0)
		then
			camera.Pitch = 0.0;
		end
	end

	camDistance();
	CalaulateCamPos();
	camera:UpdateCameraVectors();
end

function camDistance()
	scrollOffset = input:GetScrollOffset();
	Distance = Distance - scrollOffset;
	if (Distance < 3.0)
	then
		Distance = 3.0;
	end
	if (Distance > 300.0)
	then
		Distance = 300.0;
	end
end


function CalaulateCamPos()
	local camera = scene:GetCamera();
	yaw = camera.Yaw - 90.0;

	pitch = -camera.Pitch;
	-- calaulate camera using camera pitch and yaw
	x = Distance * math.cos(math.rad(pitch)) * math.sin(math.rad(-yaw));
	y = Distance * math.sin(math.rad(pitch));
	z = Distance * math.cos(math.rad(pitch)) * math.cos(math.rad(yaw));

	local player = scene:GetObject("Player")
	--add player/target location to camera position to move the camera with it
	camera.position.x = player.position.x + -x;
	camera.position.y = player.position.y + y;
	camera.position.z = player.position.z + -z;
end

local debugPress = false;
function KeyPressFunc(dt)

	local player = scene:GetObject("Player")
	local camera = scene:GetCamera();
	local turnspeed = 1500 * dt;
	local movespeed = 4000 * dt;
	local finalMove = movespeed;
	local finalTurn = turnspeed;
	local boostMult = 2;
	local driftMult = 2;
	local playerHeight = player.position.y - terrain:GetHeight(player.position.x,player.position.z);
	--local playerSpeed = Length(player.rigidBody:GetLinearVelocity());

	--print(player.position.x .. " " .. player.position.z);
	if(input:GetKeyState("escape") and  current_menu ~= 1)
	then
		Sound:playSound("pause",camera.position);
		current_menu = 1;
	end


	if(input:GetKeyState("debug") and (not debugPress))
	then
		debugPress = true;
		--renderer:ToggleWireFrame();
		physics:ToggleDebugDisplay();

	elseif(input:GetKeyState("debug"))
	then
		debugPress = true;
	else
		debugPress = false;
	end

	if(input:GetKeyState("toggleControlMenu"))
	then
		controlPress = true;
	end


	local currentBoost = Player:GetData("boost");
	if(input:GetKeyState("boost") and currentBoost > 0)
	then
		Sound:playSoundLoop("boost",camera.position,1);
		Player:AddData("boost", currentBoost - (dt * 20));
	else
		Sound:stopSoundLoop("boost");
	end


	if(math.floor(currentBoost) < 0)
	then
		Player:AddData("boost", 0);
	end

	if(playerHeight  < 1)
	then 
		Player.rigidBody:SetDampeningLinear(0.9);	

		if(input:GetKeyState("boost") and currentBoost > 0)
		then
			finalMove = finalMove * boostMult;
		end

		if(input:GetKeyState("drift") and (input:GetKeyState("forward") or input:GetKeyState("backward")))
		then
			Sound:playSoundLoop("drift",camera.position,1);
			finalTurn = finalTurn * driftMult;
		else
			Sound:stopSoundLoop("drift");
		end

		if(input:GetKeyState("forward"))
		then
			local dir = vec3:new(1,0,0):multiply(-finalMove);
			player.rigidBody:ApplyForceLocal(dir)
		end
		
		if(input:GetKeyState("backward"))
		then
			local dir = vec3:new(1,0,0):multiply(finalMove);
			player.rigidBody:ApplyForceLocal(dir)
		end
		
		if(input:GetKeyState("left"))
		then
			local dir = vec3:new(0,1,0):multiply(finalTurn);
			player.rigidBody:ApplyTorqueLocal(dir)
		end
		
		if(input:GetKeyState("right"))
		then
			local dir = vec3:new(0,1,0):multiply(-finalTurn);
			player.rigidBody:ApplyTorqueLocal(dir)
		end
	else
		Player.rigidBody:SetDampeningLinear(0);
	end
	
end