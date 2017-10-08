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

#ifndef JSON_H
#define JSON_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

typedef enum
{
	JSON_NULL,
	JSON_STRING,
	JSON_BOOL,
	JSON_INT,
	JSON_DOUBLE,
	JSON_OBJECT,
	JSON_ARRAY,
} JsonType_t;

intmax_t ParseJSON(const char *filename,
			  void callback(const char*, const char *, JsonType_t));

#ifdef __cplusplus
} // extern "C"
#endif

#endif	/* JSON_H */
