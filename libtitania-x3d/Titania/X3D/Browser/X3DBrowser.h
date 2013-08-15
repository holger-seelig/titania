// /* -*- Mode: C++; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*- */
// /*************************************************************************
//  *
//  * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
//  *
//  * Copyright 1999, 2012 Holger Seelig <holger.seelig@yahoo.de>.
//  *
//  * Titania - a multi-platform office productivity suite
//  *
//  * This file is part of the Titania Project.
//  *
//  * Titania is free software: you can redistribute it and/or modify
//  * it under the terms of the GNU Lesser General Public License version 3
//  * only, as published by the Free Software Foundation.
//  *
//  * Titania is distributed in the hope that it will be useful,
//  * but WITHOUT ANY WARRANTY; without even the implied warranty of
//  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  * GNU Lesser General Public License version 3 for more details
//  * (a copy is included in the LICENSE file that accompanied this code).
//  *
//  * You should have received a copy of the GNU Lesser General Public License
//  * version 3 along with Titania.  If not, see
//  * <http://www.gnu.org/licenses/lgpl.html>
//  * for a copy of the LGPLv3 License.
//  *
//  ************************************************************************/

#ifndef __TITANIA_X3D_BROWSER_X3DBROWSER_H__
#define __TITANIA_X3D_BROWSER_X3DBROWSER_H__

#include "../Browser/X3DBrowserContext.h"

#include "../Components/Networking/X3DUrlObject.h"
#include "../Configuration/SupportedComponents.h"
#include "../Configuration/SupportedFields.h"
#include "../Configuration/SupportedNodes.h"
#include "../Configuration/SupportedProfiles.h"
#include "../Execution/Scene.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace titania {
namespace X3D {

class X3DBrowser :
	public X3DBrowserContext, public X3DUrlObject
{
public:

	virtual
	X3DBrowser*
	getBrowser () const;

	const std::string &
	getVersion () const
	throw (Error <DISPOSED>)
	{ return version; }

	void
	setDescription (const std::string & value)
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>)
	{ description = value; }

	const SFString &
	getDescription ()
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>)
	{ return description; }

	virtual
	const basic::uri &
	getWorldURL () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) final;

	const FieldTypesArray &
	getSupportedFields () const
	throw (Error <DISPOSED>);

	const BasicNodeArray &
	getSupportedNodes () const
	throw (Error <DISPOSED>);

	const ComponentInfoArray &
	getSupportedComponents () const
	throw (Error <DISPOSED>);

	const ProfileInfoArray &
	getSupportedProfiles () const
	throw (Error <DISPOSED>);

	const X3DFieldDefinition*
	getFieldType (const std::string &) const
	throw (Error <INVALID_NAME>);

	const X3DBaseNode*
	getNode (const std::string &) const
	throw (Error <INVALID_NAME>);

	const ComponentInfo*
	getComponent (const std::string &, const size_t) const
	throw (Error <NOT_SUPPORTED>);

	const ProfileInfo*
	getProfile (const std::string &) const
	throw (Error <NOT_SUPPORTED>);

	const X3DSFNode <Scene> &
	getExecutionContext () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>)
	{ return scene; }

	X3DSFNode <Scene>
	createScene () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	X3DSFNode <Scene>
	createScene (const ProfileInfo*, const ComponentInfoArray &) const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	void
	replaceWorld (const X3DSFNode <Scene> &)
	throw (Error <INVALID_SCENE>);

	void
	loadURL (const MFString &)
	throw (Error <INVALID_URL>,
	       Error <URL_UNAVAILABLE>);

	virtual
	void
	loadURL (const MFString &, const MFString &)
	throw (Error <INVALID_URL>,
	       Error <URL_UNAVAILABLE>) final;

	X3DSFNode <Scene>
	importDocument (/*const XML DOMNode &*/)
	throw (Error <INVALID_DOCUMENT>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>,
	       Error <NOT_SUPPORTED>);

	void
	beginUpdate ()
	throw (Error <DISPOSED>);

	void
	endUpdate ()
	throw (Error <DISPOSED>);

	const X3DSFNode <RenderingProperties> &
	getRenderingProperties () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>)
	{ return renderingProperties; }

	const X3DSFNode <BrowserProperties> &
	getBrowserProperties () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>)
	{ return browserProperties; }

	//	bool
	//	setBrowserOption (SAIString, SAIObject)
	//	throw (Error <INVALID_OPERATION_TIMING>,
	//	       Error <DISPOSED>);

	const X3DSFNode <BrowserOptions> &
	getBrowserOptions () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>)
	{ return browserOptions; }

	void
	changeViewpoint (const std::string &)
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	//	void
	//	changeViewpoint (SAIAction)
	//	throw (Error <INVALID_OPERATION_TIMING>,
	//	       Error <DISPOSED>);
	//
	//	void
	//	changeViewpoint (SAIAction, int32_t SAILayerID)
	//	throw (Error <INVALID_OPERATION_TIMING>,
	//	       Error <DISPOSED>);

	///  @name print

	template <typename ... Args>
	void
	print (Args && ... args)
	throw (Error <DISPOSED>);

	template <typename ... Args>
	void
	println (Args && ... args)
	throw (Error <DISPOSED>)
	{ print (args ..., '\n'); }

	///  @name Destruction

	virtual
	void
	dispose () override;


protected:

	X3DBrowser ();

	virtual
	void
	initialize () override;

	virtual
	~X3DBrowser ();


private:

	virtual
	const X3DSFNode <World> &
	getWorld () const final
	{ return world; }

	template <typename First, typename ... Args>
	void
	print (std::ostringstream & stream, First && first, Args && ... args);

	void
	print (std::ostringstream &)
	{ }

	using X3DUrlObject::url;

	void
	set_scene ();

	static const std::string version;

	SupportedFields     supportedFields;
	SupportedNodes      supportedNodes;
	SupportedComponents supportedComponents;
	SupportedProfiles   supportedProfiles;

	SFString description;

	X3DSFNode <Scene> scene;
	X3DSFNode <World> world;

};

template <typename ... Args>
inline
void
X3DBrowser::print (Args && ... args)
throw (Error <DISPOSED>)
{
	std::ostringstream ostream;

	print (ostream, args ...);

	getConsole () -> set_string () .emplace_back (ostream .str ());

	std::clog << ostream .str ();
}

template <typename First, typename ... Args>
inline
void
X3DBrowser::print (std::ostringstream & ostream, First && first, Args && ... args)
{
	ostream << first;

	print (ostream, args ...);
}

} // X3D
} // titania

#endif
