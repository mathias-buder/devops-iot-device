/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        (c) SEWELA 2020

*********************************************************************/

/*********************************************************************/
/*      INCLUDES                                                     */
/*********************************************************************/
#include <stdio.h>
#include <math.h>

#include "esp_log.h"

#include "../Config/dd_icm-20600_cfg.h"
#include "dd_database.h"
#include "dd_icm-20600.h"
#include "dd_i2c.h"


/*********************************************************************/
/*      GLOBAL VARIABLES                                             */
/*********************************************************************/


/*********************************************************************/
/*      PRIVATE FUNCTION DECLARATIONS                                */
/*********************************************************************/
BOOLEAN         dd_icm_20600_reset_soft( void );
BOOLEAN         dd_icm_20600_who_am_i_read( DD_ICM_20600_DATA* p_input_data_s );
PRIVATE BOOLEAN dd_icm_20600_set_accel_full_scale( const DD_ICM_20600_AFS scale_e );
PRIVATE BOOLEAN dd_icm_20600_set_gyro_full_scale( const DD_ICM_20600_GFS scale_e );
BOOLEAN         dd_icm_20600_temperature_read( DD_ICM_20600_DATA* p_input_data_s );
BOOLEAN         dd_icm_20600_accel_data_read_raw( DD_ICM_20600_DATA* p_input_data_s );
BOOLEAN         dd_icm_20600_gyro_data_read_raw( DD_ICM_20600_DATA* p_input_data_s );
PRIVATE BOOLEAN dd_icm_20600_self_test( DD_ICM_20600_DATA* p_data_s );
BOOLEAN         dd_icm_20600_calibrate( F32* p_gyro_bias_f32, F32* p_accel_bias_f32 );

/*********************************************************************/
/*   FUNCTION DEFINITIONS                                            */
/*********************************************************************/

BOOLEAN dd_icm_20600_init( void )
{
    ESP_LOGI(DD_ICM_20600_LOG_MSG_TAG, "Initializing ..." );

    /* Initialize global driver data structure. Default value for global
     * variables is "0" according to ANSI-C standard. Only variables with values
     * other then "0" need to be initialized here. */
    dd_icm_20600_data_s.dev_state_s.state_e    = DD_STATE_TEST;

    /* Check for matching device id */
    if (    ( TRUE != dd_icm_20600_who_am_i_read( &dd_icm_20600_data_s ) )
         || ( DD_ICM_20600_DEVICE_ID != dd_icm_20600_data_s.chip_id_u8 ) )
    {
        return FALSE;
    }

    /* Clear sleep mode bit (6), enable all sensors */
    if ( FALSE == dd_i2c_write_single( DD_ICM_20600_I2C_ADDR, DD_ICM_20600_PWR_MGMT_1, 0x00 ) )
    {
        return FALSE;
    }

    /* Delay 100 ms for PLL to get established on x-axis gyro;
     * should check for PLL ready interrupt */
    vTaskDelay( portTICK_PERIOD_MS );

    /* Get stable time source and set clock source to be PLL with x-axis
    * gyroscope reference, bits 2:0 = 001 */
    if ( FALSE == dd_i2c_write_single( DD_ICM_20600_I2C_ADDR, DD_ICM_20600_PWR_MGMT_1, 0x01 ) )
    {
        return FALSE;
    }

    /* Configure gyroscope and accelerometer
    * Disable FSYNC and set accelerometer and gyro bandwidth to 44 and 42 Hz, respectively.
    * DLPF_CFG = bits 2:0 = 010; this sets the sample rate at 1 kHz for both
    * Maximum delay is 4.9 ms which is just over a 200 Hz maximum rate */
    if ( FALSE == dd_i2c_write_single( DD_ICM_20600_I2C_ADDR, DD_ICM_20600_CONFIG, 0x03 ) )
    {
        return FALSE;
    }

    /* Set sample rate = gyroscope output rate / ( 1 + SMPLRT_DIV ).
     * Use a 200 Hz rate as it was set in CONFIG above */
    if ( FALSE == dd_i2c_write_single( DD_ICM_20600_I2C_ADDR, DD_ICM_20600_SMPLRT_DIV, 0x04 ) )
    {
        return FALSE;
    }

    /* Set gyroscope full scale range. */
    if ( FALSE == dd_icm_20600_set_gyro_full_scale( DD_ICM_20600_G_SCALSE ) )
    {
        return FALSE;
    }

    /* Set gyroscope full scale range. */
    if ( FALSE == dd_icm_20600_set_accel_full_scale( DD_ICM_20600_A_SCALSE ) )
    {
        return FALSE;
    }

    /* Configure Interrupts and Bypass Enable. Set interrupt pin active high,
     * push-pull, and clear on read of INT_STATUS, enable I2C_BYPASS_EN so
     * additional chips can join the I2C bus */
    if ( FALSE == dd_i2c_write_single( DD_ICM_20600_I2C_ADDR, DD_ICM_20600_INT_PIN_CFG, 0x22 ) )
    {
        return FALSE;
    }

    /* Enable data ready (bit 0) interrupt */
    if ( FALSE == dd_i2c_write_single( DD_ICM_20600_I2C_ADDR, DD_ICM_20600_INT_ENABLE, 0x01 ) )
    {
        return FALSE;
    }

    ESP_LOGI(DD_ICM_20600_LOG_MSG_TAG, "Done" );

    return TRUE;
}

