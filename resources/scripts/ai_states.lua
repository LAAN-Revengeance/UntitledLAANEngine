
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
				--NPC STATE GLOBAL--
----------------------------------------------------------

function global_enter(ent, dt)
	ent:AddEmotion("Bored");
	ent:GetEmotion("Bored").emotionStrength = math.random();
end

function global_update(ent, dt)

	if(ent:GetEmotion("Anger").emotionStrength > 0.8)
	then
		ent.stateMachine:ChangeState(aimanager:GetState("attack_state"))

	elseif(ent:GetEmotion("Fear").emotionStrength > 0.8)
	then
		ent.stateMachine:ChangeState(aimanager:GetState("run_state"))

	elseif((ent:GetEmotion("Gratitude").emotionStrength > 0.8) or (ent:GetEmotion("Hope").emotionStrength > 0.8))
	then
		ent.stateMachine:ChangeState(aimanager:GetState("thank_state"))

	elseif(ent:GetEmotion("Bored").emotionStrength > 0.8)
	then
		ent.stateMachine:ChangeState(aimanager:GetState("wander_state"))
	else

	end
end

function global_exit(ent, dt)

end

function global_message(ent, msg)

end
----------------------------------------------------------


----------------------------------------------------------
				--AI STATE WANDER FUNCTIONS--
----------------------------------------------------------

function wander_enter(ent, dt)
end

function wander_update(ent, dt)

	if(not ent:GetIsMoving())
	then
		ent:GetAnimation():Animate("run")
		ent:MoveToPoint(ent:FindRandomNode());
	end

	if(ent:GetEmotion("Bored").emotionStrength  < 0.1)
	then
		ent.stateMachine:ChangeState(aimanager:GetState("idle_state"))
	end
		ent:GetEmotion("Bored").emotionStrength = 	ent:GetEmotion("Bored").emotionStrength - dt * 0.2;
end

function wander_exit(ent, dt)
	ent:CancelPath();
	ent:GetAnimation():Animate("idle")
end

function wander_message(ent, msg)
	
end
----------------------------------------------------------


----------------------------------------------------------
				--AI STATE RUN FUNCTIONS--
----------------------------------------------------------

function run_enter(ent, dt)
	ent:GetAnimation():Animate("run")
	ent:MoveToPoint(ent:FindFurthestNode());
	print("enter run")
end

function run_update(ent, dt)

	if(not ent:GetIsMoving())
	then
		ent.stateMachine:ChangeState(aimanager:GetState("idle_state"))
		print("stop running")
	end
end

function run_exit(ent, dt)
	ent:CancelPath();
	ent:GetAnimation():Animate("idle")
	print("exit run")
end

function run_message(ent, msg)
	
end
----------------------------------------------------------


----------------------------------------------------------
				--AI STATE IDLE FUNCTIONS--
----------------------------------------------------------

function idle_enter(ent, dt)
	ent:GetAnimation():Animate("idle");
	ent:CancelPath();
end

function idle_update(ent, dt)
	ent:GetEmotion("Bored").emotionStrength = ent:GetEmotion("Bored").emotionStrength + dt * 0.1
end

function idle_exit(ent, dt)
	print("idle exit")
end

function idle_message(ent, msg)
end
----------------------------------------------------------


----------------------------------------------------------
				--ATTACK STATE FUNCTIONS--
----------------------------------------------------------

function attack_enter(ent, dt)
	ent:GetAnimation():Animate("attack");
end

function attack_update(ent, dt)

	local target = ent:GetLastInterracted();

	if(not (target == nil))
	then	
		--look at target
		local lookPos = target.position - ent.position;
		local up = vec3:new(0,20,0);
		ent:LookAt(CrossVectors(up,lookPos));

		if(lookPos:length() < 10)
		then
		print("here");
			ent:LookAt(target)
			ent.affordances:GetAffordance("punch"):Activate(target);
			ent.stateMachine:ChangeState(aimanager:GetState("idle_state"))
			ent:LookAt(CrossVectors(up,lookPos));
		end
	end
end

function attack_exit(ent, dt)
	ent:GetEmotion("Anger").emotionStrength = 0.0;
end

function attack_message(ent, msg)
end
----------------------------------------------------------


----------------------------------------------------------
				--THANK STATE FUNCTIONS--
----------------------------------------------------------

function thank_enter(ent, dt)
	ent:GetAnimation():Animate("shoot");
end

function thank_update(ent, dt)
	ent:GetEmotion("Gratitude").emotionStrength = ent:GetEmotion("Gratitude").emotionStrength - dt * 0.2;
	--look at target
	local target = ent:GetLastInterracted();
	if(not(target == nil))
	then
		local lookPos = target.position - ent.position;
		local up = vec3:new(0,20,0);
		ent:LookAt(CrossVectors(up,lookPos));
	end
end

function thank_exit(ent, dt)
	ent:GetAnimation():Animate("idle");
end

function thank_message(ent, msg)
end
----------------------------------------------------------

function expose_states()

	global_state	= ScriptableState:new(global_enter, global_update,	global_exit,	global_message);
	wander_state	= ScriptableState:new(wander_enter, wander_update,	wander_exit,	wander_message);
	run_state		= ScriptableState:new(run_enter,	run_update,		run_exit,		run_message);
	idle_state		= ScriptableState:new(idle_enter,	idle_update,	idle_exit,		idle_message);
	attack_state	= ScriptableState:new(attack_enter, attack_update,	attack_exit,	attack_message);
	thank_state		= ScriptableState:new(thank_enter,	thank_update,	thank_exit,		thank_message);

	aimanager:AddState("global_state",global_state);
	aimanager:AddState("wander_state",wander_state);
	aimanager:AddState("run_state",run_state);
	aimanager:AddState("idle_state",idle_state);
	aimanager:AddState("attack_state",attack_state);
	aimanager:AddState("thank_state",thank_state);

	print("states exposed success");
end