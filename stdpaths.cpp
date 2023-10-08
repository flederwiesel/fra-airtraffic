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

#include "stdpaths.h"
#include <wx/msgdlg.h>

MyStandardPaths MyStandardPaths::my_gs_stdPaths;

MyStandardPaths::MyStandardPaths() { }

MyStandardPaths& MyStandardPaths::Get()
{
	return my_gs_stdPaths;
}

// %ProgramData%
// /var/cache
wxString MyStandardPaths::GetDataDir() const
{
#ifdef __WINDOWS__
	return wxStandardPaths::DoGetDirectory(CSIDL_COMMON_APPDATA);
#else
	return AppendAppInfo(wxT("/var/cache"));
#endif
}

// %ProgramData%
// (%ProgramFiles%)
// {reg:HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\saver_is1|InstallLocation}
// /usr/local/share
wxString MyStandardPaths::GetResourceDir() const
{
#ifdef __WINDOWS__
	return wxStandardPaths::DoGetDirectory(CSIDL_PROGRAM_FILES);
#else
	return wxStandardPaths::GetDataDir();
#endif
}
