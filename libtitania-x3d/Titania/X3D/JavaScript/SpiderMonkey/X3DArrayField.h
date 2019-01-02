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

#ifndef __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_X3DARRAY_FIELD_H__
#define __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_X3DARRAY_FIELD_H__

#include "X3DField.h"

namespace titania {
namespace X3D {
namespace spidermonkey {

class X3DArrayField
{
public:

	///  @name Member types

	using internal_type = X3D::X3DFieldDefinition;

	///  @name Construction

	static
	JSObject*
	init (JSContext* const cx, JS::HandleObject global, JS::HandleObject parent);

	static
	const JSClass*
	getClass ()
	{ return &static_class; }

	static
	constexpr
	ObjectType
	getId ()
	{ return ObjectType::X3DArrayField; }


private:

	///  @name Construction

	static bool construct (JSContext* cx, unsigned argc, JS::Value* vp);

	///  @name Static members

	static const JSClassOps     class_ops;
	static const JSClass        static_class;
	static const JSPropertySpec properties [ ];
	static const JSFunctionSpec functions [ ];

};

template <class ValueType, class InternalType>
class X3DArrayFieldTemplate :
	public X3DField
{
public:

	///  @name Member types

	using value_type    = ValueType;
	using internal_type = InternalType;
	using single_type   = typename ValueType::internal_type;

	///  @name Construction

	static
	JSObject*
	init (JSContext* const cx, JS::HandleObject global, JS::HandleObject parent);

	static
	JS::Value
	create (JSContext* const cx, InternalType* const field);

	static
	const JSClass*
	getClass ()
	{ return &static_class; }

	static
	constexpr
	ObjectType
	getId ()
	{ throw std::runtime_error ("getId"); }

private:

	///  @name Construction

	static bool construct (JSContext* cx, unsigned argc, JS::Value* vp);

	///  @name Member access

	static bool getLength (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool setLength (JSContext* cx, unsigned argc, JS::Value* vp);

	///  @name Functions



	///  @name Member access

	///  throws std::invalid_argument, std::domain_error
	template <class Class>
	static
	std::enable_if_t <
		not (std::is_integral_v <typename Class::internal_type::internal_type> or
		     std::is_floating_point_v <typename Class::internal_type::internal_type> or
		     std::is_same_v <typename Class::internal_type::internal_type, std::string> or
		     std::is_same_v <typename Class::internal_type::internal_type, X3D::String>),
		typename Class::internal_type &
	>
	getArgument (JSContext* const cx, const JS::CallArgs & args, const size_t index)
	{
		return *spidermonkey::getArgument <Class> (cx, args, index);
	}

	///  throws std::invalid_argument, std::domain_error
	template <class Class>
	static
	std::enable_if_t <
		std::is_integral_v <typename Class::internal_type::internal_type> or
		std::is_floating_point_v <typename Class::internal_type::internal_type> or
		std::is_same_v <typename Class::internal_type::internal_type, std::string> or
		std::is_same_v <typename Class::internal_type::internal_type, X3D::String>,
		typename Class::internal_type::internal_type
	>
	getArgument (JSContext* const cx, const JS::CallArgs & args, const size_t index)
	{
		return spidermonkey::getArgument <typename Class::internal_type::internal_type> (cx, args, index);
	}

	///  throws std::invalid_argument, std::domain_error
	template <class Class>
	static
	std::enable_if_t <
		std::is_integral_v <typename Class::internal_type> or
		std::is_floating_point_v <typename Class::internal_type> or
		std::is_same_v <typename Class::internal_type, std::string> or
		std::is_same_v <typename Class::internal_type, X3D::String>,
		typename Class::internal_type
	>
	getArgument (JSContext* const cx, const JS::CallArgs & args, const size_t index)
	{
		return spidermonkey::getArgument <typename Class::internal_type> (cx, args, index);
	}

	///  @name Static members

	static const JSClassOps     class_ops;
	static const JSClass        static_class;
	static const JSPropertySpec properties [ ];
	static const JSFunctionSpec functions [ ];

};

template <class ValueType, class InternalType>
const JSClassOps X3DArrayFieldTemplate <ValueType, InternalType>::class_ops = {
	nullptr, // addProperty
	nullptr, // delProperty
	nullptr, // getProperty
	nullptr, // setProperty
	nullptr, // enumerate
	nullptr, // resolve
	nullptr, // mayResolve
	finalize <X3DArrayFieldTemplate>, // finalize
	nullptr, // call
	nullptr, // hasInstance
	nullptr, // construct
	nullptr, // trace
};

template <class ValueType, class InternalType>
const JSPropertySpec X3DArrayFieldTemplate <ValueType, InternalType>::properties [ ] = {
	JS_PSGS ("length", getLength, setLength, JSPROP_PERMANENT),
	JS_PS_END
};

template <class ValueType, class InternalType>
const JSFunctionSpec X3DArrayFieldTemplate <ValueType, InternalType>::functions [ ] = {
//	JS_FS ("unshift", unshift, 1, JSPROP_PERMANENT),
//	JS_FS ("push",    push,    1, JSPROP_PERMANENT),
//	JS_FS ("shift",   shift,   0, JSPROP_PERMANENT),
//	JS_FS ("pop",     pop,     0, JSPROP_PERMANENT),
//	JS_FS ("splice",  splice,  2, JSPROP_PERMANENT),
	JS_FS_END
};

template <class ValueType, class InternalType>
JSObject*
X3DArrayFieldTemplate <ValueType, InternalType>::init (JSContext* const cx, JS::HandleObject global, JS::HandleObject parent)
{
	const auto proto = JS_InitClass (cx, global, parent, &static_class, construct, 0, properties, functions, nullptr, nullptr);

	if (not proto)
		throw std::runtime_error ("Couldn't initialize JavaScript global object.");

	return proto;
}

template <class ValueType, class InternalType>
JS::Value
X3DArrayFieldTemplate <ValueType, InternalType>::create (JSContext* const cx, InternalType* const field)
{
	return X3DField::create <X3DArrayFieldTemplate> (cx, field) .first;
}

template <class ValueType, class InternalType>
bool
X3DArrayFieldTemplate <ValueType, InternalType>::construct (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc == 0)
		{
			JS::CallArgsFromVp (argc, vp) .rval () .set (create (cx, new InternalType ()));
			return true;
		}
		else
		{
			const auto args  = JS::CallArgsFromVp (argc, vp);
			const auto array = new InternalType ();
			
			for (unsigned i = 0; i < argc; ++ i)
			{
				array -> emplace_back (getArgument <ValueType> (cx, args, i));
			}

			args .rval () .set (create (cx, array));
			return true;
		}
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "new %s: %s.", getClass () -> name, error .what ());
	}
}

template <class ValueType, class InternalType>
bool
X3DArrayFieldTemplate <ValueType, InternalType>::getLength (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		const auto args  = JS::CallArgsFromVp (argc, vp);
		const auto array = getThis <X3DArrayFieldTemplate> (cx, args);

		args .rval () .setNumber (uint32_t (array -> size ()));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .length: %s.", getClass () -> name, error .what ());
	}
}

template <class ValueType, class InternalType>
bool
X3DArrayFieldTemplate <ValueType, InternalType>::setLength (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		const auto args  = JS::CallArgsFromVp (argc, vp);
		const auto array = getThis <X3DArrayFieldTemplate> (cx, args);
		const auto size  = spidermonkey::getArgument <uint32_t> (cx, args, 0);

		array -> resize (size);
		return true;
	}
	catch (const std::bad_alloc & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .length: out of memory.", getClass () -> name);
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .length: %s.", getClass () -> name, error .what ());
	}
}

} // spidermonkey
} // X3D
} // titania

#endif
