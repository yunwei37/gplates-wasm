/* $Id: PreconditionViolationError.h 11211 2011-03-21 01:57:58Z mchin $ */

/**
 * \file 
 * File specific comments.
 *
 * Most recent change:
 *   $Date: 2011-03-20 18:57:58 -0700 (Sun, 20 Mar 2011) $
 * 
 * Copyright (C) 2003, 2004, 2005, 2006, 2007 The University of Sydney, Australia
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

#ifndef GPLATES_GLOBAL_PRECONDITIONVIOLATIONERROR_H
#define GPLATES_GLOBAL_PRECONDITIONVIOLATIONERROR_H

#include "GPlatesException.h"


namespace GPlatesGlobal
{
	/**
	 * This is the base class of all exceptions in GPlates which are used to report that
	 * erroneous parameters were supplied to a function, violating the precondition of that
	 * function.
	 */
	class PreconditionViolationError:
			public Exception
	{
	public:
		explicit
			PreconditionViolationError(
			const GPlatesUtils::CallStack::Trace &exception_source) :
		Exception(exception_source)
		{  }

		~PreconditionViolationError() throw() { }

	protected:
		virtual
			const char *
			exception_name() const
		{
			return "PreconditionViolationError";
		}
	};
}

#endif  // GPLATES_GLOBAL_PRECONDITIONVIOLATIONERROR_H
