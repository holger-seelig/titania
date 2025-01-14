// ============================================================================
// gzstream, C++ iostream classes wrapping the zlib compression library.
// Copyright (C) 2001  Deepak Bandyopadhyay, Lutz Kettner
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
// ============================================================================
//
// File          : gzstream .h
// Revision      : $Revision: 1.5 $
// Revision_date : $Date: 2002/04/26 23:30:15 $
// Author(s)     : Deepak Bandyopadhyay, Lutz Kettner
//
// Standard streambuf implementation following Nicolai Josuttis, "The
// Standard C++ Library".
// ============================================================================

#ifndef __TITANIA_GZSTREAM_H__
#define __TITANIA_GZSTREAM_H__

// standard C++ with new header file names and  std::namespace
#include <fstream>
#include <iostream>
#include <zlib.h>

namespace titania {
namespace basic {

// ----------------------------------------------------------------------------
// Internal classes to implement gzstream. See below for user classes.
// ----------------------------------------------------------------------------

class gzstreambuf :
	public std::streambuf
{
public:

	gzstreambuf () :
		opened (0)
	{
		setp (buffer, buffer + (bufferSize - 1));
		setg (buffer + 4,                    // beginning of putback area
		      buffer + 4,                    // read position
		      buffer + 4);                   // end position
		// ASSERT: both input & output capabilities will not be used together
	}

	int
	is_open () { return opened; }

	gzstreambuf*
	open (const std::string & name, int open_mode);

	gzstreambuf*
	close ();

	~gzstreambuf () { close (); }

	virtual int
	overflow (int c = EOF);

	virtual int
	underflow ();

	virtual int
	sync ();


private:

	static const int bufferSize = 47 + 256; // size of data buff
	// totals 512 bytes under g++ for igzstream at the end.

	gzFile file;                            // file handle for compressed file
	char   buffer [bufferSize];             // data buffer
	char   opened;                          // open/close state of stream
	int    mode;                            // I/O mode

	int
	flush_buffer ();

};

class gzstreambase :
	virtual public std::ios
{
public:

	gzstreambase () { init (&buf); }

	gzstreambase (const std::string & name, int open_mode);

	~gzstreambase ();

	void
	open (const std::string & name, int open_mode);

	void
	close ();

	gzstreambuf*
	rdbuf () { return &buf; }


protected:

	gzstreambuf buf;

};

// ----------------------------------------------------------------------------
// User classes. Use igzstream and ogzstream analogously to ifstream and
// ofstream respectively. They read and write files based on the gz*
// function interface of the zlib. Files are compatible with gzip compression.
// ----------------------------------------------------------------------------

class igzstream :
	public gzstreambase, public std::istream
{
public:

	igzstream () :
		std::istream (&buf) { }

	igzstream (const std::string & name) :
		gzstreambase (name, std::ios::in), std::istream (&buf) { }

	gzstreambuf*
	rdbuf () { return gzstreambase::rdbuf (); }

	void
	open (const std::string & name)
	{
		gzstreambase::open (name, std::ios::in);
	}

};

class ogzstream :
	public gzstreambase, public std::ostream
{
public:

	ogzstream () :
		std::ostream (&buf) { }

	ogzstream (const std::string & name) :
		gzstreambase (name, std::ios::out), std::ostream (&buf) { }

	gzstreambuf*
	rdbuf () { return gzstreambase::rdbuf (); }

	void
	open (const std::string & name)
	{
		gzstreambase::open (name, std::ios::out);
	}

};

} // basic
} // titania

#endif
