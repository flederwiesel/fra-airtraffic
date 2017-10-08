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

#include <float.h>
#include <stdexcept>
#include "stdpaths.h"
#include "fra-airtraffic.h"
#include "container.h"
#include "content.h"
#include "gc.h"

wxBitmap SaverContent::bkgnd;
wxBitmap SaverContent::clock;

int SaverContent::PointSize = 0;

airport_t *SaverContent::fra = NULL;
std::vector<airport_t> SaverContent::airports;
std::vector<flight_t> SaverContent::flights;

long SaverContent::now = -1;

unsigned long SaverContent::arrivals = 0;
unsigned long SaverContent::departures  = 0;

#ifdef _DEBUG
size_t SaverContent::selected = 0;
#endif

unsigned long SaverContent::refs = 0;
SaverContent *SaverContent::savers[16];

BEGIN_EVENT_TABLE(SaverContent, wxWindow)
	EVT_PAINT(SaverContent::OnPaint)
	EVT_SIZE(SaverContent::OnSize)
	EVT_KEY_DOWN(SaverContent::OnKeyDown)
	EVT_MOUSE_EVENTS(SaverContent::OnMouse)
END_EVENT_TABLE()

SaverContent::SaverContent(wxWindow* parent) :
	wxWindow(parent, 0)
{
	size_t num;
	struct tm *local;

	if (-1 == now)
	{
		time((time_t*)&now);
		local = localtime((time_t*)&now);
	}
	else
	{
		local = NULL;
	}

	SetBackgroundStyle(wxBG_STYLE_PAINT);

	if (bkgnd.IsNull())
	{
		wxImage::AddHandler(new wxJPEGHandler);

		// https://visibleearth.nasa.gov/useterms.php
#ifdef _DEBUG
		wxString imagepath(wxGetCwd() + wxT("/img/bluemarble"));
#else
		wxString imagepath(MyStandardPaths::Get().GetResourceDir() +
						   wxT("/flederwiesel/fra-airtraffic/img"));
#endif
		imagepath.append(
			wxString::Format(wxT("/1920x960/bmng/%02d.jpg"),
							 local ? local->tm_mon ? local->tm_mon + 1 : 2 : 2));

		bkgnd.LoadFile(imagepath, wxBITMAP_TYPE_JPEG);
	}

	if (clock.IsNull())
	{
		wxImage::AddHandler(new wxPNGHandler);

#ifdef _DEBUG
		wxString imagepath(wxGetCwd() + wxT("/img/clock.png"));
#else
		wxString imagepath(MyStandardPaths::Get().GetResourceDir() +
						   wxT("/flederwiesel/fra-airtraffic/img/clock.png"));
#endif

		clock.LoadFile(imagepath, wxBITMAP_TYPE_PNG);
	}

	if (0 == airports.size() && 0 == flights.size())
	{
		if (0 == airports.size())
		{
#ifdef _DEBUG
			wxString data(wxGetCwd() + wxT("/json/schedule-debug.json"));
#else
			wxString data(MyStandardPaths::Get().GetDataDir() +
						  wxT("/flederwiesel/fra-airtraffic/schedule.json"));
#endif

			::ParseJSON(data.c_str(), ParseJSON);

			num = airports.size();

			while (num--)
			{
				if (airports[num].icao)
				if (0 == strcmp("EDDF", airports[num].icao))
				{
					fra = &airports[num];
					break;
				}
			}
		}

		if (0 == airports.size() || 0 == flights.size())
		{
			throw std::runtime_error("Error initialising tables.");
		}
		else
		{
			if (local)
			{
				now = local->tm_hour * 3600 + (local->tm_min - local->tm_min % 10) * 60;
			}
			else
			{
				now %= 86400;
				now -= now % 600;
			}
		}
	}

	if (refs < ElementsIn(savers))
		savers[refs++] = this;
}

SaverContent::~SaverContent()
{
	size_t n;

	if (0 == --refs)
	{
		for (n = 0; n < airports.size(); n++)
		{
			if (airports[n].icao)
				free((void*)airports[n].icao);
#ifdef _DEBUG
			if (airports[n].iata)
				free((void*)airports[n].iata);

			if (airports[n].name)
				free((void*)airports[n].name);
#endif
		}

#ifdef _DEBUG
		for (n = 0; n < flights.size(); n++)
		{
			if (flights[n].code)
				free((void*)flights[n].code);
		}
#endif
	}
}

void SaverContent::SetPointSize(unsigned int size)
{
	PointSize = size;
}

