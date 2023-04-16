/* $Id: PoleSequenceTableWidget.cc 8106 2010-04-21 15:33:48Z rwatson $ */

/**
 * \file 
 * $Revision: 8106 $
 * $Date: 2010-04-21 08:33:48 -0700 (Wed, 21 Apr 2010) $ 
 * 
 * Copyright (C) 2010 Geological Survey of Norway
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

#include "PoleSequenceTableWidget.h"

GPlatesQtWidgets::PoleSequenceTableWidget::PoleSequenceTableWidget(
	QWidget *parent_):
	QWidget(parent_)
{
	setupUi(this);
}

