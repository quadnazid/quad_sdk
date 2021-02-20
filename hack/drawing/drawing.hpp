#pragma once
#include <string>
#include "singleton.hpp"
#include "../interfaces/interfaces.hpp"
#include "c_color.hpp"
enum font_flags {
	fontflag_none,
	fontflag_italic = 0x001,
	fontflag_underline = 0x002,
	fontflag_strikeout = 0x004,
	fontflag_symbol = 0x008,
	fontflag_antialias = 0x010,
	fontflag_gaussianblur = 0x020,
	fontflag_rotary = 0x040,
	fontflag_dropshadow = 0x080,
	fontflag_additive = 0x100,
	fontflag_outline = 0x200,
	fontflag_custom = 0x400,
	fontflag_bitmap = 0x800,
};

class c_drawing : public singleton<c_drawing> {
public: // fonts
	DWORD watermark_font;

public: // basic renderer
	void setup_fonts() {
		static bool _o = false;
		if (!_o) {
			watermark_font = interfaces::surface->font_create();
			interfaces::surface->set_font_glyph(watermark_font, "Tahoma", 12, 500, 0, 0, font_flags::fontflag_dropshadow);;

			_o = true;
		}
	}
	void draw_line(int x1, int y1, int x2, int y2, color colour) {
		interfaces::surface->set_drawing_color(colour.r, colour.g, colour.b, colour.a);
		interfaces::surface->draw_line(x1, y1, x2, y2);
	}
	void draw_text(int x, int y, unsigned long font, const char* string, bool text_centered, color colour) {
		va_list va_alist;
		char buf[1024];
		va_start(va_alist, string);
		_vsnprintf(buf, sizeof(buf), string, va_alist);
		va_end(va_alist);
		wchar_t wbuf[1024];
		MultiByteToWideChar(CP_UTF8, 0, buf, 256, wbuf, 256);

		int width, height;
		interfaces::surface->get_text_size(font, wbuf, width, height);

		interfaces::surface->set_text_color(colour.r, colour.g, colour.b, colour.a);
		interfaces::surface->draw_text_font(font);
		if (text_centered)
			interfaces::surface->draw_text_pos(x - (width / 2), y);
		else
			interfaces::surface->draw_text_pos(x, y);
		interfaces::surface->draw_render_text(wbuf, wcslen(wbuf));
	}
	void draw_string(int x, int y, unsigned long font, bool text_centered, color colour, std::string string, ...) {
		std::wstring text = std::wstring(string.begin(), string.end());
		const wchar_t* converted_text = text.c_str();

		int width, height;
		interfaces::surface->get_text_size(font, converted_text, width, height);

		interfaces::surface->set_text_color(colour.r, colour.g, colour.b, colour.a);
		interfaces::surface->draw_text_font(font);
		if (text_centered)
			interfaces::surface->draw_text_pos(x - (width / 2), y);
		else
			interfaces::surface->draw_text_pos(x, y);
		interfaces::surface->draw_render_text(converted_text, wcslen(converted_text));
	}
	void draw_text(int x, int y, unsigned long font, std::string string, bool text_centered, color colour) {
		std::wstring text = std::wstring(string.begin(), string.end());
		const wchar_t* converted_text = text.c_str();

		int width, height;
		interfaces::surface->get_text_size(font, converted_text, width, height);

		interfaces::surface->set_text_color(colour.r, colour.g, colour.b, colour.a);
		interfaces::surface->draw_text_font(font);
		if (text_centered)
			interfaces::surface->draw_text_pos(x - (width / 2), y);
		else
			interfaces::surface->draw_text_pos(x, y);
		interfaces::surface->draw_render_text(converted_text, wcslen(converted_text));
	}
	void draw_weapon(int x, int y, unsigned long font, const std::string string, bool text_centered, color colour) {
		std::wstring text = std::wstring(string.begin(), string.end());
		const wchar_t* converted_text = text.c_str();

		int width, height;
		interfaces::surface->get_text_size(font, converted_text, width, height);

		interfaces::surface->set_text_color(colour.r, colour.g, colour.b, colour.a);
		interfaces::surface->draw_text_font(font);
		if (text_centered)
			interfaces::surface->draw_text_pos(x - (width / 2), y);
		else
			interfaces::surface->draw_text_pos(x, y);
		interfaces::surface->draw_render_text(converted_text, wcslen(converted_text));
	}
	void draw_rect(int x, int y, int w, int h, color color) {
		interfaces::surface->set_drawing_color(color.r, color.g, color.b, color.a);
		interfaces::surface->draw_outlined_rect(x, y, w, h);
	}

