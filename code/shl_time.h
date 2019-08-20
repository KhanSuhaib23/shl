/*

** shl_time **

-> This a single header library that provides program timing related function
-> It provides a way to get current time (current time is calculated from different bases so it is useless in and off itself)
-> You can also get the elapsed time between current time and the previous defined time 
-> The same can be done with cycles (cycles are a between way to measure performance since they aren't CPU dependent)
-> It also provides a convinient interface to get a time struct that has all necessary information like 
   cycles, seconds, milliseconds, microseconds, nanoseconds, etc. 
   
Documentation:

To use this library 

 #include "shl_time.h" 
 //in every translation unit where you have to use
 // (this declare the functions and defines data types so should be in every translation unit it is to be used in)
 
 #define SHL_TIME_IMPLEMENTATION
 #include "shl_time.h"
 // in one and only one translation unit 
 // (this defines the functions so should happen only once in the entire program)
 
 #define SHL_TIME_STATIC 
 // define this before including shl_time.h if you want the implementations to be internal to the file
 // using this the functions could not be used in any other file that doesn't also have the implementation
 
Functions

SHLAPI void shl_time_init ();
	* initializes the frequency value (used in conversion from cycles to seconds)
	* must be called before calling any other function
 
SHLAPI f64 shl_get_base_time ();

	* gets a time in seconds
	* the starting point (base) is platform based 
	* therefore in and of itself this is useless
 
SHLAPI f64 shl_get_elapsed_time (f64 start_time);

	* pass in the time gotten from the shl_get_base_time function
	* it returns the time elapsed in seconds from that base
 
SHLAPI u64 shl_get_base_cycle ();

	* gets the number of cycles
	* the starting point (base) is *usually* the time from the most recent time computer was powered up
	* still should not be used in and of itself
 
SHLAPI u64 shl_get_elapsed_cycles (u64 start_cycle);

    * pass in the cycle count gotten from the shl_get_base_cycle function
    * it returns the cycles elapsed from that base
    * cycles are not a good measure of long scale time keeping and should be limited to use in perf testing small bits of code
 
SHLAPI shl_time shl_get_base_time_struct ();

    * gets a time struct getting a convinient list of info
    * the struct provides cycle count, time in seconds (double), milliseconds (double), microseconds (double and long long),
   nanoseconds (double and long long)
    * still should not be used in and of itself for similar reasons as stated above
 
SHLAPI shl_time shl_get_elapsed_time_struct (shl_time start);

    * gets the same time struct as before with all values representing the time/cycles elapsed from the base
 
SHLAPI u32 shl_write_elapsed_time_struct (shl_time* start);

    * same as the above from instead of returning the elapsed time struct, it overwrites the one provided by the user
    * returns 0 if the pointer passed in was a null
    * else returns 1
 
SHLAPI void shl_busy_wait (f64 time);

    * takes in a time and waits using a simple while loop
    * high accuracy but high computation cost as well
 
SHLAPI void shl_busy_wait_till (f64 base_time, f64 time_to_wait_since_base);

    * it busy waits till that time elapsed from the base_time till now is less than the time_to_wait_since_base
 
SHLAPI void shl_display_time_struct (shl_time time);

    * utility function to display the entire time struct in a legible manner
 
NOTE: The function using time struct are just for convinience and are must slower compared to the other functions

Structs

typedef struct shl_time
{
	u64 cycles; 
	f64 seconds;
	f64 milliseconds;
	f64 fmicroseconds;
	u64 umicroseconds;
	f64 fnanoseconds;
	u64 unanoseconds;
} shl_time;

    * mostly self explanatory 
    * microseconds and nanoseconds are available both are double and as long long (u64)
 
Defines

#define MILLISECONDS(seconds) (seconds) * 1000.0
#define MICROSECONDS(seconds) MILLISECONDS(seconds) * 1000.0
#define NANOSECONDS(seconds) MICROSECONDS(seconds) * 1000.0

	* used for conversion from seconds to other units of time
 
*/


