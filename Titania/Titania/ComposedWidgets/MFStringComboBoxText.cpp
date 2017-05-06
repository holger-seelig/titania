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

#include "MFStringComboBoxText.h"

namespace titania {
namespace puck {

MFStringComboBoxText::MFStringComboBoxText (X3DBaseInterface* const editor,
                                            Gtk::ComboBoxText & comboBoxText,
                                            const std::string & name,
                                            const size_t index,
                                            const std::string & defaultValue) :
	 X3DBaseInterface (editor -> getBrowserWindow (), editor -> getCurrentBrowser ()),
	X3DComposedWidget (editor),
	     comboBoxText (comboBoxText),
	            nodes (),
	             name (name),
	            index (index),
	     defaultValue (defaultValue),
	         undoStep (),
	         changing (false),
	           buffer ()
{
	addChildObjects (nodes, buffer);

	buffer .addInterest (&MFStringComboBoxText::set_buffer, this);

	comboBoxText .signal_changed () .connect (sigc::mem_fun (this, &MFStringComboBoxText::on_changed));
	setup ();
}

void
MFStringComboBoxText::setNodes (const X3D::MFNode & value)
{
	for (const auto & node : nodes)
	{
		try
		{
			node -> getField <X3D::MFString> (name) .removeInterest (&MFStringComboBoxText::set_field, this);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	nodes = value;

	for (const auto & node : nodes)
	{
		try
		{
			node -> getField <X3D::MFString> (name) .addInterest (&MFStringComboBoxText::set_field, this);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	set_field ();
}

void
MFStringComboBoxText::on_changed ()
{
	if (nodes .empty ())
		return;

	if (changing)
		return;

	addUndoFunction <X3D::MFString> (nodes, name, undoStep);

	for (const auto & node : nodes)
	{
		try
		{
			auto & field = node -> getField <X3D::MFString> (name);

			field .removeInterest (&MFStringComboBoxText::set_field, this);
			field .addInterest (&MFStringComboBoxText::connect, this);

			field .set1Value (index, comboBoxText .get_active_text ());
		}
		catch (const X3D::X3DError &)
		{ }
	}

	addRedoFunction <X3D::MFString> (nodes, name, undoStep);
}

void
MFStringComboBoxText::set_field ()
{
	buffer .addEvent ();
}

void
MFStringComboBoxText::set_buffer ()
{
	undoStep .reset ();

	changing = true;

	const auto pair = getString (nodes, name, index, defaultValue);

	if (pair .second > 0)
	{
		comboBoxText .set_active_text (pair .first);

		if (comboBoxText .get_active_row_number () < 0)
			comboBoxText .set_active_text (defaultValue);
	}
	else if (pair .second == 0)
		comboBoxText .set_active_text (defaultValue);
	else
		comboBoxText .set_active (-1);

	comboBoxText .set_sensitive (pair .second not_eq -2);

	changing = false;
}

void
MFStringComboBoxText::connect (const X3D::MFString & field)
{
	field .removeInterest (&MFStringComboBoxText::connect, this);
	field .addInterest (&MFStringComboBoxText::set_field, this);
}

} // puck
} // titania
