/******************************************************************************
* Copyright (c) ATTX LLC 2024. All Rights Reserved.
*
* This software and associated documentation (the "Software") are the 
* proprietary and confidential information of ATTX, LLC. The Software is 
* furnished under a license agreement between ATTX and the user organization 
* and may be used or copied only in accordance with the terms of the agreement.
* Refer to 'license/attx_license.adoc' for standard license terms.
*
* EXPORT CONTROL NOTICE: THIS SOFTWARE MAY INCLUDE CONTENT CONTROLLED UNDER THE
* INTERNATIONAL TRAFFIC IN ARMS REGULATIONS (ITAR) OR THE EXPORT ADMINISTRATION 
* REGULATIONS (EAR99). No part of the Software may be used, reproduced, or 
* transmitted in any form or by any means, for any purpose, without the express 
* written permission of ATTX, LLC.
******************************************************************************/
/*
Point mass gravity model header file

Author: Alex Reynolds
*/

#ifndef MODELS_SLOPE_INTERCEPT_MODEL_H
#define MODELS_SLOPE_INTERCEPT_MODEL_H

#include "ModelSpace.h"

/**
 * @brief   Slope-intercept "model"
 * 
 * This model demonstrates a simple "slope-intercept" in ModelSpace format
*/
class SlopeInterceptModel : public Model {
public:
    // TODO: Replace need for mass with direct acceleration calculation when 
    // body accel methods are implemented
    // Model params
    //         NAME                     TYPE                    DEFAULT VALUE
    START_PARAMS
        /** This is the "m" value in the calculation y=mx+b */
        SIGNAL(m,                       double,                 1.0)
        /** This is the "b" value in the calculation y=mx+b */
        SIGNAL(b,                       double,                 0.0)
    END_PARAMS

    // Model inputs
    //         NAME                     TYPE                    DEFAULT VALUE
    START_INPUTS
        /** This is the "x" value in the calculation y=mx+b */
        SIGNAL(x,                       double,                 0.0)
    END_INPUTS

    // Model outputs
    //         NAME                     TYPE                    DEFAULT VALUE
    START_OUTPUTS
        /** This is the "y" value in the calculation y=mx+b  */
        SIGNAL(y,                       double,                 0.0)
    END_OUTPUTS

    // Model dependencies
    struct Dependency { } dependency;

    // Model-specific implementations of startup and derivative
    SlopeInterceptModel() : Model() {}
    SlopeInterceptModel(Model &pnt, int schedule_slot=0, const std::string &m_name="slope_intercept") 
        : Model(pnt, schedule_slot, m_name) {}
    SlopeInterceptModel(SimulationExecutive &e, int schedule_slot=0, const std::string &m_name="slope_intercept") 
        : Model(e, schedule_slot, m_name) {}
    ~SlopeInterceptModel() {}
    int execute(); 
protected:
    double _y_tmp;
};

#endif