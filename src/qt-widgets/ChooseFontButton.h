/* $Id: ChooseFontButton.h 13766 2013-01-24 02:29:49Z mchin $ */

/**
 * \file
 *
 * $Revision: 13766 $
 * $Date: 2013-01-23 18:29:49 -0800 (Wed, 23 Jan 2013) $ 
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
 
#ifndef GPLATES_QTWIDGETS_CHOOSEFONTBUTTON_H
#define GPLATES_QTWIDGETS_CHOOSEFONTBUTTON_H

#include <QToolButton>
#include <QFont>


namespace GPlatesQtWidgets
{
	class ChooseFontButton :
			public QToolButton
	{
		Q_OBJECT

	public:

		explicit
		ChooseFontButton(
				QWidget *parent_ = NULL);

		void
		set_font(
				const QFont &font_);

		const QFont &
		get_font() const
		{
			return d_font;
		}

	private Q_SLOTS:

		void
		handle_clicked();

	private:

		QFont d_font;
	};
}

#endif  // GPLATES_QTWIDGETS_CHOOSEFONTBUTTON_H