void dd_icm_20600_main( void )
{

    /* icm-20600 state-maschine */

    switch ( dd_icm_20600_data_s.dev_state_s.state_e )
    {
    case DD_STATE_TEST:

        /* Execute self test and store result */
        dd_icm_20600_data_s.self_test_passed_b = dd_icm_20600_self_test( &dd_icm_20600_data_s );

        /* Go to running state */
        dd_icm_20600_data_s.dev_state_s.state_e = DD_STATE_RUN;

        break;

    case DD_STATE_CALIB:
        break;

    case DD_STATE_RUN:

        /* Read all raw sensor data form ICM-20600 */
        dd_icm_20600_temperature_read( &dd_icm_20600_data_s );
        dd_icm_20600_accel_data_read_raw( &dd_icm_20600_data_s );
        dd_icm_20600_gyro_data_read_raw( &dd_icm_20600_data_s );

        break;

    default:

        /* Should never happen */
        assert( DD_STATE_INIT  == dd_icm_20600_data_s.dev_state_s.state_e );
        assert( DD_STATE_CALIB == dd_icm_20600_data_s.dev_state_s.state_e );
        assert( DD_STATE_RUN   == dd_icm_20600_data_s.dev_state_s.state_e );

        break;
    }
}

BOOLEAN dd_icm_20600_reset_soft( void )
{
    return dd_i2c_read_modify_write_bit( DD_ICM_20600_I2C_ADDR,
                                         DD_ICM_20600_PWR_MGMT_1,
                                         DD_ICM_20600_PWR_MGMT_1_DEVICE_RESET_B,
                                         TRUE );
}

BOOLEAN dd_icm_20600_who_am_i_read( DD_ICM_20600_DATA* p_input_data_s )
{
    U8 register_value_u8;

    if ( NULL != p_input_data_s )
    {
        if ( TRUE == dd_i2c_read_single( DD_ICM_20600_I2C_ADDR, DD_ICM_20600_WHO_AM_I, &register_value_u8 ) )
        {
            p_input_data_s->chip_id_u8 = register_value_u8;
        }
        else
        {
            /* Reset chip id */
            p_input_data_s->chip_id_u8 = 0xFF;
            return FALSE;
        }
    }
    else
    {
        assert( NULL != p_input_data_s );
        return FALSE;
    }

    return TRUE;
}

PRIVATE BOOLEAN dd_icm_20600_set_accel_full_scale( const DD_ICM_20600_AFS scale_e )
{
    U8  scale_u8;
    F32 resolution_f32;

    switch ( scale_e )
    {
    case DD_ICM_20600_AFS_2G:
        scale_u8       = DD_ICM_20600_ACCL_CFG_FS_SEL_2G;
        resolution_f32 = DD_ICM_20600_ACCL_REL_2G;
        break;

    case DD_ICM_20600_AFS_4G:
        scale_u8       = DD_ICM_20600_ACCL_CFG_FS_SEL_4G;
        resolution_f32 = DD_ICM_20600_ACCL_REL_4G;
        break;

    case DD_ICM_20600_AFS_8G:
        scale_u8       = DD_ICM_20600_ACCL_CFG_FS_SEL_8G;
        resolution_f32 = DD_ICM_20600_ACCL_REL_8G;
        break;

    case DD_ICM_20600_AFS_16G:
        scale_u8       = DD_ICM_20600_ACCL_CFG_FS_SEL_16G;
        resolution_f32 = DD_ICM_20600_ACCL_REL_16G;
        break;

    default:
        assert( scale_e == DD_ICM_20600_AFS_2G  );
        assert( scale_e == DD_ICM_20600_AFS_4G  );
        assert( scale_e == DD_ICM_20600_AFS_8G  );
        assert( scale_e == DD_ICM_20600_AFS_16G );

        return FALSE;
    }

    if ( FALSE == dd_i2c_read_modify_write_mask( DD_ICM_20600_I2C_ADDR, DD_ICM_20600_ACCEL_CONFIG, DD_ICM_20600_ACCL_CFG_FS_SEL_MASK, scale_u8 ) )
    {
        return FALSE;
    }
    else
    {
        /* Update global database in case of success */
        dd_icm_20600_data_s.accel_full_scale_sel_e = scale_e;
        dd_icm_20600_data_s.accel_resolution_f32   = resolution_f32;

        ESP_LOGD( DD_ICM_20600_LOG_MSG_TAG, "ACCL - Scale: %i, Res: %f", dd_icm_20600_data_s.accel_full_scale_sel_e,
                                                                            dd_icm_20600_data_s.accel_resolution_f32 );
    }

    return TRUE;
}

