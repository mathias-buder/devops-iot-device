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

idf_component_register(SRCS "dd_adc.cpp"
                            "dd_i2c.cpp"
                            #"dd_icm-20600.cpp"
                            "dd_ina-219.cpp"
                            #"dd_max-30102.cpp"
                            #"dd_mcpwm.cpp"
                            "dd_sd.cpp"
                            #"dd_tmp-102.cpp"
                            "DD.cpp"
                       INCLUDE_DIRS "."
                       REQUIRES fatfs esp_adc_cal )
