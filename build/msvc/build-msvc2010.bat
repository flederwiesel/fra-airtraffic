if "%VS100COMNTOOLS%" == "" (
	echo MSVC not found. >&2
	exit /b 9009
)

rd /S /Q "%~dp0Win32" "%~dp0x64"

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

%COMSPEC% /c ""%VS100COMNTOOLS%\..\..\VC\vcvarsall.bat" x86 && MSBuild.exe /p:Configuration=Release /p:Platform=Win32 %~dp0\fra-airtraffic.sln" && ^
%COMSPEC% /c ""%VS100COMNTOOLS%\..\..\VC\vcvarsall.bat" x64 && MSBuild.exe /p:Configuration=Release /p:Platform=x64   %~dp0\fra-airtraffic.sln" && ^
iscc "%~dp0..\fra-airtraffic.iss"

if %errorlevel% == 0 goto eof

:error
exit /b %errorlevel%
