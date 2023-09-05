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
#include "gc.h"

double /*[rad]*/ GCBearing(double lat1, double lon1, double lat2, double lon2)
{
	double dl;	// delta(lon2, lon1)

	// This is basically the first equation from
	// https://en.wikipedia.org/wiki/Great-circle_navigation#Course_and_distance
	// with vairables interchanged and using atan2 instead if tan
	dl = rad(lon2 - lon1);
	lat2 = rad(lat2);
	lat1 = rad(lat1);

	return atan2(sin(dl), cos(lat1) * tan(lat2) - sin(lat1) * cos(dl));
}

double /*[rad]*/ GCDistance(double lat1, double lon1, double lat2, double lon2)
{
	double a;	// Segment BC
	double b;	// Segment AC
	double c;	// Segment AB
	double C;	// Angle ACB

	// Third equation from
	// https://en.wikipedia.org/wiki/Great-circle_navigation#Course_and_distance
	// is in fact the speric cosine rule over a triangle with given
	// angle between two sides
	C = rad(lon2 - lon1);
	a = rad(lat1);
	b = rad(lat2);
	c = acos(sin(a) * sin(b) + cos(a) * cos(b) * cos(C));

	if (c < 0)
		c += M_PI;

	return c;
}

gps_t GCTravel(double lat1, double lon1, double distance /*[rad]*/, double bearing)
{
	gps_t pos;
	double a;	// Segment BC
	double b;	// Segment AC
	double q;
	double C;	// Angle ACB

	if (distance < DBL_EPSILON)
	{
		pos.lat = lat1;
		pos.lon = lon1;
	}
	else
	{
		if (0.0 == bearing || 180.0 == bearing)
		{
			pos.lon = lon1;

			if (0.0 == bearing)
				pos.lat = lat1 + deg(distance);
			else
				pos.lat = lat1 - deg(distance);
		}
		else if (90.0 == bearing || -90.0 == bearing)
		{
			pos.lat = lat1;

			if (90.0 == bearing)
				pos.lon = lon1 + deg(distance);
			else
				pos.lon = lon1 - deg(distance);
		}
		else
		{
			b = rad(90.0 - lat1);
			a = acos(cos(b) * cos(distance) + sin(b) * sin(distance) * cos(bearing));
			q = sin(a) * sin(b);

			if (0.0 == q)
				C = 0.0;
			else
				C = acos((cos(distance) - cos(a) * cos(b)) / q);

			pos.lat = 90.0 - deg(a);
			pos.lon = bearing < 0.0 ? lon1 - deg(C) : lon1 + deg(C);
		}
	}

	return pos;
}
