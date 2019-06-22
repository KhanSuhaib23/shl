// TODO(Suhaib): Load every os specific function dynamically
/* Important(Suhaib):
*  Windows Functions used
*  CreateThread
*  ResumeThread
*  SuspendThread
*  TerminateThread
*  WaitForSingleObject
*  CreateSemaphoreEx
*  CreateEvent
*  SetEvent
*  ResetEvent
*  ReleaseSemaphore
*  
*/

// NOTE(Suhaib): For some reason gcc doesn;t recogonize Ex functions
// NOTE(Suhaib): unless /NODEFAULTLIB is specified Kernel32.lib is automatically linked
// TODO(Suhaib): Writing your own stdint?


#include <stdint.h>

#define s32 int32_t
#define u32 uint32_t
#define ulong unsigned long

#ifndef SHL_THREAD_H
	#define SHL_THREAD_H

	#ifndef SHL_THREAD_ARG_CHECK
		#define SHL_THREAD_ARG_CHECK 1
	#endif
	
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
		#undef _WIN32_WINNT
		#define _WIN32_WINNT 0x0600
		#include <windows.h>
	#endif

	#if defined(SHL_IS_CPP)
		#if defined(SHL_THREAD_STATIC)
			#define SHLAPI static extern "C"
		#else
			#define SHLAPI extern "C" 
		#endif
	#else
		#if defined(SHL_THREAD_STATIC)
			#define SHLAPI static
		#else
			#define SHLAPI
		#endif
	#endif


	#include <malloc.h>


	typedef ulong (thread_function(void* data));

	typedef struct shl_thread                     shl_thread;
	typedef struct shl_semaphore                  shl_semaphore;
	typedef struct shl_mutex                      shl_mutex;

	#if defined(SHL_WIN)

		struct shl_thread
		{
		    HANDLE handle;
		    DWORD id;
		    thread_function* function;
		    void* user_data;
		};


		struct shl_semaphore
		{
		    HANDLE handle;
		    HANDLE full_event;
		};

		struct shl_mutex
		{
		    HANDLE handle;
		};


	#endif

	SHLAPI u32                         shl_create_thread               (shl_thread* thread, thread_function* function, void* data);
	SHLAPI u32                         shl_start_thread                (shl_thread* thread);
	SHLAPI u32                         shl_pause_thread                (shl_thread* thread);
	SHLAPI u32                         shl_terminate_thread            (shl_thread* thread, u32 exit_code);
	SHLAPI u32                         shl_wait_for_thread             (shl_thread* thread);
	SHLAPI void*                       shl_get_thread_associated_data  (void* thread_context);
	SHLAPI shl_thread*                 shl_get_thread_from_context     (void* thread_context);

	SHLAPI u32                         shl_create_semaphore            (shl_semaphore* semaphore, s32 initial_count, s32 maximum_count);
	SHLAPI u32                         shl_semaphore_wait              (shl_semaphore* semaphore);
	SHLAPI u32                         shl_semaphore_signal            (shl_semaphore* semaphore);

	SHLAPI u32                         shl_create_mutex                (shl_mutex* mutex);
	SHLAPI u32                         shl_mutex_lock                  (shl_mutex* mutex);
	SHLAPI u32                         shl_mutex_unlock                (shl_mutex* mutex);

	


#endif

