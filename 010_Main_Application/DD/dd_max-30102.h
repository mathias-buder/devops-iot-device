/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        @file dd_max-30102.h
        @details MAX-30102 device driver implementation

        (c) SEWELA 2020

*********************************************************************/
#ifndef DD_MAX_30102_H_
#define DD_MAX_30102_H_

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include "../../types.h"


/*************************************************************/
/*      COMPILE TIME CONFIGURATION                           */
/*************************************************************/
#define DD_MAX_30105_LOG_MSG_TAG           "DD_MAX_30102"
#define DD_MAX_30105_DEVICE_ID              0x15 /* Unique device id */
#define DD_MAX_30105_TEMP_TIME_OUT_CNT      3U
#define DD_MAX_30105_TEMP_DELAY_TICKS       10U



/*************************************************************/
/*      GLOBAL DEFINES                                       */
/*************************************************************/

/* Status Registers */
#define DD_MAX_30102_INT_STAT_1                 0x00
#define DD_MAX_30102_INT_STAT_2                 0x01
#define DD_MAX_30102_INT_ENABLE_1               0x02
#define DD_MAX_30102_INT_ENABLE_2               0x03

/* FIFO Registers */
#define DD_MAX_30102_FIFO_WRITE_PTR             0x04
#define DD_MAX_30102_FIFO_OVF_COUNTER           0x05
#define DD_MAX_30102_FIFO_READ_PTR              0x06
#define DD_MAX_30102_FIFO_DATA                  0x07

/* Configuration Registers */
#define DD_MAX_30102_FIFO_CONFIG                0x08
#define DD_MAX_30102_MODE_CONFIG                0x09
#define DD_MAX_30102_SPO2_CONFIG                0x0A
#define DD_MAX_30102_LED1_PULSE_AMP             0x0C
#define DD_MAX_30102_LED2_PULSE_AMP             0x0D
#define DD_MAX_30102_LED_PROX_AMP               0x10
#define DD_MAX_30102_MULTI_LED_CONFIG_1         0x11
#define DD_MAX_30102_MULTI_LED_CONFIG_2         0x12

/* Die Temperature Registers */
#define DD_MAX_30102_DIE_TEMP_INT               0x1F
#define DD_MAX_30102_DIE_TEMP_FRAC              0x20
#define DD_MAX_30102_DIE_TEMP_CONFIG            0x21
#define DD_MAX_30102_DIE_TEMP_FRAC_RES          0.0625F

/* Proximity Function Registers */
#define DD_MAX_30102_PROX_INT_THRESH            0x30

/* Part ID Registers */
#define DD_MAX_30102_REVISION_ID                0xFE
#define DD_MAX_30102_PART_ID                    0xFF    // Should always be 0x15. Identical to MAX30102.

/* Interrupt configuration for Interrupt Status 1
 * and Interrupt Status 2*/

/* A_FULL: FIFO Almost Full Flag */
#define DD_MAX_30102_INT_A_FULL_MASK            0x7F /* (byte)~0b10000000 */
#define DD_MAX_30102_INT_A_FULL_ENABLE          0x80
#define DD_MAX_30102_INT_A_FULL_DISABLE         0x00

/* PPG_RDY: New FIFO Data Ready */
#define DD_MAX_30102_INT_PPG_RDY_MASK           0xBF /* (byte)~0b01000000 */
#define DD_MAX_30102_INT_PPG_RDY_ENABLE         0x40
#define DD_MAX_30102_INT_PPG_RDY_DISABLE        0x00

/* ALC_OVF: Ambient Light Cancellation Overflow */
#define DD_MAX_30102_INT_ALC_OVF_MASK           0xDF /*(byte)~0b00100000 */
#define DD_MAX_30102_INT_ALC_OVF_ENABLE         0x20
#define DD_MAX_30102_INT_ALC_OVF_DISABLE        0x00

/* PROX_INT: Proximity Threshold Triggered */
#define DD_MAX_30102_INT_PROX_INT_MASK          0xEF /* (byte)~0b00010000 */
#define DD_MAX_30102_INT_PROX_INT_ENABLE        0x10
#define DD_MAX_30102_INT_PROX_INT_DISABLE       0x00

