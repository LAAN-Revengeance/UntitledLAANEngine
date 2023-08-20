


--load menu texutres
resources:LoadTexture("driver"	,"resources/textures/GUI/DRIVE.png");
resources:LoadTexture("exlr8"	,"resources/textures/GUI/exlr8.png");
resources:LoadTexture("friction","resources/textures/GUI/friction.png");
resources:LoadTexture("gas"		,"resources/textures/GUI/GAS.png");
resources:LoadTexture("exit"		,"resources/textures/exit.png");

--current menu in use
current_menu = 1;

--gui aesthetics
buttonWidth = 300;
buttonHeight = 50;

--array each menu with assigned index
local cases = {
	[1] = function()
		input:SetMouseLock(true);
		engine:SetSimulation(false);
		draw_main_menu();
	end,
	[2] = function()
		input:SetMouseLock(true);
		engine:SetSimulation(false);
		draw_save_menu();
	end,
	[3] = function()
		input:SetMouseLock(true);
		engine:SetSimulation(false);
		draw_options_menu();
	end,
	[4] = function()
		input:SetMouseLock(false);
		engine:SetSimulation(true);
		draw_ingame();
	end,
	[5] = function()
		input:SetMouseLock(true);
		engine:SetSimulation(false);
		draw_window_option();
	end,
	[6] = function()
		input:SetMouseLock(true);
		engine:SetSimulation(false);
		draw_exit();
	end,
	[7] = function()
		input:SetMouseLock(true);
		engine:SetSimulation(false);
		draw_lose();
	end,
	[8] = function()
		input:SetMouseLock(true);
		engine:SetSimulation(false);
		draw_instructions_menu();
	end
}

--called every update, renders current menu
function draw_menu()
	if(cases[current_menu])
	then
		cases[current_menu]();
	end;	
end


function draw_main_menu()

	GUI:Start(true);
	GUI:Tab(300);
	GUI:Text("Main Menu",0.5);

	GUI:Tab(10);

	if(GUI:Button("Start Game",0.5, buttonWidth, buttonHeight))
	then
		Sound:playSound("click",camera.position);
		current_menu = 4;
	end

	if(GUI:Button("Instructions",0.5, buttonWidth, buttonHeight))
	then
		Sound:playSound("click",camera.position);
		current_menu = 8;
	end

	if(GUI:Button("Load Game",0.5, buttonWidth, buttonHeight))
	then
		Sound:playSound("click",camera.position);
		current_menu = 2;
	end

	if(GUI:Button("Options",0.5, buttonWidth, buttonHeight))
	then
		Sound:playSound("click",camera.position);
		current_menu = 3;
	end

	if(GUI:Button("Exit",0.5, buttonWidth, buttonHeight))
	then
		Sound:playSound("click",camera.position);
		current_menu = 6
	end

	GUI:End();
end


