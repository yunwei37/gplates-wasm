/* $Id: GlobalTestSuite.cc 7584 2010-02-10 08:29:36Z mchin $ */

/**
 * \file 
 * $Revision: 7584 $
 * $Date: 2010-02-10 00:29:36 -0800 (Wed, 10 Feb 2010) $
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

#include <QDebug>

#include "unit-test/GlobalTestSuite.h"
#include "unit-test/TestSuiteFilter.h"

GPlatesUnitTest::GlobalTestSuite::GlobalTestSuite(
		unsigned level) : 
	GPlatesUnitTest::GPlatesTestSuite(
			"GlobalTestSuite")
{
	init(level);
}

void 
GPlatesUnitTest::GlobalTestSuite::construct_maps()
{
	//ADD YOUR TEST SUITE HERE
}


