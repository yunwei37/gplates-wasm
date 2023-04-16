/* $Id: RevisionAwareIterator.cc 7990 2010-04-12 08:48:29Z elau $ */

/**
 * \file 
 * Contains template specialisations for the templated RevisionAwareIterator class.
 *
 * Most recent change:
 *   $Date: 2010-04-12 01:48:29 -0700 (Mon, 12 Apr 2010) $
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

#include "RevisionAwareIterator.h"

namespace GPlatesModel
{

	template<>
	RevisionAwareIterator<FeatureHandle>::value_type
	RevisionAwareIterator<FeatureHandle>::current_element() const
	{
		return TopLevelPropertyRef(*this);
	}

}

