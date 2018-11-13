// TODO(Suhaib): Only have done for NO GL don't know for others

#include <stdint.h>

#define u32 uint32_t
#define s32 int32_t
#define uint unsigned int
#define u8 uint8_t
#define f32 float

#ifndef SHL_PLATFORM_H
	#define SHL_PLATFORM_H

	#define SHL_MAX_KEYS 256
	#define SHL_MAX_MBUTTON 8
	
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
			#define SHL_IS_CPP
		#else
			#define SHL_IS_C
		#endif

	#endif


	#if defined(SHL_WIN)
		#include <windows.h>
		#include <windowsx.h>
	
		#pragma comment(lib, "Gdi32.lib")
		#pragma comment(lib, "User32.lib")

	#endif


	#if defined(SHL_IS_CPP)
		
		#define SHL_ZERO_INIT(type) {}
		
		#if defined(SHL_PLATFORM_STATIC)
			#define SHLAPI static extern "C"
		#else
			#define SHLAPI extern "C" 
		#endif
	#else

		#define SHL_ZERO_INIT(type) (type) {0}

		#if defined(SHL_PLATFORM_STATIC)
			#define SHLAPI static
		#else
			#define SHLAPI
		#endif
	#endif


	#ifndef shl_platform_malloc
		#include <malloc.h>
		#define shl_platform_malloc malloc
	#endif

	
	#ifndef shl_platform_free
		#include <malloc.h>
		#define shl_platform_free free
	#endif	

	typedef enum shl_virtual_keys
	{
		SHL_NULL                       = 0x00,
	    SHL_BREAK	                   = 0x03,
	    SHL_BACKSPACE                  = 0x08,
	    SHL_TAB	                       = 0x09,
	    SHL_CLEAR	                   = 0x0C,
	    SHL_ENTER 	                   = 0x0D,
	    SHL_SHIFT                      = 0x10,
	    SHL_CTRL                       = 0x11,
	    SHL_ALT                        = 0x12,
	    SHL_PAUSE                      = 0x13,
	    SHL_CAPSLOCK                   = 0x14,
	    SHL_KANA                       = 0x15,
	    SHL_JUNJA                      = 0x17,
	    SHL_FINAL                      = 0x18,
	    SHL_KANJI                      = 0x19,
	    SHL_ESCAPE	                   = 0x1B,
	    SHL_CONVERT                    = 0x1C,
	    SHL_NONCONVERT                 = 0x1D,
	    SHL_ACCEPT                     = 0x1E,
	    SHL_MODECHANGE                 = 0x1F,
	    SHL_SPACE	                   = 0x20,
	    SHL_PAGEUP                     = 0x21,
	    SHL_PAGEDOWN                   = 0x22,
	    SHL_END                        = 0x23,
	    SHL_HOME                       = 0x24,
	    SHL_DLEFT                      = 0x25,
	    SHL_DUP                        = 0x26,
	    SHL_DRIGHT                     = 0x27,
	    SHL_DDOWN                      = 0x28,
	    SHL_SELECT	                   = 0x29,
	    SHL_PRINT                      = 0x2A,
	    SHL_EXECUTE	                   = 0x2B,
	    SHL_PRINTSCREEN                = 0x2C,
	    SHL_INSERT                     = 0x2D,
	    SHL_DELETE                     = 0x2E,
	    SHL_HELP                       = 0x2F,
	    SHL_0    	                   = 0x30,
	    SHL_1    	                   = 0x31,
	    SHL_2    	                   = 0x32,
	    SHL_3    	                   = 0x33,
	    SHL_4	                       = 0x34,
	    SHL_5	                       = 0x35,
	    SHL_6	                       = 0x36,
	    SHL_7	                       = 0x37,
	    SHL_8	                       = 0x38,
	    SHL_9	                       = 0x39,
	    SHL_A	                       = 0x41,
	    SHL_B	                       = 0x42,
	    SHL_C	                       = 0x43,
	    SHL_D	                       = 0x44,
	    SHL_E    	                   = 0x45,
	    SHL_F	                       = 0x46,
	    SHL_G	                       = 0x47,
	    SHL_H	                       = 0x48,
	    SHL_I	                       = 0x49,
	    SHL_J    	                   = 0x4A,
	    SHL_K	                       = 0x4B,
	    SHL_L	                       = 0x4C,
	    SHL_M	                       = 0x4D,
	    SHL_N	                       = 0x4E,
	    SHL_O	                       = 0x4F,
	    SHL_P    	                   = 0x50,
	    SHL_Q	                       = 0x51,
	    SHL_R	                       = 0x52,
	    SHL_S	                       = 0x53,
	    SHL_T	                       = 0x54,
	    SHL_U	                       = 0x55,
	    SHL_V    	                   = 0x56,
	    SHL_W	                       = 0x57,
	    SHL_X	                       = 0x58,
	    SHL_Y	                       = 0x59,
	    SHL_Z	                       = 0x5A,
	    SHL_LWINDOW                    = 0x5B,
	    SHL_RWINDOW                    = 0x5C,
	    SHL_APPS                       = 0x5D,
	    SHL_SLEEP                      = 0x5F,
	    SHL_NUMPAD0	                   = 0x60,
	    SHL_NUMPAD1	                   = 0x61,
	    SHL_NUMPAD2	                   = 0x62,
	    SHL_NUMPAD3	                   = 0x63,
	    SHL_NUMPAD4	                   = 0x64,
	    SHL_NUMPAD5	                   = 0x65,
	    SHL_NUMPAD6	                   = 0x66,
	    SHL_NUMPAD7	                   = 0x67,
	    SHL_NUMPAD8	                   = 0x68,
	    SHL_NUMPAD9	                   = 0x69,
	    SHL_NUMPAD_MUL                 = 0x6A,
	    SHL_NUMPAD_ADD                 = 0x6B,
	    SHL_SEPARATOR	               = 0x6C,
	    SHL_NUMPAD_SUB	               = 0x6D,
	    SHL_NUMPAD_DECIMAL             = 0x6E,
	    SHL_NUMPADDIV                  = 0x6F,
	    SHL_F1                         = 0x70,
	    SHL_F2                         = 0x71,
	    SHL_F3                         = 0x72,
	    SHL_F4                         = 0x73,
	    SHL_F5                         = 0x74,
	    SHL_F6                         = 0x75,
	    SHL_F7                         = 0x76,
	    SHL_F8                         = 0x77,
	    SHL_F9                         = 0x78,
	    SHL_F10                        = 0x79,
	    SHL_F11                        = 0x7A,
	    SHL_F12                        = 0x7B,
	    SHL_F13                        = 0x7C,
	    SHL_F14                        = 0x7D,
	    SHL_F15                        = 0x7E,
	    SHL_F16                        = 0x7F,
	    SHL_F17                        = 0x80,
	    SHL_F18                        = 0x81,
	    SHL_F19                        = 0x82,
	    SHL_F20                        = 0x83,
	    SHL_F21                        = 0x84,
	    SHL_F22                        = 0x85,
	    SHL_F23                        = 0x86,
	    SHL_F24                        = 0x87,
	    SHL_NUMLOCK                    = 0x90,
	    SHL_SCROLL                     = 0x91,
	    SHL_JISHO                      = 0x92,
	    SHL_MASSHOU	                   = 0x93,
	    SHL_TOUROKU	                   = 0x94,
	    SHL_LOYA	                   = 0x95,
	    SHL_ROYA	                   = 0x96,
	    SHL_LSHIFT                     = 0xA0,
	    SHL_RSHIFT                     = 0xA1,
	    SHL_LCTRL                      = 0xA2,
	    SHL_RCTRL                      = 0xA3,
	    SHL_LALT                       = 0xA4,
	    SHL_RALT                       = 0xA5,
	    SHL_BROWSER_BACK               = 0xA6,
	    SHL_BROWSER_FORWARD            = 0xA7,
	    SHL_BROWSER_REFRESH            = 0xA8,
	    SHL_BROWSER_STOP               = 0xA9,
	    SHL_BROWSER_SEARCH             = 0xAA,
	    SHL_BROWSER_FAVORITES          = 0xAB,
	    SHL_BROWSER_HOME               = 0xAC,
	    SHL_VOL_MUTE                   = 0xAD,
	    SHL_VOL_DOWN                   = 0xAE,
	    SHL_VOL_UP                     = 0xAF,
	    SHL_MEDIA_NEXT_TRACK           = 0xB0,
	    SHL_MEDIA_PREV_TRACK           = 0xB1,
	    SHL_MEDIA_STOP                 = 0xB2,
	    SHL_MEDIA_PLAY_PAUSE           = 0xB3,
	    SHL_LAUNCH_MAIL                = 0xB4,
	    SHL_LAUNCH_MEDIA_SELECT        = 0xB5,
	    SHL_LAUNCH_APP1                = 0xB6,
	    SHL_LAUNCH_APP2                = 0xB7,
	    SHL_COLON	                   = 0xBA,  // [ :  ; ] 
	    SHL_EQUAL_PLUS                 = 0xBB,  // [ =  + ]
	    SHL_COMMA_LT 	               = 0xBC,  // [ ,  < ]
	    SHL_HYPHEN_UNDERSCORE          = 0xBD,  // [ -  _ ]
	    SHL_PERIOD_GT                  = 0xBE,  // [ .  > ]
	    SHL_BACKSLASH_QUES             = 0xBF,  // [ /  ? ] // TODO(Suhaib): Confirm back and forward slash
	    SHL_SOMETHING_TILDE            = 0xC0,  // [ '  ~ ] // TODO(Suhaib): What is ` called 
	    SHL_ABNT_C1	                   = 0xC1,  
	    SHL_ABNT_C2	                   = 0xC2,  
	    SHL_BOX_CURL_OP                = 0xDB,  // [ [  { ]
	    SHL_FWDSLASH_OR                = 0xDC,  // [ \  | ] // TODO(Suhaib): Confirm back and forward slash
	    SHL_BOX_CURL_CL                = 0xDD,  // [ ]  } ]
	    SHL_INV_COMMAS                 = 0xDE,  // [ '  " ]
	    SHL_OEM_8	                   = 0xDF,  // [Â§  ! ] // TODO(Suhaib): Don't know what this is
	    SHL_AX    	                   = 0xE1,  // // TODO(Suhaib): Note sure what this is
	    SHL_ANG_BRACS                  = 0xE2,  // [ <  > ]
	    SHL_ICO_HELP	               = 0xE3,
	    SHL_ICO_00                     = 0xE4,  // TODO(Suhaib): Everything from this thing on I dont know
	    SHL_PROCESSKEY	               = 0xE5,
	    SHL_ICO_CLEAR	               = 0xE6,
	    SHL_PACKET	                   = 0xE7,
	    SHL_OEM_RESET	               = 0xE9,
	    SHL_OEM_JUMP 	               = 0xEA,
	    SHL_OEM_PA1	                   = 0xEB,
	    SHL_OEM_PA2	                   = 0xEC,
	    SHL_OEM_PA3	                   = 0xED,
	    SHL_OEM_WSCTRL	               = 0xEE,
	    SHL_OEM_CUSEL	               = 0xEF,
	    SHL_OEM_ATTN	               = 0xF0,
	    SHL_OEM_FINISH	               = 0xF1,
	    SHL_OEM_COPY	               = 0xF2,
	    SHL_OEM_AUTO	               = 0xF3,
	    SHL_OEM_ENLW	               = 0xF4,
	    SHL_OEM_BACKTAB	               = 0xF5,
	    SHL_ATTN	                   = 0xF6,
	    SHL_CRSEL	                   = 0xF7,
	    SHL_EXSEL	                   = 0xF8,
	    SHL_EREOF	                   = 0xF9,
	    SHL_PLAY	                   = 0xFA,
	    SHL_ZOOM	                   = 0xFB,
	    SHL_NONAME	                   = 0xFC,
	    SHL_PA1	                       = 0xFD,
	    SHL_OEM_CLEAR	               = 0xFE,
	    SHL_NONE                       = 0xFF 

	} shl_virtual_keys;

	typedef enum shl_virtual_mbuttons
	{	
		SHL_RMBUTTON = 0,
		SHL_LMBUTTON = 1,
		SHL_MMBUTTON = 2

	} shl_virtual_mbuttons;

	typedef enum shl_input_action
	{
		SHL_NOACTION   = 0,  // no action in the current frame
		SHL_PRESSED    = 1,  // the key/button is being held down this current frame 
		SHL_DOWN       = 2,  // the key has just been pressed this current frame
		SHL_RELEASED   = 3   // the key has just been released this current frame

	} shl_input_action;
	
	

	typedef struct shl_platform
	{
		char* window_name;
		int width, height;
		int x, y;
		int prev_width, prev_height;
		int prev_x, prev_y;
		int bits_per_pixel;
		void* screen_buffer;
		int has_keyboard_focus;
		int has_mouse_hover;
		int keys[SHL_MAX_KEYS];
		int buttons[SHL_MAX_MBUTTON];
		char typed;
		s32 x_mpos, y_mpos;
		f32 mouse_wheel_delta;
		int running;
		#ifdef SHL_WIN

		HWND window_handle;
		BITMAPINFO bit_map_info;

		#endif

	} shl_platform;

	SHLAPI uint shl_platform_init(shl_platform* platform);
	SHLAPI void shl_platform_pull(shl_platform* platform);
	SHLAPI void shl_platform_push(shl_platform* platform);
	SHLAPI void shl_toggle_fullscreen(shl_platform* platform);



