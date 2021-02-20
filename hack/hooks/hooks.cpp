#pragma once
#include <algorithm>
#include <cmath>
#include <sstream>
#include "../../includes.hpp"

uint8_t* present_address;
hooks::present_fn original_present;
uint8_t* reset_address;
hooks::reset_fn original_reset;
HWND hooks::window;
WNDPROC hooks::wndproc_original = NULL;

hooks::frame_stage_notify_fn original_frame_stage_notify;
hooks::override_view_fn original_override_view;
hooks::create_move_fn original_create_move_hook;
hooks::scene_end_fn original_scene_end;
hooks::do_post_screen_effects_fn original_do_post_screen_effects_hook;
hooks::paint_traverse_fn original_panel;
hooks::lock_cursor_fn original_lockcursor;
hooks::draw_model_execute_fn original_draw_model_execute;

void hooks::initialize() {

	//client
	const auto frame_stage_notify_hook = reinterpret_cast<void*>(get_virtual(interfaces::client, 37));

	//client mode
	const auto override_view_hook = reinterpret_cast<void*>(get_virtual(interfaces::clientmode, 18));
	const auto create_move_hook = reinterpret_cast<void*>(get_virtual(interfaces::clientmode, 24));
	const auto do_post_screen_effects_hook = reinterpret_cast<void*>(get_virtual(interfaces::clientmode, 44));
	const auto viewmodel_fov_hook = reinterpret_cast<void*>(get_virtual(interfaces::clientmode, 35));

	//panel
	const auto panel_hook = reinterpret_cast<void*>(get_virtual(interfaces::panel, 41));

	const auto render_view_hook = reinterpret_cast<void*>(get_virtual(interfaces::render_view, 9));
	const auto surface_hook = reinterpret_cast<void*>(get_virtual(interfaces::surface, 67));
	const auto model_render_hook = reinterpret_cast<void*>(get_virtual(interfaces::model_render, 21));

	const auto present_hook = reinterpret_cast<void*>(get_virtual(interfaces::directx, 17));
	const auto reset_hook = reinterpret_cast<void*>(get_virtual(interfaces::directx, 16));

	if (MH_Initialize() != MH_OK)
		throw std::runtime_error("failed to initialize MH_Initialize.");

	// client
	if (MH_CreateHook(frame_stage_notify_hook, &frame_stage_notify, reinterpret_cast<void**>(&original_frame_stage_notify)) != MH_OK)
		throw std::runtime_error("failed to initialize frame_stage_notify. (outdated index?)");


	//client mode
	if (MH_CreateHook(override_view_hook, &override_view, reinterpret_cast<void**>(&original_override_view)) != MH_OK)
		throw std::runtime_error("failed to initialize override_view_hook. (outdated index?)");

	if (MH_CreateHook(create_move_hook, &create_move, reinterpret_cast<void**>(&original_create_move_hook)) != MH_OK)
		throw std::runtime_error("failed to initialize create_move_hook. (outdated index?)");

	if (MH_CreateHook(do_post_screen_effects_hook, &do_post_screen_effects, reinterpret_cast<void**>(&original_do_post_screen_effects_hook)) != MH_OK)
		throw std::runtime_error("failed to initialize do_post_screen_effects_hook. (outdated index?)");

	//panel
	if (MH_CreateHook(panel_hook, &paint_traverse, reinterpret_cast<void**>(&original_panel)) != MH_OK)
		throw std::runtime_error("failed to initialize paint_traverse_hook. (outdated index?)");

	// scenes
	if (MH_CreateHook(render_view_hook, &scene_end, reinterpret_cast<void**>(&original_scene_end)) != MH_OK)
		throw std::runtime_error("failed to initialize scene_end_hook. (outdated index?)");

	if (MH_CreateHook(surface_hook, &lock_cursor, reinterpret_cast<void**>(&original_lockcursor)) != MH_OK)
		throw std::runtime_error("failed to initialize lock_cursor_hook. (outdated index?)");

	if (MH_CreateHook(model_render_hook, &draw_model_execute, reinterpret_cast<void**>(&original_draw_model_execute)) != MH_OK)
		throw std::runtime_error("failed to initialize draw_model_execute_hook. (outdated index?)");

	//directx
	if (MH_CreateHook(present_hook, &present, reinterpret_cast<void**>(&original_present)) != MH_OK)
		throw std::runtime_error("failed to initialize paint_traverse_hook. (outdated index?)");

	if (MH_CreateHook(reset_hook, &reset, reinterpret_cast<void**>(&original_reset)) != MH_OK)
		throw std::runtime_error("failed to initialize scene_end_hook. (outdated index?)");


	if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
		throw std::runtime_error("failed to enable hooks.");

	window = FindWindow("Valve001", NULL);

	wndproc_original = (WNDPROC)SetWindowLongPtrA(window, GWL_WNDPROC, (LONG)wndproc);

	interfaces::console->get_convar("crosshair")->set_value(1);
	interfaces::console->get_convar("viewmodel_fov")->callbacks.set_size(false);
	interfaces::console->get_convar("viewmodel_offset_x")->callbacks.set_size(false);
	interfaces::console->get_convar("viewmodel_offset_y")->callbacks.set_size(false);
	interfaces::console->get_convar("viewmodel_offset_z")->callbacks.set_size(false);

	c_drawing::get().setup_fonts();
}

