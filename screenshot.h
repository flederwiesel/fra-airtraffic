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

#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#include <wx/frame.h>
#include <wx/timer.h>
#include "screenshot.h"

class ScreenshotContainer : public wxFrame
{
public:
	ScreenshotContainer(wxSize);
	~ScreenshotContainer();

	void OnTimer(wxTimerEvent &event);
	void OnClose(wxCloseEvent &event);

private:
	wxTimer *m_MovementTimer;
	long now;

	DECLARE_EVENT_TABLE()
};

#endif // SCREENSHOT_H
