/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        (c) SEWELA 2020

        @file dd_public_types.h
        @details Some detailed description

*********************************************************************/
#ifndef DD_PUBLIC_TYPES_H
#define DD_PUBLIC_TYPES_H

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include <stdio.h>
#include <sys/stat.h>

#include <esp_err.h>

/* To get all MCPWM dependent defines/enums */
#include "driver/mcpwm.h"

#include "../../types.h"

/* TODO: DD-Types include concept needs to be reworked */
#include "../Config/dd_types_cfg.h"

/*************************************************************/
/*                      ENUMERATORS                           */
/*************************************************************/

/*************************************************************/
/*                          DD                              */
/*************************************************************/

/**
 * @details enumerator of ...
 */
typedef enum DD_STATE_TAG
{
    DD_STATE_INIT = 0, /**< @details ... */
    DD_STATE_TEST,     /**< @details ... */
    DD_STATE_CALIB,    /**< @details ... */
    DD_STATE_RUN,      /**< @details ... */
    DD_STATE_SIZE      /**< @details ... */
} DD_STATE;

typedef struct DD_DEV_STATE_TAG
{
    DD_STATE state_e; /**< @details ... */
} DD_DEV_STATE;

/*************************************************************/
/*                          DD_I2C                           */
/*************************************************************/

/**
 * @details enumerator of ...
 */
typedef enum DD_I2C_ERROR_TAG
{
    DD_I2C_ERROR_FAIL         = ESP_FAIL,             /*!< Generic esp_err_t code indicating failure */
    DD_I2C_ERROR_OK           = ESP_OK,               /*!< esp_err_t value indicating success (no error) */
    DD_I2C_ERROR_INVALID_SIZE = ESP_ERR_INVALID_SIZE, /** @details Acceleration raw data */
    DD_I2C_ERROR_INVALID_ARG  = ESP_ERR_INVALID_ARG,  /** @details Acceleration raw data */
    DD_I2C_ERROR_TIMEOUT      = ESP_ERR_TIMEOUT,      /*!< Operation timed out */
    DD_I2C_ERROR_SIZE
} DD_I2C_ERROR;

/**
 * @details enumerator of ...
 */
typedef enum DD_I2C_ACCESS_TYPE_TAG
{
    DD_I2C_ACCESS_TYPE_RD_SINGLE = 0U, /**< @details ... */
    DD_I2C_ACCESS_TYPE_RD_BURST,       /**< @details ... */
    DD_I2C_ACCESS_TYPE_WD_SINGLE,      /**< @details ... */
    DD_I2C_ACCESS_TYPE_WD_BURST,       /**< @details ... */
    DD_I2C_ACCESS_TYPE_SIZE            /**< @details ... */
} DD_I2C_ACCESS_TYPE;

/*************************************************************/
/*                        DD_ICM_20600                       */
/*************************************************************/

/**
 * @details enumerator of ...
 */
typedef enum DD_ICM_20600_AFS_TAG
{
    DD_ICM_20600_AFS_2G = 0,  /**< @details  2 * 9.81 m/s^2 */
    DD_ICM_20600_AFS_4G,      /**< @details  4 * 9.81 m/s^2 */
    DD_ICM_20600_AFS_8G,      /**< @details  8 * 9.81 m/s^2 */
    DD_ICM_20600_AFS_16G,     /**< @details 16 * 9.81 m/s^2 */
    DD_ICM_20600_SCALE_A_SIZE
} DD_ICM_20600_AFS;

/**
 * @details enumerator of ...
 */
typedef enum DD_ICM_20600_GFS_TAG
{
    DD_ICM_20600_GFS_250DPS = 0, /**< @details  250 °/s */
    DD_ICM_20600_GFS_500DPS,     /**< @details  500 °/s */
    DD_ICM_20600_GFS_1000DPS,    /**< @details 1000 °/s */
    DD_ICM_20600_GFS_2000DPS,    /**< @details 2000 °/s */
    DD_ICM_20600_SCALE_G_SIZE
} DD_ICM_20600_GFS;

/**
 * @details enumerator of ...
 */
