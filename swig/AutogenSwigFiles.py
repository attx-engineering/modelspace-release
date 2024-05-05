###############################################################################
# Copyright (c) ATTX LLC 2024. All Rights Reserved.
#
# This software and associated documentation (the "Software") are the 
# proprietary and confidential information of ATTX, LLC. The Software is 
# furnished under a license agreement between ATTX and the user organization 
# and may be used or copied only in accordance with the terms of the agreement.
# Refer to 'license/attx_license.adoc' for standard license terms.
#
# EXPORT CONTROL NOTICE: THIS SOFTWARE MAY INCLUDE CONTENT CONTROLLED UNDER THE
# INTERNATIONAL TRAFFIC IN ARMS REGULATIONS (ITAR) OR THE EXPORT ADMINISTRATION 
# REGULATIONS (EAR99). No part of the Software may be used, reproduced, or 
# transmitted in any form or by any means, for any purpose, without the express 
# written permission of ATTX, LLC.
###############################################################################
import os, re, argparse

ITEM_NAME = "<<ItemName>>"
ITEM_PARAMS_CONTENT = "<<ItemParamsContent>>"
ITEM_INPUTS_CONTENT = "<<ItemInputsContent>>"
ITEM_OUTPUTS_CONTENT = "<<ItemOutputsContent>>"
INCLUDE_DIR = "<<IncludeDir>>"
CLOCKWERK_SWIG_DIR = "<<ClockwerkSwigPath>>"

TEMPLATE_FILE = "swigtemplate.txt"

class AutogenSwigFiles:
    """
    This python class is used to auto-generate swig .i files for
    wrapping .cpp model, task, monitor, and event files into other
    high-level languages.
    """
    # Define our internal variables for auto-generating swig files
    # Each of these must be set for our swig auto-generator to work

    def __init__(self, include_path, cwk_swig, output_file=None, template_file=TEMPLATE_FILE):
        """
        This function initializes the auto generator with appropriate information
        """
        self._item_params_content = None
        self._item_inputs_content = None
        self._item_outputs_content = None
        self._file_string = None
        self._input_string = None
        self._item_name = None
        self._include_path = include_path
        self._template_file = template_file
        self._output_file = output_file
        self._cwk_swig = cwk_swig
        self._is_item = True

    def parseHeaderFile(self):
        """
        This function loads in the header file at include path as a string for
        parsing
        """
        with open(self._include_path, 'r') as file:
            self._input_string = file.read()
            file.close()

        if "Model" not in self._input_string and "Task" not in self._input_string and "Monitor" not in self._input_string and "Event" not in self._input_string:
            self._is_item = False

    def parseInputFile(self):
        """
        This function parses the input file from our text template and saves
        it to our string variable
        """
        with open(self._template_file, 'r') as file:
            self._file_string = file.read()
            file.close()

        # Get our object name via regular expression
        try:
            header = self._include_path[max(loc for loc, val in enumerate(self._include_path) if val == '/'):]
        except:
            header = self._include_path
        self._item_name = self._getStringBetweenMarkers("/", ".h", header)

        if self._item_name is None:
            raise ValueError("Unable to parse item name from file input. Is it a .h?")
        
    def _getStringBetweenMarkers(self, start, end, parse):
        """
        A function to get the string between two text markers
        """
        # Split our string at our start delimeter
        start_split = parse.split(start)
        if len(start_split) != 2:
            return ""
        
        internal_split = start_split[-1].split(end)
        if len(internal_split) != 2:
            return ""

        return internal_split[0]

    def setValuesFromHeader(self):
        """
        This function parses the input header file for string information
        """
        # Use regular expressions to parse key markers from our file
        # First, find our params

        self._item_params_content = self._getStringBetweenMarkers("START_PARAMS", "END_PARAMS", self._input_string)

        self._item_inputs_content = self._getStringBetweenMarkers("START_INPUTS", "END_INPUTS", self._input_string)

        self._item_outputs_content = self._getStringBetweenMarkers("START_OUTPUTS", "END_OUTPUTS", self._input_string)

    def customizeOutputFile(self):
        """
        This function customizes the file output by replacing template markers
        in our file string with actual desired values
        """
        self._file_string = self._file_string.replace(ITEM_NAME, self._item_name)
        self._file_string = self._file_string.replace(ITEM_PARAMS_CONTENT, self._item_params_content)
        self._file_string = self._file_string.replace(ITEM_INPUTS_CONTENT, self._item_inputs_content)
        self._file_string = self._file_string.replace(ITEM_OUTPUTS_CONTENT, self._item_outputs_content)
        self._file_string = self._file_string.replace(INCLUDE_DIR, self._include_path)
        self._file_string = self._file_string.replace(CLOCKWERK_SWIG_DIR, self._cwk_swig)

    def writeOutputFile(self):
        """
        This function writes out the string _file_string to our file address
        """
        # Now determine where we're writing to
        if self._output_file is None:
            if not os.path.exists('swig_auto'):
                os.mkdir('swig_auto')
            self._output_file = os.path.join('swig_auto', self._item_name + '.i')
        else:
            if not os.path.exists(self._output_file):
                os.mkdir(self._output_file)
            self._output_file = os.path.join(self._output_file, self._item_name + '.i')

        if self._is_item:
            with open(self._output_file, 'w') as file:
                file.write(self._file_string)
                file.close()

if __name__ == "__main__":
    # Create our argument parser
    parser = argparse.ArgumentParser(prog='SWIG File Auto-Generator',
                                     description='Auto-generates SWIG .i files for modelspace')
    
    # Add our arguments for input
    parser.add_argument('--include_path')
    parser.add_argument('--incl_swig_dir')
    parser.add_argument('--out_dir')

    # Parse our args
    args = parser.parse_args()
    
    # Run each function in our class
    aswig = AutogenSwigFiles(args.include_path, args.incl_swig_dir, args.out_dir)
    aswig.parseHeaderFile()
    aswig.parseInputFile()
    aswig.setValuesFromHeader()
    aswig.customizeOutputFile()
    aswig.writeOutputFile()