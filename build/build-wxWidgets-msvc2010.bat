@echo off

rem check whether VC2010 is installed
if "%VS100COMNTOOLS%" == "" (
	echo MSVC not found. >&2
	exit /b 9009
)

cd /d "%~dp0"

setlocal EnableDelayedExpansion

cd ../wxWidgets/build/msw

for %%p in (Win32 x64) do (
	for %%c in ("DLL Debug" "DLL Release") do (
		%COMSPEC% /c ""!VS100COMNTOOLS!\..\..\VC\vcvarsall.bat" && MSBuild.exe /m /property:wxVendor=flederwiesel /p:Configuration=%%c /p:Platform=%%p wx_vc10.sln"
	)
)

rem If started from explorer, let cmd window persist
if /i not "%CMDCMDLINE: =%" == "%COMSPEC%" pause
