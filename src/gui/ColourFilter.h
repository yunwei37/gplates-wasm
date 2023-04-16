/* $Id: ColourFilter.h 8162 2010-04-22 18:15:07Z elau $ */

/**
 * @file 
 * Contains the defintion of the ColourFilter class.
 * 
 * Most recent change:
 *   $Date: 2010-04-22 11:15:07 -0700 (Thu, 22 Apr 2010) $
 * 
 * Copyright (C) 2010 The University of Sydney, Australia
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

#ifndef GPLATES_GUI_COLOURFILTER_H
#define GPLATES_GUI_COLOURFILTER_H

#include "Colour.h"


namespace GPlatesGui
{
	/**
	 * Base class for classes that modify the output from ColourSchemes.
	 * See the comments in ColourProxy for the rationale for this class.
	 */
	class ColourFilter
	{
	public:

		//! Virtual destructor
		virtual
		~ColourFilter()
		{
		}

		//! Maps @a input_colour to another colour.
		virtual
		const Colour &
		change_colour(
				const Colour &input_colour) = 0;

	};

}

#endif  // GPLATES_GUI_COLOURFILTER_H
