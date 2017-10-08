/*
 * This file is part of fra-airtraffic screensaver,
 * https://github.com/flederwiesel/fra-airtraffic
 * Copyright © 2017 Tobias Kühne
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef STDPATHS_H
#define STDPATHS_H

#include <wx/stdpaths.h>

// https://msdn.microsoft.com/en-us/library/bb762494%28v=vs.85%29.aspx
// https://msdn.microsoft.com/en-us/library/dd378457%28v=vs.85%29.aspx
// http://docs.wxwidgets.org/3.0/classwx_standard_paths.html#a00cb1fa092b0a164c3d1b5813b836a93a555b4185a68ec29bce358ac2718e78bc
#define CSIDL_DESKTOP 0x0000
#define CSIDL_INTERNET 0x0001
#define CSIDL_PROGRAMS 0x0002
#define CSIDL_CONTROLS 0x0003
#define CSIDL_PRINTERS 0x0004
#define CSIDL_PERSONAL 0x0005
#define CSIDL_FAVORITES 0x0006
#define CSIDL_STARTUP 0x0007
#define CSIDL_RECENT 0x0008
#define CSIDL_SENDTO 0x0009
#define CSIDL_BITBUCKET 0x000a
#define CSIDL_STARTMENU 0x000b
#define CSIDL_MYDOCUMENTS CSIDL_PERSONAL
#define CSIDL_MYMUSIC 0x000d
#define CSIDL_MYVIDEO 0x000e
#define CSIDL_DESKTOPDIRECTORY 0x0010
#define CSIDL_DRIVES 0x0011
#define CSIDL_NETWORK 0x0012
#define CSIDL_NETHOOD 0x0013
#define CSIDL_FONTS 0x0014
#define CSIDL_TEMPLATES 0x0015
#define CSIDL_COMMON_STARTMENU 0x0016
#define CSIDL_COMMON_PROGRAMS 0x0017
#define CSIDL_COMMON_STARTUP 0x0018
#define CSIDL_COMMON_DESKTOPDIRECTORY 0x0019
#define CSIDL_APPDATA 0x001a
#define CSIDL_PRINTHOOD 0x001b

#ifndef CSIDL_LOCAL_APPDATA
#define CSIDL_LOCAL_APPDATA 0x001c
#endif

#define CSIDL_ALTSTARTUP 0x001d
#define CSIDL_COMMON_ALTSTARTUP 0x001e
#define CSIDL_COMMON_FAVORITES 0x001f

#ifndef _SHFOLDER_H_
#define CSIDL_INTERNET_CACHE 0x0020
#define CSIDL_COOKIES 0x0021
#define CSIDL_HISTORY 0x0022
#define CSIDL_COMMON_APPDATA 0x0023
#define CSIDL_WINDOWS 0x0024
#define CSIDL_SYSTEM 0x0025
#define CSIDL_PROGRAM_FILES 0x0026
#define CSIDL_MYPICTURES 0x0027
#endif

#define CSIDL_PROFILE 0x0028
#define CSIDL_SYSTEMX86 0x0029
#define CSIDL_PROGRAM_FILESX86 0x002a

#ifndef _SHFOLDER_H_
#define CSIDL_PROGRAM_FILES_COMMON 0x002b
#endif

#define CSIDL_PROGRAM_FILES_COMMONX86 0x002c
#define CSIDL_COMMON_TEMPLATES 0x002d

#ifndef _SHFOLDER_H_
#define CSIDL_COMMON_DOCUMENTS 0x002e
#define CSIDL_COMMON_ADMINTOOLS 0x002f
#define CSIDL_ADMINTOOLS 0x0030
#endif

#define CSIDL_CONNECTIONS 0x0031
#define CSIDL_COMMON_MUSIC 0x0035
#define CSIDL_COMMON_PICTURES 0x0036
#define CSIDL_COMMON_VIDEO 0x0037
#define CSIDL_RESOURCES 0x0038

#ifndef _SHFOLDER_H_
#define CSIDL_RESOURCES_LOCALIZED 0x0039
#endif

#define CSIDL_COMMON_OEM_LINKS 0x003a
#define CSIDL_CDBURN_AREA 0x003b

#define CSIDL_COMPUTERSNEARME 0x003d

class MyStandardPaths : public wxStandardPaths
{
public:
	MyStandardPaths();

	static MyStandardPaths& Get();

	wxString GetDataDir() const;
	wxString GetResourceDir() const;

private:

	static MyStandardPaths my_gs_stdPaths;
};

#endif	// STDPATHS_H
