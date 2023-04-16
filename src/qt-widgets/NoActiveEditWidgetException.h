/* $Id: NoActiveEditWidgetException.h 12448 2011-10-24 04:47:04Z mchin $ */

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

#ifndef GPLATES_QTWIDGETS_NOACTIVEEDITWIDGETEXCEPTION_H
#define GPLATES_QTWIDGETS_NOACTIVEEDITWIDGETEXCEPTION_H

#include "global/PreconditionViolationError.h"

namespace GPlatesQtWidgets
{
	/**
	 * Exception thrown by EditWidgetGroupBox when a precondition of
	 * at least one edit widget being active is violated.
	 */
	class NoActiveEditWidgetException:
			public GPlatesGlobal::PreconditionViolationError
	{
		public:
			explicit
			NoActiveEditWidgetException(
					const GPlatesUtils::CallStack::Trace &exception_source) :
				GPlatesGlobal::PreconditionViolationError(exception_source)
			{  }
			
			~NoActiveEditWidgetException() throw(){}
			
			const char *
			exception_name() const
			{
				return "NoActiveEditWidgetException";
			}
	};
}

#endif	// GPLATES_QTWIDGETS_NOACTIVEEDITWIDGETEXCEPTION_H
