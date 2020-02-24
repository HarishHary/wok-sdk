#pragma once

class IKContext {
public:
	void init(void* hdr, Vector& angles, Vector& origin, float curtime, int framecount, int bone_mask) {
		static const auto init = SIG("client_panorama.dll", "55 8B EC 83 EC ? 8B 45 ? 56 57 8B F9 8D 8F ? ? ? ?");
		typedef void(__thiscall* fn) (IKContext*, void*, Vector&, Vector&, float, int, int);
		return reinterpret_cast<fn>(init)(this, hdr, angles, origin, curtime, framecount, bone_mask);
	}
	void update_targets(Vector* pos, Quaternion* q, matrix3x4_t* bone_array, uint8_t* computed) {
		static const auto update_targets = SIG("client_panorama.dll", "55 8B EC 83 E4 ? 81 EC ? ? ? ? 33 D2");
		typedef void(__thiscall* fn) (IKContext*, Vector*, void*, matrix3x4_t*, uint8_t*);
		return reinterpret_cast<fn>(update_targets)(this, pos, q, bone_array, computed);
	}
	void solve_dependencies(Vector* pos, Quaternion* q, matrix3x4_t* bone_array, uint8_t* computed) {
		static const auto solve_dependencies = SIG("client_panorama.dll", "55 8B EC 83 E4 ? 81 EC ? ? ? ? 8B 81 ? ? ? ? 56");
		typedef void(__thiscall* fn) (IKContext*, Vector*, void*, matrix3x4_t*, uint8_t*);
		return reinterpret_cast<fn>(solve_dependencies)(this, pos, q, bone_array, computed);
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

class C_BaseEntity : public IClientEntity {
public:
	VFUNC(get_pred_desc_map(), 17, datamap_t*(__thiscall *)(void*));

	NETVAR(get_rotation(), Vector, _("DT_BaseEntity"), _("m_angRotation"));
	NETVAR(get_team(), int, _("DT_BaseEntity"), _("m_iTeamNum"));
	NETVAR(get_origin(), Vector, _("DT_BaseEntity"), _("m_vecOrigin"));
	NETVAR(get_owner_entity(), CBaseHandle, _("DT_BaseEntity"), _("m_hOwnerEntity"));

	NETVAR(get_simulation_time(), float, _("DT_BaseEntity"), _("m_flSimulationTime"));
	NETVAR_OFFSET(get_old_simulation_time(), float, 0x4, _("DT_BaseEntity"), _("m_flSimulationTime"));

	VFUNC(get_abs_angles(), 11, QAngle&(__thiscall*)(void*));

	CUSTOM_VFUNC(set_abs_angles(QAngle angles), "client_panorama.dll", "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1", void(__thiscall*)(void*, const QAngle&), angles);
	CUSTOM_VFUNC(set_abs_origin(Vector origin), "client_panorama.dll", "55 8B EC 83 E4 F8 51 53 56 57 8B F1", void(__thiscall*)(void*, const Vector&), origin);

	OFFSET(get_renderable(), IClientRenderable*, 0x4);
	OFFSET(get_networkable(), IClientNetworkable*, 0x8);

	OFFSET(get_model_hdr(), studiohdr_t*, 0x294C);
	OFFSET(get_bone_cache(), matrix3x4_t*, 0x290C + 0x4);
	OFFSET(get_bone_cache_count(), int, 0x2918 + 0x4);
	OFFSET(get_occlusion_mask(), int, 0xA24);
	OFFSET(get_occlusion_framecount(), int, 0xA30);
	OFFSET(get_unknown_occlusion_flags(), int, 0xA2C);
	OFFSET(get_occlusion_flags(), int, 0xA28);
	OFFSET(get_bone_array_for_write(), matrix3x4_t*, 0x26A8);
	OFFSET(get_last_setup_bones_framecount(), int, 0xA64 + 0x4);
	OFFSET(get_predictable(), int, 0x2EA + 0x4);
	OFFSET(get_accumulated_bone_mask(), int, 0x269C + 0x4);
	OFFSET(get_prev_bone_mask(), int, 0x2698 + 0x4);
	OFFSET(get_readable_bones(), int, 0x26A8 + 0x4);
	OFFSET(get_writable_bones(), int, 0x26AC + 0x4);
	OFFSET(get_most_recent_model_bone_counter(), int, 0x268C + 0x4);
	OFFSET(get_last_setup_bones_time(), float, 0x2920 + 0x4);
	OFFSET(get_ik_context(), IKContext*, 0x266C + 0x4);

	OFFSET(get_take_damage(), int, 0x280);

	DATAMAP(get_effects(), int, "m_fEffects");
	DATAMAP(get_eflags(), int, "m_iEFlags");
	DATAMAP(get_abs_velocity(), Vector, "m_vecAbsVelocity");
	DATAMAP(get_abs_rotation(), Vector, "m_angAbsRotation");
	DATAMAP(get_abs_origin(), Vector, "m_vecAbsOrigin");

	bool is_breakable() {
		if (!this)
			return false;

		auto backup_take_damage = get_take_damage();
		static const auto is_breakable_fn = reinterpret_cast<bool(__thiscall*)(C_BaseEntity*)>(SIG("client_panorama.dll", "55 8B EC 51 56 8B F1 85 F6 74 68"));

		bool is_breakable_class = (GetClientClass()->m_ClassID == CBaseDoor
			|| GetClientClass()->m_ClassID == CBreakableSurface
			|| GetClientClass()->m_ClassID == CFuncBrush
			|| (GetClientClass()->m_ClassID == CBaseEntity && GetCollideable()->GetSolid() == SOLID_BSP));

		is_breakable_class ? get_take_damage() = DAMAGE_YES : 0;
		auto result = is_breakable_fn(this);
		is_breakable_class ? get_take_damage() = backup_take_damage : 0;

		return result;
	}
};

class C_BaseCombatCharacter : public C_BaseEntity {
public:
	NETVAR(get_weapons(), CBaseHandle, _("DT_BaseCombatCharacter"), _("m_hMyWeapons"));
	NETVAR(get_wearables(), CBaseHandle, _("DT_BaseCombatCharacter"), _("m_hMyWearables"));
	NETVAR(get_active_weapon_handle(), CBaseHandle, _("DT_BaseCombatCharacter"), _("m_hActiveWeapon"));

	C_BaseCombatWeapon* get_weapon() { 
		if (!get_active_weapon_handle().IsValid()) 
			return nullptr; 

		return reinterpret_cast<C_BaseCombatWeapon*>(g_pEntityList->GetClientEntityFromHandle(get_active_weapon_handle()));
	}
};

class C_BaseAttributableItem : public C_BaseEntity {
public:
	NETVAR(get_item_definition_index(), short, _("DT_BaseAttributableItem"), _("m_AttributeManager"), _("m_Item"), _("m_iItemDefinitionIndex"));
	NETVAR(get_account_id(), int, _("DT_BaseAttributableItem"), _("m_AttributeManager"), _("m_Item"), _("m_iAccountID")); 
	NETVAR(get_item_quality(), int, _("DT_BaseAttributableItem"), _("m_AttributeManager"), _("m_Item"), _("m_iEntityQuality"));
	NETVAR(get_item_id_high(), int, _("DT_BaseAttributableItem"), _("m_AttributeManager"), _("m_Item"), _("m_iItemIDHigh"));
	NETVAR(get_item_id_low(), int, _("DT_BaseAttributableItem"), _("m_AttributeManager"), _("m_Item"), _("m_iItemIDLow"));
	NETVAR(get_fallback_paint_kit(), int, _("DT_BaseAttributableItem"), _("m_nFallbackPaintKit"));
	NETVAR(get_fallback_seed(), int, _("DT_BaseAttributableItem"), _("m_nFallbackSeed"));
	NETVAR(get_fallback_wear(), int, _("DT_BaseAttributableItem"), _("m_flFallbackWear"));
	NETVAR(get_fallback_stat_trak(), int, _("DT_BaseAttributableItem"), _("m_nFallbackStatTrak"));
};

class C_BasePlayer : public C_BaseCombatCharacter {
public: 
	DATAMAP(get_duck_amount(), float, "m_flDuckAmount");
	DATAMAP(get_ground_entity(), CBaseHandle, "m_hGroundEntity");
	DATAMAP(get_move_type(), int, _("m_MoveType"));
	DATAMAP(get_next_attack(), float, _("m_flNextAttack"));
	
	NETVAR(get_flags(), int, _("DT_BasePlayer"), _("m_fFlags"));
	NETVAR(get_vehicle(), CBaseHandle, _("DT_BasePlayer"), _("m_hVehicle"));
	NETVAR(get_water_level(), int, _("DT_BasePlayer"), _("m_nWaterLevel"));
	NETVAR(get_next_think_tick(), int, _("DT_BasePlayer"), _("localdata"), _("m_nNextThinkTick"));
	NETVAR(get_tickbase(), int, _("DT_BasePlayer"), _("localdata"), _("m_nTickBase"));
	NETVAR(get_duck_speed(), float, _("DT_BasePlayer"), _("m_flDuckSpeed"));
	NETVAR(get_view_offset(), Vector, _("DT_BasePlayer"), _("localdata"), _("m_vecViewOffset[0]"));
	NETVAR(get_health(), int, _("DT_BasePlayer"), _("m_iHealth"));
	NETVAR(get_life_state(), LifeState, _("DT_BasePlayer"), _("m_lifeState"));
	bool is_alive() { return get_health() && get_life_state() == LifeState::ALIVE; }

	VFUNC(set_local_view_angles(QAngle& angle), 372, void(__thiscall*)(void*, QAngle&), angle);

	CUSTOM_VFUNC(using_standard_weapons_in_vehicle(), "client_panorama.dll", "56 57 8B F9 8B 97 ? ? ? ? 83 FA FF 74 41", bool(__thiscall*)(void*));
	CUSTOM_VFUNC(physics_run_think(int index), "client_panorama.dll", "55 8B EC 83 EC 10 53 56 57 8B F9 8B 87", bool(__thiscall*)(void*, int), index);
	CUSTOM_VFUNC(select_item(const char* string, int sub_type = 0), "client_panorama.dll", "55 8B EC 56 8B F1 ? ? ? 85 C9 74 71 8B 06", bool(__thiscall*)(void*, const char*, int), string, sub_type);

	Vector get_eye_position() {
		Vector out;
		typedef void(__thiscall *fn)(void*, Vector&);
		utils::get_vfunc<fn>(this, 284)(this, out);
		out.z -= get_view_offset().z - floor(get_view_offset().z);
		return out;
	}
};

class C_BaseAnimating : public C_BasePlayer {
public:
	OFFSET(get_anim_layers(), anim_layers, 0x2990);
	NETVAR(get_pose_params(), pose_params, _("DT_BaseAnimating"), _("m_flPoseParameter"));
	NETPROP(get_client_side_animation_prop(), _("DT_BaseAnimating"), _("m_bClientSideAnimation"));
	NETVAR(get_client_side_animation(), bool, _("DT_BaseAnimating"), _("m_bClientSideAnimation"));

	void set_anim_layers(anim_layers& layers) {
		auto this_layers = get_anim_layers();
		std::copy(std::begin(this_layers), std::end(this_layers), std::begin(layers));
	}

	void set_pose_params(pose_params& params) {
		auto this_params = get_pose_params();
		std::copy(std::begin(this_params), std::end(this_params), std::begin(params));
	}

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
	NETVAR(has_defuser(), bool, _("DT_CSPlayer"), _("m_bHasDefuser"));
	NETVAR(has_helmet(), bool, _("DT_CSPlayer"), _("m_bHasHelmet"));
	NETVAR(get_armor_value(), int, _("DT_CSPlayer"), _("m_ArmorValue"));
	NETVAR(has_heavy_armor(), bool, _("DT_CSPlayer"), _("m_bHasHeavyArmor"));
	NETVAR(get_eye_angles(), QAngle, _("DT_CSPlayer"), _("m_angEyeAngles"));
	NETVAR(is_scoped(), bool, _("DT_CSPlayer"), _("m_bIsScoped"));
	NETVAR(is_immune(), bool, _("DT_CSPlayer"), _("m_bGunGameImmunity"));
	NETVAR(get_health_boost_time(), float, _("DT_CSPlayer"), _("m_flHealthShotBoostExpirationTime"));
	NETVAR(get_lby(), float, _("DT_CSPlayer"), _("m_flLowerBodyYawTarget"));
	OFFSET(get_animstate(), C_AnimState*, 0x3900);

	VFUNC(standard_blending_rules(studiohdr_t* hdr, Vector* vec, Quaternion* q, const float time, const int mask), 205, void(__thiscall*)(void*, studiohdr_t*, Vector*, Quaternion*, float, int), hdr, vec, q, time, mask);
	VFUNC(build_transformations(studiohdr_t* hdr, Vector* vec, Quaternion* q, matrix3x4_t& transform, const int mask, uint8_t* computed), 189, void(__thiscall*)(void*, studiohdr_t*, Vector*, Quaternion*, matrix3x4_t const&, int, uint8_t*), hdr, vec, q, transform, mask, computed);
	VFUNC(update_ik_locks(float curtime), 191, void(__thiscall*)(void*, float), curtime);
	VFUNC(calculate_ik_locks(float curtime), 192, void(__thiscall*)(void*, float), curtime);
	VFUNC(update_client_side_animation(), 223, void(__thiscall*)(void*));
};

class C_BaseCombatWeapon : public C_BaseAttributableItem, public C_BaseEntity {
public:
	VFUNC(get_cs_weapon_data(), 459, CCSWeaponData*(__thiscall*)(void*));
	VFUNC(get_inaccuracy(), 481, float(__thiscall*)(void*));
	VFUNC(get_spread(), 451, float(__thiscall*)(void*));
	VFUNC(update_accuracy(), 482, void(__thiscall*)(void*));

	NETVAR(get_pin_pulled(), bool, _("DT_BaseCSGrenade"), _("m_bPinPulled"));
	NETVAR(get_throw_time(), float, _("DT_BaseCSGrenade"), _("m_fThrowTime"));

	NETVAR(get_post_pone_fire_ready_time(), float, _("DT_WeaponCSBase"), _("m_flPostponeFireReadyTime"));
	NETVAR(get_accuracy_penalty(), float, _("DT_WeaponCSBase"), _("m_fAccuracyPenalty"));
	NETVAR(get_recoil_index(), float, _("DT_WeaponCSBase"), _("m_flRecoilIndex"));
	NETVAR(get_last_shot_time(), float, _("DT_WeaponCSBase"), _("m_fLastShotTime"));
	NETVAR(get_ammo(), int, _("DT_BaseCombatWeapon"), _("m_iClip1"));
	NETVAR(get_zoom_level(), int, _("DT_WeaponCSBaseGun"), _("m_zoomLevel"));
	NETVAR(get_next_primary_attack(), float, _("DT_BaseCombatWeapon"), _("LocalActiveWeaponData"), _("m_flNextPrimaryAttack"));
	NETVAR(get_next_secondary_attack(), float, _("DT_BaseCombatWeapon"), _("LocalActiveWeaponData"), _("m_flNextSecondaryAttack"));

	float get_standing_accuracy() {
		auto max_speed = (get_zoom_level() > 0) ? get_cs_weapon_data()->flMaxSpeedAlt : get_cs_weapon_data()->flMaxSpeed;
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
