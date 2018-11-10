#define f64 double
#define u64 unsigned long long
#define s64 long long

#ifndef SHL_TIME_H
	#define SHL_TIME_H

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

	#if !defined(SHL_PLATFORM_API_DEF)
		#define SHL_PLATFORM_API_DEF
		#if defined(SHL_CPP)
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
	#endif

	#define shl__cpucycle_intrinsic __rdtsc()


	#if defined(SHL_WIN)
		#include <windows.h>
	#endif



	SHLAPI void          shl_init_clock          ();
	SHLAPI f64           shl_get_time            ();
	SHLAPI f64           shl_elapsed_time        (f64 start_time);
	SHLAPI u64           shl_get_cycle           ();
	SHLAPI u64           shl_elapsed_cycles      (u64 start_cycle);
	
#endif


#ifdef SHL_TIME_IMPLEMENTATION
	
	static f64 shl__frequency;

	SHLAPI void shl_init_clock()
	{
		LARGE_INTEGER freq;

		QueryPerformanceFrequency(&freq);

		shl__frequency = (f64) freq.QuadPart;
	}

	SHLAPI f64 shl_get_time()
	{
		LARGE_INTEGER time;

		QueryPerformanceCounter(&time);

		return (f64) time.QuadPart / shl__frequency;
	}

	SHLAPI f64 shl_elapsed_time(f64 start_time)
	{
		LARGE_INTEGER time;

		QueryPerformanceCounter(&time);

		return ((f64) time.QuadPart / shl__frequency - start_time);
	}

	SHLAPI u64 shl_get_cycle()
	{
		return shl__cpucycle_intrinsic;
	}

	SHLAPI u64 shl_elapsed_cycles(u64 start_cycle)
	{
		return shl__cpucycle_intrinsic - start_cycle;
	}



#endif

#undef SHLAPI
#undef f64
#undef u64
#undef s64