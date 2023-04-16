/* $Id: NullIntrusivePointerHandler.h 3268 2008-07-12 02:36:59Z jboyden $ */

/**
 * \file 
 * File specific comments.
 *
 * Most recent change:
 *   $Date: 2008-07-11 19:36:59 -0700 (Fri, 11 Jul 2008) $
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

#ifndef GPLATES_UTILS_NULLINTRUSIVEPOINTERHANDLER_H
#define GPLATES_UTILS_NULLINTRUSIVEPOINTERHANDLER_H

namespace GPlatesUtils
{
	class NullIntrusivePointerHandler
	{
	public:
		NullIntrusivePointerHandler()
		{  }

		~NullIntrusivePointerHandler()
		{  }

		void
		operator()() const;
	};
}

#endif  // GPLATES_UTILS_NULLINTRUSIVEPOINTERHANDLER_H
