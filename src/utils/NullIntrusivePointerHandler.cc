/* $Id: NullIntrusivePointerHandler.cc 11591 2011-05-18 19:48:15Z jcannon $ */

/**
 * \file
 * File specific comments.
 *
 * Most recent change:
 *   $Date: 2011-05-18 12:48:15 -0700 (Wed, 18 May 2011) $
 *
 * Copyright (C) 2008 The University of Sydney, Australia
 *
 * This file is part of GPlates.
 *
 * GPlates is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 2, as published by
 * the Free Software Foundation.
 *
 * GPlates is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "NullIntrusivePointerHandler.h"
#include "NullNonNullIntrusivePointerException.h"

#include "global/GPlatesAssert.h"


void
GPlatesUtils::NullIntrusivePointerHandler::operator()() const
{
#ifdef GPLATES_DEBUG
	// Abort in debug mode so that we can observe the stack trace in the debugger.
	GPlatesGlobal::Abort(GPLATES_ASSERTION_SOURCE);
#else
	throw NullNonNullIntrusivePointerException(GPLATES_EXCEPTION_SOURCE);
#endif
}
