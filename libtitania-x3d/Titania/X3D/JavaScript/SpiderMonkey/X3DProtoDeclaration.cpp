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

#include "X3DProtoDeclaration.h"

#include "Context.h"
#include "FieldDefinitionArray.h"
#include "Fields/SFNode.h"
#include "String.h"

#include "../../Components/Core/X3DPrototypeInstance.h"
#include "../../Prototype/ProtoDeclaration.h"

namespace titania {
namespace X3D {
namespace spidermonkey {

const JSClassOps X3DProtoDeclaration::class_ops = {
	nullptr, // addProperty
	nullptr, // delProperty
	nullptr, // enumerate
	nullptr, // newEnumerate
	nullptr, // resolve
	nullptr, // mayResolve
	finalize, // finalize
	nullptr, // call
	nullptr, // hasInstance
	nullptr, // construct
	nullptr, // trace
};

const JSClass X3DProtoDeclaration::static_class = {
	"X3DProtoDeclaration",
	JSCLASS_HAS_PRIVATE | JSCLASS_HAS_RESERVED_SLOTS (size_t (SlotType::SIZE)) | JSCLASS_FOREGROUND_FINALIZE,
	&class_ops
};

const JSPropertySpec X3DProtoDeclaration::properties [ ] = {
	JS_PSGS ("name",          getName,       nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("fields",        getFields,     nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("isExternProto", isExternProto, nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PS_END
};

const JSFunctionSpec X3DProtoDeclaration::functions [ ] = {
	JS_FN ("newInstance",  newInstance,  0, JSPROP_PERMANENT),
	JS_FN ("toVRMLString", toVRMLString, 0, JSPROP_PERMANENT),
	JS_FN ("toXMLString",  toXMLString,  0, JSPROP_PERMANENT),
	JS_FS_END
};

JSObject*
X3DProtoDeclaration::init (JSContext* const cx, JS::HandleObject global, JS::HandleObject parent)
{
	const auto proto = JS_InitClass (cx, global, parent, &static_class, construct, 0, properties, functions, nullptr, nullptr);

	if (not proto)
		throw std::runtime_error ("Couldn't initialize JavaScript global object.");

	return proto;
}

bool
X3DProtoDeclaration::construct (JSContext* cx, unsigned argc, JS::Value* vp)
{
	return ThrowException <JSProto_Error> (cx, "new %s: %s.", getClass () -> name, "object is not constructible");
}

JS::Value
X3DProtoDeclaration::create (JSContext* const cx, const X3D::ProtoDeclarationPtr & proto)
{
	const auto context = getContext (cx);
	const auto key     = size_t (proto .getValue ());
	const auto obj     = context -> getObject (key);

	if (obj)
	{
		return JS::ObjectValue (*obj);
	}
	else
	{
		const auto obj = JS_NewObjectWithGivenProto (cx, getClass (), context -> getProto (getId ()));

		if (not obj)
			throw std::runtime_error ("out of memory");

		const auto self = new internal_type (proto);

		setObject (obj, self);
		setContext (obj, context);
		setKey (obj, key);

		context -> addObject (key, self, obj);

		return JS::ObjectValue (*obj);
	}
}

// Properties

bool
X3DProtoDeclaration::getName (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		const auto   args = JS::CallArgsFromVp (argc, vp);
		const auto & self = *getThis <X3DProtoDeclaration> (cx, args);

		args .rval () .set (StringValue (cx, self -> getName ()));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .name: %s.", getClass () -> name, error .what ());
	}
}

bool
X3DProtoDeclaration::getFields (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		const auto   args = JS::CallArgsFromVp (argc, vp);
		const auto & self = *getThis <X3DProtoDeclaration> (cx, args);

		args .rval () .set (FieldDefinitionArray::create (cx, self -> getFieldDefinitions ()));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .fields: %s.", getClass () -> name, error .what ());
	}
}

bool
X3DProtoDeclaration::isExternProto (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		const auto   args = JS::CallArgsFromVp (argc, vp);
		const auto & self = *getThis <X3DProtoDeclaration> (cx, args);

		args .rval () .setBoolean (self -> isExternproto ());
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .isExternProto: %s.", getClass () -> name, error .what ());
	}
}

// Functions

bool
X3DProtoDeclaration::newInstance (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 0)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .newInstance: wrong number of arguments.", getClass () -> name);

		const auto   args = JS::CallArgsFromVp (argc, vp);
		const auto & self = *getThis <X3DProtoDeclaration> (cx, args);

		X3D::SFNode instance (self -> createInstance ());

		instance -> setup ();

		args .rval () .set (SFNode::create (cx, instance));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .newInstance: %s.", getClass () -> name, error .what ());
	}
}

bool
X3DProtoDeclaration::toVRMLString (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 0)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .toVRMLString: wrong number of arguments.", getClass () -> name);

		const auto   args    = JS::CallArgsFromVp (argc, vp);
		const auto   context = getContext (cx);
		const auto & self    = *getThis <X3DProtoDeclaration> (cx, args);
		const auto   version = context -> getExecutionContext () -> getSpecificationVersion ();

		std::ostringstream osstream;

		osstream .imbue (std::locale::classic ());

		Generator::SpecificationVersion (osstream, version);
		Generator::NicestStyle (osstream);

		self -> toStream (osstream);

		args .rval () .set (StringValue (cx, osstream .str ()));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .toVRMLString: %s.", getClass () -> name, error .what ());
	}
}

bool
X3DProtoDeclaration::toXMLString (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 0)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .toXMLString: wrong number of arguments.", getClass () -> name);

		const auto   args    = JS::CallArgsFromVp (argc, vp);
		const auto   context = getContext (cx);
		const auto & self    = *getThis <X3DProtoDeclaration> (cx, args);
		auto         version = context -> getExecutionContext () -> getSpecificationVersion ();

		std::ostringstream osstream;

		osstream .imbue (std::locale::classic ());

		if (version == VRML_V2_0)
			version = LATEST_VERSION;

		Generator::SpecificationVersion (osstream, version);
		Generator::NicestStyle (osstream);

		self -> toXMLStream (osstream);

		args .rval () .set (StringValue (cx, osstream .str ()));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .toXMLString: %s.", getClass () -> name, error .what ());
	}
}

// Destruction

void
X3DProtoDeclaration::finalize (JSFreeOp* fop, JSObject* obj)
{
	const auto context = getContext (obj);
	const auto self    = getObject <internal_type*> (obj);

	// Proto objects have no private.

	if (self)
		context -> removeObject (getKey (obj));
}

} // spidermonkey
} // X3D
} // titania
