
#include "gamepadWrapper.hpp"


//HINSTANCE xinputDll = LoadLibrary("xinput1_4.dll");
//typedef DWORD(__stdcall *XInputGetStateEx_t) (DWORD, XINPUT_STATE*);
//XInputGetStateEx_t XInputGetState = (XInputGetStateEx_t)GetProcAddress(xinputDll, (LPCSTR)100);


namespace remapper
{
	namespace wrapper
	{

		void send_inp_down(int8_t code)
		{
			INPUT inp;
			inp.type = INPUT_KEYBOARD;
			inp.ki.wScan = 0; // hardware scan code for key
			inp.ki.time = 0;
			inp.ki.dwExtraInfo = 0;

			inp.ki.wVk = code; // virtual-key code for the "a" key
			inp.ki.dwFlags = 0; // 0 for key press
			SendInput(1, &inp, sizeof(INPUT));
		}


		void guideButtonPressed(XINPUT_STATE state, bool wasPressedPrev, bool powerOff, uint8_t whichKey, uint64_t delays, uint64_t durations, bool shouldHold)
		{
			//
		}
	}
}