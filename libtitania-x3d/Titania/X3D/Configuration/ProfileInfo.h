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

#ifndef __TITANIA_X3D_CONFIGURATION_PROFILE_INFO_H__
#define __TITANIA_X3D_CONFIGURATION_PROFILE_INFO_H__

#include "../Base/X3DChildObject.h"
#include "../Bits/X3DConstants.h"
#include "../Configuration/ComponentInfoArray.h"
#include "../Fields/X3DPtr.h"

namespace titania {
namespace X3D {

class ProfileInfo :
	public X3DChildObject
{
public:

	///  @name Construction

	ProfileInfo (const std::string & title, const std::string & name, const ComponentInfoArray & components);

	ProfileInfo*
	copy (const CopyType type) const;

	ProfileInfo*
	copy (X3DExecutionContext* const executionContext, const CopyType type) const
	{ return copy (type); }

	///  @name Common members

	virtual
	const std::string &
	getTypeName () const final override
	{ return typeName; }

	///  @name Member access

	const std::string &
	getTitle () const
	{ return title; }

	const std::string &
	getProviderUrl () const
	{ return providerUrl; }

	const ComponentInfoArray &
	getComponents () const
	{ return *components; }

	///  @name Input/Output

	virtual
	void
	fromStream (std::istream & istream) final override;

	virtual
	void
	toStream (std::ostream & ostream) const final override;

	virtual
	void
	toXMLStream (std::ostream & ostream) const final override;

	virtual
	void
	toJSONStream (std::ostream & ostream) const final override;

	///  @name Destruction

	virtual
	void
	dispose () final override;

	virtual
	~ProfileInfo () final override;


private:

	///  @name Static members

	static const std::string typeName;

	///  @name Members

	const std::string title;
	const std::string providerUrl;

	ComponentInfoArrayPtr components;

};

template <class CharT, class Traits>
std::basic_ostream <CharT, Traits> &
operator << (std::basic_ostream <CharT, Traits> & ostream, const ProfileInfo & profileInfo)
{
	profileInfo .toStream (ostream);
	return ostream;
}

using ProfileInfoPtr = X3DPtr <ProfileInfo>;

struct XMLEncodeProfileType { const ProfileInfoPtr & profile; };

inline
XMLEncodeProfileType
XMLEncode (const ProfileInfoPtr & profile)
{
	return XMLEncodeProfileType { profile };
}

template <typename CharT, typename Traits>
inline
std::basic_ostream <CharT, Traits> &
operator << (std::basic_ostream <CharT, Traits> & ostream, const XMLEncodeProfileType & value)
{
	value .profile -> toXMLStream (ostream);
	return ostream;
}

} // X3D
} // titania

#endif
