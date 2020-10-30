/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must--
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        (c) SEWELA 2020

        @file DD.c
        @details Some detailed description

*********************************************************************/

/*********************************************************************/
/*      INCLUDES                                                     */
/*********************************************************************/
#include "DD.h"

#include "Core/dd_database.h"
#include "Core/dd_i2c.h"
#include "Core/dd_adc.h"
#include "Core/dd_mcpwm.h"
#include "Interface/dd_mcpwm_if.h"
#include "Core/dd_icm-20600.h"
#include "Core/dd_max-30102.h"
#include "Core/dd_ina-219.h"
#include "Core/dd_sd.h"
#include "esp_log.h"

#include <stdio.h>
#include <string.h>

/*************************************************************/
/*      GLOBAL DEFINES                                       */
/*************************************************************/
#define DD_LOG_MSG_TAG "DD"

/*********************************************************************/
/*      GLOBAL VARIABLES                                             */
/*********************************************************************/

/*********************************************************************/
/*   FUNCTION DEFINITIONS                                            */
/*********************************************************************/
void dd_init( void )
{
    dd_sd_init();        /* Initialize SD card driver */
    dd_i2c_init();       /* Initialize I2C basic device driver */
    dd_adc_init();       /* Initialize ADC basic device driver */
    dd_mcpwm_init();     /* Initialize MCPWM basic device driver */
    dd_mcpwm_if_init();  /* Initialize MCPWM input interface module */
    dd_icm_20600_init(); /* Initialize ICM-2600 motion subsystem */
    dd_max_30102_init(); /* Initialize MAX-30102 HR+SpO2 subsystem */
    dd_ina_219_init();   /* Initialize INA-219 Current/Voltage/Power measuring subsystem */
}

void dd_main( void )
{
    dd_adc_main();       /* Schedule ADC basic device driver */
    dd_mcpwm_if_main();  /* Schedule MCPWM input interface module ( must be called before dd_mcpwm_main() ) */
    dd_mcpwm_main();     /* Schedule MCPWM basic device driver */
    dd_icm_20600_main(); /* Schedule ICM-2600 motion subsystem */
    dd_max_30102_main(); /* Schedule MAX-30102 HR+SpO2 subsystem */
    dd_ina_219_main();   /* Schedule INA-219 Current/Voltage/Power measuring subsystem */
}
