
#include <stdio.h>

// #define SHL_TIME_IMPLEMENTATION
// #include "shl_time.h"


#define SHL_THREAD_REF_API
#define SHL_THREAD_IMPLEMENTATION
#include "shl_thread.h"

#define SHL_TIME_IMPLEMENTATION
#include "shl_time.h"

#if 0
void time_test() 
{
	int sleep = 5000;
    
	// printf("Hello World\n");
	shl_time_init();
    
	double start_time, end_time;
	unsigned long long start_cycle, end_cycle;
	shl_time start_struct, end_struct;
    
	start_time = shl_get_base_time();
    
	Sleep(sleep);
    
	end_time = shl_get_elapsed_time(start_time);
    
	printf("time(): %lf\n", end_time);
    
	start_cycle = shl_get_base_cycle();
    
	Sleep(sleep);
    
	end_cycle = shl_get_elapsed_cycles(start_cycle);
    
	printf("cycles(): %llu\n", end_cycle);
    
	start_struct = shl_get_base_time_struct();
    
	Sleep(sleep);
	
	end_struct = shl_get_elapsed_time_struct(start_struct);
    
	shl_display_time_struct(end_struct);
    
    
	end_struct = shl_get_base_time_struct();
    
	shl_busy_wait(3.0);
    
	shl_write_elapsed_time_struct(&end_struct);
    
	shl_display_time_struct(end_struct);
}
#endif

#define N 5

shl_semaphore can_produce;
shl_semaphore can_consume;

int produce_index = 0;
int consume_index = 0;

unsigned long producer(void* data) 
{
	while (1) 
	{
		shl_semaphore_wait(&can_produce);
		printf("Producing %d\n", produce_index++);
		shl_semaphore_release(&can_consume);

		shl_busy_wait(1.0f);
	}

	return 0;
}

unsigned long consumer(void* data) 
{
	while (1) 
	{
		shl_semaphore_wait(&can_consume);
		printf("Consuming %d\n", consume_index++);
		shl_semaphore_release(&can_produce);

		shl_busy_wait(2.0f);

	}

	return 0;
}


int main(int argc, char** argv)
{

	shl_time_init();
	shl_thread thread1;
	shl_thread thread2;

	shl_create_semaphore(&can_produce, N, N);
	shl_create_semaphore(&can_consume, 0, N);

	shl_create_thread(&thread1, producer, 0, 0);
	shl_create_thread(&thread2, consumer, 0, 0);

	shl_start_thread(&thread1);
	shl_start_thread(&thread2);


	shl_wait_for_thread(&thread1, SHL_INF);
	shl_wait_for_thread(&thread2, SHL_INF);


}