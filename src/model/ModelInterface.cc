/* $Id: ModelInterface.cc 5185 2009-03-11 05:48:47Z jboyden $ */

/**
 * \file 
 * File specific comments.
 *
 * Most recent change:
 *   $Date: 2009-03-10 22:48:47 -0700 (Tue, 10 Mar 2009) $
 * 
 * Copyright (C) 2009 The University of Sydney, Australia
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

#include "ModelInterface.h"
#include "Model.h"


GPlatesModel::ModelInterface::ModelInterface():
	d_model_ptr(new Model())
{  }


