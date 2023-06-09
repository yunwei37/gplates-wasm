/**
 * \file 
 * File specific comments.
 *
 * Most recent change:
 *   $Date: 2008-08-15 02:13:48 -0700 (Fri, 15 Aug 2008) $
 * 
 * Copyright (C) 2008, 2010 The University of Sydney, Australia
 * Copyright (C) 2008, 2009 California Institute of Technology 
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

#include <cstddef> // For std::size_t
#include <vector>
#include <boost/foreach.hpp>
#include <QDebug>

#include "TopologyGeometryResolver.h"

#include "GeometryUtils.h"
#include "ReconstructedFeatureGeometry.h"
#include "Reconstruction.h"
#include "ReconstructionGeometryUtils.h"
#include "TopologyInternalUtils.h"
#include "TopologyUtils.h"

#include "feature-visitors/PropertyValueFinder.h"

#include "global/AssertionFailureException.h"
#include "global/GPlatesAssert.h"

#include "maths/GeometryOnSphere.h"

#include "property-values/GpmlConstantValue.h"
#include "property-values/GpmlPiecewiseAggregation.h"
#include "property-values/GpmlTopologicalPolygon.h"
#include "property-values/GpmlTopologicalSection.h"
#include "property-values/GpmlTopologicalLine.h"
#include "property-values/GpmlTopologicalLineSection.h"
#include "property-values/GpmlTopologicalPoint.h"

#include "utils/GeometryCreationUtils.h"
#include "utils/Profile.h"
#include "utils/UnicodeStringUtils.h"


GPlatesAppLogic::TopologyGeometryResolver::TopologyGeometryResolver(
		std::vector<ResolvedTopologicalLine::non_null_ptr_type> &resolved_topological_lines,
		ReconstructHandle::type reconstruct_handle,
		const ReconstructionTreeCreator &reconstruction_tree_creator,
		const double &reconstruction_time,
		boost::optional<const std::vector<ReconstructHandle::type> &> topological_sections_reconstruct_handles) :
	d_resolved_topological_lines(resolved_topological_lines),
	d_reconstruct_handle(reconstruct_handle),
	d_reconstruction_tree_creator(reconstruction_tree_creator),
	d_reconstruction_tree(reconstruction_tree_creator.get_reconstruction_tree(reconstruction_time)),
	d_topological_sections_reconstruct_handles(topological_sections_reconstruct_handles)
{  
}


GPlatesAppLogic::TopologyGeometryResolver::TopologyGeometryResolver(
		std::vector<ResolvedTopologicalBoundary::non_null_ptr_type> &resolved_topological_boundaries,
		ReconstructHandle::type reconstruct_handle,
		const ReconstructionTreeCreator &reconstruction_tree_creator,
		const double &reconstruction_time,
		boost::optional<const std::vector<ReconstructHandle::type> &> topological_sections_reconstruct_handles) :
	d_resolved_topological_boundaries(resolved_topological_boundaries),
	d_reconstruct_handle(reconstruct_handle),
	d_reconstruction_tree_creator(reconstruction_tree_creator),
	d_reconstruction_tree(reconstruction_tree_creator.get_reconstruction_tree(reconstruction_time)),
	d_topological_sections_reconstruct_handles(topological_sections_reconstruct_handles)
{  
}


GPlatesAppLogic::TopologyGeometryResolver::TopologyGeometryResolver(
		std::vector<ResolvedTopologicalLine::non_null_ptr_type> &resolved_topological_lines,
		std::vector<ResolvedTopologicalBoundary::non_null_ptr_type> &resolved_topological_boundaries,
		ReconstructHandle::type reconstruct_handle,
		const ReconstructionTreeCreator &reconstruction_tree_creator,
		const double &reconstruction_time,
		boost::optional<const std::vector<ReconstructHandle::type> &> topological_sections_reconstruct_handles) :
	d_resolved_topological_lines(resolved_topological_lines),
	d_resolved_topological_boundaries(resolved_topological_boundaries),
	d_reconstruct_handle(reconstruct_handle),
	d_reconstruction_tree_creator(reconstruction_tree_creator),
	d_reconstruction_tree(reconstruction_tree_creator.get_reconstruction_tree(reconstruction_time)),
	d_topological_sections_reconstruct_handles(topological_sections_reconstruct_handles)
{  
}


bool
GPlatesAppLogic::TopologyGeometryResolver::initialise_pre_feature_properties(
		GPlatesModel::FeatureHandle &feature_handle)
{
	// NOTE: We don't test for topological feature types anymore.
	// If a feature has a topological polygon or topological line property then it will
	// get resolved, otherwise no reconstruction geometries will be generated.
	// We're not testing feature type because we're introducing the ability for any feature type
	// to allow a topological (or static) geometry property.
	// This will mean that some features, in a feature collection, that contain non-topological
	// geometries will be unnecessarily visited (but at least nothing meaningful will happen).

	// Keep track of the feature we're visiting - used for debug/error messages.
	d_currently_visited_feature = feature_handle.reference();

	// Collect some reconstruction properties from the feature such as reconstruction
	// plate ID and time of appearance/disappearance.
	d_reconstruction_params.visit_feature(d_currently_visited_feature);

	// If the feature is not defined at the reconstruction time then don't visit the properties.
	if (!d_reconstruction_params.is_feature_defined_at_recon_time(
		d_reconstruction_tree->get_reconstruction_time()))
	{
		return false;
	}

	// Now visit each of the properties in turn.
	return true;
}


void
GPlatesAppLogic::TopologyGeometryResolver::visit_gpml_constant_value(
		GPlatesPropertyValues::GpmlConstantValue &gpml_constant_value)
{
	gpml_constant_value.value()->accept_visitor(*this);
}


void
GPlatesAppLogic::TopologyGeometryResolver::visit_gpml_piecewise_aggregation(
		GPlatesPropertyValues::GpmlPiecewiseAggregation &gpml_piecewise_aggregation)
{
	std::vector<GPlatesPropertyValues::GpmlTimeWindow> &time_windows = gpml_piecewise_aggregation.time_windows();

	// NOTE: If there's only one time window then we do not check its time period against the
	// current reconstruction time.
	// This is because GPML files created with old versions of GPlates set the time period,
	// of the sole time window, to match that of the 'feature's time period (in the topology
	// build/edit tools) - newer versions set it to *all* time (distant past/future) - in fact
	// newer versions just use a GpmlConstantValue instead of GpmlPiecewiseAggregation because
	// the topology tools cannot yet create time-dependent topology (section) lists.
	// With old versions if the user expanded the 'feature's time period *after* building/editing
	// the topology then the *un-adjusted* time window time period will be incorrect and hence
	// we need to ignore it here.
	// Those old versions were around 4 years ago (prior to GPlates 1.3) - so we really shouldn't
	// be seeing any old topologies.
	// Actually I can see there are some currently in the sample data for GPlates 2.0.
	// So as a compromise we'll ignore the reconstruction time if there's only one time window
	// (a single time window shouldn't really have any time constraints on it anyway)
	// and respect the reconstruction time if there's more than one time window
	// (since multiple time windows need non-overlapping time constraints).
	// This is especially true now that pyGPlates will soon be able to generate time-dependent
	// topologies (where the reconstruction time will need to be respected otherwise multiple
	// topologies from different time periods will get created instead of just one of them).
	if (time_windows.size() == 1)
	{
		visit_gpml_time_window(time_windows.front());
		return;
	}

	BOOST_FOREACH(GPlatesPropertyValues::GpmlTimeWindow &time_window, time_windows)
	{
		// If the time window period contains the current reconstruction time then visit.
		// The time periods should be mutually exclusive - if we happen to be in
		// two time periods then we're probably right on the boundary between the two
		// in which case we'll only visit the first time window encountered.
		if (time_window.valid_time()->contains(d_reconstruction_tree->get_reconstruction_time()))
		{
			visit_gpml_time_window(time_window);
			return;
		}
	}
}


void
GPlatesAppLogic::TopologyGeometryResolver::visit_gpml_time_window(
		GPlatesPropertyValues::GpmlTimeWindow &gpml_time_window)
{
	gpml_time_window.time_dependent_value()->accept_visitor(*this);
}


void
GPlatesAppLogic::TopologyGeometryResolver::visit_gpml_topological_polygon(
		GPlatesPropertyValues::GpmlTopologicalPolygon &gpml_topological_polygon)
{
	// Only resolve topological boundaries (polygons) if we've been requested to.
	if (!d_resolved_topological_boundaries)
	{
		return;
	}

	PROFILE_FUNC();

	// Prepare for a new topological polygon.
	d_resolved_geometry.reset();

	// Visiting a topological polygon property.
	d_current_resolved_geometry_type = RESOLVE_BOUNDARY;

	//
	// Visit the topological sections to gather needed information and store
	// it internally in 'd_resolved_geometry'.
	//
	record_topological_sections(
			gpml_topological_polygon.exterior_sections_begin(),
			gpml_topological_polygon.exterior_sections_end());

	//
	// Now iterate over our internal structure 'd_resolved_geometry' and
	// intersect neighbouring sections that require it and
	// generate the resolved boundary subsegments.
	//
	process_resolved_boundary_topological_section_intersections();

	//
	// Now create the resolved topological boundary.
	//
	create_resolved_topological_boundary();

	// Finished visiting topological polygon property.
	d_current_resolved_geometry_type = boost::none;
}


void
GPlatesAppLogic::TopologyGeometryResolver::visit_gpml_topological_line(
		GPlatesPropertyValues::GpmlTopologicalLine &gpml_topological_line)
{
	// Only resolve topological lines if we've been requested to.
	if (!d_resolved_topological_lines)
	{
		return;
	}

	PROFILE_FUNC();

	// Prepare for a new topological line.
	d_resolved_geometry.reset();

	// Visiting a topological line property.
	d_current_resolved_geometry_type = RESOLVE_LINE;

	//
	// Visit the topological sections to gather needed information and store
	// it internally in 'd_resolved_geometry'.
	//
	record_topological_sections(
			gpml_topological_line.sections_begin(),
			gpml_topological_line.sections_end());

	//
	// Now iterate over our internal structure 'd_resolved_geometry' and
	// intersect neighbouring sections that require it and
	// generate the resolved line subsegments.
	//
	process_resolved_line_topological_section_intersections();

	//
	// Now create the resolved topological line.
	//
	create_resolved_topological_line();

	// Finished visiting topological line property.
	d_current_resolved_geometry_type = boost::none;
}


template <typename TopologicalSectionsIterator>
void
GPlatesAppLogic::TopologyGeometryResolver::record_topological_sections(
		const TopologicalSectionsIterator &sections_begin,
		const TopologicalSectionsIterator &sections_end)
{
	// loop over all the sections
	for (TopologicalSectionsIterator sections_iter = sections_begin;
		sections_iter != sections_end;
		++sections_iter)
	{
		GPlatesPropertyValues::GpmlTopologicalSection *topological_section = sections_iter->get();

		topological_section->accept_visitor(*this);
	}
}


void
GPlatesAppLogic::TopologyGeometryResolver::visit_gpml_topological_line_section(
		GPlatesPropertyValues::GpmlTopologicalLineSection &gpml_topological_line_section)
{  
	const GPlatesModel::FeatureId source_feature_id =
			gpml_topological_line_section.get_source_geometry()->feature_id();

	boost::optional<ResolvedGeometry::Section> section =
			record_topological_section_reconstructed_geometry(
					source_feature_id,
					*gpml_topological_line_section.get_source_geometry(),
					gpml_topological_line_section.get_reverse_order());
	if (!section)
	{
		// Return without adding topological section to the list of sections.
		return;
	}

	// Add to internal sequence.
	d_resolved_geometry.d_sections.push_back(*section);
}


void
GPlatesAppLogic::TopologyGeometryResolver::visit_gpml_topological_point(
		GPlatesPropertyValues::GpmlTopologicalPoint &gpml_topological_point)
{  
	const GPlatesModel::FeatureId source_feature_id =
			gpml_topological_point.get_source_geometry()->feature_id();

	boost::optional<ResolvedGeometry::Section> section =
			record_topological_section_reconstructed_geometry(
					source_feature_id,
					*gpml_topological_point.get_source_geometry(),
					// This topological section is a point, so cannot be intersected with its neighbours,
					// and so has no reversal information...
					false/*reverse_hint*/);
	if (!section)
	{
		// Return without adding topological section to the list of sections.
		return;
	}

	// Add to internal sequence.
	d_resolved_geometry.d_sections.push_back(*section);
}