void SaverContent::OnSize(wxSizeEvent& event)
{
	int cx, cy;
	int padding;
	wxFontMetrics fm;
	wxWindowDC dc(this);

	dc.GetSize(&cx, &cy);

	// Determine if we're boxed, if so, do not display text
	SaverContainer *parent = dynamic_cast<SaverContainer*>(GetParent());

	if (NULL == parent)
	{
		padding = 0;
	}
	else
	{
		dc.SetFont(wxFont(12,
						  wxFONTFAMILY_DEFAULT,
						  wxFONTSTYLE_NORMAL,
						  wxFONTWEIGHT_NORMAL));

		fm = dc.GetFontMetrics();

		padding = 3 * fm.height;
	}

	// Scale background image to full screen width,
	// unless this would overflow screen height
	if (cx / 2 + 2 * padding > cy)
	{
		rc.y = padding;
		rc.height = cy - rc.y * 2;
		rc.width = rc.height * 2;
		rc.x = (cx - rc.width) / 2;
	}
	else
	{
		rc.width = cx;
		rc.height = cx / 2;
		rc.y = (cy - rc.height) / 2;
		rc.x = 0;
	}

	Refresh(true);
}

void SaverContent::DrawInfoText(wxAutoBufferedPaintDC &dc)
{
	wxMemoryDC mdc;
	wxFontMetrics fm;
	wxString text;
	wxSize extent;
	int ch;
	int x, y;
	int radius;
	int min;

	// Draw area above/below bitmap
	dc.SetPen(*wxTRANSPARENT_PEN);	// No bounding rect
	dc.SetTextForeground(wxColour(255, 255, 255));

	dc.SetFont(wxFont(12,
					  wxFONTFAMILY_DEFAULT,
					  wxFONTSTYLE_NORMAL,
					  wxFONTWEIGHT_NORMAL));

	fm = dc.GetFontMetrics();
	ch = fm.ascent - fm.externalLeading - fm.internalLeading;

	x = rc.x;
	y = rc.y - 2 * fm.height - fm.descent;

	////////////////////////////////////////////////////////////////////
	//// FRA

	text = wxT("FRA");
	extent = dc.GetMultiLineTextExtent(text);
	dc.DrawText(text, x, y);

	x += extent.x + fm.averageWidth / 2;

	//// FRA (└)

	// Make the clock height odd, as for small images the pointer
	// will be rendered better - as middle pixel
	radius = ch + (ch + 1) % 2;

	mdc.SelectObject(clock);
	dc.StretchBlit(x, y + fm.externalLeading + fm.internalLeading - (ch + 1) % 2,
				   radius, radius,
				   &mdc, 0, 0, clock.GetWidth(), clock.GetHeight());
	mdc.SelectObject(wxNullBitmap);

	x += radius + fm.averageWidth / 2;

	//// FRA (└) 00:00

	min = (now % 3600) / 60;
	min -= min % 10;

	text = wxString::Format(wxT("%02lu:%02lu"), now / 3600, min);
	dc.DrawText(text, x, y);

#ifdef _DEBUG
	////////////////////////////////////////////////////////////////////
	//// [0] FLIGHT#

	if (flights[selected].code)
	{
		x = rc.x;

		dc.DrawText(wxString::Format("[%zu] %s",
						selected, flights[selected].code),
					x, y + fm.height);
	}
#endif

	////////////////////////////////////////////////////////////////////
	////                                                              []
	////                                                              []

	x = rc.x + rc.width - ch + ch / 4;
	y = rc.y - 2 * fm.height + ch / 4 + 1;

	dc.SetBrush(wxBrush(wxColour(255, 255, 0)));
	dc.DrawRectangle(x, y, ch / 2, ch / 2);

	dc.SetBrush(wxBrush(wxColour(255, 127, 0)));
	dc.DrawRectangle(x, y + fm.height, ch / 2, ch / 2);

	////                                                     arrivals []
	////                                                   departures []

	x -= ch / 2;
	y = rc.y - 2 * fm.height - fm.descent;

	text = wxString::Format(wxT("%lu"), arrivals);
	extent = dc.GetMultiLineTextExtent(text);
	dc.DrawText(text, x - extent.x, y);

	text = wxString::Format(wxT("%lu"), departures);
	extent = dc.GetMultiLineTextExtent(text);
	dc.DrawText(text, x - extent.x, y + fm.height);

	////////////////////////////////////////////////////////////////////
	//// Credit of earth image

	dc.SetFont(wxFont(8,
					  wxFONTFAMILY_DEFAULT,
					  wxFONTSTYLE_NORMAL,
					  wxFONTWEIGHT_NORMAL));

	dc.DrawText(wxT("Earth image: Credit Reto St\u00f6ckli, ")
				wxT("NASA Earth Observatory http://visibleearth.nasa.gov"),
				rc.x, rc.y + rc.height);
}

