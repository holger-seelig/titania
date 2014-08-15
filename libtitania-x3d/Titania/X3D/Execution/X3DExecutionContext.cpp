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

#include "X3DExecutionContext.h"

#include "../Bits/Cast.h"
#include "../Browser/X3DBrowser.h"
#include "../Components/Core/X3DPrototypeInstance.h"
#include "../Components/Navigation/X3DViewpointNode.h"
#include "../Components/Networking/Inline.h"
#include "../Execution/ImportedNode.h"
#include "../Execution/NamedNode.h"
#include "../Parser/RegEx.h"
#include "../Prototype/ExternProtoDeclaration.h"
#include "../Prototype/ProtoDeclaration.h"

#include <Titania/String/to_string.h>
#include <Titania/Utility/Range.h>

#include <algorithm>
#include <random>

namespace titania {
namespace X3D {

static std::default_random_engine
random_engine (std::chrono::system_clock::now () .time_since_epoch () .count ());

const UnitIndex X3DExecutionContext::unitCategories = {
	std::make_pair ("angle",  UnitCategory::ANGLE),
	std::make_pair ("force",  UnitCategory::FORCE),
	std::make_pair ("length", UnitCategory::LENGTH),
	std::make_pair ("mass",   UnitCategory::MASS)

};

const UnitArray X3DExecutionContext::standardUnits = {
	Unit ("angle",  "radian",   1),
	Unit ("force",  "newton",   1),
	Unit ("length", "metre",    1),
	Unit ("mass",   "kilogram", 1)

};

X3DExecutionContext::X3DExecutionContext () :
	         X3DBaseNode (),
	            encoding ("X3D"),
	specificationVersion ("3.3"),
	   characterEncoding ("utf8"),
	             comment ("Titania"),
	             profile (),
	          components (),
	               units (standardUnits),
	          namedNodes (),
	       importedNodes (),
	       importedNames (),
	 importedNodesOutput (),
	          prototypes (),
	    prototypesOutput (),
	        externProtos (),
	  externProtosOutput (),
	              routes (),
	           rootNodes (new MFNode ()),
	  uninitializedNodes ()
{
	addType (X3DConstants::X3DExecutionContext);

	addChildren (importedNodesOutput,
	             prototypesOutput,
	             externProtosOutput,
	             uninitializedNodes);

	// Root nodes must be added and removed as/from child in the node that derives from X3DExecutionContext.
	getRootNodes () .setName ("rootNodes");
	getRootNodes () .addClones (1);
}

void
X3DExecutionContext::initialize ()
{
	uninitializedNodes .isTainted (true); // !!! Prevent generating events when protos add nodes.

	if (not isProtoDeclaration ())
		realize ();
}

void
X3DExecutionContext::realize ()
{
	if (getBrowser () -> makeCurrent ())
	{
		while (not uninitializedNodes .empty ())
		{
			for (const auto & uninitializedNode : MFNode (std::move (uninitializedNodes)))
				uninitializedNode -> setup ();
		}
	}
	//else
	//	throw Error <INVALID_OPERATION_TIMING> ("Couldn't realize nodes.");
}

VersionType
X3DExecutionContext::getVersion () const
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	static const std::map <std::string, VersionType> versions = {
		std::make_pair ("2.0", VRML_V2_0),
		std::make_pair ("3.0",  X3D_V3_0),
		std::make_pair ("3.1",  X3D_V3_1),
		std::make_pair ("3.2",  X3D_V3_2),
		std::make_pair ("3.3",  X3D_V3_3),
	};

