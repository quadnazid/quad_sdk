#pragma once
#include <windows.h>
#include <cstdint>
#include <intrin.h>
#include <exception>
#include <memory>
#include <string>
#include <sstream>
#include <map>
namespace calling {
	template<typename Fn>
	Fn unknowncall(void* Base, std::int32_t Index)
	{
		std::uintptr_t** uVTablePtr = static_cast<std::uintptr_t**>(Base);
		std::uintptr_t* uVTableFnBase = *uVTablePtr;
		std::uintptr_t uAddress = uVTableFnBase[Index];
		return reinterpret_cast<Fn>(uAddress);
	}
}
class i_surface {
public:
	void set_drawing_color(int r, int g, int b, int a = 255) {
		using original_fn = void(__thiscall*)(i_surface*, int, int, int, int);
		return (*(original_fn**)this)[15](this, r, g, b, a);
	}

	void play_sound(const char* sound_path) {
		using original_fn = void(__thiscall*)(i_surface*, const char*);
		return (*(original_fn**)this)[82](this, sound_path);
	}

	void set_text_color(int r, int g, int b, int a = 255) {
		using original_fn = void(__thiscall*)(i_surface*, int, int, int, int);
		return (*(original_fn**)this)[25](this, r, g, b, a);
	}

	void fillRectangle(int x1, int y1, int x2, int y2)
	{
		using original_fn = void(__thiscall*)(i_surface*, int, int, int, int);
		return (*(original_fn**)this)[16](this, x1, y1, x2, y2);
	}

	void draw_filled_rectangle(int x, int y, int w, int h) {
		using original_fn = void(__thiscall*)(i_surface*, int, int, int, int);
		return (*(original_fn**)this)[16](this, x, y, x + w, y + h);
	}

	void set_texture(int id) {
		using original_fn = void(__thiscall*)(i_surface*, int);
		return (*(original_fn**)this)[38](this, id);
	}

	inline void draw_textured_rectangle(int x, int y, int w, int h) {
		typedef void(__thiscall* original_fn)(void*, int, int, int, int);
		return (*(original_fn**)this)[41](this, x, y, w, h);
	}

	void set_texture_rgba(int id, const unsigned char* rgba, int wide, int tall) {
		using original_fn = void(__thiscall*)(i_surface*, int, const unsigned char*, int, int, int, bool);
		return (*(original_fn**)this)[37](this, id, rgba, wide, tall, 0, false);
	}

	int create_new_texture_id(bool procedural = false) {
		using original_fn = int(__thiscall*)(i_surface*, bool);
		return (*(original_fn**)this)[43](this, procedural);
	}

	void draw_outlined_rect(int x, int y, int w, int h) {
		using original_fn = void(__thiscall*)(i_surface*, int, int, int, int);
		return (*(original_fn**)this)[18](this, x, y, x + w, y + h);
	}
	void draw_line(int x1, int y1, int x2, int y2) {
		using original_fn = void(__thiscall*)(i_surface*, int, int, int, int);
		return (*(original_fn**)this)[19](this, x1, y1, x2, y2);
	}

	void draw_text_font(unsigned long font) {
		using original_fn = void(__thiscall*)(i_surface*, unsigned long);
		return (*(original_fn**)this)[23](this, font);
	}

	void draw_text_pos(int x, int y) {
		using original_fn = void(__thiscall*)(i_surface*, int, int);
		return (*(original_fn**)this)[26](this, x, y);
	}

	void draw_render_text(const wchar_t* text, int textLen) {
		using original_fn = void(__thiscall*)(i_surface*, const wchar_t*, int, int);
		return (*(original_fn**)this)[28](this, text, textLen, 0);
	}

	unsigned long font_create() {
		using original_fn = unsigned int(__thiscall*)(i_surface*);
		return (*(original_fn**)this)[71](this);
	}

	void set_font_glyph(unsigned long font, const char* windowsFontName, int tall, int weight, int blur, int scanlines, int flags) {
		using original_fn = void(__thiscall*)(i_surface*, unsigned long, const char*, int, int, int, int, int, int, int);
		return (*(original_fn**)this)[72](this, font, windowsFontName, tall, weight, blur, scanlines, flags, 0, 0);
	}

