dofile("resources/scripts/keybinds.lua")
dofile("resources/scripts/ai_states.lua")

--main init function, called once before update
function init()
	initInputs();
	player = resources:GetGameObject("player");
	expose_states();

	init_AI();

	input:SetMouseLock(true);
	print("init lua");
end

--main update function, called every frame
function update(deltaTime)
	

	--show emotional state of NPC looking at
	object = physics:RaycastNPC(scene:GetCamera().position,scene:GetCamera().front,20);
	if(not exitMenuOpen)
	then
		draw_instruction_gui();
		if(not (object == nil))
		then
			draw_emotion_gui(object);
		end
	end

	--toggle exit splash
	if(input:GetKeyDown(KEY_ESCAPE))
	then
		exitMenuOpen = not exitMenuOpen;
	end
	
	--use player inputs or show exit
	if(not exitMenuOpen)
	then
		keyInput(deltaTime);
		mouseMoveFunc(deltaTime);
	else
		showExitSplash();
	end
	
end

function initInputs()

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

	forward = camera.front:multiply(12.0);

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

	if(input:GetKeyDown(KEY_F))
	then
		if(player.affordances:GetAffordance("pickup"):GetIsActive())
		then
			player.affordances:GetAffordance("pickup"):Deactivate();
		else
			object = physics:Raycast(camera.position,camera.front,3);
			if(not(object == nil))
			then
				player.affordances:GetAffordance("pickup"):Activate(object);
			end
		end
	end

	if(input:GetKeyDown(KEY_Q))
	then
		if(player.affordances:GetAffordance("pickup"):GetIsActive())
		then
			player.affordances:GetAffordance("pickup"):Deactivate();
			object = physics:Raycast(camera.position,camera.front,3);
			if(not(object == nil))
			then
				object.physicsBody:ApplyForceImpulse(forward.x * 3,forward.y * 3,forward.z * 3);
				object.physicsBody.Kinematic = false;
			end
		end
	end

	if(input:GetKeyDown(KEY_M))
	then
		if(player.affordances:GetAffordance("giveMoney"):GetIsActive())
		then
			player.affordances:GetAffordance("giveMoney"):Deactivate();
		else
			object = physics:Raycast(camera.position,camera.front,5);
			if(not(object == nil))
			then
				player.affordances:GetAffordance("giveMoney"):Activate(object);
			end
		end
	end

	if(input:GetKeyDown(KEY_N))
	then
		if(player.affordances:GetAffordance("compliment"):GetIsActive())
		then
			player.affordances:GetAffordance("compliment"):Deactivate();
		else
			object = physics:Raycast(camera.position,camera.front,5);
			if(not(object == nil))
			then
				player.affordances:GetAffordance("compliment"):Activate(object);
			end
		end
	end

	if(input:GetKeyDown(KEY_T))
	then
		if(player.affordances:GetAffordance("threaten"):GetIsActive())
		then
			player.affordances:GetAffordance("threaten"):Deactivate();
		else
			object = physics:Raycast(camera.position,camera.front,5);
			if(not(object == nil))
			then
				player.affordances:GetAffordance("threaten"):Activate(object);
			end
		end
	end

	if(input:GetKeyDown(KEY_G))
	then
		if(player.affordances:GetAffordance("generousOffer"):GetIsActive())
		then
			player.affordances:GetAffordance("generousOffer"):Deactivate();
		else
			object = physics:Raycast(camera.position,camera.front,5);
			if(not(object == nil))
			then
				player.affordances:GetAffordance("generousOffer"):Activate(object);
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

function draw_emotion_gui(go)

	wWidth = GUI:GetWindowWidth();
	wHeight = GUI:GetWindowHeight();
	wRatio = wWidth/wHeight;

	sWidth = 0.1;
	sHeight = (sWidth) * wRatio;

	GUI:StartWindow("emotiongui",false,sWidth,sHeight + 0.2,0.35,sWidth/ wRatio);
		
		local anger = go:GetEmotion("Anger").emotionStrength;
		local fear = go:GetEmotion("Fear").emotionStrength;
		local grat = go:GetEmotion("Gratitude").emotionStrength;
		local hope = go:GetEmotion("Hope").emotionStrength;

		local image = "Neutral";
		if((anger + fear + grat + hope) > 0.0)
		then

			if((anger > grat) and (anger > hope))
			then
				image = "Angry"

			elseif((fear > grat) and (fear > hope))
			then
				image = "Fear"

			elseif(grat > hope)
			then
				image = "Positive"
			else

				image = "Hope"
			end
		end

		GUI:ImageButton(image,(wWidth * sWidth) * 0.8,(wHeight * sHeight) * 0.8,0.5,0.5)
		GUI:Text("--Emotions--");
		
		GUI:Text("Anger:"..anger);
		GUI:Text("Fear:"..fear);
		GUI:Text("Gratitude:"..grat);
		GUI:Text("Hope:"..hope);

	GUI:EndEndWindow();
end

exitMenuOpen = false;
function showExitSplash()
	input:SetMouseLock(false);

	wWidth = GUI:GetWindowWidth();
	wHeight = GUI:GetWindowHeight();
	wRatio = wWidth/wHeight;

	sWidth = 0.3;
	sHeight = sWidth * wRatio;

	GUI:StartWindow("ExitSplash",false,sWidth,sHeight + 0.07,0.35,sWidth/ wRatio);

		if(GUI:ImageButton("ExitSplash",(wWidth * sWidth),(wHeight * sHeight),0.5,0.5))
		then
			CloseWindow(true);
		end

	GUI:EndEndWindow();
end

function draw_instruction_gui()
	GUI:StartWindow("poop",true,0.15,0.3,0,0);
	
		GUI:Text("----Affordance Controls----",0);
		GUI:Tab(10);

		GUI:Text("Interation Affodances:",0);
		GUI:Text("[E] Sit",0);
		GUI:Text("[F] Pickup",0);


		GUI:Text("Negative Affodances:",0);
		GUI:Text("[I] Poke",0);
		GUI:Text("[O] Slap",0);
		GUI:Text("[P] Punch",0);

		GUI:Text("[T] Threaten",0);

		GUI:Text("Positive Affodances:",0);
		GUI:Text("[N] compliment",0);
		GUI:Text("[M] Give Money",0);

		GUI:Tab(10);
		GUI:Text("----Player Controls----",0);
		GUI:Tab(10);
		GUI:Text("[W][A][S][D] Move ",0);
		GUI:Text("[Mouse] Look ",0);
		GUI:Text("[ESC] Exit Game ",0);

	GUI:EndEndWindow();
end

function init_AI()
	robot = resources:GetGameObject("Robot");
	robot.stateMachine:ChangeState(idle_state);
	robot.stateMachine:ChangeGlobalState(global_state);

	robot1 = resources:GetGameObject("Robot1");
	robot1.stateMachine:ChangeState(idle_state);
	robot1.stateMachine:ChangeGlobalState(global_state);

	robot2 = resources:GetGameObject("Robot2");
	robot2.stateMachine:ChangeState(idle_state);
	robot2.stateMachine:ChangeGlobalState(global_state);
end