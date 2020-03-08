#include "../../sdk/sdk.h"

void c_render::destroy_fonts() {
	fonts.verdana = 0;
	initialized = false;
}

void c_render::init_fonts() {
	auto create_font = [](HFont& to_create, const char* windows_font_name, int tall, int weight, int blur, int scanlines, int flags) -> void {
		to_create = g_pSurface->CreateFont();
		g_pFontManager->SetFontGlyphSet(to_create, windows_font_name, tall, weight, blur, scanlines, flags, 0, 0);
	};

	if (initialized)
		return;

	create_font(fonts.verdana, "Verdana", 12, FW_MEDIUM, 0, 0, FONTFLAG_OUTLINE);
	initialized = true;
}

bool c_render::world_to_screen(const Vector& origin, Vector2D& screen) {
	const auto screen_transform = [&origin, &screen]() -> bool {
		static uintptr_t view_matrix;
		if (!view_matrix) {
			view_matrix = reinterpret_cast<uintptr_t>(SIG("client_panorama.dll", "0F 10 05 ? ? ? ? 8D 85 ? ? ? ? B9"));
			view_matrix += 3;
			view_matrix = *reinterpret_cast<uintptr_t*>(view_matrix);
			view_matrix += 176;
		}

		const VMatrix& w2s_matrix = *reinterpret_cast<VMatrix*>(view_matrix);
		screen.x = w2s_matrix.m[0][0] * origin.x + w2s_matrix.m[0][1] * origin.y + w2s_matrix.m[0][2] * origin.z + w2s_matrix.m[0][3];
		screen.y = w2s_matrix.m[1][0] * origin.x + w2s_matrix.m[1][1] * origin.y + w2s_matrix.m[1][2] * origin.z + w2s_matrix.m[1][3];

		auto w = w2s_matrix.m[3][0] * origin.x + w2s_matrix.m[3][1] * origin.y + w2s_matrix.m[3][2] * origin.z + w2s_matrix.m[3][3];

		if (w < 0.001f) {
			screen.x *= 100000;
			screen.y *= 100000;
			return true;
		}

		auto inv_w = 1.f / w;
		screen.x *= inv_w;
		screen.y *= inv_w;

		return false;
	};

	if (!screen_transform()) {
		screen.x = (screen_size.x * 0.5f) + (screen.x * screen_size.x) * 0.5f;
		screen.y = (screen_size.y * 0.5f) - (screen.y * screen_size.y) * 0.5f;
		return true;
	}
	return false;
}

void c_render::draw_text(Vector2D pos, Color color, HFont font, bool centered, const char* text) {
	int x = pos.x, y = pos.y;
	if (centered) {
		Vector2D size;
		get_text_size(font, text, size);
		x -= size.x / 2;
	}
	g_pSurface->DrawColoredText(font, x, y, color.red, color.green, color.blue, color.alpha, text);
}

void c_render::draw_text(Vector2D pos, Color color, HFont font, bool centered, std::string_view text) {
	draw_text(pos, color, font, centered, text.data());
}

void c_render::draw_gradient_rect(Vector2D pos, Vector2D size, Color c, Color c_, bool horizontal) {
	g_pSurface->DrawSetColor(c);
	g_pSurface->DrawFilledRectFade(pos.x, pos.y, pos.x + size.x, pos.y + size.y, 255, 0, horizontal);
	g_pSurface->DrawSetColor(c_);
	g_pSurface->DrawFilledRectFade(pos.x, pos.y, pos.x + size.x, pos.y + size.y, 0, 255, horizontal);
}

void c_render::draw_circle(Vector2D pos, int radius, Color color) {
	g_pSurface->DrawSetColor(color);
	g_pSurface->DrawOutlinedCircle(pos.x, pos.y, radius, 2000);
}

void c_render::draw_textured_polygon(int n, Vertex_t* vertices, Color color) {
	static const auto texture_id = g_pSurface->CreateNewTextureID(true);
	static unsigned char buf[4] = { 255, 255, 255, 255 };
	g_pSurface->DrawSetColor(color);
	g_pSurface->DrawSetTexture(texture_id);
	g_pSurface->DrawTexturedPolygon(n, vertices);
}

void c_render::draw_rounded_filled_rect(Vector2D pos, Vector2D size, float radius, Color color) {
	Vector2D add = { 0, 0 };
	static Vertex_t vertices[24];
	for (int i = 0; i < 24; i++) {
		float angle = (static_cast<float>(i) / -24) * 6.28f - (6.28f / 16.f);

		vertices[i].mPosition.x = radius + pos.x + add.x + (radius * sin(angle));
		vertices[i].mPosition.y = size.y - radius + pos.y + add.y + (radius * cos(angle));

		if (i == 4) {
			add.y = -size.y + (radius * 2);
		}
		else if (i == 10) {
			add.x = size.x - (radius * 2);
		}
		else if (i == 16) {
			add.y = 0;
		}
		else if (i == 22) {
			add.x = 0;
		}
	}
	g_pSurface->DrawSetColor(color);
	g_pSurface->DrawTexturedPolygon(24, vertices);
}

void c_render::draw_filled_rect(Vector2D pos, Vector2D size, Color color) {
	g_pSurface->DrawSetColor(color);
	g_pSurface->DrawFilledRect(pos.x, pos.y, pos.x + size.x, pos.y + size.y);
}

