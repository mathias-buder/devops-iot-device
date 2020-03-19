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


idf_component_register(SRCS "Config/sense_ts_cfg.c"
                            "Config/sense_hr_cfg.c"
                            "Config/sense_spo2_cfg.c"
                            "Core/sense_database.c"
                            "Core/sense_ts.c"
                            "Core/sense_hr.c"
                            "Core/sense_spo2.c"
                            "Interface/sense_ts_interface.c"
                            "Interface/sense_hr_interface.c"
                            "Interface/sense_spo2_interface.c"
                            "SENSE.c"
                       INCLUDE_DIRS ".")
