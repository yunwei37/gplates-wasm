/* $Id: IndeterminateArcRotationAxisException.h 12448 2011-10-24 04:47:04Z mchin $ */

/**
 * \file 
 * File specific comments.
 *
 * Most recent change:
 *   $Date: 2011-10-23 21:47:04 -0700 (Sun, 23 Oct 2011) $
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

#ifndef GPLATES_MATHS_INDETERMINATEARCROTATIONAXISEXCEPTION_H
#define GPLATES_MATHS_INDETERMINATEARCROTATIONAXISEXCEPTION_H

#include "global/PreconditionViolationError.h"
#include "GreatCircleArc.h"


namespace GPlatesMaths
{
	/**
	 * This is the exception thrown when an attempt is made to access the rotation axis of a
	 * zero-length great-circle arc (which does not have a determinate rotation axis).
	 */
	class IndeterminateArcRotationAxisException:
			public GPlatesGlobal::PreconditionViolationError
	{
	public:
		IndeterminateArcRotationAxisException(
				const GPlatesUtils::CallStack::Trace &exception_source,
				const GreatCircleArc &arc_):
			GPlatesGlobal::PreconditionViolationError(exception_source),
			d_arc(arc_)
		{  }

		const GreatCircleArc &
		arc() const
		{
			return d_arc;
		}

		~IndeterminateArcRotationAxisException() throw(){}

	protected:
		virtual
		const char *
		exception_name() const
		{
			// FIXME:  This function should really be defined in a .cc file.
			return "IndeterminateArcRotationAxisException";
		}

		virtual
		void
		write_message(
				std::ostream &os) const
		{
			// FIXME:  This function should really be defined in a .cc file.
		}
	private:
		GreatCircleArc d_arc;
	};
}

#endif  // GPLATES_MATHS_INDETERMINATEARCROTATIONAXISEXCEPTION_H
