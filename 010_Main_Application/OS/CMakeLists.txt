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
idf_component_register(SRCS "Config/os_ota_cfg.c"
							"Config/os_tm_cfg.c"
							"Config/os_wifi_cfg.c"
							"Config/os_ws_cfg.c"
							"Core/os_ota.c"
							"Core/os_tm.c"
							"Core/os_wifi.c"
							"Core/os_ws.c"
                            "OS.c"
                       INCLUDE_DIRS "."
                       REQUIRES spi_flash )