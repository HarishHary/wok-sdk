#include "menu.h"

void c_menu::on_paint(){
	render->draw_text(Vector2D(render->screen_size.x / 2, render->screen_size.y / 8), Color::Purple(), render->fonts.verdana, true, _("wok sdk :sunglasses:"));
	render->draw_text(Vector2D(render->screen_size.x / 2, render->screen_size.y / 8 + 12), Color::Purple(), render->fonts.verdana, true, _("-________________-"));
}