PRIVATE BOOLEAN dd_icm_20600_set_gyro_full_scale( const DD_ICM_20600_GFS scale_e )
{
    U8  scale_u8;
    F32 resolution_f32;

    switch ( scale_e )
    {
    case DD_ICM_20600_GFS_250DPS:
        scale_u8       = DD_ICM_20600_GYRO_CFG_FS_SEL_250_DPS;
        resolution_f32 = DD_ICM_20600_GYRO_REL_250_DPS;
        break;

    case DD_ICM_20600_GFS_500DPS:
        scale_u8       = DD_ICM_20600_GYRO_CFG_FS_SEL_500_DPS;
        resolution_f32 = DD_ICM_20600_GYRO_REL_500_DPS;
        break;

    case DD_ICM_20600_GFS_1000DPS:
        scale_u8       = DD_ICM_20600_GYRO_CFG_FS_SEL_1000_DPS;
        resolution_f32 = DD_ICM_20600_GYRO_REL_1000_DPS;
        break;

    case DD_ICM_20600_GFS_2000DPS:
        scale_u8       = DD_ICM_20600_GYRO_CFG_FS_SEL_2000_DPS;
        resolution_f32 = DD_ICM_20600_GYRO_REL_2000_DPS;
        break;

    default:
        assert( scale_e == DD_ICM_20600_GFS_250DPS  );
        assert( scale_e == DD_ICM_20600_GFS_500DPS  );
        assert( scale_e == DD_ICM_20600_GFS_1000DPS );
        assert( scale_e == DD_ICM_20600_GFS_2000DPS );

        return FALSE;
    }

    if ( FALSE == dd_i2c_read_modify_write_mask( DD_ICM_20600_I2C_ADDR, DD_ICM_20600_GYRO_CONFIG, DD_ICM_20600_GYRO_CFG_FS_SEL_MASK, scale_u8 ) )
    {
        return FALSE;
    }
    else
    {
        /* Update global database in case of success */
        dd_icm_20600_data_s.gyro_full_scale_sel_e = scale_e;
        dd_icm_20600_data_s.gyro_resolution_f32 = resolution_f32;

        ESP_LOGD( DD_ICM_20600_LOG_MSG_TAG, "GYRO - Scale: %i, Res: %f", dd_icm_20600_data_s.gyro_full_scale_sel_e,
                                                                            dd_icm_20600_data_s.gyro_resolution_f32 );
    }

    return TRUE;
}

BOOLEAN dd_icm_20600_temperature_read( DD_ICM_20600_DATA* p_input_data_s )
{
    U8 register_data_vu8[2];

    if ( NULL != p_input_data_s )
    {
        if ( FALSE == dd_i2c_read_burst( DD_ICM_20600_I2C_ADDR, DD_ICM_20600_TEMP_OUT_H, register_data_vu8, sizeof( register_data_vu8 ) ) )
        {
            p_input_data_s->temperature_raw_u16 =  0xFFFF;
            p_input_data_s->temperature_deg_f32 = -1000.0F;

            return FALSE;
        }
        else
        {
            p_input_data_s->temperature_raw_u16 = ( register_data_vu8[0] << 8U ) | register_data_vu8[1];
            p_input_data_s->temperature_deg_f32 = ( p_input_data_s->temperature_raw_u16 * DD_ICM_20600_TEMP_SENSITIVITY_INV ) + DD_ICM_20600_TEMP_ROOM_OFFSET;
        }
    }
    else
    {
        assert( NULL != p_input_data_s );
        return FALSE;
    }

    ESP_LOGD( DD_ICM_20600_LOG_MSG_TAG, "Temp: %0.2f", dd_icm_20600_data_s.temperature_deg_f32 );

    return TRUE;
}

BOOLEAN dd_icm_20600_accel_data_read_raw( DD_ICM_20600_DATA* p_input_data_s )
{
    U8 register_data_vu8[2U * DD_ICM_20600_ACCEL_SIZE];

    if ( NULL != p_input_data_s )
    {
        if ( FALSE == dd_i2c_read_burst( DD_ICM_20600_I2C_ADDR, DD_ICM_20600_ACCEL_XOUT_H, register_data_vu8, sizeof( register_data_vu8 ) ) )
        {
            p_input_data_s->accel_data_raw_s16[DD_ICM_20600_ACCEL_X] = 0U;
            p_input_data_s->accel_data_raw_s16[DD_ICM_20600_ACCEL_Y] = 0U;
            p_input_data_s->accel_data_raw_s16[DD_ICM_20600_ACCEL_Z] = 0U;

            return FALSE;
        }
        else
        {
            p_input_data_s->accel_data_raw_s16[DD_ICM_20600_ACCEL_X] = ( register_data_vu8[0] << 8U ) | register_data_vu8[1];
            p_input_data_s->accel_data_raw_s16[DD_ICM_20600_ACCEL_Y] = ( register_data_vu8[2] << 8U ) | register_data_vu8[3];
            p_input_data_s->accel_data_raw_s16[DD_ICM_20600_ACCEL_Z] = ( register_data_vu8[4] << 8U ) | register_data_vu8[5];
        }
    }
    else
    {
        assert( NULL != p_input_data_s );
        return FALSE;
    }

    ESP_LOGD( DD_ICM_20600_LOG_MSG_TAG, "XA: %i, YA: %i, ZA: %i", p_input_data_s->accel_data_raw_s16[DD_ICM_20600_GYRO_X],
                                                                  p_input_data_s->accel_data_raw_s16[DD_ICM_20600_GYRO_Y],
                                                                  p_input_data_s->accel_data_raw_s16[DD_ICM_20600_GYRO_Z] );

    return TRUE;
}

