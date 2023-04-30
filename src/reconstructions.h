#ifndef GPLATES_RECONSTRUCTION_DEMO_H
#define GPLATES_RECONSTRUCTION_DEMO_H

#include "model/FeatureCollectionHandle.h"
#include "model/FeatureCollectionRevision.h"
#include "model/FeatureHandle.h"
#include "model/FeatureRevision.h"
#include "model/Model.h"
#include "model/ModelInterface.h"
#include "model/ModelUtils.h"
#include "model/XmlNode.h"
#include "model/Gpgim.h"

#include "file-io/GpmlOutputVisitor.h"
#include "file-io/GpmlPropertyStructuralTypeReader.h"
#include "file-io/PlatesLineFormatWriter.h"
#include "file-io/GpmlReader.h"
#include "file-io/ReadErrorAccumulation.h"
#include "file-io/FileInfo.h"

#include <boost/optional.hpp>

void output_reconstructions(
    GPlatesModel::FeatureCollectionHandle::weak_ref isochrons,
    GPlatesModel::FeatureCollectionHandle::weak_ref total_recon_seqs);

void output_reconstructions_with_times(
    GPlatesModel::FeatureCollectionHandle::weak_ref isochrons,
    GPlatesModel::FeatureCollectionHandle::weak_ref total_recon_seqs,
    QVector<double> recon_times_to_test);

const std::pair<GPlatesModel::FeatureCollectionHandle::weak_ref,
                GPlatesModel::FeatureCollectionHandle::weak_ref>
populate_feature_store(
    GPlatesModel::ModelInterface &model);

void output_as_gpml(
    const GPlatesModel::FeatureCollectionHandle::weak_ref &features);

void output_reconstructions(
    GPlatesModel::FeatureCollectionHandle::weak_ref isochrons,
    GPlatesModel::FeatureCollectionHandle::weak_ref total_recon_seqs);

/**
 * The results of parsing the GUI command-line options.
 *
 * Any command-line options specific to a particular non-GUI command are handled by
 * GPlatesCli::CommandDispatcher (when GPlates is *not* used as the familiar GUI application).
 */
class GuiCommandLineOptions
{
public:
    GuiCommandLineOptions() : debug_gui(false),
        enable_python(false), // Enabled by default.
        enable_external_syncing(false),
        enable_data_mining(false), // Enable data mining by default
        enable_symbol_table(false),
        enable_hellinger_three_plate(false) // Disable three-plate fitting by default
    {
    }

    boost::optional<QString> project_filename;
    QStringList feature_collection_filenames;
    bool debug_gui;
    bool enable_python;
    bool enable_external_syncing;
    bool enable_data_mining;
    bool enable_symbol_table;
    bool enable_hellinger_three_plate;
};

boost::optional<GuiCommandLineOptions>
process_command_line_options(
    int argc,
    char *argv[]);

#endif
