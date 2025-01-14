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

#include "Context.h"

#include "Arguments.h"
#include "Error.h"
#include "Fields.h"
#include "Globals.h"
#include "String.h"
#include "value.h"

#include "ComponentInfo.h"
#include "ComponentInfoArray.h"
#include "ExternProtoDeclarationArray.h"
#include "FieldDefinitionArray.h"
#include "ProfileInfo.h"
#include "ProfileInfoArray.h"
#include "ProtoDeclarationArray.h"
#include "RouteArray.h"
#include "X3DBrowser.h"
#include "X3DConstants.h"
#include "X3DExecutionContext.h"
#include "X3DExternProtoDeclaration.h"
#include "X3DFieldDefinition.h"
#include "X3DProtoDeclaration.h"
#include "X3DRoute.h"
#include "X3DScene.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../../Thread/SceneFuture.h"

#include <js/Warnings.h>
#include <js/CompilationAndEvaluation.h>
#include <js/SourceText.h>

extern "C"
{
	#include "C-bind/bind.h"
}

#include <cassert>

namespace titania {
namespace X3D {
namespace spidermonkey {

const Component   Context::component      = Component ("Titania", 1);
const std::string Context::typeName       = "SpiderMonkeyContext";
const std::string Context::containerField = "context";

const JSClassOps Context::globalOps = {
	nullptr, // addProperty
	nullptr, // delProperty
	nullptr, // enumerate
	nullptr, // newEnumerate
	resolveStandardClasses, // resolve
	nullptr, // mayResolve
	nullptr, // finalize
	nullptr, // call
	nullptr, // hasInstance
	nullptr, // construct
	JS_GlobalObjectTraceHook, // trace
};

const JSClass Context::globalClass = {
	"global",
	JSCLASS_GLOBAL_FLAGS | JSCLASS_HAS_PRIVATE,
	&globalOps
};

const size_t    Context::MAX_OBJECTS = 65'536;
const time_type Context::GC_INTERVAL = 2;

///  throws std::exception
Context::Context (JSContext* const cx, X3D::Script* const script, const std::string & ecmascript, const basic::uri & uri) :
	         X3D::X3DBaseNode (script -> getBrowser (), script -> getExecutionContext ()),
	X3D::X3DJavaScriptContext (script, ecmascript),
	                 worldURL (uri),
	                       cx (cx),
	                   global (),
	                   fields (),
	                   protos (size_t (ObjectType::SIZE)),
	                  objects (),
	               references (),
	         shutdownFunction (),
	                  futures ()
{
	if (not cx)
		throw std::runtime_error ("Couldn't create JavaScript context.");

	addChildObjects (futures);
}

X3DBaseNode*
Context::create (X3D::X3DExecutionContext* const) const
{
	return new Context (cx, getScriptNode (), getECMAScript (), worldURL);
}

void
Context::setExecutionContext (X3D::X3DExecutionContext* const executionContext)
{
	for (const auto & future : futures)
		future -> setExecutionContext (executionContext);

	X3DJavaScriptContext::setExecutionContext (executionContext);
}

bool
Context::resolveStandardClasses (JSContext* cx, JS::HandleObject obj, JS::HandleId id, bool* resolvedp)
{
	if (JS_ResolveStandardClass (cx, obj, id, resolvedp))
		return true;

	*resolvedp = false;
	return true;
}

void
Context::addClasses ()
{
	Globals::init (cx, *global);

	addProto (X3DBrowser::getId (),          X3DBrowser::init          (cx, *global, nullptr));
	addProto (X3DConstants::getId (),        X3DConstants::init        (cx, *global, nullptr));
	addProto (X3DExecutionContext::getId (), X3DExecutionContext::init (cx, *global, nullptr));
	addProto (X3DScene::getId (),            X3DScene::init            (cx, *global, getProto (X3DExecutionContext::getId ())));

	addProto (ProfileInfo::getId (),               ProfileInfo::init               (cx, *global, nullptr));
	addProto (ComponentInfo::getId (),             ComponentInfo::init             (cx, *global, nullptr));
	addProto (X3DExternProtoDeclaration::getId (), X3DExternProtoDeclaration::init (cx, *global, nullptr));
	addProto (X3DProtoDeclaration::getId (),       X3DProtoDeclaration::init       (cx, *global, nullptr));
	addProto (X3DRoute::getId (),                  X3DRoute::init                  (cx, *global, nullptr));
	addProto (X3DFieldDefinition::getId (),        X3DFieldDefinition::init        (cx, *global, nullptr));

	addProto (ProfileInfoArray::getId (),            ProfileInfoArray::init            (cx, *global, nullptr));
	addProto (ComponentInfoArray::getId (),          ComponentInfoArray::init          (cx, *global, nullptr));
	addProto (ExternProtoDeclarationArray::getId (), ExternProtoDeclarationArray::init (cx, *global, nullptr));
	addProto (ProtoDeclarationArray::getId (),       ProtoDeclarationArray::init       (cx, *global, nullptr));
	addProto (RouteArray::getId (),                  RouteArray::init                  (cx, *global, nullptr));
	addProto (FieldDefinitionArray::getId (),        FieldDefinitionArray::init        (cx, *global, nullptr));

	addProto (X3DField::getId (),      X3DField::init      (cx, *global, nullptr));
	addProto (X3DArrayField::getId (), X3DArrayField::init (cx, *global, getProto (X3DField::getId ())));

	addProto (SFColor::getId (),     SFColor::init     (cx, *global, getProto (X3DField::getId ())));
	addProto (SFColorRGBA::getId (), SFColorRGBA::init (cx, *global, getProto (X3DField::getId ())));
	addProto (SFImage::getId (),     SFImage::init     (cx, *global, getProto (X3DField::getId ())));
	addProto (SFMatrix3d::getId (),  SFMatrix3d::init  (cx, *global, getProto (X3DField::getId ())));
	addProto (SFMatrix3f::getId (),  SFMatrix3f::init  (cx, *global, getProto (X3DField::getId ())));
	addProto (SFMatrix4d::getId (),  SFMatrix4d::init  (cx, *global, getProto (X3DField::getId ())));
	addProto (SFMatrix4f::getId (),  SFMatrix4f::init  (cx, *global, getProto (X3DField::getId ())));
	addProto (SFNode::getId (),      SFNode::init      (cx, *global, getProto (X3DField::getId ())));
	addProto (SFRotation::getId (),  SFRotation::init  (cx, *global, getProto (X3DField::getId ())));
	addProto (SFVec2d::getId (),     SFVec2d::init     (cx, *global, getProto (X3DField::getId ())));
	addProto (SFVec2f::getId (),     SFVec2f::init     (cx, *global, getProto (X3DField::getId ())));
	addProto (SFVec3d::getId (),     SFVec3d::init     (cx, *global, getProto (X3DField::getId ())));
	addProto (SFVec3f::getId (),     SFVec3f::init     (cx, *global, getProto (X3DField::getId ())));
	addProto (SFVec4d::getId (),     SFVec4d::init     (cx, *global, getProto (X3DField::getId ())));
	addProto (SFVec4f::getId (),     SFVec4f::init     (cx, *global, getProto (X3DField::getId ())));
	addProto (VrmlMatrix::getId (),  VrmlMatrix::init  (cx, *global, getProto (X3DField::getId ())));

	addProto (MFBool::getId (),      MFBool::init      (cx, *global, getProto (X3DArrayField::getId ())));
	addProto (MFColor::getId (),     MFColor::init     (cx, *global, getProto (X3DArrayField::getId ())));
	addProto (MFColorRGBA::getId (), MFColorRGBA::init (cx, *global, getProto (X3DArrayField::getId ())));
	addProto (MFDouble::getId (),    MFDouble::init    (cx, *global, getProto (X3DArrayField::getId ())));
	addProto (MFFloat::getId (),     MFFloat::init     (cx, *global, getProto (X3DArrayField::getId ())));
	addProto (MFImage::getId (),     MFImage::init     (cx, *global, getProto (X3DArrayField::getId ())));
	addProto (MFInt32::getId (),     MFInt32::init     (cx, *global, getProto (X3DArrayField::getId ())));
	addProto (MFMatrix3d::getId (),  MFMatrix3d::init  (cx, *global, getProto (X3DArrayField::getId ())));
	addProto (MFMatrix3f::getId (),  MFMatrix3f::init  (cx, *global, getProto (X3DArrayField::getId ())));
	addProto (MFMatrix4d::getId (),  MFMatrix4d::init  (cx, *global, getProto (X3DArrayField::getId ())));
	addProto (MFMatrix4f::getId (),  MFMatrix4f::init  (cx, *global, getProto (X3DArrayField::getId ())));
	addProto (MFNode::getId (),      MFNode::init      (cx, *global, getProto (X3DArrayField::getId ())));
	addProto (MFRotation::getId (),  MFRotation::init  (cx, *global, getProto (X3DArrayField::getId ())));
	addProto (MFString::getId (),    MFString::init    (cx, *global, getProto (X3DArrayField::getId ())));
	addProto (MFTime::getId (),      MFTime::init      (cx, *global, getProto (X3DArrayField::getId ())));
	addProto (MFVec2d::getId (),     MFVec2d::init     (cx, *global, getProto (X3DArrayField::getId ())));
	addProto (MFVec2f::getId (),     MFVec2f::init     (cx, *global, getProto (X3DArrayField::getId ())));
	addProto (MFVec3d::getId (),     MFVec3d::init     (cx, *global, getProto (X3DArrayField::getId ())));
	addProto (MFVec3f::getId (),     MFVec3f::init     (cx, *global, getProto (X3DArrayField::getId ())));
	addProto (MFVec4d::getId (),     MFVec4d::init     (cx, *global, getProto (X3DArrayField::getId ())));
	addProto (MFVec4f::getId (),     MFVec4f::init     (cx, *global, getProto (X3DArrayField::getId ())));

	JS_DefineProperty (cx, *global, "Browser",      JS::RootedObject (cx, X3DBrowser::create (cx)),   JSPROP_PERMANENT | JSPROP_READONLY | JSPROP_ENUMERATE);
	JS_DefineProperty (cx, *global, "X3DConstants", JS::RootedObject (cx, X3DConstants::create (cx)), JSPROP_PERMANENT | JSPROP_READONLY | JSPROP_ENUMERATE);
}

void
Context::addUserDefinedFields ()
{
	for (const auto & field : getScriptNode () -> getUserDefinedFields ())
	{
		switch (field -> getType ())
		{
			case X3D::X3DConstants::SFBool:
			case X3D::X3DConstants::SFDouble:
			case X3D::X3DConstants::SFFloat:
			case X3D::X3DConstants::SFInt32:
			case X3D::X3DConstants::SFString:
			case X3D::X3DConstants::SFTime:
			{
				break;
			}
			default:
			{
				fields .emplace (field -> getName (), std::make_unique <JS::PersistentRooted <JS::Value>> (cx, getValue (cx, field)));
				break;
			}
		}

		switch (field -> getAccessType ())
		{
			case X3D::initializeOnly:
			case X3D::outputOnly:
			{
				defineProperty (*global, field, field -> getName (), JSPROP_ENUMERATE);
				break;
			}
			case X3D::inputOnly:
				break;
			case X3D::inputOutput:
			{
				defineProperty (*global, field, field -> getName (),              JSPROP_ENUMERATE);
				defineProperty (*global, field, field -> getName () + "_changed", 0);
				break;
			}
		}
	}
}

void
Context::defineProperty (JS::HandleObject obj,
                         X3D::X3DFieldDefinition* const field,
                         const std::string & name,
                         const uint32_t attrs)
{
	switch (field -> getType ())
	{
		case X3D::X3DConstants::SFBool:
		case X3D::X3DConstants::SFDouble:
		case X3D::X3DConstants::SFFloat:
		case X3D::X3DConstants::SFInt32:
		case X3D::X3DConstants::SFNode:
		case X3D::X3DConstants::SFString:
		case X3D::X3DConstants::SFTime:
		{
			JS_DefineProperty (cx,
			                   obj,
			                   name .c_str (),
			                   JSNative (partial_bind ((void*) &Context::getBuildInProperty, 4, 1, field)),
			                   JSNative (partial_bind ((void*) &Context::setProperty,        4, 1, field)),
			                   JSPROP_PERMANENT | attrs);
			break;
		}
		default:
		{
			JS_DefineProperty (cx,
			                   obj,
			                   name .c_str (),
			                   JSNative (partial_bind ((void*) &Context::getProperty, 4, 1, field)),
			                   JSNative (partial_bind ((void*) &Context::setProperty, 4, 1, field)),
			                   JSPROP_PERMANENT | attrs);
			break;
		}
	}
}

bool
Context::setProperty (X3D::X3DFieldDefinition* const field, JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		const auto args = JS::CallArgsFromVp (argc, vp);

		setValue (cx, field, args [0]);
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "Couldn't assing value to user-defined field '%s': %s.", field -> getName () .c_str (), error .what ());
	}
}

bool
Context::getBuildInProperty (X3D::X3DFieldDefinition* const field, JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		const auto args = JS::CallArgsFromVp (argc, vp);

		args .rval () .set (getValue (cx, field));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "Couldn't retrieve value of user-defined field '%s': %s.", field -> getName () .c_str (), error .what ());
	}
}

