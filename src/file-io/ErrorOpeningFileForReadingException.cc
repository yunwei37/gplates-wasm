/* $Id: ErrorOpeningFileForReadingException.cc 6804 2009-10-12 04:00:43Z jcannon $ */

/**
 * \file 
 * $Revision: 6804 $
 * $Date: 2009-10-11 21:00:43 -0700 (Sun, 11 Oct 2009) $
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

#include "ErrorOpeningFileForReadingException.h"


void
GPlatesFileIO::ErrorOpeningFileForReadingException::write_message(
		std::ostream &os) const
{
	os << "Error opening file '" << d_filename.toStdString() << "' for reading";
}
