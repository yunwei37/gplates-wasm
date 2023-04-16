/* $Id: ChangeLightDirectionMap.cc 15095 2014-03-18 07:11:00Z jcannon $ */

/**
 * \file 
 * $Revision: 15095 $
 * $Date: 2014-03-18 00:11:00 -0700 (Tue, 18 Mar 2014) $
 * 
 * Copyright (C) 2013 The University of Sydney, Australia
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

#include "ChangeLightDirectionMap.h"

#include "qt-widgets/MapView.h"
#include "qt-widgets/ViewportWindow.h"

#include "view-operations/RenderedGeometryCollection.h"


GPlatesCanvasTools::ChangeLightDirectionMap::ChangeLightDirectionMap(
		GPlatesQtWidgets::MapCanvas &map_canvas_,
		GPlatesQtWidgets::MapView &map_view_,
		GPlatesViewOperations::RenderedGeometryCollection &rendered_geometry_collection,
		GPlatesQtWidgets::ViewportWindow &viewport_window_,
		GPlatesGui::MapTransform &map_transform_) :
	MapCanvasTool(map_canvas_, map_view_, map_transform_),
	d_rendered_geometry_collection(rendered_geometry_collection),
	d_viewport_window_ptr(&viewport_window_),
	d_map_transform_ptr(&map_transform_)
{  }
