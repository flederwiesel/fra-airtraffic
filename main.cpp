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

#include <stdlib.h>
#include <wx/app.h>
#include <wx/wx.h>
#include <wx/cmdline.h>
#include "stdpaths.h"
#include "container.h"
#include "screenshot.h"
#include "preview.h"

#ifdef _MSC_VER
#define putenv	_putenv
#define tzset	_tzset
#endif

// Define the MainApp
class MainApp : public wxApp
{
public:
	MainApp() { }
	virtual ~MainApp() { }

	virtual bool OnInit();
};

DECLARE_APP(MainApp)
IMPLEMENT_APP(MainApp)

bool MainApp::OnInit()
{
	wxCmdLineParser parser(argc, argv);
	wxWindow *saver = NULL;

	// We're going to display FRA local time...
	// http://man7.org/linux/man-pages/man3/tzset.3.html
	if (::putenv("TZ=CET-01:00:00CEST-02:00:00,M3.5.0/2,M10.5.0/3") == 0)
		tzset();

	MyStandardPaths::Get().UseAppInfo(wxStandardPathsBase::AppInfo_None);

	// https://support.microsoft.com/en-us/kb/182383
	parser.AddSwitch(wxT("S"), wxEmptyString,
		wxT("Run the Screen Saver"),
		wxCMD_LINE_PARAM_OPTIONAL);

	parser.AddSwitch(wxT("s"), wxEmptyString,
		wxT("Run the Screen Saver"),
		wxCMD_LINE_PARAM_OPTIONAL);

	parser.AddOption(wxT("c"), wxEmptyString,
		wxT("Show the Settings dialog box, modal to the foreground window <HWND>."),
		wxCMD_LINE_VAL_NUMBER,
		wxCMD_LINE_PARAM_OPTIONAL);

	parser.AddOption(wxT("p"), wxT("preview"),
		wxT("Preview Screen Saver as child of window <HWND>"),
		wxCMD_LINE_VAL_NUMBER,
		wxCMD_LINE_PARAM_OPTIONAL);

	parser.AddLongSwitch(wxT("screenshots"), wxEmptyString,
		wxCMD_LINE_PARAM_OPTIONAL);

	try
	{
		if (parser.Parse(false) == 0)
		{
			wxNativeContainerWindowHandle parentHWND(NULL);

			if (parser.Found(wxT("c"), (long*)&parentHWND))
			{
				wxMessageBox(wxT("No settings for this screensaver.\n")
							 wxT("For more information visit http://wxwidgets.info"),
							 wxString().Format(wxT("%p"), parentHWND));
			}
			else if (parser.Found(wxT("p"), (long*)&parentHWND))
			{
				if (parentHWND != 0)
					saver = new SaverPreview(parentHWND);
			}
			else if (parser.Found(wxT("S")) || parser.Found(wxT("s")))
			{
				saver = new SaverContainer();
			}
			else if (parser.Found(wxT("screenshots")))
			{
				saver = new ScreenshotContainer(wxSize(480, 280));
			}
		}
	}
	catch (...)
	{
	}

	return saver ? true : false;
}