bool
Context::getProperty (X3D::X3DFieldDefinition* const field, JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		const auto args = JS::CallArgsFromVp (argc, vp);

		args .rval () .set (*getContext (cx) -> fields .at (field -> getName ()));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "Couldn't retrieve value of user-defined field '%s': %s.", field -> getName () .c_str (), error .what ());
	}
}

bool
Context::evaluate (const std::string & string, const std::string & filename)
{
	JS::SourceText <mozilla::Utf8Unit> srcBuf;
	JS::CompileOptions options (cx);
	JS::RootedValue rval (cx);

	options .setFile (filename .c_str ());

	srcBuf .init (cx, string .c_str (), string .size (), JS::SourceOwnership::Borrowed);

	if (JS::Evaluate (cx, options, srcBuf, &rval))
		return true;

	reportException ();
	return false;
}

void
Context::call (const std::string & value)
{
	JS::RootedValue fval (cx);

	if (getFunction (value, &fval))
		call (fval);
}

void
Context::call (JS::HandleValue value)
{
	JS::RootedValue rval (cx);

	JS_CallFunctionValue (cx, *global, value, JS::HandleValueArray::empty (), &rval);

	reportException ();
}

bool
Context::getFunction (const std::string & name, JS::MutableHandleValue value) const
{
	if (JS_GetProperty (cx, *global, name .c_str (), value))
	{
		if (value .isObjectOrNull ())
		{
			const auto obj = value .toObjectOrNull ();

			if (obj and JS_ObjectIsFunction (obj))
				return true;
		}
	}

	return false;
}

