/*********************************************************************
    MODULE SPECIFICATION:

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        (c) SEWELA 2020

        @file dlg_log.h
        @details Some detailed description

*********************************************************************/
#ifndef DLG_LOG_H_
#define DLG_LOG_H_

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include <stdio.h>
#include <string.h>

#include "../../types.h"


/*************************************************************/
/*      COMPILE TIME CONFIGURATION                           */
/*************************************************************/
/* Log message tag string */
#define DLG_LOG_LOG_MSG_TAG                      "DLG_LOG"     /**< @details Domain log message tag string */
#define DLG_LOG_DEV_SERIAL_NO                    "XC12C45D"    /**< Device specific serial number string (will be provided by Non-volatile storage library in the future) */
#define DLG_LOG_FILE_DOT_EXTENSION               ".sbf"        /**< Format: .sbf: (s)ewela-(b)inary-(f)ile */
#define DLG_LOG_MAX_FILE_NAME_LENGTH             30U           /**< Maximum number of characters allowed to be used for a file name */
#define DLG_LOG_NUM_FILES_PER_MINUTE             6U            /**< Number of .sbf files per minute */
#define DLG_LOG_FILE_SIZE_IN_KBYTE               20U           /**< File size in kByte */
#define DLG_LOG_MAIN_CYCLE_TIME_MS               100U          /**< Main loop cycle time -> will be replaced with value provided by os_time module in the future */
#define DLG_LOG_USE_FILES_PER_MINUTE             TRUE          /**< Flag to decide whether DLG_LOG_NUM_FILES_PER_MINUTE or DLG_LOG_FILE_SIZE_IN_KBYTE is used to control */


/**
 * @details Flag to enable logging test mode. If defined every
 * signal will be overwritten by log_test_mode_cnt_u8 with an
 * value of 0 <= n <= 255
*/
// #define DLG_LOG_TEST_ENABLE



/*************************************************************
*                      ENUMERATORS                           *
*************************************************************/




/*************************************************************
*      STRUCTURES                                            *
*************************************************************/
/**
 * @details enumerator of ...
 */
typedef struct DLG_LOG_DATA_TAG
{
    char    file_name_vc[DLG_LOG_MAX_FILE_NAME_LENGTH];
    FILE*   p_file_handle;
    U32     num_data_chunk_per_file_u32;
    BOOLEAN logging_enabled_b;
} DLG_LOG_DATA;


/**
 * @details enumerator of ...
 */
