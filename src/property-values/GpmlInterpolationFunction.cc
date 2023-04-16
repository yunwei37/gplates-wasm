/* $Id: GpmlInterpolationFunction.cc 10554 2010-12-13 05:57:08Z mchin $ */

/**
 * \file 
 * File specific comments.
 *
 * Most recent change:
 *   $Date: 2010-12-12 21:57:08 -0800 (Sun, 12 Dec 2010) $
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

#include <iostream>

#include "GpmlInterpolationFunction.h"


std::ostream &
GPlatesPropertyValues::GpmlInterpolationFunction::print_to(
		std::ostream &os) const
{
	const GPlatesUtils::UnicodeString &value_type_as_string = d_value_type.build_aliased_name();
	return os << value_type_as_string;
}

