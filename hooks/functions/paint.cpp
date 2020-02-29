#include "../hooks.h"

void __fastcall hooks::paint(uintptr_t ecx, uintptr_t edx, int mode) {
	_paint(ecx, mode);

	static int w, h;
	g_pEngine->GetScreenSize(w, h);
	if (render->screen_size != Vector2D(w, h)) {
		if (render->initialized) {
			render->destroy_fonts();
			render->init_fonts();
		}
		render->screen_size = Vector2D(w, h);
	}

	if (mode & PAINT_UIPANELS || mode & PAINT_INGAMEPANELS) {
		g_pSurface->StartDrawing(); {																			
			menu->on_paint();
		}
		g_pSurface->FinishDrawing();
	}
}