	try
	{
		return versions .at (getSpecificationVersion ());
	}
	catch (const std::out_of_range &)
	{
		return LATEST_VERSION;
	}
}

void
X3DExecutionContext::updateUnit (const std::string & category, const std::string & name, const double conversion)
throw (Error <INVALID_NAME>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	const auto unitCategory = unitCategories .find (category);

	if (unitCategory not_eq unitCategories .end ())
	{
		units [size_t (unitCategory -> second)] = Unit (category, name, conversion);
		return;
	}

	throw Error <INVALID_NAME> ("Unkown base unit category '" + category + "'.");
}

SFNode
X3DExecutionContext::createNode (const std::string & name)
throw (Error <INVALID_NAME>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	const X3DBaseNode* const declaration = getBrowser () -> getSupportedNode (name);

	if (getProfile () or not getComponents () .empty ())
	{
		if (getProfile ())
		{
			try
			{
				profile -> getComponents () .rfind (declaration -> getComponentName ());

				return declaration -> create (this);
			}
			catch (const std::out_of_range &)
			{ }
		}

		try
		{
			getComponents () .rfind (declaration -> getComponentName ());

			return declaration -> create (this);
		}
		catch (const std::out_of_range &)
		{ }

		throw Error <INVALID_NAME> ("Node type '" + name + "' not supported by profile or component specification.");
	}

	return declaration -> create (this);
}

X3DPrototypeInstancePtr
X3DExecutionContext::createPrototypeInstance (const std::string & name)
throw (Error <INVALID_NAME>,
       Error <INVALID_X3D>,
       Error <INVALID_FIELD>,
       Error <INVALID_ACCESS_TYPE>,
       Error <URL_UNAVAILABLE>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	return findProtoDeclaration (name, AvailableType { }) -> createInstance (this);
}

// Named node handling

// DONE: A node may be part of more than one run-time name scope. A node shall be removed from a name scope when it is
// removed from the scene graph. See: http://www.web3d.org/files/specifications/19775-1/V3.3/Part01/concepts.html#Runtimenamescope

void
X3DExecutionContext::addNamedNode (const std::string & name, const SFNode & node)
throw (Error <NODE_IN_USE>,
       Error <IMPORTED_NODE>,
       Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	if (namedNodes .count (name))
		throw Error <NODE_IN_USE> ("Couldn't add named node: Node name '" + name + "' is already in use.");

	updateNamedNode (name, node);
}

void
X3DExecutionContext::updateNamedNode (const std::string & name, const SFNode & node)
throw (Error <IMPORTED_NODE>,
       Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	if (not node)
		throw Error <INVALID_NODE> ("Couldn't update named node: node IS NULL.");

	if (node -> getExecutionContext () not_eq this)
		throw Error <IMPORTED_NODE> ("Couldn't update named node: the node does not belong to this execution context.");

	if (name .empty ())
		throw Error <INVALID_NAME> ("Couldn't update named node: node name is empty.");

	// Remove named node.

	removeNamedNode (node -> getName ());
	removeNamedNode (name);

	// Update named node.

	node -> setName (name);

	auto & namedNode = namedNodes [name];
	namedNode = new NamedNode (this, node);

	namedNode .isTainted (true);
	namedNode .addParent (this);

	if (isInitialized ())
		namedNode -> setup ();
	else
		addUninitializedNode (namedNode);
}

void
X3DExecutionContext::removeNamedNode (const std::string & name)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	if (name .empty ())
		return;

	const auto namedNode = namedNodes .find (name);

	if (namedNode not_eq namedNodes .end ())
	{
		try
		{
			namedNode -> second -> getLocalNode () -> setName ("");
		}
		catch (const X3DError &)
		{ }

		namedNodes .erase (namedNode);
	}
}

SFNode
X3DExecutionContext::getNamedNode (const std::string & name) const
throw (Error <INVALID_NAME>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	const auto namedNode = namedNodes .find (name);

	if (namedNode not_eq namedNodes .end ())
		return namedNode -> second -> getLocalNode ();

	throw Error <INVALID_NAME> ("Named node '" + name + "' not found.");
}

/***
 *  Returns a name that is unique in this execution contentext.
 */
std::string
X3DExecutionContext::getUniqueName (std::string name) const
{
	RegEx::LastNumber_ .Replace ("", &name);

	std::string newName = name;
	size_t      i       = 64;

	for ( ; i;)
	{
		if (namedNodes .count (newName) or newName .empty ())
		{
			const auto min = i;
			std::uniform_int_distribution <size_t> random (min, i <<= 1);

			newName = name;
			newName += '_';
			newName += basic::to_string (random (random_engine));
		}
		else
			break;
	}
	
	return newName;
}

/***
 *  Returns a name that is unique in this execution contentext and in @a executionContext.
 */
std::string
X3DExecutionContext::getUniqueName (X3DExecutionContext* const executionContext, std::string name) const
{
	RegEx::LastNumber_ .Replace ("", &name);
	
	std::string newName = name;
	size_t      i       = 64;

	for ( ; i;)
	{
		if (namedNodes .count (newName) or executionContext -> namedNodes .count (newName) or newName .empty ())
		{
			const auto min = i;
			std::uniform_int_distribution <size_t> random (min, i <<= 1);

			newName = name;
			newName += '_';
			newName += basic::to_string (random (random_engine));
		}
		else
			break;
	}

	return newName;
}

