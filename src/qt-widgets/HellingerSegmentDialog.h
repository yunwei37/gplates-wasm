/* $Id: HellingerNewSegmentDialog.h 227 2012-02-24 14:46:55Z juraj.cirbus $ */

/**
 * \file
 * $Revision: 227 $
 * $Date: 2012-02-24 15:46:55 +0100 (Fri, 24 Feb 2012) $
 *
 * Copyright (C) 2011, 2012, 2013 Geological Survey of Norway
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

#ifndef GPLATES_QTWIDGETS_HELLINGERSEGMENTDIALOG_H
#define GPLATES_QTWIDGETS_HELLINGERSEGMENTDIALOG_H

#include "boost/optional.hpp"

#include <QItemDelegate> // for spinbox delegate
#include <QStandardItemModel>

#include "ui_HellingerSegmentDialogUi.h"
#include "HellingerDialog.h"
#include "HellingerModel.h"

namespace GPlatesQtWidgets
{


	class HellingerDialog;
	class HellingerNewSegmentWarning;

	/**
	 * @brief The SpinBoxDelegate class
	 *
	 * This lets us customise the spinbox behaviour in the TableView. Borrowed largely from the
	 * Qt example here:
	 * http://qt-project.org/doc/qt-4.8/itemviews-spinboxdelegate.html
	 *
	 */
	class SpinBoxDelegate : public QItemDelegate
	{
		Q_OBJECT

	public:


		SpinBoxDelegate(
				bool three_way_fitting_is_enabled,
				QObject *parent = 0);

		QWidget *createEditor(
				QWidget *parent,
				const QStyleOptionViewItem &option,
				const QModelIndex &index) const;

		void setEditorData(
				QWidget *editor,
				const QModelIndex &index) const;

		void setModelData(
				QWidget *editor,
				QAbstractItemModel *model,
				const QModelIndex &index) const;

		void updateEditorGeometry(
				QWidget *editor,
				const QStyleOptionViewItem &option,
				const QModelIndex &index) const;

		void paint(
				QPainter *painter,
				const QStyleOptionViewItem &option,
				const QModelIndex &index) const;

	private:
		bool d_three_way_fitting_is_enabled;
	};





	class HellingerSegmentDialog:
			public QDialog,
			protected Ui_HellingerSegmentDialog
	{
		Q_OBJECT

	public:

		enum ColumnType{
			COLUMN_PLATE_INDEX = 0,
			COLUMN_LAT,
			COLUMN_LON,
			COLUMN_UNCERTAINTY,
			COLUMN_ENABLED,

			NUM_COLUMNS
		};

		HellingerSegmentDialog(
				HellingerDialog *hellinger_dialog,
				HellingerModel *hellinger_model,
				bool create_new_segment = false);

		void
		initialise_with_segment(
				const int &segment_number);

		void
		initialise();

		boost::optional<GPlatesQtWidgets::HellingerPick>
		current_pick() const;

		void
		update_pick_coords(
				const GPlatesMaths::LatLonPoint &llp);

		void
		begin_segment_operation();

	Q_SIGNALS:

		void
		finished_editing();

	private Q_SLOTS:

		void
		handle_selection_changed(const QItemSelection &, const QItemSelection &);

		void
		handle_add_segment();

		void
		handle_add_line();

		void
		handle_remove_line();

		void
		add_segment_to_model();

		void
		change_pick_type_of_whole_table();

		void
		update_buttons();

		void
		handle_reset();

		void
		handle_enable();

		void
		handle_disable();

		void
		close();

		void
		reject();

	private:

		void
		fill_widgets();

		void
		handle_edited_segment();

		void
		handle_new_segment();

		void
		set_initial_row_values(const int &row);

		void
		set_row_values(const int &row, const GPlatesQtWidgets::HellingerPick &pick);

		HellingerDialog *d_hellinger_dialog_ptr;
		QStandardItemModel *d_table_model;
		HellingerModel *d_hellinger_model_ptr;
		HellingerNewSegmentWarning *d_hellinger_new_segment_warning;

		SpinBoxDelegate *d_spin_box_delegate;

		bool d_creating_new_segment;

		boost::optional<int> d_original_segment_number;

		boost::optional<HellingerPick> d_current_pick;

		unsigned int d_current_row;

		bool d_three_way_fitting_is_enabled;

	};
}

#endif //GPLATES_QTWIDGETS_HELLINGERSEGMENTDIALOG_H
