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

#ifndef CONTENT_H
#define CONTENT_H

#include <vector>
#include <wx/window.h>
#include <wx/dcbuffer.h>
#include "fra-airtraffic.h"
#include "json.h"

class SaverContent : public wxWindow
{
public:

	SaverContent(wxWindow *parent);
	~SaverContent();

	static void SetPointSize(unsigned int);

	static void GetTime(long &);
	static void SetTime(long);

	void OnPaint(wxPaintEvent &event);
	void OnEraseBackground(wxEraseEvent &event);
	void OnSize(wxSizeEvent &event);
	void OnKeyDown(wxKeyEvent &event);
	void OnMouse(wxMouseEvent &event);

private:

	void DrawInfoText(wxAutoBufferedPaintDC &dc);

	static void ParseSchedule(const char *name, const char *value, JsonType_t type);

private:

	wxRect rc;

	static wxBitmap bkgnd;
	static wxBitmap clock;

	static int PointSize;

	static airport_t *fra;
	static std::vector<airport_t> airports;
	static std::vector<flight_t> flights;

	static long now;

	static unsigned long arrivals;
	static unsigned long departures;

#ifdef _DEBUG
	static size_t selected;
#endif

	static unsigned long refs;
	static SaverContent *savers[16];

	DECLARE_EVENT_TABLE()
};

#endif	// CONTENT_H
