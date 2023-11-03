
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

end

function global_update(ent, dt)

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
		ent:GetEmotion("Anger").emotionStrength = 0.0;
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
	if(ent:GetEmotion("Anger").emotionStrength > 0.5)
	then
		ent.stateMachine:ChangeState(aimanager:GetState("run_state"))
	end
end

function idle_exit(ent, dt)
	print("idle exit")
end

function idle_message(ent, msg)
end
----------------------------------------------------------


function expose_states()

	wander_state  = ScriptableState:new(wander_enter, wander_update, wander_exit, wander_message);
	run_state  = ScriptableState:new(run_enter, run_update, run_exit, run_message);
	idle_state  = ScriptableState:new(idle_enter, idle_update, idle_exit, idle_message);

	aimanager:AddState("wander_state",wander_state);
	aimanager:AddState("run_state",run_state);
	aimanager:AddState("idle_state",idle_state);

	print("states exposed success");
end