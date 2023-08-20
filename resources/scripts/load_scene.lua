
--Creates game objects, places them in the scene and sets scene visuals up
function load_scene()
	
	--Setup scene lighting
	lighting = scene:GetLights();
	lighting:SetAmbient(0.2,0.2,0.2);
	lighting:AddDirectionLight(NormalizeVector(vec3.new( 0,0.5,-1)),vec3.new( 0.0,0.5,0.6),vec3.new(0,1,1));
	lighting:AddPointLight( vec3:new(1140,30,640),
							vec3.new( 1,0,1),
							vec3.new( 0.98,0.8789,0.695),
							1.0,0.007,0.0002);
	
	--Load terrain
	terrain = resources:CreateTerrain("Terrain","heightMap",{"dirt","grass","rock"},"detailMap","detailMap","", 100 , 12,0.8,12);
	terrain:GetDrawItem():SetShine(20);
	terrain:SetTextureHeights({-30,-5,40});
	physics:AddRigidBody(terrain,2);
	physics:AddRigidBodyColliderHeightMap(terrain);
	scene:AddObject(terrain);

	--load Water
	waterHeight = -50;
	water = resources:CreateWater( "watertest", 256, { "water", "flowMap", "DerivHeightMap" }, 50, 100, 1, 100);
	water.position = vec3:new(0,waterHeight,0);
	scene:AddObject(water);

	--Setup Player
	Player = resources:CreateNPCObject("Player", "AE86", "");
	Player.position = vec3:new(800,35,100);
	physics:AddRigidBody(Player,3);
	local scale = vec3:new(1.5,0.2,0.7)
	local mass = 2000;
	local bounce = 0;
	local friction = 0.1;
	physics:AddRigidBodyColliderBox(Player,scale,vec3:new(0,0,0), mass,bounce,friction);
	Player.rigidBody:SetCenterOfMass(vec3:new(0,-2.5,0));
	Player.rigidBody:SetDampeningAngle(10);
	Player.rigidBody:SetDampeningLinear(0.9);
	
	--player stats/health
	Player:AddData("health", 100);
	Player:AddData("score", 0);
	Player:AddData("boost", 99);
	Player:AddData("isAlive", 1);

	scene:AddObject(Player);

	--Add City
	math.randomseed(os.time());
	resources:LoadShader("holo","resources/shaders/Default.vert", "resources/shaders/hologram/Holo.frag", "");
	hologram = resources:CreateGameObject("zdancer", "dancer","holo");
	hologram.position = vec3:new(1140,50,640);
	hologram.scale = vec3:new(1,1,1):multiply(100);
	scene:AddObject(hologram);
	count = 0;
	rots = {0,90,180,270};

	for i = 0,22,1
	do
		for j = 0, 12, 1
		do
			if(math.random(1,10) < 3)
			then
				Building1 = resources:CreateGameObject("building"..count, "building"..math.random(1,6),"");
				Building1:SetPosition(vec3:new(700 + i * 40, 15, 350 + j * 50));
				Building1.scale = vec3:new(10,10,10);
				
				local scale = vec3:new(10,60,10)
				physics:AddRigidBody(Building1,2);
				physics:AddRigidBodyColliderBox(Building1,scale,vec3:new(0,0,0), mass,bounce,friction);

				m_rot = rots[math.random(1,4)];
				Building1:SetRotation(vec3:new(0,m_rot,0));
				scene:AddObject(Building1);
			end
	
			count = count + 1;
		end
	end

	
	--NPC spawning
	math.randomseed(os.time());
	local tSize = terrain:GetSize() * terrain.scaleX;

	attack_state  = ScriptableState:new(attack_enter, attack_update, attack_exit, attack_message);
	charge_state  = ScriptableState:new(charge_enter, charge_update, charge_exit, charge_message);
	dead_state    = ScriptableState:new(dead_enter, dead_update, dead_exit, dead_message);
	global_state  = ScriptableState:new(global_enter, global_update, global_exit, global_message);
	empty_state   = ScriptableState:new(empty_enter, empty_update, empty_exit, empty_message);
	idle_state   = ScriptableState:new(idle_enter, idle_update, idle_exit, idle_message);
	aimanager:AddState("attack_state",attack_state);
	aimanager:AddState("charge_state",charge_state);
	aimanager:AddState("dead_state",dead_state);
	aimanager:AddState("global_state",global_state);
	aimanager:AddState("empty_state",empty_state);
	aimanager:AddState("idle_state",idle_state);

	robotCount = 150;
	for i = 1,robotCount,1
	do
		--choose enemy type , very 10th is a Charger
		if(math.fmod(i,10) == 0)
		then
			--Charger Enemy
			Robot = resources:CreateNPCObject("robot"..i, "robot_shiny","");
			Robot:AddData("type", 1);
			Robot.scale = vec3:new(4,4,4);
		else
			--Minion Enemy
			Robot = resources:CreateNPCObject("robot"..i, "robot","");
			Robot:AddData("type", 2);
			Robot.scale = vec3:new(2,2,2);	
		end

		local xpos = math.random(-(tSize)/4,tSize/4);
		local zpos = math.random(-(tSize)/4,tSize/4);
		local ypos = terrain:GetHeight(xpos,zpos) + 5;
		

		physics:AddRigidBody(Robot,1);
		local scale = vec3:new(1,1,1)
		local offset = vec3:new(0,2,0);
		local mass = 100;
		local bounce = 0;
		local friction = 1;
		physics:AddRigidBodyColliderBox(Robot,scale,offset, mass,bounce,friction);
		Robot.rigidBody:SetCenterOfMass(vec3:new(0,2,0));
		Robot.rigidBody:SetDampeningAngle(0.5);
		Robot.rigidBody:SetDampeningLinear(10);

		Robot:SetPosition(vec3:new(xpos,ypos,zpos));

		
		
		Robot.stateMachine:ChangeGlobalState(idle_state);

		scene:AddObject(Robot);
	end

	--terrain obstacle elements
	for i = 1,35,1
	do
		local xpos = 0;
		local zpos = 0;
		local ypos = -900;
		
		--ensure position is not in city and above water line
		while((ypos < -40) or (((xpos > 670) and (xpos < 1610)) and ((zpos > 310) and (zpos < 975))))
		do
			xpos = math.random(1,tSize) - tSize/2;
			zpos = math.random(1,tSize) - tSize/2;
			ypos = terrain:GetHeight(xpos,zpos) - 1;
		end
		tree = resources:CreateNPCObject("tree"..i, "Palm","");

		physics:AddRigidBody(tree,2);
		local scale = vec3:new(1,6,1)
		local offset = vec3:new(0,5,0);
		local mass = 100;
		local bounce = 0;
		local friction = 1;
		physics:AddRigidBodyColliderBox(tree,scale,offset, mass,bounce,friction);

		tree:SetPosition(vec3:new(xpos,ypos,zpos));
		tree.scale = vec3:new(1,1,1);

		scene:AddObject(tree);
	end
end
