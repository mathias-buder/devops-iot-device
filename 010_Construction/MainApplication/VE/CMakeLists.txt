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
if(SEWELASIM)

   # SeWelaSim specific variables

   SET(VE_SRCS_VLE ${CMAKE_CURRENT_SOURCE_DIR}/ve_grid.cpp
                    ${CMAKE_CURRENT_SOURCE_DIR}/VE.cpp )

   SET(VLE_SRCS     ${VLE_SRCS}
                    ${VE_SRCS_VLE}
                    PARENT_SCOPE )

else()

    idf_component_register(SRCS "ve_grid.cpp"
                                "VE.cpp"
                           INCLUDE_DIRS ".")

endif()