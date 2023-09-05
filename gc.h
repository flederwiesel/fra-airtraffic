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

#ifndef GC_H
#define GC_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <wx/math.h>

#define deg(d)	((d) * 360.0 / 2.0 / M_PI)
#define rad(d)	((d) * 2.0 * M_PI / 360.0)

#define EARTH_RADIUS	6371.000785	/* [km] as of GRS-80 */

typedef struct
{
	double lat;	/*[deg]*/
	double lon;	/*[deg]*/
} gps_t;

double /*[rad]*/ GCDistance(double lat1 /*[deg]*/, double lon1 /*[deg]*/,
							double lat2 /*[deg]*/, double lon2 /*[deg]*/);

double /*[rad]*/ GCBearing(double lat1 /*[deg]*/, double lon1 /*[deg]*/,
						   double lat2 /*[deg]*/, double lon2 /*[deg]*/);

gps_t GCTravel(double lat1 /*[deg]*/, double lon1 /*[deg]*/,
			   double distance /*[rad]*/, double bearing /*[rad]*/);

#ifdef __cplusplus
}	// extern "C"
#endif

#endif	/* GC_H */