// Imported nodes handling

const ImportedNodePtr &
X3DExecutionContext::addImportedNode (const InlinePtr & inlineNode, const std::string & exportedName, std::string importedName)
throw (Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <NODE_IN_USE>,
       Error <URL_UNAVAILABLE>,
       Error <NODE_NOT_AVAILABLE>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	if (importedNodes .count (importedName))
		throw Error <NODE_IN_USE> ("Couldn't add imported node: imported name '" + importedName + "' already in use.");

	return updateImportedNode (inlineNode, exportedName, importedName);
}

const ImportedNodePtr &
X3DExecutionContext::updateImportedNode (const InlinePtr & inlineNode, const std::string & exportedName, std::string importedName)
throw (Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <URL_UNAVAILABLE>,
       Error <NODE_NOT_AVAILABLE>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	if (not inlineNode)
		throw Error <INVALID_NODE> ("Couldn't update imported node: Inline node is NULL.");

	// We do not throw Error <IMPORTED_NODE> as X3DPrototypeInctances can be of type Inline.

	if (exportedName .empty ())
		throw Error <INVALID_NAME> ("Couldn't update imported node: exported node name is empty.");

	if (importedName .empty ())
		importedName = exportedName;

	// Update imported node.

	auto & importedNode = importedNodes [importedName];
	importedNode = new ImportedNode (this, inlineNode, exportedName, importedName);

	importedNode .isTainted (true);
	importedNode .addParent (this);

	if (isInitialized ())
		importedNode -> setup ();
	else
		addUninitializedNode (importedNode);

	// Add imported name.

	const auto exportedNode = importedNode -> getExportedNode ();

	importedNode -> disposed () .addInterest (this,
	                                          &X3DExecutionContext::removeImportedName,
	                                          importedNames .emplace (exportedNode -> getId (), importedName));

	importedNodesOutput = getCurrentTime ();

	return importedNode;
}

void
X3DExecutionContext::removeImportedNode (const std::string & importedName)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	importedNodes .erase (importedName);

	importedNodesOutput = getCurrentTime ();
}

void
X3DExecutionContext::removeImportedName (const ImportedNamesIndex::iterator & iter)
{
	importedNames .erase (iter);
}

SFNode
X3DExecutionContext::getImportedNode (const std::string & importedName) const
throw (Error <INVALID_NAME>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	const auto iter = importedNodes .find (importedName);

	if (iter not_eq importedNodes .end ())
		return iter -> second -> getExportedNode ();

	throw Error <INVALID_NAME> ("Imported node '" + importedName + "' not found.");
}

bool
X3DExecutionContext::isImportedNode (const SFNode & node) const
throw (Error <INVALID_NODE>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	if (not node)
		throw Error <INVALID_NODE> ("Node is NULL.");

	return importedNames .count (node -> getId ());
}

/***
 *  Returns a name that is unique in this execution contentext.
 */
std::string
X3DExecutionContext::getUniqueImportedName (const X3DExecutionContext* const executionContext, std::string importedName) const
{
	RegEx::LastNumber_ .Replace ("", &importedName);

	std::string newName = importedName;
	size_t      i       = 64;

	for ( ; i;)
	{
		if (importedNodes .count (newName) or executionContext -> importedNodes .count (newName) or newName .empty ())
		{
			const auto min = i;
			std::uniform_int_distribution <size_t> random (min, i <<= 1);

			newName = importedName;
			newName += '_';
			newName += basic::to_string (random (random_engine));
		}
		else
			break;
	}
	
	return newName;
}

SFNode
X3DExecutionContext::getLocalNode (const std::string & name) const
throw (Error <INVALID_NAME>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	try
	{
		return getNamedNode (name);
	}
	catch (const X3DError &)
	{
		try
		{
			return getImportedNode (name);
		}
		catch (const X3DError &)
		{
			throw Error <INVALID_NAME> ("Unknown named or imported node '" + name + "'.");
		}
	}
}

