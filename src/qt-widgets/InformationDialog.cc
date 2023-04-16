/* $Id: InformationDialog.cc 8643 2010-06-04 08:56:28Z elau $ */

/**
 * \file 
 * $Revision: 8643 $
 * $Date: 2010-06-04 01:56:28 -0700 (Fri, 04 Jun 2010) $ 
 * 
 * Copyright (C) 2006, 2007, 2010 The University of Sydney, Australia
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
 
#include "InformationDialog.h"


GPlatesQtWidgets::InformationDialog::InformationDialog(
		const QString &text_,
		const QString &title_,
		QWidget *parent_):
	QDialog(parent_, Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint)
{
	setupUi(this);

	text_information->setText(text_);
	setWindowTitle(title_);
}


void
GPlatesQtWidgets::InformationDialog::set_text(
		const QString &text_)
{
	text_information->setText(text_);
}


void
GPlatesQtWidgets::InformationDialog::set_title(
		const QString &title_)
{
	setWindowTitle(title_);
}

