/* $Id$ */

/**
 * @file 
 * $Revision$
 * $Date$
 * 
 * Copyright (C) 2013, 2014 Geological Survey of Norway
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

#include <QObject>

#include "qt-widgets/HellingerDialog.h"
#include "maths/ProximityHitDetail.h"
#include "view-operations/RenderedGeometry.h"
#include "view-operations/RenderedGeometryFactory.h"
#include "view-operations/RenderedGeometryLayer.h"
#include "view-operations/RenderedGeometryProximity.h"
#include "SelectHellingerGeometries.h"

namespace
{

}

GPlatesCanvasTools::SelectHellingerGeometries::SelectHellingerGeometries(
		const status_bar_callback_type &status_bar_callback,
		GPlatesViewOperations::RenderedGeometryCollection &rendered_geom_collection,
		GPlatesViewOperations::RenderedGeometryCollection::MainLayerType main_rendered_layer_type,
		GPlatesQtWidgets::HellingerDialog &hellinger_dialog) :
	CanvasTool(status_bar_callback),
	d_rendered_geom_collection_ptr(&rendered_geom_collection),
	d_hellinger_dialog_ptr(&hellinger_dialog),
	d_mouse_is_over_editable_pick(false),
	d_pick_is_being_dragged(false)
{
	set_up_connections();
}

void
GPlatesCanvasTools::SelectHellingerGeometries::handle_activation()
{
	set_default_tool_status_message();
}

void
GPlatesCanvasTools::SelectHellingerGeometries::handle_deactivation()
{
}


void
GPlatesCanvasTools::SelectHellingerGeometries::handle_left_click(
		const GPlatesMaths::PointOnSphere &point_on_sphere,
		bool is_on_earth,
		double proximity_inclusion_threshold)
{

	if (d_hellinger_dialog_ptr->is_in_edit_segment_state() ||
			d_hellinger_dialog_ptr->is_in_new_segment_state())
	{
		return;
	}

	if (!is_on_earth)
	{
		return;
	}


	GPlatesMaths::ProximityCriteria proximity_criteria(
			point_on_sphere,
			proximity_inclusion_threshold);
	std::vector<GPlatesViewOperations::RenderedGeometryProximityHit> sorted_hits;
	if (GPlatesViewOperations::test_proximity(
				sorted_hits,
				proximity_criteria,
				*d_hellinger_dialog_ptr->get_pick_layer()))
	{
		const unsigned int index = sorted_hits.front().d_rendered_geom_index;
		d_hellinger_dialog_ptr->set_selected_pick(index);
	}
	else
	{
		d_hellinger_dialog_ptr->clear_selection_layer();
	}

	if (d_hellinger_dialog_ptr->is_in_new_point_state())
	{
		d_hellinger_dialog_ptr->update_edit_layer(point_on_sphere);
	}
}



void
GPlatesCanvasTools::SelectHellingerGeometries::handle_move_without_drag(
		const GPlatesMaths::PointOnSphere &point_on_sphere,
		bool is_on_earth,
		double proximity_inclusion_threshold)
{
	if (d_hellinger_dialog_ptr->is_in_edit_segment_state() ||
			d_hellinger_dialog_ptr->is_in_new_segment_state())
	{
		return;
	}

	GPlatesMaths::ProximityCriteria proximity_criteria(
			point_on_sphere,
			proximity_inclusion_threshold);
	std::vector<GPlatesViewOperations::RenderedGeometryProximityHit> sorted_hits;

	if (d_hellinger_dialog_ptr->is_in_new_point_state())
	{
		if (GPlatesViewOperations::test_vertex_proximity(
					sorted_hits,
					*d_rendered_geom_collection_ptr,
					GPlatesViewOperations::RenderedGeometryCollection::RECONSTRUCTION_LAYER,
					proximity_criteria))
		{
			// highlight the vertex
			qDebug() << "Checking for geometry in MWD";
			GPlatesViewOperations::RenderedGeometryProximityHit hit = sorted_hits.front();
			GeometryFinder finder(hit.d_proximity_hit_detail->index());
			GPlatesViewOperations::RenderedGeometry rg =
					hit.d_rendered_geom_layer->get_rendered_geometry(
						hit.d_rendered_geom_index);
			rg.accept_visitor(finder);
			const boost::optional<GPlatesMaths::PointOnSphere> &pos = finder.get_geometry();
			if (pos)
			{
				qDebug() << "MWD: Found existing vertex";
				d_hellinger_dialog_ptr->set_feature_highlight(pos.get());
			}
		}
		else
		{
			d_hellinger_dialog_ptr->clear_feature_highlight_layer();
		}
	}

	// Check editing layer
	else if (d_hellinger_dialog_ptr->is_in_edit_point_state())
	{
		if (GPlatesViewOperations::test_proximity(
					sorted_hits,
					proximity_criteria,
					*d_hellinger_dialog_ptr->get_editing_layer()))
		{
			//qDebug() << "moving over editable geom";
			d_mouse_is_over_editable_pick = true;
			d_hellinger_dialog_ptr->set_enlarged_edit_geometry();
		}
		else
		{
			//qDebug() << "not moving over editable geom";
			d_mouse_is_over_editable_pick = false;
			d_hellinger_dialog_ptr->set_enlarged_edit_geometry(false);
		}
	}
	else if (GPlatesViewOperations::test_proximity(
				sorted_hits,
				proximity_criteria,
				*d_hellinger_dialog_ptr->get_pick_layer()))
	{
		const unsigned int index = sorted_hits.front().d_rendered_geom_index;
		d_hellinger_dialog_ptr->set_hovered_pick(index);
	}
	else
	{
		d_hellinger_dialog_ptr->clear_hovered_layer_and_table();
	}
}

void
GPlatesCanvasTools::SelectHellingerGeometries::handle_shift_left_click(
		const GPlatesMaths::PointOnSphere &point_on_sphere,
		bool is_on_earth,
		double proximity_inclusion_threshold)
{

	if (d_hellinger_dialog_ptr->is_in_edit_segment_state() ||
			d_hellinger_dialog_ptr->is_in_new_segment_state())
	{
		return;
	}

	if (!is_on_earth)
	{
		return;
	}

	GPlatesMaths::ProximityCriteria proximity_criteria(
			point_on_sphere,
			proximity_inclusion_threshold);



	if (!d_hellinger_dialog_ptr->is_in_new_point_state())
	{
		// Check the hellinger pick layer. The shift-left-click action takes us directly to
		// editing mode.
		std::vector<GPlatesViewOperations::RenderedGeometryProximityHit> sorted_hits;
		if (GPlatesViewOperations::test_proximity(
					sorted_hits,
					proximity_criteria,
					*d_hellinger_dialog_ptr->get_pick_layer()))
		{
			const unsigned int index = sorted_hits.front().d_rendered_geom_index;
			d_hellinger_dialog_ptr->set_selected_pick(index);
			d_hellinger_dialog_ptr->edit_current_pick();
			set_status_bar_message(
						QT_TR_NOOP("Click and drag the highlighted pick on the canvas."));
		}
		else
		{
			d_hellinger_dialog_ptr->clear_selection_layer();
		}
	}
	else
	{
		// If we are in "new point" mode, check the feature geometries.
		std::vector<GPlatesViewOperations::RenderedGeometryProximityHit> sorted_hits;
		if (GPlatesViewOperations::test_vertex_proximity(
					sorted_hits,
					*d_rendered_geom_collection_ptr,
					GPlatesViewOperations::RenderedGeometryCollection::RECONSTRUCTION_LAYER,
					proximity_criteria))
		{
			qDebug() << "SLC: Hit a feature geometry";
			GPlatesViewOperations::RenderedGeometryProximityHit hit = sorted_hits.front();

			// TODO: I think the vertex index is stored in the hit's "ProximityHitDetail"; probably
			// I want to pass this info to the Finder so that I can extract the correct vertex
			// geometry.
			GeometryFinder finder(hit.d_proximity_hit_detail->index());
			GPlatesViewOperations::RenderedGeometry rg =
					hit.d_rendered_geom_layer->get_rendered_geometry(
						hit.d_rendered_geom_index);
			rg.accept_visitor(finder);
			const boost::optional<GPlatesMaths::PointOnSphere> &pos = finder.get_geometry();
			if (pos)
			{
				qDebug() << "SLC: Found vertex";
				d_hellinger_dialog_ptr->update_edit_layer(pos.get());
			}
		}
	}
}

void
GPlatesCanvasTools::SelectHellingerGeometries::handle_left_press(
		const GPlatesMaths::PointOnSphere &point_on_sphere,
		bool is_on_earth,
		double proximity_inclusion_threshold)
{
	if (d_hellinger_dialog_ptr->is_in_edit_segment_state() ||
			d_hellinger_dialog_ptr->is_in_new_segment_state())
	{
		return;
	}

	if (!d_mouse_is_over_editable_pick)
	{
		return;
	}


	GPlatesMaths::ProximityCriteria proximity_criteria(
			point_on_sphere,
			proximity_inclusion_threshold);
	std::vector<GPlatesViewOperations::RenderedGeometryProximityHit> sorted_hits;

	// Check editing layer first
	if (d_hellinger_dialog_ptr->get_editing_layer()->is_active())
	{
		if (GPlatesViewOperations::test_proximity(
					sorted_hits,
					proximity_criteria,
					*d_hellinger_dialog_ptr->get_editing_layer()))
		{
			d_pick_is_being_dragged = true;
		}
		else
		{
			d_mouse_is_over_editable_pick = false;
		}
	}
}

void
GPlatesCanvasTools::SelectHellingerGeometries::handle_shift_left_drag(
		const GPlatesMaths::PointOnSphere &initial_point_on_sphere,
		bool was_on_earth,
		double initial_proximity_inclusion_threshold,
		const GPlatesMaths::PointOnSphere &current_point_on_sphere,
		bool is_on_earth,
		double current_proximity_inclusion_threshold,
		const boost::optional<GPlatesMaths::PointOnSphere> &centre_of_viewport)
{

}

void
GPlatesCanvasTools::SelectHellingerGeometries::handle_left_release_after_drag(
		const GPlatesMaths::PointOnSphere &initial_point_on_sphere,
		bool was_on_earth,
		double initial_proximity_inclusion_threshold,
		const GPlatesMaths::PointOnSphere &current_point_on_sphere,
		bool is_on_earth,
		double current_proximity_inclusion_threshold,
		const boost::optional<GPlatesMaths::PointOnSphere> &centre_of_viewport)
{
	d_pick_is_being_dragged = false;
	d_hellinger_dialog_ptr->set_enlarged_edit_geometry(false);
	d_hellinger_dialog_ptr->update_edit_layer(current_point_on_sphere);

}

void
GPlatesCanvasTools::SelectHellingerGeometries::handle_left_drag(
		const GPlatesMaths::PointOnSphere &initial_point_on_sphere,
		bool was_on_earth,
		double initial_proximity_inclusion_threshold,
		const GPlatesMaths::PointOnSphere &current_point_on_sphere,
		bool is_on_earth,
		double current_proximity_inclusion_threshold,
		const boost::optional<GPlatesMaths::PointOnSphere> &centre_of_viewport)
{
	if (d_pick_is_being_dragged)
	{
		d_hellinger_dialog_ptr->update_edit_layer(current_point_on_sphere);
	}
}

void
GPlatesCanvasTools::SelectHellingerGeometries::paint()
{
#if 0
	// Delay any notification of changes to the rendered geometry collection
	// until end of current scope block
	GPlatesViewOperations::RenderedGeometryCollection::UpdateGuard update_guard;
#endif
}

void
GPlatesCanvasTools::SelectHellingerGeometries::set_up_connections()
{
	QObject::connect(d_hellinger_dialog_ptr,SIGNAL(finished_editing()),
					 this,SLOT(handle_finished_editing()));
	QObject::connect(d_hellinger_dialog_ptr,SIGNAL(begin_new_pick()),
					 this,SLOT(handle_begin_new_pick()));
	QObject::connect(d_hellinger_dialog_ptr,SIGNAL(begin_edit_pick()),
					 this,SLOT(handle_begin_editing()));
}

void
GPlatesCanvasTools::SelectHellingerGeometries::handle_finished_editing()
{
	set_default_tool_status_message();
}

void
GPlatesCanvasTools::SelectHellingerGeometries::handle_begin_editing()
{
	set_status_bar_message(
				QT_TR_NOOP("Click and drag the highlighted pick on the canvas."));
}

void
GPlatesCanvasTools::SelectHellingerGeometries::handle_begin_new_pick()
{
	set_status_bar_message(
				QT_TR_NOOP("Click to select coordinates of a new pick. Shift-click to use an existing feature."));
}

void
GPlatesCanvasTools::SelectHellingerGeometries::set_default_tool_status_message()
{
	set_status_bar_message(QT_TR_NOOP("Click to select a pick. Shift+click to edit a pick."));
}