function draw_save_menu()

	GUI:Start(true);

	GUI:Tab(300);
	GUI:Text("Save Current Game",0.5);
	GUI:Tab(20);

	saves = loader:GetSaves("resources/saves");

	if(GUI:Button("--Save--",0.5, buttonWidth, buttonHeight))
	then
		Sound:playSound("click",camera.position);
		loader:SaveScene(engine.scene,"resources/saves/save"..(#saves + 1)..".json");
	end
	GUI:Tab(20);
	GUI:Text("Choose a save to load:",0.5);
	GUI:Tab(10);
	for i, v in ipairs(saves)
	do
		if(GUI:Button(v, 0.5, buttonWidth, buttonHeight))
		then
			Sound:playSound("click",camera.position);
			nscene = loader:LoadScene("resources/saves/"..v);
			engine:SwitchScenes(nscene);
			scene = nscene;
		end
    end
	GUI:Tab(30);

	if(GUI:Button("Back",0.5, buttonWidth, buttonHeight))
	then
		Sound:playSound("click",camera.position);
		current_menu = 1;
	end

	GUI:End();
end

function draw_options_menu()

	GUI:Start(true);

	GUI:Tab(300);
	GUI:Text("Options",0.5);
	GUI:Tab(10);
	if(GUI:Button("Window Options",0.5, buttonWidth, buttonHeight))
	then
		Sound:playSound("click",camera.position);
		current_menu = 5;
	end

	if(GUI:Button("Back",0.5, buttonWidth, buttonHeight))
	then
		Sound:playSound("click",camera.position);
		current_menu = 1;
	end

	GUI:End();
end

function draw_ingame()

	GUI:Start(false);

	GUI:Tab(50);
	GUI:Text("Health: " .. math.floor(Player:GetData("health")), 0.9);
	GUI:Text("Score: " ..  math.floor(Player:GetData("score")), 0.9);
	GUI:Text("Boost: " ..  math.floor(Player:GetData("boost") + 1), 0.9);
	
	--GUI:SetCursorPosX(0)
	--GUI:SetCursorPosY(0)
	--GUI:Text("in-game",0.5);
	--local fps = math.floor(renderer:GetFPS() + 0.5); 
	--GUI:Text((fps.."fps"),0.5);

	GUI:End();
end

function draw_window_option()

	GUI:Start(true);
	GUI:Tab(300);
	GUI:Text("Change Window Mode",0.5);
	GUI:Tab(10);

	if(GUI:Button("Windowed",0.5, buttonWidth, buttonHeight))
	then
		Sound:playSound("click",camera.position);
		engine:SetWindowType(1);
	end
	if(GUI:Button("Borderless",0.5, buttonWidth, buttonHeight))
	then
		Sound:playSound("click",camera.position);
		engine:SetWindowType(2);
	end
	if(GUI:Button("Full Screen",0.5, buttonWidth, buttonHeight))
	then
		Sound:playSound("click",camera.position);
		engine:SetWindowType(3);
	end

	if(GUI:Button("Back",0.5, buttonWidth, buttonHeight))
	then
		Sound:playSound("click",camera.position);
		current_menu = 3;
	end

	GUI:End();
end

function draw_exit()

	GUI:Start(true);

	if(GUI:ImageButton("exit",500,500,0.5,0.5))
	then
		engine:Shutdown();
	end
	
	GUI:Tab(10);

	if(GUI:Button("Back",0.5, buttonWidth, buttonHeight))
	then
		Sound:playSound("click",camera.position);
		current_menu = 1;
	end

	GUI:End();
end

function draw_lose()
	
	GUI:Start(true);

	GUI:Tab(300);
	GUI:Text("YOU DIED",0.5);
	GUI:Text("Score: " ..  math.floor(Player:GetData("score")), 0.5);
	if(GUI:Button("Back",0.5, buttonWidth, buttonHeight))
	then
		Sound:playSound("click",camera.position);

		--send message to reset npcs
		local delay = 0;
		local sender = -1;
		local receiver = -1;
		local type = 0;
		aimanager:SendMessage(delay, sender, receiver, type);

		--reset player
		Player:SetPosition(vec3:new(800,35,500));
		Player:AddData("health", 100);
		Player:AddData("score", 0);
		Player:AddData("boost", 99);
		Player:AddData("isAlive", 1);
		Player.rigidBody:SetLinearVelocity(0,0,0);
		Player.rigidBody:SetAngularVelocity(0,0,0);

		current_menu = 1;
	end

	GUI:End();
end

function draw_instructions_menu()

	GUI:Start(true);

	GUI:Tab(300);
	GUI:Text("How To Play:",0.5);
	GUI:Text("Run over a robot to start. Run over small robots to score.",0.5);
	GUI:Text("Avoid The large robots.",0.5);
	GUI:Text("Avoid moving slowly, or the small robots will have a chance to hit you.",0.5);

	GUI:Tab(20);
	GUI:Text("--Controls--",0.5);
	GUI:Tab(20);
	GUI:Text("Accelerate---------[W]",0.5);
	GUI:Text("Reverse---------------[S]",0.5);
	GUI:Tab(10);
	GUI:Text("Steer Left----------[A]",0.5);
	GUI:Text("Steer Right---------[D]",0.5);
	GUI:Tab(10);
	GUI:Text("Boost--------------[SHIFT]",0.5);
	GUI:Text("Drift-----------------[SPACE]",0.5);
	GUI:Tab(10);
	GUI:Text("Move Camera-------------[Mouse Move]",0.5);
	GUI:Text("Camera Zoom------------------[Scroll Wheel]",0.5);
	GUI:Tab(10);
	GUI:Text("Pause-------------------[X]",0.5);
	GUI:Tab(10);
	GUI:Text("Wireframe Mode-[K]",0.5);
	GUI:Tab(20);
	if(GUI:Button("Back",0.5, buttonWidth, buttonHeight))
	then
		Sound:playSound("click",camera.position);
		current_menu = 1;
	end

	GUI:End();
end