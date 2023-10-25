dofile("resources/scripts/keybinds.lua")

--main init function, called once before update
function init()

	--set inputs
	input:SetMouseLock(false);
	input:BindKey("Fire",KEY_SPACE);

	scene:GetCamera().position = vec3:new(0,0,-20);

	material = resources:CreateMaterial("default","","");
	print("init lua");
end

--main update function, called every frame
function update(deltaTime)
	
	keyInput(deltaTime)

end

numThings = 0;
spread = 20;
upforce = 70;
function keyInput(dt)
		
	if(input:GetKeyState("Fire"))
	then
		--print("Fire!");
		--GameObject& ResourceManager::CreateGameObject(std::string objectName, std::string modelName, std::string shaderName)
		nName = ("object" .. numThings);
		ball = resources:CreateGameObject(nName, "Box", "default",material);
		ball.physicsBody = physics:CreatePhysicsBody();
		ball.physicsBody:SetMass(10);

		ball.physicsBody:SetVelocity(math.random(-spread,spread),math.random(upforce - 10,upforce),math.random(-spread,spread));
		scene:AddObject(ball);
		numThings = numThings + 1;
		--print(nName);
	end
	
end