typedef enum DD_ICM_20600_ACCEL_TYPE_TAG
{
    DD_ICM_20600_ACCEL_X = 0, /**< @details ... */
    DD_ICM_20600_ACCEL_Y,     /**< @details ... */
    DD_ICM_20600_ACCEL_Z,     /**< @details ... */
    DD_ICM_20600_ACCEL_SIZE   /**< @details ... */
} DD_ICM_20600_ACCEL_TYPE;

/**
 * @details enumerator of ...
 */
typedef enum DD_ICM_20600_GYRO_TYPE_TAG
{
    DD_ICM_20600_GYRO_X = 0, /**< @details ... */
    DD_ICM_20600_GYRO_Y,     /**< @details ... */
    DD_ICM_20600_GYRO_Z,     /**< @details ... */
    DD_ICM_20600_GYRO_SIZE   /**< @details ... */
} DD_ICM_20600_GYRO_TYPE;

/**
 * @details enumerator of ...
 */
typedef enum DD_ICM_20600_SELF_TEST_TAG
{
    DD_ICM_20600_SELF_TEST_XA = 0, /**< @details ... */
    DD_ICM_20600_SELF_TEST_YA,     /**< @details ... */
    DD_ICM_20600_SELF_TEST_ZA,     /**< @details ... */
    DD_ICM_20600_SELF_TEST_XG,     /**< @details ... */
    DD_ICM_20600_SELF_TEST_YG,     /**< @details ... */
    DD_ICM_20600_SELF_TEST_ZG,     /**< @details ... */
    DD_ICM_20600_SELF_TEST_SIZE
} DD_ICM_20600_SELF_TEST;

/*************************************************************/
/*      STRUCTURES                                           */
/*************************************************************/

/*************************************************************/
/*                          DD_I2C                           */
/*************************************************************/

/**
 * @details enumerator of ...
 */
typedef struct DD_I2C_ERROR_INFO_TYPE_TAG
{
    DD_I2C_ERROR       error_e;          /**< @details ... */
    DD_I2C_ACCESS_TYPE access_type_e;    /**< @details ... */
    U8                 device_addr_u8;   /**< @details ... */
    U8                 register_addr_u8; /**< @details ... */
    F32                time_stamp_f32;   /**< @details ... */
} DD_I2C_ERROR_INFO_TYPE;

/**
 * @details enumerator of ...
 */
typedef struct DD_I2C_ERROR_DATA_TYPE_TAG
{
    DD_I2C_ERROR_INFO_TYPE error_info_vs[DD_I2C_ERROR_BUFFER_LENGTH]; /**< @details ... */
    U8                     current_error_idx_u8;                      /**< @details ... */
    U8                     last_error_idx_u8;                         /**< @details ... */
    BOOLEAN                is_error_present_b;                        /**< @details ... */
} DD_I2C_ERROR_DATA_TYPE;

/**
 * @brief   ICM-20600 Output Interface Data Structure
 * @details ICM-20600 Output Interface Data Structure gathers all required
            motion information such as pitch, roll, yaw, ...
 * @ingroup DriverStructures
 */
typedef struct DD_ICM_20600_DATA_TYPE_TAG
{
    U8               chip_id_u8;                                           /**< @details Unique chip id */
    F32              temperature_deg_f32;                                  /**< @details Internal core (die) temperature @unit °C */
    U16              temperature_raw_u16;                                  /**< @details Internal core (die) temperature raw data */
    DD_ICM_20600_AFS accel_full_scale_sel_e;                               /**< @details Acceleration raw data */
    DD_ICM_20600_GFS gyro_full_scale_sel_e;                                /**< @details Acceleration raw data */
    F32              accel_resolution_f32;                                 /**< @details Acceleration raw data */
    F32              gyro_resolution_f32;                                  /**< @details Acceleration raw data */
    S16              accel_data_raw_s16[DD_ICM_20600_ACCEL_SIZE];          /**< @details Acceleration raw data */
    S16              gyro_data_raw_s16[DD_ICM_20600_GYRO_SIZE];            /**< @details Acceleration raw data */
    U8               self_test_vu8[DD_ICM_20600_SELF_TEST_SIZE];           /**< @details main icm-20600 device state */
    BOOLEAN          self_test_passed_b;                                   /**< @details main icm-20600 device state */
    BOOLEAN          is_calibrated_b;                                      /**< @details main icm-20600 device state */
    F32              factory_trim_vf32[DD_ICM_20600_SELF_TEST_SIZE];       /**< @details main icm-20600 device state */
    F32              fac_trim_deviation_vf32[DD_ICM_20600_SELF_TEST_SIZE]; /**< @details main icm-20600 device state */
    DD_DEV_STATE     dev_state_s;                                          /**< @details main icm-20600 device state */
} DD_ICM_20600_DATA_TYPE;