boost::optional<GPlatesAppLogic::TopologyGeometryResolver::ResolvedGeometry::Section>
GPlatesAppLogic::TopologyGeometryResolver::record_topological_section_reconstructed_geometry(
		const GPlatesModel::FeatureId &source_feature_id,
		const GPlatesPropertyValues::GpmlPropertyDelegate &geometry_delegate,
		bool reverse_hint)
{
	// Get the reconstructed geometry of the topological section's delegate.
	// The referenced RGs must be in our sequence of reconstructed/resolved topological sections.
	// If we need to restrict the topological section RGs to specific reconstruct handles...
	boost::optional<const std::vector<ReconstructHandle::type> &> topological_sections_reconstruct_handles;
	if (d_topological_sections_reconstruct_handles)
	{
		topological_sections_reconstruct_handles = d_topological_sections_reconstruct_handles.get();
	}
	// Find the topological section reconstruction geometry.
	boost::optional<ReconstructionGeometry::non_null_ptr_type> source_rg =
			TopologyInternalUtils::find_topological_reconstruction_geometry(
					geometry_delegate,
					d_reconstruction_tree->get_reconstruction_time(),
					topological_sections_reconstruct_handles);
	if (!source_rg)
	{
		// If no RG was found then it's possible that the current reconstruction time is
		// outside the age range of the feature this section is referencing.
		// This is ok - it's not necessarily an error.
		// We just won't add it to the list of sections. This means either:
		//  - rubber banding will occur between the two sections adjacent to this section
		//    since this section is now missing, or
		//  - one of the adjacent sections did not exist until just now (because of its age range)
		//    and now it is popping in to replace the current section which is disappearing (an
		//    example of this is a bunch of sections that are mid-ocean ridge features that do not
		//    overlap in time and represent different geometries, from isochrons, of the same ridge).
		return boost::none;
	}

	//
	// Currently, topological sections can only be reconstructed feature geometries
	// (for resolved lines) and/or resolved topological *lines* (for resolved boundaries).
	//

	// See if topological section is a reconstructed feature geometry (or any of its derived types).
	boost::optional<ReconstructedFeatureGeometry *> source_rfg =
			ReconstructionGeometryUtils::get_reconstruction_geometry_derived_type<
					ReconstructedFeatureGeometry *>(source_rg.get());
	if (source_rfg)
	{
		// Store the feature id and reconstruction geometry.
		return ResolvedGeometry::Section(
				source_feature_id,
				source_rfg.get(),
				source_rfg.get()->reconstructed_geometry(),
				reverse_hint);
	}

	if (d_current_resolved_geometry_type == RESOLVE_BOUNDARY)
	{
		// See if topological section is a resolved topological line.
		boost::optional<ResolvedTopologicalLine *> source_rtl =
				ReconstructionGeometryUtils::get_reconstruction_geometry_derived_type<
						ResolvedTopologicalLine *>(source_rg.get());
		if (source_rtl)
		{
			// Store the feature id and reconstruction geometry.
			return ResolvedGeometry::Section(
					source_feature_id,
					source_rtl.get(),
					source_rtl.get()->resolved_topology_line(),
					reverse_hint);
		}
	}

	// If we got here then either (1) the user created a malformed GPML file somehow (eg, with a script)
	// or (2) it's a program error (because the topology build/edit tools should only currently allow
	// the user to add topological sections that are reconstructed static geometries
	// (or resolved topological *lines* when resolving boundaries).
	// We'll assume (1) and emit an error message rather than asserting/aborting.
	if (d_current_resolved_geometry_type == RESOLVE_BOUNDARY)
	{
		qWarning() << "Ignoring topological section, for resolved boundary, that is not a regular feature or topological *line*.";
	}
	else
	{
		qWarning() << "Ignoring topological section, for resolved line, that is not a regular feature.";
	}

	return boost::none;
}