#endif

#ifdef SHL_PLATFORM_IMPLEMENTATION


	char shl__get_typed_letter(s32 vkcode, s32 caps_val, s32 shft_val)
	{
		char res = 0;

		if (vkcode >= SHL_A && vkcode <= SHL_Z)
		{
			res = (char) ((vkcode - SHL_A) + (caps_val ^ shft_val ? 'A' : 'a'));
		}
		else if (vkcode >= SHL_NUMPAD0 && vkcode <= SHL_NUMPAD9)
		{
			res = (char) ((vkcode - SHL_NUMPAD0) + '0');
		}
		else if (vkcode >= SHL_0 && vkcode <= SHL_9)
		{
			res = (char) ((vkcode - SHL_0) + '0');
		}
		else
		{
			switch(vkcode)
			{
				case SHL_SPACE: 
					res = ' '; 
				break;
				case SHL_ENTER: 
					res = '\n'; 
				break;
				case SHL_BACKSPACE: 
					res = '\b'; 
				break;
				case SHL_TAB: 
					res = '\t'; 
				break;
			}
		}

		return res;
	}

	#if defined(SHL_WIN)

		LRESULT CALLBACK shl__window_callback(HWND window_handle, UINT message, WPARAM wparam, LPARAM lparam)
		{
			LRESULT result = 0;
    
		    shl_platform* platform = (shl_platform*) GetProp(window_handle, "SHL");
		    
		    switch(message)
		    {
		        case WM_DEVICECHANGE:
		        {
		            // for usb and media device plug and remove
		            
		            printf("Device Changed\n");
		        }
		        break;
		        
		        case WM_CLOSE: 
		        {
		            DestroyWindow(window_handle); // sends a destroy message
		        }
		        break;
		        case WM_QUIT:
		        {
		            DestroyWindow(window_handle); // sends a destroy message
		        }
		        break;
		        case WM_DESTROY:
		        {
		            platform->running = 0;
		        }
		        break;
		        
		        case WM_SETFOCUS:
		        {
		            if (platform != NULL)
		            {
		                platform->has_keyboard_focus = 1;
		            }
		        }
		        break;
		        case WM_KILLFOCUS:
		        {
		            if (platform != NULL)
		            {
		                platform->has_keyboard_focus = 0;
		            }
		        }
		        break;
		        case WM_SIZE:
		        {
		            if (platform != NULL)
		            {
		                
		                RECT client_rect = SHL_ZERO_INIT(RECT);

		                GetClientRect(window_handle, &client_rect);

		                int width = client_rect.right - client_rect.left;
		                int height = client_rect.bottom - client_rect.top;

		                platform->bit_map_info.bmiHeader.biSize = sizeof(platform->bit_map_info.bmiHeader);
		                platform->bit_map_info.bmiHeader.biWidth = width;
		                platform->bit_map_info.bmiHeader.biHeight = height;
		                
		                platform->width = width;
		                platform->height = height;
		                
		                if (platform->screen_buffer != NULL)
		                {
		                    shl_platform_free(platform->screen_buffer);
		                }
		                
		                platform->screen_buffer = malloc(platform->width * platform->height * platform->bits_per_pixel / 8);
		                
		            }
		            
		        }
		        break;
		        case WM_MOVE:
		        {
		        }
		        break;
		        default:
		        {
		            result = DefWindowProc(window_handle, message, wparam, lparam);
		        }
		    }
		    
		    return result;
		}

		SHLAPI uint shl_platform_init(shl_platform* platform)
		{
			uint result = 0;

			HINSTANCE instance = GetModuleHandle(NULL);
	    
		    /*Create and Register a window class*/
		    WNDCLASSEX window_class = SHL_ZERO_INIT(WNDCLASSEX); // Creating a window class
		    
		    window_class.cbSize = sizeof(WNDCLASSEX);
		    window_class.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; 
		    window_class.lpfnWndProc = shl__window_callback;
		    window_class.cbClsExtra = 0;
		    window_class.cbWndExtra = 0;
		    window_class.hInstance = instance;
		    window_class.hIcon = NULL; // TODO(Suhaib): Add an icon
		    window_class.hCursor = NULL; // TODO(Suhaib): Add a cursor
		    window_class.hbrBackground = NULL; // We will paint our backgrounds our selves
		    window_class.lpszMenuName = NULL; // No default menu
		    window_class.lpszClassName = platform->window_name;
		    window_class.hIconSm = NULL;
		    
		    if (RegisterClassEx(&window_class) != 0) // TODO(Suhaib): Maybe check unicode vs ASCII
		    {
		        /*Create a window*/
		        HWND window_handle = CreateWindowEx(
		            0,
		            platform->window_name,
		            platform->window_name,
		            WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		            CW_USEDEFAULT, CW_USEDEFAULT,
		            platform->width, platform->height, 
		            NULL,
		            NULL,
		            instance,
		            NULL
		            );
		        
		        if (window_handle != NULL)
		        {
		            
		            platform->bit_map_info.bmiHeader.biSize = sizeof(platform->bit_map_info.bmiHeader);
		            platform->bit_map_info.bmiHeader.biWidth = platform->width;
		            platform->bit_map_info.bmiHeader.biHeight = platform->height;
		            platform->bit_map_info.bmiHeader.biPlanes = 1;
		            platform->bit_map_info.bmiHeader.biBitCount = (WORD) platform->bits_per_pixel;
		            platform->bit_map_info.bmiHeader.biCompression = BI_RGB;
		            platform->bit_map_info.bmiHeader.biSizeImage = 0;
		            platform->bit_map_info.bmiHeader.biXPelsPerMeter = 0;
		            platform->bit_map_info.bmiHeader.biYPelsPerMeter = 0;
		            platform->bit_map_info.bmiHeader.biClrUsed = 0;
		            platform->bit_map_info.bmiHeader.biClrImportant = 0;
		            


		            platform->screen_buffer = shl_platform_malloc(platform->width * platform->height * platform->bits_per_pixel / 8);
		            
		            platform->running = 1;
		            platform->window_handle = window_handle;
		            
		            platform->has_keyboard_focus = 1;
		            
		            SetProp(window_handle, "SHL", platform);

		            result = 1;
		            

		        }
		        else
		        {
		            result = 0;
		        }
		        
		    }
		    else 
		    {
		        result = 0;
		    }

		    return result;
		}

		SHLAPI void shl_platform_pull(shl_platform* platform)
		{
			MSG message;

			platform->typed = 0;

			platform->mouse_wheel_delta = 0.0f;

			for (int i = 0; i < SHL_MAX_KEYS; i++)
			{
				platform->keys[i] = (platform->keys[i] == SHL_PRESSED ? SHL_DOWN : (platform->keys[i] == SHL_RELEASED ? SHL_NOACTION : platform->keys[i]));
			}

			for (int i = 0; i < SHL_MAX_MBUTTON; i++)
			{
				platform->buttons[i] = (platform->buttons[i] == SHL_PRESSED ? SHL_DOWN : (platform->buttons[i] == SHL_RELEASED ? SHL_NOACTION : platform->buttons[i]));	
			}

			u32 vkcode;

			s32 caps_val = GetKeyState(SHL_CAPSLOCK) & 0x1;
			s32 shft_val = (GetKeyState(SHL_SHIFT) >> (sizeof(SHORT) * 8 - 1)) & 0x1;

			//printf("Caps : %d , Shift : %0x\n", caps_val, shft_val);



			// come here


			while (PeekMessage(&message, platform->window_handle, 0, 0, PM_REMOVE))
	        {
	            TranslateMessage(&message);
	            
	            
	            switch (message.message)
	            {
	                case WM_KEYDOWN: // a key is pressed
	                {
	                    vkcode = (u32) message.wParam;

	                    platform->keys[vkcode] = (platform->keys[vkcode] == SHL_DOWN ? SHL_DOWN : SHL_PRESSED);

	                    platform->typed = shl__get_typed_letter(vkcode, caps_val, shft_val);

	                }
	                break;
	                case WM_KEYUP:
	                {
	                    vkcode = (u32) message.wParam;

	                    platform->keys[vkcode] = SHL_RELEASED;
	                }
	                break;
	                case WM_SYSKEYDOWN: // NOTE(Suhaib): SYS KEY UP and DOWN are basically the same as normal ones except we need to handle the same of ALT
	                {
	                    
	                    // TODO(Suhaib): Handle alt key presses properly
	                    
	#ifdef SHL_ALTF4CLOSE
	                    
	                    if (message.wParam == SHL_F4)
	                    {
	                        DestroyWindow(platform->window_handle);
	                    }
	                    
	#endif
	                    
	#ifdef SHL_ALTF5FULLSCREEN
	                    
	                    if (message.wParam == SHL_F5)
	                    {
	                        shl_toggle_fullscreen(platform);
	                    }
	                    
	#endif
	                    
	#ifdef SHL_ALTENTERFULLSCREEN
	                    
	                    if (message.wParam == SHL_ENTER)
	                    {
	                        shl_toggle_fullscreen(platform);
	                    }
	                    
	#endif
	                    
	                }
	                break;
	                case WM_SYSKEYUP:
	                {
	                }
	                break;
	                
	                case WM_MOUSEMOVE:
	                {

	                    platform->x_mpos = GET_X_LPARAM(message.lParam); 
	                    platform->y_mpos = GET_Y_LPARAM(message.lParam);
	                    
	                }
	                break;
	                
	                case WM_MOUSEWHEEL:
	                {
	                    platform->mouse_wheel_delta = (f32) GET_WHEEL_DELTA_WPARAM(message.wParam) / 120.0f;   
	                }
	                break;
	                
	                // TODO(Suhaib): Scan codes for mouse button
	                case WM_LBUTTONDOWN:
	                {
	                    vkcode = (u32) SHL_LMBUTTON;

	                    platform->buttons[vkcode] = (platform->buttons[vkcode] == SHL_DOWN ? SHL_DOWN : SHL_PRESSED);
	                }
	                break;
	                case WM_LBUTTONUP:
	                {
	                    vkcode = (u32) SHL_LMBUTTON;

	                    platform->buttons[vkcode] = SHL_RELEASED;
	                }
	                break;
	                case WM_RBUTTONDOWN:
	                {
	                    vkcode = (u32) SHL_RMBUTTON;

	                    platform->buttons[vkcode] = (platform->buttons[vkcode] == SHL_DOWN ? SHL_DOWN : SHL_PRESSED);
	                }
	                break;
	                case WM_RBUTTONUP:
	                {
	                    vkcode = (u32) SHL_RMBUTTON;

	                    platform->buttons[vkcode] = SHL_RELEASED;
	                }
	                break;
	                case WM_MBUTTONDOWN:
	                {
	                    vkcode = (u32) SHL_MMBUTTON;

	                    platform->buttons[vkcode] = (platform->buttons[vkcode] == SHL_DOWN ? SHL_DOWN : SHL_PRESSED);
	                }
	                break;
	                case WM_MBUTTONUP:
	                {
	                    vkcode = (u32) SHL_MMBUTTON;

	                    platform->buttons[vkcode] = SHL_RELEASED;
	                }
	                break;
	                
	                default:
	                {
	                    DispatchMessage(&message);
	                }
	            }
	        }
		}

		SHLAPI void shl_platform_push(shl_platform* platform)
		{
			StretchDIBits(
		        GetDC(platform->window_handle),
		        0, 0,
		        platform->width, platform->height,
		        0, 0,
		        platform->width, platform->height,
		        platform->screen_buffer,
		        &platform->bit_map_info,
		        DIB_RGB_COLORS, SRCCOPY
		        );
		}

		SHLAPI void shl_toggle_fullscreen(shl_platform* platform)
		{
		    DWORD style = GetWindowLong(platform->window_handle, GWL_STYLE);
		    
		    if (style & WS_OVERLAPPEDWINDOW) // not already full screen
		    {
		        
		        RECT window_rect;
		        
		        GetClientRect(platform->window_handle, &window_rect);
		        
		        // TODO(Suhaib): This is not correct cause left and top are always 0
		        platform->prev_x = window_rect.left;
		        platform->prev_y = window_rect.top;
		        
		        
		        platform->prev_width = window_rect.right - window_rect.left;
		        platform->prev_height = window_rect.bottom - window_rect.top;
		        
		        // TODO(Suhaib): Multi Monitor support
		        int width = GetSystemMetrics(SM_CXSCREEN);
		        int height = GetSystemMetrics(SM_CYSCREEN);
		        
		        SetWindowLong(platform->window_handle, GWL_STYLE, style & ~WS_OVERLAPPEDWINDOW);
		        SetWindowPos(platform->window_handle, HWND_TOP, 0, 0, width, height, SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
		    }
		    else
		    {
		        SetWindowLong(platform->window_handle, GWL_STYLE, style | WS_OVERLAPPEDWINDOW);
		        
		        
		        SetWindowPos(platform->window_handle, HWND_TOP, platform->prev_x, platform->prev_y, platform->prev_width, platform->prev_height, SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
		    }
		    
		}





	#endif

#endif

#undef SHLAPI
#undef u32
#undef s32
#undef uint
#undef u8
#undef f32