const std::string &
X3DExecutionContext::getLocalName (const SFNode & node) const
throw (Error <INVALID_NODE>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	if (not node)
		throw Error <INVALID_NODE> ("Couldn't get local name: node is NULL.");
	
	if (node -> getExecutionContext () == this)
		return node -> getName ();

	auto equalRange = importedNames .equal_range (node -> getId ());

	if (equalRange .first not_eq equalRange .second)
		return (-- equalRange .second) -> second;

	throw Error <INVALID_NODE> ("Couldn't get local name: node is shared.");
}

bool
X3DExecutionContext::isLocalNode (const SFNode & node) const
throw (Error <INVALID_NODE>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	if (not node)
		throw Error <INVALID_NODE> ("Node is NULL.");

	if (node -> getExecutionContext () == this)
		return true;

	if (isImportedNode (node))
		return true;

	return false;
}

//	Proto declaration handling

ProtoDeclarationPtr
X3DExecutionContext::createProtoDeclaration (const std::string & name, const FieldDefinitionArray & interfaceDeclarations)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	if (name .empty ())
		throw Error <INVALID_NAME> ("Couldn't create proto declaration: proto name is empty.");

	const ProtoDeclarationPtr prototype = new ProtoDeclaration (this);

	prototype -> setName (name);

	for (const auto & field : interfaceDeclarations)
	{
		prototype -> addUserDefinedField (field -> getAccessType (),
		                                  field -> getName (),
		                                  field);
	}

	if (isInitialized ())
		prototype -> setup ();
	else
		addUninitializedNode (prototype);

	return prototype;
}

const ProtoDeclarationPtr &
X3DExecutionContext::addProtoDeclaration (const std::string & name, const ProtoDeclarationPtr & prototype)
throw (Error <INVALID_NAME>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	if (name .empty ())
		throw Error <INVALID_NAME> ("Couldn't add proto declaration: proto name is empty.");
	
	if (prototypes .count (name))
		throw Error <INVALID_NAME> ("Couldn't add proto declaration: proto '" + name + "' is already in use.");

	prototypes .push_back (name, prototype);
	prototypes .back () .isTainted (true);
	prototypes .back () .addParent (this);

	prototype -> setName (name);

	prototypesOutput = getCurrentTime ();

	return prototypes .back ();
}

void
X3DExecutionContext::updateProtoDeclaration (const std::string & name, const ProtoDeclarationPtr & prototype)
throw (Error <INVALID_NAME>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	if (name .empty ())
		throw Error <INVALID_NAME> ("Couldn't update proto declaration: proto name is empty.");

	try
	{
		if (prototypes .rfind (prototype -> getName ()) == prototype)
			prototypes .remap (prototype -> getName (), name);
	}
	catch (const std::out_of_range &)
	{
		try
		{
			prototypes .rfind (name) = prototype;
		}
		catch (const std::out_of_range &)
		{
			prototypes .push_back (name, prototype);
			prototypes .back () .isTainted (true);
			prototypes .back () .addParent (this);
		}
	}

	prototype -> setName (name);

	prototypesOutput = getCurrentTime ();
}

void
X3DExecutionContext::removeProtoDeclaration (const std::string & name)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	prototypes .erase (name);

	prototypesOutput = getCurrentTime ();
}

const ProtoDeclarationPtr &
X3DExecutionContext::getProtoDeclaration (const std::string & name)
throw (Error <INVALID_NAME>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	try
	{
		return prototypes .rfind (name);
	}
	catch (const std::out_of_range &)
	{
		throw Error <INVALID_NAME> ("PROTO '" + name + "' not found.");
	}
}

//	ExternProto declaration handling

ExternProtoDeclarationPtr
X3DExecutionContext::createExternProtoDeclaration (const std::string & name, const FieldDefinitionArray & externInterfaceDeclarations, const MFString & URLList)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	const ExternProtoDeclarationPtr externProto = new ExternProtoDeclaration (this);

	externProto -> setName (name);

	for (const auto & field : externInterfaceDeclarations)
	{
		externProto -> addUserDefinedField (field -> getAccessType (),
		                                    field -> getName (),
		                                    field);
	}

	externProto -> url () = URLList;
	externProto -> setup ();

	return externProto;
}

const ExternProtoDeclarationPtr &
X3DExecutionContext::addExternProtoDeclaration (const std::string & name, const ExternProtoDeclarationPtr & externProto)
throw (Error <INVALID_NAME>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	if (name .empty ())
		throw Error <INVALID_NAME> ("Couldn't add proto declaration: proto name is empty.");

	if (externProtos .count (name))
		throw Error <INVALID_NAME> ("Couldn't add extern proto declaration: extern proto '" + name + "' is already in use.");

	externProtos .push_back (name, externProto);
	externProtos .back () .isTainted (true);
	externProtos .back () .addParent (this);

	externProto -> setName (name);

	externProtosOutput = getCurrentTime ();

	return externProtos .back ();
}

