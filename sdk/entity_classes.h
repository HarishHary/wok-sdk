#pragma once

class IKContext {
public:
	void init(void* hdr, QAngle& angles, Vector& origin, float curtime, int framecount, int bone_mask) {
		static const auto init_fn = reinterpret_cast<void(__thiscall*)(IKContext*, void*, QAngle&, Vector&, float, int, int)>(SIG("client_panorama.dll", "55 8B EC 83 EC ? 8B 45 ? 56 57 8B F9 8D 8F ? ? ? ?"));
		return init_fn(this, hdr, angles, origin, curtime, framecount, bone_mask);
	}
	void update_targets(Vector* pos, Quaternion* q, matrix3x4_t* bone_array, uint8_t* computed) {
		static const auto update_targets_fn = reinterpret_cast<void(__thiscall*)(IKContext*, Vector*, void*, matrix3x4_t*, uint8_t*)>(SIG("client_panorama.dll", "55 8B EC 83 E4 ? 81 EC ? ? ? ? 33 D2"));
		return update_targets_fn(this, pos, q, bone_array, computed);
	}
	void solve_dependencies(Vector* pos, Quaternion* q, matrix3x4_t* bone_array, uint8_t* computed) {
		static const auto solve_dependencies_fn = reinterpret_cast<void(__thiscall*)(IKContext*, Vector*, void*, matrix3x4_t*, uint8_t*)>(SIG("client_panorama.dll", "55 8B EC 83 E4 ? 81 EC ? ? ? ? 8B 81 ? ? ? ? 56"));
		return solve_dependencies_fn(this, pos, q, bone_array, computed);
	}
	void clear_targets() {
		int v112 = 0;
		int v70 = *(int*)((uintptr_t)this + 4080);

		if (v70 > 0) {
			uintptr_t v60 = (uintptr_t)((uintptr_t)this + 208);
			do {
				*(int*)(v60) = -9999;
				v60 += 340;
				++v112;
			} while (v112 < v70);
		}
	}
};

class CBoneSetup {
public:
	void accumulate_pose(Vector* pos, Quaternion* q, int sequence, float cycle, float weight, float time, IKContext* ik) {
		static const auto accumulate_pose_fn = reinterpret_cast<void(__thiscall*)(CBoneSetup*, Vector*, Quaternion*, int, float, float, float, IKContext*)>(SIG("client_panorama.dll", "55 8B EC 83 E4 F0 B8 ? ? ? ? E8 ? ? ? ? A1"));
		return accumulate_pose_fn(this, pos, q, sequence, cycle, weight, time, ik);
	}
};

class C_CSPlayer;
class C_BaseCombatWeapon;

class C_BaseEntity : public IClientEntity {
public:
	VFUNC(get_pred_desc_map(), 17, datamap_t*(__thiscall *)(void*))

	NETVAR(get_rotation(), QAngle, "CBaseEntity->m_angRotation")
	NETVAR(get_team(), TeamId, "CBaseEntity->m_iTeamNum")
	NETVAR(get_origin(), Vector, "CBaseEntity->m_vecOrigin")
	NETVAR(get_owner_entity(), CHandle<C_CSPlayer>, "CBaseEntity->m_hOwnerEntity")

	NETVAR(get_simulation_time(), float, "CBaseEntity->m_flSimulationTime")
	NETVAR_OFFSET(get_old_simulation_time(), float, "CBaseEntity->m_flSimulationTime", 0x4)
	NETVAR_OFFSET(get_rgfl_coordinate_frame(), matrix3x4_t, "CBaseEntity->m_CollisionGroup", -0x30)

	VFUNC(get_abs_angles(), 11, QAngle&(__thiscall*)(void*))
	VFUNC(get_abs_origin(), 10, Vector&(__thiscall*)(void*))

	CUSTOM_VFUNC(set_abs_angles(QAngle angles), "client_panorama.dll", "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1", void(__thiscall*)(void*, const QAngle&), angles)
	CUSTOM_VFUNC(set_abs_origin(Vector origin), "client_panorama.dll", "55 8B EC 83 E4 F8 51 53 56 57 8B F1", void(__thiscall*)(void*, const Vector&), origin)

	OFFSET(get_renderable(), IClientRenderable*, 0x4)
	OFFSET(get_networkable(), IClientNetworkable*, 0x8)

