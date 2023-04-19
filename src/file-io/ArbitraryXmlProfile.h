/* $Id: ArbitraryXmlProfile.h 12848 2012-01-30 06:19:37Z mchin $ */

/**
 * \file 
 * File specific comments.
 *
 * Most recent change:
 *   $Date: 2012-01-29 22:19:37 -0800 (Sun, 29 Jan 2012) $
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

#ifndef GPLATES_FILEIO_ARBITRARYXMLPROFILE_H
#define GPLATES_FILEIO_ARBITRARYXMLPROFILE_H

#include <QObject>
#include <QString>

#include "File.h"
#include "model/FeatureCollectionHandle.h"

namespace GPlatesFileIO
{

	class ArbitraryXmlProfile
	{
	public:

		virtual
		void
		populate(
				File::Reference&) = 0;

		virtual
		void
		populate(
				QByteArray& xml_data,
				GPlatesModel::FeatureCollectionHandle::weak_ref fch) = 0;
		
		virtual
		int
		count_features(
				QByteArray& xml_data) = 0;
		
		virtual
		~ArbitraryXmlProfile(){}

	};
}

#endif  // GPLATES_FILEIO_ARBITRARYXMLPROFILE_H