void
Context::addProto (const ObjectType type, JSObject* const proto)
{
	protos [size_t (type)] = std::make_unique <JS::PersistentRooted <JSObject*>> (cx, proto);
}

void
Context::addObject (const size_t key, X3D::X3DFieldDefinition* const field, JSObject* const obj)
{
	assert (objects .emplace (key, std::pair (field, obj)) .second);

	field -> addParent (this);
}

void
Context::removeObject (const size_t key)
{
	const auto iter = objects .find (key);

	assert (iter not_eq objects .end ());

	const auto field = iter -> second .first;

	field -> removeParent (this);

	objects .erase (iter);
}

JSObject*
Context::getObject (const size_t key) const
{
	const auto iter = objects .find (key);

	if (iter == objects .end ())
		return nullptr;

	return iter -> second .second;
}

void
Context::addReference (X3D::X3DFieldDefinition* const array, const size_t index, JSObject* const obj)
{
	assert (references .emplace (std::pair (array, index), obj) .second);
}

void
Context::removeReference (X3D::X3DFieldDefinition* const array, const size_t index)
{
	references .erase (std::pair (array, index));
}

JSObject*
Context::getReference (X3D::X3DFieldDefinition* const array, const size_t index)
{
	const auto iter = references .find (std::pair (array, index));

	if (iter == references .end ())
		return nullptr;

	return iter -> second;
}

