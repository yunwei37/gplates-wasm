/* $Id: DeleteFeatureOperation.h 13766 2013-01-24 02:29:49Z mchin $ */

/**
 * \file
 * Contains the definition of the DeleteFeatureOperation class.
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

#ifndef GPLATES_VIEWOPERATIONS_DELETEFEATUREOPERATION_H
#define GPLATES_VIEWOPERATIONS_DELETEFEATUREOPERATION_H

#include <QObject>

namespace GPlatesGui
{
	class FeatureFocus;
}

namespace GPlatesAppLogic
{
	class ApplicationState;
}

namespace GPlatesViewOperations
{
	/**
	 * This class encapsulates the logic behind deleting the currently focused feature.
	 */
	class DeleteFeatureOperation :
			public QObject
	{
		Q_OBJECT

	public:

		DeleteFeatureOperation(
				GPlatesGui::FeatureFocus &feature_focus,
				GPlatesAppLogic::ApplicationState &application_state);

	public Q_SLOTS:

		void
		delete_focused_feature();

	private:

		GPlatesGui::FeatureFocus &d_feature_focus;
		GPlatesAppLogic::ApplicationState &d_application_state;
	};
}

#endif  // GPLATES_VIEWOPERATIONS_DELETEFEATUREOPERATION_H
