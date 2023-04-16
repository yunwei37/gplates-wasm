/* $Id: ReconstructMethodVirtualGeomagneticPole.h 14130 2013-05-10 19:50:05Z jcannon $ */

/**
 * \file 
 * $Revision: 14130 $
 * $Date: 2013-05-10 12:50:05 -0700 (Fri, 10 May 2013) $
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

#ifndef GPLATES_APP_LOGIC_RECONSTRUCTMETHODVIRTUALGEOMAGNETICPOLE_H
#define GPLATES_APP_LOGIC_RECONSTRUCTMETHODVIRTUALGEOMAGNETICPOLE_H

#include <boost/optional.hpp>

#include "ReconstructedFeatureGeometry.h"
#include "ReconstructMethodInterface.h"

#include "utils/non_null_intrusive_ptr.h"


namespace GPlatesAppLogic
{
	/**
	 * Reconstructs a virtual geomagnetic pole feature.
	 */
	class ReconstructMethodVirtualGeomagneticPole :
			public ReconstructMethodInterface
	{
	public:
		// Convenience typedefs for a shared pointer to a @a ReconstructMethodVirtualGeomagneticPole.
		typedef GPlatesUtils::non_null_intrusive_ptr<ReconstructMethodVirtualGeomagneticPole> non_null_ptr_type;
		typedef GPlatesUtils::non_null_intrusive_ptr<const ReconstructMethodVirtualGeomagneticPole> non_null_ptr_to_const_type;


		/**
		 * Returns true if can reconstruct the specified feature.
		 *
		 * Feature is expected to have a feature type of "VirtualGeomagneticPole".
		 */
		static
		bool
		can_reconstruct_feature(
				const GPlatesModel::FeatureHandle::const_weak_ref &feature_weak_ref);


		/**
		 * Creates a @a ReconstructMethodHalfStageRotation object associated with the specified feature.
		 */
		static
		ReconstructMethodVirtualGeomagneticPole::non_null_ptr_type
		create(
				const GPlatesModel::FeatureHandle::weak_ref &feature_ref,
				const Context &context)
		{
			return non_null_ptr_type(new ReconstructMethodVirtualGeomagneticPole(feature_ref, context));
		}


		/**
		 * Returns the present day geometries of the feature associated with this reconstruct method.
		 */
		virtual
		void
		get_present_day_feature_geometries(
				std::vector<Geometry> &present_day_geometries) const;


		/**
		 * Reconstructs the feature associated with this reconstruct method to the specified
		 * reconstruction time and returns one or more reconstructed feature geometries.
		 */
		virtual
		void
		reconstruct_feature_geometries(
				std::vector<ReconstructedFeatureGeometry::non_null_ptr_type> &reconstructed_feature_geometries,
				const ReconstructHandle::type &reconstruct_handle,
				const Context &context,
				const double &reconstruction_time);


		/**
		 * Reconstructs the specified geometry from present day to the specified reconstruction time -
		 * unless @a reverse_reconstruct is true in which case the geometry is assumed to be
		 * the reconstructed geometry (at the reconstruction time) and the returned geometry will
		 * then be the present day geometry.
		 */
		virtual
		GPlatesMaths::GeometryOnSphere::non_null_ptr_to_const_type
		reconstruct_geometry(
				const GPlatesMaths::GeometryOnSphere::non_null_ptr_to_const_type &geometry,
				const Context &context,
				const double &reconstruction_time,
				bool reverse_reconstruct);

	private:

		explicit
		ReconstructMethodVirtualGeomagneticPole(
				const GPlatesModel::FeatureHandle::weak_ref &feature_ref,
				const Context &context) :
			ReconstructMethodInterface(ReconstructMethod::VIRTUAL_GEOMAGNETIC_POLE, feature_ref)
		{  }
	};
}

#endif // GPLATES_APP_LOGIC_RECONSTRUCTMETHODVIRTUALGEOMAGNETICPOLE_H