typedef struct DLG_LOG_DATA_IN_TYPE_TAG
{
    F32 dlg_time_stamp_in_sec_f32;                  /**< @details Global time stamp in seconds */
    F32 dd_icm_20600_temperature_deg_f32;           /**< @details ICM-20600 Internal core (die) temperature @unit °C */
    F32 dd_icm_20600_factory_trim_xa_f32;           /**< @details ICM-20600 XA factory trim value */
    F32 dd_icm_20600_factory_trim_ya_f32;           /**< @details ICM-20600 YA factory trim value */
    F32 dd_icm_20600_factory_trim_za_f32;           /**< @details ICM-20600 ZA factory trim value */
    F32 dd_icm_20600_factory_trim_xg_f32;           /**< @details ICM-20600 XG factory trim value */
    F32 dd_icm_20600_factory_trim_yg_f32;           /**< @details ICM-20600 YG factory trim value */
    F32 dd_icm_20600_factory_trim_zg_f32;           /**< @details ICM-20600 ZG factory trim value */
    F32 dd_icm_20600_factory_trim_dev_xa_f32;       /**< @details ICM-20600 XA factory trim deviation @unit % */
    F32 dd_icm_20600_factory_trim_dev_ya_f32;       /**< @details ICM-20600 YA factory trim deviation @unit % */
    F32 dd_icm_20600_factory_trim_dev_za_f32;       /**< @details ICM-20600 ZA factory trim deviation @unit % */
    F32 dd_icm_20600_factory_trim_dev_xg_f32;       /**< @details ICM-20600 XG factory trim deviation @unit % */
    F32 dd_icm_20600_factory_trim_dev_yg_f32;       /**< @details ICM-20600 YG factory trim deviation @unit % */
    F32 dd_icm_20600_factory_trim_dev_zg_f32;       /**< @details ICM-20600 ZG factory trim deviation @unit % */
    F32 dd_max_30102_temperature_f32;               /**< @details MAX-30102 Internal core (die) temperature @unit °C */
    F32 dd_adc_raw_level_f32;                       /**< @details ADC level where Full-Scale (4095) is mapped to 1.0 and 0 is mapped to 0.0 */
    F32 dd_adc_filtered_level_f32;                  /**< @details Filtered ADC level where Full-Scale (4095) is mapped to 1.0 and 0 is mapped to 0.0 */
    F32 sense_ts_alpha_filtered_adc_level_f32;      /**< @details Alpha filtered ADC level where Full-Scale (4095) is mapped to 1.0 and 0 is mapped to 0.0 */
    F32 sense_ts_alpha_beta_filtered_adc_level_f32; /**< @details Alpha/Beta filtered ADC level where Full-Scale (4095) is mapped to 1.0 and 0 is mapped to 0.0 */
    F32 sense_ts_touch_confidence_f32;              /**< @details Touch confidence with a range of 0.0 to 1.0 */
    F32 sense_ts_touch_confidence_max_f32;          /**< @details Maximum touch confidence with a range of 0.0 to 1.0 °C */
    F32 dd_ina_219_shunt_voltage_mV_f32;            /**< @details Voltage across the shunt resistor @unit mV */
    F32 dd_ina_219_bus_voltage_V_f32;               /**< @details Bus voltage @unit V */
    F32 dd_ina_219_power_mW_f32;                    /**< @details Power @unit mW */
    F32 dd_ina_219_current_mA_f32;                  /**< @details Current @unit mA */
    F32 dd_tmp_102_temperature_deg_f32;             /**< @details Current temperature reading @unit °C */
    F32 dd_mcpwm_ch_1_duty_cycle_f32;               /**< @details MCPWM duty cycle for channels 1 */
    F32 dd_mcpwm_ch_2_duty_cycle_f32;               /**< @details MCPWM duty cycle for channels 2 */
    F32 dd_mcpwm_ch_3_duty_cycle_f32;               /**< @details MCPWM duty cycle for channels 3 */
    F32 dd_mcpwm_ch_4_duty_cycle_f32;               /**< @details MCPWM duty cycle for channels 4 */
    F32 dd_mcpwm_ch_5_duty_cycle_f32;               /**< @details MCPWM duty cycle for channels 5 */
    F32 dd_mcpwm_ch_6_duty_cycle_f32;               /**< @details MCPWM duty cycle for channels 6 */
    F32 dd_mcpwm_ch_7_duty_cycle_f32;               /**< @details MCPWM duty cycle for channels 7 */
    F32 dd_mcpwm_ch_8_duty_cycle_f32;               /**< @details MCPWM duty cycle for channels 8 */
    F32 dd_mcpwm_ch_9_duty_cycle_f32;               /**< @details MCPWM duty cycle for channels 9 */
    F32 dd_mcpwm_ch_10_duty_cycle_f32;              /**< @details MCPWM duty cycle for channels 10 */
    F32 dd_mcpwm_ch_11_duty_cycle_f32;              /**< @details MCPWM duty cycle for channels 11 */
    F32 dd_mcpwm_ch_12_duty_cycle_f32;              /**< @details MCPWM duty cycle for channels 12 */

    U32 dd_max_30102_red_data_raw_u32;              /**< @details MAX-30102 Red led ADC raw value */
    U32 dd_max_30102_ir_data_raw_u32;               /**< @details MAX-30102 IR led ADC raw value */

    U16 dd_icm_20600_temperature_raw_u16;           /**< @details Internal core (die) temperature raw data */
    U16 dd_adc_raw_data_u16;                        /**< @details Current raw 12-bit ADC sample @unit LSB */
    U16 dd_adc_previous_raw_data_u16;               /**< @details Previous raw 12-bit ADC sample @unit LSB */
    U16 dd_adc_voltage_u16;                         /**< @details Actual voltage measured at the ADC input pin @unit mV */
    U16 dd_ina_219_power_raw_u16;                   /**< @details Main system power ADC reading */
    U16 dd_ina_219_bus_voltage_raw_u16;             /**< @details Raw most recent bus voltage ADC reading */

    S16 dd_icm_20600_accel_raw_data_x_s16;          /**< @details ICM-20600 X acceleration raw value */
    S16 dd_icm_20600_accel_raw_data_y_s16;          /**< @details ICM-20600 Y acceleration raw value */
    S16 dd_icm_20600_accel_raw_data_z_s16;          /**< @details ICM-20600 Z acceleration raw value */
    S16 dd_icm_20600_gyro_raw_data_x_s16;           /**< @details ICM-20600 X gyroscope raw value */
    S16 dd_icm_20600_gyro_raw_data_y_s16;           /**< @details ICM-20600 Y gyroscope raw value */
    S16 dd_icm_20600_gyro_raw_data_z_s16;           /**< @details ICM-20600 Z gyroscope raw value */
    S16 dd_i2c_error_code_s16;                      /**< @details I2C Current error code */
    S16 dd_ina_219_shunt_voltage_raw_s16;           /**< @details Raw ADC reading representing the voltage across the shunt resistor */
    S16 dd_ina_219_current_raw_s16;                 /**< @details Main system current ADC reading */
    S16 dd_tmp_102_temperature_raw_s16;             /**< @details Raw ADC temperature reading @unit LSB */

    U8  dd_icm_20600_chip_id_u8;                    /**< @details ICM-20600 Unique chip id */
    U8  dd_icm_20600_dev_state_u8;                  /**< @details ICM-20600 Current device state */
    U8  dd_icm_20600_is_calibrated_u8;              /**< @details ICM-20600 TRUE in case device has been calibrated */
    U8  dd_icm_20600_self_test_passed_u8;           /**< @details ICM-20600 TRUE in case self-test has passed */
    U8  dd_icm_20600_self_test_xa_u8;               /**< @details ICM-20600 X acceleration self test value */
    U8  dd_icm_20600_self_test_ya_u8;               /**< @details ICM-20600 Y acceleration self test value */
    U8  dd_icm_20600_self_test_za_u8;               /**< @details ICM-20600 Z acceleration self test value */
    U8  dd_icm_20600_self_test_xg_u8;               /**< @details ICM-20600 X gyroscope self test value */
    U8  dd_icm_20600_self_test_yg_u8;               /**< @details ICM-20600 Y gyroscope self test value */
    U8  dd_icm_20600_self_test_zg_u8;               /**< @details ICM-20600 Z gyroscope self test value */
    U8  dd_i2c_is_error_present_u8;                 /**< @details I2C TRUE in case I2C error is present */
    U8  dd_i2c_access_type_u8;                      /**< @details I2C Current access type, e.g. READ, WRITE */
    U8  dd_i2c_device_addr_u8;                      /**< @details I2C Current device address */
    U8  dd_i2c_register_addr_u8;                    /**< @details I2C Current register address that has been accessed */
    U8  dd_max_30102_int_status_u8;                 /**< @details MAX-30102 Status of all interrupt bits */
    U8  dd_max_30102_part_id_u8;                    /**< @details MAX-30102 Devices part id*/
    U8  dd_max_30102_rev_id_u8;                     /**< @details MAX-30102 Devices revision id*/
    U8  dd_max_30102_read_ptr_u8;                   /**< @details MAX-30102 Current read pointer position */
    U8  dd_max_30102_write_ptr_u8;                  /**< @details MAX-30102 Current read write position */
    U8  dd_max_30102_mode_u8;                       /**< @details MAX-30102 Current device mode */
    U8  dd_max_30102_temperature_raw_int_u8;        /**< @details MAX-30102 Internal raw die temperature, integer part */
    U8  dd_max_30102_temperature_raw_frac_u8;       /**< @details MAX-30102 Internal raw die temperature, fractional part */
    U8  dd_ina_219_shunt_voltage_range_u8;          /**< @details Current full-scale current sense (input) voltage range */
    U8  dd_ina_219_bus_voltage_range_u8;            /**< @details Current bus voltage (input voltage) range */
    U8  dd_mcpwm_ch_1_mode_u8;                      /**< @details MCPWM mode for channels 1 */
    U8  dd_mcpwm_ch_2_mode_u8;                      /**< @details MCPWM mode for channels 2 */
    U8  dd_mcpwm_ch_3_mode_u8;                      /**< @details MCPWM mode for channels 3 */
    U8  dd_mcpwm_ch_4_mode_u8;                      /**< @details MCPWM mode for channels 4 */
    U8  dd_mcpwm_ch_5_mode_u8;                      /**< @details MCPWM mode for channels 5 */
    U8  dd_mcpwm_ch_6_mode_u8;                      /**< @details MCPWM mode for channels 6 */
    U8  dd_mcpwm_ch_7_mode_u8;                      /**< @details MCPWM mode for channels 7 */
    U8  dd_mcpwm_ch_8_mode_u8;                      /**< @details MCPWM mode for channels 8 */
    U8  dd_mcpwm_ch_9_mode_u8;                      /**< @details MCPWM mode for channels 9 */
    U8  dd_mcpwm_ch_10_mode_u8;                     /**< @details MCPWM mode for channels 10 */
    U8  dd_mcpwm_ch_11_mode_u8;                     /**< @details MCPWM mode for channels 11 */
    U8  dd_mcpwm_ch_12_mode_u8;                     /**< @details MCPWM mode for channels 12 */

    U8 dlg_global_msg_cnt_u8;                       /**< @details Global message counter, shall always be the last entry */

} DLG_LOG_DATA_IN_TYPE;

/*************************************************************/
/*      CLASS DEFINITION                                     */
/*************************************************************/
class DLG_LOG_C {

  private:
    static DLG_LOG_DATA data_s;
    static U32          file_count_u32;
    static U32          data_chunk_cnt_u32;
    static U32          test_mode_cnt_u32;

    static BOOLEAN      handle_file( U32 id_u32 );
    static void         create_dd_i2c_data_frame( void );
    static void         create_dd_adc_data_frame( void );
    static void         create_dd_mcpwm_data_frame( void );
    static void         create_dd_icm_20600_data_frame( void );
    static void         create_dd_max_30102_data_frame( void );
    static void         create_dd_ina_219_data_frame( void );
    static void         create_dd_tmp_102_data_frame( void );
    static void         create_sense_ts_data_frame( void );

  public:
    /**
     * @details This function initialized the I2C interface
     */
    static void init( void );

    static void main( DLG_LOG_DATA_IN_TYPE* p_data_in_s );
};

#endif /* DLG_LOG_H_ */
