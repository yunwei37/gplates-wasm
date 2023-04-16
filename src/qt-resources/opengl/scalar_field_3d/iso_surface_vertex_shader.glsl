/* $Id: iso_surface_vertex_shader.glsl 14199 2013-05-27 09:18:05Z jcannon $ */

/**
 * \file 
 * $Revision: 14199 $
 * $Date: 2013-05-27 02:18:05 -0700 (Mon, 27 May 2013) $
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

//
// Vertex shader for rendering a single iso-surface.
//

// Screen-space coordinates are post-projection coordinates in the range [-1,1].
varying vec2 screen_coord;

void main()
{
	gl_Position = gl_Vertex;

	screen_coord = gl_Vertex.xy;
}
