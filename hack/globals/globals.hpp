#include "../../includes.hpp"
namespace g
{
	extern HWND csgo_hwnd;
	extern bool send_packet;
	extern int choke_amount;
	extern bool disable_fakelag;
	extern vec3_t real_angles;
	extern vec3_t fake_angles;
	using msg_t = void(__cdecl*)(const char*, ...);
	extern msg_t		Msg;
}