void
X3DExecutionContext::updateExternProtoDeclaration (const std::string & name, const ExternProtoDeclarationPtr & externProto)
throw (Error <INVALID_NAME>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	if (name .empty ())
		throw Error <INVALID_NAME> ("Couldn't update proto declaration: proto name is empty.");

	try
	{
		if (externProtos .rfind (externProto -> getName ()) == externProto)
			externProtos .remap (externProto -> getName (), name);
	}
	catch (const std::out_of_range &)
	{
		try
		{
			externProtos .rfind (name) = externProto;
		}
		catch (const std::out_of_range &)
		{
			externProtos .push_back (name, externProto);
			externProtos .back () .isTainted (true);
			externProtos .back () .addParent (this);
		}
	}

	externProto -> setName (name);

	externProtosOutput = getCurrentTime ();
}

void
X3DExecutionContext::removeExternProtoDeclaration (const std::string & name)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	externProtos .erase (name);

	externProtosOutput = getCurrentTime ();
}

const
ExternProtoDeclarationPtr &
X3DExecutionContext::getExternProtoDeclaration (const std::string & name)
throw (Error <INVALID_NAME>,
       Error <URL_UNAVAILABLE>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	try
	{
		return externProtos .rfind (name);
	}
	catch (const std::out_of_range &)
	{
		throw Error <INVALID_NAME> ("EXTERNPROTO '" + name + "' not found.");
	}
}

// ProtoObject handling

X3DProtoDeclarationNode*
X3DExecutionContext::findProtoDeclaration (const std::string & name, const AvailableType & available) const
throw (Error <INVALID_NAME>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	try
	{
		return prototypes .rfind (name) .getValue ();
	}
	catch (const std::out_of_range &)
	{
		try
		{
			return externProtos .rfind (name) .getValue ();
		}
		catch (const std::out_of_range &)
		{
			if (not isRootContext ())
				return getExecutionContext () -> findProtoDeclaration (name, available);

			throw Error <INVALID_NAME> ("Unknown proto or externproto type '" + name + "'.");
		}
	}
}

X3DProtoDeclarationNode*
X3DExecutionContext::findProtoDeclaration (const std::string & name) const
throw (Error <INVALID_NAME>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	const auto protoNodes = findProtoDeclarations ();
	const auto iter       = protoNodes .find (name);
	
	if (iter not_eq protoNodes .end ())
		return iter -> second;

	throw Error <INVALID_NAME> ("Unknown proto object '" + name + "'.");
}

std::map <std::string, X3DProtoDeclarationNode*>
X3DExecutionContext::findProtoDeclarations () const
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	const X3DExecutionContext* executionContext = this;

	// Find all available prototypes

	std::map <std::string, X3DProtoDeclarationNode*> prototypes;
	std::string                                      current;

	for (;;)
	{
		// Skip all prototypes that are below a current prototype.

		bool skip = true;

		for (const auto & prototype : basic::make_reverse_range (executionContext -> getProtoDeclarations ()))
		{
			if (skip and not current .empty ())
			{
				if (current == prototype -> getName ())
					skip = false;

				continue;
			}

			prototypes .emplace (prototype -> getName (), prototype);
		}

		for (const auto & prototype : executionContext -> getExternProtoDeclarations ())
			prototypes .emplace (prototype -> getName (), prototype);

		if (executionContext -> isRootContext ())
			break;

		if (executionContext -> isProtoDeclaration ())
			current = executionContext -> getName ();
		else
			current .clear ();

		current = executionContext -> isProtoDeclaration () ? executionContext -> getName () : "";

		executionContext = executionContext -> getExecutionContext ();
	}

	return prototypes;
}

//	Dynamic route handling

const RoutePtr &
X3DExecutionContext::addRoute (const SFNode & sourceNode,      const std::string & sourceFieldId,
                               const SFNode & destinationNode, const std::string & destinationFieldId)
