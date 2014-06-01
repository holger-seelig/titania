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

#include "pbParser.h"

#include "../../Execution/X3DExecutionContext.h"

// Grammar

#include <Titania/InputOutput.h>

namespace titania {
namespace X3D {
namespace peaseblossom {

class Grammar
{
public:

	///  @name General
	static const io::sequence whitespaces;

	// Keywords

	static const io::string _false;
	static const io::string _null;
	static const io::string _true;

	static const std::set <std::string> Keyword;
	static const std::set <std::string> FutureReservedWord;
	
	// Operator
	static const io::character Multiplication;
	static const io::character Division;
	static const io::character Remainder;
	static const io::character Addition;
	static const io::character Subtraction;
	static const io::string    LeftShift;
	static const io::string    RightShift;
	static const io::string    UnsignedRightShift;
	static const io::character Less;
	static const io::character Greater;
	static const io::string    LessEqual;
	static const io::string    GreaterEqual;
	static const io::string    instanceof;
	static const io::string    in;
	static const io::string    Equal;
	static const io::string    NotEqual;
	static const io::string    StrictEqual;
	static const io::string    StrictNotEqual;
	static const io::character BitwiseAND;
	static const io::character BitwiseXOR;
	static const io::character BitwiseOR;
	static const io::string    LogicalAND;
	static const io::string    LogicalOR;
	static const io::character QuestionMark;
	static const io::character Colon;
	static const io::character Assignment;
	static const io::string    MultiplicationAssigment;
	static const io::string    DivisionAssignment;
	static const io::string    RemainderAssignment;
	static const io::string    AdditionAssignment;
	static const io::string    SubtractionAssignment;
	static const io::string    LeftShiftAssignment;
	static const io::string    RightShiftAssignment;
	static const io::string    UnsignedRightShiftAssignment;
	static const io::string    BitwiseANDAssignment;
	static const io::string    BitwiseXORAssignment;
	static const io::string    BitwiseORAssignment;