void
GPlatesAppLogic::TopologyGeometryResolver::process_resolved_boundary_topological_section_intersections()
{
	// Iterate over our internal sequence of sections that we built up by
	// visiting the topological sections of a topological geometry property.
	const std::size_t num_sections = d_resolved_geometry.d_sections.size();

	// If there's only one section then don't try to intersect it with itself.
	if (num_sections < 2)
	{
		return;
	}

	PROFILE_FUNC();

	// Special case treatment when there are exactly two sections.
	// In this case the two sections can intersect twice to form a closed polygon.
	// This is the only case where two adjacent sections are allowed to intersect twice.
	if (num_sections == 2)
	{
		// NOTE: We use index 1 instead of 0 to match similar code in the topology builder tool.
		// This makes a difference if the user builds a topology with two sections that only
		// intersect once (not something the user should be building) and means that the
		// same topology will be creating here as in the builder.
		process_resolved_boundary_topological_section_intersection(1/*section_index*/, true/*two_sections*/);
		return;
	}

	// Iterate over the sections and process intersections between each section
	// and its previous neighbour.
	for (std::size_t section_index = 0; section_index < num_sections; ++section_index)
	{
		process_resolved_boundary_topological_section_intersection(section_index);
	}
}


void
GPlatesAppLogic::TopologyGeometryResolver::process_resolved_boundary_topological_section_intersection(
		const std::size_t current_section_index,
		const bool two_sections)
{
	//
	// Intersect the current section with the previous section.
	//

	const std::size_t num_sections = d_resolved_geometry.d_sections.size();

	ResolvedGeometry::Section &current_section = d_resolved_geometry.d_sections[current_section_index];

	//
	// We get the start intersection geometry the previous section in the topological geometry's
	// list of sections whose valid time ranges include the current reconstruction time.
	//

	// Topological *boundaries* form a closed loop of sections so handle wraparound.
	const std::size_t prev_section_index = (current_section_index == 0)
			? num_sections - 1
			: current_section_index - 1;

	ResolvedGeometry::Section &prev_section = d_resolved_geometry.d_sections[prev_section_index];

	// If both sections refer to the same geometry then don't intersect.
	// This can happen when the same geometry is added more than once to the topology
	// when it forms different parts of the resolved topological geometry - normally there
	// are other geometries in between but when building topologies it's possible to
	// add the geometry as first section, then add another geometry as second section,
	// then add the first geometry again as the third section and then add another
	// geometry as the fourth section - before the fourth section is added the
	// first and third sections are adjacent and they are the same geometry - and if
	// the topology build/edit tool creates the topology when only three sections are
	// added then we have to deal with it here in the topology resolver.
	if (prev_section.d_source_rg.get() == current_section.d_source_rg.get())
	{
		return;
	}

	//
	// Process the actual intersection.
	//
	if (two_sections)
	{
		current_section.d_intersection_results->
				intersect_with_previous_section_allowing_two_intersections(
						prev_section.d_intersection_results);
	}
	else
	{
		current_section.d_intersection_results->intersect_with_previous_section(
				prev_section.d_intersection_results);
	}

	// NOTE: We don't need to look at the end intersection because the next topological
	// section that we visit will have this current section as its start intersection and
	// hence the intersection of this current section and its next section will be
	// taken care of during that visit.
}


