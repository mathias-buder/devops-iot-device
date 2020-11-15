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

idf_component_register(SRCS "Config/dd_adc_cfg.cpp"
                            "Config/dd_i2c_cfg.cpp"
                            "Config/dd_icm-20600_cfg.cpp"
                            "Config/dd_ina-219_cfg.cpp"
                            "Config/dd_max-30102_cfg.cpp"
                            "Config/dd_mcpwm_cfg.cpp"
                            "Config/dd_sd_cfg.cpp"
                            "Config/dd_tmp-102_cfg.cpp"
                            "Core/dd_adc.cpp"
                            "Core/dd_database.cpp"
                            "Core/dd_i2c.cpp"
                            "Core/dd_icm-20600.cpp"
                            "Core/dd_ina-219.cpp"
                            "Core/dd_max-30102.cpp"
                            "Core/dd_mcpwm.cpp"
                            "Core/dd_sd.cpp"
                            "Core/dd_tmp-102.cpp"
                            "DD.cpp"
                       INCLUDE_DIRS "."
                       REQUIRES fatfs)
