/* $Id:  $ */

/**
 * \file 
 * $Revision: 7584 $
 * $Date: 2010-02-10 19:29:36 +1100 (Wed, 10 Feb 2010) $
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
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <QDebug>

#include "app-logic/CoRegistrationData.h"

#include "data-mining/DataMiningUtils.h"
#include "data-mining/DataSelector.h"
#include "data-mining/OpaqueDataToQString.h"
#include "data-mining/RegionOfInterestFilter.h"

#include "file-io/ReadErrorAccumulation.h"
#include "file-io/FeatureCollectionFileFormatRegistry.h"

#include "global/NotYetImplementedException.h"
#include "global/python.h"

#include "model/ModelInterface.h"


using namespace GPlatesAppLogic;
using namespace GPlatesFileIO;
using namespace GPlatesDataMining;
using namespace GPlatesModel;

using namespace boost::python;

// TODO: Re-implement this when the lower-level python API is implemented.
// This will use the same functionality to access co-registration without reference to layers.
#if 0
namespace
{
	class CoRegistration
	{
	public:

		CoRegistration() :
			d_registry(new FeatureCollectionFileFormat::Registry())
		{ }

		~CoRegistration()
		{
			delete d_registry;
		}

		bool
		exec(const char* cfg_file)
		{
			load_cfg_file(cfg_file);
			return exec();
		}

		bool 
		exec()
		{
			double current = d_e_time;
			for(; d_s_time >= current; current += d_inc_time)
			{
				gen_data(current);
			}
			return true;
		}

		bool 
		exec(double time)
		{
			gen_data(time);
			return true;
		}

		void
		clear()
		{
			d_rotation_files.clear();
			d_coreg_files.clear();
			d_seed_files.clear();
			d_rotation_fc.clear(), d_seed_fc.clear(),d_coreg_fc.clear();
			d_output_prefix.clear(), d_output_path.clear(), d_cfg_file.clear();
			d_s_time = 0.0, d_e_time = 0.0, d_inc_time = 0.0;
			d_cfg_table.clear(); d_result_table.clear();
		}

		void set_start_time(double t){d_s_time=t;}
		void set_end_time(double t){d_e_time=t;}
		void set_inc_time(double t){d_inc_time=t;}
		
		void 
		load_recon_files(list& fl)
		{
			d_rotation_files.clear();
			std::vector<QString> files;
			for (int i = 0; i < len(fl); ++i)
			{
				files.push_back(QString(extract<const char*>(fl[i])));
			}
			d_rotation_fc = DataMiningUtils::load_files(files, d_rotation_files, *d_registry);
		}

		void 
		load_seed_files(list& fl)
		{
			std::vector<QString> files;
			for (int i = 0; i < len(fl); ++i)
			{
				files.push_back(QString(extract<const char*>(fl[i])));
			}
			d_seed_fc = DataMiningUtils::load_files(files, d_seed_files, *d_registry);
		}

		void 
		load_coreg_files(list& fl)
		{
			std::vector<QString> files;
			for (int i = 0; i < len(fl); ++i)
			{
				files.push_back(QString(extract<const char*>(fl[i])));
			}
			d_coreg_fc = DataMiningUtils::load_files(files, d_coreg_files, *d_registry);
		}

		void
		load_cfg_file(const char* cfg_file)
		{
			d_cfg_file = cfg_file;
			load_cfg_file();
		}

		void
		add_cfg_row(const char* line)
		{
			d_cfg_table.push_back(parse_cfg_row(QString(line)));
		}

		void
		clear_cfg_rows()
		{
			d_cfg_table.clear();
		}

		void
		set_output_path(const char* path)
		{
			d_output_path = QString(path);
		}

		void
		set_output_prefix(const char* prefix)
		{
			d_output_prefix = QString(prefix);
		}

		void
		export_data()
		{
			BOOST_FOREACH(const DataTable& table, d_result_table)
			{
				table.export_as_CSV(
						d_output_path + d_output_prefix + 
						QString().setNum(table.reconstruction_time()) + ".csv");
			}
			return;
		}

		// The BOOST_FOREACH macro in versions of boost before 1.37 uses the same local
		// variable name in each instantiation. Nested BOOST_FOREACH macros therefore
		// cause GCC to warn about shadowed declarations.
		DISABLE_GCC_WARNING("-Wshadow")

		void
		print()
		{
			std::cout << "Start time: " <<d_s_time <<
				"  End time: " << d_e_time << "  Increment time: " << d_inc_time << std::endl;

			std::cout << "Output path: " << d_output_path.toStdString() << std::endl;
			std::cout << "Output prefix: " << d_output_prefix.toStdString() << std::endl;
			std::cout << "Configuration file: " << d_cfg_file.toStdString() << std::endl;

			std::cout << "Seed files: "  << std::endl;
			BOOST_FOREACH(File::non_null_ptr_type& file, d_seed_files)
			{
				std::cout << file->get_reference().get_file_info().get_display_name(true).toStdString() << std::endl;
			}

			std::cout << "co-registration files: "  << std::endl;
			BOOST_FOREACH(File::non_null_ptr_type& file, d_coreg_files)
			{
				std::cout << file->get_reference().get_file_info().get_display_name(true).toStdString() << std::endl;
			}

			std::cout << "Reconstruction files: "  << std::endl;
			BOOST_FOREACH(File::non_null_ptr_type& file, d_rotation_files)
			{
				std::cout << file->get_reference().get_file_info().get_display_name(true).toStdString() << std::endl;
			}

			std::cout << "Configuration table: "  << std::endl;
			BOOST_FOREACH(const ConfigurationTableRow& row, d_cfg_table)
			{
				std::cout << row << std::endl;
			}
			std::cout << "Result data: "  << std::endl;
			BOOST_FOREACH(const DataTable& table, d_result_table)
			{
				std::cout << "time: "<< table.reconstruction_time() << std::endl;
				std::cout << table << std::endl;
			}
			return;
		}

		boost::python::list
		feature_ids(boost::python::object file)
		{
			boost::python::list ret;
			File::non_null_ptr_type file_ptr = 
				DataMiningUtils::load_file(QString(boost::python::extract<const char*>(file)), *d_registry);
			FeatureCollectionHandle::weak_ref fch = file_ptr->get_reference().get_feature_collection();
			BOOST_FOREACH(FeatureHandle::non_null_ptr_to_const_type fh, *fch)
			{
				ret.append(
							boost::python::object(
									fh->feature_id().get().qstring().toStdString()));
			}
			return ret;
		}

		boost::python::list
		roi_filter(
				boost::python::object time, 
				boost::python::object range, 
				boost::python::object seed_id)
		{
			double t = boost::python::extract<double>(time);
			double r = boost::python::extract<double>(range);
			QString s_id = QString(boost::python::extract<const char*>(seed_id));
			boost::python::list ret_list;
			qDebug() << t; qDebug() << r; qDebug() << s_id;

			ReconstructMethodRegistry reconstruct_method_registry;

			ReconstructionTreeCreator reconstruction_tree_creator =
				get_cached_reconstruction_tree_creator(
						d_rotation_fc,
						t,
						0/*anchor plate*/);
			//seed
			std::vector<ReconstructedFeatureGeometry::non_null_ptr_type> reconstructed_seeds;
			ReconstructUtils::reconstruct(
					reconstructed_seeds,
					t,
					0, //anchor plate
					reconstruct_method_registry,
					d_seed_fc,
					reconstruction_tree_creator);

			// co-registration features collection.
			std::vector<ReconstructedFeatureGeometry::non_null_ptr_type> reconstructed_coreg;
			ReconstructUtils::reconstruct(
					reconstructed_coreg,
					t,
					0, //anchor plate
					reconstruct_method_registry,
					d_coreg_fc,
					reconstruction_tree_creator);

			RegionOfInterestFilter::Config filter_cfg(r);
			std::vector<const ReconstructedFeatureGeometry*> s_rfgs = get_seed_rfg(s_id,reconstructed_seeds);
			boost::scoped_ptr<CoRegFilter> f( filter_cfg.create_filter(s_rfgs) );
			
			std::vector<const ReconstructedFeatureGeometry*> co_rfgs, ret_rfgs;
			BOOST_FOREACH(ReconstructedFeatureGeometry::non_null_ptr_type rfg, reconstructed_coreg)
			{
				co_rfgs.push_back(rfg.get());
			}
			f->process(co_rfgs.begin(), co_rfgs.end(), ret_rfgs);

			std::set<QString> r_ids;
			BOOST_FOREACH(const ReconstructedFeatureGeometry* r, ret_rfgs)
			{
				r_ids.insert(r->feature_handle_ptr()->feature_id().get().qstring());
			}
			BOOST_FOREACH(const QString& s, r_ids)
			{
				ret_list.append(boost::python::object(s.toStdString()));
			}

			return ret_list;
		}

		list
		get_result_data_from_layer()
		{
			list ret;
			std::vector<std::vector<QString> > table;
			GPlatesDataMining::DataSelector::get_data_table().to_qstring_table(table);
			std::vector<std::vector<QString> >::const_iterator iter_1 = table.begin(), iter_end_1 = table.end();
			for(; iter_1 != iter_end_1; iter_1++)
			{
				list tmp;
				std::vector<QString>::const_iterator iter_2 = iter_1->begin(), iter_end_2 = iter_1->end();
				for(; iter_2 != iter_end_2; iter_2++)
				{
					tmp.append(boost::python::object(iter_2->toStdString()));
				}
				ret.append(tmp);
			}
			return ret;
		}


		list
		get_data_header()
		{
			list ret;
			const GPlatesDataMining::TableHeader& header = GPlatesDataMining::DataSelector::get_data_table().table_header();
			GPlatesDataMining::TableHeader::const_iterator iter= header.begin(), iter_end = header.end();
			for(; iter != iter_end; iter++)
			{
				ret.append(boost::python::object(iter->toStdString()));
			}
			return ret;
		}

	protected:
		std::vector<const ReconstructedFeatureGeometry*>
		get_seed_rfg(
				const QString& id,
				std::vector<ReconstructedFeatureGeometry::non_null_ptr_type>& rfgs)
		{
			std::vector<const ReconstructedFeatureGeometry*> ret;
			BOOST_FOREACH(const ReconstructedFeatureGeometry::non_null_ptr_type r, rfgs)
			{
				GPlatesModel::FeatureHandle* f = r->feature_handle_ptr();
				if(id == f->feature_id().get().qstring())
				{
					ret.push_back(r.get());
				}
			}
			return ret;
		}

		void
		gen_data(double time)
		{
			ReconstructMethodRegistry reconstruct_method_registry;

			ReconstructionTreeCreator reconstruction_tree_creator =
				get_cached_reconstruction_tree_creator(
						d_rotation_fc,
						time,
						0/*anchor plate*/);
			//seed
			std::vector<ReconstructedFeatureGeometry::non_null_ptr_type> reconstructed_seeds;
			ReconstructUtils::reconstruct(
					reconstructed_seeds,
					time,
					0, //anchor plate
					reconstruct_method_registry,
					d_seed_fc,
					reconstruction_tree_creator);

			// co-registration features collection.
			std::vector<ReconstructedFeatureGeometry::non_null_ptr_type> reconstructed_coreg;
			ReconstructUtils::reconstruct(
					reconstructed_coreg,
					time,
					0, //anchor plate
					reconstruct_method_registry,
					d_coreg_fc,
					reconstruction_tree_creator);

			boost::shared_ptr< DataSelector > selector(	DataSelector::create(d_cfg_table) );
			DataTable result;
			selector->select(reconstructed_seeds, reconstructed_coreg, result);
			result.set_reconstruction_time(time);
			d_result_table.push_back(result);
		}

		void
		load_cfg_file()
		{
			using namespace DataMiningUtils;
			QStringList times = load_one_line_cfg(d_cfg_file, "time_range").split(',');
			if(times.size()==3)
			{
				d_s_time		=	times[0].trimmed().toDouble();
				d_e_time		=	times[1].trimmed().toDouble();
				d_inc_time		=	times[2].trimmed().toDouble();
			}
			else
			{
				d_s_time = 140; d_e_time = 0; d_inc_time = 10;
			}

			d_rotation_fc = 
				load_files(
						load_cfg(d_cfg_file, "rotation files"),
						d_rotation_files, *d_registry);

			d_seed_fc =
				load_files(
						load_cfg(d_cfg_file, "seed files"),
						d_seed_files, *d_registry);

			d_coreg_fc =
				load_files(
						load_cfg(d_cfg_file, "coreg files"),
						d_coreg_files, *d_registry);

			if(d_rotation_fc.empty()) 
				qDebug() << "No rotation file.";
			if(d_seed_fc.empty())
				qDebug() << "No seed file.";
			if(d_coreg_fc.empty())
				qDebug() << "No coreg file.";

			d_output_path = load_one_line_cfg(d_cfg_file,"output path");
			if(d_output_path.size() == 0)
				d_output_path = "./";
			d_output_prefix = load_one_line_cfg(d_cfg_file, "output prefix");
			if(d_output_prefix.size() == 0)
				d_output_prefix = "coreg";
			populate_cfg_table(d_cfg_table,d_cfg_file);
		}

		void
		populate_cfg_table(
				CoRegConfigurationTable& table,
				const QString& filename)
		{
			std::vector<QString> lines = DataMiningUtils::load_cfg(d_cfg_file,"input table");
			BOOST_FOREACH(const QString& line, lines)
			{
				table.push_back(parse_cfg_row(line));
			}
		}

		ConfigurationTableRow
		parse_cfg_row(const QString& line)
		{
			enum ColName
			{
				FC_NAME,//0
				COREG_OP,//1
				ATTR_NAME,//2
				DATA_OP,//3
				SHAPE_ATTR,//4
				COL_NUM//5
			};
			
			std::map<QString, AttributeType> attr_map;
			attr_map["DISTANCE"] =			DISTANCE_ATTRIBUTE;
			attr_map["PRESENCE"] =			PRESENCE_ATTRIBUTE;
			attr_map["NUMBER_IN_REGION"] =	NUMBER_OF_PRESENCE_ATTRIBUTE;

			std::map<QString, ReducerType> reducer_map;
			reducer_map["MIN"] =				REDUCER_MIN;
			reducer_map["MAX"] = 				REDUCER_MAX;
			reducer_map["MEAN"] = 				REDUCER_MEAN;
			reducer_map["MEDIAN"] = 			REDUCER_MEDIAN;
			reducer_map["LOOKUP"] = 			REDUCER_LOOKUP;
			reducer_map["VOTE"] = 				REDUCER_VOTE;
			reducer_map["WEIGHTED_MEAN"] =		REDUCER_WEIGHTED_MEAN;
			reducer_map["PERCENTILE"] = 		REDUCER_PERCENTILE;

			ConfigurationTableRow row;
			QString tmp_line = line.trimmed().simplified();
			QStringList items = tmp_line.split(',');
			if(items.size() != COL_NUM)
			{
				qWarning() << "Invalid configuration line.";
				return row;
			}
			BOOST_FOREACH(GPlatesFileIO::File::non_null_ptr_type file, d_coreg_files)
			{
				if(file->get_reference().get_file_info().get_display_name(false) == items[FC_NAME])
				{
					qDebug() << "Find the feature collection.";
					row.target_layer = file->get_reference().get_feature_collection();
				} 
			}
			QString& s_ref = items[COREG_OP];
			s_ref.insert(s_ref.indexOf('('),' '); // The space is essential for stringstream to work.
			std::stringstream ss(s_ref.trimmed().toStdString().c_str());
			std::string op_type_str;
			double d = 0.0;
			ss >> op_type_str; ss.ignore(256,'('); ss >> d; 
			boost::to_upper(op_type_str);
			row.filter_cfg.reset(new RegionOfInterestFilter::Config(d));

			std::map<QString, AttributeType>::iterator it = attr_map.find(items[ATTR_NAME].trimmed().toUpper());
			row.attr_type = it != attr_map.end() ?  it->second : CO_REGISTRATION_GPML_ATTRIBUTE;
			row.attr_name = items[ATTR_NAME].trimmed();

			row.reducer_type = reducer_map[items[DATA_OP].trimmed().toUpper()];

			if(items[SHAPE_ATTR].trimmed() == "true")
				row.attr_type =  CO_REGISTRATION_SHAPEFILE_ATTRIBUTE;

			return row;
		}
		
	private:
		std::vector<File::non_null_ptr_type> d_rotation_files, d_seed_files, d_coreg_files;
		std::vector<FeatureCollectionHandle::weak_ref> d_rotation_fc, d_seed_fc,d_coreg_fc;
		QString d_output_prefix, d_output_path, d_cfg_file;
		double d_s_time, d_e_time, d_inc_time;
		CoRegConfigurationTable d_cfg_table;
		std::vector<DataTable> d_result_table;
		FeatureCollectionFileFormat::Registry* d_registry;  
	};
}
#endif

