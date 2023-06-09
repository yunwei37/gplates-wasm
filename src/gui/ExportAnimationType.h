/* $Id$ */

/**
 * \file 
 * $Revision$
 * $Date$
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

#ifndef GPLATES_GUI_EXPORTANIMATIONTYPE_H
#define GPLATES_GUI_EXPORTANIMATIONTYPE_H

#include <vector>
#include <boost/cstdint.hpp>
#include <QString>


namespace GPlatesGui
{
	namespace ExportAnimationType
	{
		/**
		 * The list of export types.
		 *
		 * This represents the list of available exports.
		 *
		 * NOTE: All enums must be sequential and start at 0 and end at @a NUM_TYPES.
		 */
		enum Type
		{
			RECONSTRUCTED_GEOMETRIES,
			PROJECTED_GEOMETRIES,
			IMAGE, // Screenshot of viewport
			COLOUR_RASTER, // Lat/lon colour raster export
			NUMERICAL_RASTER, // Lat/lon floating-point raster export
			SCALAR_COVERAGES, // Per-point scalar values.
			DEFORMATION,
			VELOCITIES,
			RESOLVED_TOPOLOGIES, // General (non-CitcomS-specific) resolved topology exports
			RESOLVED_TOPOLOGIES_CITCOMS, // CitcomS-specific resolved topology exports
			RELATIVE_TOTAL_ROTATION,
			EQUIVALENT_TOTAL_ROTATION,
			RELATIVE_STAGE_ROTATION,
			EQUIVALENT_STAGE_ROTATION,
			FLOWLINES,
			MOTION_PATHS,
			CO_REGISTRATION,
			NET_ROTATIONS,

			NUM_TYPES,

			INVALID_TYPE // Must be after NUM_TYPES.
		};

		/**
		 * The list of exporter formats.
		 *
		 * These are the supported file formats.
		 * Note that not all formats apply to all export names.
		 *
		 * NOTE: All enums must be sequential and start at 0 and end at @a NUM_FORMATS.
		 */
		enum Format
		{
			GMT,
			SHAPEFILE,
			OGRGMT,
			GEOJSON,
			SVG,
			GPML,
			CSV_COMMA,
			CSV_SEMICOLON,
			CSV_TAB,

			// Colour raster/image formats.
			BMP,
			JPG,
			JPEG,
			PNG,
			PPM,
			TIFF,
			XBM,
			XPM,

			// Numerical raster formats.
			NETCDF_NC,		 // NetCDF/GMT with '.nc' extension.
			NETCDF_GRD,      // NetCDF/GMT with '.grd' extension.
			GEOTIFF,
			ERDAS_IMAGINE,
			ERMAPPER,

			CITCOMS_GLOBAL,  // CitcomS global velocity file.
			TERRA_TEXT,      // Terra velocity text file.

			NUM_FORMATS,

			INVALID_FORMAT // Must be after NUM_FORMATS.
		};

		/**
		 * An identifier, that can be generated via @a get_export_id, to identify
		 * an exporter that is a combination of @a Type and @a Format.
		 */
		typedef boost::uint32_t ExportID;


		/**
		 * Returns the name of the specified export type.
		 */
		const QString &
		get_export_type_name(
				Type type);


		/**
		 * Returns the description of the specified export type.
		 */
		const QString &
		get_export_type_description(
				Type type);


		/**
		 * Returns the description of the specified export format.
		 */
		const QString &
		get_export_format_description(
				Format format);


		/**
		 * Returns the filename extension of the specified export format.
		 *
		 * Note that some formats (like CitcomS velocity or Terra velocity) have no filename extension.
		 */
		const QString &
		get_export_format_filename_extension(
				Format format);


		/**
		 * Returns the export animation type corresponding to the specified export name and format.
		 */
		ExportID
		get_export_id(
				Type type,
				Format format);

		/**
		 * Returns the export type corresponding to the specified export ID.
		 */
		Type
		get_export_type(
				ExportID export_id);

		/**
		 * Returns the export format corresponding to the specified export ID.
		 */
		Format
		get_export_format(
				ExportID export_id);

		/**
		 * Returns a unique list of export types in @a export_ids.
		 */
		std::vector<Type>
		get_export_types(
				const std::vector<ExportID> &export_ids);

		/**
		 * Returns those export formats in @a export_ids that have the specified export type.
		 */
		std::vector<Format>
		get_export_formats(
				const std::vector<ExportID> &export_ids,
				Type export_type);
	};
}

#endif // GPLATES_GUI_EXPORTANIMATIONTYPE_H