/*************************************************************/
/*                        DD_MAX_30102                       */
/*************************************************************/

/**
 * @details enumerator of ...
 */
typedef enum DD_MAX_30102_PTR_TYPE_TAG
{
    DD_MAX_30102_PTR_TYPE_READ = 0U,   /**< @details ... */
    DD_MAX_30102_PTR_TYPE_WRITE,        /**< @details ... */
    DD_MAX_30102_PTR_TYPE_SIZE
} DD_MAX_30102_PTR_TYPE;

typedef enum DD_MAX_30102_LED_TYPE_TAG
{
    DD_MAX_30102_LED_TYPE_RED = 0U,   /**< @details ... */
    DD_MAX_30102_LED_TYPE_IR,        /**< @details ... */
    DD_MAX_30102_LED_TYPE_PROX,
    DD_MAX_30102_LED_TYPE_SIZE
} DD_MAX_30102_LED_TYPE;

typedef enum DD_MAX_30102_SLOT_TAG
{
    DD_MAX_30102_SLOT_1 = 0U,   /**< @details ... */
    DD_MAX_30102_SLOT_2,        /**< @details ... */
    DD_MAX_30102_SLOT_3,
    DD_MAX_30102_SLOT_4,
    DD_MAX_30102_SLOT_SIZE
} DD_MAX_30102_SLOT;

typedef enum DD_MAX_30102_SLOT_MODE_TAG
{
    DD_MAX_30102_SLOT_MODE_NONE      = 0U,  /**< @details ... */
    DD_MAX_30102_SLOT_MODE_LED_RED,         /**< @details ... */
    DD_MAX_30102_SLOT_MODE_LED_IR,          /**< @details ... */
    DD_MAX_30102_SLOT_MODE_LED_RED_PILOT,
    DD_MAX_30102_SLOT_MODE_LED_IR_PILOT,
    DD_MAX_30102_SLOT_MODE_SIZE
} DD_MAX_30102_SLOT_MODE;

typedef enum DD_MAX_30102_MODE_TAG
{
    DD_MAX_30102_MODE_SLEEP = 0U,
    DD_MAX_30102_MODE_HR,        /**< @details ... */
    DD_MAX_30102_MODE_SPO2,      /**< @details ... */
    DD_MAX_30102_MODE_MULTI_LED  /**< @details ... */
} DD_MAX_30102_MODE;

typedef enum DD_MAX_30102_ADC_RANGE_TAG
{
    DD_MAX_30102_ADC_RANGE_2048 = 0U, /**< @details ... */
    DD_MAX_30102_ADC_RANGE_4096,     /**< @details ... */
    DD_MAX_30102_ADC_RANGE_8192,     /**< @details ... */
    DD_MAX_30102_ADC_RANGE_16384,
    DD_MAX_30102_ADC_RANGE_SIZE
} DD_MAX_30102_ADC_RANGE;

typedef enum DD_MAX_30102_SAMPLE_RATE_TAG
{
    DD_MAX_30102_SAMPLE_RATE_50 = 0U, /**< @details ... */
    DD_MAX_30102_SAMPLE_RATE_100,    /**< @details ... */
    DD_MAX_30102_SAMPLE_RATE_200,    /**< @details ... */
    DD_MAX_30102_SAMPLE_RATE_400,    /**< @details ... */
    DD_MAX_30102_SAMPLE_RATE_800,    /**< @details ... */
    DD_MAX_30102_SAMPLE_RATE_1000,   /**< @details ... */
    DD_MAX_30102_SAMPLE_RATE_1600,   /**< @details ... */
    DD_MAX_30102_SAMPLE_RATE_3200,   /**< @details ... */
    DD_MAX_30102_SAMPLE_RATE_SIZE
} DD_MAX_30102_SAMPLE_RATE;