/* PWR_RDY: Power Ready Flag */
#define DD_MAX_30102_INT_PWR_RDY_MASK           0xFE /* (byte)~0b00000001 */
#define DD_MAX_30102_INT_PWR_RDY_ENABLE         0x01
#define DD_MAX_30102_INT_PWR_RDY_DISABLE        0x00

/* DIE_TEMP_RDY: Internal Temperature Ready Flag */
#define DD_MAX_30102_INT_DIE_TEMP_RDY_MASK      0xFD /* (byte)~0b00000010 */
#define DD_MAX_30102_INT_DIE_TEMP_RDY_ENABLE    0x02
#define DD_MAX_30102_INT_DIE_TEMP_RDY_DISABLE   0x00

/* Sample Averaging (SMP_AVE) */
#define DD_MAX_30102_SMP_AVG_MASK               0x1F /* (byte)~0b11100000 */
#define DD_MAX_30102_SMP_AVG_NONE               0x00
#define DD_MAX_30102_SMP_AVG_2                  0x20
#define DD_MAX_30102_SMP_AVG_4                  0x40
#define DD_MAX_30102_SMP_AVG_8                  0x60
#define DD_MAX_30102_SMP_AVG_16                 0x80
#define DD_MAX_30102_SMP_AVG_32                 0xA0

/* FIFO Rolls on Full (FIFO_ROLLOVER_EN) */
#define DD_MAX_30102_ROLL_OVER_MASK             0xEF /* (byte)~0b00010000 */
#define DD_MAX_30102_ROLL_OVER_ENABLE           0x10
#define DD_MAX_30102_ROLL_OVER_DISABLE          0x00

/* FIFO Almost Full Value (FIFO_A_FULL) */
#define DD_MAX_30102_A_FULL_MASK                0xF0 /* (byte)~0b00001111 */
#define DD_MAX_30102_A_FULL_MAX_VAL             0x0F /* See datasheet, pg. 17 */

/* Mode configuration commands */

/* Shutdown Control (SHDN) */
#define DD_MAX_30102_SHUTDOWN_MASK              0x7F
#define DD_MAX_30102_SHUTDOWN                   0x80
#define DD_MAX_30102_WAKEUP                     0x00

/* Reset Control (RESET) */
#define DD_MAX_30102_RESET_MASK                 0xBF
#define DD_MAX_30102_RESET                      0x40

/* Mode Control */
#define DD_MAX_30102_LED_MODE_MASK              0xF8
#define DD_MAX_30102_LED_MODE_HR                0x02
#define DD_MAX_30102_LED_MODE_SPO2              0x03
#define DD_MAX_30102_LED_MODE_MULTI             0x07

/* SpO2 Configuration */

/* SpO2 ADC Range Control */
#define DD_MAX_30102_SPO2_ADC_RANGE_MASK        0x9F
#define DD_MAX_30102_SPO2_ADC_RANGE_2048        0x00
#define DD_MAX_30102_SPO2_ADC_RANGE_4096        0x20
#define DD_MAX_30102_SPO2_ADC_RANGE_8192        0x40
#define DD_MAX_30102_SPO2_ADC_RANGE_16384       0x60

/* SpO2 Sample Rate Control */
#define DD_MAX_30102_SPO2_SMP_RATE_MASK         0xE3
#define DD_MAX_30102_SPO2_SMP_RATE_50           0x00
#define DD_MAX_30102_SPO2_SMP_RATE_100          0x04
#define DD_MAX_30102_SPO2_SMP_RATE_200          0x08
#define DD_MAX_30102_SPO2_SMP_RATE_400          0x0C
#define DD_MAX_30102_SPO2_SMP_RATE_800          0x10
#define DD_MAX_30102_SPO2_SMP_RATE_1000         0x14
#define DD_MAX_30102_SPO2_SMP_RATE_1600         0x18
#define DD_MAX_30102_SPO2_SMP_RATE_3200         0x1C

/* LED Pulse Width Control and ADC Resolution */
#define DD_MAX_30102_LED_PULSE_WIDTH_MASK       0xFC /* (byte)~0b00000011 */
#define DD_MAX_30102_LED_PULSE_WIDTH_69         0x00
#define DD_MAX_30102_LED_PULSE_WIDTH_118        0x01
#define DD_MAX_30102_LED_PULSE_WIDTH_215        0x02
#define DD_MAX_30102_LED_PULSE_WIDTH_411        0x03

