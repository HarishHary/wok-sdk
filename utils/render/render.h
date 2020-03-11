#pragma once

class c_render : public c_singleton<c_render> {
public:
	struct {
		HFont verdana;
	} fonts;

	c_render() { init_fonts(); }

	Vector2D screen_size;
	bool initialized = false;
	void destroy_fonts();
	void init_fonts();
	bool world_to_screen(const Vector& origin, Vector2D& screen);

	void draw_text(Vector2D pos, Color color, HFont font, bool centered, const char* text);
	void draw_text(Vector2D pos, Color color, HFont font, bool centered, std::string_view text);
	void draw_gradient_rect(Vector2D pos, Vector2D size, Color c, Color c_, bool horizontal);
	void draw_circle(Vector2D pos, int radius, Color color);
	void draw_textured_polygon(int n, Vertex_t* vertices, Color color);
	void draw_rounded_filled_rect(Vector2D pos, Vector2D size, float radius, Color color);
	void draw_filled_rect(Vector2D pos, Vector2D size, Color color);
	void get_text_size(HFont font, std::string_view text, Vector2D& out);
	void get_text_size(HFont font, const char* text, Vector2D& out);
	Vector2D get_text_size(HFont font, const char* text);
	void draw_outlined_rect(Vector2D pos, Vector2D size, Color color);
	void draw_line(Vector2D start, Vector2D end, Color color);
	void draw_filled_circle(Vector2D pos, int radius, float points, Color color);
	void draw_vertices(Vertex_t* vertices, int num, Color color);
	Vector2D get_mouse_position();
	bool mouse_in_region(int x, int y, int x2, int y2);
	void draw_box(Vector origin, Vector2D size, Color color);
	void draw_filled_triangle(std::array<Vector2D, 3>points, Color color);
};
#define render c_render::instance()