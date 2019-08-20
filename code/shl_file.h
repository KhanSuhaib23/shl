// TODO(Suhaib): Error correction

#define s64 long long
#define u32 unsigned long

#ifndef SHL_FILE_H
#define SHL_FILE_H

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
#endif

#if defined(SHL_IS_CPP)
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

typedef enum shl_file_access
{	
	SHL_FILE_READ = 0x1,
	SHL_FILE_WRITE = 0x2,

} shl_file_access;

typedef enum shl_file_offset_origin
{
	SHL_FILE_START,
	SHL_FILE_CURR,
	SHL_FILE_END

} shl_file_offset_origin;

typedef struct shl_file
{
	long long size;

	#if defined(SHL_WIN)
	HANDLE file_handle;
	#endif

} shl_file;


SHLAPI void      shl_open_file          (shl_file* file, char* filename, u32 access);
SHLAPI u32       shl_read               (shl_file* file, void* buffer, u32 length);
SHLAPI u32       shl_read_at            (shl_file* file, void* buffer, u32 offset, shl_file_offset_origin origin, u32 length);
SHLAPI u32       shl_write              (shl_file* file, void* buffer, u32 length);
SHLAPI u32       shl_write_at           (shl_file* file, void* buffer, u32 offset, shl_file_offset_origin origin, u32 length);
SHLAPI u32       shl_append             (shl_file* file, void* buffer, u32 length);
SHLAPI void      shl_close_file         (shl_file* file);
SHLAPI void      shl_set_file_pointer   (shl_file* file, u32 offset, shl_file_offset_origin origin);

#endif // SHL_FILE_H

#ifdef SHL_FILE_IMPLEMENTATION

#if defined(SHL_WIN)

SHLAPI void shl_file_open(shl_file* file, char* filename, u32 access)
{
	file->file_handle = CreateFile(filename, 
									((access & SHL_FILE_READ) != 0 ? GENERIC_READ : 0) | ((access & SHL_FILE_WRITE) != 0 ? GENERIC_WRITE : 0),
									0, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	LARGE_INTEGER temp;

	GetFileSizeEx(file->file_handle, &temp);

	file->size = temp.QuadPart;

}

SHLAPI u32 shl_file_read(shl_file* file, void* buffer, u32 length)
{
	DWORD read = 0;
	if (ReadFile(file->file_handle, buffer, length, &read, 0))
	{
		return (u32) read;
	}
	else 
	{
		return 0;
	}
}

SHLAPI u32 shl_file_write(shl_file* file, void* buffer, u32 length)
{
	DWORD written = 0;
	if (WriteFile(file->file_handle, buffer, length, &written, 0))
	{
		return (u32) written;
	}
	else 
	{
		return 0;
	}

}

SHLAPI u32 shl_file_read_at(shl_file* file, void* buffer, u32 offset, shl_file_offset_origin origin, u32 length)
{
	LARGE_INTEGER _offset;

	_offset.QuadPart = (LONGLONG) offset;
	SetFilePointerEx(file->file_handle, _offset, 0, origin); 

	return shl_file_read(file, buffer, length);
}

SHLAPI u32 shl_file_write_at(shl_file* file, void* buffer, u32 offset, shl_file_offset_origin origin, u32 length)
{
	LARGE_INTEGER _offset;

	_offset.QuadPart = (LONGLONG) offset;
	SetFilePointerEx(file->file_handle, _offset, 0, origin);
	
	return shl_file_write(file, buffer, length);
}	

SHLAPI u32 shl_file_append(shl_file* file, void* buffer, u32 length)
{
	LARGE_INTEGER _offset;

	_offset.QuadPart = (LONGLONG) 0;
	SetFilePointerEx(file->file_handle, _offset, 0, SHL_FILE_END);

	return shl_file_write(file, buffer, length);
}

SHLAPI u32 shl_file_set_pointer(shl_file* file, u32 offset, shl_file_offset_origin origin)
{
	LARGE_INTEGER _offset;

	_offset.QuadPart = (LONGLONG) offset;
	return SetFilePointerEx(file->file_handle, _offset, 0, origin) != 0;
}

SHLAPI void shl_file_close(shl_file* file)
{
	CloseHandle(file->file_handle);
	file->file_handle = 0;
	file->size = 0;
}




#endif // SHL_WIN

#endif // SHL_FILE_IMPLEMENTATION


#undef SHLAPI
#undef u32
#undef s64