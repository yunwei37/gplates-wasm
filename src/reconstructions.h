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

void output_reconstructions(
    GPlatesModel::FeatureCollectionHandle::weak_ref isochrons,
    GPlatesModel::FeatureCollectionHandle::weak_ref total_recon_seqs);

void
output_reconstructions_with_times(
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

#endif
