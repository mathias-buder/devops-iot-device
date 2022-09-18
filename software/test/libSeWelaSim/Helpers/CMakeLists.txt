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

SET (LOCAL_SIM_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/VE/hlp_ve_grid.cpp )

# propergate INCLUDE_DIRS, VLE_SRCS, and SIM_SRCS up to calling Makefile
get_property(dirs DIRECTORY . PROPERTY INCLUDE_DIRECTORIES)

SET(INCLUDE_DIRS ${INCLUDE_DIRS} ${dirs} PARENT_SCOPE)
SET(SIM_SRCS ${SIM_SRCS} ${LOCAL_SIM_SRCS} PARENT_SCOPE)