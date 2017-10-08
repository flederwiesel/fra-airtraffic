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

#ifndef CONTAINER_H
#define CONTAINER_H

#include <wx/frame.h>
#include <wx/timer.h>
#include "content.h"

class SaverContainer : public wxTopLevelWindow
{
public:
	SaverContainer();
	~SaverContainer();

	void OnTimer(wxTimerEvent &event);
	void OnClose(wxCloseEvent &event);
	void OnKeyDown(wxKeyEvent &event);
	void OnMouse(wxMouseEvent &event);

protected:
	wxTimer *m_MovementTimer;
	long now;

private:
	DECLARE_EVENT_TABLE()
};

#endif // CONTAINER_H
