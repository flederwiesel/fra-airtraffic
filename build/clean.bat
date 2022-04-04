del /F /Q *.aps *.sdf
del /F /A:H /Q *.suo
rd /S /Q "%~dp0.vs" "%~dp0ipch" "%~dp0redist" "%~dp0Win32" "%~dp0x64"
