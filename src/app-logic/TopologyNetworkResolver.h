
/**
 * \file 
 * File specific comments.
 *
 * Most recent change:
 *   $Date: 2008-08-15 02:13:48 -0700 (Fri, 15 Aug 2008) $
 * 
 * Copyright (C) 2008, 2010 The University of Sydney, Australia
 * Copyright (C) 2008, 2009, 2013 California Institute of Technology 
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

#ifndef GPLATES_APP_LOGIC_TOPOLOGY_NETWORK_RESOLVER_H
#define GPLATES_APP_LOGIC_TOPOLOGY_NETWORK_RESOLVER_H

#include <cstddef> // For std::size_t
#include <set>
#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/optional.hpp>
#include <boost/random.hpp>
#include <boost/generator_iterator.hpp>

#include "ReconstructedFeatureGeometry.h"
#include "ReconstructHandle.h"
#include "ReconstructionFeatureProperties.h"
#include "ReconstructionGeometry.h"
#include "ResolvedTopologicalLine.h"
#include "ResolvedTopologicalNetwork.h"
#include "ResolvedTriangulationNetwork.h"
#include "TopologyIntersections.h"
#include "TopologyNetworkParams.h"

#include "maths/AngularExtent.h"
#include "maths/GeometryOnSphere.h"

#include "model/types.h"
#include "model/FeatureId.h"
#include "model/FeatureHandle.h"
#include "model/FeatureVisitor.h"
#include "model/FeatureCollectionHandle.h"
#include "model/Model.h"

#include "property-values/GpmlTopologicalNetwork.h"
#include "property-values/GpmlTopologicalSection.h"


namespace GPlatesPropertyValues
{
	class GpmlPropertyDelegate;
	class GpmlTimeWindow;
}

namespace GPlatesAppLogic
{
	/**
	 * Finds all topological network features (in the features visited)
	 * that exist at a particular reconstruction time and creates
	 * @a ResolvedTopologicalNetwork objects for each one.
	 */
	class TopologyNetworkResolver: 
		public GPlatesModel::FeatureVisitor,
		private boost::noncopyable
	{

	public:
		/**
		 * The resolved networks are appended to @a resolved_topological_networks.
		 *
		 * @param reconstruct_handle is placed in all resolved topological networks as a reconstruction identifier.
		 * @param reconstruction_tree is associated with the output resolved topological networks.
		 * @param topological_geometry_reconstruct_handles is a list of reconstruct handles that
		 *        identifies the subset, of all RFGs observing the topological boundary section and/or
		 *        interior features, and all resolved topological lines (@a ResolvedTopologicalLine)
		 *        observing the topological boundary section features, that should be searched when
		 *        resolving the topological networks.
		 *        This is useful to avoid outdated RFGs and RTGS still in existence (among other scenarios).
		 * @param topology_network_params parameters used when creating the resolved networks.
		 */
		TopologyNetworkResolver(
				std::vector<ResolvedTopologicalNetwork::non_null_ptr_type> &resolved_topological_networks,
				const double &reconstruction_time,
				ReconstructHandle::type reconstruct_handle,
				boost::optional<const std::vector<ReconstructHandle::type> &> topological_geometry_reconstruct_handles,
				const TopologyNetworkParams &topology_network_params = TopologyNetworkParams());

		virtual
		~TopologyNetworkResolver() 
		{  }

		virtual
		bool
		initialise_pre_feature_properties(
				GPlatesModel::FeatureHandle &feature_handle);

		virtual
		void
		finalise_post_feature_properties(
				GPlatesModel::FeatureHandle &feature_handle);

		virtual
		void
		visit_gpml_constant_value(
				GPlatesPropertyValues::GpmlConstantValue &gpml_constant_value);

		virtual
		void
		visit_gpml_piecewise_aggregation(
				GPlatesPropertyValues::GpmlPiecewiseAggregation &gpml_piecewise_aggregation);

		void
		visit_gpml_time_window(
				GPlatesPropertyValues::GpmlTimeWindow &gpml_time_window);

		virtual
		void
		visit_gpml_topological_network(
			 	GPlatesPropertyValues::GpmlTopologicalNetwork &gpml_topological_network);

		virtual
		void
		visit_gpml_topological_line_section(
				GPlatesPropertyValues::GpmlTopologicalLineSection &gpml_topological_line_section);

		virtual
		void
		visit_gpml_topological_point(
				GPlatesPropertyValues::GpmlTopologicalPoint &gpml_topological_point);

		void
		visit_gpml_plate_id(
				GPlatesPropertyValues::GpmlPlateId &gpml_plate_id);

		void
		visit_xs_double(
				GPlatesPropertyValues::XsDouble &xs_double);

	private:
		/**
		 * Stores/builds information from iterating over @a GpmlTopologicalSection objects.
		 */
		class ResolvedNetwork
		{
		public:
			//! Reset in preparation for a new sequence of topological sections.
			void
			reset()
			{
				boundary_sections.clear();
				interior_geometries.clear();
				topological_network_property = boost::none;
			}

			//! Record the topological network property of the feature being visited.
			void
			initialise(
					boost::optional<GPlatesModel::FeatureHandle::iterator> topological_network_property_)
			{
				topological_network_property = topological_network_property_;
			}

			//! Do we have a resolved network?
			bool
			has_resolved_network() const
			{
				// Must have boundary sections. Don't need interior geometries though.
				return topological_network_property && !boundary_sections.empty();
			}

			//! Keeps track of topological boundary section information when visiting topological sections.
			class BoundarySection
			{
			public:
				BoundarySection(
						const GPlatesModel::FeatureId &source_feature_id,
						const ReconstructionGeometry::non_null_ptr_type &source_rg,
						const GPlatesMaths::GeometryOnSphere::non_null_ptr_to_const_type &section_geometry,
						bool reverse_hint) :
					d_source_feature_id(source_feature_id),
					d_source_rg(source_rg),
					d_intersection_results(TopologicalIntersections::create(source_rg, section_geometry, reverse_hint))
				{  }

				//! The feature id of the feature referenced by this topological section.
				GPlatesModel::FeatureId d_source_feature_id;

				//! The source reconstruction geometry.
				ReconstructionGeometry::non_null_ptr_type d_source_rg;

				/**
				 * Keeps track of temporary results from intersections of this section
				 * with its neighbours.
				 */
				TopologicalIntersections::shared_ptr_type d_intersection_results;
			};

			//! Keeps track of interior geometry information when visiting topological interiors.
			class InteriorGeometry
			{
			public:
				InteriorGeometry(
						const GPlatesModel::FeatureId &source_feature_id,
						const ReconstructionGeometry::non_null_ptr_type &source_rg,
						const GPlatesMaths::GeometryOnSphere::non_null_ptr_to_const_type &geometry) :
					d_source_feature_id(source_feature_id),
					d_source_rg(source_rg),
					d_geometry(geometry)
				{  }

				//! The feature id of the feature referenced by this topological interior.
				GPlatesModel::FeatureId d_source_feature_id;

				//! The source reconstruction geometry.
				ReconstructionGeometry::non_null_ptr_type d_source_rg;

				//! The geometry-on-sphere.
				GPlatesMaths::GeometryOnSphere::non_null_ptr_to_const_type d_geometry;
			};


			//! Typedef for a sequence of boundary sections.
			typedef std::vector<BoundarySection> boundary_section_seq_type;

			//! Typedef for a sequence of interior geometries.
			typedef std::vector<InteriorGeometry> interior_geometry_seq_type;


			//! Sequence of boundary sections.
			boundary_section_seq_type boundary_sections;

			//! Sequence of interior geometries.
			interior_geometry_seq_type interior_geometries;

			//! The topological network feature property containing the boundary and interior sections.
			boost::optional<GPlatesModel::FeatureHandle::iterator> topological_network_property;
		};

		/**
		* Feature properties if this network is a rift.
		*
		* A network is a rift if the network feature has rift left/right plate IDs.
		*/
		struct RiftProperties
		{
		public:
			void
			reset()
			{
				left_plate_id = boost::none;
				right_plate_id = boost::none;

				exponential_stretching_constant = boost::none;
				strain_rate_resolution = boost::none;
				edge_length_threshold = boost::none;
			}

			boost::optional<GPlatesModel::integer_plate_id_type> left_plate_id;
			boost::optional<GPlatesModel::integer_plate_id_type> right_plate_id;

			boost::optional<double> exponential_stretching_constant;
			boost::optional<double> strain_rate_resolution;
			boost::optional<GPlatesMaths::AngularExtent> edge_length_threshold;
		};


		/**
		 * The resolved topological networks we're generating.
		 */
		std::vector<ResolvedTopologicalNetwork::non_null_ptr_type> &d_resolved_topological_networks;

		/**
		 * The time at which topologies are being resolved.
		 */
		double d_reconstruction_time;

		/**
		 * The reconstruction identifier placed in all resolved topological networks.
		 */
		ReconstructHandle::type d_reconstruct_handle;

		/**
		 * The reconstructed topological boundary sections and/or interior geometries we're using to
		 * assemble our network.
		 *
		 * NOTE: We don't reference them directly but by forcing clients to produce them we
		 * ensure they exist while we search for them indirectly via feature observers.
		 * If the client didn't produce them, or forgot to, then we'd find no RFGs during the global search.
		 */
		boost::optional<std::vector<ReconstructHandle::type> > d_topological_geometry_reconstruct_handles;

		/**
		 * Parameters, including whether to smooth the deformation strain rate calculation results across the triangulation.
		 */
		TopologyNetworkParams d_topology_network_params;

		//! The current feature being visited.
		GPlatesModel::FeatureHandle::weak_ref d_currently_visited_feature;

		//! Gathers some useful reconstruction parameters.
		ReconstructionFeatureProperties d_current_reconstruction_params;

		//! Parameters if this network is a rift.
		RiftProperties d_current_rift_params;

		//! Used to help build the resolved network of the current topological polygon.
		ResolvedNetwork d_current_resolved_network;


		boost::optional<ReconstructionGeometry::non_null_ptr_type>
		find_topological_reconstruction_geometry(
				const GPlatesPropertyValues::GpmlPropertyDelegate &geometry_delegate);

		bool
		is_deprecated_seed_geometry(
				const ReconstructionGeometry::non_null_ptr_type &reconstruction_geometry);

		void
		record_topological_interior_geometries(
				GPlatesPropertyValues::GpmlTopologicalNetwork &gpml_topological_network);

		void
		record_topological_interior_geometry(
				const GPlatesPropertyValues::GpmlPropertyDelegate &gpml_topological_interior);

		boost::optional<ResolvedNetwork::InteriorGeometry>
		record_topological_interior_reconstructed_geometry(
				const GPlatesModel::FeatureId &interior_source_feature_id,
				const ReconstructionGeometry::non_null_ptr_type &interior_source_rg);

		void
		record_topological_boundary_sections(
				GPlatesPropertyValues::GpmlTopologicalNetwork &gpml_topological_network);

		boost::optional<ResolvedNetwork::BoundarySection>
		record_topological_boundary_section_reconstructed_geometry(
				const GPlatesModel::FeatureId &boundary_section_source_feature_id,
				const ReconstructionGeometry::non_null_ptr_type &boundary_section_source_rg,
				bool reverse_hint);


		void
		process_topological_boundary_section_intersections();

		void
		process_topological_section_intersection_boundary(
				const std::size_t current_section_index,
				const bool two_sections = false);


		/**
		 * Create a @a ResolvedTopologicalNetwork from information gathered
		 * from the most recently visited topological polygon
		 * (stored in @a d_resolved_network) and add it to the @a Reconstruction.
		 */
		void
		create_resolved_topology_network();
	};
}

#endif  // GPLATES_APP_LOGIC_TOPOLOGY_NETWORK_RESOLVER_H
