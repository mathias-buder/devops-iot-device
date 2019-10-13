/*********************************************************************
    MODULE SPECIFICATION:

        $URL$
        $Revision$
        $Author$
        $Date$

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        (c) SEWELA 2019

*********************************************************************/

/*********************************************************************/
/*      INCLUDES                                                     */
/*********************************************************************/
#include <stdio.h>
#include <math.h>

#include "dd_icm-20600.h"
#include "dd_i2c.h"
#include "dd_database.h"

#include "../Config/dd_icm-20600_Cfg.h"


/*********************************************************************/
/*      GLOBAL VARIABLES                                             */
/*********************************************************************/


/*********************************************************************/
/*      PRIVATE FUNCTION DECLARATIONS                                */
/*********************************************************************/



/*********************************************************************/
/*   FUNCTION DEFINITIONS                                            */
/*********************************************************************/

void dd_icm_20600_reset_soft(void)
{

    dd_i2c_read_modify_write( DD_ICM_20600_I2C_ADDR,
                              DD_ICM_20600_PWR_MGMT_1,
                              DD_ICM_20600_PWR_MGMT_1_DEVICE_RESET_B,
                              TRUE );
}


void dd_icm_20600_who_am_i_read(void)
{
    BOOLEAN state_b = FALSE;
    U8      register_value_u8;

    state_b = dd_i2c_read_single( DD_ICM_20600_I2C_ADDR,
                                  DD_ICM_20600_WHO_AM_I,
                                 &register_value_u8 );
    /* Check for I2C error */
    if( FALSE != state_b )
    {
        dd_icm_20600_output_s.chip_id_u8 = register_value_u8;
    }
    else
    {
        /* Reset chip id */
        dd_icm_20600_output_s.chip_id_u8 = 0xFF;
    }
}


void dd_icm_20600_temperature_read(void)
{
    BOOLEAN state_b             = FALSE;
    U16     temperature_raw_u16 = 0xFFFF;
    F32     temperature_deg_f32 = -1000.0F;
    U8      register_data_v8[2];


    state_b = dd_i2c_read_burst( DD_ICM_20600_I2C_ADDR,
                                 DD_ICM_20600_TEMP_OUT_H,
                                 register_data_v8,
                                 sizeof(register_data_v8) );
    /* Check for I2C error */
    if( FALSE != state_b )
    {
        temperature_raw_u16 = ( register_data_v8[0] << 8U ) | register_data_v8[1];
        temperature_deg_f32 = ( temperature_raw_u16 * DD_ICM_20600_TEMP_SENSITIVITY_INV ) + DD_ICM_20600_TEMP_ROOM_OFFSET;
    }

    dd_icm_20600_output_s.temperature_raw_u16 = temperature_raw_u16;
    dd_icm_20600_output_s.temperature_deg_f32 = temperature_deg_f32;
}


void dd_icm_20600_accel_data_read_raw(void)
{
    BOOLEAN state_b = FALSE;
    U8      register_data_v8[2 * DD_ICM_20600_ACCEL_SIZE];
    U16     accel_data_u16[DD_ICM_20600_ACCEL_SIZE] = {0xFFFF, 0xFFFF, 0xFFFF};

    state_b = dd_i2c_read_burst( DD_ICM_20600_I2C_ADDR,
                                 DD_ICM_20600_ACCEL_XOUT_H,
                                 register_data_v8,
                                 sizeof(register_data_v8) );
    /* Check for I2C error */
    if( FALSE != state_b )
    {
        accel_data_u16[DD_ICM_20600_ACCEL_X] = ( register_data_v8[0] << 8U ) | register_data_v8[1];
        accel_data_u16[DD_ICM_20600_ACCEL_Y] = ( register_data_v8[2] << 8U ) | register_data_v8[3];
        accel_data_u16[DD_ICM_20600_ACCEL_Z] = ( register_data_v8[4] << 8U ) | register_data_v8[5];
    }

    dd_icm_20600_output_s.accel_data_raw_u16[DD_ICM_20600_ACCEL_X] = accel_data_u16[DD_ICM_20600_ACCEL_X];
    dd_icm_20600_output_s.accel_data_raw_u16[DD_ICM_20600_ACCEL_Y] = accel_data_u16[DD_ICM_20600_ACCEL_Y];
    dd_icm_20600_output_s.accel_data_raw_u16[DD_ICM_20600_ACCEL_Z] = accel_data_u16[DD_ICM_20600_ACCEL_Z];
}

