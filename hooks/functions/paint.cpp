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
			render->draw_text(Vector2D(render->screen_size.x / 2, render->screen_size.y / 8), Color::Purple(), render->fonts.verdana, true, _("wok sdk :sunglasses:"));
			render->draw_text(Vector2D(render->screen_size.x / 2, render->screen_size.y / 8 + 12), Color::Purple(), render->fonts.verdana, true, _("-________________-"));
		}
		g_pSurface->FinishDrawing();
	}
}