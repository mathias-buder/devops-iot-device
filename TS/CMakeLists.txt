# *********************************************************************
#   MODULE SPECIFICATION:
#
#        File: CMakeList for module TS
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

idf_component_register(SRCS "Config/ts_fsr_preproc_Cfg.c"
                            "Core/ts_database.c"
                            "Core/ts_fsr_preproc.c"
                            "Interface/ts_input_interface.c"
                            "Interface/ts_output_interface.c"
                            "TS.c"
                       INCLUDE_DIRS ".")