typedef enum DD_MAX_30102_PULSE_WIDTH_TAG
{
    DD_MAX_30102_PULSE_WIDTH_69 = 0, /**< @details ... */
    DD_MAX_30102_PULSE_WIDTH_118,    /**< @details ... */
    DD_MAX_30102_PULSE_WIDTH_215,    /**< @details ... */
    DD_MAX_30102_PULSE_WIDTH_411,    /**< @details ... */
    DD_MAX_30102_PULSE_WIDTH_SIZE
} DD_MAX_30102_PULSE_WIDTH;

typedef enum DD_MAX_30102_SAMPLE_AVG_TAG
{
    DD_MAX_30102_SAMPLE_AVG_NONE = 0U, /**< @details ... */
    DD_MAX_30102_SAMPLE_AVG_2,        /**< @details ... */
    DD_MAX_30102_SAMPLE_AVG_4,        /**< @details ... */
    DD_MAX_30102_SAMPLE_AVG_8,        /**< @details ... */
    DD_MAX_30102_SAMPLE_AVG_16,       /**< @details ... */
    DD_MAX_30102_SAMPLE_AVG_32,       /**< @details ... */
    DD_MAX_30102_SAMPLE_AVG_SIZE
} DD_MAX_30102_SAMPLE_AVG;

typedef enum DD_MAX_30102_TEMP_COMP_TAG
{
    DD_MAX_30102_TEMP_COMP_INT = 0U, /**< @details ... */
    DD_MAX_30102_TEMP_COMP_FRAC,     /**< @details ... */
    DD_MAX_30102_TEMP_COMP_SIZE
} DD_MAX_30102_TEMP_COMP;

typedef enum DD_MAX_30102_INT_TYPE_TAG
{
    DD_MAX_30102_INT_TYPE_A_FULL = 0U,  /**< @details ... */
    DD_MAX_30102_INT_TYPE_PRG_RDY,      /**< @details ... */
    DD_MAX_30102_INT_TYPE_ALC_OVF,      /**< @details ... */
    DD_MAX_30102_INT_TYPE_PROX_INT,     /**< @details ... */
    DD_MAX_30102_INT_TYPE_PWR_RDY,      /**< @details ... */
    DD_MAX_30102_INT_TYPE_DIE_TEMP_RDY, /**< @details ... */
    DD_MAX_30102_INT_TYPE_SIZE
} DD_MAX_30102_INT_TYPE;

/*************************************************************/
/*      STRUCTURES                                           */
/*************************************************************/

/**
 * @brief   MAX-30102 Output Interface Data Structure
 * @details MAX-30102 Output Interface Data Structure ...
 * @ingroup DriverStructures
 */
typedef struct DD_MAX_30102_DATA_TYPE_TAG
{
    BOOLEAN           int_status_vb[DD_MAX_30102_INT_TYPE_SIZE];
    U8                part_id_u8;
    U8                rev_id_u8;
    U8                read_ptr_u8;
    U8                write_ptr_u8;
    U8                active_leds_u8;
    U32               red_data_raw_u32;
    U32               ir_data_raw_u32;
    U8                temperature_raw_vu8[DD_MAX_30102_TEMP_COMP_SIZE];
    F32               temperature_f32;
    DD_MAX_30102_MODE mode_e;
} DD_MAX_30102_DATA_TYPE;

/*************************************************************/
/*                           DD_SD                           */
/*************************************************************/
/**
 * @details enumerator of ...
 */
typedef enum DD_SD_FILE_MODE_TAG
{
    DD_SD_FILE_MODE_WRITE,
    DD_SD_FILE_MODE_WRITE_BINARY,
    DD_SD_FILE_MODE_SIZE
} DD_SD_FILE_MODE;

/*************************************************************/
/*      STRUCTURES                                           */
/*************************************************************/

/**
 * @brief  File Structure
 * @details
 * @ingroup DriverStructures
 */
typedef struct DD_SD_FILE_TYPE_TAG
{
    FILE*       p_handle;                                 /**< @details FILE pointer */
    struct stat attrib_s;                                 /**< @details POSIX structure containing file attributes */
    char        full_path_vc[DD_SD_MAX_FILE_PATH_LENGTH]; /**< @details Complete file path (incl. mount point) */
    BOOLEAN     is_open_b;                                /**< @details Flag indicating that file is currently open */
} DD_SD_FILE_TYPE;

/**
 * @brief  SD-Card Output Interface Data Structure
 * @details
 * @ingroup DriverStructures
 */
