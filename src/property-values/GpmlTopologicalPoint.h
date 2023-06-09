
/**
 * \file 
 * File specific comments.
 *
 * Most recent change:
 *   $Date: 2008-07-11 19:36:59 -0700 (Fri, 11 Jul 2008) $
 * 
 * Copyright (C) 2008, 2009, 2010 California Institute of Technology
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

#ifndef GPLATES_PROPERTYVALUES_GPMLTOPOLOGICALPOINT_H
#define GPLATES_PROPERTYVALUES_GPMLTOPOLOGICALPOINT_H

#include "GpmlPropertyDelegate.h"
#include "GpmlTopologicalSection.h"


// Enable GPlatesFeatureVisitors::get_property_value() to work with this property value.
// First parameter is the namespace qualified property value class.
// Second parameter is the name of the feature visitor method that visits the property value.
DECLARE_PROPERTY_VALUE_FINDER(GPlatesPropertyValues::GpmlTopologicalPoint, visit_gpml_topological_point)

namespace GPlatesPropertyValues
{

	class GpmlTopologicalPoint:
			public GpmlTopologicalSection
	{

	public:

		/**
		 * A convenience typedef for GPlatesUtils::non_null_intrusive_ptr<GpmlTopologicalPoint>.
		 */
		typedef GPlatesUtils::non_null_intrusive_ptr<GpmlTopologicalPoint> non_null_ptr_type;

		/**
		 * A convenience typedef for GPlatesUtils::non_null_intrusive_ptr<const GpmlTopologicalPoint>.
		 */
		typedef GPlatesUtils::non_null_intrusive_ptr<const GpmlTopologicalPoint> non_null_ptr_to_const_type;


		virtual
		~GpmlTopologicalPoint()
		{  }

		static
		const non_null_ptr_type
		create(
				GpmlPropertyDelegate::non_null_ptr_type source_geometry) 
		{
			return non_null_ptr_type(new GpmlTopologicalPoint(source_geometry));
		}

		const non_null_ptr_type
		clone() const
		{
			return non_null_ptr_type(
					new GpmlTopologicalPoint(*this));
		}

		const GpmlTopologicalPoint::non_null_ptr_type
		deep_clone() const;

		DEFINE_FUNCTION_DEEP_CLONE_AS_PROP_VAL()

		DEFINE_FUNCTION_DEEP_CLONE_AS_TOPO_SECTION()

		/**
		 * Returns the structural type associated with this property value class.
		 */
		virtual
		StructuralType
		get_structural_type() const
		{
			static const StructuralType STRUCTURAL_TYPE = StructuralType::create_gpml("TopologicalPoint");
			return STRUCTURAL_TYPE;
		}

		/**
		 * Accept a ConstFeatureVisitor instance.
		 *
		 * See the Visitor pattern (p.331) in Gamma95 for information on the purpose of
		 * this function.
		 */
		virtual
		void
		accept_visitor(
				GPlatesModel::ConstFeatureVisitor &visitor) const
		{
			visitor.visit_gpml_topological_point(*this);
		}

		/**
		 * Accept a FeatureVisitor instance.
		 *
		 * See the Visitor pattern (p.331) in Gamma95 for information on the purpose of
		 * this function.
		 */
		virtual
		void
		accept_visitor(
				GPlatesModel::FeatureVisitor &visitor)
		{
			visitor.visit_gpml_topological_point(*this);
		}


		// access to d_source_geometry
		GpmlPropertyDelegate::non_null_ptr_type
		get_source_geometry() const
		{
			return d_source_geometry;
		}

		void
		set_source_geometry(
				GpmlPropertyDelegate::non_null_ptr_type intersection_geom)
		{
			d_source_geometry = intersection_geom;
			update_instance_id();
		} 

	protected:

		// This constructor should not be public, because we don't want to allow
		// instantiation of this type on the stack.
		GpmlTopologicalPoint(
				GpmlPropertyDelegate::non_null_ptr_type source_geometry) :
			GpmlTopologicalSection(),
			d_source_geometry( source_geometry )
		{  }

#if 0
		// This constructor should not be public, because we don't want to allow
		// instantiation of this type on the stack.
		//
		// Note that this should act exactly the same as the default (auto-generated)
		// copy-constructor, except it should not be public.
		GpmlTopologicalPoint(
				const GpmlTopologicalPoint &other) :
			GpmlTopologicalSection(other),
			d_source_geometry(other.d_source_geometry) // will get overwritten in deep_clone() later.
		{  }
#endif

		virtual
		bool
		directly_modifiable_fields_equal(
				const PropertyValue &other) const;

	private:

		// This operator should never be defined, because we don't want/need to allow
		// copy-assignment:  All copying should use the virtual copy-constructor 'clone'
		// (which will in turn use the copy-constructor); all "assignment" should really
		// only be assignment of one intrusive_ptr to another.
		GpmlTopologicalPoint &
		operator=(const GpmlTopologicalPoint &);

		GpmlPropertyDelegate::non_null_ptr_type d_source_geometry;

	};

}

#endif  // GPLATES_PROPERTYVALUES_GPMLTOPOLOGICALPOINT_H
