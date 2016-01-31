/* Copyright (C) 2016 Doubango Telecom <https://www.doubango.org>
*
* This file is part of Open Source ComputerVision (a.k.a CompV) project.
* Source code hosted at https://github.com/DoubangoTelecom/compv
* Website hosted at http://compv.org
*
* CompV is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* CompV is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with CompV.
*/
#include "compv/features/compv_feature.h"
#include "compv/features/fast/compv_feature_fast_dete.h"
#include "compv/features/orb/compv_feature_orb_dete.h"
#include "compv/features/orb/compv_feature_orb_desc.h"
#include "compv/compv_engine.h"

COMPV_NAMESPACE_BEGIN()

std::map<int, const CompVFeatureFactory*> CompVFeature::s_Factories;

// Declare built-in factories
static const CompVFeatureFactory fastFactory = {
    COMPV_FAST_ID,
    "FAST (Features from Accelerated Segment Test)",
    CompVFeatureDeteFAST::newObj,
    NULL,
};
static const CompVFeatureFactory orbFactory = {
    COMPV_ORB_ID,
    "ORB (Oriented FAST and Rotated BRIEF)",
    CompVFeatureDeteORB::newObj,
    CompVFeatureDescORB::newObj,
};

//
//	CompVFeatureDescriptions
//

CompVFeatureDescriptions::CompVFeatureDescriptions(int nFeatures_, int nFeatureBits_)
    : m_nFeatureBits(0)
    , m_nFeaturesCount(0)
{
    if (COMPV_ERROR_CODE_IS_OK(CompVBuffer::newObjAndCopyData(NULL, nFeatures_ * ((nFeatureBits_ + 7) >> 3), &m_data))) {
        m_nFeaturesCount = nFeatures_;
        m_nFeatureBits = nFeatureBits_;
    }
}

CompVFeatureDescriptions::~CompVFeatureDescriptions()
{

}

COMPV_ERROR_CODE CompVFeatureDescriptions::newObj(int nFeaturesCount, int nFeatureBits, CompVObjWrapper<CompVFeatureDescriptions*>* descriptions)
{
    COMPV_CHECK_EXP_RETURN(!descriptions || nFeaturesCount <= 0 || nFeatureBits <= 0, COMPV_ERROR_CODE_E_INVALID_PARAMETER);
    CompVObjWrapper<CompVFeatureDescriptions*> descriptions_ = new CompVFeatureDescriptions(nFeaturesCount, nFeatureBits);
    if (!descriptions_ || descriptions_->m_nFeatureBits != nFeatureBits || descriptions_->m_nFeaturesCount != nFeaturesCount || !descriptions_->m_data) {
        COMPV_CHECK_CODE_RETURN(COMPV_ERROR_CODE_E_OUT_OF_MEMORY);
    }
    *descriptions = descriptions_;
    return COMPV_ERROR_CODE_S_OK;
}

//
//	CompVFeature
//

CompVFeature::CompVFeature()
{

}

CompVFeature::~CompVFeature()
{

}

COMPV_ERROR_CODE CompVFeature::init()
{
    COMPV_DEBUG_INFO("Features initialization");

    /* Register built-in factories */

    // FAST (Features from Accelerated Segment Test)
    COMPV_CHECK_CODE_RETURN(addFactory(&fastFactory));
    // ORB(ORiented BRIEF)
    COMPV_CHECK_CODE_RETURN(addFactory(&orbFactory));

    return COMPV_ERROR_CODE_S_OK;
}

COMPV_ERROR_CODE CompVFeature::addFactory(const CompVFeatureFactory* factory)
{
    COMPV_CHECK_EXP_RETURN(factory == NULL, COMPV_ERROR_CODE_E_INVALID_PARAMETER);
    if (s_Factories.find(factory->id) != s_Factories.end()) {
        const CompVFeatureFactory* old = s_Factories.find(factory->id)->second;
        COMPV_DEBUG_WARN("Feature factory with id = %d already exist and will be replaced old name=%s, new name=%s", factory->id, old->name, factory->name);
    }
    COMPV_DEBUG_INFO("Registering feature factory with id = %d and name = '%s'...", factory->id, factory->name);
    s_Factories[factory->id] = factory;
    return COMPV_ERROR_CODE_S_OK;
}

const CompVFeatureFactory* CompVFeature::findFactory(int deteId)
{
    std::map<int, const CompVFeatureFactory*>::const_iterator it = s_Factories.find(deteId);
    if (it == s_Factories.end()) {
        return NULL;
    }
    return it->second;
}

//
//	CompVFeatureDete
//

CompVFeatureDete::CompVFeatureDete(int id)
    : m_nId(id)
{

}

CompVFeatureDete::~CompVFeatureDete()
{

}

COMPV_ERROR_CODE CompVFeatureDete::newObj(int deteId, CompVObjWrapper<CompVFeatureDete* >* dete)
{
    COMPV_CHECK_CODE_RETURN(CompVEngine::init());
    COMPV_CHECK_EXP_RETURN(dete == NULL, COMPV_ERROR_CODE_E_INVALID_PARAMETER);
    const CompVFeatureFactory* factory_ = CompVFeature::findFactory(deteId);
    if (!factory_) {
        COMPV_DEBUG_ERROR("Failed to find feature factory with id = %d", deteId);
        return COMPV_ERROR_CODE_E_INVALID_PARAMETER;
    }
    if (!factory_->newObjDete) {
        COMPV_DEBUG_ERROR("Factory with id = %d and name = '%s' doesn't have a constructor for detectors", factory_->id, factory_->name);
        return COMPV_ERROR_CODE_E_INVALID_CALL;
    }
    return factory_->newObjDete(dete);
}

//
//	CompVFeatureDesc
//

CompVFeatureDesc::CompVFeatureDesc(int id)
    : m_nId(id)
{

}

CompVFeatureDesc::~CompVFeatureDesc()
{

}

COMPV_ERROR_CODE CompVFeatureDesc::newObj(int descId, CompVObjWrapper<CompVFeatureDesc* >* desc)
{
    COMPV_CHECK_CODE_RETURN(CompVEngine::init());
    COMPV_CHECK_EXP_RETURN(desc == NULL, COMPV_ERROR_CODE_E_INVALID_PARAMETER);
    const CompVFeatureFactory* factory_ = CompVFeature::findFactory(descId);
    if (!factory_) {
        COMPV_DEBUG_ERROR("Failed to find feature factory with id = %d", descId);
        return COMPV_ERROR_CODE_E_INVALID_PARAMETER;
    }
    if (!factory_->newObjDesc) {
        COMPV_DEBUG_ERROR("Factory with id = %d and name = '%s' doesn't have a constructor for descriptors", factory_->id, factory_->name);
        return COMPV_ERROR_CODE_E_INVALID_CALL;
    }
    return factory_->newObjDesc(desc);
}

COMPV_NAMESPACE_END()
