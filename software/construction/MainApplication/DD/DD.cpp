/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must--
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        @file DD.c
        @details Global device driver implementation

        (c) SEWELA 2020

*********************************************************************/

/*********************************************************************/
/*      INCLUDES                                                     */
/*********************************************************************/
#include <stdio.h>
#include <string.h>

#include "esp_log.h"

#include "DD.h"
#include "dd_sd.h"

/*************************************************************/
/*      GLOBAL DEFINES                                       */
/*************************************************************/
#define DD_LOG_MSG_TAG "DD"


/*********************************************************************/
/*      GLOBAL VARIABLES                                             */
/*********************************************************************/
DD_DATA_OUT_TYPE DD_C::data_out_s;                           /**< @details Global domain (DD) output structure */
DD_MCPWM_C       dd_mcpwm_channel_vo[DD_MCPWM_CHANNEL_SIZE]; /**< @details Array of MCPWM instances of size DD_MCPWM_CHANNEL_SIZE */
DD_TMP_102_C     dd_tmp_102_A( 0x49U );                      /**< @details TMP-102 instance A, @param[in] I2C address*/
DD_INA_219_C     dd_ina_219_A( 0x40U, 0.1F );                /**< @details INA-219 instance A, @param[in] I2C address, @param[in] Shunt resistor size (Ohm)*/
DD_MAX_30102_C   dd_max_30102( 0x57U );                      /**< @details MAX-30102 instance, @param[in] I2C address*/
DD_ICM_20600_C   dd_icm_20600( 0x68U );                      /**< @details ICM-20600 instance, @param[in] I2C address*/

