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
#ifndef PROJECT_ENV_H
#define PROJECT_ENV_H

#include <string>

// Includes the CLOCKWERK_DIR and ROOT_DIR arguments.
#include "msenvironment.h"

// Define our modelspace directory from our environment variable
const std::string PROJECT_DIR = PROJECT_ROOT_DIR;

#endif