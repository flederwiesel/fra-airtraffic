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

#include "json.h"

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>


#ifndef ElementsIn
#define ElementsIn(a)	(sizeof(a) / sizeof(a)[0])
#endif

#define JSON_ERROR	-1
#define JSON_NAME	-2
#define JSON_VALUE	-3

typedef struct JsonValue
{
	JsonType_t type;
	union
	{
		char *str;
	} value;
} JsonValue_t;

intmax_t ParseJSON(const char *filename,
			  void callback(const char*, const char *, JsonType_t))
{
	FILE *file;
	char line[256];
	intmax_t len;
	intmax_t pos;
	int sp = -1;
	int stack[64];
	char *name;
	char *value;
	JsonType_t type;

	pos = -1;

	if (NULL == callback)
	{
		errno = EINVAL;
	}
	else
	{
		file = fopen(filename, "r");

		if (!file)
		{
			callback(strerror(errno), filename, JSON_NULL);
		}
		else
		{
			pos = 0;
			len = 1;
			name = NULL;
			value = NULL;
			type = JSON_NULL;
			stack[++sp] = JSON_NULL;


			while (len > 0)
			{
				if (name)
				{
					pos = name - line;
					len = sizeof(line) - pos;
					memcpy(line, &line[pos], (size_t)len);
					pos = len;

					if (value)
					{
						len = value - name;
						value = &line[len];
					}

					name = line;
				}

				len = fread(&line[pos], 1, sizeof(line) - (size_t)pos, file);

				if (len < 0)
				{
					callback(strerror(errno), filename, JSON_NULL);
				}
				else if (len > 0)
				{
					len += pos;

					while (pos < len)
					{
						switch (stack[sp])
						{
						case JSON_NAME:
							if ('"' == line[pos])
							{
								line[pos] = '\0';
								stack[sp--] = JSON_NULL;
							}
							else
							{
								if (NULL == name)
									name = &line[pos];
							}
							break;

						case JSON_STRING:
							if ('"' == line[pos])
							{
								line[pos] = '\0';
								stack[sp--] = JSON_NULL;
							}
							else
							{
								if (NULL == value)
									value = &line[pos];
							}
							break;

						default:

							switch (line[pos])
							{
							case '[':
								stack[++sp] = JSON_ARRAY;
								callback(name, value, JSON_ARRAY);
								break;

							case ']':
								assert(sp > -1);

								if (name && value)
									callback(name, value, type);

								name = NULL;
								value = NULL;

								stack[sp--] = JSON_NULL;

								break;

							case '{':
								stack[++sp] = JSON_OBJECT;
								callback(name, value, JSON_OBJECT);
								name = NULL;
								break;

							case '}':

								assert(sp > -1);

								if (name && value)
									callback(name, value, type);

								name = NULL;
								value = NULL;

								stack[sp--] = JSON_NULL;

								break;

							case ',':

								if (name && value)
									callback(name, value, type);

								name = NULL;
								value = NULL;

								break;

							case '"':
								if (NULL == name)
								{
									stack[++sp] = JSON_NAME;
								}
								else
								{
									stack[++sp] = JSON_STRING;
									type = JSON_STRING;
								}
								break;

							case ':':
								assert(name);
								break;

							case ' ':
							case '\n':
							case '\t':
								break;

							default:

								if (name && !value)
									value = &line[pos];

								break;
							}
						}

						pos++;

						assert(sp < (int)ElementsIn(stack));
					}

					pos = 0;
				}
			}

			fclose(file);
		}
	}

	return pos;
}
