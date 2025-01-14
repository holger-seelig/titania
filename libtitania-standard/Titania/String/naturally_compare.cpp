/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstraße 31a, Leipzig, Germany 2011.
 *
 * All rights reserved. Holger Seelig <holger.seelig@yahoo.de>.
 *
 * THIS IS UNPUBLISHED SOURCE CODE OF create3000.
 *
 * The copyright notice above does not evidence any actual of intended
 * publication of such source code, and is an unpublished work by create3000.
 * This material contains CONFIDENTIAL INFORMATION that is the property of
 * create3000.
 *
 * No permission is granted to copy, distribute, or create derivative works from
 * the contents of this software, in whole or in part, without the prior written
 * permission of create3000.
 *
 * NON-MILITARY USE ONLY
 *
 * All create3000 software are effectively free software with a non-military use
 * restriction. It is free. Well commented source is provided. You may reuse the
 * source in any way you please with the exception anything that uses it must be
 * marked to indicate is contains 'non-military use only' components.
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright 1999, 2016 Holger Seelig <holger.seelig@yahoo.de>.
 *
 * This file is part of the Titania Project.
 *
 * Titania is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License version 3 only, as published by the
 * Free Software Foundation.
 *
 * Titania is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU General Public License version 3 for more
 * details (a copy is included in the LICENSE file that accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version 3
 * along with Titania.  If not, see <http://www.gnu.org/licenses/gpl.html> for a
 * copy of the GPLv3 License.
 *
 * For Silvio, Joy and Adi.
 *
 ******************************************************************************/
#include "naturally_compare.h"

#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>     /* mbstowcs() */
#include <string.h>     /* strcmp() */
#include <wchar.h>      /* wcstoul() */
#include <wctype.h>     /* iswdigit(), iswspace(), iswpunct() */

namespace titania {
namespace basic {

extern "C"
{

/**
 * natsort - Copyright (C) 2013 Olivier Brunel
 *
 * sort.c
 * Copyright (C) 2013 Olivier Brunel <i.am.jack.mail@gmail.com>
 *
 * This file is part of natsort.
 *
 * natsort is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * natsort is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * natsort. If not, see http://www.gnu.org/licenses/
 */

// http://jjacky.com/natsort/

typedef enum
{
	DONNA_SORT_NATURAL_ORDER    = (1 << 0),
	DONNA_SORT_CASE_INSENSITIVE = (1 << 1),
	DONNA_SORT_DOT_FIRST        = (1 << 2),
	DONNA_SORT_DOT_MIXED        = (1 << 3),
	DONNA_SORT_IGNORE_SPUNCT    = (1 << 4)
} DonnaSortOptions;

static wchar_t*
_mbstowcs (wchar_t* dest, const char* src)
{
	size_t len;

	len = mbstowcs (dest, src, 255);

	if (len == (size_t) -1)
		/* invalid mb sequence */
		return NULL;
	else if (len >= 255)
	{
		/* we need more than 255 wide chars */
		len  = mbstowcs (NULL, src, 0);
		dest = (wchar_t*) malloc (sizeof (wchar_t) * (len + 1));
		len  = mbstowcs (dest, src, len);

		if (len == (size_t) - 1)
		{
			/* invalid mb seq */
			free (dest);
			return NULL;
		}
	}

	return dest;
}

int
strcmp_ext (const char* s1, const char* s2, int options)
{
	wchar_t  wb1 [256], * _ws1 = wb1;
	wchar_t  wb2 [256], * _ws2 = wb2;
	wchar_t* ws1, * ws2;
	bool     is_string = true;
	int      res_fb    = 0; /* fallback */
	int      res_cs    = 0; /* case-sensitive */
	int      res       = 0;

	/* if at least one string if NULL or empty, we have a result */
	if (! s1 or * s1 == '\0')
	{
		if (s2 && *s2 not_eq '\0')
			return -1;
		else
			return 0;
	}
	else if (! s2 or * s2 == '\0')
		return 1;

	if (options & DONNA_SORT_DOT_FIRST)
	{
		if (*s1 == '.')
		{
			if (*s2 not_eq '.')
				/* only s1 is dotted, it comes first */
				return -1;
			else
			{
				/* both are dotted, skip the dot */
				++ s1;
				++ s2;
			}
		}
		else if (*s2 == '.')
			/* only s2 is dotted, it comes first */
			return 1;
	}
	else if (options & DONNA_SORT_DOT_MIXED)
	{
		if (*s1 == '.')
			++ s1;

		if (*s2 == '.')
			++ s2;
	}

	/* to continue we'll need the wide-char strings */
	_ws1 = _mbstowcs (wb1, s1);

	if (! _ws1)
		/* convertion failed, fallback to strcmp() */
		return strcmp (s1, s2);

	_ws2 = _mbstowcs (wb2, s2);

	if (! _ws2)
	{
		/* convertion failed, fallback to strcmp() */
		if (_ws1 not_eq wb1)
			free (_ws1);

		return strcmp (s1, s2);
	}

	ws1 = _ws1;
	ws2 = _ws2;

	for ( ; ;)
	{
		wint_t wc1, wc2;

		wc1 = (wint_t) *ws1;
		wc2 = (wint_t) *ws2;

		if (is_string)
		{
			/* is at least one string over? */
			if (wc1 == L'\0')
			{
				if (wc2 == L'\0')
					res = 0;
				else
					/* shorter first */
					res = -1;

				goto done;
			}
			else if (wc2 == L'\0')
			{
				/* shorter first */
				res = 1;
				goto done;
			}

			if (options & DONNA_SORT_IGNORE_SPUNCT)
			{
				while (iswspace (wc1) or iswpunct (wc1))
					wc1 = (wint_t) *++ ws1;

				while (iswspace (wc2) or iswpunct (wc2))
					wc2 = (wint_t) *++ ws2;
			}

			/* is at least one string a number? */
			if (iswdigit (wc1))
			{
				if (iswdigit (wc2))
				{
					if (options & DONNA_SORT_NATURAL_ORDER)
					{
						/* switch to number comparison */
						is_string = false;
						continue;
					}
				}
				else
				{
					/* number first */
					res = -1;
					goto done;
				}
			}
			else if (iswdigit (wc2))
			{
				/* number first */
				res = 1;
				goto done;
			}

			/* compare chars */
			if (wc1 > wc2)
				res_cs = 1;
			else if (wc1 < wc2)
				res_cs = -1;

			if (options & DONNA_SORT_CASE_INSENSITIVE)
			{
				/* compare uppper chars */
				wc1 = towupper (wc1);
				wc2 = towupper (wc2);

				if (wc1 > wc2)
				{
					res = 1;
					goto done;
				}
				else if (wc1 < wc2)
				{
					res = -1;
					goto done;
				}
				else if (res_fb == 0)
					/* set case sensitive result as fallback in case strings are
					 * otherwise the same */
					res_fb = res_cs;
			}

			/* do we have a res_cs yet? */
			else if (res_cs not_eq 0)
			{
				res = res_cs;
				goto done;
			}

			/* next chars */
			++ ws1;
			++ ws2;
		}

		/* mode number */
		else
		{
			unsigned long n1, n2;

			if (res_fb == 0)
			{
				/* count number of leading zeros */
				for (n1 = 0; ws1 [n1] == L'0'; ++ n1)
					;

				for (n2 = 0; ws2 [n2] == L'0'; ++ n2)
					;

				/* try to set a fallback to put less leading zeros first */
				if (n1 > n2)
					res_fb = 1;
				else if (n1 < n2)
					res_fb = -1;
			}

			errno = 0;
			n1    = wcstoul (ws1, &ws1, 10);

			if (errno not_eq 0)
			{
				/* failed to get number -- fallback to strcmp() */
				res = strcmp (s1, s2);
				goto done;
			}

			n2 = wcstoul (ws2, &ws2, 10);

			if (errno not_eq 0)
			{
				/* failed to get number -- fallback to strcmp() */
				res = strcmp (s1, s2);
				goto done;
			}

			if (n1 > n2)
			{
				res = 1;
				goto done;
			}
			else if (n1 < n2)
			{
				res = -1;
				goto done;
			}

			/* back to string comparison */
			is_string = true;
		}
	}

done:

	if (_ws1 not_eq wb1)
		free (_ws1);

	if (_ws2 not_eq wb2)
		free (_ws2);

	return (res not_eq 0) ? res : res_fb;
}

}

bool
naturally_compare (const std::string & lhs, const std::string & rhs)
{
	return strcmp_ext (lhs .c_str (), rhs .c_str (),
	                   DONNA_SORT_NATURAL_ORDER |
	                   DONNA_SORT_CASE_INSENSITIVE |
	                   DONNA_SORT_DOT_FIRST |
	                   DONNA_SORT_IGNORE_SPUNCT) < 0;
}

} // basic
} // titania