void
Context::addFuture (const X3D::X3DPtr <X3D::SceneFuture> & future)
{
	futures .erase (std::remove_if (futures .begin (), futures .end (),
	[ ] (const X3D::X3DPtr <X3D::SceneFuture> & future)
	{
		if (future)
			return future -> isReady ();

		return true;
	}),
	futures .end ());

	futures .emplace_back (future);
}

void
Context::initialize ()
{
	X3DJavaScriptContext::initialize ();

	JS::RealmOptions options;

	global = std::make_unique <JS::PersistentRooted <JSObject*>> (cx, JS_NewGlobalObject (cx, &globalClass, nullptr, JS::FireOnNewGlobalHook, options));

	if (not *global)
		throw std::runtime_error ("Couldn't create JavaScript global object.");

	const JSAutoRealm ar (cx, *global);

	setObject (global .get () -> get (), this);

//	if (not JS_InitStandardClasses (cx, *global))
//		throw std::runtime_error ("Couldn't create JavaScript standard classes.");

	JS::SetWarningReporter (cx, &Context::reportError);

	const JS::AutoSaveExceptionState ases (cx);

	addClasses ();
	addUserDefinedFields ();

	if (not evaluate (getECMAScript (), worldURL))
		throw std::invalid_argument ("Couldn't evaluate script.");

	getExecutionContext () -> isLive () .addInterest (this, &Context::set_live);
	isLive () .addInterest (this, &Context::set_live);

	set_live ();

	if ((getExecutionContext () -> isLive () or getExecutionContext () -> isType ({ X3D::X3DConstants::X3DPrototypeInstance })) and isLive ())
	{
		call ("initialize");

		shutdownFunction = std::make_unique <JS::PersistentRooted <JS::Value>> (cx);

		if (not getFunction ("shutdown", shutdownFunction .get ()))
			shutdownFunction .reset ();
	}

	JS_MaybeGC (cx);
}