typedef struct DD_SD_DATA_TYPE_TAG
{
    DD_SD_FILE_TYPE file_s;
    BOOLEAN         is_fs_mounted_b; /**< @details Flag indicating that file system is mounted on SD card */
} DD_SD_DATA_TYPE;

/*************************************************************/
/*                        DD_ADC                         */
/*************************************************************/


/*************************************************************/
/*      STRUCTURES                                           */
/*************************************************************/

/**
 * @brief   ADC Output Interface Data Structure
 * @details ADC Output Interface Data Structure ...
 * @ingroup DriverStructures
 */
typedef struct DD_ADC_DATA_TYPE_TAG
{
    U16 raw_sample_u16;          /**< @details Current raw 12-bit ADC sample @unit [LSB] */
    U16 previous_raw_sample_u16; /**< @details Previous raw 12-bit ADC sample @unit [LSB] */
    U16 voltage_u16;             /**< @details Actual voltage measured at the ADC input pin @unit [mV] */
    F32 raw_level_f32;           /**< @details ADC level where Full-Scale (4095) is mapped to 1.0 and 0 is mapped to 0.0 */
    F32 filtered_level_f32;      /**< @details Filtered ADC level where Full-Scale (4095) is mapped to 1.0 and 0 is mapped to 0.0 */
} DD_ADC_DATA_TYPE;


/*************************************************************/
/*                        DD_MCPWM                           */
/*************************************************************/

/**
 * @details enumerator of ...
 */
typedef enum DD_MCPWM_MODE_TAG
{
    DD_MCPWM_MODE_OFF,  /**< @details Default mode: Output is constant low/GND (0.0V) */
    DD_MCPWM_MODE_ON, /**< @details Output is constant high (3.3V) */
    DD_MCPWM_MODE_PWM,  /**< @details Output is running an PWM signal with an duty cycle of duty_cycle_f32 */
    DD_MCPWM_MODE_SIZE
} DD_MCPWM_MODE;

/**
 * @details enumerator of ...
 */
typedef enum DD_MCPWM_CHANNEL_TAG
{
#ifndef JTAG_DEBUG_SUPPORT_ENABLE
    DD_MCPWM_CHANNEL_1,     /*!< Shared with JTAG pin: TMS (14) */
#endif
    DD_MCPWM_CHANNEL_2,
    DD_MCPWM_CHANNEL_3,
    DD_MCPWM_CHANNEL_4,
    DD_MCPWM_CHANNEL_5,
    DD_MCPWM_CHANNEL_6,
#ifndef JTAG_DEBUG_SUPPORT_ENABLE
    DD_MCPWM_CHANNEL_7,     /*!< Shared with JTAG pin: TCK (13) */
    DD_MCPWM_CHANNEL_8,     /*!< Shared with JTAG pin: TDI (12) */
    DD_MCPWM_CHANNEL_9,     /*!< Shared with JTAG pin: TDO (15) */
#endif
    DD_MCPWM_CHANNEL_10,
    DD_MCPWM_CHANNEL_11,
    DD_MCPWM_CHANNEL_12,
    DD_MCPWM_CHANNEL_SIZE
} DD_MCPWM_CHANNEL;

/*************************************************************/
/*      STRUCTURES                                           */
/*************************************************************/

typedef struct DD_MCPWM_CHANNEL_TYPE_TAG
{
    /* Hardware dependent (ESP32) configuration */
    const mcpwm_unit_t       unit_e;         /**< @details MCPWM unit(0-1) */
    const mcpwm_io_signals_t io_signal_e;    /**< @details MCPWM signals, each MCPWM unit has 6 output(MCPWMXA, MCPWMXB) and 9 input(SYNC_X, FAULT_X, CAP_X) 'X' is timer_num(0-2) */
    const gpio_num_t         io_pin_e;       /**< @details Corresponding I/O pin to be multiplexed to output the PWM signal */
    const mcpwm_timer_t      timer_e;        /**< @details Timer number(0-2) of MCPWM, each MCPWM unit has 3 timers */
    const mcpwm_operator_t   operator_e;     /**< @details The operator(MCPWMXA/MCPWMXB), 'X' is timer number selected */

    DD_MCPWM_MODE            mode_e;         /**< @details Channel mode */
    F32                      duty_cycle_f32; /**< @details Channel duty cycle @unit [%] */
} DD_MCPWM_CHANNEL_TYPE;