	OFFSET(get_model_hdr(), studiohdr_t*, 0x294C)
	OFFSET(get_bone_cache(), matrix3x4_t*, 0x290C + 0x4)
	OFFSET(get_bone_cache_count(), int, 0x2918 + 0x4)
	OFFSET(get_occlusion_mask(), int, 0xA24)
	OFFSET(get_occlusion_framecount(), int, 0xA30)
	OFFSET(get_unknown_occlusion_flags(), int, 0xA2C)
	OFFSET(get_occlusion_flags(), int, 0xA28)
	OFFSET(get_bone_array_for_write(), matrix3x4_t*, 0x26A8)
	OFFSET(get_last_setup_bones_framecount(), int, 0xA64 + 0x4)
	OFFSET(get_predictable(), int, 0x2EA + 0x4)
	OFFSET(get_accumulated_bone_mask(), int, 0x269C + 0x4)
	OFFSET(get_prev_bone_mask(), int, 0x2698 + 0x4)
	OFFSET(get_readable_bones(), int, 0x26A8 + 0x4)
	OFFSET(get_writable_bones(), int, 0x26AC + 0x4)
	OFFSET(get_most_recent_model_bone_counter(), int, 0x268C + 0x4)
	OFFSET(get_last_setup_bones_time(), float, 0x2920 + 0x4)
	OFFSET(get_ik_context(), IKContext*, 0x266C + 0x4)

	OFFSET(get_take_damage(), int, 0x280)

	DATAMAP(get_effects(), int, "m_fEffects")
	DATAMAP(get_eflags(), int, "m_iEFlags")
	DATAMAP(get_abs_velocity(), Vector, "m_vecAbsVelocity")
	DATAMAP(get_abs_rotation(), QAngle, "m_angAbsRotation")

	bool is_breakable() {
		if (!this)
			return false;

		auto backup_take_damage = get_take_damage();
		static const auto is_breakable_fn = reinterpret_cast<bool(__thiscall*)(void*)>(SIG("client_panorama.dll", "55 8B EC 51 56 8B F1 85 F6 74 68"));

		auto class_id = GetClientClass()->m_ClassID;
		auto is_breakable_class = (class_id == CBaseDoor 
			|| class_id == CBreakableSurface || class_id == CFuncBrush
			|| class_id == CBaseEntity && GetCollideable()->GetSolid() == SOLID_BSP);

		is_breakable_class ? get_take_damage() = DAMAGE_YES : 0;
		auto result = is_breakable_fn(this);
		is_breakable_class ? get_take_damage() = backup_take_damage : 0;

		return result;
	}
};

class C_BaseAttributableItem : public C_BaseEntity {
public:
	NETVAR(get_item_definition_index(), short, "CBaseAttributableItem->m_iItemDefinitionIndex")
	NETVAR(get_account_id(), int, "CBaseAttributableItem->m_iAccountID")
	NETVAR(get_item_quality(), int, "CBaseAttributableItem->m_iEntityQuality")
	NETVAR(get_item_id_high(), int, "CBaseAttributableItem->m_iItemIDHigh")
	NETVAR(get_item_id_low(), int, "CBaseAttributableItem->m_iItemIDLow")
	NETVAR(get_fallback_paint_kit(), int, "CBaseAttributableItem->m_nFallbackPaintKit")
	NETVAR(get_fallback_seed(), int, "CBaseAttributableItem->m_nFallbackSeed")
	NETVAR(get_fallback_wear(), int, "CBaseAttributableItem->m_flFallbackWear")
	NETVAR(get_fallback_stat_trak(), int, "CBaseAttributableItem->m_nFallbackStatTrak")
};

class C_BaseCombatCharacter : public C_BaseEntity {
public:
	PNETVAR(get_weapons(), CHandle<C_BaseCombatWeapon>, "CBaseCombatCharacter->m_hMyWeapons")
	PNETVAR(get_wearables(), CHandle<C_BaseAttributableItem>, "CBaseCombatCharacter->m_hMyWearables")
	NETVAR(get_active_weapon_handle(), CHandle<C_BaseCombatWeapon>, "CBaseCombatCharacter->m_hActiveWeapon")

	C_BaseCombatWeapon* get_active_weapon() {
		auto handle = get_active_weapon_handle();
		if (!handle.IsValid())
			return nullptr;

		return reinterpret_cast<C_BaseCombatWeapon*>(g_pEntityList->GetClientEntityFromHandle(handle));
	}
};

class C_BasePlayer : public C_BaseCombatCharacter {
public: 
	DATAMAP(get_duck_amount(), float, "m_flDuckAmount")
	DATAMAP(get_ground_entity(), CHandle<C_BaseEntity>, "m_hGroundEntity")
	DATAMAP(get_move_type(), int, "m_MoveType")
	DATAMAP(get_next_attack(), float, "m_flNextAttack")
	
	OFFSET(get_spawn_time(), float, 0xA360)