void
Context::set_live ()
{
	const JSAutoRealm ar (cx, *global);

	if ((getExecutionContext () -> isLive () or getExecutionContext () -> isType ({ X3D::X3DConstants::X3DPrototypeInstance })) and isLive ())
	{
		const auto prepareEvents   = std::make_shared <JS::PersistentRooted <JS::Value>> (cx);
		const auto eventsProcessed = std::make_shared <JS::PersistentRooted <JS::Value>> (cx);

		if (getFunction ("prepareEvents", prepareEvents .get ()))
			getBrowser () -> prepareEvents () .addInterest (&Context::prepareEvents, this, prepareEvents);

		if (getFunction ("eventsProcessed", eventsProcessed .get ()))
			getScriptNode () -> addInterest (&Context::eventsProcessed, this, eventsProcessed);

		getScriptNode () -> addInterest (&Context::collectGarbage, this);

		for (const auto & field : getScriptNode () -> getUserDefinedFields ())
		{
			switch (field -> getAccessType ())
			{
				case inputOnly:
				{
					const auto inputFunction = std::make_shared <JS::PersistentRooted <JS::Value>> (cx);

					if (getFunction (field -> getName (), inputFunction .get ()))
						field -> addInterest (&Context::set_field, this, field, inputFunction);

					break;
				}
				case inputOutput:
				{
					const auto inputFunction = std::make_shared <JS::PersistentRooted <JS::Value>> (cx);

					if (getFunction ("set_" + field -> getName (), inputFunction .get ()))
						field -> addInterest (&Context::set_field, this, field, inputFunction);

					break;
				}
				default:
					break;
			}
		}
	}
	else
	{
		getBrowser () -> prepareEvents () .removeInterest (&Context::prepareEvents, this);
		getScriptNode () -> removeInterest (&Context::eventsProcessed, this);
		getScriptNode () -> removeInterest (&Context::collectGarbage, this);

		for (const auto & field : getScriptNode () -> getUserDefinedFields ())
			field -> removeInterest (&Context::set_field, this);
	}
}

