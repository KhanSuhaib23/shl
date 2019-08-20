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

// Create API for both reference and values 
// Change the argument validation code

// NOTE(Suhaib): For some reason gcc doesn;t recogonize Ex functions
// NOTE(Suhaib): unless /NODEFAULTLIB is specified Kernel32.lib is automatically linked
// TODO(Suhaib): Writing your own stdint?


#define s32 long
#define u32 unsigned long
#define ulong unsigned long

#ifndef SHL_THREAD_H
#define SHL_THREAD_H

#ifndef SHL_THREAD_ARG_VALIDATION
#define SHL_THREAD_ARG_VALIDATION 1 
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
    // complex literal are defined differently in c and cpp
    #define SHL_COMP_LIT(type)
#else
    #if defined(SHL_THREAD_STATIC)
        #define SHLAPI static
    #else
        #define SHLAPI
    #endif
    #define SHL_COMP_LIT(type) (type)
#endif



#include <malloc.h>


typedef ulong (thread_function(void* data));

typedef struct  shl_thread                      shl_thread;
typedef struct  shl_semaphore                   shl_semaphore;
typedef struct  shl_mutex                       shl_mutex;
typedef enum    shl_thread_return_code           shl_thread_return_code;
typedef struct  shl_thread_return                shl_thread_return;

enum shl_thread_return_code
{
    SHL_THREAD_SUCCESS = 0,
    SHL_THREAD_NATIVE_ERROR = 1,
    SHL_THREAD_INVALID_ARGS = 2,
    SHL_THREAD_TIMEOUT = 3,
};

struct shl_thread_return 
{
    shl_thread_return_code return_code;
    char* return_message;
    s32 native_return_code;
    char* native_return_message;
};

#if defined(SHL_WIN)



struct shl_thread
{
    HANDLE handle;
    DWORD id;
    thread_function* function;
    void* user_data;
    shl_thread_return return_val;
};


struct shl_semaphore
{
    HANDLE handle;
    shl_thread_return return_val;
};

struct shl_mutex
{
    HANDLE handle;
    shl_thread_return return_val;
};


#endif

SHLAPI shl_thread*       shl_create_thread           (shl_thread* thread, thread_function* function, u32 stack_size, void* data);
SHLAPI shl_thread*       shl_start_thread            (shl_thread* thread);
SHLAPI shl_thread*       shl_pause_thread            (shl_thread* thread);
SHLAPI shl_thread*       shl_terminate_thread        (shl_thread* thread, u32 exit_code);
SHLAPI void*             shl_get_thread_data         (void* thread_context);
SHLAPI shl_thread*       shl_get_thread_from_context (void* thread_context);
SHLAPI shl_semaphore*    shl_create_semaphore        (shl_semaphore* semaphore, u32 initial_count, u32 maximum_count);
SHLAPI shl_semaphore*    shl_semaphore_wait          (shl_semaphore* semaphore);
SHLAPI shl_semaphore*    shl_semaphore_release       (shl_semaphore* semaphore);
SHLAPI shl_mutex*        shl_create_mutex            (shl_mutex* mutex);
SHLAPI shl_mutex*        shl_mutex_lock              (shl_mutex* mutex);
SHLAPI shl_mutex*        shl_mutex_unlock            (shl_mutex* mutex);



#define shl_thread_success(object) ((object) != 0 && (object)->return_val.return_code == SHL_THREAD_SUCCESS)
#define shl_thread_get_return(object) (((object) == 0) ? _shl_def_return() : (object)->return_val)

#endif


#if defined(SHL_THREAD_IMPLEMENTATION)

#define SHL_INF 0

#if defined(SHL_WIN)

shl_thread_return _shl_def_return() {
    return SHL_COMP_LIT(shl_thread_return) { SHL_THREAD_INVALID_ARGS, "Thread parameter could not be NULL", 0, 0};//(shl_thread_return) { SHL_THREAD_INVALID_ARGS, "Thread parameter could not be NULL", 0, 0};
}

static void shl_thread_set_native_return(shl_thread_return* return_val)  
{
    return_val->native_return_code = GetLastError();

    return_val->native_return_message = "Microsoft Error";
}

SHLAPI shl_thread* shl_create_thread(shl_thread* thread, thread_function* function, u32 stack_size, void* data)
{
    
    if (thread && function)
    {
        thread->user_data = data;
        thread->handle = CreateThread(NULL, stack_size, (LPTHREAD_START_ROUTINE) function, thread, CREATE_SUSPENDED, &thread->id);
        
        if (thread->handle) 
        {
            thread->return_val.return_code = SHL_THREAD_SUCCESS;
        } 
        else 
        {
            thread->return_val.return_code = SHL_THREAD_NATIVE_ERROR;
            thread->return_val.return_message = "";

            shl_thread_set_native_return(&thread->return_val);
        }
    } 
    else 
    {
        if (thread) 
        {
            if (!function) 
            {
                thread->return_val.return_code = SHL_THREAD_INVALID_ARGS;
                thread->return_val.return_message = "Parameter function should not be NULL";
            }

        }
    }
    
    return thread;
}

SHLAPI shl_thread* shl_start_thread(shl_thread* thread)
{
    if (thread)
    {
        DWORD ret = ResumeThread(thread->handle);

        if (ret != -1) 
        {
            thread->return_val.return_code = SHL_THREAD_SUCCESS;
        }
        else 
        {
            thread->return_val.return_code = SHL_THREAD_NATIVE_ERROR;
            thread->return_val.return_message = "";

            shl_thread_set_native_return(&thread->return_val);
        }
    }
    
    return thread;
}

