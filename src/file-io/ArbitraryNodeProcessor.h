/* $Id: ArbitraryNodeProcessor.h 11447 2011-05-05 05:31:16Z mchin $ */

/**
 * \file 
 * File specific comments.
 *
 * Most recent change:
 *   $Date: 2011-05-04 22:31:16 -0700 (Wed, 04 May 2011) $
 * 
 * Copyright (C) 2011 The University of Sydney, Australia
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

#ifndef GPLATES_FILEIO_ARBITRARYNODEPROCESSOR_H
#define GPLATES_FILEIO_ARBITRARYNODEPROCESSOR_H

#include <vector>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <QBuffer>
#include <QFile>
#include <QString>
//#include <QXmlQuery>

namespace GPlatesFileIO
{
	class ArbitraryNodeProcessor
	{
	public:
		
		virtual
		void
		execute(
				QBuffer& xml_data) = 0;
		virtual
		~ArbitraryNodeProcessor(){}

	};
}

#endif  // GPLATES_FILEIO_ARBITRARYNODEPROCESSOR_H
