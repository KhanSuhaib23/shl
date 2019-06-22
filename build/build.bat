@echo off

set FilesToCompile=../code/main.c

if %compiler% == msvc32 (
set CommonCompilerFlags=/nologo /MD /EHsc /Od /Zi /W4 /wd4100 /wd4099 /wd4204 /wd4996 /wd4505 /we4702 /wd4201 /wd4200 /wd4133
set Defines=/D_CRT_SECURE_NO_WARNINGS
set LinkLibraries=Kernel32.lib Ole32.lib


cl %CommonCompilerFlags% %Defines% %FilesToCompile% /link %AdditionalLibDirectories% %LinkLibraries%
)

if %compiler% == msvc64 (
set CommonCompilerFlags=/nologo /MD /EHsc /Od /Zi /W4 /wd4100 /wd4099 /wd4204 /wd4996 /wd4505 /we4702 /wd4201 /wd4200 /wd4133
set Defines=/D_CRT_SECURE_NO_WARNINGS
set LinkLibraries=Kernel32.lib Ole32.lib


cl %CommonCompilerFlags% %Defines% %FilesToCompile% /link %AdditionalLibDirectories% %LinkLibraries%
)

if %compiler% == gcc (
set CommonCompilerFlags=/nologo /MD /EHsc /Od /Zi /W4 /wd4100 /wd4099 /wd4204 /wd4996 /wd4505 /we4702 /wd4201 /wd4200 /wd4133
set Defines=/D_CRT_SECURE_NO_WARNINGS
set LinkLibraries=Kernel32.lib Ole32.lib


gcc %FilesToCompile% -o main.exe -mwindows
)




popd
