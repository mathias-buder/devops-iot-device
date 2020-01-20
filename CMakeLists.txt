# *********************************************************************
#
#       SEWELA owns the copyright in this document and associated
#       documents and all rights are reserved. These documents must
#       not be used for any purpose other than that for which they
#       are supplied and must not be copied in whole or in part or
#       disclosed to others without prior written consent of SEWELA.
#       Any copy of this drawing or document made by any method
#       must also include a copy of this legend.
#
#       (c) SEWELA 2020
#
# *********************************************************************/

# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)


# Set custom component directories
set(EXTRA_COMPONENT_DIRS "TS"
                         "DD")

# Add IDF specific environment
include($ENV{IDF_PATH}/tools/cmake/project.cmake)

# Set projekt name
project(smart_vibrator)