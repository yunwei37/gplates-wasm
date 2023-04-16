/* $Id: ScalarField3DFileFormat.cc 13171 2012-07-07 05:50:09Z jcannon $ */

/**
 * \file 
 * $Revision: 13171 $
 * $Date: 2012-07-06 22:50:09 -0700 (Fri, 06 Jul 2012) $
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

#include "ScalarField3DFileFormat.h"


GPlatesFileIO::ScalarField3DFileFormat::UnsupportedVersion::UnsupportedVersion(
		const GPlatesUtils::CallStack::Trace &exception_source,
		quint32 unrecognised_version_) :
	Exception(exception_source),
	d_unrecognised_version(unrecognised_version_)
{
}


const char *
GPlatesFileIO::ScalarField3DFileFormat::UnsupportedVersion::exception_name() const
{

	return "ScalarField3DFileFormat::UnsupportedVersion";
}


void
GPlatesFileIO::ScalarField3DFileFormat::UnsupportedVersion::write_message(
		std::ostream &os) const
{
	os << "unsupported version: " << d_unrecognised_version;
}
