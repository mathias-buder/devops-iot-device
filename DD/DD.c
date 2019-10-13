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
#include "DD.h"

#include "Core/dd_i2c.h"
#include "Core/dd_icm-20600.h"
#include "Core/dd_database.h"

/* Need to be removed later, only for testing purpose */
#include "Config/dd_icm-20600_Cfg.h"

/*********************************************************************/
/*      GLOBAL VARIABLES                                             */
/*********************************************************************/
#define REGISTER_ADDR   0x1A
#define REGISTER_VAL    0xAA

#define BIT_NO          2
#define LENGTH          3

U8 reg_val = 0U;


/*********************************************************************/
/*      PRIVATE FUNCTION DECLARATIONS                                */
/*********************************************************************/

/*********************************************************************/
/*   FUNCTION DEFINITIONS                                            */
/*********************************************************************/

void dd_init(void)
{
    /* Initialize I2C basic device driver */
    dd_i2c_init();
}


void dd_main(void)
{
//    BOOLEAN bit_b = FALSE;
//    U8 register_value_u8 = 0U;
//    U8 bit_data_u8 = 0U;
    DD_I2C_ERROR_TYPE* i2c_error_s;


//    dd_i2c_read_single( DD_ICM_20600_I2C_ADDR, REGISTER_ADDR, &register_value_u8 );
//
//    printf( "Register 0x%x: 0x%x\n", REGISTER_ADDR, register_value_u8 );
//
//
//    dd_i2c_write_single(DD_ICM_20600_I2C_ADDR, REGISTER_ADDR, reg_val);
//
//
//
//    dd_i2c_read_single( DD_ICM_20600_I2C_ADDR, REGISTER_ADDR, &register_value_u8 );
//
//    printf( "Register 0x%x: 0x%x (0x%x)\n", REGISTER_ADDR, register_value_u8, reg_val);
//
//
//    bit_b = dd_i2c_read_bit(DD_ICM_20600_I2C_ADDR, REGISTER_ADDR, BIT_NO);
//
//
//    printf( "Register 0x%x, BIT%i=%i\n", REGISTER_ADDR, BIT_NO, bit_b);
//
//
//    dd_i2c_read_modify_write(DD_ICM_20600_I2C_ADDR, REGISTER_ADDR, BIT_NO, TRUE);
//
//
//    dd_i2c_read_single( DD_ICM_20600_I2C_ADDR, REGISTER_ADDR, &register_value_u8 );
//
//    printf( "Register 0x%x: 0x%x\n", REGISTER_ADDR, register_value_u8 );


//    /* Bit position seems to be wrong, BIT0 is currently at position of BIT1 */
//    dd_i2c_read_bits(DD_ICM_20600_I2C_ADDR, REGISTER_ADDR, BIT_NO, LENGTH, &bit_data_u8);
//
//    printf( "Register 0x%x, BIT%i-%i=0x%x\n", REGISTER_ADDR, BIT_NO, BIT_NO + LENGTH - 1, bit_data_u8);

   // reg_val++;


    // printf( "ICM-20600 identity valid: 0x%x\n", dd_icm_20600_who_am_i_read() );
//

    i2c_error_s = dd_i2c_get_error();

    /* Read all required sensor data form ICM-20600 */
    dd_icm_20600_acquire_sensor_data();

    printf( "Temperature: %0.2f °C, i2c-state: %s \n", dd_icm_20600_output_s.temperature_deg_f32, esp_err_to_name( i2c_error_s->current_t ));


    printf("X: %i, Y: %i, Z: %i\n", dd_icm_20600_output_s.accel_data_raw_u16[DD_ICM_20600_ACCEL_X], dd_icm_20600_output_s.accel_data_raw_u16[DD_ICM_20600_ACCEL_Y], dd_icm_20600_output_s.accel_data_raw_u16[DD_ICM_20600_ACCEL_Z]);


}