/* Multi-LED Mode Control */
#define DD_MAX_30102_SLOT_1_MASK                0xF8
#define DD_MAX_30102_SLOT_2_MASK                0x8F
#define DD_MAX_30102_SLOT_3_MASK                0xF8
#define DD_MAX_30102_SLOT_4_MASK                0x8F

#define DD_MAX_30102_SLOT_NONE                  0x00
#define DD_MAX_30102_SLOT_RED_LED               0x01
#define DD_MAX_30102_SLOT_IR_LED                0x02
#define DD_MAX_30102_SLOT_RED_PILOT             0x05
#define DD_MAX_30102_SLOT_IR_PILOT              0x06

/* FIFO properties */
#define DD_MAX_30102_FIFO_SIZE                  32U /* FIFO size in bytes */
#define DD_MAX_30102_FIFO_CHANNEL_SIZE          3U  /* FIFO channel size in bytes */

/*************************************************************
*                      ENUMERATORS                           *
*************************************************************/

/**
 * @details MAX-30102 pointer type
 */
typedef enum DD_MAX_30102_PTR_TAG
{
    DD_MAX_30102_PTR_TYPE_READ = 0U, /**< @details Read pointer */
    DD_MAX_30102_PTR_TYPE_WRITE,     /**< @details Write pointer */
    DD_MAX_30102_PTR_TYPE_SIZE
} DD_MAX_30102_PTR;

/**
 * @details MAX-30102 LED
 */
typedef enum DD_MAX_30102_LED_TAG
{
    DD_MAX_30102_LED_TYPE_RED = 0U, /**< @details Red LED */
    DD_MAX_30102_LED_TYPE_IR,       /**< @details IR LED */
    DD_MAX_30102_LED_TYPE_PROX,     /**< @details Proximity LED */
    DD_MAX_30102_LED_TYPE_SIZE
} DD_MAX_30102_LED;

/**
 * @details MAX-30102 SLOT number
 */
typedef enum DD_MAX_30102_SLOT_TAG
{
    DD_MAX_30102_SLOT_1 = 0U, /**< @details Slot 1*/
    DD_MAX_30102_SLOT_2,      /**< @details Slot 2 */
    DD_MAX_30102_SLOT_3,      /**< @details Slot 3 */
    DD_MAX_30102_SLOT_4,      /**< @details Slot 4 */
    DD_MAX_30102_SLOT_SIZE
} DD_MAX_30102_SLOT;

/**
 * @details MAX-30102 SLOT mode
 */
typedef enum DD_MAX_30102_SLOT_MODE_TAG
{
    DD_MAX_30102_SLOT_MODE_NONE = 0U,     /**< @details No slot mode active */
    DD_MAX_30102_SLOT_MODE_LED_RED,       /**< @details Red Led */
    DD_MAX_30102_SLOT_MODE_LED_IR,        /**< @details Red Ir */
    DD_MAX_30102_SLOT_MODE_LED_RED_PILOT, /**< @details Red/Pilot */
    DD_MAX_30102_SLOT_MODE_LED_IR_PILOT,  /**< @details Ir/Pilot */
    DD_MAX_30102_SLOT_MODE_SIZE
} DD_MAX_30102_SLOT_MODE;

/**
 * @details MAX-30102 operation mode
 */
typedef enum DD_MAX_30102_MODE_TAG
{
    DD_MAX_30102_MODE_SLEEP = 0U, /**< @details Sleep mode */
    DD_MAX_30102_MODE_HR,         /**< @details Hear-Rate mode */
    DD_MAX_30102_MODE_SPO2,       /**< @details SpO2 mode */
    DD_MAX_30102_MODE_MULTI_LED   /**< @details Multi-LED mode */
} DD_MAX_30102_MODE;

/**
 * @details MAX-30102 ADC range
 */
typedef enum DD_MAX_30102_ADC_RANGE_TAG
{
    DD_MAX_30102_ADC_RANGE_2048 = 0U, /**< @details ADC range of 2048 */
    DD_MAX_30102_ADC_RANGE_4096,      /**< @details ADC range of 4096 */
    DD_MAX_30102_ADC_RANGE_8192,      /**< @details ADC range of 8192 */
    DD_MAX_30102_ADC_RANGE_16384,     /**< @details ADC range of 16384 */
    DD_MAX_30102_ADC_RANGE_SIZE
} DD_MAX_30102_ADC_RANGE;

