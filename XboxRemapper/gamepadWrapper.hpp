#pragma once
#ifndef GAMEPADWRAPPERHPP
#define GAMEPADWRAPPERHPP


#include <windows.h>

/* https://msdn.microsoft.com/en-us/library/microsoft.directx_sdk.reference.xinput_gamepad(v=vs.85).aspx */
typedef struct _XINPUT_GAMEPAD {
	WORD  wButtons;
	BYTE  bLeftTrigger;
	BYTE  bRightTrigger;
	SHORT sThumbLX;
	SHORT sThumbLY;
	SHORT sThumbRX;
	SHORT sThumbRY;
} XINPUT_GAMEPAD, *PXINPUT_GAMEPAD;

/* https://msdn.microsoft.com/en-us/library/microsoft.directx_sdk.reference.xinput_state(v=vs.85).aspx */
typedef struct _XINPUT_STATE {
	DWORD          dwPacketNumber;
	XINPUT_GAMEPAD Gamepad;
} XINPUT_STATE, *PXINPUT_STATE;

typedef DWORD(WINAPI* XInputPowerOffController_t)(DWORD i);

namespace remapper
{
	namespace wrapper
	{
		// buttons
		const int 
			DPAD_UP			= 0x0001,
			DPAD_DOWN		= 0x0002,
			DPAD_LEFT		= 0x0004,
			DPAD_RIGHT		= 0x0008,

			START_BUTTON	= 0x0010,
			BACK_BUTTON		= 0x0020,

			LEFT_THUMB		= 0x0040,
			RIGHT_THUMB		= 0x0080,

			LEFT_SHOULDER	= 0x0100,
			RIGHT_SHOULDER	= 0x0200,

			GUIDE_BUTTON	= 0x400,

			A_BUTTON		= 0x1000,
			B_BUTTON		= 0x2000,
			X_BUTTON		= 0x4000,
			Y_BUTTON		= 0x8000;

		// todo, analog controls (bLeftTrigger, bRightTrigger, sThumbLX, sThumbLY, sThumbRX, and sThumbRY)
	}
}

#endif // GAMEPADWRAPPER