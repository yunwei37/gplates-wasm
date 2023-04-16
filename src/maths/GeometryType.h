/* $Id: GeometryType.h 14130 2013-05-10 19:50:05Z jcannon $ */

/**
 * \file 
 * $Revision: 14130 $
 * $Date: 2013-05-10 12:50:05 -0700 (Fri, 10 May 2013) $
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

#ifndef GPLATES_MATHS_GEOMETRYTYPE_H
#define GPLATES_MATHS_GEOMETRYTYPE_H

namespace GPlatesMaths
{
	namespace GeometryType
	{
		/**
		 * Types of @a GeometryOnSphere.
		 */
		enum Value
		{
			NONE,
			POINT,
			MULTIPOINT,
			POLYLINE,
			POLYGON
		};
	}
}

#endif // GPLATES_MATHS_GEOMETRYTYPE_H
