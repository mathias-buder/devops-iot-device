/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        @file dd_mcpwm.h
        @details MCPWM device driver implementation

        (c) SEWELA 2020

*********************************************************************/

#ifndef DD_MCPWM_H_
#define DD_MCPWM_H_

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include "../../types.h"
#include "driver/mcpwm.h"

/*************************************************************/
/*      COMPILE TIME CONFIGURATION                           */
/*************************************************************/

#define DD_MCPWM_LOG_MSG_TAG              "DD_MCPWM"          /**< Domain log message tag string */
#define DD_MCPWM_FREQUANCY                1000U               /**< PWM frequency for all channels @unit Hz */
#define DD_MCPWM_COUNTER_MODE             MCPWM_UP_COUNTER    /**< Asymmetric MCPWM*/
#define DD_MCPWM_DUTY_MODE                MCPWM_DUTY_MODE_0   /**< Active high duty, i.e. duty cycle proportional to high time for asymmetric MCPWM */
#define DD_MCPWM_MAX_DUTY_CYCLE           100.0F              /**< Active Maximum allowed duty cycle per channel @unit % */



/*************************************************************
*                      ENUMERATORS                           *
*************************************************************/
/**
 * @details enumerator of ...
 */
typedef enum DD_MCPWM_MODE_TAG
{
    DD_MCPWM_MODE_OFF = 0U, /**< @details Default mode: Output is constant low/GND (0.0V) */
    DD_MCPWM_MODE_ON,       /**< @details Output is constant high (3.3V) */
    DD_MCPWM_MODE_PWM,      /**< @details Output is running an PWM signal with an duty cycle of duty_cycle_f32 */
    DD_MCPWM_MODE_SIZE
} DD_MCPWM_MODE;

/**
 * @details enumerator of ...
 */
typedef enum DD_MCPWM_CHANNEL_TAG
{
#ifndef JTAG_DEBUG_SUPPORT_ENABLE
    DD_MCPWM_CHANNEL_1 = 0U, /*!< Shared with JTAG pin: TMS (14) */
#endif
    DD_MCPWM_CHANNEL_2,
    DD_MCPWM_CHANNEL_3,
    DD_MCPWM_CHANNEL_4,
    DD_MCPWM_CHANNEL_5,
    DD_MCPWM_CHANNEL_6,
#ifndef JTAG_DEBUG_SUPPORT_ENABLE
    DD_MCPWM_CHANNEL_7,      /*!< Shared with JTAG pin: TCK (13) */
    DD_MCPWM_CHANNEL_8,      /*!< Shared with JTAG pin: TDI (12) */
    DD_MCPWM_CHANNEL_9,      /*!< Shared with JTAG pin: TDO (15) */
#endif
    DD_MCPWM_CHANNEL_10,
    DD_MCPWM_CHANNEL_11,
    DD_MCPWM_CHANNEL_12,
    DD_MCPWM_CHANNEL_SIZE
} DD_MCPWM_CHANNEL;


/*************************************************************
*      STRUCTURES                                            *
*************************************************************/

/**
 * @brief   MCPWM channel definition structure
 * @details Defines all hardware related settings for each channel
 * @ingroup DriverStructures
 */
typedef struct DD_MCPWM_CONFIG_TYPE_TAG
{
    DD_MCPWM_CHANNEL   name_e;         /**< @details Channel name */
    mcpwm_unit_t       unit_e;         /**< @details MCPWM unit(0-1) */
    mcpwm_io_signals_t io_signal_e;    /**< @details MCPWM signals, each MCPWM unit has 6 output(MCPWMXA, MCPWMXB) and 9 input(SYNC_X, FAULT_X, CAP_X) 'X' is timer_num(0-2) */
    gpio_num_t         io_pin_e;       /**< @details Corresponding I/O pin to be multiplexed to output the PWM signal */
    mcpwm_timer_t      timer_e;        /**< @details Timer number(0-2) of MCPWM, each MCPWM unit has 3 timers */
    mcpwm_operator_t   operator_e;     /**< @details The operator(MCPWMXA/MCPWMXB), 'X' is timer number selected */
    DD_MCPWM_MODE      mode_e;         /**< @details Initial channel mode */
    F32                duty_cycle_f32; /**< @details Initial channel duty cycle @unit [%] */
} DD_MCPWM_CONFIG_TYPE;

/**
 * @brief   MCPWM input data structure
 * @details Defines all inputs required by for MCPWM driver
 * @ingroup DriverStructures
 */
typedef struct DD_MCPWM_DATA_IN_TYPE_TAG
{
    DD_MCPWM_MODE mode_e;         /**< @details Channel mode */
    F32           duty_cycle_f32; /**< @details Channel duty cycle @unit [%] */
} DD_MCPWM_DATA_IN_TYPE;

/*************************************************************/
/*      CLASS DEFINITION                                     */
/*************************************************************/
class DD_MCPWM_C {

  private:
    const mcpwm_config_t m_timer_cfg_s = {
            .frequency    = DD_MCPWM_FREQUANCY,   /*!< Default PWM frequency for all channels */
            .cmpr_a       = 0.0F,                 /*!< Default duty cycle for operator a(MCPWMXA) set to 0.0 */
            .cmpr_b       = 0.0F,                 /*!< Default duty cycle for operator b(MCPWMXB) set to 0.0 */
            .duty_mode    = DD_MCPWM_DUTY_MODE,   /*!< Default duty mode for all channels */
            .counter_mode = DD_MCPWM_COUNTER_MODE /*!< Default counter mode for all channels */
        };

    DD_MCPWM_CONFIG_TYPE m_channel_s;

  public:
    /**
   * @details Default constructor
   * @param[in] i2c_addr_u8 I2C device address
   */
    DD_MCPWM_C( void );

    /**
   * @details Default destructor
   */
    ~DD_MCPWM_C();

    /**
   * @details This function initialized the MCPWM device
   *          with its default configuration.
   * @return TRUE in case no error was reported during
   *              MCPWM module configuration
   */
    BOOLEAN init( const DD_MCPWM_CONFIG_TYPE &r_config_s );


    /**
   * @details This function initialized the MCPWM device
   * @param[in] Copy of input data structure
   * @return[in] TRUE in case no error was reported during
   *             MCPWM module update
   */
    BOOLEAN update( DD_MCPWM_DATA_IN_TYPE &r_data_in_s );
};

#endif /* DD_MCPWM_H_ */
