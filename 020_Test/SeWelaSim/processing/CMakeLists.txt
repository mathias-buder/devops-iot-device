
#/*********************************************************************
#    MODULE SPECIFICATION:
#
#        $ProjectName: /DAS/040_Test_Projects/<CoreFus> DAS Core Fusion/060_Test_Projects/Capra_OF_DEV/060_Software_Construction/WeGetASim/CommsProcessing/project.pj $
#        $RCSfile: CMakeLists.txt $
#        $Revision: 1.1 $
#        $Date: 2018/05/17 12:13:37CEST $
#
#        TRW Ltd owns the copyright in this document and associated
#        documents and all rights are reserved.  These documents must
#        not be used for any purpose other than that for which they
#        are supplied and must not be copied in whole or in part or
#        disclosed to others without prior written consent of TRW
#        Ltd. Any copy of this drawing or document made by any method
#        must also include a copy of this legend.
#
#    $CopyrightDate: (C) ZF TRW LTD 2018 $
#
#*********************************************************************/

SET (LOCAL_SIM_SRCS
${CMAKE_CURRENT_SOURCE_DIR}/dlg_processing.cpp
)

# propergate INCLUDE_DIRS, VLE_SRCS, and SIM_SRCS up to calling Makefile
get_property(dirs DIRECTORY . PROPERTY INCLUDE_DIRECTORIES)

SET(INCLUDE_DIRS ${INCLUDE_DIRS} ${dirs} PARENT_SCOPE)
SET(SIM_SRCS ${SIM_SRCS} ${LOCAL_SIM_SRCS} PARENT_SCOPE)