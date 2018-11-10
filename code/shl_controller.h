#ifndef SHL_CONTROLLER_H
	#define SHL_CONTROLLER_H

	#if !defined(SHL_PLATFORM_DETECTED)
		#define SHL_PLATFORM_DETECTED

		#if defined(_WIN32)

			#if defined(_WIN64)
				#define SHL_WIN32
				#define SHL_WIN
			#else 
				#define SHL_WIN64
				#define SHL_WIN
			#endif 

		#endif

		#if defined(__cplusplus)
			#define SHL_CPP
		#else
			#define SHL_C
		#endif

	#endif


	#if defined(SHL_WIN)
		#include <windows.h>
		#include <xinput.h>

		#define SHL_CONTROLLER_MAX_COUNT XUSER_MAX_COUNT

	#endif

	#if defined(SHL_CPP)
		#if defined(SHL_FILE_STATIC)
			#define SHLAPI static extern "C"
		#else
			#define SHLAPI extern "C" 
		#endif
	#else
		#if defined(SHL_FILE_STATIC)
			#define SHLAPI static
		#else
			#define SHLAPI
		#endif
	#endif
	
	typedef struct shl_controller
	{
		int connected;
	    int up, down, left, right;
	    int start, select;
	    int left_thumb, right_thumb;
	    int left_shoulder, right_shoulder;
	    int a, b, x, y;
	    int left_trigger, right_trigger;
	    int left_analog_x, left_analog_y;
	    int right_analog_x, right_analog_y;
	    int right_motor, left_motor;

	} shl_controller;

	typedef struct shl_controllers
	{
		shl_controller controllers[XUSER_MAX_COUNT];

	} shl_controllers;

	SHLAPI void shl_controller_init();
	SHLAPI void shl_controller_pull(shl_controllers* controllers);
	SHLAPI void shl_controller_push(shl_controllers* controllers);


#endif

#ifdef SHL_CONTROLLER_IMPLEMENTATION

	#if defined(SHL_WIN)

		typedef DWORD shl__get_controller_signature(DWORD , XINPUT_STATE* ); 
		typedef DWORD shl__set_controller_signature(DWORD , XINPUT_VIBRATION* );

		static shl__get_controller_signature* shl__get_controller_state; // function pointer to XInputGetState
		static shl__set_controller_signature* shl__set_controller_state; // function pointer to XInputSetState

		DWORD shl__get_controller_stub(DWORD index, XINPUT_STATE* state)
		{
			return ERROR_SUCCESS;
		}

		DWORD shl__set_controller_stub(DWORD index, XINPUT_VIBRATION* vibration)
		{
			return ERROR_SUCCESS;
		}

		SHLAPI void shl_controller_init()
		{
			HMODULE xinput_dll = LoadLibrary("xinput1_4.dll"); // try loading the latest version
    
		    if (xinput_dll == NULL) // not found
		    {
		        xinput_dll = LoadLibrary("xinput1_3.dll"); // recursively try to load earlier versions
		        
		        if (xinput_dll == NULL)
		        {
		            xinput_dll = LoadLibrary("xinput9_1_0.dll");
		        }
		    }
		    
		    if (xinput_dll != NULL)
		    {
		        shl__get_controller_state = (shl__get_controller_signature*) GetProcAddress(xinput_dll, "XInputGetState");
		        shl__set_controller_state = (shl__set_controller_signature*) GetProcAddress(xinput_dll, "XInputSetState");
		    }
		}

		SHLAPI void shl_controller_pull(shl_controllers* controllers)
		{
			for (int i = 0; i < XUSER_MAX_COUNT; i++)
			{

				controllers->controllers[i].left_motor = 0;
				controllers->controllers[i].right_motor = 0;

				XINPUT_STATE state;

				if (shl__get_controller_state(i, &state) == ERROR_SUCCESS)
				{
					XINPUT_GAMEPAD gamepad_state = state.Gamepad;

					controllers->controllers[i].connected = 1;
					controllers->controllers[i].up             = (gamepad_state.wButtons & XINPUT_GAMEPAD_DPAD_UP) != 0;
		            controllers->controllers[i].down           = (gamepad_state.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) != 0;
		            controllers->controllers[i].left           = (gamepad_state.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) != 0;
		            controllers->controllers[i].right          = (gamepad_state.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) != 0;
		            controllers->controllers[i].start          = (gamepad_state.wButtons & XINPUT_GAMEPAD_START) != 0;
		            controllers->controllers[i].select         = (gamepad_state.wButtons & XINPUT_GAMEPAD_BACK) != 0;
		            controllers->controllers[i].left_thumb     = (gamepad_state.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) != 0;
		            controllers->controllers[i].right_thumb    = (gamepad_state.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) != 0;
		            controllers->controllers[i].left_shoulder  = (gamepad_state.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) != 0;
		            controllers->controllers[i].right_shoulder = (gamepad_state.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0;
		            controllers->controllers[i].a              = (gamepad_state.wButtons & XINPUT_GAMEPAD_A) != 0;
		            controllers->controllers[i].b              = (gamepad_state.wButtons & XINPUT_GAMEPAD_B) != 0;
		            controllers->controllers[i].x              = (gamepad_state.wButtons & XINPUT_GAMEPAD_X) != 0;
		            controllers->controllers[i].y              = (gamepad_state.wButtons & XINPUT_GAMEPAD_Y) != 0;
		            controllers->controllers[i].left_trigger   = gamepad_state.bLeftTrigger;
		            controllers->controllers[i].right_trigger  = gamepad_state.bRightTrigger;
		            controllers->controllers[i].left_analog_x  = gamepad_state.sThumbLX;
		            controllers->controllers[i].left_analog_y  = gamepad_state.sThumbLY;
		            controllers->controllers[i].right_analog_x = gamepad_state.sThumbRX;
		            controllers->controllers[i].right_analog_y = gamepad_state.sThumbRY;	
				}
				else
				{
					controllers->controllers[i].connected = 0;
				}

				
			}
		}

		SHLAPI void shl_controller_push(shl_controllers* controllers)
		{
			for (int i = 0; i < XUSER_MAX_COUNT; i++)
			{
				XINPUT_VIBRATION vibration = {(WORD)controllers->controllers[i].left_motor, (WORD)controllers->controllers[i].right_motor};

				shl__set_controller_state(i, &vibration);		
			}
		}


	#endif

#endif

#undef SHLAPI