void
Context::prepareEvents (const std::shared_ptr <JS::PersistentRooted <JS::Value>> & functionValue)
{
	const JSAutoRealm ar (cx, *global);
	const JS::AutoSaveExceptionState ases (cx);

	JS::RootedValue rval (cx);
	JS::AutoValueArray <1> args (cx);

	args [0] .set (JS::DoubleValue (getCurrentTime ()));

	JS_CallFunctionValue (cx, *global, *functionValue, args, &rval);
	reportException ();
}

void
Context::set_field (X3D::X3DFieldDefinition* const field, const std::shared_ptr <JS::PersistentRooted <JS::Value>> & inputFunction)
{
	const JSAutoRealm ar (cx, *global);
	const JS::AutoSaveExceptionState ases (cx);

	field -> setTainted (true);

	JS::RootedValue rval (cx);
	JS::AutoValueArray <2> args (cx);

	args [0] .set (getValue (cx, field));
	args [1] .set (JS::DoubleValue (getCurrentTime ()));

	JS_CallFunctionValue (cx, *global, *inputFunction, args, &rval);
	reportException ();

	field -> setTainted (false);
}

void
Context::eventsProcessed (const std::shared_ptr <JS::PersistentRooted <JS::Value>> & functionValue)
{
	const JSAutoRealm ar (cx, *global);
	const JS::AutoSaveExceptionState ases (cx);

	call (*functionValue);
}

void
Context::shutdown ()
{
	const JSAutoRealm ar (cx, *global);
	const JS::AutoSaveExceptionState ases (cx);

	if (shutdownFunction)
		call (*shutdownFunction);

	X3D::X3DJavaScriptContext::shutdown ();
}

void
Context::collectGarbage ()
{
	const JSAutoRealm ar (cx, *global);

	//__LOG__ << objects .size () << std::endl;

	JS_MaybeGC (cx);
}

void
Context::reportException ()
{
	if (not JS_IsExceptionPending (cx))
		return;

	JS::RootedValue value (cx);

	if (not JS_GetPendingException (cx, &value))
		return;

	JS_ClearPendingException (cx);

	const JS::HandleValue exception (&value);

	if (exception .isObject ())
	{
		const auto object = JS::RootedObject  (cx, exception .toObjectOrNull ());
		const auto report = JS_ErrorFromException (cx, object);

		if (report)
		{
			reportError (cx, report);
			return;
		}
	}

	setError (to_string (cx, exception) .c_str (), worldURL .size () ? worldURL : "<inline>", 0, 0, "");
}

void
Context::reportError (JSContext* cx, JSErrorReport* const report)
{
	const auto context = getContext (cx);

	context -> setError (report -> message () .c_str (),
	                     report -> filename ? report -> filename : "<inline>",
	                     report -> lineno,
	                     report -> column,
	                     to_string (report -> linebuf (), report -> linebufLength ()));
}

void
Context::dispose ()
{
	const JSAutoRealm ar (cx, *global);

	futures          .clear ();
	shutdownFunction .reset ();
	fields           .clear ();
	protos           .clear ();
	global           .reset ();

	// finalize is not called for global values, probably the global object is not disposed.

	//assert (objects .empty ());

	for (const auto & [key, pair] : Objects (objects))
	{
		const auto [field, obj] = pair;

		setObject (obj, nullptr);
		removeObject (key);
	}

	references .clear ();
	objects    .clear ();

	X3D::X3DJavaScriptContext::dispose ();
}

Context::~Context ()
{ }

} // spidermonkey
} // X3D
} // titania
