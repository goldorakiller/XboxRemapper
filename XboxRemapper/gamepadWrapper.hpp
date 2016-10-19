#pragma once
#ifndef GAMEPADWRAPPERHPP
#define GAMEPADWRAPPERHPP


#include <windows.h>
#include <Xinput.h>
#include "INI.hpp"
/*
xinput dll ordinals

ordinal hint RVA      name

1		0	00002700 DllMain
5		1	00002F90 XInputEnable
7		2	00003160 XInputGetBatteryInformation
4		3	00002DA0 XInputGetCapabilities
6		4	000036F0 XInputGetDSoundAudioDeviceGuids
8		5	000039C0 XInputGetKeystroke
2		6	00002850 XInputGetState (without guide button)
3		7	00002C20 XInputSetState
100			00002AA0 XInputGetState
101			00003430 [NONAME]
102			000035D0 [NONAME]
103			00003040 [NONAME]

Version 1_3
100			XInputGetStateEx(x, x)
101			XInputWaitForGuideButton(x, x, x) - help needed with the parameters
102			XInputCancelGuideButtonWait(int) - guessing parameter here
103			XInputPowerOffController(int)
Version 1_4
104			XInputGetBaseBusInformation(x, x)
108			XInputGetCapabilitiesEx(x, x, x, x)
*/


/* https://msdn.microsoft.com/en-us/library/microsoft.directx_sdk.reference.xinput_gamepad(v=vs.85).aspx */
//typedef struct _XINPUT_GAMEPAD {
//	WORD  wButtons;
//	BYTE  bLeftTrigger;
//	BYTE  bRightTrigger;
//	SHORT sThumbLX;
//	SHORT sThumbLY;
//	SHORT sThumbRX;
//	SHORT sThumbRY;
//} XINPUT_GAMEPAD, *PXINPUT_GAMEPAD;

/* https://msdn.microsoft.com/en-us/library/microsoft.directx_sdk.reference.xinput_state(v=vs.85).aspx */
//typedef struct _XINPUT_STATE {
//	DWORD          dwPacketNumber;
//	XINPUT_GAMEPAD Gamepad;
//} XINPUT_STATE, *PXINPUT_STATE;

namespace remapper
{
	namespace wrapper
	{
		typedef DWORD(WINAPI* XInputPowerOffController_t)(DWORD);
		typedef DWORD(WINAPI* XInputGetStateEx) (DWORD, XINPUT_STATE*);

		void send_inp_down(int8_t code);

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

		void guideButtonPressed(XINPUT_STATE state, bool wasPressedPrev, bool powerOff, uint8_t whichKey, uint64_t delays, uint64_t durations, bool shouldHold);
		// todo, analog controls (bLeftTrigger, bRightTrigger, sThumbLX, sThumbLY, sThumbRX, and sThumbRY)
	}
}

#endif // GAMEPADWRAPPER