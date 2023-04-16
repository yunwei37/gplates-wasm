/* $Id: CoRegistrationLayerParams.cc 17704 2016-06-29 15:44:25Z jcannon $ */

/**
 * \file 
 * $Revision: 17704 $
 * $Date: 2016-06-29 08:44:25 -0700 (Wed, 29 Jun 2016) $
 * 
 * Copyright (C) 2016 The University of Sydney, Australia
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

#include "CoRegistrationLayerParams.h"


void
GPlatesAppLogic::CoRegistrationLayerParams::set_cfg_table(
		const GPlatesDataMining::CoRegConfigurationTable &table)
{
	if (d_cfg_table == table)
	{
		return;
	}

	d_cfg_table = table;

	Q_EMIT modified_cfg_table(*this);
	emit_modified();
}
