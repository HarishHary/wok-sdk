#include "../hooks.h"

bool __fastcall hooks::setup_bones(uintptr_t ecx, uintptr_t edx, matrix3x4_t* bone_to_world_out, int max_bones, int bone_mask, float current_time) {
	auto entity = reinterpret_cast<C_CSPlayer*>(ecx - 0x4);
	if (!entity || entity->GetClientClass()->m_ClassID != CCSPlayer)
		return _setup_bones(ecx, bone_to_world_out, max_bones, bone_mask, current_time);

	const auto ret = _setup_bones(ecx, bone_to_world_out, max_bones, bone_mask, current_time);

	return ret;
}