void SaverContent::OnPaint(wxPaintEvent &event)
{
	wxAutoBufferedPaintDC dc(this);
	wxMemoryDC mdc;
	airport_t *ap;
	gps_t pos;
	long departure;
	long arrival;
	unsigned long arr, dep;
	double bearing;
	double distance;
	double completed;
	unsigned int i;
	int x, y;
	int cx, cy;
	int w;
	wxString text;
	wxSize extent;


	dc.SetBrush(wxBrush(wxColour(0, 0, 0)));
	dc.GetSize(&cx, &cy);

	if (rc.x)
	{
		// Draw area left/right of bitmap
		dc.DrawRectangle(0, 0, rc.x, cy);
		dc.DrawRectangle(rc.x + rc.width, 0, rc.x, cy);
	}

	if (rc.y)
	{
		// Draw area above/below bitmap
		dc.DrawRectangle(0, 0, cx, rc.y);
		dc.DrawRectangle(0, rc.y + rc.height, cx, rc.y);
	}

	// Draw bitmap
	if (bkgnd.IsNull())
	{
		dc.FloodFill(0, 0, wxColour(0, 127, 0), wxFLOOD_SURFACE);
	}
	else
	{
		mdc.SelectObject(bkgnd);
		dc.StretchBlit(rc.x, rc.y, rc.width, rc.height,
						&mdc, 0, 0, bkgnd.GetWidth(), bkgnd.GetHeight());
		mdc.SelectObject(wxNullBitmap);
	}

	dc.SetBrush(wxBrush(wxColour(0, 127, 127)));
	dc.SetPen(*wxTRANSPARENT_PEN);	// No bounding rect

	if (PointSize)
		w = PointSize;
	else
		w = (rc.width >= 640) ? 3 : (rc.width > 320) ? 2 : 1;

	for (i = 0; i < airports.size(); i++)
	{
		if (!wxIsNaN(airports[i].lat) &&
			!wxIsNaN(airports[i].lon))
		{
			x = (int)(( airports[i].lon + 180.0) / 360.0 * (double)rc.width) + rc.x;
			y = (int)((-airports[i].lat +  90.0) / 180.0 * (double)rc.height) + rc.y;

			dc.DrawRectangle(x - w / 2, y - w / 2, w, w);
		}
	}

	arr = 0;
	dep = 0;

	for (i = 0; i < flights.size(); i++)
	{
		ap = &airports[flights[i].airport];

		if (!wxIsNaN(ap->lat) &&
			!wxIsNaN(ap->lon))
		{
			distance = GCDistance(fra->lat, fra->lon, ap->lat, ap->lon);
			distance *= EARTH_RADIUS;

			// openflights.org calculates flight duration as
			// "30 min plus 1 hour per every 500 miles".
			// If it fits their purpose, it is good enough for us...
			if (flights[i].duration < 0)
				flights[i].duration = -1800 - (long)(distance / 5.0 * 36.0);
			else
				flights[i].duration = 1800 + (long)(distance / 5.0 * 36.0);

			// Is this flight airborne?
			if (flights[i].duration > 0)
			{
				departure = flights[i].scheduled;
				arrival   = flights[i].scheduled + flights[i].duration;

				if (arrival - now > 86400)
					arrival -= 86400;

				if (departure > arrival)
					departure -= 86400;
			}
			else
			{
				arrival   = flights[i].scheduled;
				departure = flights[i].scheduled + flights[i].duration;

				if (now - departure > 86400)
					departure += 86400;

				if (arrival < departure)
					arrival += 86400;
			}

			if (now >= departure && now <= arrival)
			{
				// yes
				if (flights[i].duration < 0)
					arr++;
				else
					dep++;

				completed = distance * (double)(now - departure) /
					(double)(flights[i].duration < 0 ?
						-flights[i].duration : flights[i].duration);

				completed /= EARTH_RADIUS;

				if (flights[i].duration > 0)
				{
					bearing = GCBearing(fra->lat, fra->lon,
										ap->lat, ap->lon);

					pos = GCTravel(fra->lat, fra->lon, completed, bearing);
				}
				else
				{
					bearing = GCBearing(ap->lat, ap->lon,
										fra->lat, fra->lon);

					pos = GCTravel(ap->lat, ap->lon, completed, bearing);
				}

				// As from FRA no flights will wrap around, this is sufficient
				x = (int)(( pos.lon + 180.0) / 360.0 * (double)rc.width) + rc.x;
				y = (int)((-pos.lat +  90.0) / 180.0 * (double)rc.height) + rc.y;

#ifdef _DEBUG
				if (selected == i)
				{
					dc.SetPen(wxPen(wxColour(255, 0, 0), 1, wxPENSTYLE_LONG_DASH));
					dc.SetBrush(*wxTRANSPARENT_BRUSH);
					dc.DrawCircle(x, y, 12);
					dc.DrawLine(x + 3, y, x + 100, y);
					dc.DrawLine(x - 3, y, x - 100, y);
					dc.DrawLine(x, y + 3, x, y + 100);
					dc.DrawLine(x, y - 3, x, y - 100);
					dc.SetPen(*wxTRANSPARENT_PEN);
				}
#endif

				if (flights[i].duration < 0)
					dc.SetBrush(wxBrush(wxColour(255, 255, 0)));
				else
					dc.SetBrush(wxBrush(wxColour(255, 127, 0)));

				dc.DrawRectangle(x - w / 2, y - w / 2, w, w);
			}
		}
	}

	if (0 == arrivals || 0 == now % 600)
		arrivals = arr;

	if (0 == departures || 0 == now % 600)
		departures = dep;

	// Determine if we're boxed, if so, do not display text
	SaverContainer *parent = dynamic_cast<SaverContainer*>(GetParent());

	if (parent)
		if (wxIS_KIND_OF(parent, SaverContainer))
			DrawInfoText(dc);
}