/**
 * @brief   MCPWM Output Interface Data Structure
 * @details MCPWM Output Interface Data Structure ...
 * @ingroup DriverStructures
 */
typedef struct DD_MCPWM_DATA_TYPE_TAG
{
    DD_MCPWM_CHANNEL_TYPE* p_channel_s; /**< @details Pointer to array containing all PWM channels */
} DD_MCPWM_DATA_TYPE;


/*************************************************************/
/*                        DD_INA_219                         */
/*************************************************************/

/**
 * @details Full-scale current sense (input) voltage range
 */
typedef enum DD_INA_219_SHUNT_VOL_RANGE_TAG
{
    DD_INA_219_SHUNT_VOL_RANGE_40MV = 0U, /**< @details FS range: +/- 40 mV  @ PGA = / 1 */
    DD_INA_219_SHUNT_VOL_RANGE_80MV,      /**< @details FS range: +/- 80 mV  @ PGA = / 2 */
    DD_INA_219_SHUNT_VOL_RANGE_160MV,     /**< @details FS range: +/- 160 mV @ PGA = / 4 */
    DD_INA_219_SHUNT_VOL_RANGE_320MV,     /**< @details FS range: +/- 320 mV @ PGA = / 8 */
    DD_INA_219_SHUNT_VOL_RANGE_SIZE
} DD_INA_219_SHUNT_VOL_RANGE;

/**
 * @details Bus voltage (input voltage) range
 */
typedef enum DD_INA_219_BUS_VOL_RANGE_TAG
{
    DD_INA_219_BUS_VOL_RANGE_16V = 0U, /**< @details 16V Bus voltage range */
    DD_INA_219_BUS_VOL_RANGE_32V,      /**< @details 32V Bus voltage range */
    DD_INA_219_BUS_VOL_RANGE_SIZE
} DD_INA_219_BUS_VOL_RANGE;

/**
 * @details  BADC Bus ADC Resolution/Averaging
 */
typedef enum DD_INA_219_BADC_RES_AVE_TAG
{
    DD_INA_219_BADC_RES_AVE_12BIT_1S_532US = 0U, /**< 12-bit shunt res = 0..4097, default */
    DD_INA_219_BADC_RES_AVE_12BIT_2S_1060US,     /**< 2 x 12-bit shunt samples averaged together */
    DD_INA_219_BADC_RES_AVE_12BIT_4S_2130US,     /**< 4 x 12-bit shunt samples averaged together */
    DD_INA_219_BADC_RES_AVE_12BIT_8S_4260US,     /**< 8 x 12-bit shunt samples averaged together */
    DD_INA_219_BADC_RES_AVE_12BIT_16S_8512US,    /**< 16 x 12-bit shunt samples averaged together */
    DD_INA_219_BADC_RES_AVE_12BIT_32S_17MS,      /**< 32 x 12-bit shunt samples averaged together */
    DD_INA_219_BADC_RES_AVE_12BIT_64S_34MS,      /**< 64 x 12-bit shunt samples averaged together */
    DD_INA_219_BADC_RES_AVE_12BIT_128S_69MS,     /**< 128 x 12-bit shunt samples averaged together */
    DD_INA_219_BADC_RES_AVE_SIZE
} DD_INA_219_BADC_RES_AVE;

/**
 * @details  SADC Shunt ADC Resolution/Averaging
 */
typedef enum DD_INA_219_SADC_RES_AVE_TAG
{
    DD_INA_219_SADC_RES_AVE_12BIT_1S_532US = 0U, /**< 12-bit shunt res = 0..4097, default */
    DD_INA_219_SADC_RES_AVE_12BIT_2S_1060US,     /**< 2 x 12-bit shunt samples averaged together */
    DD_INA_219_SADC_RES_AVE_12BIT_4S_2130US,     /**< 4 x 12-bit shunt samples averaged together */
    DD_INA_219_SADC_RES_AVE_12BIT_8S_4260US,     /**< 8 x 12-bit shunt samples averaged together */
    DD_INA_219_SADC_RES_AVE_12BIT_16S_8512US,    /**< 16 x 12-bit shunt samples averaged together */
    DD_INA_219_SADC_RES_AVE_12BIT_32S_17MS,      /**< 32 x 12-bit shunt samples averaged together */
    DD_INA_219_SADC_RES_AVE_12BIT_64S_34MS,      /**< 64 x 12-bit shunt samples averaged together */
    DD_INA_219_SADC_RES_AVE_12BIT_128S_69MS,     /**< 128 x 12-bit shunt samples averaged together */
    DD_INA_219_SADC_RES_AVE_SIZE
} DD_INA_219_SADC_RES_AVE;