/**
 * @details MAX-30102 ADC sample rate
 */
typedef enum DD_MAX_30102_SAMPLE_RATE_TAG
{
    DD_MAX_30102_SAMPLE_RATE_50 = 0U, /**< @details ADC sample rate of 50 @unit us */
    DD_MAX_30102_SAMPLE_RATE_100,     /**< @details ADC sample rate of 100 @unit us */
    DD_MAX_30102_SAMPLE_RATE_200,     /**< @details ADC sample rate of 200 @unit us */
    DD_MAX_30102_SAMPLE_RATE_400,     /**< @details ADC sample rate of 400 @unit us */
    DD_MAX_30102_SAMPLE_RATE_800,     /**< @details ADC sample rate of 800 @unit us */
    DD_MAX_30102_SAMPLE_RATE_1000,    /**< @details ADC sample rate of 1.0 @unit ms */
    DD_MAX_30102_SAMPLE_RATE_1600,    /**< @details ADC sample rate of 1.6 @unit ms */
    DD_MAX_30102_SAMPLE_RATE_3200,    /**< @details ADC sample rate of 3.2 @unit ms */
    DD_MAX_30102_SAMPLE_RATE_SIZE
} DD_MAX_30102_SAMPLE_RATE;

/**
 * @details MAX-30102 LED PWM pulse width
 */
typedef enum DD_MAX_30102_PULSE_WIDTH_TAG
{
    DD_MAX_30102_PULSE_WIDTH_69 = 0, /**< @details LED PWM pulse width of 69 @unit ms */
    DD_MAX_30102_PULSE_WIDTH_118,    /**< @details LED PWM pulse width of 118 @unit ms */
    DD_MAX_30102_PULSE_WIDTH_215,    /**< @details LED PWM pulse width of 215 @unit ms */
    DD_MAX_30102_PULSE_WIDTH_411,    /**< @details LED PWM pulse width of 144 @unit ms */
    DD_MAX_30102_PULSE_WIDTH_SIZE
} DD_MAX_30102_PULSE_WIDTH;

/**
 * @details MAX-30102 sample averaging
 */
typedef enum DD_MAX_30102_SAMPLE_AVG_TAG
{
    DD_MAX_30102_SAMPLE_AVG_NONE = 0U, /**< @details No sample averaging */
    DD_MAX_30102_SAMPLE_AVG_2,         /**< @details Sample averaging over 2 samples */
    DD_MAX_30102_SAMPLE_AVG_4,         /**< @details Sample averaging over 4 samples */
    DD_MAX_30102_SAMPLE_AVG_8,         /**< @details Sample averaging over 8 samples */
    DD_MAX_30102_SAMPLE_AVG_16,        /**< @details Sample averaging over 16 samples */
    DD_MAX_30102_SAMPLE_AVG_32,        /**< @details Sample averaging over 32 samples */
    DD_MAX_30102_SAMPLE_AVG_SIZE
} DD_MAX_30102_SAMPLE_AVG;

/**
 * @details MAX-30102 temperature components
 */
typedef enum DD_MAX_30102_TEMP_COMP_TAG
{
    DD_MAX_30102_TEMP_COMP_INT = 0U, /**< @details Integer part */
    DD_MAX_30102_TEMP_COMP_FRAC,     /**< @details Fractional part */
    DD_MAX_30102_TEMP_COMP_SIZE
} DD_MAX_30102_TEMP_COMP;

/**
 * @details MAX-30102 interrupt status
 */
typedef enum DD_MAX_30102_INT_TYPE_TAG
{
    DD_MAX_30102_INT_TYPE_A_FULL = 0U,  /**< @details FIFO almost full flag */
    DD_MAX_30102_INT_TYPE_PRG_RDY,      /**< @details New FIFO Data Ready */
    DD_MAX_30102_INT_TYPE_ALC_OVF,      /**< @details Ambient Light Cancellation Overflow */
    DD_MAX_30102_INT_TYPE_PROX_INT,     /**< @details Proximity Threshold Triggered */
    DD_MAX_30102_INT_TYPE_PWR_RDY,      /**< @details Power Ready Flag */
    DD_MAX_30102_INT_TYPE_DIE_TEMP_RDY, /**< @details Internal Temperature Ready Flag */
    DD_MAX_30102_INT_TYPE_SIZE
} DD_MAX_30102_INT_TYPE;

