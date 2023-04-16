/* $Id: GmlTimePeriodFinder.cc 5528 2009-04-20 11:04:51Z jcannon $ */

/**
 * \file 
 * File specific comments.
 *
 * Most recent change:
 *   $Date: 2009-04-20 04:04:51 -0700 (Mon, 20 Apr 2009) $
 * 
 * Copyright (C) 2008 The University of Sydney, Australia
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

#include <algorithm>  // std::find

#include "GmlTimePeriodFinder.h"

#include "model/FeatureHandle.h"
#include "model/InlinePropertyContainer.h"
#include "property-values/GmlTimePeriod.h"


void
GPlatesFeatureVisitors::GmlTimePeriodFinder::visit_feature_handle(
		const GPlatesModel::FeatureHandle &feature_handle)
{
	// Now visit each of the properties in turn.
	visit_feature_properties(feature_handle);
}


namespace
{
	template<typename C, typename E>
	bool
	contains_elem(
			const C &container,
			const E &elem)
	{
		return (std::find(container.begin(), container.end(), elem) != container.end());
	}
}


void
GPlatesFeatureVisitors::GmlTimePeriodFinder::visit_inline_property_container(
		const GPlatesModel::InlinePropertyContainer &inline_property_container)
{
	const GPlatesModel::PropertyName &curr_prop_name = inline_property_container.property_name();

	if ( ! d_property_names_to_allow.empty()) {
		// We're not allowing all property names.
		if ( ! contains_elem(d_property_names_to_allow, curr_prop_name)) {
			// The current property name is not allowed.
			return;
		}
	}

	visit_property_values(inline_property_container);
}


void
GPlatesFeatureVisitors::GmlTimePeriodFinder::visit_gml_time_period(
		const GPlatesPropertyValues::GmlTimePeriod &gml_time_period)
{
	d_found_time_periods.push_back(
			GPlatesPropertyValues::GmlTimePeriod::non_null_ptr_to_const_type(&gml_time_period,
					GPlatesUtils::NullIntrusivePointerHandler()));
}
