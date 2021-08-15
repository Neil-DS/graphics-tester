@echo off
rem turning off some error warnings that are of no use.
set TurnOff=-wd4324 -wd4201 -wd4101 -wd4189 -wd4700 -wd4701 -wd4100 -wd4267 -wd28251
set CompilerFlags=%TurnOff% -GR- -MTd -nologo -EHsc -EHa- -Od -Oi -W4 -Z7 -std:c++17

rem additonal include/library directory locations for searching.
set add_include= -I..\dependancies\include
set add_lib= -LIBPATH:..\dependancies\lib


IF NOT EXIST ..\build mkdir ..\build
pushd ..\build

call ..\code\logbuilder.bat
rem cl /c %CompilerFlags% %add_include% ..\code\shader.cpp -Fmshader.map

cl /c %CompilerFlags% %add_include% ..\code\main.cpp -Fmmain.map

cl %CompilerFlags% %add_include% ..\code\win32_main.cpp -link %add_lib% user32.lib gdi32.lib winmm.lib glew32.lib glew32s.lib Opengl32.lib logging.lib shader.obj main.obj

popd
