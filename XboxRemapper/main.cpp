/*******************************************************************
 * Xbox 360 controller remapper
 * By Reed Jones
 * Based off pinumbernumber's solution found below
 * https://github.com/jcoddaire/Xbox-360-Guide-Button-Remapper/tree/master/button_on_360_guide_v5/src
 *******************************************************************/

// #include <iostream>

using namespace std;

//#include <stdio.h>
#include <stdbool.h>
#include <windows.h>
#include <chrono>
#include <thread>
#include "INI.hpp"
#include "gamepadWrapper.hpp"

void send_inp(int8_t code, int64_t delay, int64_t duration)
{
	INPUT inp;
	inp.type = INPUT_KEYBOARD;
	inp.ki.wScan = 0; // hardware scan code for key
	inp.ki.time = 0;
	inp.ki.dwExtraInfo = 0;

	std::this_thread::sleep_for(std::chrono::milliseconds(delay));

	/********************* PROOF OF CONCEPT ***********************/
	/**************** LIKELY HORRIBLY INEFECIENT ******************/
	HINSTANCE xinputDll = LoadLibrary("xinput1_4.dll");

	typedef DWORD(__stdcall *XInputGetStateEx_t) (DWORD, XINPUT_STATE*);

	XInputGetStateEx_t XInputGetStateEx = (XInputGetStateEx_t)GetProcAddress(xinputDll, (LPCSTR)100);

	XINPUT_STATE playerStates[4];

	XInputGetStateEx(0, &(playerStates[0]));

	if (playerStates[0].Gamepad.wButtons & remapper::wrapper::GUIDE_BUTTON)
	{
		// Press the key
		inp.ki.wVk = code; // virtual-key code for the key
		inp.ki.dwFlags = 0; // 0 for key press
		SendInput(1, &inp, sizeof(INPUT));

		std::this_thread::sleep_for(std::chrono::milliseconds(duration));

		// Release the key
		inp.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
		SendInput(1, &inp, sizeof(INPUT));
	}
}

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

void send_inp_up(int8_t code)
{
	INPUT inp;
	inp.type = INPUT_KEYBOARD;
	inp.ki.wScan = 0; // hardware scan code for key
	inp.ki.time = 0;
	inp.ki.dwExtraInfo = 0;

	inp.ki.wVk = code; // virtual-key code for the "a" key
	inp.ki.dwFlags = KEYEVENTF_KEYUP; // 0 for key press
	SendInput(1, &inp, sizeof(INPUT));
}

int main()
{
	//TCHAR dll_path[MAX_PATH];
	//GetSystemDirectory(dll_path, sizeof(dll_path));
	//strcat_s(dll_path, "\\xinput1_4.dll");		// xinput1_3.dll for windows 7 & older
	HINSTANCE xinputDll = LoadLibrary("xinput1_4.dll");

	typedef DWORD(__stdcall *XInputGetStateEx_t) (DWORD, XINPUT_STATE*);

	XInputGetStateEx_t XInputGetStateEx = (XInputGetStateEx_t)GetProcAddress(xinputDll, (LPCSTR)100);

	XINPUT_STATE playerStates[4];
	for (auto& state : playerStates)
		ZeroMemory(&state, sizeof(XINPUT_STATE));

	//
	bool wasPressedPrev[4] = { false,false,false,false };
	// constant for all controllers
	//bool powerOff[4] = { false,false,false,false };




	printf("Congratulations, the code compiled!\nTry pressing a button\n\n");

	uint8_t whichKey[4];
	int64_t delays[4];
	int64_t durations[4];
	bool    shouldHold[4];
	bool powerOff[4];

	int64_t minimiseDelay = 0;

	{
		// load config
		remapper::iniReader::INIFile ini;
		ini.loadFile("button.ini");

		// load defaults if config is messed up
		for (int i = 0; i < 4; ++i)
		{
			whichKey[i] = ini.getInteger("config", "key_player" + to_string(i + 1), 500);
			delays[i] = ini.getInteger("config", "delay_player" + to_string(i + 1), 0);
			durations[i] = ini.getInteger("config", "duration_player" + to_string(i + 1), 0);
			shouldHold[i] = (ini.getInteger("config", "hold_player" + to_string(i + 1), 0) == 1);
			powerOff[i] = (ini.getInteger("config", "off_player" + to_string(i + 1), 0) == 1);
		}
	}

	for (;;)
	{
		for (int i = 0; i < 4; i++)
		{
			// ensure player even exists
			if (XInputGetStateEx(i, &(playerStates[i])) == ERROR_SUCCESS)
			{
				//// Diagnose which button was pressed, and what its mapped too
				/*if (playerStates[i].Gamepad.wButtons != 0)
				{
					printf("Player %d pressed %d on the gamepad.\n", i+1, playerStates[i].Gamepad);
					printf("This is mapped to %d on the keyboard\n\n", whichKey[i]);
				}*/

				// compare with guide button, if true, check if its alread being held
				if (playerStates[i].Gamepad.wButtons & remapper::wrapper::GUIDE_BUTTON)
				{
					// check if it was previously pressed (as in being held... for-loops move fast!)
					if (!wasPressedPrev[i])
					{
						// if controller poweroff is desired
						if (powerOff[i])
						{
							// poweroff code found here: https://sourceforge.net/projects/turnoffxboxcontroller/
							XInputPowerOffController_t realXInputPowerOffController = (XInputPowerOffController_t)GetProcAddress(xinputDll, (LPCSTR)103);
							realXInputPowerOffController(i);
							printf("The Controller is powered down for player %d.\n", i + 1);
						}
						// send keypress
						else
						{
							// what key is "pressed
							printf("Player %i pressed %d\n", i + 1, playerStates[i].Gamepad);

							if (!shouldHold[i])
							{
								send_inp_down(whichKey[i]);
							}
							else
							{
								//TODO:: Go into send_inp and add a timer, of delay[i] seconds, after which,
								// check if button is still being held, and run action
								// (may want to look into threading). perhaps ask simon.
								send_inp(whichKey[i], delays[i], durations[i]);
							}
						}
					}

					wasPressedPrev[i] = true;
				}
				else
				{
					if ((shouldHold[i]) && (wasPressedPrev[i]))
					{
						send_inp_up(whichKey[i]);
					}
					wasPressedPrev[i] = false;
				}
			}
		}

		// 33
		Sleep(40);
	}

	// free library, safely exit.
	FreeLibrary(xinputDll);
	return 0;
}
