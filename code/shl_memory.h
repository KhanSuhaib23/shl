// NOTE(Suhaib): Memory Allocating might be too hard for me now and too complex, avoid it for now

#include <stdint.h>
#include <malloc.h>

#define s32 int32_t
#define u32 uint32_t
#define u8 uint8_t
#define usize unsigned long long

#ifndef SHL_MEMORY_H
	
	#define SHL_MEMORY_H
	
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

	#if defined(SHL_IS_CPP)
		#if defined(SHL_MEMORY_STATIC)
			#define SHLAPI static extern "C"
		#else
			#define SHLAPI extern "C" 
		#endif
	#else
		#if defined(SHL_MEMORY_STATIC)
			#define SHLAPI static
		#else
			#define SHLAPI
		#endif
	#endif

	SHLAPI void* shl_malloc(usize size);
	SHLAPI void shl_free(void* mem);
	SHLAPI usize shl_memsize(void* mem);

	#define shl_nalloc(num, type) shl_malloc(num * sizeof(type))

#endif

#ifdef SHL_MEMORY_IMPLEMENTATION

	SHLAPI void* shl_malloc(usize size)
	{
		usize* total_mem = (usize*) malloc(size + sizeof(usize));
		*total_mem = size;
		return (void*) (total_mem + 1);
	}

	SHLAPI void shl_free(void* mem)
	{
		free((void*) ((usize*) mem - 1));
	}

	SHLAPI usize shl_memsize(void* mem)
	{
		return (usize) *((usize*) mem - 1);
	}


#endif

#undef SHLAPI
#undef usize
#undef s32
#undef u32
#undef u8