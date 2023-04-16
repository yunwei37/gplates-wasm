/* $Id: ViolatedClassInvariantException.h 11211 2011-03-21 01:57:58Z mchin $ */

/**
 * \file 
 * File specific comments.
 *
 * Most recent change:
 *   $Date: 2011-03-20 18:57:58 -0700 (Sun, 20 Mar 2011) $
 * 
 * Copyright (C) 2004, 2005, 2006 The University of Sydney, Australia
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

#ifndef _GPLATES_MATHS_VIOLATEDCLASSINVARIANTEXCEPTION_H_
#define _GPLATES_MATHS_VIOLATEDCLASSINVARIANTEXCEPTION_H_

#include "MathematicalException.h"

namespace GPlatesMaths
{
	/**
	 * The Exception thrown when class invariants are violated.
	 */
	class ViolatedClassInvariantException
		: public MathematicalException
	{
		public:
			/**
			 * @param msg is a description of the conditions
			 * which cause the invariant to be violated.
			 */
			ViolatedClassInvariantException(
					const GPlatesUtils::CallStack::Trace &exception_source,
					const char *msg) :
				MathematicalException(exception_source),
				_msg(msg)
			{  }

			virtual
			~ViolatedClassInvariantException() throw() {  }

		protected:
			virtual const char *
			exception_name() const {

				return "ViolatedClassInvariantException";
			}

			virtual
			void
			write_message(
					std::ostream &os) const
			{
				write_string_message(os, _msg);
			}

		private:
			std::string _msg;
	};
}

#endif  // _GPLATES_MATHS_VIOLATEDCLASSINVARIANTEXCEPTION_H_
