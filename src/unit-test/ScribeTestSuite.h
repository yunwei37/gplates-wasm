/* $Id: ScribeTestSuite.h 17704 2016-06-29 15:44:25Z jcannon $ */

/**
 * \file 
 * $Revision: 17704 $
 * $Date: 2016-06-29 08:44:25 -0700 (Wed, 29 Jun 2016) $
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

#ifndef GPLATES_UNIT_TEST_SCRIBETESTSUITE_H
#define GPLATES_UNIT_TEST_SCRIBETESTSUITE_H

#include <boost/test/unit_test.hpp>

#include "unit-test/GPlatesTestSuite.h"


//
// To run only Scribe test suite:
//
// gplates-unit-test.exe --G_test_to_run=Scribe
//

namespace GPlatesUnitTest
{
	class ScribeTestSuite : 
			public GPlatesUnitTest::GPlatesTestSuite
	{
	public:
		ScribeTestSuite(
				unsigned depth);

	protected:
		void 
		construct_maps();
	};
}

#endif // GPLATES_UNIT_TEST_SCRIBETESTSUITE_H
