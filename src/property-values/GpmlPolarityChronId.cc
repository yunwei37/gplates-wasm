/* $Id: GpmlPolarityChronId.cc 16827 2015-11-19 07:02:48Z jcannon $ */

/**
 * \file 
 * File specific comments.
 *
 * Most recent change:
 *   $Date $
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

#include "GpmlPolarityChronId.h"


std::ostream &
GPlatesPropertyValues::GpmlPolarityChronId::print_to(
		std::ostream &os) const
{
	if (d_era)
	{
		os << d_era->toStdString() << " ";
	}
	if (d_major_region)
	{
		os << *d_major_region << " ";
	}
	if (d_minor_region)
	{
		os << d_minor_region->toStdString();
	}
	return os;
}