	///  @name Values
	static const io::string hex;
	static const io::string HEX;

};

// General
const io::sequence Grammar::whitespaces ("\r\n \t");

// Keywords
const io::string Grammar::_false ("false");
const io::string Grammar::_null ("null");
const io::string Grammar::_true ("true");

const std::set <std::string> Grammar::Keyword = {
	"break",    "do",       "instanceof", "typeof",
	"case",     "else",     "new",        "var",
	"catch",    "finally",  "return",     "void",
	"continue", "for",      "switch",     "while",
	"debugger", "function", "this",       "with",
	"default",  "if",       "throw",
	"delete",   "in",       "try"

};

const std::set <std::string> Grammar::FutureReservedWord = {
	"class", "enum",   "extends", "super",
	"const", "export", "import"

};
	
// Operator
const io::character Grammar::Multiplication ('*');
const io::character Grammar::Division ('/');
const io::character Grammar::Remainder ('%');
const io::character Grammar::Addition ('+');
const io::character Grammar::Subtraction ('-');
const io::string    Grammar::LeftShift ("<<");
const io::string    Grammar::RightShift (">>");
const io::string    Grammar::UnsignedRightShift (">>>");
const io::character Grammar::Less ('<');
const io::character Grammar::Greater ('>');
const io::string    Grammar::LessEqual ("<=");
const io::string    Grammar::GreaterEqual (">=");
const io::string    Grammar::instanceof ("instanceof");
const io::string    Grammar::in ("in");
const io::string    Grammar::Equal ("==");
const io::string    Grammar::NotEqual ("!=");
const io::string    Grammar::StrictEqual ("===");
const io::string    Grammar::StrictNotEqual ("!==");
const io::character Grammar::BitwiseAND ('&');
const io::character Grammar::BitwiseXOR ('^');
const io::character Grammar::BitwiseOR ('|');
const io::string    Grammar::LogicalAND ("&&");
const io::string    Grammar::LogicalOR ("||");
const io::character Grammar::QuestionMark ('?');
const io::character Grammar::Colon (':');
const io::character Grammar::Assignment ('=');
const io::string    Grammar::MultiplicationAssigment ("*=");
const io::string    Grammar::DivisionAssignment ("/=");
const io::string    Grammar::RemainderAssignment ("%=");
const io::string    Grammar::AdditionAssignment ("+=");
const io::string    Grammar::SubtractionAssignment ("-=");
const io::string    Grammar::LeftShiftAssignment ("<<=");
const io::string    Grammar::RightShiftAssignment (">>=");
const io::string    Grammar::UnsignedRightShiftAssignment (">>>=");
const io::string    Grammar::BitwiseANDAssignment ("&=");
const io::string    Grammar::BitwiseXORAssignment ("^=");
const io::string    Grammar::BitwiseORAssignment ("|=");

// Values
const io::string Grammar::hex ("0x");
const io::string Grammar::HEX ("0X");

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const std::string Parser::componentName  = "PeaseBlossom";
const std::string Parser::typeName       = "Parser";
const std::string Parser::containerField = "parser";

Parser::Parser (std::istream & istream, X3DExecutionContext* const executionContext) :
	X3DBaseNode (executionContext -> getBrowser (), executionContext),
	  X3DParser (),
	    istream (istream),
	whitespaces ()
{ }

void
Parser::parseIntoContext ()
{
	//__LOG__ << std::endl;

	try
	{
		program ();
	}
	catch (const X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
Parser::comments ()
{
	Grammar::whitespaces (istream, whitespaces);
}

// A.1 Lexical Grammar

bool
Parser::identifier ()
{
	//__LOG__ << std::endl;

	std::string _identifierName;

	if (identifierName (_identifierName))
	{
		if (reservedWord (_identifierName))
			throw Error <INVALID_X3D> ("reservedWord");

		return true;
	}

	return false;
}

bool
Parser::identifierName (std::string & _identifierName)
{
	//__LOG__ << std::endl;

	// ...

	return false;
}

bool
Parser::reservedWord (const std::string & _string)
{
	//__LOG__ << std::endl;

	if (Grammar::Keyword .count (_string))
		return true;

	if (Grammar::FutureReservedWord .count (_string))
		return true;

	std::istringstream istream (_string);

	if (nullLiteral (istream))
		return true;

	if (booleanLiteral (istream))
		return true;

	return false;
}

bool
Parser::literal ()
{
	//__LOG__ << std::endl;

	if (nullLiteral (istream))
		return true;

	if (booleanLiteral (istream))
		return true;

	if (numericLiteral ())
		return true;

	// ...

	return false;
}

bool
Parser::nullLiteral (std::istream & istream /*, Value & _value*/)
{
	//__LOG__ << std::endl;

	comments ();

	if (Grammar::_null (istream))
		return true;

	return false;
}

bool
Parser::booleanLiteral (std::istream & istream /*, Value & _value*/)
{
	//__LOG__ << std::endl;

	comments ();

	if (Grammar::_true (istream))
		return true;

	if (Grammar::_false (istream))
		return true;

	return false;
}

bool
Parser::numericLiteral (/*Value & _value*/)
{
	//__LOG__ << std::endl;

	if (hexIntegerLiteral ())
		return true;

	if (decimalLiteral ())
		return true;

	return false;
}

bool
Parser::decimalLiteral (/*Value & _value*/)
{
	//__LOG__ << std::endl;

	comments ();

	double _value;

	if (istream >> _value)
		return true;

	istream .clear ();

	return false;
}

bool
Parser::hexIntegerLiteral (/*Value & _value*/)
{
	//__LOG__ << std::endl;

	comments ();

	if (Grammar::hex (istream) or Grammar::HEX (istream))
	{
		uint32_t _value;

		if (istream >> std::hex >> _value)
			return true;

		istream .clear ();
	}

	return false;
}

// A.2 Number Conversions

// A.3 Expressions

bool
Parser::primaryExpression ()
{
	//__LOG__ << std::endl;

	// ...

	if (identifier ())
		return true;

	if (literal ())
		return true;

	// ...

	return false;
}

bool
Parser::memberExpression ()
{
	//__LOG__ << std::endl;

	if (primaryExpression ())
		return true;

	// ...

	return false;
}

bool
Parser::newExpression ()
{
	//__LOG__ << std::endl;

	if (memberExpression ())
		return true;

	// ...

	return false;
}

bool
Parser::leftHandSideExpression ()
{
	//__LOG__ << std::endl;

	if (newExpression ())
		return true;

	//if (callExpression ())
	//	return true;

	return false;
}

bool
Parser::postfixExpression ()
{
	//__LOG__ << std::endl;

	if (leftHandSideExpression ())
		return true;

	// ...

	return false;
}

bool
Parser::unaryExpression ()
{
	//__LOG__ << std::endl;

	if (postfixExpression ())
		return true;

	// ...

	return false;
}

bool
Parser::multiplicativeExpression ()
{
	//__LOG__ << std::endl;

	if (unaryExpression ())
	{
		for (;;)
		{
			comments ();
		
			if (Grammar::Multiplication (istream))
			{
				if (unaryExpression ())
				{
					continue;
				}

				throw Error <INVALID_X3D> ("multiplicativeExpression");
			}

			if (Grammar::Division (istream))
			{
				if (unaryExpression ())
				{
					continue;
				}

				throw Error <INVALID_X3D> ("multiplicativeExpression");
			}

			if (Grammar::Remainder (istream))
			{
				if (unaryExpression ())
				{
					continue;
				}

				throw Error <INVALID_X3D> ("multiplicativeExpression");
			}

			return true;
		}
	}

	return false;
}

bool
Parser::additiveExpression ()
{
	//__LOG__ << std::endl;

	if (multiplicativeExpression ())
	{
		for (;;)
		{
			comments ();
		
			if (Grammar::Addition (istream))
			{
				if (multiplicativeExpression ())
				{
					continue;
				}

				throw Error <INVALID_X3D> ("additiveExpression");
			}

			if (Grammar::Subtraction (istream))
			{
				if (multiplicativeExpression ())
				{
					continue;
				}

				throw Error <INVALID_X3D> ("additiveExpression");
			}

			return true;
		}
	}

	return false;
}

bool
Parser::shiftExpression ()
{
	//__LOG__ << std::endl;

	if (additiveExpression ())
	{
		for (;;)
		{
			comments ();
		
			if (Grammar::LeftShift (istream))
			{
				if (additiveExpression ())
				{
					continue;
				}

				throw Error <INVALID_X3D> ("shiftExpression");
			}

			if (Grammar::UnsignedRightShift (istream))
			{
				if (additiveExpression ())
				{
					continue;
				}

				throw Error <INVALID_X3D> ("shiftExpression");
			}

			if (Grammar::RightShift (istream))
			{
				if (additiveExpression ())
				{
					continue;
				}

				throw Error <INVALID_X3D> ("shiftExpression");
			}

			return true;
		}
	}

	return false;
}

bool
Parser::relationalExpression ()
{
	//__LOG__ << std::endl;

	if (shiftExpression ())
	{
		for (;;)
		{
			comments ();

			if (Grammar::LessEqual (istream))
			{
				if (shiftExpression ())
				{
					continue;
				}

				throw Error <INVALID_X3D> ("relationalExpression");
			}

			if (Grammar::GreaterEqual (istream))
			{
				if (shiftExpression ())
				{
					continue;
				}

				throw Error <INVALID_X3D> ("relationalExpression");
			}

			if (Grammar::Less (istream))
			{
				if (shiftExpression ())
				{
					continue;
				}

				throw Error <INVALID_X3D> ("relationalExpression");
			}

			if (Grammar::Greater (istream))
			{
				if (shiftExpression ())
				{
					continue;
				}

				throw Error <INVALID_X3D> ("relationalExpression");
			}

			if (Grammar::instanceof (istream))
			{
				if (shiftExpression ())
				{
					continue;
				}

				throw Error <INVALID_X3D> ("relationalExpression");
			}

			if (Grammar::in (istream))
			{
				if (shiftExpression ())
				{
					continue;
				}

				throw Error <INVALID_X3D> ("relationalExpression");
			}

			return true;
		}
	}

	return false;
}

bool
Parser::equalityExpression ()
{
	//__LOG__ << std::endl;

	if (relationalExpression ())
	{
		for (;;)
		{
			comments ();

			if (Grammar::StrictEqual (istream))
			{
				if (relationalExpression ())
				{
					continue;
				}

				throw Error <INVALID_X3D> ("equalityExpression");
			}

			if (Grammar::StrictNotEqual (istream))
			{
				if (relationalExpression ())
				{
					continue;
				}

				throw Error <INVALID_X3D> ("equalityExpression");
			}

			if (Grammar::Equal (istream))
			{
				if (relationalExpression ())
				{
					continue;
				}

				throw Error <INVALID_X3D> ("equalityExpression");
			}

			if (Grammar::NotEqual (istream))
			{
				if (relationalExpression ())
				{
					continue;
				}

				throw Error <INVALID_X3D> ("equalityExpression");
			}

			return true;
		}
	}

	return false;
}

bool
Parser::bitwiseANDExpression ()
{
	//__LOG__ << std::endl;

	if (equalityExpression ())
	{
		for (;;)
		{
			comments ();
			
			if (Grammar::LogicalAND .test (istream))
				return true;

			if (Grammar::BitwiseAND (istream))
			{
				if (equalityExpression ())
				{
					continue;
				}

				throw Error <INVALID_X3D> ("bitwiseANDExpression");
			}

			return true;
		}
	}

	return false;
}

bool
Parser::bitwiseXORExpression ()
{
	//__LOG__ << std::endl;

	if (bitwiseANDExpression ())
	{
		for (;;)
		{
			comments ();
			
			if (Grammar::BitwiseXOR (istream))
			{
				if (bitwiseANDExpression ())
				{
					continue;
				}

				throw Error <INVALID_X3D> ("bitwiseXORExpression");
			}

			return true;
		}
	}

	return false;
}

bool
Parser::bitwiseORExpression ()
{
	//__LOG__ << std::endl;

	if (bitwiseXORExpression ())
	{
		for (;;)
		{
			comments ();

			if (Grammar::LogicalOR .test (istream))
				return true;
		
			if (Grammar::BitwiseOR (istream))
			{
				if (bitwiseXORExpression ())
				{
					continue;
				}

				throw Error <INVALID_X3D> ("bitwiseORExpression");
			}

			return true;
		}
	}

	return false;
}

bool
Parser::logicalANDExpression ()
{
	//__LOG__ << std::endl;

	if (bitwiseORExpression ())
	{
		for (;;)
		{
			comments ();

			if (Grammar::LogicalAND (istream))
			{
				if (bitwiseORExpression ())
				{
					continue;
				}

				throw Error <INVALID_X3D> ("logicalANDExpression");
			}

			return true;
		}
	}

	return false;
}

bool
Parser::logicalORExpression ()
{
	//__LOG__ << std::endl;

	if (logicalANDExpression ())
	{
		for (;;)
		{
			comments ();

			if (Grammar::LogicalOR (istream))
			{
				if (logicalANDExpression ())
				{
					continue;
				}

				throw Error <INVALID_X3D> ("logicalORExpression");
			}

			return true;
		}
	}

	return false;
}

bool
Parser::conditionalExpression ()
{
	//__LOG__ << std::endl;

	if (logicalORExpression ())
	{
		comments ();

		if (Grammar::QuestionMark (istream))
		{
			if (assignmentExpression ())
			{
				comments ();

				if (Grammar::Colon (istream))
				{
					if (assignmentExpression ())
					{
						return true;
					}
				}
			}

			throw Error <INVALID_X3D> ("conditionalExpression");
		}
	
		return true;
	}

	return false;
}

bool
Parser::assignmentExpression ()
{
	//__LOG__ << std::endl;

	if (conditionalExpression ())
		return true;

	// ...

	return false;
}

bool
Parser::expression ()
{
	//__LOG__ << std::endl;

	if (assignmentExpression ())
		return true;

	// ...

	return false;
}

// A.4 Statements

bool
Parser::statement ()
{
	//__LOG__ << std::endl;

	if (expressionStatement ())
		return true;

	return false;
}

bool
Parser::expressionStatement ()
{
	//__LOG__ << std::endl;

	// [lookahead ? {{, function}]

	if (expression ())
		return true;

	return false;
}

// A.5 Functions and Programs

bool
Parser::functionDeclaration ()
{
	//__LOG__ << std::endl;

	return false;
}

void
Parser::program ()
{
	sourceElements ();
}

void
Parser::sourceElements ()
{
	//__LOG__ << std::endl;

	while (sourceElement ())
		;
}

bool
Parser::sourceElement ()
{
	//__LOG__ << std::endl;

	if (statement ())
		return true;

	if (functionDeclaration ())
		return true;

	return false;
}

} // peaseblossom
} // X3D
} // titania
