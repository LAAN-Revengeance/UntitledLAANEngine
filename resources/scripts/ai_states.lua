
----------------------------------------------------------
				--EMPTY STATE FUNCTIONS--
----------------------------------------------------------

function empty_enter(ent, dt)
end

function empty_update(ent, dt)
end

function empty_exit(ent, dt)
end

function empty_message(ent, msg)
end
----------------------------------------------------------


----------------------------------------------------------
				--PLAYER STATE FUNCTIONS--
----------------------------------------------------------

function player_enter(ent, dt)
	
end

function player_update(ent, dt)
		
end

function player_exit(ent, dt)
	
end

function player_message(ent, msg)

end
----------------------------------------------------------


----------------------------------------------------------
			-- ENEMY ATTACK STATE FUNCTIONS--
----------------------------------------------------------

attackDelay = 0.5;
function attack_enter(ent, dt)

	ent:GetDrawItem():Animate("attack");
	ent.rigidBody:SetLinearVelocity(0,0,0);
	ent.rigidBody:SetAngularVelocity(0,0,0);
	ent.rigidBody:ModType(1);
	--Sound:playSound("grunt",camera.position);

	if(not ent:HasData("lastAttack"))
	then
		ent:AddData("lastAttack",0);
	end

end

function attack_update(ent, dt)
	
	local playerDist = Length(Player.position - ent.position);
	local newLAttack = ent:GetData("lastAttack") + dt;
	ent:AddData("lastAttack",newLAttack);

	if(newLAttack > attackDelay and playerDist < 10)
	then
		Sound:playSound("hitcar",camera.position);
		ent:AddData("lastAttack",0);
		Player:AddData("health", Player:GetData("health") - 1);
	end
		
	ent:SetPosition(vec3:new(ent.position.x,terrain:GetHeight(ent.position.x,ent.position.z) - 1,ent.position.z));
	ent:LookAt(Player.position);

end

function attack_exit(ent, dt)

	ent.rigidBody:ModType(1);

end

function attack_message(ent, msg)
	
end
----------------------------------------------------------


----------------------------------------------------------
				--CHARGE ATTACK STATE FUNCTIONS--
----------------------------------------------------------

function charge_enter(ent, dt)
	ent:GetDrawItem():Animate("run");
	ent:GetDrawItem():SetAnimationSpeed(100);
	ent.rigidBody:ModType(1);

	if(ent:GetData("isCharging") == 0)
	then
		ent:AddData("playerHit",0);	

		local targetVel = Player.rigidBody:GetLinearVelocity();

		targetPos = Player.position + targetVel:multiply(2)
	

		local toTarget = NormalizeVector(targetPos - ent.position);
		
		local target = targetPos + toTarget:multiply(math.random(50,70));
		
		ent:LookAt(target);

		ent:AddData("cX", target.x);
		ent:AddData("cZ", target.z);
		ent:AddData("cY", terrain:GetHeight(target.x,target.z) - 1);

		ent:AddData("isCharging", 1);
	end

end

function charge_update(ent, dt)
	local cX = ent:GetData("cX");
	local cY = ent:GetData("cY");
	local cZ = ent:GetData("cZ");

	local target = vec3:new(cX,cY,cZ);
	local dist = Length(target - ent.position);
	local distPlayer = Length(Player.position - ent.position);

	ent:MoveTo3D(target,50,2)
	ent:SetPosition(vec3:new(ent.position.x,terrain:GetHeight(ent.position.x,ent.position.z) - 1,ent.position.z));

	if(distPlayer < 3 and ent:GetData("playerHit") == 0)
	then
		Sound:playSound("hitcar",camera.position);
		ent:AddData("playerHit",1);
		Player:AddData("health", Player:GetData("health") - 10);
	end

	if(dist < 5)
	then
		ent.stateMachine:ChangeGlobalState(global_state);
		ent:AddData("isCharging", 0);
	end
end

function charge_exit(ent, dt)
end

function charge_message(ent, msg)
end
----------------------------------------------------------


----------------------------------------------------------
			--GLOBAL ENEMY STATE FUNCTIONS--
----------------------------------------------------------
melee_atkrange = 6;
charge_atkRange = 35;
hitRange = 5;
hitVelocity = 20;

function global_enter(ent, dt)
	ent:SetPosition(vec3:new(ent.position.x,terrain:GetHeight(ent.position.x,ent.position.z) - 1,ent.position.z));
	ent.rigidBody:SetLinearVelocity(0,0,0);
	ent.rigidBody:SetAngularVelocity(0,0,0);
	ent.rigidBody:ModType(3);
end