BOOLEAN dd_icm_20600_gyro_data_read_raw( DD_ICM_20600_DATA* p_input_data_s )
{
    U8 register_data_vu8[2U * DD_ICM_20600_ACCEL_SIZE];

    if ( NULL != p_input_data_s )
    {
        if ( FALSE == dd_i2c_read_burst( DD_ICM_20600_I2C_ADDR, DD_ICM_20600_GYRO_XOUT_H, register_data_vu8, sizeof( register_data_vu8 ) ) )
        {
            p_input_data_s->gyro_data_raw_s16[DD_ICM_20600_GYRO_X] = 0U;
            p_input_data_s->gyro_data_raw_s16[DD_ICM_20600_GYRO_Y] = 0U;
            p_input_data_s->gyro_data_raw_s16[DD_ICM_20600_GYRO_Z] = 0U;

            return FALSE;
        }
        else
        {
            p_input_data_s->gyro_data_raw_s16[DD_ICM_20600_GYRO_X] = ( register_data_vu8[0] << 8U ) | register_data_vu8[1];
            p_input_data_s->gyro_data_raw_s16[DD_ICM_20600_GYRO_Y] = ( register_data_vu8[2] << 8U ) | register_data_vu8[3];
            p_input_data_s->gyro_data_raw_s16[DD_ICM_20600_GYRO_Z] = ( register_data_vu8[4] << 8U ) | register_data_vu8[5];
        }
    }
    else
    {
        assert( NULL != p_input_data_s );
        return FALSE;
    }

    ESP_LOGD( DD_ICM_20600_LOG_MSG_TAG, "XG: %i, YG: %i, ZG: %i", p_input_data_s->gyro_data_raw_s16[DD_ICM_20600_GYRO_X],
                                                                  p_input_data_s->gyro_data_raw_s16[DD_ICM_20600_GYRO_Y],
                                                                  p_input_data_s->gyro_data_raw_s16[DD_ICM_20600_GYRO_Z] );

    return TRUE;
}

/* Accelerometer and gyroscope self test; check calibration wrt factory settings
   Should return percent deviation from factory trim values, +/- 14 or less deviation is a pass */
