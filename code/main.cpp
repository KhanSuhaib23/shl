#include <stdio.h>
#include <iostream>

#define SHL_TIME_IMPLEMENTATION
#include "shl_time.h"

#define SHL_MEMORY_IMPLEMENTATION
#include "shl_memory.h"

#define SHL_THREAD_IMPLEMENTATION
#include "shl_thread.h"

#define SHL_ALTF4CLOSE
#define SHL_ALTF5FULLSCREEN
#define SHL_PLATFORM_IMPLEMENTATION
#include "shl_platform.h"

#define SHL_FILE_IMPLEMENTATION
#include "shl_file.h"

#define SHL_CONTROLLER_IMPLEMENTATION
#include "shl_controller.h"


unsigned long WindowFunction(void* data)
{
	shl_platform* platform = (shl_platform*) shl_get_thread_associated_data(data);

	shl_platform_init(platform);

	while (platform->running)
	{
		shl_platform_pull(platform);

		char typed = platform->typed;

		if (typed) printf("%c", typed);

		int* buffer = (int*) platform->screen_buffer;

		for (int i = 0; i < platform->width * platform->height; i++)
		{
			buffer[i] = 0xff00ff00;
		}

		shl_platform_push(platform);
	}

	return 0;
}

void platform_test()
{
	shl_platform platform_1 = {};

	platform_1.window_name = "shl_platform_window";
	platform_1.width = 600;
	platform_1.height = 400;
	platform_1.x = 20;
	platform_1.y = 40;

	platform_1.bits_per_pixel = 32;

	shl_platform platform_2 = {};

	platform_2.window_name = "shl_platform_window_2";
	platform_2.width = 1280;
	platform_2.height = 720;
	platform_2.x = 100;
	platform_2.y = 200;

	platform_2.bits_per_pixel = 32;


	shl_thread window_1;
	shl_thread window_2;

	shl_create_thread(&window_1, WindowFunction, &platform_1);
	shl_create_thread(&window_2, WindowFunction, &platform_2);

	shl_start_thread(&window_1);
	shl_start_thread(&window_2);

	shl_wait_for_thread(&window_1);
	shl_wait_for_thread(&window_2);
}

void file_test()
{
	shl_file file;

	shl_open_file(&file, "hello.txt", SHL_FILE_READ | SHL_FILE_WRITE);

	int written = shl_append(&file, (void*) "newer text", 10);
	written = shl_append(&file, (void*) "bam", 3);

	shl_close_file(&file);
}


int main(int argc, char** argv)
{	
	shl_controller_init();

	shl_controllers controllers;

	int times = 1000;

	while (1)
	{
		shl_controller_pull(&controllers);

		printf("{%d, %d}\n", controllers.controllers[0].left_trigger, controllers.controllers[0].right_trigger);

		if (times-- >= 0) controllers.controllers[0].left_motor = 65000;
		else controllers.controllers[0].left_motor = 0;

		shl_controller_push(&controllers);
	
	}

	
	return 0;
}

