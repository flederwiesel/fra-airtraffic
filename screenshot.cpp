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

#include <wx/app.h>
#include <wx/window.h>
#include <wx/filename.h>
#include "content.h"
#include "screenshot.h"

BEGIN_EVENT_TABLE(ScreenshotContainer, wxWindow)
	EVT_TIMER(-1, ScreenshotContainer::OnTimer)
	EVT_CLOSE(ScreenshotContainer::OnClose)
END_EVENT_TABLE()

ScreenshotContainer::ScreenshotContainer(wxSize size) :
	wxFrame(NULL, 0, "fra-airtraffic"),
	now(0)
{
#if defined(__WXMSW__)
	SetIcon(wxIcon("ApplicationIcon"));
#endif

	SaverContent::SetTime(now);
	SaverContent::SetPointSize(2);

	SaverContent *content = new SaverContent(this);

	if (content)
		content->SetSize(size);

	size += GetSize();
	size -= GetClientSize();

	SetSize(size);
	Show();

	m_MovementTimer = new wxTimer(this, wxNewId());

	if (m_MovementTimer)
		m_MovementTimer->Start();
}

ScreenshotContainer::~ScreenshotContainer()
{
	// SaverContent *content will be deleted by wx
}

void ScreenshotContainer::OnClose(wxCloseEvent &event)
{
	wxDELETE(m_MovementTimer);
	Destroy();
}


void ScreenshotContainer::OnTimer(wxTimerEvent &event)
{
	SaverContent::SetTime(now);

	wxClientDC dc(this);

	//Get the size of the screen/DC
	wxPoint origin;
	wxCoord cx, cy;

	GetClientSize(&cx, &cy);

	origin = GetClientAreaOrigin();

	//Create a Bitmap that will later on hold the screenshot image
	//Note that the Bitmap must have a size big enough to hold the screenshot
	//-1 means using the current default colour depth
	wxBitmap screenshot(cx, cy, -1);

	//Create a memory DC that will be used for actually taking the screenshot
	wxMemoryDC memDC;

	//Tell the memory DC to use our Bitmap
	//all drawing action on the memory DC will go to the Bitmap now
	memDC.SelectObject(screenshot);

	//Blit (in this case copy) the actual screen on the memory DC
	//and thus the Bitmap
	memDC.Blit( 0, //Copy to this X coordinate
				0, //Copy to this Y coordinate
				cx, //Copy this width
				cy, //Copy this height
				&dc, //From where do we copy?
				origin.x, //What's the X offset in the original DC?
				origin.y  //What's the Y offset in the original DC?
	);

	//Select the Bitmap out of the memory DC by selecting a new
	//uninitialized Bitmap
	memDC.SelectObject(wxNullBitmap);

	//Our Bitmap now has the screenshot, so let's save it :-)
	if (!wxFileName::Mkdir(wxGetCwd() + wxT("/screenshots"),
						   wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL))
	{
		Destroy();
	}
	else
	{
		int h = now / 3600;
		int m = (now % 3600) / 60;

		if (!screenshot.SaveFile(wxGetCwd() +
				wxString().Format(wxT("/screenshots/%02d%02d.png"), h, m),
				wxBITMAP_TYPE_PNG))
		{
			Destroy();
		}
		else
		{
			now += 1800;
			now %= 86400;

			if (0 == now)
				Close();
			else
				SaverContent::SetTime(now);
		}
	}
}
