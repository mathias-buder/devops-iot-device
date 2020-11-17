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
typedef struct DD_MAX_30102_DATA_IN_TYPE_TAG
{
    DD_MAX_30102_MODE        mode_cfg_e;
    DD_MAX_30102_ADC_RANGE   adc_range_cfg_e;
    DD_MAX_30102_SAMPLE_RATE sample_rate_cfg_e;
    DD_MAX_30102_SAMPLE_AVG  sample_avg_cfg_e;
    DD_MAX_30102_PULSE_WIDTH pulse_width_cfg_e;
    U8                       led_amplitude_cfg_u8;
    U8                       temp_delay_ticks_cfg_u8;
    U8                       prox_threshold_cfg_u8;
    BOOLEAN                  fifo_roll_over_cfg_b;
    BOOLEAN                  fifo_a_full_int_enable_cfg_b;
    U8                       fifo_a_full_value_cfg_u8;
    BOOLEAN                  die_temp_rdy_int_enable_cfg_b;
    BOOLEAN                  prox_int_enable_cfg_b;
} DD_MAX_30102_DATA_IN_TYPE;

/**
 * @brief   MAX-30102 Output Interface Data Structure
 * @details MAX-30102 Output Interface Data Structure ...
 * @ingroup DriverStructures
 */
typedef struct DD_MAX_30102_DATA_OUT_TYPE_TAG
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
} DD_MAX_30102_DATA_OUT_TYPE;


/*************************************************************/
/*      CLASS DEFINITION                                     */
/*************************************************************/

class DD_MAX_30102_C {


  private:

    DD_MAX_30102_DATA_OUT_TYPE data_out_s;
    U8                         i2c_addr_u8;

    BOOLEAN get_int1( U8* const p_register_u8 );
    BOOLEAN get_int2( U8* const p_register_u8 );
    BOOLEAN get_int_status( BOOLEAN* p_int_status_vb );
    BOOLEAN get_temperature( DD_MAX_30102_DATA_OUT_TYPE* p_data_s );
    BOOLEAN set_fifo_int_a_full( const BOOLEAN enable_b );
    BOOLEAN set_data_ready( const BOOLEAN enable_b );
    BOOLEAN set_alc_ovf( const BOOLEAN enable_b );
    BOOLEAN set_prox_int( const BOOLEAN enable_b );
    BOOLEAN set_die_temp_rdy_int( const BOOLEAN enable_b );
    BOOLEAN soft_reset( void );
    BOOLEAN set_wake_up( const BOOLEAN enable_b );
    BOOLEAN set_mode( DD_MAX_30102_DATA_OUT_TYPE* const p_data_s, const DD_MAX_30102_MODE mode_e );
    BOOLEAN set_adc_range( const DD_MAX_30102_ADC_RANGE range_e );
    BOOLEAN set_sample_rate( const DD_MAX_30102_SAMPLE_RATE rate_e );
    BOOLEAN set_pulse_width( const DD_MAX_30102_PULSE_WIDTH width_e );
    BOOLEAN set_pulse_amp( const DD_MAX_30102_LED_TYPE type_e, const U8 amplitude_u8 );
    BOOLEAN set_proximity_threshold( const U8 threshold_u8 );
    BOOLEAN setup_slot( const DD_MAX_30102_SLOT slot_e, const DD_MAX_30102_SLOT_MODE mode_e );
    BOOLEAN set_sample_average( const DD_MAX_30102_SAMPLE_AVG average_e );
    BOOLEAN set_fifo_roll_over( BOOLEAN enable_b );
    BOOLEAN set_fifo_a_full_value( U8 value_u8 );
    BOOLEAN set_fifo_clear( void );
    BOOLEAN get_fifo_ptr_by_type( const DD_MAX_30102_PTR_TYPE ptr_type_e, U8* const p_value_u8 );
    BOOLEAN get_fifo_ovf_cnt( U8* const p_ovf_cnt_u8 );
    BOOLEAN get_part_id( U8* const p_register_u8 );
    BOOLEAN get_rev_id( U8* const p_register_u8 );
    BOOLEAN get_fifo_data( DD_MAX_30102_DATA_OUT_TYPE* p_data_s );

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
     * @details This function initialized the INA-219 device
     * @param[in] Pointer device input structure
     * @return Pointer to global device data structure
     */
    DD_MAX_30102_DATA_OUT_TYPE* init( DD_MAX_30102_DATA_IN_TYPE* p_data_in_s );

    /**
     * @details This is the drivers main function that shall be called
     * cyclicly and will provide all data through DD_INA_219_DATA_OUT_TYPE
     */
    void main( void );
};

#endif /* DD_MAX_30102_H_ */
