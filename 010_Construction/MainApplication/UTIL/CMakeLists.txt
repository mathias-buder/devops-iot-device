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

#idf_component_register(SRCS "util_conf.cpp"
#                             "util_gen.cpp"
#                       INCLUDE_DIRS ".")
                       
                       
SET(UTIL_SRCS_VLE ${CMAKE_CURRENT_SOURCE_DIR}/util_conf.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/util_gen.cpp )
                   
SET(VLE_SRCS       ${VLE_SRCS}
                   ${UTIL_SRCS_VLE}
                   PARENT_SCOPE )

