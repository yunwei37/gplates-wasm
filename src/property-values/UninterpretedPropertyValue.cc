/* $Id: UninterpretedPropertyValue.cc 13507 2012-11-01 04:31:49Z jcannon $ */

/**
 * \file 
 * File specific comments.
 *
 * Most recent change:
 *   $Date: 2012-10-31 21:31:49 -0700 (Wed, 31 Oct 2012) $
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
#include <typeinfo>

#include "UninterpretedPropertyValue.h"


std::ostream &
GPlatesPropertyValues::UninterpretedPropertyValue::print_to(
		std::ostream &os) const
{
	return os << d_value->get_name().build_aliased_name();
}
