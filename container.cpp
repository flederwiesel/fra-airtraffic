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

#include "container.h"
#include "fra-airtraffic.h"

#include <wx/app.h>
#include <wx/display.h>
#include <wx/window.h>


#define TIMER_PERIOD	25	/*[ms]*/

#if defined(__WXGTK__)
	#define hideCursor() wxSetCursor(wxCURSOR_BLANK)
	#define showCursor() wxSetCursor(*wxSTANDARD_CURSOR)
#elif defined(__WXMSW__)
	#define hideCursor() ShowCursor(0)
	#define showCursor() ShowCursor(1)
#endif

BEGIN_EVENT_TABLE(SaverContainer, wxTopLevelWindow)
	EVT_TIMER(-1, SaverContainer::OnTimer)
	EVT_MOUSE_EVENTS(SaverContainer::OnMouse)
	EVT_KEY_DOWN(SaverContainer::OnKeyDown)
	EVT_CLOSE(SaverContainer::OnClose)
END_EVENT_TABLE()

SaverContainer::SaverContainer() :
	wxTopLevelWindow(NULL, 0, wxT("fra-airtraffic"))
{
#if defined(__WXMSW__)
	SetIcon(wxIcon(wxT("ApplicationIcon")));
#endif

	int xmin, xmax;
	int ymin, ymax;
	unsigned int m;
	unsigned int monitors;
	SaverContent *content;
	wxRect rect;

	xmin = xmax =
	ymin = ymax = 0;

	monitors = wxDisplay::GetCount();

	for (m = 0; m < monitors; m++)
	{
		rect = wxDisplay(m).GetGeometry();

		if (xmin > rect.x)
			xmin = rect.x;

		if (xmax < rect.x + rect.width)
			xmax = rect.x + rect.width;

		if (ymin > rect.y)
			ymin = rect.y;

		if (ymax < rect.y + rect.height)
			ymax = rect.y + rect.height;
	}

	// Eliminate flicker upon application start
	SetBackgroundStyle(wxBG_STYLE_PAINT);

	// This operates on one display only
	ShowFullScreen(true);
	// Expand to virtual screens
	SetSize(xmin, ymin, xmax - xmin, ymax - ymin);

	for (m = 0; m < monitors; m++)
	{
		rect = wxDisplay(m).GetGeometry();

		content = new SaverContent(this);

		// As xmin/ymin may have a negative value, we need to
		// respect this when positioning the client windows
		if (content)
			content->SetSize(rect.x - xmin, rect.y - ymin, rect.width, rect.height);
	}

	SaverContent::GetTime(now);

	m_MovementTimer = new wxTimer(this, wxNewId());

	if (m_MovementTimer)
		m_MovementTimer->Start(TIMER_PERIOD);

#ifndef _DEBUG
	hideCursor();
#endif
}

SaverContainer::~SaverContainer()
{
	// SaverContent *content[] will be deleted by wx
}

void SaverContainer::OnClose(wxCloseEvent &event)
{
	wxDELETE(m_MovementTimer);
	showCursor();
	Destroy();
}

void SaverContainer::OnTimer(wxTimerEvent &event)
{
	now += 150;
	now %= 86400;

	SaverContent::SetTime(now);
}

void SaverContainer::OnMouse(wxMouseEvent &event)
{
	static bool quit = false;

	if (!(event.Entering() ||
		  event.Leaving()))
	{
		if (!quit)
		{
			if (event.Moving())
				quit = true;
		}
		else
		{
			Close();
		}
	}
}

// SaverContainer is the widget having the keyboard focus, so keyboard events
// will be reported here, in contrast to mouse events, which will be delivered
// to the widget under the cursor
void SaverContainer::OnKeyDown(wxKeyEvent &event)
{
#ifdef _DEBUG
	switch (event.GetKeyCode())
	{
	case WXK_LEFT:

		if (now % 300)
			now -= now % 300;

		if (now < 300)
			now = 86400;
		else
			now -= 300 * (event.ShiftDown() ? 10 : 1);

		SaverContent::SetTime(now);
		m_MovementTimer->Stop();

		break;

	case WXK_RIGHT:

		if (now % 300)
			now -= now % 300;

		now += 300 * (event.ShiftDown() ? 10 : 1);
		now %= 86400;

		SaverContent::SetTime(now);
		m_MovementTimer->Stop();

		break;

	case WXK_ESCAPE:
		Close();
		break;

	default:
		;
	}
#else
	Close();
#endif
}
