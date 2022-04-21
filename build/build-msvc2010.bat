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
	for %%d in ( ^
		"%~dp0..\wxWidgets\lib\vc_dll" ^
		"%~dp0..\wxWidgets\lib\vc_x64_dll" ^
		"%~dp0..\wxWidgets\build\msw\vc_mswudll" ^
		"%~dp0..\wxWidgets\build\msw\vc_x64_mswudll" ^
		"%~dp0Win32" ^
		"%~dp0x64" ^
		"%~dp0redist") do (
		if exist %%d rmdir /s /q %%d
	)
)

for %%p in (Win32 x64) do (
	rem === Source environment

	if "Win32" == "%%p" set platform=x86
	if "x64"   == "%%p" set platform=x64

	call "%VS100COMNTOOLS%\..\..\VC\vcvarsall.bat" !platform!

	rem === Build wxWidgets

	for %%c in ("DLL Debug" "DLL Release") do (
		if not exist %~dp0%%p\xWidgets-%%c.stamp (
			if not exist "%~dp0%%p" mkdir "%~dp0%%p"

			MSBuild -m -p:wxVendor=flederwiesel -p:Configuration=%%c -p:Platform=%%p ^
				"%~dp0..\wxWidgets\build\msw\wx_vc10.sln" || ^
			exit /b 2

			type nul >> "%~dp0%%p\xWidgets-%%c.stamp"
		)
	)

	rem === Build fra-airtraffic

	MSBuild -m -p:Configuration=Release -p:Platform=%%p "%~dp0fra-airtraffic.sln" || ^
	exit /b 3

	rem === Download redistributable

	if "x86" == "!platform!" set dlpath=C/6/D/C6D0FD4E-9E53-4897-9B91-836EBA2AACD3/vcredist_x86.exe
	if "x64" == "!platform!" set dlpath=A/8/0/A80747C3-41BD-45DF-B505-E9710D2744E0/vcredist_x64.exe

	if not exist "%~dp0redist" md "%~dp0redist" || exit /b 4

	if not exist redist\vcredist_!platform!.exe (
		cscript.exe //nologo "%~dp0download.vbs" ^
			"https://download.microsoft.com/download/!dlpath!" ^
			"%~dp0redist\vcredist_!platform!.exe" || exit /b 4
	)
)

rem === Build and sign installer

iscc "%~dp0fra-airtraffic.iss" || exit /b 5