	NETVAR(get_fall_velocity(), float, "CBasePlayer->m_flFallVelocity")
	NETVAR(get_observer_mode(), ObserverMode, "CBasePlayer->m_iObserverMode")
	NETVAR(get_observer_target(), CHandle<C_CSPlayer>, "CBasePlayer->m_hObserverTarget")
	NETVAR(get_flags(), int, "CBasePlayer->m_fFlags")
	NETVAR(get_velocity(), Vector, "CBasePlayer->m_vecVelocity[0]")
	NETVAR(get_vehicle(), CHandle<C_BaseEntity>, "CBasePlayer->m_hVehicle")
	NETVAR(get_water_level(), int, "CBasePlayer->m_nWaterLevel")
	NETVAR(get_tickbase(), int, "CBasePlayer->m_nTickBase")
	NETVAR(get_duck_speed(), float, "CBasePlayer->m_flDuckSpeed")
	NETVAR(get_view_offset(), Vector, "CBasePlayer->m_vecViewOffset[0]")
	NETVAR(get_health(), int, "CBasePlayer->m_iHealth")
	NETVAR(get_life_state(), LifeState, "CBasePlayer->m_lifeState")

	VFUNC(set_local_view_angles(QAngle& angle), 372, void(__thiscall*)(void*, QAngle&), angle)

	VFUNC(think(), 138, void(__thiscall*)(void*)) // 8B C1 8B 50 40 (client_panorama.dll)
	VFUNC(pre_think(), 317, void(__thiscall*)(void*)) // 55 8B EC 83 E4 F8 51 56 8B F1 8B 06 (client_panorama.dll)

	CUSTOM_VFUNC(using_standard_weapons_in_vehicle(), "client_panorama.dll", "56 57 8B F9 8B 97 ? ? ? ? 83 FA FF 74 41", bool(__thiscall*)(void*))
	CUSTOM_VFUNC(physics_run_think(int index), "client_panorama.dll", "55 8B EC 83 EC 10 53 56 57 8B F9 8B 87", bool(__thiscall*)(void*, int), index)
	CUSTOM_VFUNC(select_item(const char* string, int sub_type = 0), "client_panorama.dll", "55 8B EC 56 8B F1 ? ? ? 85 C9 74 71 8B 06", bool(__thiscall*)(void*, const char*, int), string, sub_type)

	bool is_alive() { return get_life_state() == LIFE_ALIVE; }
	
	Vector get_eye_position() {
		Vector out;
		utils::get_vfunc<void(__thiscall*)(void*, Vector&)>(this, 284)(this, out);
		out.z -= get_view_offset().z - floor(get_view_offset().z);
		return out;
	}
};

class C_BaseAnimating : public C_BasePlayer {
public:
	PPOFFSET(get_anim_layers(), anim_layers, 0x2990)
	NETVAR(get_pose_params(), pose_params, "CBaseAnimating->m_flPoseParameter")
	NETPROP(get_client_side_animation_prop(), "CBaseAnimating->m_bClientSideAnimation")
	NETVAR(get_client_side_animation(), bool, "CBaseAnimating->m_bClientSideAnimation")
	NETVAR(get_sequence(), int, "CBaseAnimating->m_nSequence")
	
	void set_anim_layers(anim_layers layers) { std::copy(std::begin(layers), std::end(layers), std::begin(get_anim_layers())); }

	void set_pose_params(pose_params params) { std::copy(std::begin(params), std::end(params), std::begin(get_pose_params())); }

	void set_pose_parameter(int param, float value) {
		static const auto studio_set_pose_parameter_fn = reinterpret_cast<void*>(SIG("client_panorama.dll", "55 8B EC 83 E4 F8 83 EC 08 F3 0F 11 54 24 ? 85 D2"));

		auto result = 0.0f;
		auto hdr = get_model_hdr();

		__asm {
			pushad
			movss xmm2, [value]
			lea eax, [result]
			push eax
			mov edx, param
			mov ecx, hdr
			call studio_set_pose_parameter_fn
			pop eax
			popad
		}

		get_pose_params()[param] = result;
	}

	int get_sequence_activity(int sequence) {
		auto model = GetModel();
		if (!model)
			return -1;

		auto hdr = g_pModelInfo->GetStudioModel(model);
		if (!hdr)
			return -1;

		static const auto get_sequence_activity_fn = reinterpret_cast<int(__fastcall*)(void*, studiohdr_t*, int)>(SIG("client_panorama.dll", "55 8B EC 53 8B 5D 08 56 8B F1 83"));

		return get_sequence_activity_fn(this, hdr, sequence);
	}

	Vector get_bone_position(int bone_id) {
		Vector position, rotation;
		static const auto get_bone_position_fn = reinterpret_cast<void(__thiscall*)(void*, int, Vector*, Vector*)>(SIG("client_panorama.dll", "55 8B EC 83 E4 F8 83 EC 30 8D"));
		get_bone_position_fn(this, bone_id, &position, &rotation);
		return position;
	}
};