/*********************************************************************/
/*   FUNCTION DEFINITIONS                                            */
/*********************************************************************/
void DD_C::init( void )
{
    U8 idx_u8;

    /**< @details MCPWM configuration ( board specific ) */
    const DD_MCPWM_CONFIG_TYPE dd_mcpwm_cfg_vs[DD_MCPWM_CHANNEL_SIZE] = {
                /* Channel Name         PWM Unit          UNIT I/O Signal    I/O Pin          Timer             PWM Operator    Operation Mode        Duty Cycle */
        #ifndef JTAG_DEBUG_SUPPORT_ENABLE
                {  DD_MCPWM_CHANNEL_1,  MCPWM_UNIT_0,     MCPWM0A,           GPIO_NUM_14,     MCPWM_TIMER_0,    MCPWM_OPR_A,    DD_MCPWM_MODE_OFF,    0.0F         },  /*!< Shared with JTAG pin: TMS (14) */
        #endif
                { DD_MCPWM_CHANNEL_2,   MCPWM_UNIT_0,     MCPWM0B,           GPIO_NUM_27,     MCPWM_TIMER_0,    MCPWM_OPR_B,    DD_MCPWM_MODE_OFF,    0.0F         },
                { DD_MCPWM_CHANNEL_3,   MCPWM_UNIT_0,     MCPWM1A,           GPIO_NUM_32,     MCPWM_TIMER_1,    MCPWM_OPR_A,    DD_MCPWM_MODE_OFF,    0.0F         },
                { DD_MCPWM_CHANNEL_4,   MCPWM_UNIT_0,     MCPWM1B,           GPIO_NUM_33,     MCPWM_TIMER_1,    MCPWM_OPR_B,    DD_MCPWM_MODE_OFF,    0.0F         },
                { DD_MCPWM_CHANNEL_5,   MCPWM_UNIT_0,     MCPWM2A,           GPIO_NUM_26,     MCPWM_TIMER_2,    MCPWM_OPR_A,    DD_MCPWM_MODE_OFF,    0.0F         },
                { DD_MCPWM_CHANNEL_6,   MCPWM_UNIT_0,     MCPWM2B,           GPIO_NUM_25,     MCPWM_TIMER_2,    MCPWM_OPR_B,    DD_MCPWM_MODE_OFF,    0.0F         },
        #ifndef JTAG_DEBUG_SUPPORT_ENABLE
                { DD_MCPWM_CHANNEL_7,   MCPWM_UNIT_1,     MCPWM0A,           GPIO_NUM_13,     MCPWM_TIMER_0,    MCPWM_OPR_A,    DD_MCPWM_MODE_OFF,    0.0F         },  /*!< Shared with JTAG pin: TCK (13) */
                { DD_MCPWM_CHANNEL_8,   MCPWM_UNIT_1,     MCPWM0B,           GPIO_NUM_12,     MCPWM_TIMER_0,    MCPWM_OPR_B,    DD_MCPWM_MODE_OFF,    0.0F         },  /*!< Shared with JTAG pin: TDI (12) */
                { DD_MCPWM_CHANNEL_9,   MCPWM_UNIT_1,     MCPWM1A,           GPIO_NUM_15,     MCPWM_TIMER_1,    MCPWM_OPR_A,    DD_MCPWM_MODE_OFF,    0.0F         },  /*!< Shared with JTAG pin: TDO (15) */
        #endif
                { DD_MCPWM_CHANNEL_10,  MCPWM_UNIT_1,     MCPWM1B,           GPIO_NUM_2,      MCPWM_TIMER_1,    MCPWM_OPR_B,    DD_MCPWM_MODE_OFF,    0.0F         },
                { DD_MCPWM_CHANNEL_11,  MCPWM_UNIT_1,     MCPWM2A,           GPIO_NUM_0,      MCPWM_TIMER_2,    MCPWM_OPR_A,    DD_MCPWM_MODE_OFF,    0.0F         },
                { DD_MCPWM_CHANNEL_12,  MCPWM_UNIT_1,     MCPWM2B,           GPIO_NUM_4,      MCPWM_TIMER_2,    MCPWM_OPR_B,    DD_MCPWM_MODE_OFF,    0.0F         }
    };

    /*!< Initialize MCPWM device driver with default channel configuration */
    for ( idx_u8 = 0U; idx_u8 < DD_MCPWM_CHANNEL_SIZE; ++idx_u8 )
    {
        dd_mcpwm_channel_vo[idx_u8].init( dd_mcpwm_cfg_vs[idx_u8] );
    }

    DD_SD_C::init();                                               /*!< Initialize SD card driver */
    DD_C::data_out_s.p_i2c_error_out_s      = DD_I2C_C::init();    /*!< Initialize I2C device driver */
    DD_C::data_out_s.p_adc_data_out_s       = DD_ADC_C::init();    /*!< Initialize ADC device driver */
    DD_C::data_out_s.p_icm_20600_data_out_s = dd_icm_20600.init(); /*!< Initialize ICM-2600 motion subsystem */
    DD_C::data_out_s.p_max_30102_data_out_s = dd_max_30102.init(); /*!< Initialize MAX-30102 HR+SpO2 subsystem with default configuration */
    DD_C::data_out_s.p_ina_219_data_out_s   = dd_ina_219_A.init(); /*!< Initialize INA-219 Current/Voltage/Power measuring subsystem */
    DD_C::data_out_s.p_tmp_102_data_out_s   = dd_tmp_102_A.init(); /*!< Initialize TMP-102 temperature sensor */
}


DD_DATA_OUT_TYPE DD_C::process_inputs( void )
{
    DD_ADC_C::main();    /*!< Schedule ADC basic device driver */
    dd_icm_20600.main(); /*!< Schedule ICM-2600 motion subsystem */
    dd_max_30102.main(); /*!< Schedule MAX-30102 HR+SpO2 subsystem */
    dd_ina_219_A.main(); /*!< Schedule INA-219 (A) Current/Voltage/Power measuring subsystem */
    dd_tmp_102_A.main(); /*!< Schedule TMP-102 (A) temperature sensor */

    /* Return copy of global domain data structure */
    return DD_C::data_out_s;
}


void DD_C::process_outputs( DD_DATA_IN_TYPE &r_data_in_s )
{
    U8 idx_u8;

    /* Update all DD_MCPWM_CHANNEL_SIZE PWM channels */
    for ( idx_u8 = 0U; idx_u8 < DD_MCPWM_CHANNEL_SIZE; ++idx_u8 )
    {
        dd_mcpwm_channel_vo[idx_u8].update( r_data_in_s.mcpwm_data_in_vs[idx_u8] );
    }
}



