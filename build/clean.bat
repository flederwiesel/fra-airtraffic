del /F /Q *.aps *.sdf
del /F /A:H /Q *.suo
	for %%d in ( ^
		"%~dp0..\wxWidgets\lib\vc_dll" ^
		"%~dp0..\wxWidgets\lib\vc_x64_dll" ^
		"%~dp0..\wxWidgets\build\msw\vc_mswudll" ^
		"%~dp0..\wxWidgets\build\msw\vc_x64_mswudll" ^
		"%~dp0.vs" ^
		"%~dp0ipch" ^
		"%~dp0Win32" ^
		"%~dp0x64" ^
		"%~dp0redist") do (
		if exist %%d rmdir /s /q %%d
	)