void hooks::shutdown() {
	MH_Uninitialize();

	MH_DisableHook(MH_ALL_HOOKS);

	SetWindowLongPtrA(FindWindow("Valve001", NULL), GWL_WNDPROC, (LONG)wndproc_original);
}

float __stdcall hooks::viewmodel_fov() {
	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));

	if (local_player && local_player->is_alive()) {
		return 68.f;
	}
	else {
		return 68.f;
	}
}

int __stdcall hooks::do_post_screen_effects(int value) {
	return original_do_post_screen_effects_hook(interfaces::clientmode, value);
}

bool __stdcall hooks::create_move(float frame_time, c_usercmd* user_cmd) {
	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	original_create_move_hook(interfaces::clientmode, frame_time, user_cmd);

	if (!user_cmd || !user_cmd->command_number)
		return original_create_move_hook;

	if (!interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()))
		return original_create_move_hook;

	if (interfaces::engine->is_connected() && interfaces::engine->is_in_game()) {
		

		// clamping movement
		auto forward = user_cmd->forwardmove;
		auto right = user_cmd->sidemove;
		auto up = user_cmd->upmove;

		if (forward > 450)
			forward = 450;

		if (right > 450)
			right = 450;

		if (up > 450)
			up = 450;

		if (forward < -450)
			forward = -450;

		if (right < -450)
			right = -450;

		if (up < -450)
			up = -450;

		// clamping angles
		utilities::clamp_angles(user_cmd->viewangles);
	}

	return false;
}

void __fastcall hooks::override_view(void* _this, void* _edx, c_viewsetup* setup) {
	original_override_view(interfaces::clientmode, _this, setup);
}

void __stdcall hooks::draw_model_execute(IMatRenderContext* ctx, const draw_model_state_t& state, const model_render_info_t& info, matrix_t* bone_to_world) {
	original_draw_model_execute(interfaces::model_render, ctx, state, info, bone_to_world);
}

void __stdcall hooks::frame_stage_notify(int frame_stage) {
	if (frame_stage == FRAME_RENDER_START) {

	}

	else if (frame_stage == FRAME_NET_UPDATE_POSTDATAUPDATE_START) {

	}

	else if (frame_stage == FRAME_NET_UPDATE_START) {

	}
	original_frame_stage_notify(interfaces::client, frame_stage);
}

void __stdcall hooks::paint_traverse(unsigned int panel, bool force_repaint, bool allow_force) {
	std::string panel_name = interfaces::panel->get_panel_name(panel);
	static unsigned int _panel = 0;

	static bool once = false;
	if (!once) {
		PCHAR panel_char = (PCHAR)interfaces::panel->get_panel_name(panel);
		if (strstr(panel_char, "MatSystemTopPanel")) {
			_panel = panel;
			once = true;
		}
	}

	else if (_panel == panel) {

		int w, h;
		interfaces::engine->get_screen_size(w, h);

		interfaces::surface->set_drawing_color(0, 0, 0, 155);
		interfaces::surface->fillRectangle(w - 100, 10, w - 0, 28);

		interfaces::surface->set_drawing_color(246, 178, 107);
		interfaces::surface->fillRectangle(w - 97, 13, w - 95, 25);

		c_drawing::get().draw_text(w - 91, 12, c_drawing::get().watermark_font, "quadSDK", false, color(255, 255, 255));
	}

	original_panel(interfaces::panel, panel, force_repaint, allow_force);
}

void __stdcall hooks::scene_end() {
	original_scene_end(interfaces::render_view);
}

extern LRESULT ImGui_ImplDX9_WndProcHandler(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
LRESULT __stdcall hooks::wndproc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
	return CallWindowProcA(wndproc_original, hwnd, message, wparam, lparam);
}

void __stdcall hooks::lock_cursor() {
	original_lockcursor(interfaces::surface);
}
long __stdcall hooks::present(IDirect3DDevice9* device, RECT* source_rect, RECT* dest_rect, HWND dest_window_override, RGNDATA* dirty_region) {
	return original_present(device, source_rect, dest_rect, dest_window_override, dirty_region);
}

long __stdcall hooks::reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* present_parameters) {
	long hr = original_reset(device, present_parameters);
	return hr;
}