/* $Id: RealTest.h 7584 2010-02-10 08:29:36Z mchin $ */

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
#ifndef GPLATES_UNIT_TEST_REAL_TEST_H
#define GPLATES_UNIT_TEST_REAL_TEST_H

#include <boost/test/unit_test.hpp>

#include "maths/Real.h"
#include "unit-test/GPlatesTestSuite.h"

namespace GPlatesUnitTest{

	class RealTest
	{
	public:
		RealTest()
		{
			zero = 0.0;
		}

		void
		test_positive_infinity();

		void
		test_negative_infinity();

		void 
		test_nan();
		
		void
		test_zero();

		

	private:
		double zero;
	};

	
	class RealTestSuite : 
		public GPlatesUnitTest::GPlatesTestSuite
	{
	public:
		RealTestSuite(
				unsigned depth);

	protected:
		void 
		construct_maps();
	};
}
#endif //GPLATES_UNIT_TEST_REAL_TEST_H 

