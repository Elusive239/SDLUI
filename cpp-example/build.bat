@echo off

set path=D:\dev\SDLUI;%path%

set SOURCES=main.cpp
set LINK=opengl32.lib winmm.lib gdi32.lib shell32.lib lib\SDL3.lib lib\SDL3main.lib lib\SDL3_ttf.lib lib\SDL3_image.lib

REM DEBUG
set CFLAGS=/MP /MTd /EHsc /Zi /FC /nologo /W1 /Od

REM RELEASE
REM set CFLAGS=/MP /MT /EHsc /FC /nologo /W1 /O2

set BUILD_DIR=/Fdbuild\ /Fobuild\
set EXE=build/sdlui_test.exe

cl %CFLAGS% %BUILD_DIR% /Fe%EXE% /Iinclude %SOURCES% /link %LINK% /SUBSYSTEM:CONSOLE

rem IF %ERRORLEVEL% EQU 0 start /wait %EXE%