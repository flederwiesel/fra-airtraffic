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

#ifndef FRA_AIRTRAFFIC_H
#define FRA_AIRTRAFFIC_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>

#ifndef ElementsIn
#define ElementsIn(x)	(sizeof(x) / sizeof(x)[0])
#endif

typedef struct
{
	const char *icao;
#ifdef _DEBUG
	const char *iata;
	const char *name;
#endif
	double lat;
	double lon;
} airport_t;

typedef struct
{
#ifdef _DEBUG
	const char *code;
#endif
	unsigned long airport;
	long scheduled;
	long duration;
} flight_t;

#ifdef __cplusplus
} // extern "C"
#endif

#endif	/* FRA_AIRTRAFFIC_H */
