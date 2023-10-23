dofile("resources/scripts/keybinds.lua")

--main init function, called once before update
function init()

	--set inputs
	input:SetMouseLock(false);
	input:BindKey("Fire",KEY_SPACE);
	scene:GetCamera().position = vec3:new(-1,0,-3);

	material = resources:CreateMaterial("Grass","","");
	
	print("init lua");
end

--main update function, called every frame
function update(deltaTime)
	keyInput(deltaTime)
end

numThings = 0;

isDown = false;
function keyInput(dt)
		
	if(input:GetKeyState("Fire"))
	then
		if(not isDown)
		then
			--GameObject& ResourceManager::CreateGameObject(std::string objectName, std::string modelName, std::string shaderName)
			nName = ("object" .. numThings);
			ball = resources:CreateGameObject(nName, "Sphere", "default",material);
		
			ball.scale = vec3:new(0.1,0.1,0.1);

			ball.physicsBody = physics:CreatePhysicsBody();
			
			physics:AddSphereCollider(ball.physicsBody,0.1);
			ball.physicsBody:SetMass(0.1);
			ball.physicsBody:SetGravity(false);

			ball.physicsBody:SetVelocity(ball.position + vec3:new(-0.5, 0, 0));

			scene:AddObject(ball);
			numThings = numThings + 1;
			--print(nName);
		end
		isDown = true;
	else
		isDown = false;
	end
	
end
