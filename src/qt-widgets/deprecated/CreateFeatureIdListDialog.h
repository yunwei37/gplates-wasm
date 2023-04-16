/* $Id: CreateFeatureIdListDialog.h 10236 2010-11-17 01:53:09Z mchin $ */
 
/**
 * \file 
 * $Revision: 10236 $
 * $Date: 2010-11-16 17:53:09 -0800 (Tue, 16 Nov 2010) $
 * 
 * Copyright (C) 2010 The University of Sydney, Australia
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
#ifndef GPLATES_WIDGETS_CREATEFEATUREIDLISTDIALOG_H
#define GPLATES_WIDGETS_CREATEFEATUREIDLISTDIALOG_H

#include <QObject>
#include <QDialog>

#include <iostream>

#include "CreateFeatureIdListDialogUi.h"
#include "CreateFeatureIdListModel.h"
#include "presentation/ViewState.h"

namespace GPlatesQtWidgets
{
	class CreateFeatureIdListDialog : 
		public QDialog,
		protected Ui_CreateFeatureIdListDialog 
	{
		Q_OBJECT

	public:
		explicit
		CreateFeatureIdListDialog(
				GPlatesPresentation::ViewState &,
				QWidget *parent_ = NULL);
		
		virtual
		~CreateFeatureIdListDialog()
		{	}

	public slots:
		void
		handle_add();
		
		void
		handle_remove();

		void
		handle_save();
		
		void
		handle_open();

		void
		handle_selection_change(
				const QItemSelection &selected,
				const QItemSelection &deselected);

		
	private slots:
		
	private:
		QModelIndex d_current_selection;
		boost::scoped_ptr< CreateFeatureIdListModel > d_model;
		GPlatesPresentation::ViewState& d_view_state;
		
	};
}

#endif //GPLATES_WIDGETS_CREATEFEATUREIDLISTDIALOG_H


