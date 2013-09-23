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

#ifndef __TITANIA_X3D_ROUTING_ROUTE_H__
#define __TITANIA_X3D_ROUTING_ROUTE_H__

#include "../Basic/X3DBaseNode.h"
#include "../Fields.h"

namespace titania {
namespace X3D {

typedef std::pair <X3DFieldDefinition*, X3DFieldDefinition*> RouteId;

class Route :
	public X3DBaseNode
{
public:

	///  @name Construction

	Route (X3DExecutionContext* const,
	       const SFNode &, X3DFieldDefinition* const,
	       const SFNode &, X3DFieldDefinition* const);

	virtual
	Route*
	clone (X3DExecutionContext* const) const final;

	///  @name Common members

	virtual
	const std::string &
	getComponentName () const final
	{ return componentName; }

	virtual
	const std::string &
	getTypeName () const
	throw (Error <DISPOSED>) final
	{ return typeName; }

	virtual
	const std::string &
	getContainerField () const final
	{ return containerField; }

	///  @name Member access

	bool
	isConnected ();

	RouteId
	getId () const;

	const SFNode &
	getSourceNode () const;

	const std::string &
	getSourceField () const;

	const SFNode &
	getDestinationNode () const;

	const std::string &
	getDestinationField () const;

	///  @name Operations

	void
	disconnect ();

	void
	remove ();

	///  @name Input/Output

	virtual
	void
	toStream (std::ostream &) const final;

	///  @name Desctuction

	virtual
	void
	dispose () final;


private:

	virtual
	X3DBaseNode*
	create (X3DExecutionContext* const) const final;

	virtual
	void
	initialize () final;

	void
	connect ();

	///  @name Static members

	static const std::string componentName;
	static const std::string typeName;
	static const std::string containerField;

	///  @name Members

	SFNode sourceNode;
	SFNode destinationNode;

	X3DFieldDefinition* const sourceField;
	X3DFieldDefinition* const destinationField;

	bool connected;

};

} // X3D
} // titania

#endif
