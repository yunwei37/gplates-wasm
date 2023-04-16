/* $Id: MinReducer.h 13562 2012-11-15 06:56:17Z mchin $ */

/**
 * \file 
 * $Revision: 13562 $
 * $Date: 2012-11-14 22:56:17 -0800 (Wed, 14 Nov 2012) $
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
#ifndef GPLATESDATAMINING_MINREDUCER_H
#define GPLATESDATAMINING_MINREDUCER_H

#include <algorithm>
#include <vector>
#include <QDebug>

#include "CoRegReducer.h"
#include "DataMiningUtils.h"
#include "OpaqueDataToDouble.h"

namespace GPlatesDataMining
{
	class MinReducer : public CoRegReducer
	{
	public:
		class Config : public CoRegReducer::Config
		{
		public:
			bool
			is_same_type(const CoRegReducer::Config* other)
			{
				return dynamic_cast<const MinReducer::Config*>(other);
			}

			~Config(){ }
		};

		~MinReducer(){}

	protected:
		OpaqueData
		exec(
				ReducerInDataset::const_iterator first,
				ReducerInDataset::const_iterator last) 
		{
			std::vector<OpaqueData> data;
			extract_opaque_data(first, last, data);

			std::vector<double> buf;
			DataMiningUtils::convert_to_double_vector(data.begin(), data.end(), buf);
			
			return buf.empty()
					? OpaqueData(EmptyData)
					: OpaqueData(*std::min_element(buf.begin(),buf.end()));
		}
	};
}
#endif





