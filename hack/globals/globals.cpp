#include "globals.hpp"
namespace g
{
	HWND csgo_hwnd = FindWindow(0, "Counter-Strike: Global Offensive");

	bool send_packet;
	int choke_amount;
	bool disable_fakelag;
	vec3_t real_angles;
	vec3_t fake_angles;
	using msg_t = void(__cdecl*)(const char*, ...);
	msg_t Msg = reinterpret_cast<msg_t>(GetProcAddress(GetModuleHandleA("tier0.dll"), "Msg"));

}