// /* -*- Mode: C++; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*- */
// /*************************************************************************
//  *
//  * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
//  *
//  * Copyright 1999, 2012 Holger Seelig <holger.seelig@yahoo.de>.
//  *
//  * Titania - a multi-platform office productivity suite
//  *
//  * This file is part of the Titania Project.
//  *
//  * Titania is free software: you can redistribute it and/or modify
//  * it under the terms of the GNU Lesser General Public License version 3
//  * only, as published by the Free Software Foundation.
//  *
//  * Titania is distributed in the hope that it will be useful,
//  * but WITHOUT ANY WARRANTY; without even the implied warranty of
//  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  * GNU Lesser General Public License version 3 for more details
//  * (a copy is included in the LICENSE file that accompanied this code).
//  *
//  * You should have received a copy of the GNU Lesser General Public License
//  * version 3 along with Titania.  If not, see
//  * <http://www.gnu.org/licenses/lgpl.html>
//  * for a copy of the LGPLv3 License.
//  *
//  ************************************************************************/

#include "InputUrlStream.h"

#include <Titania/LOG.h>

namespace titania {
namespace basic {

iurlstream::iurlstream () :
	         std::istream (),
	                  buf (new urlstreambuf ()),
	  request_headers_map (),
	 response_headers_map (),
	response_http_version (),
	      response_status (0),
	      response_reason ("Could not establish connection.")
{
	init (buf .get ());
	clear ();
}

iurlstream::iurlstream (const basic::uri & url, size_t timeout) :
	          iurlstream ()
{
	open (url, timeout);
}

iurlstream::iurlstream (iurlstream && other) :
	         std::istream (),
	                  buf (std::move (other .buf)),
	  request_headers_map (std::move (other .request_headers_map)),
	 response_headers_map (std::move (other .response_headers_map)),
	response_http_version (std::move (other .response_http_version)),
	      response_status (other .response_status),
	      response_reason (std::move (other .response_reason))
{
	init (buf .get ());

	clear (other .rdstate ());
	other .clear (std::ios::badbit);
}

void
iurlstream::open (const basic::uri & url, size_t timeout)
{
	clear ();

	buf -> timeout (timeout);

	if (not buf -> open (url))
		setstate (std::ios::failbit);
}

void
iurlstream::send ()
{
	if (buf -> send (request_headers_map))
	{
		while (parse_status_line ())
			parse_response_headers ();
			
		return;
	}

	setstate (std::ios::failbit);
}

bool
iurlstream::parse_status_line ()
{
	std::stringstream line;

	rdbuf () -> headers () .get (*line .rdbuf (), widen ('\n'));

	if (rdbuf () -> headers ())
	{
		line
			>> response_http_version
			>> response_status;
		
		if (line)
		{
			sentry s (line);

			std::ostringstream oss_reason;
			oss_reason << line .rdbuf ();

			response_reason = oss_reason .str ();
			if (response_reason .size ())
				response_reason .resize (response_reason .size () - 1);

			if (rdbuf () -> headers () .get () == '\n')
				return true;
		}
	}

	return false;
}

void
iurlstream::parse_response_headers ()
{
	while (rdbuf () -> headers () .peek () not_eq '\r' and rdbuf () -> headers () .peek () not_eq urlstreambuf::traits_type::eof ())
		parse_response_header ();

	if (rdbuf () -> headers () .get () not_eq '\r')
		return close ();

	if (rdbuf () -> headers () .get () not_eq '\n')
		return close ();
}

void
iurlstream::parse_response_header ()
{
	std::stringstream line;
	std::string       header;
	std::stringstream value;

	rdbuf () -> headers () .get (*line .rdbuf (), widen ('\n'));

	// Header:
	line >> header;

	if (header .back () not_eq ':')
		return close ();

	header .resize (header .size () - 1);

	// Space
	line .get ();

	// Value
	line .get (*value .rdbuf (), widen ('\r'));

	if (rdbuf () -> headers () .get () not_eq '\n')
		return close ();

	response_headers_map .insert (std::make_pair (header, value .str ()));
}

void
iurlstream::close ()
{
	buf -> close ();

	request_headers_map   .clear ();
	response_headers_map  .clear ();
	response_http_version .clear ();
	response_reason       .clear ();
	response_status = 0;

	setstate (std::ios::badbit);
}

void
iurlstream::request_header (const std::string & header, const std::string & value)
{
	request_headers_map .insert (std::make_pair (header, value));
}

iurlstream::~iurlstream ()
{ }

} // basic
} // titania
