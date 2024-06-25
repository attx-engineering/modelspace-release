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
/**
 * File: locations.h
 * 
 * In this file, locations for items like root directories, separators, etc.
 * are defined in a flexible and portable manner that will work effectively
 * on whatever system they are distributed to.
 */

#include <string>
#include <vector>

/// @brief This is the location of the "clockwerk" directory on the target system
const std::string CLOCKWERK_DIR = CLOCKWERK_ROOT_DIR;

/// @brief This is the location of the "modelspace" directory on the target system
const std::string MODELSPACE_DIR = MODELSPACE_ROOT_DIR;

/// @brief This is the location of the project's root directory. Will be the same as MODELSPACE_DIR within modelspace
const std::string ROOT_DIR = PROJECT_ROOT_DIR;

/// @brief Function to get the location of the clockwerk directory on the target system
/// @return The location of the directory as a std::string
std::string clockwerkDir();

/// @brief Function to get the location of the modelspace directory on the target system
/// @return The location of the directory as a std::string
std::string modelspaceDir();

/// @brief Function to get the location of the project directory on the target system
/// @return The location of the directory as a std::string
std::string rootDir();

/// @brief Function to return a slash (either / or \) that is system-flexible
/// @return The slash character for the target system as a / (linux, mac) or \ (windows)
std::string slash();

/// @brief Function to return a list of default spice kernels for the simulation
/// @return Vectorized list of paths to default kernels
std::vector<std::string> defaultSpiceKernels();

/// @brief Function to return the location and name of the MS license file
/// @return The license file location as a std::string
std::string licenseLocation();