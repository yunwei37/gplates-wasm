/* $Id: GpgimInitialisationException.cc 13507 2012-11-01 04:31:49Z jcannon $ */

/**
 * \file 
 * $Revision: 13507 $
 * $Date: 2012-10-31 21:31:49 -0700 (Wed, 31 Oct 2012) $
 * 
 * Copyright (C) 2012 The University of Sydney, Australia
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

#include "GpgimInitialisationException.h"


void
GPlatesModel::GpgimInitialisationException::write_message(
		std::ostream &os) const
{
	os
		<< "Error initialising GPGIM from file '" << d_gpgim_filename.toStdString()
		<< "' at line '" << d_line_number
		<< "': " << d_msg.toStdString();
}