void
export_co_registration()
{
#if 1
	// TODO: Re-implement this when the lower-level python API is implemented.
	// This will use the same functionality to access co-registration without reference to layers.
	qWarning() << "export_co_registration: not implemented.";
	throw GPlatesGlobal::NotYetImplementedException(GPLATES_EXCEPTION_SOURCE);

#else
	bool    (CoRegistration::*exec)()				 	 = &CoRegistration::exec;
	bool    (CoRegistration::*exec_file)(const char*)    = &CoRegistration::exec;
	bool    (CoRegistration::*exec_time)(double)		 = &CoRegistration::exec;
	void    (CoRegistration::*load_cfg_file)(const char*)= &CoRegistration::load_cfg_file;

	class_<CoRegistration>("CoRegistration")
		.def("run",							exec)
		.def("run",							exec_file)
		.def("run",							exec_time)
		.def("clear",						&CoRegistration::clear)
		.def("load_coreg_files",			&CoRegistration::load_coreg_files)
		.def("load_recon_files",			&CoRegistration::load_recon_files)
		.def("load_seed_files",				&CoRegistration::load_seed_files)
		.def("load_cfg_file",				load_cfg_file)
		.def("set_start_time",				&CoRegistration::set_start_time)
		.def("set_end_time",				&CoRegistration::set_end_time)
		.def("set_inc_time",				&CoRegistration::set_inc_time)
		.def("set_output_path",				&CoRegistration::set_output_path)
		.def("set_output_prefix",			&CoRegistration::set_output_prefix)
		.def("add_cfg_row",					&CoRegistration::add_cfg_row)
		.def("dump",						&CoRegistration::print)
		.def("export",						&CoRegistration::export_data)
		.def("clear_cfg_rows",				&CoRegistration::clear_cfg_rows)
		.def("feature_ids",					&CoRegistration::feature_ids)
		.def("roi_filter",					&CoRegistration::roi_filter)
		.def("get_result_data_from_layer",	&CoRegistration::get_result_data_from_layer)
		.def("get_data_header",				&CoRegistration::get_data_header)
		;
#endif
}
