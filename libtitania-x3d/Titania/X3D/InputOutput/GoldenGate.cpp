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

#include "GoldenGate.h"

#include "../Fields.h"
#include "../Browser/Sound/MediaStream.h"
#include "../Parser/JSONParser.h"
#include "../Parser/XMLParser.h"
#include "../Parser/Filter.h"
#include "../Parser/Autodesk/Parser.h"
#include "../Parser/PDF/Parser.h"
#include "../Parser/SVG/Parser.h"
#include "../Parser/VRML1/Parser.h"
#include "../Parser/Wavefront/Parser.h"

#include <Titania/OS.h>
#include <Titania/Physics/Constants.h>
#include <Titania/String/to_string.h>

#include <Magick++.h>
#include <giomm.h>
#include <regex>
#include <sys/wait.h>

namespace titania {
namespace X3D {

static const std::regex Name        ("__NAME__");
static const std::regex Description ("__DESCRIPTION__");
static const std::regex Width       ("__WIDTH__");
static const std::regex Height      ("__HEIGHT__");
static const std::regex URL         ("__URL__");

template <class Type>
static
void
golden_parser (const X3DScenePtr & scene, const basic::uri & uri, basic::ifilestream & istream)
{
	Type (scene, uri, istream) .parseIntoScene ();
}

static
bool
golden_is_vrml1 (std::istream & istream)
{
	const auto   state     = istream .rdstate ();
	auto         data      = std::string (32, 0);
	const size_t data_size = istream .rdbuf () -> sgetn (&data [0], data .size ());

	// Reset stream.

	istream .clear (state);

	for (size_t i = 0; i < data_size; ++ i)
		istream .unget ();

	static const std::regex VRML1 (R"/(^#VRML\s+V1.[01])/");

	return std::regex_search (data, VRML1);
}

static
void
golden_x3dv (const X3DScenePtr & scene, const basic::uri & uri, basic::ifilestream & istream)
{
	if (golden_is_vrml1 (istream))
		VRML1::Parser (scene, uri, istream) .parseIntoScene ();

	else
		scene -> fromStream (uri, istream);
}

static
void
golden_text (const X3DScenePtr & scene, const basic::uri & uri, basic::ifilestream & istream)
{
	// Test

	const auto state = istream .rdstate ();
	const auto pos   = istream .tellg ();

	try
	{
		return golden_x3dv (scene, uri, istream);
	}
	catch (const X3DError & error)
	{
		istream .clear (state);

		for (size_t i = 0, size = istream .tellg () - pos; i < size; ++ i)
			istream .unget ();

		return golden_parser <XMLParser> (scene, uri, istream);
	}
}

static
void
golden_image (const X3DScenePtr & scene, const basic::uri & uri, basic::ifilestream & istream)
{
	Magick::Image image;
	image .read (uri);
	image .resolutionUnits (Magick::PixelsPerInchResolution);

	const auto width  = double (image .size () .width  ()) / double (image .density () .width  ()) * inch <double>;
	const auto height = double (image .size () .height ()) / double (image .density () .height ()) * inch <double>;

	std::string file = os::load_file (os::find_data_file ("titania/goldengate/image.x3dv"));

	file = std::regex_replace (file, Name,   get_name_from_uri (uri));
	file = std::regex_replace (file, Width,  basic::to_string (width,  std::locale::classic ()));
	file = std::regex_replace (file, Height, basic::to_string (height, std::locale::classic ()));
	file = std::regex_replace (file, URL,    "[ " + SFString (uri .basename ()) .toString () + ", " + SFString (uri .str ()) .toString () + " ]");

	// Parse into scene.

	basic::ifilestream goldenstream (file);
	
	scene -> fromStream (uri, goldenstream);
}

static
void
golden_audio (const X3DScenePtr & scene, const basic::uri & uri, basic::ifilestream & istream)
{
	std::string file = os::load_file (os::find_data_file ("titania/goldengate/audio.x3dv"));

	file = std::regex_replace (file, Name,        get_name_from_uri (uri));
	file = std::regex_replace (file, Description, SFString (uri .basename (false)) .toString ());
	file = std::regex_replace (file, URL,         "[ " + SFString (uri .basename ()) .toString () + ", " + SFString (uri .str ()) .toString () + " ]");

	// Parse into scene.

	basic::ifilestream goldenstream (file);
	
	scene -> fromStream (uri, goldenstream);
}

static
void
golden_video (const X3DScenePtr & scene, const basic::uri & uri, basic::ifilestream & istream)
{
	MediaStream mediaStream;

	mediaStream .setup ();
	mediaStream .setUri (uri);
	mediaStream .sync ();

	std::string file = os::load_file (os::find_data_file ("titania/goldengate/video.x3dv"));

	const auto width  = mediaStream .getWidth  () ? mediaStream .getWidth  () / 72.0 * inch <double> : 1.0;
	const auto height = mediaStream .getHeight () ? mediaStream .getHeight () / 72.0 * inch <double> : 1.0;

	file = std::regex_replace (file, Name,        get_name_from_uri (uri));
	file = std::regex_replace (file, Description, SFString (uri .basename (false)) .toString ());
	file = std::regex_replace (file, Width,       basic::to_string (width,  std::locale::classic ()));
	file = std::regex_replace (file, Height,      basic::to_string (height, std::locale::classic ()));
	file = std::regex_replace (file, URL,         "[ " + SFString (uri .basename ()) .toString () + ", " + SFString (uri .str ()) .toString () + " ]");

	// Parse into scene.

	basic::ifilestream goldenstream (file);
	
	scene -> fromStream (uri, goldenstream);
}

using GoldenFunction = std::function <void (const X3DScenePtr &, const basic::uri &, basic::ifilestream &)>;

static
std::map <std::string, GoldenFunction>
get_content_types ()
{
	std::map <std::string, GoldenFunction> contentTypes;

	contentTypes .emplace ("model/vrml",                       &golden_x3dv);
	contentTypes .emplace ("x-world/x-vrml",                   &golden_x3dv);
	contentTypes .emplace ("model/x3d+vrml",                   &golden_x3dv);
	contentTypes .emplace ("model/x3d+xml",                    &golden_parser <XMLParser>);
	contentTypes .emplace ("application/xml",                  &golden_parser <XMLParser>);
	contentTypes .emplace ("application/vnd.hzn-3d-crossword", &golden_parser <XMLParser>);
	contentTypes .emplace ("application/json",                 &golden_parser <JSONParser>);
	contentTypes .emplace ("application/ogg",                  &golden_video);
	contentTypes .emplace ("application/x-3ds",                &golden_parser <Autodesk::Parser>);
	contentTypes .emplace ("image/x-3ds",                      &golden_parser <Autodesk::Parser>);

	if (os::program_exists ("inkscape"))
	{
		contentTypes .emplace ("application/pdf",                  &golden_parser <PDF::Parser>);
		contentTypes .emplace ("application/x-pdf",                &golden_parser <PDF::Parser>);
		contentTypes .emplace ("application/x-gzpdf",              &golden_parser <PDF::Parser>);
	}

	contentTypes .emplace ("image/svg+xml", &golden_parser <SVG::Parser>);
	contentTypes .emplace ("text/plain",    &golden_text);

	return contentTypes;
}

static
std::map <std::string, GoldenFunction>
get_suffixes ()
{
	std::map <std::string, GoldenFunction> suffixes;

	// VRML
	suffixes .emplace (".wrl",    &golden_x3dv);
	suffixes .emplace (".wrz",    &golden_x3dv);
	suffixes .emplace (".wrl.gz", &golden_x3dv); /// Todo: does not work with URI::suffix
	suffixes .emplace (".vrml",   &golden_x3dv);
	suffixes .emplace (".vrm",    &golden_x3dv);

	// X3D Vrml Classic Encoding 
	suffixes .emplace (".x3dv",    &golden_x3dv);
	suffixes .emplace (".x3dvz",   &golden_x3dv);
	suffixes .emplace (".x3dv.gz", &golden_x3dv); /// Todo: does not work with URI::suffix

	// X3D XML Encoding 
	suffixes .emplace (".x3d",    &golden_parser <XMLParser>);
	suffixes .emplace (".x3dz",   &golden_parser <XMLParser>);
	suffixes .emplace (".x3d.gz", &golden_parser <XMLParser>); /// Todo: does not work with URI::suffix
	suffixes .emplace (".xml",    &golden_parser <XMLParser>);

	// X3D XML Encoding 
	suffixes .emplace (".json", &golden_parser <JSONParser>);

	// Autodesk 3DS Max
	suffixes .emplace (".3ds", &golden_parser <Autodesk::Parser>);

	// Wavefront OBJ
	suffixes .emplace (".obj", &golden_parser <Wavefront::Parser>);

	if (os::program_exists ("inkscape"))
	{
		// PDF
		suffixes .emplace (".pdf", &golden_parser <PDF::Parser>);
	}

	// SVG
	suffixes .emplace (".svg",  &golden_parser <SVG::Parser>);
	suffixes .emplace (".svgz", &golden_parser <SVG::Parser>);

	return suffixes;
}

void
golden_gate (const X3DScenePtr & scene, const basic::uri & uri, basic::ifilestream & istream)
{
	try
	{
		static const auto contentTypes = get_content_types ();
		static const auto suffixes     = get_suffixes ();

		try
		{
			const std::string contentType = istream .response_headers () .at ("Content-Type");
		
			//__LOG__ << contentType << " : " << uri << std::endl;
	
			try
			{
				return contentTypes .at (contentType) (scene, uri, istream);
			}
			catch (const std::out_of_range &)
			{
				try
				{
					return suffixes .at (uri .suffix ()) (scene, uri, istream);
				}
				catch (const std::out_of_range &)
				{
					if (Gio::content_type_is_a (contentType, "image/*"))
						return golden_image (scene, uri, istream);
	
					if (Gio::content_type_is_a (contentType, "audio/*"))
						return golden_audio (scene, uri, istream);
	
					if (Gio::content_type_is_a (contentType, "video/*"))
						return golden_video (scene, uri, istream);
				}
			}
		}
		catch (const std::out_of_range &)
		{ }
	
		golden_text (scene, uri, istream);
	}
	catch (const X3DError &)
	{
		throw;
	}
	catch (const std::exception & error)
	{
		throw Error <INVALID_X3D> (error .what ());
	}
	catch (...)
	{
		throw Error <INVALID_X3D> ("Unkown error while loading '" + uri .str () + "'.");
	}
}

} // X3D
} // titania
