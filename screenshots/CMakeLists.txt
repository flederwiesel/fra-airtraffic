# Executable rendering one cycle into screenshot files
# to be combined into animated GIF for the README
set(CMAKE_EXECUTABLE_SUFFIX .exe)

add_target(
	screenshots
		COMMAND "$(Windir)\\System32\\rundll32.exe"
		ARGUMENTS "shell32.dll,Control_RunDLL desk.cpl,fra-airtraffic,@ScreenSaver"
)
