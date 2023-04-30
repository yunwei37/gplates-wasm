/* $Id: gplates_demo_no_gui_main.cc 19759 2020-12-16 14:00:41Z jcannon $ */

/**
 * @file
 * Contains the main function of the GPlates no-GUI demo.
 *
 * This file used to be "model/compilation_test.cc".  It was used during the implementation of the
 * new GPGIM-1.5-based Model, to check that the new Model code compiled without error and ran
 * correctly.  The code in this file constructs some hard-coded GPGIM features (which are
 * minimalist but otherwise structurally accurate) and outputs them as GPML.
 *
 * Most recent change:
 *   $Date: 2020-12-16 06:00:41 -0800 (Wed, 16 Dec 2020) $
 *
 * Copyright (C) 2006, 2007, 2009, 2010 The University of Sydney, Australia
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

#include <QtXml/QDomDocument>
#include <emscripten.h>
#include <iostream>
#include <streambuf>
#include <iostream>
#include <proj_api.h>
#include <vector>
#include <chrono>
#include "reconstructions.h"

EMSCRIPTEN_KEEPALIVE
    extern "C" void log_to_console(const char* message) {
    std::cout << message << std::endl;
}

EM_JS(void, append_output_text, (const char *text), {
    var element = document.getElementById("output");
    element.innerHTML += UTF8ToString(text) + "<br />";
});

class OutputLabelStreamBuffer : public std::streambuf {
public:
    int overflow(int c) override {
        if (c != EOF) {
            char ch = static_cast<char>(c);
            if (ch == '\n') {
                append_output_text(buffer.c_str());
                buffer.clear();
            } else {
                buffer.push_back(ch);
            }
        }
        return c;
    }

private:
    std::string buffer;
};

struct Coordinate {
    double x;
    double y;
};

std::vector<Coordinate> generate_virtual_coordinates(int num_points) {
    std::vector<Coordinate> coordinates(num_points);
    for (int i = 0; i < num_points; ++i) {
        coordinates[i].x = -180.0 + (rand() % 360);
        coordinates[i].y = -90.0 + (rand() % 180);
    }
    return coordinates;
}

extern "C" {

void convert_coordinates(int num_points) {
    projPJ source, target;
    source = pj_init_plus("+proj=longlat +ellps=WGS84 +datum=WGS84 +no_defs");
    target = pj_init_plus("+proj=lcc +lat_1=33.90363494575049 +lat_2=33.62529000000013 +lat_0=33.26439869496681 +lon_0=-117.8193892996399 +x_0=200000.0000000002 +y_0=500000.0000000001 +ellps=GRS80 +datum=NAD83 +units=m +no_defs");

    std::vector<Coordinate> coordinates = generate_virtual_coordinates(num_points);
    double z = 0;
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < num_points; ++i) {
        pj_transform(source, target, 1, 1, &coordinates[i].x, &coordinates[i].y, &z);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "转换 " << num_points << " 个点耗时：" << duration << " 毫秒" << std::endl;

    pj_free(source);
    pj_free(target);
}

}

void test_reconstruction() {
    // Initialize Qt resources that exist in the static 'qt-resources' library.
    // NOTE: This is done here so that both the GUI and command-line-only paths have initialized resources.
    //
    // NOTE: According to the QtResources documentation calls to Q_INIT_RESOURCE are not needed if
    // the resources are compiled into a shared library (further, if resources only accessed from
    // within shared library then there's also no issue with the shared library not being loaded yet).
    // So Q_INIT_RESOURCE is not called for the python (API) shared library (since this source
    // file is not included in it) but that's no problem since the python shared library is used
    // externally (ie, used by an external python interpreter not the GPlates embedded interpreter)
    // and so the resources are only accessed internally by the shared library.
    //
    Q_INIT_RESOURCE(opengl);
    Q_INIT_RESOURCE(python);
    Q_INIT_RESOURCE(gpgim);
    Q_INIT_RESOURCE(qt_widgets);
    qDebug() << "init resource";
    GPlatesMaths::assert_has_infinity_and_nan();

    GPlatesModel::ModelInterface model;

    std::cout << "read structural types from a GPML file." << std::endl;
    GPlatesModel::Gpgim::instance();
    // Used to read structural types from a GPML file.
    GPlatesFileIO::GpmlPropertyStructuralTypeReader::non_null_ptr_to_const_type gpml_structural_type_reader =
        GPlatesFileIO::GpmlPropertyStructuralTypeReader::create();
    std::cout << "create a new GpmlPropertyStructuralType" << std::endl;

    std::pair<GPlatesModel::FeatureCollectionHandle::weak_ref,
              GPlatesModel::FeatureCollectionHandle::weak_ref>
        isochrons_and_total_recon_seqs =
        ::populate_feature_store(model);
    std::cout << "populate_feature_store in model" << std::endl;

    GPlatesModel::FeatureCollectionHandle::weak_ref isochrons =
        isochrons_and_total_recon_seqs.first;
    GPlatesModel::FeatureCollectionHandle::weak_ref total_recon_seqs =
        isochrons_and_total_recon_seqs.second;

    ::output_as_gpml(isochrons);
    ::output_reconstructions(isochrons, total_recon_seqs);
}

int
main()
{

    qDebug() << QString("Hello from qDebug()");
	OutputLabelStreamBuffer htmlStreamBuffer;
    std::streambuf *oldCoutBuffer = std::cout.rdbuf();
    std::cout.rdbuf(&htmlStreamBuffer);
    std::cout << "begin..." << std::endl;
    // in js: test performance
    // convert_coordinates(10000);
    // convert_coordinates(50000);
    // convert_coordinates(100000);
    // convert_coordinates(500000);
    // convert_coordinates(1000000);
    // convert_coordinates(2000000);
    // convert_coordinates(4000000);
    // convert_coordinates(6000000);
    // convert_coordinates(8000000);
    // convert_coordinates(10000000);
//    test_reconstruction();
#if 1
	// Test GPML 1.6 reader.
	if (argc > 1) {

		std::cout << "Attempting to read \"" << argv[1] << "\"." << std::endl;
		QString filename = argv[1];

		GPlatesFileIO::FileInfo fileinfo(filename);
		GPlatesModel::ModelInterface new_model;
		GPlatesFileIO::ReadErrorAccumulation accum;
		bool contains_unsaved_changes;


		// Create a file with an empty feature collection.
		GPlatesFileIO::File::non_null_ptr_type file = GPlatesFileIO::File::create_file(fileinfo);

		// Read new features from the file into the empty feature collection.
		GPlatesFileIO::GpmlReader::read_file(
				file->get_reference(),
				gpml_structural_type_reader,
				accum,
				contains_unsaved_changes);

		GPlatesModel::FeatureCollectionHandle::weak_ref features =
				file->get_reference().get_feature_collection();
		::output_as_gpml(features);
#endif
#if 0
		QFile file(filename);
		file.open(QIODevice::ReadOnly | QIODevice::Text);
		QXmlStreamReader reader(&file);
		while ( ! reader.atEnd()) {
			reader.readNext();
		   	if (reader.isStartElement())
				break;
		}
		if ( ! reader.atEnd()) {
			GPlatesModel::XmlNode::non_null_ptr_type xml = 
				GPlatesModel::XmlElementNode::create(reader);

			QFile out;
			out.open(1, QIODevice::WriteOnly | QIODevice::Text);
			QXmlStreamWriter writer(&out);
			writer.setAutoFormatting(true);
			writer.writeStartDocument("1.0");
			xml->write_to(writer);
		}
	}
#endif
	return 0;
}