/*

NOTES:

 Works on Windows 10 (and mostly Vistas and onwards) for 
  MSVC 19.21.27702.2 (x64 and x86)
  GCC  8.1.0 (using mingw-w64)
  
 Future
  Testing*
  Busy waiting api call*
  Better inlining choice or maybe perf testing to see which is better (no choice)
  Older gcc and msvc versions
  Clang
  
  
 */


///////////////////////////////
//          shl_time         //
///////////////////////////////


// defines for convience (later undefined to avoid conficts with the user)
#define f64 double
#define u64 unsigned long long
#define s64 long long
#define u32 unsigned long

// header gaurd
#ifndef SHL_TIME_H
#define SHL_TIME_H


// if not already detected the platform (OS/Architecture) we detect
// TODO: Right now we are only detecting Windows OS and C/C++ in the future code to detect other architecture 
// and OSes should also be added


#if !defined(SHL_PLATFORM_DETECTED)
	#define SHL_PLATFORM_DETECTED

	#if defined(_WIN32)

		#if defined(_WIN64)
			#define SHL_WIN64
			#define SHL_WIN
		#else 
			#define SHL_WIN32
			#define SHL_WIN
		#endif

	#endif

	#if defined(__cplusplus)
		#define SHL_IS_CPP
	#else
		#define SHL_IS_C
	#endif


#endif

// based on whether user is using C/C++ and whether they want internal functions or not we define 
// SHLAPI to be the appropriate value
// SHLAPI is undefined to allow the user to use different parts of the shl differently


#if defined(SHL_IS_CPP)
	#if defined(SHL_TIME_STATIC)
		#define SHLAPI static extern "C"
	#else
		#define SHLAPI extern "C"
	#endif
#else
	#if defined(SHL_TIME_STATIC)
		#define SHLAPI static
	#else
		#define SHLAPI
	#endif
#endif


// TODO: the CPU intrinsics to get the cycle count may not be same for all CPU architectures

#if defined(SHL_WIN)
	#undef _WIN32_WINNT
	#define _WIN32_WINNT 0x0600
	#include <windows.h>
#endif

#define MILLISECONDS(seconds) (seconds) * 1000.0
#define MICROSECONDS(seconds) MILLISECONDS(seconds) * 1000.0
#define NANOSECONDS(seconds) MICROSECONDS(seconds) * 1000.0

typedef struct shl_time
{
	u64 cycles;
	f64 seconds;
	f64 milliseconds;
	f64 fmicroseconds;
	u64 umicroseconds;
	f64 fnanoseconds;
	u64 unanoseconds;
} shl_time;

SHLAPI void          shl_time_init                   ();
SHLAPI f64           shl_get_base_time               ();
SHLAPI f64           shl_get_elapsed_time            (f64 start_time);
SHLAPI u64           shl_get_base_cycle              ();
SHLAPI u64           shl_get_elapsed_cycles          (u64 start_cycle);
SHLAPI shl_time      shl_get_base_time_struct        ();
SHLAPI shl_time      shl_get_elapsed_time_struct     (shl_time start);
SHLAPI u32           shl_write_elapsed_time_struct   (shl_time* start);
SHLAPI void			 shl_busy_wait                   (f64 time);
SHLAPI void          shl_busy_wait_till              (f64 base_time, f64 time_to_wait_since_base);
SHLAPI void          shl_display_time_struct         (shl_time time);

#endif // shl time definitions end


#ifdef SHL_TIME_IMPLEMENTATION

static f64 shl__frequency;

// cpu cycles are used to measrues really small time interval
// we don't want either the compiler or the CPU reordering instructions around it 
// therefore we have this setup
static inline u64 shl__cpucycle_intrinsic() 
{
	MemoryBarrier();
	_ReadWriteBarrier();
	u64 value = __rdtsc();
	_ReadWriteBarrier();
	MemoryBarrier();
    
	return value;
} 

