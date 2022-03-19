@echo off

setlocal EnableExtensions EnableDelayedExpansion

if "%VS100COMNTOOLS%" == "" (
	echo MSVC not found. >&2
	exit /b 9009
)

rem === Delete output, if clean build is requested

if "%1" == "/c" set build=clean
if "%1" == "-c" set build=clean
if "%1" == "/clean" set build=clean
if "%1" == "--clean" set build=clean

if "%build%" == "clean" (
	for %%d in ("%~dp0Win32" "%~dp0x64") do (
		if exist "%%d" rmdir /s /q "%%d"
	)
)

for %%p in (Win32 x64) do (
	rem === Source environment

	if "Win32" == "%%p" set platform=x86
	if "x64"   == "%%p" set platform=x64

	call "%VS100COMNTOOLS%\..\..\VC\vcvarsall.bat" %platform%

	rem === Build fra-airtraffic

	MSBuild -m -p:Configuration=Release -p:Platform=%%p "%~dp0fra-airtraffic.sln" || ^
	exit /b 2
)

:: download resistributable
if not exist "%~dp0redist" md "%~dp0redist" || exit /b %errorlevel%

if not exist redist\vcredist_x86.exe (
	cscript.exe //nologo "%~dp0download.vbs" ^
		"https://download.microsoft.com/download/C/6/D/C6D0FD4E-9E53-4897-9B91-836EBA2AACD3/vcredist_x86.exe" ^
		"%~dp0redist\vcredist_x86.exe" || exit /b %errorlevel%
)

if not exist redist\vcredist_x64.exe (
	cscript.exe //nologo "%~dp0download.vbs" ^
		"https://download.microsoft.com/download/A/8/0/A80747C3-41BD-45DF-B505-E9710D2744E0/vcredist_x64.exe" ^
		"%~dp0redist\vcredist_x64.exe" || exit /b %errorlevel%
)

iscc "%~dp0fra-airtraffic.iss"

if %errorlevel% == 0 goto :eof

:error
@echo off
rem If started from explorer, let cmd window persist
if /i not "%CMDCMDLINE:"=%" == "%COMSPEC%" pause
exit /b %errorlevel%
