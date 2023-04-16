/* $Id: render_sphere_normals_fragment_shader.glsl 14199 2013-05-27 09:18:05Z jcannon $ */

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

/*
 * Fragment shader source code to render sphere normals as part of clearing a render target before
 * rendering a normal-map raster.
 *
 * For example, for a *regional* normal map raster the normals outside the region are not rendered by
 * the normal map raster itself and so must be initialised to be normal to the globe's surface.
 */

void main (void)
{
	// Normalize the interpolated 3D texture coordinate (which represents position on a rendered cube).
	// The cube vertices are unit length but not between vertices.
	vec3 sphere_normal = normalize(gl_TexCoord[0].xyz);

	// All components of world-space normal are signed and need to be converted to
	// unsigned ([-1,1] -> [0,1]) before storing in fixed-point 8-bit RGBA render target.
	gl_FragColor.xyz = 0.5 * sphere_normal + 0.5;
	gl_FragColor.w = 1;
}
