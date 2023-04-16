/* $Id: InvalidLatLonException.cc 5528 2009-04-20 11:04:51Z jcannon $ */

/**
 * \file 
 * $Revision: 5528 $
 * $Date: 2009-04-20 04:04:51 -0700 (Mon, 20 Apr 2009) $
 * 
 * Copyright (C) 2009 The University of Sydney, Australia
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

#include <ostream>

#include "InvalidLatLonException.h"


void
GPlatesMaths::InvalidLatLonException::write_message(
		std::ostream &os) const
{
	// FIXME:  This function should really be defined in a .cc file.
	os << "invalid "
			<< ((lat_or_lon() == Latitude) ? "latitude" : "longitude")
			<< " value " << d_invalid_value;
}
