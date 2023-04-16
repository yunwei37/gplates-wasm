/* $Id: FeatureType.h 9606 2010-09-05 18:45:52Z elau $ */

/**
 * \file 
 * File specific comments.
 *
 * Most recent change:
 *   $Date: 2010-09-05 11:45:52 -0700 (Sun, 05 Sep 2010) $
 * 
 * Copyright (C) 2006, 2007, 2008, 2010 The University of Sydney, Australia
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

#ifndef GPLATES_MODEL_FEATURETYPE_H
#define GPLATES_MODEL_FEATURETYPE_H

#include "QualifiedXmlName.h"


namespace GPlatesModel
{
	class FeatureTypeFactory
	{
	public:

		static
		GPlatesUtils::StringSet &
		instance()
		{
			return StringSetSingletons::feature_type_instance();
		}

	private:

		FeatureTypeFactory();
	};

	typedef QualifiedXmlName<FeatureTypeFactory> FeatureType;
}

#endif  // GPLATES_MODEL_FEATURETYPE_H
