
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
				--AI STATE WANDER FUNCTIONS--
----------------------------------------------------------

function wander_enter(ent, dt)
end

function wander_update(ent, dt)
		print("im wandering here, ayyyyyyyyy. fuggetaboutit")
end

function wander_exit(ent, dt)
	
end

function wander_message(ent, msg)
	print("ayyyyyyyy i reived a message! IM still wanderin here!")
end
----------------------------------------------------------

function expose_states()
	wander_state  = ScriptableState:new(wander_enter, wander_update, wander_exit, wander_message);
	aimanager:AddState("wander_state",wander_state);
	print("states exposed success");
end
	--attack_state  = ScriptableState:new(attack_enter, attack_update, attack_exit, attack_message);
	--charge_state  = ScriptableState:new(charge_enter, charge_update, charge_exit, charge_message);
	--dead_state    = ScriptableState:new(dead_enter, dead_update, dead_exit, dead_message);
	--global_state  = ScriptableState:new(global_enter, global_update, global_exit, global_message);
	--empty_state   = ScriptableState:new(empty_enter, empty_update, empty_exit, empty_message);
	--idle_state   = ScriptableState:new(idle_enter, idle_update, idle_exit, idle_message);
	--aimanager:AddState("attack_state",attack_state);
	--aimanager:AddState("charge_state",charge_state);
	--aimanager:AddState("dead_state",dead_state);
	--aimanager:AddState("global_state",global_state);
	--aimanager:AddState("empty_state",empty_state);
	--aimanager:AddState("idle_state",idle_state);