throw (Error <INVALID_NODE>,
       Error <INVALID_FIELD>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	const auto routeKey = getRouteId (sourceNode, sourceFieldId, destinationNode, destinationFieldId);

	try
	{
		// Silently return if route already exists.

		return routes .rfind (routeKey);
	}
	catch (const std::out_of_range &)
	{
		// Add route.

		auto & route = addRoute (new Route (this, sourceNode, routeKey .first, destinationNode, routeKey .second));

		if (isInitialized ())
			route -> setup ();
		else
			addUninitializedNode (route);

		return route;
	}
}

const RoutePtr &
X3DExecutionContext::addRoute (Route* const value)
throw (Error <INVALID_NODE>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	if (not value)
		throw Error <INVALID_NODE> ("Bad ROUTE specification: route is NULL in deleteRoute.");

	if (value -> getExecutionContext () not_eq this)
		throw Error <INVALID_NODE> ("Bad ROUTE specification: route does not belong to this execution context.");

	routes .push_back (value -> getKey (), value);

	auto & route = routes .back ();

	route .isTainted (true);
	route .addParent (this);

	return route;
}

void
X3DExecutionContext::deleteRoute (const SFNode & sourceNode,      const std::string & sourceFieldId,
                                  const SFNode & destinationNode, const std::string & destinationFieldId)
throw (Error <INVALID_NODE>,
       Error <INVALID_FIELD>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	try
	{
		const auto routeKey = getRouteId (sourceNode, sourceFieldId, destinationNode, destinationFieldId);

		routes .rfind (routeKey) -> disconnect ();
		routes .erase (routeKey);
	}
	catch (const std::out_of_range &)
	{
		// Silently return if route not exists.
	}
}

void
X3DExecutionContext::deleteRoute (Route* const route)
throw (Error <INVALID_NODE>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	if (not route)
		throw Error <INVALID_NODE> ("Bad ROUTE specification: route is NULL in deleteRoute.");

	if (route -> getExecutionContext () not_eq this)
		throw Error <INVALID_NODE> ("Bad ROUTE specification: route does not belong to this execution context.");

	const auto routeKey = route -> getKey ();

	route -> disconnect ();
	routes .erase (routeKey);
}

RouteId
X3DExecutionContext::getRouteId (const SFNode & sourceNode,      const std::string & sourceFieldId,
                                 const SFNode & destinationNode, const std::string & destinationFieldId)
throw (Error <INVALID_NODE>,
       Error <INVALID_FIELD>)
{
	if (not sourceNode .getValue ())
		throw Error <INVALID_NODE> ("Bad ROUTE specification: source node is NULL.");

	if (not destinationNode .getValue ())
		throw Error <INVALID_NODE> ("Bad ROUTE specification: destination node is NULL.");

	X3DFieldDefinition* sourceField = nullptr;

	try
	{
		sourceField = sourceNode -> getField (sourceFieldId);

		if (not sourceField -> isOutput ())
			throw Error <INVALID_FIELD> ("Bad ROUTE specification: Field named '" + sourceField -> getName () + "' in node named '" + sourceNode -> getName () + "' of type " + sourceNode -> getTypeName () + " is not an output field.");
	}
	catch (const Error <INVALID_NAME> &)
	{
		throw Error <INVALID_FIELD> ("Bad ROUTE specification: Unknown eventOut '" + sourceFieldId + "' in node named '" + sourceNode -> getName () + "' of type " + sourceNode -> getTypeName () + ".");
	}

	X3DFieldDefinition* destinationField = nullptr;

	try
	{
		destinationField = destinationNode -> getField (destinationFieldId);

		if (not destinationField -> isInput ())
			throw Error <INVALID_FIELD> ("Bad ROUTE specification: Field named '" + destinationField -> getName () + "' in node named '" + destinationNode -> getName () + "' of type " + destinationNode -> getTypeName () + " is not an input field.");
	}
	catch (const Error <INVALID_NAME> &)
	{
		throw Error <INVALID_FIELD> ("Bad ROUTE specification: Unknown eventIn '" + destinationFieldId + "' in node named '" + destinationNode -> getName () + "' of type " + destinationNode -> getTypeName () + ".");
	}

	if (sourceField -> getType () not_eq destinationField -> getType ())
		throw Error <INVALID_FIELD> ("ROUTE types " + sourceField -> getTypeName () + " and " + destinationField -> getTypeName () + " do not match.");

	return std::make_pair (sourceField, destinationField);
}

