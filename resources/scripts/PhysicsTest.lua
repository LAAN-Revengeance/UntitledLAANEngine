dofile("resources/scripts/keybinds.lua")

--main init function, called once before update
function init()

	--set inputs
	input:SetMouseLock(false);
	input:BindKey("Fire",KEY_SPACE);

	scene:GetCamera().position = vec3:new(0,0,-5);
	scene:GetCamera():LookAt(vec3:new(0,0,0));

	Target = resources:GetGameObject("Target");
	print("init lua");
end

--main update function, called every frame
function update(deltaTime)
	
	keyInput(deltaTime)

end


function keyInput(dt)
		
	if(input:GetKeyState("Fire"))
	then
		print("Fire!");
	end
	
end