	void get_text_size(unsigned long font, const wchar_t* text, int& wide, int& tall) {
		using original_fn = void(__thiscall*)(i_surface*, unsigned long, const wchar_t*, int&, int&);
		return (*(original_fn**)this)[79](this, font, text, wide, tall);
	}

	void unlock_cursor() {
		using original_fn = void(__thiscall*)(i_surface*);
		return (*(original_fn**)this)[66](this);
	}

	/*void set_drawing_color(int r, int g, int b, int a = 255) {
		calling::unknowncall<void(__thiscall*)(void*, int, int, int, int)>(this, 15)(this, r, g, b, a);
	}

	void play_sound(const char* sound_path) {
		calling::unknowncall<void(__thiscall*)(void*, const char*)>(this, 82)(this, sound_path);
	}

	void set_text_color(int r, int g, int b, int a = 255) {
		calling::unknowncall<void(__thiscall*)(void*, int, int, int, int)>(this, 25)(this, r, g, b, a);
	}

	void draw_filled_rectangle(int x, int y, int w, int h) {
		calling::unknowncall<void(__thiscall*)(void*, int, int, int, int)>(this, 16)(this, x, y, x + w, y + h);
	}

	void set_texture(int id) {
		calling::unknowncall<void(__thiscall*)(void*, int)>(this, 38)(this, id);
	}

	inline void draw_textured_rectangle(int x, int y, int w, int h) {
		calling::unknowncall<void(__thiscall*)(void*, int, int, int, int)>(this, 41)(this, x, y, w, h);
	}

	void set_texture_rgba(int id, const unsigned char* rgba, int wide, int tall) {
		calling::unknowncall<void(__thiscall*)(void*, int, const unsigned char*, int, int, int, bool)>(this, 37)(this, id, rgba, wide, tall, 0, false);
	}
	void fillRectangle(int x1, int y1, int x2, int y2)
	{
		using original_fn = void(__thiscall*)(i_surface*, int, int, int, int);
		return (*(original_fn**)this)[16](this, x1, y1, x2, y2);
	}
	void draw_outlined_rect(int x, int y, int w, int h) {
		calling::unknowncall<void(__thiscall*)(void*, int, int, int, int)>(this, 18)(this, x, y, x + w, y + h);
	}

	void draw_line(int x1, int y1, int x2, int y2) {
		calling::unknowncall<void(__thiscall*)(void*, int, int, int, int)>(this, 19)(this, x1, y1, x2, y2);
	}

	void draw_text_font(unsigned long font) {
		calling::unknowncall<void(__thiscall*)(void*, unsigned long)>(this, 23)(this, font);
	}

	void draw_text_pos(int x, int y) {
		calling::unknowncall<void(__thiscall*)(void*, int, int)>(this, 26)(this, x, y);
	}

	void draw_render_text(const wchar_t* text, int textLen) {
		calling::unknowncall<void(__thiscall*)(void*, const wchar_t*, int, int)>(this, 28)(this, text, textLen, 0);
	}

	unsigned long font_create() {
		using original_fn = unsigned int(__thiscall*)(i_surface*);
		return (*(original_fn**)this)[71](this);
	}

	void set_font_glyph(unsigned long font, const char* windowsFontName, int tall, int weight, int blur, int scanlines, int flags) {
		calling::unknowncall<void(__thiscall*)(void*, unsigned long, const char*, int, int,
			int, int, int, int, int)>(this, 72)(this, font, windowsFontName, tall, weight, blur, scanlines, flags, 0, 0);
	}

	void get_text_size(unsigned long font, const wchar_t* text, int& wide, int& tall) {
		calling::unknowncall<void(__thiscall*)(void*, unsigned long, const wchar_t*, int&, int&)>(this, 79)(this, font, text, wide, tall);
	}

	void unlock_cursor() {
		calling::unknowncall<void(__thiscall*)(void*)>(this, 66)(this);
	}

	void draw_text(int x, int y, std::string_view string, int r, int g, int b, int a)
	{
		unsigned long font = NULL;
		this->set_font_glyph(font = this->font_create(), "Tahoma", 12, 700, 0, 0, 0x080);

		const auto wchar = std::wstring(string.begin(), string.end());

		int w, h;
		this->get_text_size(font, wchar.data(), w, h);
		this->draw_text_font(font);
		this->draw_text_pos(x, y);
		this->set_text_color(r, g, b, a);
		this->draw_render_text(wchar.data(), wcslen(wchar.data()));
	}*/
};