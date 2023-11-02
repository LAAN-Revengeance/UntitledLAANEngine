
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
end

function wander_message(ent, msg)
	
end
----------------------------------------------------------



function expose_states()
	wander_state  = ScriptableState:new(wander_enter, wander_update, wander_exit, wander_message);
	aimanager:AddState("wander_state",wander_state);
	print("states exposed success");
end