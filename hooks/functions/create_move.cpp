#include "../hooks.h"

bool __stdcall hooks::create_move(float input_sample_time, CUserCmd* cmd) {
	if (!cmd || !cmd->commandnumber) 
		return _create_move(input_sample_time, cmd);

	g::cmd = cmd;

	return false;
}