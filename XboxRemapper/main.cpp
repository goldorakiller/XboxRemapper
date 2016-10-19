/*******************************************************************
 * Xbox 360 controller remapper
 * By Reed Jones
 * Based off pinumbernumber's solution found below
 * https://github.com/jcoddaire/Xbox-360-Guide-Button-Remapper/tree/master/button_on_360_guide_v5/src
 *******************************************************************/

 // #include <iostream>


//#include <stdio.h>
//#include <stdbool.h>
#include <windows.h>
#include <Xinput.h>
//#include <chrono>
//#include <thread>
#include "INI.hpp"
#include "gamepadWrapper.hpp"

int main()
{
	// get the xinput dll, for older computers (such as windows 7), try xinput1_3.dll instead
	HINSTANCE xinputDll = LoadLibrary("xinput1_4.dll");
	remapper::wrapper::XInputGetStateEx XInputGetState = (remapper::wrapper::XInputGetStateEx)GetProcAddress(xinputDll, (LPCSTR)100);
	remapper::wrapper::XInputPowerOffController_t realXInputPowerOffController = (remapper::wrapper::XInputPowerOffController_t)GetProcAddress(xinputDll, (LPCSTR)103);

	XINPUT_STATE state;										// Stores the gamepad state of all players
	ZeroMemory(&state, sizeof(XINPUT_STATE));

	uint8_t whichKey[4];									// key to be simulated

	bool powerOff[4],										// if guide button is to become power button for controller
		wasPressedPrev[4] = { false,false,false,false };	// no key has been pressed yet

	// load ini config
	remapper::iniReader::INIFile ini;
	ini.loadFile("button.ini");

	// load defaults if config is messed up. Last digit is the defaults.
	for (int i = 0; i < 4; ++i)
	{
		std::string player = "_player" + std::to_string(i + 1);				// player1, player2 etc...
		whichKey[i] = ini.getInteger("config", "key" + player, 27);			// defaults to escape
		powerOff[i] = ini.getInteger("config", "off" + player, 0);			// defaults to not poweroff
	}

	printf("Congratulations, the code compiled!\nTry pressing a button (probably the guide at this point)\n\n");

	for (;;)
	{
		for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
		{
			// ensure player even exists & controller is connected
			if (XInputGetState(i, &state) == ERROR_SUCCESS)
			{
				//guideButtonPressed(state, wasPressedPrev[i], powerOff[i], whichKey[i], delays[i], durations[i], shouldHold[i]);
				////// Diagnose which button was pressed, and what its mapped too
				if (state.Gamepad.wButtons != 0)
				{
					printf("Player %d pressed %d on the gamepad.\n", i + 1, state.Gamepad);
					printf("This is mapped to %d on the keyboard\n\n", whichKey[i]);
				}

				// compare with guide button, if true, check if its alread being held
				if (state.Gamepad.wButtons & remapper::wrapper::GUIDE_BUTTON)
				{
					// check if it was previously pressed so as to only press once
					if (!wasPressedPrev[i])
					{
						// if controller poweroff is desired
						if (powerOff[i])
						{
							// poweroff code found here: https://sourceforge.net/projects/turnoffxboxcontroller/
							realXInputPowerOffController(i);
							printf("The Controller is powered down for player %d.\n", i + 1);
						}
						// send keypress
						else
						{
							// what key is "pressed
							printf("Player %i pressed %d\n", i + 1, state.Gamepad);

							// press key
							remapper::wrapper::send_inp_down(whichKey[i]);
						}
					}

					wasPressedPrev[i] = true;
				}
				else
				{
					wasPressedPrev[i] = false;
				}
			}
		}
		// 33
		Sleep(40);
	}

	// free library, safely exit.
	FreeLibrary(xinputDll);
	xinputDll = NULL;
	return 0;
}
