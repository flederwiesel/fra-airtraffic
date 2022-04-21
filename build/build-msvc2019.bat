@echo off

setlocal EnableExtensions EnableDelayedExpansion

rem === Determine Visual Studio Version

set VSWHERE="%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"

for /f "usebackq tokens=*" %%p in (`%VSWHERE% ^
		-format value -latest -version 16 -property installationPath`) do (
	set VS=%%p
)

if "%VS%" == "" exit /b 1

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

	call "%VS%\VC\Auxiliary\Build\vcvarsall.bat" !platform!

	rem === Build wxWidgets

	for %%c in ("DLL Debug" "DLL Release") do (
		if not exist %~dp0%%p\xWidgets-%%c.stamp (
			if not exist "%~dp0%%p" mkdir "%~dp0%%p"

			MSBuild -m -p:wxVendor=flederwiesel -p:Configuration=%%c -p:Platform=%%p ^
				"%~dp0..\wxWidgets\build\msw\wx_vc16.sln" || ^
			exit /b 2

			type nul >> "%~dp0%%p\xWidgets-%%c.stamp"
		)
	)

	rem === Build fra-airtraffic

	MSBuild -m -p:Configuration=Release -p:Platform=%%p "%~dp0fra-airtraffic.sln" || ^
	exit /b 3

	rem === Download redistributable

	if "x86" == "!platform!" set dlpath=8ecb9800-52fd-432d-83ee-d6e037e96cc2/50A3E92ADE4C2D8F310A2812D46322459104039B9DEADBD7FDD483B5C697C0C8/VC_redist.x86.exe
	if "x64" == "!platform!" set dlpath=89a3b9df-4a09-492e-8474-8f92c115c51d/B1A32C71A6B7D5978904FB223763263EA5A7EB23B2C44A0D60E90D234AD99178/VC_redist.x64.exe

	if not exist "%~dp0redist" md "%~dp0redist" || exit /b 4

	if not exist "%~dp0redist\vcredist_!platform!.exe" (
		cscript.exe //nologo "%~dp0download.vbs" ^
			"https://download.visualstudio.microsoft.com/download/pr/!dlpath!" ^
			"%~dp0redist\vcredist_!platform!.exe" || exit /b 4
	)
)

rem === Build and sign installer

iscc "%~dp0fra-airtraffic.iss" || exit /b 5
