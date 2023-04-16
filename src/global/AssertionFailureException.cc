/* $Id: AssertionFailureException.cc 12607 2011-12-05 07:31:31Z jcannon $ */

/**
 * \file 
 * $Revision: 12607 $
 * $Date: 2011-12-04 23:31:31 -0800 (Sun, 04 Dec 2011) $
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

#include "AssertionFailureException.h"


void
GPlatesGlobal::AssertionFailureException::write_message(
		std::ostream &os) const
{
	os << "Assertion failure";
}