	void draw_filled_rect(int x, int y, int w, int h, color colour) {
		interfaces::surface->set_drawing_color(colour.r, colour.g, colour.b, colour.a);
		interfaces::surface->draw_filled_rectangle(x, y, w, h);
	}

	void draw_outline(int x, int y, int w, int h, color colour) {
		interfaces::surface->set_drawing_color(colour.r, colour.g, colour.b, colour.a);
		interfaces::surface->draw_outlined_rect(x, y, w, h);
	}
	void draw_box(int x, int y, int w, int h, int lw, int r, int g, int b, int a)
	{
		interfaces::surface->set_drawing_color(r, g, b, a);

		interfaces::surface->draw_outlined_rect(x, y, w, lw);
		interfaces::surface->draw_outlined_rect(x + w - lw, y + lw, lw, h - lw);
		interfaces::surface->draw_outlined_rect(x, y + lw, lw, h - lw);
		interfaces::surface->draw_outlined_rect(x + lw, y + h - lw, w - lw * 2, lw);
	}
	void gradient_line(int x, int y, int w, int h, color c1, color c2)
	{

		c_drawing::get().draw_filled_rect(x, y, w, h, c1);
		BYTE first = c2.red;
		BYTE second = c2.green;
		BYTE third = c2.blue;
		for (int i = 0; i < w; i++)
		{
			float fi = i, fw = w;
			float a = fi / fw;
			DWORD ia = a * 255;
			c_drawing::get().draw_filled_rect(x + i, y, 1, h, color(first, second, third, ia));
		}

	}
	void get_text_size(unsigned long font, const char* string, int w, int h) {
		va_list va_alist;
		char buf[1024];
		va_start(va_alist, string);
		_vsnprintf(buf, sizeof(buf), string, va_alist);
		va_end(va_alist);
		wchar_t out[1024];
		MultiByteToWideChar(CP_UTF8, 0, buf, 256, out, 256);

		interfaces::surface->get_text_size(font, out, w, h);
	}
	void get_text_size(unsigned long font, std::string string, int w, int h) {
		std::wstring text = std::wstring(string.begin(), string.end());
		const wchar_t* out = text.c_str();

		interfaces::surface->get_text_size(font, out, w, h);
	}
	vec2_t get_screen_size(vec2_t area)
	{
		static int old_w, old_h;
		interfaces::engine->get_screen_size((int&)area.x, (int&)area.y);

		if ((int&)area.x != old_w || (int&)area.y != old_h)
		{
			old_w = (int&)area.x;
			old_h = (int&)area.y;
			return vec2_t(old_w, old_h);
		}
		return area;
	}
	float text_width(unsigned long font, const char* msg, ...) {
		va_list va_alist;
		char buffer[1024];
		va_start(va_alist, msg);
		_vsnprintf(buffer, sizeof(buffer), msg, va_alist);
		va_end(va_alist);
		wchar_t wbuf[1024];

		MultiByteToWideChar(CP_UTF8, 0, buffer, 256, wbuf, 256);

		int width, height;
		interfaces::surface->get_text_size(font, wbuf, width, height);

		return width;
	}
	void old_text(int x, int y, const wchar_t* _Input, int font, color color)
	{
		interfaces::surface->draw_text_font(font);
		interfaces::surface->set_text_color(color.r, color.g, color.b, color.a);
		interfaces::surface->draw_text_pos(x, y);
		interfaces::surface->draw_render_text(_Input, wcslen(_Input));
	}
	RECT viewport() {
		RECT viewport = { 0, 0, 0, 0 };

		int width, height;
		interfaces::engine->get_screen_size(width, height);
		viewport.right = width;
		viewport.bottom = height;

		return viewport;
	}
};