/*************************************************************/
/*      STRUCTURES                                           */
/*************************************************************/

/**
 * @brief   MAX-30102 configuration data structure
 * @details Contains all parameters required to configure
 *          the MAX-30102
 * @ingroup DriverStructures
 */
typedef struct DD_MAX_30102_CONFIG_TYPE_TAG
{
    DD_MAX_30102_MODE        mode_cfg_e;                    /**< @details Operation mode */
    DD_MAX_30102_ADC_RANGE   adc_range_cfg_e;               /**< @details ADC range */
    DD_MAX_30102_SAMPLE_RATE sample_rate_cfg_e;             /**< @details ADC sample rate */
    DD_MAX_30102_SAMPLE_AVG  sample_avg_cfg_e;              /**< @details ADC sample averaging */
    DD_MAX_30102_PULSE_WIDTH pulse_width_cfg_e;             /**< @details LED PWM pulse width */
    U8                       led_amplitude_cfg_u8;          /**< @details LED amplitude */
    U8                       prox_threshold_cfg_u8;         /**< @details Proximity threshold to trigger interrupt */
    BOOLEAN                  fifo_roll_over_cfg_b;          /**< @details Flag to indicate whether to fill FIFO with new data if its already full*/
    BOOLEAN                  fifo_a_full_int_enable_cfg_b;  /**< @details Flag to indicate whether rise the "FIFO almost full" interrupt */
    U8                       fifo_a_full_value_cfg_u8;      /**< @details Sets the threshold at which the "FIFO almost full" interrupt is set */
    BOOLEAN                  die_temp_rdy_int_enable_cfg_b; /**< @details Flag to indicate whether to rise the "Internal Temperature Ready" interrupt or not */
    BOOLEAN                  prox_int_enable_cfg_b;         /**< @details Flag to indicate whether to rise the "Proximity Threshold" interrupt or not */
} DD_MAX_30102_CONFIG_TYPE;

/**
 * @brief   MAX-30102 Output Interface Data Structure
 * @details Contains all data provided by the MAX-30102
 * @ingroup DriverStructures
 */
typedef struct DD_MAX_30102_DATA_OUT_TYPE_TAG
{
    BOOLEAN           int_status_vb[DD_MAX_30102_INT_TYPE_SIZE];        /**< @details Current interrupt status */
    U8                part_id_u8;                                       /**< @details Part id */
    U8                rev_id_u8;                                        /**< @details Revision id */
    U8                read_ptr_u8;                                      /**< @details Current read pointer value */
    U8                write_ptr_u8;                                     /**< @details Current write pointer value */
    U32               red_data_raw_u32;                                 /**< @details RED LED ADC data @unit LSB */
    U32               ir_data_raw_u32;                                  /**< @details IR LED ADC data @unit LSB */
    U8                temperature_raw_vu8[DD_MAX_30102_TEMP_COMP_SIZE]; /**< @details Raw temperature readings */
    F32               temperature_f32;                                  /**< @details Temperature @unit °C */
    DD_MAX_30102_MODE mode_e;                                           /**< @details Device mode */
} DD_MAX_30102_DATA_OUT_TYPE;

/*************************************************************/
/*      CLASS DEFINITION                                     */
/*************************************************************/

class DD_MAX_30102_C {

  private:
    DD_MAX_30102_DATA_OUT_TYPE data_out_s;
    U8                         i2c_addr_u8;