function global_update(ent, dt)
	local playerDist = Length(Player.position - ent.position);
	local playerVel = Length(Player.rigidBody:GetLinearVelocity());
	local type = ent:GetData("type");

	--choose behaviour bases on player distance and speed
	if(playerVel > hitVelocity and playerDist < hitRange * 5 and type == 2)
	then
		--ent.stateMachine:ChangeState(state_flee);
	else
		if(playerDist < melee_atkrange and type == 2)
		then
			ent.stateMachine:ChangeState(attack_state);
		elseif(playerDist < charge_atkRange and type == 1)
		then
			ent.stateMachine:ChangeState(charge_state);
			ent.stateMachine:ChangeGlobalState(empty_state);
		else
			ent.stateMachine:ChangeState(state_chase);
		end
	end

	--check if hit by player
	if(playerDist < hitRange and playerVel > hitVelocity)
	then
		ent.stateMachine:ChangeState(dead_state);
		ent.stateMachine:ChangeGlobalState(empty_state);
		Player:AddData("score", Player:GetData("score") + 1);
	end

end

function global_exit(ent, dt)
	
end

function global_message(ent, msg)
	
	--restart message
	if(msg.msgType == 0)
	then
		restartNPC(ent);
	end

end
----------------------------------------------------------


----------------------------------------------------------
				--DEAD ENEMY STATE FUNCTIONS--
----------------------------------------------------------
respawnTimeMin = 5;
respawnTimeMax = 10;
respawnRadius = 100;
function dead_enter(ent, dt)
	
	ent:StopMoving();

	Sound:playSound("explode",camera.position);
	ent:GetDrawItem():Animate("fall");

	local respawnTime = math.random(respawnTimeMin, respawnTimeMax) + math.random() 
	ent:AddData("timeToRespawn",respawnTime);

	--modify physics to prevent collisions with other objects
	ent.rigidBody:SetDampeningLinear(0.5);
	ent.rigidBody:ModType(1);

	--Fly away from player in random position
	local force = Player.rigidBody:GetLinearVelocity():multiply(math.random(1,3) + math.random());
	force.y = force.y + 20;
	ent.rigidBody:SetLinearVelocity(force.x,force.y,force.z);
	ent.rigidBody:SetAngularVelocity(math.random(-10,10),math.random(-10,10),math.random(-10,10));

end

function dead_update(ent, dt)

	--simulate gravity for kinematic body
	gravity = vec3:new(0, -9.81, 0)
	gravityForce = gravity:multiply(0.0167);
	newVelocity = ent.rigidBody:GetLinearVelocity() + gravityForce;
	ent.rigidBody:SetLinearVelocity(newVelocity.x,newVelocity.y,newVelocity.z);

	local timeToRespawn = ent:GetData("timeToRespawn");
	ent:AddData("timeToRespawn",ent:GetData("timeToRespawn") - dt);

	if(ent:GetData("timeToRespawn") < 0)
	then
		ent.stateMachine:ChangeGlobalState(global_state);
	end

end

function dead_exit(ent, dt)

	--spawn in random position respawnRadius meters away from playera
	math.randomseed(os.time() * ent:GetID());
	local theta = math.rad(math.random(0,360));
	local xPos = Player.position.x + (respawnRadius * math.cos(theta));
	local zPos = Player.position.z + (respawnRadius * math.sin(theta));
	ent:SetPosition(vec3:new(xPos,0,zPos));

	--return physics properties to normal
	ent.rigidBody:SetDampeningLinear(10);
	ent.rigidBody:ModType(1);
	ent:GetDrawItem():Animate("idle");
end

function dead_message(ent, msg)
	
end
----------------------------------------------------------


----------------------------------------------------------
				--IDLE STATE FUNCTIONS--
----------------------------------------------------------

function idle_enter(ent, dt)
	ent.rigidBody:SetDampeningLinear(10);
	ent.rigidBody:ModType(1);
	ent:GetDrawItem():Animate("idle");
end

function idle_update(ent, dt)
	local playerDist = Length(Player.position - ent.position);
	local playerVel = Length(Player.rigidBody:GetLinearVelocity());

	--check if hit by player, send message to all npcs to start attacking
	if(playerDist < hitRange and playerVel > hitVelocity)
	then
		ent.stateMachine:ChangeState(dead_state);
		ent.stateMachine:ChangeGlobalState(empty_state);
		Player:AddData("score", Player:GetData("score") + 1);
		Sound:playSound("alarm",camera.position);

		--send message to reset npcs
		local delay = 0;
		local sender = -1;
		local receiver = -1;
		local type = 1;
		aimanager:SendMessage(delay, sender, receiver, type);

	end

	ent:SetPosition(vec3:new(ent.position.x,terrain:GetHeight(ent.position.x,ent.position.z) - 1,ent.position.z));
end

function idle_exit(ent, dt)
	
end

function idle_message(ent, msg)

	--restart message
	if(msg.msgType == 1)
	then
		ent.stateMachine:ChangeGlobalState(global_state);
	end
end
----------------------------------------------------------


function restartNPC(ent)

	math.randomseed(os.time());
	local tSize = terrain:GetSize() * terrain.scaleX;
	local xpos = math.random(1,tSize) - tSize/2;
	local zpos = math.random(1,tSize) - tSize/2;
	local ypos = terrain:GetHeight(xpos,zpos) + 5;
	ent.rigidBody:SetPosition(vec3:new(xpos,ypos,zpos));
	ent.rigidBody:SetDampeningLinear(10);
	ent.rigidBody:ModType(1);
	ent.stateMachine:ChangeGlobalState(global_state);
end