void
GPlatesAppLogic::TopologyGeometryResolver::process_resolved_line_topological_section_intersections()
{
	// Iterate over our internal sequence of sections that we built up by
	// visiting the topological sections of a topological geometry property.
	const std::size_t num_sections = d_resolved_geometry.d_sections.size();

	// If there's only one section then don't try to intersect it with itself.
	if (num_sections < 2)
	{
		return;
	}

	PROFILE_FUNC();

	// Resolved topological *lines* do not form a closed loop like boundaries.
	// So there's no need to treat the special case of two topological sections forming a closed loop.

	// Iterate over the sections and process intersections between each section
	// and its previous neighbour.
	for (std::size_t section_index = 0; section_index < num_sections; ++section_index)
	{
		process_resolved_line_topological_section_intersection(section_index);
	}
}


void
GPlatesAppLogic::TopologyGeometryResolver::process_resolved_line_topological_section_intersection(
		const std::size_t current_section_index)
{
	//
	// Intersect the current section with the previous section.
	//

	ResolvedGeometry::Section &current_section = d_resolved_geometry.d_sections[current_section_index];

	//
	// We get the start intersection geometry from the previous section in the topological geometry's
	// list of sections whose valid time ranges include the current reconstruction time.
	//

	// Topological *lines* don't form a closed loop of sections so we don't handle wraparound.
	// For the first section there is no previous section so there's no intersection to process.
	if (current_section_index == 0)
	{
		return;
	}

	const std::size_t prev_section_index = current_section_index - 1;

	ResolvedGeometry::Section &prev_section = d_resolved_geometry.d_sections[prev_section_index];

	// If both sections refer to the same geometry then don't intersect.
	// This can happen when the same geometry is added more than once to the topology
	// when it forms different parts of the resolved topological geometry - normally there
	// are other geometries in between but when building topologies it's possible to
	// add the geometry as first section, then add another geometry as second section,
	// then add the first geometry again as the third section and then remove the second section
	// and then remove the first section - before the first section is removed (after second section
	// removed) the first and third sections are adjacent and they are the same geometry - and if
	// the topology build/edit tool creates the topology before the first section is removed then
	// we have to deal with it here in the topology resolver.
	if (prev_section.d_source_rg.get() == current_section.d_source_rg.get())
	{
		return;
	}

	//
	// Process the actual intersection.
	//
	current_section.d_intersection_results->intersect_with_previous_section(
			prev_section.d_intersection_results);

	// NOTE: We don't need to look at the end intersection because the next topological
	// section that we visit will have this current section as its start intersection and
	// hence the intersection of this current section and its next section will be
	// taken care of during that visit.
}