// Mouse events will be deliviered to the widget the cursor is hovering above
// Keyboard events, however will be delivered to the SaverContainer widget, which
// has the keyboard focus
void SaverContent::OnMouse(wxMouseEvent &event)
{
#ifndef _DEBUG
	event.ResumePropagation(1);
	event.Skip ();
#endif
}

void SaverContent::OnKeyDown(wxKeyEvent &event)
{
#ifdef _DEBUG
	unsigned long i;

	switch (event.GetKeyCode())
	{
	case WXK_UP:

		selected++;
		selected %= flights.size();

		for (i = 0; i < refs; i++)
			savers[i]->Refresh(true);

		break;

	case WXK_DOWN:

		if (0 == selected--)
			selected = flights.size() - 1;

		for (i = 0; i < refs; i++)
			savers[i]->Refresh(true);

		break;
	default:
#endif
		event.ResumePropagation(1);
		event.Skip ();
#ifdef _DEBUG
	}
#endif
}
#define append push_back

void SaverContent::ParseJSON(const char *name, const char *value, JsonType_t type)
{
	size_t n;
	static airport_t a;
	static flight_t f;
	static enum
	{
		OBJTYPE_UNKNOWN,
		OBJTYPE_AIRPORT,
		OBJTYPE_FLIGHT,
	} objtype = OBJTYPE_UNKNOWN;

	if (JSON_NULL == type)
	{
		// error
		throw(std::runtime_error((const char*)(wxString(name) + wxT(": ") + wxString(value))));
	}
	else if (JSON_ARRAY == type)
	{
		if (name)
		{
			if (0 == strcmp("airports", name))
				objtype = OBJTYPE_AIRPORT;

			if (0 == strcmp("flights", name))
				objtype = OBJTYPE_FLIGHT;
		}
	}
	else if (JSON_OBJECT == type)
	{
		if (OBJTYPE_AIRPORT == objtype)
		{
			a.icao = NULL;
#ifdef _DEBUG
			a.iata = NULL;
			a.name = NULL;
#endif
			a.lat = strtod("NAN", NULL);
			a.lon = strtod("NAN", NULL);

			airports.append(a);
		}

		if (OBJTYPE_FLIGHT == objtype)
		{
#ifdef _DEBUG
			f.code = NULL;
#endif
			f.airport = 0;
			f.scheduled = 0;
			f.duration = 0;

			flights.append(f);
		}
	}
	else
	{
		if (OBJTYPE_AIRPORT == objtype)
		{
			n = airports.size();

			if (n-- > 0)
			{
				if (0 == strcmp("lat", name))
				{
					sscanf(value, "%lf", &airports[n].lat);
				}
				else if (0 == strcmp("lon", name))
				{
					sscanf(value, "%lf", &airports[n].lon);
				}
				else if (0 == strcmp("icao", name))
				{
					airports[n].icao = strdup(value);
				}
#ifdef _DEBUG
				else if (0 == strcmp("iata", name))
				{
					airports[n].iata = strdup(value);
				}
				else if (0 == strcmp("name", name))
				{
					airports[n].name = strdup(value);
				}
#endif
			}
		}

		if (OBJTYPE_FLIGHT == objtype)
		{
			n = flights.size();

			if (n--)
			{
				if (0 == strcmp("airport", name))
				{
					sscanf(value, "%lu", &flights[n].airport);
				}
				else if (0 == strcmp("scheduled", name))
				{
					sscanf(value, "%ld", &flights[n].scheduled);
				}
				else if (0 == strcmp("duration", name))
				{
					sscanf(value, "%ld", &flights[n].duration);
				}
#ifdef _DEBUG
				else if (0 == strcmp("flight", name))
				{
					flights[n].code = strdup(value);
				}
#endif
			}
		}
	}
}

#undef append

void SaverContent::GetTime(long &time)
{
	time = now;
}

void SaverContent::SetTime(long time)
{
	now = time % 86400;

	for (unsigned long i = 0; i < refs; i++)
		savers[i]->Refresh(0 == now % 600);
}