   /**
   * @details This function reads the interrupt 1 register
   * @param[in] r_register_u8 Reference to variable that stores the interrupt 1
   *            register content
   * @return TRUE in case no error was reported during I2C transaction TRUE in case no error was reported during
   */
    BOOLEAN get_int1( U8& r_register_u8 );
    /**
    * @details This function reads the interrupt 2 register
    * @param[in] r_register_u8 Reference to variable that stores the interrupt 2
    *            register content
   * @return TRUE in case no error was reported during I2C transaction TRUE in case no error was reported during
    */
    BOOLEAN get_int2( U8& r_register_u8 );
    /**
    * @details This function reads the interrupt 1+2 register
    * @param[in] r_int_status_vb Reference to array that stores the interrupt 1+2
    *            register content
    * @return TRUE in case no error was reported during I2C transaction TRUE in case no error was reported during
    */
    BOOLEAN get_int_status( BOOLEAN ( &r_int_status_vb )[DD_MAX_30102_INT_TYPE_SIZE] );
    /**
    * @details This function reads the current device temperature
    * @param[in] r_data_s Reference to data structure where the temperature
    *            is stored
    * @return TRUE in case no error was reported during I2C transaction TRUE in case no error was reported during
    */
    BOOLEAN get_temperature( DD_MAX_30102_DATA_OUT_TYPE& r_data_s );
    /**
    * @details This function sets the "FIFO almost full" interrupt enable flag
    * @param[in] enable_b FLag to enable/disable the "FIFO almost full" interrupt
    *            enable flag
    * @return TRUE in case no error was reported during I2C transaction TRUE in case no error was reported during I2C transaction
    */
    BOOLEAN set_fifo_int_a_full( const BOOLEAN enable_b );
    /**
    * @details This function sets the "New FIFO Data Ready" interrupt enable flag
    * @param[in] enable_b FLag to enable/disable the "New FIFO Data Ready" interrupt
    *            enable flag
    * @return TRUE in case no error was reported during I2C transaction TRUE in case no error was reported during
    */
    BOOLEAN set_data_ready( const BOOLEAN enable_b );
    /**
    * @details This function sets the "Ambient Light Cancellation Overflow" interrupt enable flag
    * @param[in] enable_b FLag to enable/disable the "Ambient Light Cancellation Overflow" interrupt
    *            enable flag
    * @return TRUE in case no error was reported during I2C transaction TRUE in case no error was reported during
    */
    BOOLEAN set_alc_ovf( const BOOLEAN enable_b );
    /**
    * @details This function sets the "Proximity Threshold Triggered" interrupt enable flag
    * @param[in] enable_b FLag to enable/disable the "Proximity Threshold Triggered" interrupt
    *            enable flag
    * @return TRUE in case no error was reported during I2C transaction TRUE in case no error was reported during
    */
    BOOLEAN set_prox_int( const BOOLEAN enable_b );
    /**
    * @details This function sets the "Internal Temperature Ready Flag" interrupt enable flag
    * @param[in] enable_b FLag to enable/disable the "Internal Temperature Ready Flag" interrupt
    *            enable flag
    * @return TRUE in case no error was reported during I2C transaction TRUE in case no error was reported during
    */
    BOOLEAN set_die_temp_rdy_int( const BOOLEAN enable_b );
    /**
    * @details This function performs a device reset triggered through software
    * @return TRUE in case no error was reported during I2C transaction TRUE in case no error was reported during
    */
    BOOLEAN soft_reset( void );
    /**
    * @details This function shuts the device down or wakes it up
    * @param[in] enable_b FLag to shuts the device down (FALSE) or wakes it up (TRUE)
    *            enable flag
    * @return TRUE in case no error was reported during I2C transaction TRUE in case no error was reported during
    */
    BOOLEAN set_wake_up( const BOOLEAN enable_b );
    /**
    * @details This function sets the into the corresponding operation mode
    * @param[in] r_data_s Reference to data structure that will store the updated mode
    * @param[out] mode_e Mode to switch the device to
    * @return TRUE in case no error was reported during I2C transaction
    */
    BOOLEAN set_mode( DD_MAX_30102_DATA_OUT_TYPE& r_data_s, const DD_MAX_30102_MODE mode_e );
    /**
    * @details This function sets the corresponding ADC range
    * @param[in] range_e Requested ADC range
    * @return TRUE in case no error was reported during I2C transaction
    */
    BOOLEAN set_adc_range( const DD_MAX_30102_ADC_RANGE range_e );
    /**
    * @details This function sets the corresponding ADC sampling rate
    * @param[in] rate_e Requested ADC sampling rate
    * @return TRUE in case no error was reported during I2C transaction
    */
    BOOLEAN set_sample_rate( const DD_MAX_30102_SAMPLE_RATE rate_e );
    /**
    * @details This function sets the corresponding LED PWM pulse width
    * @param[in] width_e Requested LED PWM pulse width
    * @return TRUE in case no error was reported during I2C transaction
    */
    BOOLEAN set_pulse_width( const DD_MAX_30102_PULSE_WIDTH width_e );
    /**
    * @details This function sets the corresponding LED amplitude
    * @param[in] type_e LED type to be updated
    * @param[in] amplitude_u8 Amplitude value
    * @return TRUE in case no error was reported during I2C transaction
    */
    BOOLEAN set_pulse_amp( const DD_MAX_30102_LED type_e, const U8 amplitude_u8 );
    /**
    * @details This function sets the proximity threshold at which the
    *          corresponding interrupt flag is set
    * @param[in] threshold_u8 Proximity threshold value
    * @return TRUE in case no error was reported during I2C transaction
    */
    BOOLEAN set_proximity_threshold( const U8 threshold_u8 );
    /**
    * @details This function sets slot settings
    * @param[in] slot_e Slot to be updated
    * @param[in] mode_e Mode to be set
    * @return TRUE in case no error was reported during I2C transaction
    */
    BOOLEAN setup_slot( const DD_MAX_30102_SLOT slot_e, const DD_MAX_30102_SLOT_MODE mode_e );
    /**
     * @details This function sets the ADC averaging
     * @param[in] average_e Sample averaging over average_e samples
     * @return TRUE in case no error was reported during I2C transaction
     */
    BOOLEAN set_sample_average( const DD_MAX_30102_SAMPLE_AVG average_e );
    /**
    * @details This function sets the FIFO roll over flag
    * @param[in] enable_b FLag to enable/disable the FIFO roll over function
    * @return TRUE in case no error was reported during I2C transaction
    */
    BOOLEAN set_fifo_roll_over( BOOLEAN enable_b );
    /**
    * @details This function sets the threshold at which the "FIFO almost full"
    *          interrupt is set
    * @param[in] enable_b FLag to enable/disable the FIFO roll over function
    * @return TRUE in case no error was reported during I2C transaction
    */
    BOOLEAN set_fifo_a_full_value( U8 value_u8 );
    /**
    * @details This function clears the FIFO content
    * @return TRUE in case no error was reported during I2C transaction
    */
    BOOLEAN set_fifo_clear( void );
    /**
    * @details This function reads the current reads pointer value
    * @param[in] ptr_type_e Pointer type to be read
    * @param[in] r_value_u8 Reference to variable that stores the pointer value
    * @return TRUE in case no error was reported during I2C transaction
    */
    BOOLEAN get_fifo_ptr_by_type( const DD_MAX_30102_PTR ptr_type_e, U8& r_value_u8 );
    /**
    * @details This function reads the current FIFO overflow count
    * @param[in] r_ovf_cnt_u8 Reference to variable that stores the overflow count
    * @return TRUE in case no error was reported during I2C transaction
    */
    BOOLEAN get_fifo_ovf_cnt( U8& r_ovf_cnt_u8 );
    /**
    * @details This function reads the device id
    * @param[in] r_device_id_u8 Reference to variable that stores the device id
    * @return TRUE in case no error was reported during I2C transaction
    */
    BOOLEAN get_part_id( U8& r_device_id_u8 );
    /**
    * @details This function reads the device revision
    * @param[in] r_revision_u8 Reference to variable that stores the device revision
    * @return TRUE in case no error was reported during I2C transaction
    */
    BOOLEAN get_rev_id( U8& r_revision_u8 );
    /**
    * @details This function reads the current FIFO data
    * @param[in] r_data_s Reference to variable that stores the FIFO data
    * @return TRUE in case no error was reported during I2C transaction
    */
    BOOLEAN get_fifo_data( DD_MAX_30102_DATA_OUT_TYPE& r_data_s );

  public:
    /**
     * @details Default constructor
     * @param[in] i2c_addr_u8 I2C device address
     */
    DD_MAX_30102_C( U8 i2c_addr_u8 );

    /**
     * @details Default destructor
     */
    ~DD_MAX_30102_C();

    /**
     * @details This function initializes the MAX-30102 device
     *          with default configuration
     * @return Pointer to global device data out structure
     */
    DD_MAX_30102_DATA_OUT_TYPE* init( void );

    /**
     * @details This function initializes the MAX-30102 device
     * @param[in] Reference to device configuration structure
     * @return Pointer to global device data out structure
     */
    DD_MAX_30102_DATA_OUT_TYPE* init( DD_MAX_30102_CONFIG_TYPE& r_data_in_s );

    /**
     * @details This is the drivers main function that shall be called
     * cyclicly and will provide all data through DD_MAX_30102_DATA_OUT_TYPE
     */
    void main( void );
};

#endif /* DD_MAX_30102_H_ */