void
GPlatesAppLogic::TopologyGeometryResolver::create_resolved_topological_boundary()
{
	PROFILE_FUNC();

	// The points to create the plate polygon with.
	std::vector<GPlatesMaths::PointOnSphere> polygon_points;

	// Sequence of subsegments of resolved topology used when creating ResolvedTopologicalBoundary.
	std::vector<ResolvedTopologicalGeometrySubSegment::non_null_ptr_type> output_subsegments;

	// Iterate over the sections of the resolved boundary and construct
	// the resolved polygon boundary and its subsegments.
	const std::size_t num_sections = d_resolved_geometry.d_sections.size();
	for (std::size_t section_index = 0; section_index < num_sections; ++section_index)
	{
		const ResolvedGeometry::Section &section = d_resolved_geometry.d_sections[section_index];

		// Get the subsegment feature reference.
		boost::optional<GPlatesModel::FeatureHandle::weak_ref> subsegment_feature_ref =
				GPlatesAppLogic::ReconstructionGeometryUtils::get_feature_ref(section.d_source_rg);
		// If the feature reference is invalid then skip the current section.
		if (!subsegment_feature_ref)
		{
			continue;
		}

		// Create a subsegment structure that'll get used when creating the resolved topological boundary.
		const ResolvedTopologicalGeometrySubSegment::non_null_ptr_type output_subsegment =
				ResolvedTopologicalGeometrySubSegment::create(
						section.d_intersection_results->get_sub_segment_range_in_section(),
						section.d_intersection_results->get_reverse_flag(),
						subsegment_feature_ref.get(),
						section.d_source_rg);
		output_subsegments.push_back(output_subsegment);

		// Append the subsegment geometry to the plate polygon points.
		// Subsegment should be reversed if that's how it contributed to the resolved topology.
		output_subsegment->get_reversed_sub_segment_points(
				polygon_points,
				ResolvedTopologicalBoundary::INCLUDE_SUB_SEGMENT_RUBBER_BAND_POINTS_IN_RESOLVED_BOUNDARY/*include_rubber_band_points*/);
	}

	// Create a polygon on sphere for the resolved boundary using 'polygon_points'.
	GPlatesUtils::GeometryConstruction::GeometryConstructionValidity polygon_validity;
	boost::optional<GPlatesMaths::PolygonOnSphere::non_null_ptr_to_const_type> plate_polygon =
			GPlatesUtils::create_polygon_on_sphere(
					polygon_points.begin(), polygon_points.end(), polygon_validity);

	// If we are unable to create a polygon (such as insufficient points) then
	// just return without creating a resolved topological geometry.
	if (polygon_validity != GPlatesUtils::GeometryConstruction::VALID)
	{
// These errors never really get fixed in the topology datasets so might as well stop spamming the log.
// Better to write a pyGPlates script to detect these types of errors as a post-process.
#if 0
		qDebug() << "ERROR: Failed to create a ResolvedTopologicalBoundary - probably has "
				"insufficient points for a polygon.";
		qDebug() << "Skipping creation for topological polygon feature_id=";
		qDebug() << GPlatesUtils::make_qstring_from_icu_string(
				d_currently_visited_feature->feature_id().get());
#endif

		return;
	}

	//
	// Create the RTB for the plate polygon.
	//
	ResolvedTopologicalBoundary::non_null_ptr_type rtb_ptr =
		ResolvedTopologicalBoundary::create(
			d_reconstruction_tree,
			d_reconstruction_tree_creator,
			*plate_polygon,
			*(current_top_level_propiter()->handle_weak_ref()),
			*(current_top_level_propiter()),
			output_subsegments.begin(),
			output_subsegments.end(),
			d_reconstruction_params.get_recon_plate_id(),
			d_reconstruction_params.get_time_of_appearance(),
			d_reconstruct_handle/*identify where/when this RTG was resolved*/);

	GPlatesGlobal::Assert<GPlatesGlobal::AssertionFailureException>(
			d_resolved_topological_boundaries,
			GPLATES_ASSERTION_SOURCE);
	d_resolved_topological_boundaries->push_back(rtb_ptr);
}