class C_CSPlayer : public C_BaseAnimating {
public:
	NETVAR(get_velocity_modifer(), float, "CCSPlayer->m_flVelocityModifier")
	NETVAR(has_defuser(), bool, "CCSPlayer->m_bHasDefuser")
	NETVAR(has_helmet(), bool, "CCSPlayer->m_bHasHelmet")
	NETVAR(get_armor_value(), int, "CCSPlayer->m_ArmorValue")
	NETVAR(has_heavy_armor(), bool, "CCSPlayer->m_bHasHeavyArmor")
	NETVAR(get_eye_angles(), QAngle, "CCSPlayer->m_angEyeAngles")
	NETVAR(is_scoped(), bool, "CCSPlayer->m_bIsScoped")
	NETVAR(is_immune(), bool, "CCSPlayer->m_bGunGameImmunity")
	NETVAR(get_health_boost_time(), float, "CCSPlayer->m_flHealthShotBoostExpirationTime")
	NETVAR(get_lby(), float, "CCSPlayer->m_flLowerBodyYawTarget")
	NETVAR_OFFSET(get_flash_alpha(), float, "CCSPlayer->m_flFlashMaxAlpha", -0x8)

	OFFSET(get_animstate(), C_AnimState*, 0x3900)

	VFUNC(standard_blending_rules(studiohdr_t* hdr, Vector* vec, Quaternion* q, const float time, const int mask), 205, void(__thiscall*)(void*, studiohdr_t*, Vector*, Quaternion*, float, int), hdr, vec, q, time, mask)
	VFUNC(build_transformations(studiohdr_t* hdr, Vector* vec, Quaternion* q, matrix3x4_t& transform, const int mask, uint8_t* computed), 189, void(__thiscall*)(void*, studiohdr_t*, Vector*, Quaternion*, matrix3x4_t const&, int, uint8_t*), hdr, vec, q, transform, mask, computed)
	VFUNC(update_ik_locks(float curtime), 191, void(__thiscall*)(void*, float), curtime)
	VFUNC(calculate_ik_locks(float curtime), 192, void(__thiscall*)(void*, float), curtime)
	VFUNC(update_client_side_animation(), 223, void(__thiscall*)(void*))
};

class C_BaseCombatWeapon : public C_BaseAttributableItem {
public:
	VFUNC(get_inaccuracy(), 482, float(__thiscall*)(void*))
	VFUNC(get_spread(), 452, float(__thiscall*)(void*))
	VFUNC(update_accuracy(), 483, void(__thiscall*)(void*))

	NETVAR(get_pin_pulled(), bool, "CBaseCSGrenade->m_bPinPulled")
	NETVAR(get_throw_time(), float, "CBaseCSGrenade->m_fThrowTime")

	NETVAR(get_post_pone_fire_ready_time(), float, "CWeaponCSBase->m_flPostponeFireReadyTime")
	NETVAR(get_accuracy_penalty(), float, "CWeaponCSBase->m_fAccuracyPenalty")
	NETVAR(get_recoil_index(), float, "CWeaponCSBase->m_flRecoilIndex")
	NETVAR(get_last_shot_time(), float, "CWeaponCSBase->m_fLastShotTime")
	NETVAR(get_ammo(), int, "CBaseCombatWeapon->m_iClip1")
	NETVAR(get_zoom_level(), int, "CWeaponCSBaseGun->m_zoomLevel")
	NETVAR(get_next_primary_attack(), float, "CBaseCombatWeapon->m_flNextPrimaryAttack")
	NETVAR(get_next_secondary_attack(), float, "CBaseCombatWeapon->m_flNextSecondaryAttack")

	CCSWeaponData* get_cs_weapon_data() {
		if (!this)
			return nullptr;

		return g_pWeaponSystem->GetWeaponData(get_item_definition_index());
	}
	
	float get_standing_accuracy() {
		auto data = get_cs_weapon_data();
		if (!data)
			return 0.f;

		auto max_speed = get_zoom_level() ? data->flMaxSpeedAlt : data->flMaxSpeed;
		return max_speed / 3.f;
	}
};

class C_LocalPlayer {
	friend bool operator==(const C_LocalPlayer& lhs, void* rhs) { return *lhs.m_loc == rhs; }
public:
	C_LocalPlayer() : m_loc(nullptr) {}

	operator bool() const { return *m_loc != nullptr; }
	operator C_CSPlayer*() const { return *m_loc; }

	C_CSPlayer* operator->() const { return *m_loc; }
private:
	C_CSPlayer** m_loc;
};