void
X3DExecutionContext::changeViewpoint (const std::string & name)
throw (Error <INVALID_NAME>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	try
	{
		const auto namedNode = getNamedNode (name);
		const auto viewpoint = x3d_cast <X3DViewpointNode*> (namedNode);

		if (not viewpoint)
			throw Error <INVALID_NAME> ("Warning: Node named '" + name + "' is not a viewpoint node.");

		if (viewpoint -> isBound ())
			viewpoint -> transitionStart (viewpoint);

		else
			viewpoint -> set_bind () = true;
	}
	catch (const X3DError & error)
	{
		if (not isRootContext ())
			getExecutionContext () -> changeViewpoint (name);
		else
			throw;
	}
}

// Import handling

void
X3DExecutionContext::import (X3DExecutionContext* const executionContext, MFNode & field)
throw (Error <INVALID_NAME>,
	    Error <NOT_SUPPORTED>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	if (getBrowser () -> makeCurrent ())
	{
		if (executionContext -> getProfile () or not executionContext -> getComponents () .empty ())
		{
			if (getProfile ())
			{
				if (executionContext -> getProfile ())
				{
					for (const auto & component : executionContext -> getProfile () -> getComponents ())
						addComponent (component);
				}

				for (const auto & component : executionContext -> getComponents ())
					addComponent (component);
			}
			else
				setProfile (getBrowser () -> getProfile ("Full"));
		}

		updateNamedNodes (executionContext);

		importNodes (executionContext);
		field .append (std::move (executionContext -> getRootNodes ()));

		importNamedNodes (executionContext);
		importExternProtos (executionContext);
		importProtos (executionContext);

		updateImportedNodes (executionContext);
		copyImportedNodes (executionContext);
		importRoutes (executionContext);

		realize ();
		return;
	}

	throw Error <INVALID_OPERATION_TIMING> ("Invalid operation timing.");
}

void
X3DExecutionContext::updateNamedNodes (X3DExecutionContext* const executionContext) const
throw (Error <IMPORTED_NODE>,
       Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	for (const auto & node : NamedNodeIndex (executionContext -> getNamedNodes ()))
	{
		executionContext -> removeNamedNode (node .first);
		executionContext -> updateNamedNode (getUniqueName (executionContext, node .first), node .second -> getLocalNode ());
	}
}

void
X3DExecutionContext::updateImportedNodes (X3DExecutionContext* const executionContext) const
throw (Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <URL_UNAVAILABLE>,
       Error <NODE_NOT_AVAILABLE>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	for (const auto & pair : ImportedNodeIndex (executionContext -> getImportedNodes ()))
	{
		const auto & importedNode       = pair .second;
		const auto   uniqueImportedName = getUniqueImportedName (executionContext, importedNode -> getImportedName ());

		executionContext -> updateImportedNode (importedNode -> getInlineNode (), importedNode -> getExportedName (), uniqueImportedName);

		if (uniqueImportedName not_eq importedNode -> getImportedName ())
			executionContext -> removeImportedNode (importedNode -> getImportedName ());
	}
}

void
X3DExecutionContext::importNodes (const X3DExecutionContext* const executionContext)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	for (auto & parent : ChildObjectSet (executionContext -> getParents ()))
	{
		const auto node = dynamic_cast <X3DBaseNode*> (parent);
		
		if (not node)
			continue;

		if (node == node -> getExecutionContext ())
			continue;

		node -> setExecutionContext (this);
	}
}

void
X3DExecutionContext::importNamedNodes (const X3DExecutionContext* const executionContext)
throw (Error <IMPORTED_NODE>,
       Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	for (const auto & node : executionContext -> getNamedNodes ())
		updateNamedNode (node .first, node .second -> getLocalNode ());
}

void
X3DExecutionContext::importRoutes (X3DExecutionContext* const executionContext)
{
	for (const auto & route : executionContext -> routes)
	  addRoute (route);

	executionContext -> routes .clear ();
}

// X3DProtoInstance import handling

void
X3DExecutionContext::importExternProtos (const X3DExecutionContext* const executionContext)
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>)
{
	for (const auto & externProto : executionContext -> getExternProtoDeclarations ())
		externProto -> copy (this, CLONE);
}

void
X3DExecutionContext::importProtos (const X3DExecutionContext* const executionContext)
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>)
{
	for (const auto & proto : executionContext -> getProtoDeclarations ())
		proto -> copy (this, CLONE);
}

