/* $Id: GPlatesTestSuite.h 8643 2010-06-04 08:56:28Z elau $ */

/**
* \file 
* $Revision: 8643 $
* $Date: 2010-06-04 01:56:28 -0700 (Fri, 04 Jun 2010) $
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
#ifndef GPLATES_UNIT_TEST_GPLATESTESTSUITE_H
#define GPLATES_UNIT_TEST_GPLATESTESTSUITE_H

#include <map>
#include <boost/test/unit_test.hpp>
#include <QDebug>

#define ADD_TESTCASE( test_suite_name, test_case_name ) \
	d_test_cases_map[#test_case_name] = BOOST_CLASS_TEST_CASE( \
		&test_suite_name::test_case_name, instance ); \
	qDebug()<<"creating "#test_case_name" testcase ...";

#define ADD_TESTSUITE( test_suite_name ) \
	d_test_suites_map[#test_suite_name] = \
		new GPlatesUnitTest::test_suite_name ## TestSuite(d_level+1); \
	qDebug()<<"creating "#test_suite_name"TestSuite ...";

namespace GPlatesUnitTest
{
	class GPlatesTestSuite : 
		public boost::unit_test::test_suite
	{

	public:
		GPlatesTestSuite(std::string name) :
				boost::unit_test::test_suite(name)
		{ }
		
	protected:
		
		typedef std::map<
			const std::string, GPlatesUnitTest::GPlatesTestSuite*> TestSuiteMap;

		typedef std::map<
			const std::string, boost::unit_test::test_case*> TestCaseMap;

		TestSuiteMap d_test_suites_map;
		TestCaseMap d_test_cases_map;
		unsigned d_level;
		
		virtual
		void 
		init(unsigned level);
		
		virtual	
		void 
		construct_maps()
		{ }
		
		virtual 
		void 
		add_test_suites();
		
		virtual 
		void 
		add_test_cases();
			
	private:
		GPlatesTestSuite();
	};
}
#endif //GPLATES_UNIT_TEST_GPLATESTESTSUITE_H

