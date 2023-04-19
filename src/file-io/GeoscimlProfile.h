/* $Id: GeoscimlProfile.h 13766 2013-01-24 02:29:49Z mchin $ */

/**
 * \file 
 * File specific comments.
 *
 * Most recent change:
 *   $Date: 2013-01-23 18:29:49 -0800 (Wed, 23 Jan 2013) $
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

#ifndef GPLATES_FILEIO_GEOSCIMLPROFILE_H
#define GPLATES_FILEIO_GEOSCIMLPROFILE_H

#include <QObject>
#include <QString>

#include "ArbitraryXmlProfile.h"
#include "model/FeatureCollectionHandle.h"

namespace GPlatesFileIO
{
	class GeoscimlProfile :
			public QObject,
			public ArbitraryXmlProfile
	{
		Q_OBJECT

	public:

		GeoscimlProfile()
		{ }

		explicit
		GeoscimlProfile(
				const QString& profile_name)
		{ }
		
		void
		populate(
				File::Reference& xml_file);

		void
		populate(
				QByteArray& xml_data,
				GPlatesModel::FeatureCollectionHandle::weak_ref fch);
				
		// check for features, return the number 
		int 
		count_features(
				QByteArray& xml_data);

	public Q_SLOTS:

		void cancel(); // will cancel read process

	protected:
		GeoscimlProfile(
					const GeoscimlProfile&);

	private:
		bool d_cancel;

	};
}

#endif  // GPLATES_FILEIO_GEOSCIMLPROFILE_H