// windows needs a query performance query frequency to convert the query performance time to seconds
// the freq value doesn't change so we get it and save it
SHLAPI void shl_time_init()
{
    LARGE_INTEGER freq;
    
    QueryPerformanceFrequency(&freq);
    
    shl__frequency = (f64) freq.QuadPart;
}

SHLAPI f64 shl_get_base_time()
{
    LARGE_INTEGER time;
    
    QueryPerformanceCounter(&time);
    
    return (f64) time.QuadPart / shl__frequency;
}

SHLAPI f64 shl_get_elapsed_time(f64 start_time)
{
    LARGE_INTEGER time;
    
    QueryPerformanceCounter(&time);
    
    return ((f64) time.QuadPart / shl__frequency - start_time);
}

SHLAPI u64 shl_get_base_cycle()
{
    return shl__cpucycle_intrinsic();
}

SHLAPI u64 shl_get_elapsed_cycles(u64 start_cycle)
{
    return shl__cpucycle_intrinsic() - start_cycle;
}

// pack all timing info in a single struct for ease of use
SHLAPI shl_time shl_get_base_time_struct()
{
	shl_time time;
	time.cycles = shl_get_base_cycle();
	time.seconds = shl_get_base_time();
	time.milliseconds = time.seconds * 1000.0;
	time.fmicroseconds = time.milliseconds * 1000.0;
	time.umicroseconds = (u64) time.fmicroseconds;
	time.fnanoseconds = time.fmicroseconds * 1000.0;
	time.unanoseconds = (u64) time.fnanoseconds;
    
	return time;
}

SHLAPI shl_time shl_get_elapsed_time_struct(shl_time start)
{
	shl_time time = shl_get_base_time_struct();
    
	time.cycles -= start.cycles;
	time.seconds -= start.seconds;
	time.milliseconds -= start.milliseconds;
	time.fmicroseconds -= start.fmicroseconds;
	time.umicroseconds -= start.umicroseconds;
	time.fnanoseconds -= start.fnanoseconds;
	time.unanoseconds -= start.unanoseconds;
    
	return time;
}

SHLAPI u32 shl_write_elapsed_time_struct(shl_time* start)
{
	u32 result = 0;
    
	if (start) 
	{
		start->cycles = shl_get_elapsed_cycles(start->cycles);
		start->seconds = shl_get_elapsed_time(start->seconds);
		start->milliseconds = start->seconds * 1000.0;
		start->fmicroseconds = start->milliseconds * 1000.0;
		start->umicroseconds = (u64) start->fmicroseconds;
		start->fnanoseconds = start->fmicroseconds * 1000.0;
		start->unanoseconds = (u64) start->fnanoseconds;
        
		result = 1;
	}
    
	return result;
}

// busy wait for the given amount of time
SHLAPI void shl_busy_wait (f64 time)
{
	f64 start = shl_get_base_time();
	while (shl_get_elapsed_time(start) <= time);
}

// usually used to control frame rate
// we have the time at which the frame started
// frame has to end after elapsing a given time
// this function can be used for that
SHLAPI void shl_busy_wait_till (f64 base_time, f64 time_to_wait_since_base) {
	while (shl_get_elapsed_time(base_time) < time_to_wait_since_base);
}

SHLAPI void shl_display_time_struct (shl_time time)
{
	printf("cycles        : %llu\nseconds       : %lf\nmilliseconds  : %lf\nfmicroseconds : %lf\numicroseconds : %llu\nfnanoseconds  : %lf\nunanoseconds  : %llu\n\n", 
           time.cycles, time.seconds, time.milliseconds, time.fmicroseconds, 
           time.umicroseconds, time.fnanoseconds, time.unanoseconds);
}



#endif // end shl time implementations

// undefining stuff to prevent polluting the user space

#undef SHLAPI
#undef f64
#undef u64
#undef s64
#undef u32