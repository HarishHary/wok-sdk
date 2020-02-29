#include "../hooks.h"

bool __stdcall hooks::create_move(float input_sample_time, CUserCmd* cmd) {
	if (!cmd || !cmd->commandnumber) 
		return _create_move(input_sample_time, cmd);

	g::cmd = cmd;

	movement->bhop();
	movement->autostrafe();

	engine_prediction->run(g::local, cmd); {

	}
	engine_prediction->end(g::local, cmd);

	return false;
}