@echo off
cd %~dp0
set tools=%~dp0
if not exist "%tools%\..\..\build" md "%tools%\..\..\build"

::VSƽ̨
set vsVersion="Visual Studio 14 2015"
::set vsVersion="Visual Studio 15 2017"
::set vsVersion="Visual Studio 16 2019"

::����
set config=RelWithDebInfo
::set config=Release

::�����������
set product=NewCalcRuleTool

::ƽ̨
set platform=Win32
::set platform=x64

::cmakelists.txt����Ŀ¼
set builderDir=%~dp0\..\..\source\cmake\

::����������Ŀ¼
set cmakeOutputDir=%~dp0\..\..\build\Sln\%platform%\%config%

cmake -S %builderDir% -B %cmakeOutputDir% -G%vsVersion% -A %platform% -D CMAKE_CONFIGURATION_TYPES=%config% -D version=%product%

pause