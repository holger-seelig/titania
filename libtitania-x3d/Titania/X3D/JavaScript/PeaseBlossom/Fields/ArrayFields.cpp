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
 * Copyright 1999, 2012 Holger Seelig <holger.seelig@yahoo.de>.
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

#include "ArrayFields.h"

namespace titania {
namespace X3D {
namespace peaseblossom {

template <>
const std::string MFBool::typeName = "MFBool";

template <>
const std::string MFDouble::typeName = "MFDouble";

template <>
const std::string MFFloat::typeName = "MFFloat";

template <>
const std::string MFInt32::typeName = "MFInt32";

template <>
const std::string MFString::typeName = "MFString";

template <>
const std::string MFTime::typeName = "MFTime";

template <>
const std::string MFVec2d::typeName = "MFVec2d";

template <>
const std::string MFVec2f::typeName = "MFVec2f";

template <>
const std::string MFVec3d::typeName = "MFVec3d";

template <>
const std::string MFVec3f::typeName = "MFVec3f";

template <>
const std::string MFVec4d::typeName = "MFVec4d";

template <>
const std::string MFVec4f::typeName = "MFVec4f";

template <>
const pb::Callbacks MFBool::callbacks = {
	enumerate,
	hasProperty,
	get1Value,
	set1Value,
	pb::ResolveCallback (),
	dispose <MFBool>
};

template <>
const pb::Callbacks MFDouble::callbacks = {
	enumerate,
	hasProperty,
	get1Value,
	set1Value,
	pb::ResolveCallback (),
	dispose <MFDouble>
};

template <>
const pb::Callbacks MFFloat::callbacks = {
	enumerate,
	hasProperty,
	get1Value,
	set1Value,
	pb::ResolveCallback (),
	dispose <MFFloat>
};

template <>
const pb::Callbacks MFInt32::callbacks = {
	enumerate,
	hasProperty,
	get1Value,
	set1Value,
	pb::ResolveCallback (),
	dispose <MFInt32>
};

template <>
const pb::Callbacks MFString::callbacks = {
	enumerate,
	hasProperty,
	get1Value,
	set1Value,
	pb::ResolveCallback (),
	dispose <MFString>
};

template <>
const pb::Callbacks MFTime::callbacks = {
	enumerate,
	hasProperty,
	get1Value,
	set1Value,
	pb::ResolveCallback (),
	dispose <MFTime>
};

template <>
const pb::Callbacks MFVec2d::callbacks = {
	enumerate,
	hasProperty,
	get1Value,
	set1Value,
	pb::ResolveCallback (),
	dispose <MFVec2d>
};

template <>
const pb::Callbacks MFVec2f::callbacks = {
	enumerate,
	hasProperty,
	get1Value,
	set1Value,
	pb::ResolveCallback (),
	dispose <MFVec2f>
};

template <>
const pb::Callbacks MFVec3d::callbacks = {
	enumerate,
	hasProperty,
	get1Value,
	set1Value,
	pb::ResolveCallback (),
	dispose <MFVec3d>
};

template <>
const pb::Callbacks MFVec3f::callbacks = {
	enumerate,
	hasProperty,
	get1Value,
	set1Value,
	pb::ResolveCallback (),
	dispose <MFVec3f>
};

template <>
const pb::Callbacks MFVec4d::callbacks = {
	enumerate,
	hasProperty,
	get1Value,
	set1Value,
	pb::ResolveCallback (),
	dispose <MFVec4d>
};

template <>
const pb::Callbacks MFVec4f::callbacks = {
	enumerate,
	hasProperty,
	get1Value,
	set1Value,
	pb::ResolveCallback (),
	dispose <MFVec4f>
};

} // peaseblossom
} // X3D
} // titania
