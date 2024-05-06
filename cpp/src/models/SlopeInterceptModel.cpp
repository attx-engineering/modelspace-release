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
#include "SlopeInterceptModel.h"
#include "architecture/EventLogger.h"
#include "architecture/Executive.h"

int SlopeInterceptModel::execute() {
    // Calculate y from x, m, and b
    _y_tmp = params.m()*inputs.x() + params.b();
    outputs.y(_y_tmp);

    return NO_ERROR;
}