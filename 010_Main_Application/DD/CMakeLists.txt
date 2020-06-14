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

idf_component_register(SRCS "Config/dd_icm-20600_cfg.c"
                            "Config/dd_max-30102_cfg.c"
                            "Config/dd_pga-302_cfg.c"
                            "Config/dd_i2c_cfg.c"
                            "Config/dd_sd_cfg.c"
                            "Core/dd_database.c"
                            "Core/dd_icm-20600.c"
                            "Core/dd_max-30102.c"
                            "Core/dd_pga-302.c"
                            "Core/dd_i2c.c"
                            "Core/dd_sd.c"
                            "Interface/dd_i2c_if.c"
                            "Interface/dd_icm-20600_if.c"
                            "Interface/dd_max-30102_if.c"
                            "Interface/dd_pga-302_if.c"
                            "Interface/dd_sd_if.c"
                            "DD.c"
                       INCLUDE_DIRS "."
                       REQUIRES fatfs)
