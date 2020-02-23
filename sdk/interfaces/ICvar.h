#pragma once

class IAppSystem {
public:
	virtual void func0() = 0;
	virtual void func1() = 0;
	virtual void func2() = 0;
	virtual void func3() = 0;
	virtual void func4() = 0;
	virtual void func5() = 0;
	virtual void func6() = 0;
	virtual void func7() = 0;
	virtual void func8() = 0;
	virtual void func9() = 0;
};

struct CVarDLLIdentifier_t;

class ICVar : public IAppSystem {
public:
	virtual void			func10() = 0;
	virtual void			RegisterConCommand(ConVar *pCommandBase) = 0;
	virtual void			UnregisterConCommand(ConVar *pCommandBase) = 0;
	virtual void			func13() = 0;
	virtual ConVar*			FindVar(const char *var_name) = 0;

	template <typename... args>
	void ConsoleColorPrintf(const bool developer, const Color& color, const char* format, args... arg) {
		static const auto print_developer_fn = reinterpret_cast<void(__stdcall*)(const Color&, char const*)>(SIG("engine.dll", "55 8B EC 81 EC 10 08 00 00 80 3D"));
		if (developer) {
			char buf[100];
			snprintf(buf, sizeof(buf), format, arg...);

			print_developer_fn(color, buf);
		}
		typedef void(*fn)(void*, const Color&, const char*, ...);
		utils::get_vfunc<fn>(this, 25)(this, color, format, arg...);
	}
};
extern ICVar* g_pCVar;