void
X3DExecutionContext::copyRootNodes (const X3DExecutionContext* const executionContext)
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>)
{
	for (const auto & rootNode : executionContext -> getRootNodes ())
	{
		if (rootNode)
			getRootNodes () .emplace_back (rootNode -> copy (this, COPY_OR_CLONE));

		else
			getRootNodes () .emplace_back ();
	}
}

void
X3DExecutionContext::copyImportedNodes (const X3DExecutionContext* const executionContext)
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>)
{
	for (const auto & importedNode : executionContext -> getImportedNodes ())
		importedNode .second -> copy (this, COPY_OR_CLONE);
}

void
X3DExecutionContext::copyRoutes (const X3DExecutionContext* const executionContext)
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>)
{
	for (const auto & route : executionContext -> getRoutes ())
		route -> copy (this, COPY_OR_CLONE);
}

void
X3DExecutionContext::toStream (std::ostream & ostream) const
{
	ostream .imbue (std::locale::classic ());

	Generator::PushExecutionContext (this);
	Generator::PushContext ();
	Generator::setImportedNodes (getImportedNodes ());

	for (const auto & externProto : getExternProtoDeclarations ())
	{
		ostream
			<< externProto
			<< Generator::TidyBreak
			<< Generator::TidyBreak;
	}

	for (const auto & proto : getProtoDeclarations ())
	{
		ostream
			<< proto
			<< Generator::TidyBreak
			<< Generator::TidyBreak;
	}

	if (not getRootNodes () .empty ())
	{
		for (const auto & rootNode : std::make_pair (getRootNodes () .cbegin (), getRootNodes () .cend () - 1))
		{
			ostream
				<< Generator::Indent
				<< rootNode
				<< Generator::TidyBreak
				<< Generator::TidyBreak;
		}

		ostream
			<< Generator::Indent
			<< getRootNodes () .back ()
			<< Generator::TidyBreak;
	}

	if (not getImportedNodes () .empty ())
	{
		ostream << Generator::TidyBreak;

		for (const auto & importedNode : getImportedNodes ())
		{
			try
			{
				ostream << importedNode .second;
			}
			catch (const X3DError &)
			{ }
		}
	}

	if (not getRoutes () .empty ())
	{
		ostream << Generator::TidyBreak;

		for (const auto & route : getRoutes ())
		{
			try
			{
				ostream
					<< route
					<< Generator::Break;
			}
			catch (const X3DError & error)
			{ }
		}
	}

	if (not getInnerComments () .empty () and not isRootContext ())
	{
		ostream << Generator::TidyBreak;

		for (const auto & comment : getInnerComments ())
		{
			ostream
				<< Generator::Indent
				<< Generator::Comment
				<< comment
				<< Generator::ForceBreak;
		}
	}

	Generator::PopContext ();
	Generator::PopExecutionContext ();
}

void
X3DExecutionContext::toXMLStream (std::ostream & ostream) const
{
	ostream .imbue (std::locale::classic ());

	Generator::PushExecutionContext (this);
	Generator::PushContext ();
	Generator::setImportedNodes (getImportedNodes ());

	for (const auto & externProto : getExternProtoDeclarations ())
	{
		ostream
			<< XMLEncode (externProto)
			<< Generator::Break;
	}

	for (const auto & proto : getProtoDeclarations ())
	{
		ostream
			<< XMLEncode (proto)
			<< Generator::Break;
	}

	if (not getRootNodes () .empty ())
	{
		ostream
			<< XMLEncode (getRootNodes ())
			<< Generator::Break;
	}

	for (const auto & importedNode : getImportedNodes ())
	{
		try
		{
			ostream
				<< XMLEncode (importedNode .second)
				<< Generator::Break;
		}
		catch (const X3DError &)
		{ }
	}

	for (const auto & route : getRoutes ())
	{
		try
		{
			ostream
				<< XMLEncode (route)
				<< Generator::Break;
		}
		catch (const X3DError &)
		{ }
	}

	Generator::PopContext ();
	Generator::PopExecutionContext ();
}

void
X3DExecutionContext::dispose ()
{
	namedNodes    .clear ();
	importedNodes .clear ();
	importedNames .clear ();
	prototypes    .clear ();
	externProtos  .clear ();
	routes        .clear ();
}

X3DExecutionContext::~X3DExecutionContext ()
{ }

} // X3D
} // titania
