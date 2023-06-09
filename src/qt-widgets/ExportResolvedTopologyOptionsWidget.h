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

#ifndef GPLATES_QT_WIDGETS_EXPORTRESOLVEDTOPOLOGYOPTIONSWIDGET_H
#define GPLATES_QT_WIDGETS_EXPORTRESOLVEDTOPOLOGYOPTIONSWIDGET_H

#include "ui_ExportResolvedTopologyOptionsWidgetUi.h"

#include "ExportOptionsWidget.h"

#include "gui/ExportResolvedTopologyAnimationStrategy.h"


namespace GPlatesQtWidgets
{
	class DatelineWrapOptionsWidget;
	class ExportFileOptionsWidget;
	class InformationDialog;


	/**
	 * General (non-CitcomS-specific) resolved topology export options.
	 */
	class ExportResolvedTopologyOptionsWidget :
			public ExportOptionsWidget,
			protected Ui_ExportResolvedTopologyOptionsWidget
	{
		Q_OBJECT

	public:
		/**
		 * Creates a @a ExportResolvedTopologyOptionsWidget containing default export options.
		 */
		static
		ExportOptionsWidget *
		create(
				QWidget *parent,
				GPlatesGui::ExportAnimationContext &export_animation_context,
				const GPlatesGui::ExportResolvedTopologyAnimationStrategy::const_configuration_ptr &
						export_configuration,
				bool configure_dateline_wrapping)
		{
			return new ExportResolvedTopologyOptionsWidget(
					parent, export_configuration, configure_dateline_wrapping);
		}


		/**
		 * Collects the options specified by the user and
		 * returns them as an export animation strategy configuration.
		 */
		virtual
		GPlatesGui::ExportAnimationStrategy::const_configuration_base_ptr
		create_export_animation_strategy_configuration(
				const QString &filename_template);

	private Q_SLOTS:

		void
		react_export_resolved_geometry_check_box_state_changed(
				int state);

		void
		react_force_polygon_orientation_check_box_state_changed(
				int state);

		void
		react_polygon_orientation_combobox_state_changed(
				int index);

	private:
		explicit
		ExportResolvedTopologyOptionsWidget(
				QWidget *parent_,
				const GPlatesGui::ExportResolvedTopologyAnimationStrategy::const_configuration_ptr &
						export_configuration,
				bool configure_dateline_wrapping);


		void
		make_signal_slot_connections();

		GPlatesGui::ExportResolvedTopologyAnimationStrategy::Configuration d_export_configuration;

		DatelineWrapOptionsWidget *d_dateline_wrap_options_widget;
		ExportFileOptionsWidget *d_export_file_options_widget;

		InformationDialog* d_help_export_topological_line_sub_segments_dialog;
	};
}

#endif // GPLATES_QT_WIDGETS_EXPORTRESOLVEDTOPOLOGYOPTIONSWIDGET_H