void
GPlatesAppLogic::TopologyGeometryResolver::create_resolved_topological_line()
{
	PROFILE_FUNC();

	// The points to create the resolved line with.
	std::vector<GPlatesMaths::PointOnSphere> resolved_line_points;

	// Sequence of subsegments of resolved topology used when creating ResolvedTopologicalLine.
	std::vector<ResolvedTopologicalGeometrySubSegment::non_null_ptr_type> output_subsegments;

	// Iterate over the sections of the resolved line and construct
	// the resolved polyline and its subsegments.
	const std::size_t num_sections = d_resolved_geometry.d_sections.size();
	for (std::size_t section_index = 0; section_index < num_sections; ++section_index)
	{
		const ResolvedGeometry::Section &section = d_resolved_geometry.d_sections[section_index];

		// Get the subsegment feature reference.
		boost::optional<GPlatesModel::FeatureHandle::weak_ref> subsegment_feature_ref =
				GPlatesAppLogic::ReconstructionGeometryUtils::get_feature_ref(section.d_source_rg);
		// If the feature reference is invalid then skip the current section.
		if (!subsegment_feature_ref)
		{
			continue;
		}

		// Create a subsegment structure that'll get used when creating the resolved topological line.
		const ResolvedTopologicalGeometrySubSegment::non_null_ptr_type output_subsegment =
				ResolvedTopologicalGeometrySubSegment::create(
						section.d_intersection_results->get_sub_segment_range_in_section(),
						section.d_intersection_results->get_reverse_flag(),
						subsegment_feature_ref.get(),
						section.d_source_rg);
		output_subsegments.push_back(output_subsegment);

		// Append the subsegment geometry to the resolved line points.
		// Subsegment should be reversed if that's how it contributed to the resolved topology.
		output_subsegment->get_reversed_sub_segment_points(
				resolved_line_points,
				ResolvedTopologicalLine::INCLUDE_SUB_SEGMENT_RUBBER_BAND_POINTS_IN_RESOLVED_LINE/*include_rubber_band_points*/);
	}

	// Create a polyline on sphere for the resolved line using 'resolved_line_points'.
	GPlatesUtils::GeometryConstruction::GeometryConstructionValidity polyline_validity;
	boost::optional<GPlatesMaths::PolylineOnSphere::non_null_ptr_to_const_type> resolved_line_geometry =
			GPlatesUtils::create_polyline_on_sphere(
					resolved_line_points.begin(), resolved_line_points.end(), polyline_validity);

	// If we are unable to create a polyline (such as insufficient points) then
	// just return without creating a resolved topological geometry.
	if (polyline_validity != GPlatesUtils::GeometryConstruction::VALID)
	{
// These errors never really get fixed in the topology datasets so might as well stop spamming the log.
// Better to write a pyGPlates script to detect these types of errors as a post-process.
#if 0
		qDebug() << "ERROR: Failed to create a ResolvedTopologicalLine - probably has "
				"insufficient points for a polyline.";
		qDebug() << "Skipping creation for topological line feature_id=";
		qDebug() << GPlatesUtils::make_qstring_from_icu_string(
				d_currently_visited_feature->feature_id().get());
#endif

		return;
	}

	//
	// Create the RTL for the resolved line.
	//
	ResolvedTopologicalLine::non_null_ptr_type rtl_ptr =
		ResolvedTopologicalLine::create(
			d_reconstruction_tree,
			d_reconstruction_tree_creator,
			*resolved_line_geometry,
			*(current_top_level_propiter()->handle_weak_ref()),
			*(current_top_level_propiter()),
			output_subsegments.begin(),
			output_subsegments.end(),
			d_reconstruction_params.get_recon_plate_id(),
			d_reconstruction_params.get_time_of_appearance(),
			d_reconstruct_handle/*identify where/when this RTG was resolved*/);

	GPlatesGlobal::Assert<GPlatesGlobal::AssertionFailureException>(
			d_resolved_topological_lines,
			GPLATES_ASSERTION_SOURCE);
	d_resolved_topological_lines->push_back(rtl_ptr);
}


void
GPlatesAppLogic::TopologyGeometryResolver::debug_output_topological_section_feature_id(
		const GPlatesModel::FeatureId &section_feature_id)
{
	qDebug() << "Topological geometry feature_id=";
	qDebug() << GPlatesUtils::make_qstring_from_icu_string(
			d_currently_visited_feature->feature_id().get());
	qDebug() << "Topological section referencing feature_id=";
	qDebug() << GPlatesUtils::make_qstring_from_icu_string(section_feature_id.get());
}
