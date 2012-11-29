/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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
 ******************************************************************************/

#include "jsMFNode.h"

namespace titania {
namespace X3D {

JSClass jsMFNode::static_class = {
	"MFNode", JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub, JS_PropertyStub, get1Value, set1Value,
	(JSEnumerateOp) enumerate, JS_ResolveStub, JS_ConvertStub, finalize,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

void
jsMFNode::init (JSContext* context, JSObject* global)
{
	JS_InitClass (context, global, NULL, &static_class, construct,
	              0, properties, functions, NULL, NULL);
}

JSBool
jsMFNode::create (JSContext* context, X3DFieldDefinition* field, jsval* vp, const bool seal)
{
	JSObject* result = JS_NewObject (context, &static_class, NULL, NULL);

	if (result == NULL)
		return JS_FALSE;

	JS_SetPrivate (context, result, field);

	//if (seal)
	//	JS_SealObject (context, result, JS_FALSE);

	*vp = OBJECT_TO_JSVAL (result);

	return JS_TRUE;
}

JSBool
jsMFNode::construct (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 0)
	{
		return create (context, new MFNode <X3DBasicNode> (), &JS_RVAL (context, vp));
	}
	else
	{
		SFNode <X3DBasicNode> values [argc];

		jsval* argv = JS_ARGV (context, vp);

		for (uintN i = 0; i < argc; ++ i)
		{
			JSObject* obj2;

			if (not JS_ValueToObject (context, argv [i], &obj2))
				return JS_FALSE;

			if (not JS_InstanceOf (context, obj2, jsSFNode::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument %d is invalid - should be SFNode, is %s", i, JS_GetClass (context, obj2) -> name);
				return JS_FALSE;
			}

			values [i] = *(X3DField <X3DBasicNode*>*) JS_GetPrivate (context, obj2);
		}

		return create (context, new MFNode <X3DBasicNode> (values, values + argc), &JS_RVAL (context, vp));
	}

	return JS_FALSE;
}

JSBool
jsMFNode::enumerate (JSContext* context, JSObject* obj, JSIterateOp enum_op, jsval* statep, jsid* idp)
{
	MFNode <X3DBasicNode>* x3darrayfield = (MFNode <X3DBasicNode>*) JS_GetPrivate (context, obj); /// XXX

	if (not x3darrayfield)
	{
		*statep = JSVAL_NULL;
		return JS_TRUE;
	}

	size_t* index;

	switch (enum_op)
	{
		case JSENUMERATE_INIT:
		case JSENUMERATE_INIT_ALL:
		{
			index   = new size_t (0);
			*statep = PRIVATE_TO_JSVAL (index);

			if (idp)
				*idp = INT_TO_JSID (x3darrayfield -> size ());

			break;
		}
		case JSENUMERATE_NEXT:
		{
			index = (size_t*) JSVAL_TO_PRIVATE (*statep);

//			if (*index < x3darrayfield -> size ())
//			{
//				JS_DefineProperty (context,
//				                   obj, (char*) *index,
//				                   JSVAL_VOID,
//				                   get1Value, set1Value,
//				                   JSPROP_INDEX | JSPROP_SHARED | JSPROP_PERMANENT);
//
//				if (idp)
//					*idp = INT_TO_JSID (*index);
//
//				*index = *index + 1;
//				break;
//			}

		//else done -- cleanup.
		}
		case JSENUMERATE_DESTROY:
		{
			index = (size_t*) JSVAL_TO_PRIVATE (*statep);
			delete index;
			*statep = JSVAL_NULL;
		}
	}

	return JS_TRUE;
}

JSBool
jsMFNode::get1Value (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	if (not JSID_IS_INT (id))
		return JS_TRUE;

	int32 index = JSID_TO_INT (id);

	if (index < 0)
	{
		JS_ReportError (context, "index out of range");
		return JS_FALSE;
	}

//	MFNode* mfnode = (MFNode*) JS_GetPrivate (context, obj);

//	return jsSFNode::create (context, new SFNode (mfnode -> get1Value (index)), vp);

	return JS_TRUE;
}

JSBool
jsMFNode::set1Value (JSContext* context, JSObject* obj, jsid id, JSBool strict, jsval* vp)
{
	if (not JSID_IS_INT (id))
		return JS_TRUE;

	int32 index = JSID_TO_INT (id);

	if (index < 0)
	{
		JS_ReportError (context, "index out of range");
		return JS_FALSE;
	}

	//MFNode* mfnode = (MFNode*) JS_GetPrivate (context, obj);

	JSObject* obj2;

	if (not JS_ValueToObject (context, *vp, &obj2))
		return JS_FALSE;

	if (not JS_InstanceOf (context, obj2, jsSFNode::getClass (), NULL))
	{
		JS_ReportError (context, "Type of argument is invalid - should be SFNode, is %s", JS_GetClass (context, obj2) -> name);
		return JS_FALSE;
	}

	//mfnode -> set1Value (index, *(X3DField <X3DBasicNode*>*) JS_GetPrivate (context, obj2));

	*vp = JSVAL_VOID;

	return JS_TRUE;
}

} // X3D
} // titania