void c_render::get_text_size(HFont font, std::string_view text, Vector2D& out) {
	int w, h;
	auto converted_text = std::wstring(text.begin(), text.end());
	g_pFontManager->GetTextSize(font, converted_text.c_str(), w, h);
	out = Vector2D(w, h);
}

void c_render::get_text_size(HFont font, const char* text, Vector2D& out) {
	get_text_size(font, std::string_view(text), out);
}

void c_render::draw_outlined_rect(Vector2D pos, Vector2D size, Color color) {
	g_pSurface->DrawSetColor(color);
	g_pSurface->DrawOutlinedRect(pos.x, pos.y, pos.x + size.x, pos.y + size.y);
}

void c_render::draw_line(Vector2D start, Vector2D end, Color color) {
	g_pSurface->DrawSetColor(color);
	g_pSurface->DrawLine(start.x, start.y, end.x, end.y);
}

void c_render::draw_filled_circle(Vector2D pos, int radius, float points, Color color) {
	static const auto texture_id = g_pSurface->CreateNewTextureID(true);
	g_pSurface->DrawSetTexture(texture_id);
	g_pSurface->DrawSetColor(color);

	std::vector<Vertex_t> vertices;
	float step = M_PI * 2.0f / points;

	for (float a = 0; a < (M_PI * 2.0f); a += step)
		vertices.push_back(Vertex_t(Vector2D(radius * cosf(a) + pos.x, radius * sinf(a) + pos.y)));

	g_pSurface->DrawTexturedPolygon(points, vertices.data());
}

RECT c_render::get_text_size_rect(HFont font, const char* text) {
	RECT out;
	Vector2D in;
	get_text_size(font, text, in);

	out.bottom = in.y;
	out.left = out.right = in.x;

	return out;
}

void c_render::draw_vertices(Vertex_t* vertices, int num, Color color) {
	static const auto texture_id = g_pSurface->CreateNewTextureID(true);
	static unsigned char color_buffer[4] = { 255, 255, 255, 255 };
	g_pSurface->DrawSetTextureRGBA(texture_id, color_buffer, 1, 1);
	g_pSurface->DrawSetColor(color);
	g_pSurface->DrawSetTexture(texture_id);
	g_pSurface->DrawTexturedPolygon(num, vertices);
}

Vector2D c_render::get_mouse_position() {
	POINT mouse_position;
	GetCursorPos(&mouse_position);
	ScreenToClient(FindWindowA(0, "Counter-Strike: Global Offensive"), &mouse_position);
	return Vector2D(mouse_position.x, mouse_position.y);
}

bool c_render::mouse_in_region(int x, int y, int x2, int y2) {
	if (get_mouse_position().x > x && get_mouse_position().y > y
		&& get_mouse_position().x < x2 + x && get_mouse_position().y < y2 + y)
		return true;

	return false;
}

void c_render::draw_box(Vector origin, Vector2D size, Color color) {
	float difw = size.x / 2.f;
	float difh = size.y / 2.f;
	Vector box_vectors[8] = {
		Vector(origin.x - difw, origin.y - difh, origin.z - difw),
		Vector(origin.x - difw, origin.y - difh, origin.z + difw),
		Vector(origin.x + difw, origin.y - difh, origin.z + difw),
		Vector(origin.x + difw, origin.y - difh, origin.z - difw),
		Vector(origin.x - difw, origin.y + difh, origin.z - difw),
		Vector(origin.x - difw, origin.y + difh, origin.z + difw),
		Vector(origin.x + difw, origin.y + difh, origin.z + difw),
		Vector(origin.x + difw, origin.y + difh, origin.z - difw),
	};

	static Vector2D vec0, vec1, vec2, vec3, vec4, vec5, vec6, vec7;
	if (world_to_screen(box_vectors[0], vec0) 
		&& world_to_screen(box_vectors[1], vec1) 
		&& world_to_screen(box_vectors[2], vec2) 
		&& world_to_screen(box_vectors[3], vec3) 
		&& world_to_screen(box_vectors[4], vec4) 
		&& world_to_screen(box_vectors[5], vec5)
		&& world_to_screen(box_vectors[6], vec6)
		&& world_to_screen(box_vectors[7], vec7)) {
		Vector2D lines[12][2];

		lines[0][0] = vec0;
		lines[0][1] = vec1;
		lines[1][0] = vec1;
		lines[1][1] = vec2;
		lines[2][0] = vec2;
		lines[2][1] = vec3;
		lines[3][0] = vec3;
		lines[3][1] = vec0;

		lines[4][0] = vec4;
		lines[4][1] = vec5;
		lines[5][0] = vec5;
		lines[5][1] = vec6;
		lines[6][0] = vec6;
		lines[6][1] = vec7;
		lines[7][0] = vec7;
		lines[7][1] = vec4;

		lines[8][0] = vec0;
		lines[8][1] = vec4;

		lines[9][0] = vec1;
		lines[9][1] = vec5;

		lines[10][0] = vec2;
		lines[10][1] = vec6;

		lines[11][0] = vec3;
		lines[11][1] = vec7;

		for (int i = 0; i < 12; i++)
			draw_line(Vector2D(lines[i][0].x, lines[i][0].y), Vector2D(lines[i][1].x, lines[i][1].y), color);
	}
};

void c_render::draw_filled_triangle(std::array<Vector2D, 3> points, Color color) {
	std::array<Vertex_t, 3> vertices{ Vertex_t(points.at(0)), Vertex_t(points.at(1)), Vertex_t(points.at(2)) };
	draw_textured_polygon(3, vertices.data(), color);
};
