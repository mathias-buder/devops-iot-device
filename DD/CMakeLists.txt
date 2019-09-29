# *********************************************************************
#   MODULE SPECIFICATION:
#
#       $URL$
#       $Revision$
#       $Author$
#       $Date$
#
#       SEWELA owns the copyright in this document and associated
#       documents and all rights are reserved. These documents must
#       not be used for any purpose other than that for which they
#       are supplied and must not be copied in whole or in part or
#       disclosed to others without prior written consent of SEWELA.
#       Any copy of this drawing or document made by any method
#       must also include a copy of this legend.
#
#       (c) SEWELA 2019
#
# *********************************************************************/

idf_component_register(SRCS "Config/dd_icm-20600_Cfg.c"
                            "Config/dd_max-30102_Cfg.c"
                            "Config/dd_xr-18910_Cfg.c"
                            "Core/dd_database.c"
                            "Core/dd_icm-20600.c"
                            "Core/dd_max-30102.c"
                            "Core/dd_xr-18910.c"
                            "Interface/dd_icm-20600_interface.c"
                            "Interface/dd_max-30102_interface.c"
                            "Interface/dd_xr-18910_interface.c"
                            "DD.c"
                       INCLUDE_DIRS ".")