SHLAPI shl_thread* shl_pause_thread(shl_thread* thread)
{
    if (thread)
    {
        DWORD ret = SuspendThread(thread->handle);

        if (ret != -1) 
        {
            thread->return_val.return_code = SHL_THREAD_SUCCESS;
        }
        else
        {
            thread->return_val.return_code = SHL_THREAD_NATIVE_ERROR;
            thread->return_val.return_message = "";

            shl_thread_set_native_return(&thread->return_val);
        }

    }
    
    return thread;
}

SHLAPI shl_thread* shl_terminate_thread(shl_thread* thread, u32 exit_code)
{
    if (thread)
    {
        DWORD ret = TerminateThread(thread->handle, exit_code);

        if (ret) 
        {
            thread->return_val.return_code = SHL_THREAD_SUCCESS;
        }
        else 
        {
            thread->return_val.return_code = SHL_THREAD_NATIVE_ERROR;
            thread->return_val.return_message = "";

            shl_thread_set_native_return(&thread->return_val);
        }
    }

    return thread;
}


SHLAPI shl_thread* shl_wait_for_thread(shl_thread* thread, DWORD time_in_ms)
{
    if (thread) 
    {
        if (time_in_ms == SHL_INF) 
        {
            time_in_ms = INFINITE;
        }
        
        DWORD ret = WaitForSingleObject(thread->handle, time_in_ms);

        if (ret == WAIT_OBJECT_0) 
        {
            thread->return_val.return_code = SHL_THREAD_SUCCESS;
        }
        else if (ret == WAIT_TIMEOUT) 
        {
            thread->return_val.return_code = SHL_THREAD_TIMEOUT;
            thread->return_val.return_message = "Alloted time elapsed before the thread was signalled";
        }
        else
        {
            thread->return_val.return_code = SHL_THREAD_NATIVE_ERROR;

            shl_thread_set_native_return(&thread->return_val);            
        }
    }   
    
    return thread;    
}

SHLAPI void* shl_get_thread_data(void* thread_context)
{
    void* user_data = NULL;
    
    if (thread_context)
    {
        user_data = ((shl_thread*) thread_context)->user_data;        
    }
    
    return user_data;
}

SHLAPI shl_thread* shl_get_thread_from_context(void* thread_context)
{
    shl_thread* thread = NULL;
    
    if (thread_context)
    {
        thread = (shl_thread*) thread_context;
    }
    
    return thread;
}

SHLAPI shl_semaphore* shl_create_semaphore(shl_semaphore* semaphore, u32 initial_count, u32 maximum_count)
{
    if (semaphore)
    {
        if (initial_count <= maximum_count) 
        {
            semaphore->handle = CreateSemaphoreEx(0, initial_count, maximum_count, NULL, 0, SYNCHRONIZE | SEMAPHORE_MODIFY_STATE);

            if (semaphore->handle)
            {
                semaphore->return_val.return_code = SHL_THREAD_SUCCESS;
            }
            else
            {
                semaphore->return_val.return_code = SHL_THREAD_NATIVE_ERROR;
                semaphore->return_val.return_message = "Couldn't create semaphore object";
                
                shl_thread_set_native_return(&semaphore->return_val);
            }

        }
        else 
        {
            semaphore->return_val.return_code = SHL_THREAD_INVALID_ARGS;

            semaphore->return_val.return_message = "Initial Count must be less than or equal to maximum count";
        }
        
    }

    return semaphore;
    
}

SHLAPI shl_semaphore* shl_semaphore_wait(shl_semaphore* semaphore)
{
    if (semaphore)
    {
        WaitForSingleObject(semaphore->handle, INFINITE);

        semaphore->return_val.return_code = SHL_THREAD_SUCCESS;
    }
    
    return semaphore;
}

SHLAPI shl_semaphore* shl_semaphore_release(shl_semaphore* semaphore)
{
    
    if (semaphore)
    {
        ReleaseSemaphore(semaphore->handle, 1, NULL);

        semaphore->return_val.return_code = SHL_THREAD_SUCCESS;
    }
    
    return semaphore;
}

SHLAPI shl_mutex* shl_create_mutex(shl_mutex* mutex)
{
    if (mutex)
    {
        mutex->handle = CreateMutexEx(NULL, NULL, 0, SYNCHRONIZE);

        if (mutex->handle) 
        {
            mutex->return_val.return_code = SHL_THREAD_SUCCESS;
        }
        else
        {
            mutex->return_val.return_code = SHL_THREAD_NATIVE_ERROR;
            shl_thread_set_native_return(&mutex->return_val);
        }
    }

    return mutex;
    
}


SHLAPI shl_mutex* shl_mutex_lock(shl_mutex* mutex)
{
    if (mutex)
    {
        WaitForSingleObject(mutex->handle, INFINITE);
        mutex->return_val.return_code = SHL_THREAD_SUCCESS;
    }
    
    return mutex;
}

SHLAPI shl_mutex* shl_mutex_unlock(shl_mutex* mutex)
{
    if (mutex)
    {
        if (ReleaseMutex(mutex->handle))
        {
            mutex->return_val.return_code = SHL_THREAD_SUCCESS;
        }
        else
        {
            mutex->return_val.return_code = SHL_THREAD_NATIVE_ERROR;
            shl_thread_set_native_return(&mutex->return_val);
        }
    }
    
    return mutex;			
}


#endif

#endif




#undef SHLAPI
#undef s32
#undef u32
#undef ulong