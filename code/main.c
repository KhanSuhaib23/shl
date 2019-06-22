#if 0

#include <stdio.h>
#include <stdlib.h> 

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

#define SHL_TIME_IMPLEMENTATION
#include "shl_time.h"

#include "shl_buffer.h"

#define SHL_AUDIO_IMPLEMENTATION
#include "shl_audio.h"


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
	shl_platform platform_1;

	platform_1.window_name = "shl_platform_window";
	platform_1.width = 600;
	platform_1.height = 400;
	platform_1.x = 20;
	platform_1.y = 40;

	platform_1.bits_per_pixel = 32;

	shl_platform platform_2;

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

	shl_init_clock();

	int size = 1000000;

	char* buffer = malloc(size);

	char arr[62] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

	for (int i = 0; i < size; i++)
	{
		buffer[i] = arr[i % 62];
	}

	shl_open_file(&file, "hello.txt", SHL_FILE_WRITE);

	shl_write(&file, buffer, size);

	shl_close_file(&file);


	double start = shl_get_time();
	shl_open_file(&file, "hello.txt", SHL_FILE_READ);

	printf("%d\n", file.size);

	shl_read(&file, buffer, file.size);

	
	printf("%lf\n", shl_elapsed_time(start) * 1000.0);


	shl_close_file(&file);
}

int clamp(int value, int min, int max)
{
	return (value > max ? max : (value < min ? min : value));
}

#if 1

void audio_test()
{
	shl_init_clock();

	int freq = 48;
	int count = 0;
	shl_audio_init();

	shl_audio audio = (shl_audio) {0};

	audio.buffer_size_seconds = 2.0f;

	shl_audio_create(&audio);

	double start;

	double target_frame_time = 1.0 / 30.0;

	int no_frame = 0;

	while (1)
	{
		start = shl_get_time();

		shl_audio_pull(&audio);

		int to_write = (int) (audio.samples_per_second * target_frame_time);// + audio.max_samples_to_write) / 2;

		to_write = clamp(to_write, audio.min_samples_to_write, audio.max_samples_to_write);

		if (to_write)
		{
			printf("{%d, %d}\n", no_frame, to_write);	
			no_frame = 0;
		}
		else
		{
			no_frame++;
		}

		

		float* buffer = (float*) audio.buffer;

		for (int i = 0; i < to_write; i++)
		{
			if (count < freq / 2)
			{
				*buffer = 0.7f;
				buffer++;
				*buffer = 0.7f;
				buffer++;
			}
			else
			{
				*buffer = 0.0f;
				buffer++;
				*buffer = 0.0f;
				buffer++;
			}

			count = (count + 1) % freq;
		}

		audio.actual_samples_written = to_write;

		shl_audio_push(&audio);

		while (shl_elapsed_time(start) < target_frame_time);

	}
}

#endif


#define MINE 0


#if 0

void buffer_test()
{
	shl_init_clock();

	#if MINE

	double total_time = 0.0;

	int total_times = 100000;
	int times = total_times;

	long long total_size = 0;

	while (times--)
	{
		int size = rand();

		total_size += size;

		double start = shl_get_time();

		shl_buffer(int) buff = 0;

		shl_buffer_reserve(buff, size);

		for (int i = 0; i < size; i++)
		{
			buff[i] = i;
		}

		double time = shl_elapsed_time(start);	

		total_time += time;

		shl_buffer_free(buff);
	}


	printf("MINE -> %lf   :    Avg Size : %lf\n", total_time, (double) total_size / (double) total_times);

	#else

	double total_time = 0.0;


	int total_times = 100000;
	int times = total_times;

	long long total_size = 0;


	while (times--)
	{
		int size = rand();

		total_size += size;

		double start = shl_get_time();
		std::vector<int> vec;

		vec.reserve(size);

		for (int i = 0; i < size; i++)
		{
			vec[i] = i;
		}

		double time = shl_elapsed_time(start);	

		total_time += time;
	}
	

	printf("THEIR -> %lf   :    Avg Size : %lf\n", total_time, (double) total_size / (double) total_times);

	 #endif
}

#endif



int main(int argc, char** argv)
{	

	file_test();
	

	return 0;
}

#include <stdio.h>
#include <stdint.h>
#include <windows.h>

int arr[128]; 

#define START() QueryPerformanceCounter(&start)
#define END() QueryPerformanceCounter(&end)

#define TIME(count) ((end.QuadPart - start.QuadPart) * 1000) / (count)

int main()
{ 
	int size = 256 * 1024 * 1024;

	int offset = 1;

	int* mall = malloc(size * sizeof(int));

	unsigned int t;

	LARGE_INTEGER start, end;

	QueryPerformanceFrequency(&start);

	printf("%lld\n", start.QuadPart);

	START();

	for (int i = 0; i < size; i+=offset)
	{
		mall[i] = 0;
	}

	END();

	printf("%lld\n",  TIME(size / offset));

	return 0;
}





#define SHL_TIME_IMPLEMENTATION
#define SHL_TIME_STATIC
#include "shl_time.h"
#include <stdio.h>


int main(int argc, char** argv)
{
	shl_time_init();

	shl_time start = shl_get_base_time_struct();

	double wait = 5.0;

	double st = shl_get_base_time();

	while (shl_get_elapsed_time(st) <= wait);

	shl_write_elapsed_time_struct(&start);

	double sec = shl_get_elapsed_time(st);

	printf("seconds: %lf\nmilliseconds: %lf\nmicroseconds: %lf\nnanoseconds: %lf\n\n", sec, MILLISECONDS(sec), MICROSECONDS(sec), 
	NANOSECONDS(sec));


}
#else

#include <stdio.h>

#define SHL_TIME_IMPLEMENTATION
#include "shl_time.h"

#define SHL_THREAD_IMPLEMENTATION
#include "shl_thread.h"

// #define SHL_PLATFORM_IMPLEMENTATION
// #include "shl_platform.h"




int main(int argc, char** argv)
{
	// printf("%s\n", __FUNCTION__);


	// shl_time_init();


}


#endif