/**
 * @brief   INA-219 Bus Voltage data and status information
 * @ingroup DriverStructures
 */
typedef struct DD_INA_219_BUS_VOL_DATA_TYPE_TAG
{
    BOOLEAN math_overflow_b;     /**< @details The Math Overflow Flag (OVF) is set when the Power or Current calculations
                                          are out of range. It indicates that current and power data may be meaningless. */
    BOOLEAN conv_ready_b;        /**< @details Although the data from the last conversion can be read at any time, the INA219
                                               Conversion Ready bit (CNVR) indicates when data from a conversion is available
                                               in the data output registers. The CNVR bit is set after all conversions, averaging,
                                               and multiplications are complete. */
    U16     voltage_raw_u16;    /**< @details Raw most recent bus voltage ADC reading */
} DD_INA_219_BUS_VOL_DATA_TYPE;

/**
 * @brief   INA-219 Output Interface Data Structure
 * @details INA-219 Output Interface Data Structure ...
 * @ingroup DriverStructures
 */
typedef struct DD_INA_219_DATA_TYPE_TAG
{
    DD_INA_219_SHUNT_VOL_RANGE   shunt_voltage_range_e;            /**< @details Current full-scale current sense (input) voltage range */
    DD_INA_219_SADC_RES_AVE      shunt_adc_resolution_averaging_e; /**< @details Shunt voltage ADC resolution/averaging setting */
    DD_INA_219_BUS_VOL_RANGE     bus_voltage_range_e;              /**< @details Current bus voltage (input voltage) range */
    DD_INA_219_BADC_RES_AVE      bus_adc_resolution_averaging_e;   /**< @details Bus voltage ADC resolution/averaging setting */
    DD_INA_219_BUS_VOL_DATA_TYPE bus_voltage_data_s;               /**< @details Raw most recent bus voltage ADC reading and status information */
    S16                          shunt_voltage_raw_s16;            /**< @details Raw ADC reading representing the voltage across the shunt resistor */
    U16                          power_raw_u16;                    /**< @details Records power in watts by multiplying the value of the current with the
                                                                      value of the bus voltage according to the Equation 5 (datasheet pg. 13) */
    S16                          current_raw_s16;                  /**< @details Value is calculated by multiplying the value in the Shunt Voltage register
                                                                      with the value in the Calibration register according to the Equation 4 (datasheet pg. 12) */
    F32                          shunt_voltage_mV_f32;             /**< @details Voltage across the shunt resistor @unit mV */
    F32                          bus_voltage_V_f32;                /**< @details Bus voltage @unit V */
    F32                          bus_voltage_mV_f32;               /**< @details Bus voltage @unit mV */
    F32                          power_mW_f32;                     /**< @details Power @unit mW */
    F32                          current_mA_f32;                   /**< @details Current @unit mA */
    F32                          current_lsb_A_f32;                /**< @details Current LSB after calibration @unit Amp/LSB */
    F32                          power_lsb_W_f32;                  /**< @details Power LSB after calibration @unit Watt/LSB */
    U16                          calibration_register_u16;         /**< @details Contains the calibration register value @unit 1/V */
} DD_INA_219_DATA_TYPE;

/*************************************************************/
/*                        DD_TMP_102                         */
/*************************************************************/
/**
 * @brief   TMP-102 Output Interface Data Structure
 * @details TMP-102 Output Interface Data Structure ...
 * @ingroup DriverStructures
 */
typedef struct DD_TMP_102_DATA_TYPE_TAG
{
    S16 temperature_raw_s16; /**< @details Raw ADC temperature reading @unit LSB */
    F32 temperature_deg_f32; /**< @details Current temperature reading @unit °C */
} DD_TMP_102_DATA_TYPE;

#endif /* DD_PUBLIC_TYPES_H */
