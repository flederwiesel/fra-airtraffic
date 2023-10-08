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

#include <wx/window.h>
#include "fra-airtraffic.h"
#include "preview.h"

#define TIMER_PERIOD	10	/*[ms]*/

BEGIN_EVENT_TABLE(SaverPreview, wxWindow)
	EVT_CLOSE(SaverPreview::OnClose)
END_EVENT_TABLE()

SaverPreview::SaverPreview(wxNativeContainerWindowHandle hwndParent) :
	wxNativeContainerWindow(hwndParent)
{
	content = new SaverContent(this);

	if (content)
	{
		content->SetSize(GetSize());

		m_MovementTimer = new wxTimer(this, wxNewId());

		if (m_MovementTimer)
		{
			m_MovementTimer->Start(TIMER_PERIOD);
			Connect(wxEVT_TIMER, wxTimerEventHandler(SaverPreview::OnTimer));
		}

		content->GetTime(now);
	}
}

SaverPreview::~SaverPreview()
{
	wxDELETE(m_MovementTimer);
}

void SaverPreview::OnClose(wxCloseEvent &event)
{
	Destroy();
}

void SaverPreview::OnTimer(wxTimerEvent &event)
{
	now += 60;
	now %= 86400;

	SaverContent::SetTime(now);
}
