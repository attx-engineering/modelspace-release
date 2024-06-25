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
#include "locations.h"

std::string clockwerkDir() {
    return CLOCKWERK_DIR + slash();
}

std::string modelspaceDir() {
    return MODELSPACE_DIR + slash();
}

std::string rootDir() {
    return ROOT_DIR + slash();
}

std::string slash() {
    #ifdef _WIN32
        return "\\";
    #else
        return "/";
    #endif
}

std::vector<std::string> defaultSpiceKernels() {
    /// @brief This is a list of default kernels to be loaded in ModelSpace
    ///
    /// The default kernels are intended to provide a basic set of SPICE kernels
    /// that a standard user might need. They are by no means comprehensive.
    /// An attempt has been made to use a minimal set of SPICE kernels 
    /// which are tied to the latest data. 
    return std::vector<std::string>  {MODELSPACE_DIR + slash() + "cspice" + slash() + "data" + slash() + "naif0012.tls",
                                      MODELSPACE_DIR + slash() + "cspice" + slash() + "data" + slash() + "de440.bsp",
                                      MODELSPACE_DIR + slash() + "cspice" + slash() + "data" + slash() + "pck00011.tpc",
                                      MODELSPACE_DIR + slash() + "cspice" + slash() + "data" + slash() + "gm_de440.tpc"};
}

std::string licenseLocation() {
    return ROOT_DIR + slash() + "license" + slash() + "licensefile.enc";
}