PRIVATE BOOLEAN dd_icm_20600_self_test( DD_ICM_20600_DATA* p_data_s )
{
    U8  idx_u8;
    U8  register_data_vu8[4];
    U8  self_test_vu8[DD_ICM_20600_SELF_TEST_SIZE];
    F32 factory_trim_vf32[DD_ICM_20600_SELF_TEST_SIZE];

    ESP_LOGD( DD_ICM_20600_LOG_MSG_TAG, "Self test started ..." );

    /* Check NULL pointer */
    if ( NULL != p_data_s )
    {
        /* Enable self test on all three axes and set accelerometer range to +/- 8 g */
        if ( FALSE == dd_i2c_write_single( DD_ICM_20600_I2C_ADDR, DD_ICM_20600_GYRO_CONFIG, 0xF0 ) )
        {
            return FALSE;
        }

        /* Enable self test on all three axes and set gyro range to +/- 250 degrees/s */
        if ( FALSE == dd_i2c_write_single( DD_ICM_20600_I2C_ADDR, DD_ICM_20600_ACCEL_CONFIG, 0xE0 ) )
        {
            return FALSE;
        }

        /* Delay a while to let the device execute the self-test */
        vTaskDelay( 3 * portTICK_PERIOD_MS );

        /* X-axis, Y-axis, Z-axis and Mixed-axis self-test results */
        if ( FALSE == dd_i2c_read_burst( DD_ICM_20600_I2C_ADDR, DD_ICM_20600_SELF_TEST_X, register_data_vu8, sizeof( register_data_vu8 ) ) )
        {
            return FALSE;
        }

        /* Extract the acceleration test results first */
        self_test_vu8[DD_ICM_20600_SELF_TEST_XA] = ( register_data_vu8[0] >> 3 ) | ( register_data_vu8[3] & 0x30 ) >> 4;   /* XA_TEST result is a five-bit unsigned integer */
        self_test_vu8[DD_ICM_20600_SELF_TEST_YA] = ( register_data_vu8[1] >> 3 ) | ( register_data_vu8[3] & 0x0C ) >> 2;   /* YA_TEST result is a five-bit unsigned integer */
        self_test_vu8[DD_ICM_20600_SELF_TEST_ZA] = ( register_data_vu8[2] >> 3 ) | ( register_data_vu8[3] & 0x03 ) >> 0;   /* ZA_TEST result is a five-bit unsigned integer */

        /* Extract the gyration test results */
        self_test_vu8[DD_ICM_20600_SELF_TEST_XG] = register_data_vu8[0] & 0x1F;   // XG_TEST result is a five-bit unsigned integer
        self_test_vu8[DD_ICM_20600_SELF_TEST_YG] = register_data_vu8[1] & 0x1F;   // YG_TEST result is a five-bit unsigned integer
        self_test_vu8[DD_ICM_20600_SELF_TEST_ZG] = register_data_vu8[2] & 0x1F;   // ZG_TEST result is a five-bit unsigned integer

        /* Process results to allow final comparison with factory set values */
        factory_trim_vf32[DD_ICM_20600_SELF_TEST_XA] = ( 4096.0F * 0.34F  ) * ( pow( ( 0.92F / 0.34F ), ( ( self_test_vu8[DD_ICM_20600_SELF_TEST_XA] - 1.0F ) / 30.0F ) ) );  /* FT[Xa] factory trim calculation */
        factory_trim_vf32[DD_ICM_20600_SELF_TEST_YA] = ( 4096.0F * 0.34F  ) * ( pow( ( 0.92F / 0.34F ), ( ( self_test_vu8[DD_ICM_20600_SELF_TEST_YA] - 1.0F ) / 30.0F ) ) );  /* FT[Ya] factory trim calculation */
        factory_trim_vf32[DD_ICM_20600_SELF_TEST_ZA] = ( 4096.0F * 0.34F  ) * ( pow( ( 0.92F / 0.34F ), ( ( self_test_vu8[DD_ICM_20600_SELF_TEST_ZA] - 1.0F ) / 30.0F ) ) );  /* FT[Za] factory trim calculation */
        factory_trim_vf32[DD_ICM_20600_SELF_TEST_XG] = ( 25.0F   * 131.0F ) * ( pow( 1.046F, ( self_test_vu8[DD_ICM_20600_SELF_TEST_XG] - 1.0F ) ) );                         /* FT[Xg] factory trim calculation */
        factory_trim_vf32[DD_ICM_20600_SELF_TEST_YG] = ( -25.0F  * 131.0F ) * ( pow( 1.046F, ( self_test_vu8[DD_ICM_20600_SELF_TEST_YG] - 1.0F ) ) );                         /* FT[Yg] factory trim calculation */
        factory_trim_vf32[DD_ICM_20600_SELF_TEST_ZG] = ( 25.0F   * 131.0F ) * ( pow( 1.046F, ( self_test_vu8[DD_ICM_20600_SELF_TEST_ZG] - 1.0F ) ) );                         /* FT[Zg] factory trim calculation */

        /* Output self-test results and factory trim calculation if desired */
        ESP_LOGD( DD_ICM_20600_LOG_MSG_TAG, "Self Test Result:" );
        ESP_LOGD( DD_ICM_20600_LOG_MSG_TAG, "Acceleration: X: %i Y: %i Z: %i", self_test_vu8[DD_ICM_20600_SELF_TEST_XA],
                                                                               self_test_vu8[DD_ICM_20600_SELF_TEST_YA],
                                                                               self_test_vu8[DD_ICM_20600_SELF_TEST_ZA] );

        ESP_LOGD( DD_ICM_20600_LOG_MSG_TAG, "Gyration: X: %i Y: %i Z: %i", self_test_vu8[DD_ICM_20600_SELF_TEST_XG],
                                                                           self_test_vu8[DD_ICM_20600_SELF_TEST_YG],
                                                                           self_test_vu8[DD_ICM_20600_SELF_TEST_ZG] );

        ESP_LOGD( DD_ICM_20600_LOG_MSG_TAG, "Factory trim calculation:\nXA: %0.3f\nYA: %0.3f\nZA: %0.3f\nXG: %0.3f\nYG: %0.3f\nZG: %0.3f",
                                            factory_trim_vf32[DD_ICM_20600_SELF_TEST_XA],
                                            factory_trim_vf32[DD_ICM_20600_SELF_TEST_YA],
                                            factory_trim_vf32[DD_ICM_20600_SELF_TEST_ZA],
                                            factory_trim_vf32[DD_ICM_20600_SELF_TEST_XG],
                                            factory_trim_vf32[DD_ICM_20600_SELF_TEST_YG],
                                            factory_trim_vf32[DD_ICM_20600_SELF_TEST_ZG] );

        /* Report results as a ratio of (STR - FT)/FT; the change from Factory Trim of the Self-Test Response
           To get to percent, must multiply by 100 and subtract result from 100 */
        for ( idx_u8 = 0; idx_u8 < DD_ICM_20600_SELF_TEST_SIZE; ++idx_u8 )
        {
            /* Store self test data */
            p_data_s->self_test_vu8[idx_u8] = self_test_vu8[idx_u8];

            /* Store factory trim data */
            p_data_s->factory_trim_vf32[idx_u8] = factory_trim_vf32[idx_u8];

            /* Calculate deviation of factory trim values in percent, +/- 14 or less deviation is a pass */
            p_data_s->fac_trim_deviation_vf32[idx_u8] = 100.0F + 100.0F * ( self_test_vu8[idx_u8] - factory_trim_vf32[idx_u8] ) / factory_trim_vf32[idx_u8];

            ESP_LOGD( DD_ICM_20600_LOG_MSG_TAG, "Factory trim deviation [+/- 14 or less deviation is a pass] %i: %0.2f", idx_u8, p_data_s->fac_trim_deviation_vf32[idx_u8] );
        }

        /* Check if all factory deviations are below DD_ICM_20600_ALLOWED_FAC_DEVIATION */
        if (    ( DD_ICM_20600_ALLOWED_FAC_DEVIATION < p_data_s->fac_trim_deviation_vf32[DD_ICM_20600_SELF_TEST_XA] )
             || ( DD_ICM_20600_ALLOWED_FAC_DEVIATION < p_data_s->fac_trim_deviation_vf32[DD_ICM_20600_SELF_TEST_YA] )
             || ( DD_ICM_20600_ALLOWED_FAC_DEVIATION < p_data_s->fac_trim_deviation_vf32[DD_ICM_20600_SELF_TEST_ZA] )
             || ( DD_ICM_20600_ALLOWED_FAC_DEVIATION < p_data_s->fac_trim_deviation_vf32[DD_ICM_20600_SELF_TEST_XG] )
             || ( DD_ICM_20600_ALLOWED_FAC_DEVIATION < p_data_s->fac_trim_deviation_vf32[DD_ICM_20600_SELF_TEST_YG] )
             || ( DD_ICM_20600_ALLOWED_FAC_DEVIATION < p_data_s->fac_trim_deviation_vf32[DD_ICM_20600_SELF_TEST_ZG] ) )
        {

            ESP_LOGW( DD_ICM_20600_LOG_MSG_TAG, "Self test failed!" );
            return FALSE;
        }
    }
    else
    {
        assert( NULL != p_data_s );
        return FALSE;
    }

    ESP_LOGD( DD_ICM_20600_LOG_MSG_TAG, "Self test passed" );
    return TRUE;
}