#if defined(SHL_THREAD_IMPLEMENTATION)

	#if defined(SHL_WIN)


		SHLAPI u32 shl_create_thread(shl_thread* thread, thread_function* function, void* data)
		{
			u32 result = 0;

			#if SHL_THREAD_ARG_CHECK

				if (thread)
				{
					thread->user_data = data;
					thread->handle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) function, thread, CREATE_SUSPENDED, &thread->id); 	
					result = 1;
				}

			#else
				thread->user_data = data;
				thread->handle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) function, thread, CREATE_SUSPENDED, &thread->id); 	
				result = 1;
			#endif

			return result;
		}

		SHLAPI u32 shl_start_thread(shl_thread* thread)
		{
			u32 result = 0;

			#if SHL_THREAD_ARG_CHECK

				if (thread)
				{
					ResumeThread(thread->handle);
					result = 1;
				}

			#else
				ResumeThread(thread->handle);
				result = 1;
			#endif

			return result;

			
		}

		SHLAPI u32 shl_pause_thread(shl_thread* thread)
		{
			u32 result = 0;

			#if SHL_THREAD_ARG_CHECK

				if (thread)
				{
					SuspendThread(thread->handle);
					result = 1;
				}

			#else
				SuspendThread(thread->handle);
				result = 1;
			#endif

			return result;
		}

		SHLAPI u32 shl_terminate_thread(shl_thread* thread, u32 exit_code)
		{
			u32 result = 0;

			#if SHL_THREAD_ARG_CHECK

				if (thread)
				{
					TerminateThread(thread->handle, exit_code);
					result = 1;
				}

			#else
				TerminateThread(thread->handle, exit_code);
				result = 1;
			#endif

			return result;
		}

		SHLAPI u32 shl_wait_for_thread(shl_thread* thread)
		{

			u32 result = 0;

			#if SHL_THREAD_ARG_CHECK

				if (thread)
				{
					WaitForSingleObject(thread->handle, INFINITE);
					result = 1;
				}

			#else
				WaitForSingleObject(thread->handle, INFINITE);
				result = 1;
			#endif

			return result;
		}

		SHLAPI void* shl_get_thread_associated_data(void* thread_context)
		{
			void* user_data = NULL;

			#if SHL_THREAD_ARG_CHECK

				if (thread_context)
				{
					user_data = ((shl_thread*) thread_context)->user_data;
				}

			#else
				user_data = ((shl_thread*) thread_context)->user_data;
			#endif

			return user_data;
		}

		SHLAPI shl_thread* shl_get_thread_from_context(void* thread_context)
		{
			shl_thread* thread = NULL;

			#if SHL_THREAD_ARG_CHECK

				if (thread_context)
				{
					thread = (shl_thread*) thread_context;
				}

			#else
				thread = (shl_thread*) thread_context;
			#endif

			return thread;
		}

		SHLAPI u32 shl_create_semaphore(shl_semaphore* semaphore, s32 initial_count, s32 maximum_count)
		{
			u32 result = 0;

			#if SHL_THREAD_ARG_CHECK
				if (semaphore)
				{
					semaphore->handle = CreateSemaphoreEx(0, initial_count, maximum_count, NULL, 0, SYNCHRONIZE | SEMAPHORE_MODIFY_STATE);
					semaphore->full_event = CreateEvent(NULL, FALSE, TRUE, NULL);
					result = 1;
				}
							
			#else
				semaphore->handle = CreateSemaphoreEx(0, initial_count, maximum_count, NULL, 0, SYNCHRONIZE | SEMAPHORE_MODIFY_STATE);	
				result = 1;
			#endif

			return result;
		}

		SHLAPI u32 shl_semaphore_wait(shl_semaphore* semaphore)
		{
			u32 result = 0;

			#if SHL_THREAD_ARG_CHECK
				if (semaphore)
				{
					WaitForSingleObject(semaphore->handle, INFINITE);
					SetEvent(semaphore->full_event);
					result = 1;
				}
							
			#else
				WaitForSingleObject(semaphore->handle, INFINITE);
				SetEvent(semaphore->full_event);
				result = 1;
			#endif

			return result;
		}

		SHLAPI u32 shl_semaphore_release(shl_semaphore* semaphore)
		{
			u32 result = 0;

			#if SHL_THREAD_ARG_CHECK
				if (semaphore)
				{
					if (ReleaseSemaphore(semaphore->handle, 1, NULL) == FALSE)
					{
						ResetEvent(semaphore->full_event);

						WaitForSingleObject(semaphore->full_event, INFINITE);
						
						ReleaseSemaphore(semaphore->handle, 1, NULL);
					}
					result = 1;
				}
							
			#else
				if (ReleaseSemaphore(semaphore->handle, 1, NULL) == FALSE)
				{
					ResetEvent(semaphore->full_event);

					WaitForSingleObject(semaphore->full_event, INFINITE);
					
					ReleaseSemaphore(semaphore->handle, 1, NULL);
				}
				result = 1;
			#endif

			return result;
		}

		SHLAPI u32 shl_create_mutex(shl_mutex* mutex)
		{
			
			u32 result = 0;

			#if SHL_THREAD_ARG_CHECK
				if (mutex)
				{
					mutex->handle = CreateMutexEx(NULL, NULL, 0, SYNCHRONIZE);
					result = 1;
				}

			#else
				mutex->handle = CreateMutexEx(NULL, NULL, 0, SYNCHRONIZE);
				result = 1;
			#endif

			return result;
		}

		SHLAPI u32 shl_mutex_lock(shl_mutex* mutex)
		{
			u32 result = 0;

			#if SHL_THREAD_ARG_CHECK
				if (mutex)
				{
					WaitForSingleObject(mutex->handle, INFINITE);
					result = 1;
				}
							
			#else
				WaitForSingleObject(mutex->handle, INFINITE);
				result = 1;
			#endif

			return result;
		}

		SHLAPI u32 shl_mutex_unlock(shl_mutex* mutex)
		{
			u32 result = 0;

			#if SHL_THREAD_ARG_CHECK
				if (mutex)
				{
					ReleaseMutex(mutex->handle);
					result = 1;
				}
							
			#else
				ReleaseMutex(mutex->handle);
				result = 1;
			#endif

			return result;			
		}


	#endif

#endif

#undef SHLAPI
#undef s32
#undef u32
#undef ulong