// Function which accumulates gyro and accelerometer data after device initialization. It calculates the average
// of the at-rest readings and then loads the resulting offsets into accelerometer and gyro bias registers.
BOOLEAN dd_icm_20600_calibrate( F32* p_gyro_bias_f32,
                                F32* p_accel_bias_f32 )
{
    U8  register_data_vu8[12]; /* data array to hold accelerometer and gyro x, y, z, data */
    U16 idx_u16;
    U16 packet_cnt_u16;
    U16 fifo_cnt_u16;
    S32 gyro_bias_vs32[3]      = {0U, 0U, 0U};
    S32 accel_bias_vs32[3]     = {0U, 0U, 0U};
    S16 accel_temp_vs32[3]     = {0U, 0U, 0U};
    S32 accel_bias_reg_vs32[3] = {0U, 0U, 0U}; /* A place to hold the factory accelerometer trim biases */
    U8  mask_bit_vu8[3]        = {0U, 0U, 0U}; /* Define array to hold mask bit for each accelerometer bias axis */
    U32 mask_u32               = 1UL;          /* Define mask for temperature compensation bit 0 of lower byte of accelerometer bias registers */
    S16 gyro_temp_vs32[3]      = {0U, 0U, 0U};
    U16 gyro_sensitivity_u16   = 131U;         /* = 131 LSB/degrees/sec */
    U16 accel_sensitivity_u16  = 16384U;       /* = 16384 LSB/g */

    /* Reset device, reset all registers, clear gyro and accelerometer bias registers.
     * Write a one to bit 7 reset bit; toggle reset device */
    if ( FALSE == dd_i2c_write_single( DD_ICM_20600_I2C_ADDR, DD_ICM_20600_PWR_MGMT_1, 0x80 ) )
    {
        return FALSE;
    }

    /* Delay a while to let the device execute the self-test */
    vTaskDelay( portTICK_PERIOD_MS );

    /* Get stable time source. Set clock source to be PLL with x-axis
     * gyroscope reference, bits 2:0 = 001 */
    if ( FALSE == dd_i2c_write_single( DD_ICM_20600_I2C_ADDR, DD_ICM_20600_PWR_MGMT_1, 0x01 ) )
    {
        return FALSE;
    }

    if ( FALSE == dd_i2c_write_single( DD_ICM_20600_I2C_ADDR, DD_ICM_20600_PWR_MGMT_1, 0x00 ) )
    {
        return FALSE;
    }

    vTaskDelay( 2 * portTICK_PERIOD_MS );

    /* Configure device for bias calculation */

    /* Disable all interrupts */
    if ( FALSE == dd_i2c_write_single( DD_ICM_20600_I2C_ADDR,  DD_ICM_20600_INT_ENABLE, 0x00 ) )
    {
        return FALSE;
    }

    /* Disable FIFO */
    if ( FALSE == dd_i2c_write_single( DD_ICM_20600_I2C_ADDR, DD_ICM_20600_FIFO_EN, 0x00 ) )
    {
        return FALSE;
    }

    /* Turn on internal clock source */
    if ( FALSE == dd_i2c_write_single( DD_ICM_20600_I2C_ADDR, DD_ICM_20600_PWR_MGMT_1, 0x00 ) )
    {
        return FALSE;
    }

    /* Disable I2C master */
    if ( FALSE == dd_i2c_write_single( DD_ICM_20600_I2C_ADDR, DD_ICM_20600_I2C_MST_CTRL, 0x00 ) )
    {
        return FALSE;
    }

    /* Disable FIFO and I2C master modes */
    if ( FALSE == dd_i2c_write_single( DD_ICM_20600_I2C_ADDR, DD_ICM_20600_USER_CTRL, 0x00 ) )
    {
        return FALSE;
    }

    /* Reset FIFO and DMP */
    if ( FALSE == dd_i2c_write_single( DD_ICM_20600_I2C_ADDR, DD_ICM_20600_USER_CTRL, 0x0C ) )
    {
        return FALSE;
    }

    vTaskDelay( portTICK_PERIOD_MS );

    /* Configure gyro and accelerometer for bias calculation
     * Set low-pass filter to 188 Hz */
    if ( FALSE == dd_i2c_write_single( DD_ICM_20600_I2C_ADDR, DD_ICM_20600_CONFIG, 0x01 ) )
    {
        return FALSE;
    }


    /* Set sample rate to 1 kHz */
    if ( FALSE == dd_i2c_write_single( DD_ICM_20600_I2C_ADDR, DD_ICM_20600_SMPLRT_DIV, 0x00 ) )
    {
        return FALSE;
    }

    /* Set gyro full-scale to 250 degrees per second, maximum sensitivity */
    if ( FALSE == dd_i2c_write_single( DD_ICM_20600_I2C_ADDR, DD_ICM_20600_GYRO_CONFIG, 0x00 ) )
    {
        return FALSE;
    }

    /* Set accelerometer full-scale to 2 g, maximum sensitivity */
    if ( FALSE == dd_i2c_write_single( DD_ICM_20600_I2C_ADDR, DD_ICM_20600_ACCEL_CONFIG, 0x00 ) )
    {
        return FALSE;
    }

    /* Configure FIFO to capture accelerometer and gyro data for bias calculation
     * Enable FIFO */
    if ( FALSE == dd_i2c_write_single( DD_ICM_20600_I2C_ADDR, DD_ICM_20600_USER_CTRL, 0x40 ) )
    {
        return FALSE;
    }


    /* Enable gyro and accelerometer sensors for FIFO  (max size 1024 bytes in MPU-6050) */
    if ( FALSE == dd_i2c_write_single( DD_ICM_20600_I2C_ADDR, DD_ICM_20600_FIFO_EN, 0x78 ) )
    {
        return FALSE;
    }

    /* accumulate 80 samples in 80 milliseconds = 960 bytes */
    vTaskDelay( 2U * portTICK_PERIOD_MS );

    /* At end of sample accumulation, turn off FIFO sensor read
    * Disable gyro and accelerometer sensors for FIFO */
    if ( FALSE == dd_i2c_write_single( DD_ICM_20600_I2C_ADDR, DD_ICM_20600_FIFO_EN, 0x00 ) )
    {
        return FALSE;
    }

    /* read FIFO sample count */
    if ( FALSE == dd_i2c_read_burst( DD_ICM_20600_I2C_ADDR, DD_ICM_20600_FIFO_COUNTH, &register_data_vu8[0], 2U ) )
    {
        return FALSE;
    }

    fifo_cnt_u16 = ( ( (U16) register_data_vu8[0] << 8U ) | register_data_vu8[1] );

    /* How many sets of full gyro and accelerometer data for averaging */
    packet_cnt_u16 = fifo_cnt_u16 / 12U;

    for ( idx_u16 = 0; idx_u16 < packet_cnt_u16; idx_u16++ )
    {
        /* Read data for averaging */
        if ( FALSE == dd_i2c_read_burst( DD_ICM_20600_I2C_ADDR, DD_ICM_20600_FIFO_R_W, &register_data_vu8[0], sizeof( register_data_vu8 ) ) )
        {
            return FALSE;
        }

        /* Form signed 16-bit integer for each sample in FIFO */
        accel_temp_vs32[0] = ( U16 )( ( (U16) register_data_vu8[0] <<  8U ) | register_data_vu8[1] );
        accel_temp_vs32[1] = ( U16 )( ( (U16) register_data_vu8[2] <<  8U ) | register_data_vu8[3] );
        accel_temp_vs32[2] = ( U16 )( ( (U16) register_data_vu8[4] <<  8U ) | register_data_vu8[5] );
        gyro_temp_vs32[0]  = ( U16 )( ( (U16) register_data_vu8[6] <<  8U ) | register_data_vu8[7] );
        gyro_temp_vs32[1]  = ( U16 )( ( (U16) register_data_vu8[8] <<  8U ) | register_data_vu8[9] );
        gyro_temp_vs32[2]  = ( U16 )( ( (U16) register_data_vu8[10] << 8U ) | register_data_vu8[11] );

        /* Sum individual signed 16-bit biases to get accumulated signed 32-bit biases */
        accel_bias_vs32[0] += (S32) accel_temp_vs32[0];
        accel_bias_vs32[1] += (S32) accel_temp_vs32[1];
        accel_bias_vs32[2] += (S32) accel_temp_vs32[2];
        gyro_bias_vs32[0]  += (S32) gyro_temp_vs32[0];
        gyro_bias_vs32[1]  += (S32) gyro_temp_vs32[1];
        gyro_bias_vs32[2]  += (S32) gyro_temp_vs32[2];
    }

    /* Normalize sums to get average count biases */
    accel_bias_vs32[0] /= (S32) packet_cnt_u16;
    accel_bias_vs32[1] /= (S32) packet_cnt_u16;
    accel_bias_vs32[2] /= (S32) packet_cnt_u16;
    gyro_bias_vs32[0]  /= (S32) packet_cnt_u16;
    gyro_bias_vs32[1]  /= (S32) packet_cnt_u16;
    gyro_bias_vs32[2]  /= (S32) packet_cnt_u16;

    /* Remove gravity from the z-axis accelerometer bias calculation */
    if ( accel_bias_vs32[2] > 0L )
    {
        accel_bias_vs32[2] -= (S32) accel_sensitivity_u16;
    }
    else
    {
        accel_bias_vs32[2] += (S32) accel_sensitivity_u16;
    }

    /* Construct the gyro biases for push to the hardware gyro bias registers, which are reset to zero upon device startup */

    /* Divide by 4 to get 32.9 LSB per deg/s to conform to expected bias input format */
    register_data_vu8[0] = ( ( -gyro_bias_vs32[0] / 4 ) >> 8 ) & 0xFF;

    /* Biases are additive, so change sign on calculated average gyro biases */
    register_data_vu8[1] = ( -gyro_bias_vs32[0] / 4 ) & 0xFF;
    register_data_vu8[2] = ( ( -gyro_bias_vs32[1] / 4 ) >> 8 ) & 0xFF;
    register_data_vu8[3] = ( -gyro_bias_vs32[1] / 4 ) & 0xFF;
    register_data_vu8[4] = ( ( -gyro_bias_vs32[2] / 4 ) >> 8 ) & 0xFF;
    register_data_vu8[5] = ( -gyro_bias_vs32[2] / 4 ) & 0xFF;

    /* Push gyro biases to hardware registers */
    if ( dd_i2c_write_burst( DD_ICM_20600_I2C_ADDR, DD_ICM_20600_XG_OFFS_USRH, &register_data_vu8[0], 6U ) )
    {
        return FALSE;
    }

    /* Construct gyro bias in deg/s for later manual subtraction */
    p_gyro_bias_f32[0] = (F32) gyro_bias_vs32[0] / (F32) gyro_sensitivity_u16;
    p_gyro_bias_f32[1] = (F32) gyro_bias_vs32[1] / (F32) gyro_sensitivity_u16;
    p_gyro_bias_f32[2] = (F32) gyro_bias_vs32[2] / (F32) gyro_sensitivity_u16;

    /* Construct the accelerometer biases for push to the hardware accelerometer bias registers. These registers contain
     * factory trim values which must be added to the calculated accelerometer biases; on boot up these registers will
     * hold non-zero values. In addition, bit 0 of the lower byte must be preserved since it is used for temperature
     * compensation calculations. Accelerometer bias registers expect bias input as 2048 LSB/g, so that the accelerometer
     * biases calculated above must be divided by 8. */

    /* Read factory accelerometer trim values */
    if ( dd_i2c_read_burst( DD_ICM_20600_I2C_ADDR, DD_ICM_20600_XA_OFFSET_H, &register_data_vu8[0], 6U ) )
    {
        return FALSE;
    }

    accel_bias_reg_vs32[0] = ( U16 )( (U16) register_data_vu8[0] << 8 ) | register_data_vu8[1];
    accel_bias_reg_vs32[1] = ( U16 )( (U16) register_data_vu8[2] << 8 ) | register_data_vu8[3];
    accel_bias_reg_vs32[2] = ( U16 )( (U16) register_data_vu8[4] << 8 ) | register_data_vu8[5];

    for ( idx_u16 = 0; idx_u16 < 3U; idx_u16++ )
    {
        /* If temperature compensation bit is set, record that fact in mask_bit */
        if ( accel_bias_reg_vs32[idx_u16] & mask_u32 )
        {
            mask_bit_vu8[idx_u16] = 0x01;
        }
    }

    /* Construct total accelerometer bias, including calculated average accelerometer bias from above
     * Subtract calculated averaged accelerometer bias scaled to 2048 LSB/g (16 g full scale) */
    accel_bias_reg_vs32[0] -= ( accel_bias_vs32[0] / 8U );
    accel_bias_reg_vs32[1] -= ( accel_bias_vs32[1] / 8U );
    accel_bias_reg_vs32[2] -= ( accel_bias_vs32[2] / 8U );

    register_data_vu8[0] = ( accel_bias_reg_vs32[0] >> 8U ) & 0xFF;
    register_data_vu8[1] = ( accel_bias_reg_vs32[0] ) & 0xFF;

    /* preserve temperature compensation bit when writing back to accelerometer bias registers */
    register_data_vu8[1] = register_data_vu8[1] | mask_bit_vu8[0];
    register_data_vu8[2] = ( accel_bias_reg_vs32[1] >> 8U ) & 0xFF;
    register_data_vu8[3] = ( accel_bias_reg_vs32[1] ) & 0xFF;

    /* preserve temperature compensation bit when writing back to accelerometer bias registers */
    register_data_vu8[3] = register_data_vu8[3] | mask_bit_vu8[1];
    register_data_vu8[4] = ( accel_bias_reg_vs32[2] >> 8U ) & 0xFF;
    register_data_vu8[5] = ( accel_bias_reg_vs32[2] ) & 0xFF;

    /* preserve temperature compensation bit when writing back to accelerometer bias registers */
    register_data_vu8[5] = register_data_vu8[5] | mask_bit_vu8[2];

    /* Push accelerometer biases to hardware registers */
    if ( dd_i2c_write_burst( DD_ICM_20600_I2C_ADDR, DD_ICM_20600_XA_OFFSET_H, &register_data_vu8[0], 6U ) )
    {
        return FALSE;
    }

    /*  Output scaled accelerometer biases for manual subtraction in the main program */
    p_accel_bias_f32[0] = (F32) accel_bias_vs32[0] / (F32) accel_sensitivity_u16;
    p_accel_bias_f32[1] = (F32) accel_bias_vs32[1] / (F32) accel_sensitivity_u16;
    p_accel_bias_f32[2] = (F32) accel_bias_vs32[2] / (F32) accel_sensitivity_u16;

    return TRUE;
}
