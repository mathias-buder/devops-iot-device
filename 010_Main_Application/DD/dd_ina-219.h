/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        (c) SEWELA 2020

        @file dd_ina_219.h
        @details INA-219 device driver implementation

*********************************************************************/
#ifndef DD_INA_219_H_
#define DD_INA_219_H_

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include "../../types.h"


/*************************************************************/
/*      COMPILE TIME CONFIGURATION                           */
/*************************************************************/
/* Log message TAG string */
#define DD_INA_219_LOG_MSG_TAG                  "DD_INA_219"  /**< @details Domain log message tag string */
#define DD_INA_219_SHUNT_RESISTOR_VALUE_OHM     ( 0.1F )      /**< Shunt resistor value @unit Ohm */



/*************************************************************/
/*      GLOBAL DEFINES                                       */
/*************************************************************/

/* Configuration Registers */
#define DD_INA_219_CONFIG                       0x00   /**< All-register reset, settings for bus voltage range, PGA Gain, ADC resolution/averaging (16 bit)*/

/* Data Output Registers */
#define DD_INA_219_SHUNT_VOLTAGE_DATA           0x01   /**< Shunt voltage measurement data */
#define DD_INA_219_BUS_VOLTAGE_DATA             0x02   /**< Bus voltage measurement data */
#define DD_INA_219_POWER_DATA                   0x03   /**< Power measurement data */
#define DD_INA_219_CURRENT_DATA                 0x04   /**< Contains the value of the current flowing through the shunt resistor */

/* Calibration Register */
#define DD_INA_219_CALIB_DATA                   0x05   /**< Sets full-scale range and LSB of current and power measurements. Overall system calibration.*/

/* RST: System reset Flag
 * setting this bit to '1' generates a system reset that is
 * the same as power-on reset. Resets all registers to default
 * values; this bit self-clears. */
#define DD_INA_219_SYS_RST_MASK                 0x7FFF /**< (word) ~0b 10000000 00000000 */
#define DD_INA_219_SYS_RST_ENABLE               0x8000

/* BRNG: Bus Voltage Range
 * 0 = 16V FSR
 * 1 = 32V FSR (default value) */
#define DD_INA_219_BRNG_MASK                    0xDFFF /**< (word) ~0b 00100000 00000000 */
#define DD_INA_219_BRNG_RANGE_16V               0x0000 /**< 16V FSR */
#define DD_INA_219_BRNG_RANGE_32V               0x2000 /**< 32V FSR, default */

/* PG: PGA (Shunt Voltage Only)
 * Sets PGA gain and range. Note that the PGA defaults to ÷8 (320mV range).
 * Table 4 (datasheet pg. 19) shows the gain and range for the various
 * product gain settings. */
#define DD_INA_219_PG_MASK                      0xE7FF /**< (word) ~0b 00110000 00000000 */
#define DD_INA_219_PG_GAIN_1_40MV               0x0000 /**< Full-scale current sense (input) voltage range: +/- 40 mV */
#define DD_INA_219_PG_GAIN_1_2_80MV             0x0800 /**< Full-scale current sense (input) voltage range: +/- 80 mV */
#define DD_INA_219_PG_GAIN_1_4_160MV            0x1000 /**< Full-scale current sense (input) voltage range: +/- 160 mV */
#define DD_INA_219_PG_GAIN_1_8_320MV            0x1800 /**< Full-scale current sense (input) voltage range: +/- 320 mV, default */

/* SADC: SADC Shunt ADC Resolution/Averaging
 * These bits adjust the Shunt ADC resolution (9-, 10-, 11-, or 12-bit) or set the number of samples
 * used when averaging results for the Shunt Voltage Register (01h). BADC (Bus) and SADC (Shunt) ADC
 * resolution/averaging and conversion time settings are shown in Table 5. (datasheet pg. 20) */
#define DD_INA_219_SADC_MASK                    0xFF87 /**< (word) ~0b 00000000 01111000 */
#define DD_INA_219_SADC_9BIT_1S_84US            0x0000 /**< 9-bit shunt Res = 0..511 */
#define DD_INA_219_SADC_10BIT_1S_148US          0x0008 /**< 10-bit shunt res = 0..1023 */
#define DD_INA_219_SADC_11BIT_1S_276US          0x0010 /**< 11-bit shunt res = 0..2047 */
#define DD_INA_219_SADC_12BIT_1S_532US          0x0018 /**< 12-bit shunt res = 0..4097, default */
#define DD_INA_219_SADC_12BIT_2S_1060US         0x0048 /**< 2 x 12-bit shunt samples averaged together */
#define DD_INA_219_SADC_12BIT_4S_2130US         0x0050 /**< 4 x 12-bit shunt samples averaged together */
#define DD_INA_219_SADC_12BIT_8S_4260US         0x0058 /**< 8 x 12-bit shunt samples averaged together */
#define DD_INA_219_SADC_12BIT_16S_8512US        0x0060 /**< 16 x 12-bit shunt samples averaged together */
#define DD_INA_219_SADC_12BIT_32S_17MS          0x0068 /**< 32 x 12-bit shunt samples averaged together */
#define DD_INA_219_SADC_12BIT_64S_34MS          0x0070 /**< 64 x 12-bit shunt samples averaged together */
#define DD_INA_219_SADC_12BIT_128S_69MS         0x0078 /**< 128 x 12-bit shunt samples averaged together */

/* BADC: BADC Bus ADC Resolution/Averaging
 * These bits adjust the Bus ADC resolution (9-, 10-, 11-, or 12-bit)
 * or set the number of samples used when averaging results for the Bus
 * Voltage Register (02h). */
#define DD_INA_219_BADC_MASK                    0xF87F /**< (word) ~0b 00000111 10000000 */
#define DD_INA_219_BADC_9BIT_1S_84US            0x0000 /**< 9-bit bus Res = 0..511 */
#define DD_INA_219_BADC_10BIT_1S_148US          0x0080 /**< 10-bit bus res = 0..1023 */
#define DD_INA_219_BADC_11BIT_1S_276US          0x0100 /**< 11-bit bus res = 0..2047 */
#define DD_INA_219_BADC_12BIT_1S_532US          0x0180 /**< 12-bit bus res = 0..4097, default*/
#define DD_INA_219_BADC_12BIT_2S_1060US         0x0480 /**< 2 x 12-bit bus samples averaged together */
#define DD_INA_219_BADC_12BIT_4S_2130US         0x0500 /**< 4 x 12-bit bus samples averaged together */
#define DD_INA_219_BADC_12BIT_8S_4260US         0x0580 /**< 8 x 12-bit bus samples averaged together */
#define DD_INA_219_BADC_12BIT_16S_8512US        0x0600 /**< 16 x 12-bit bus samples averaged together */
#define DD_INA_219_BADC_12BIT_32S_17MS          0x0680 /**< 32 x 12-bit bus samples averaged together */
#define DD_INA_219_BADC_12BIT_64S_34MS          0x0700 /**< 64 x 12-bit bus samples averaged together */
#define DD_INA_219_BADC_12BIT_128S_69MS         0x0780 /**< 128 x 12-bit bus samples averaged together */

/* MODE: Operating Mode
 * Selects continuous, triggered, or power-down mode of operation.
 * These bits default to continuous shunt and bus measurement mode.
 * The mode settings are shown in Table 6. (datasheet pg. 20) */
#define DD_INA_219_MODE_MASK                    0xFFF8 /**< (word) ~0b 00000000 00000111 */
#define DD_INA_219_MODE_POWER_DOWN              0x0000
#define DD_INA_219_MODE_SHUNT_VOLT_TRG          0x0001 /**< Shunt voltage, triggered */
#define DD_INA_219_MODE_BUS_VOLT_TRG            0x0002 /**< Bus voltage, triggered */
#define DD_INA_219_MODE_SHUNT_BUS_VOLT_TRG      0x0003 /**< Shunt and bus, triggered */
#define DD_INA_219_MODE_ADC_OFF                 0x0004 /**< ADC off (disabled) */
#define DD_INA_219_MODE_BUS_VOLT_CONT           0x0005 /**< Shunt voltage, continuous*/
#define DD_INA_219_MODE_SHUNT_VOLT_CONT         0x0006 /**< Bus voltage, continuous */
#define DD_INA_219_MODE_SHUNT_BUS_VOLT_CONT     0x0007 /**< Shunt and bus, continuous, default */

/* Bus Voltage Register
 * The Bus Voltage register stores the most recent bus voltage reading, VBUS.
 * At full-scale range = 32 V (decimal = 8000, hex = 1F40), and LSB = 4 mV. */
#define DD_INA_219_V_BUS_LSB_VOLT               ( 0.004F ) /**< V/LSB */
#define DD_INA_219_V_BUS_LSB_MILLI_VOLT         ( 4.0F )  /**< mV/LSB */

#define DD_INA_219_V_BUS_STAT_MASK              0xFFFC /**< (word) ~0b 00000000 00000011 */
#define DD_INA_219_V_BUS_STAT_OVF               0x0001 /**< Math Overflow Flag */
#define DD_INA_219_V_BUS_STAT_CNVR              0x0002 /**< Conversion Ready */

/* Shunt Voltage Register
 * The Shunt Voltage register stores the current shunt voltage reading, VSHUNT.
 * At PGA = /1, full-scale range = ±40 mV (decimal = 4000). For VSHUNT = +40 mV,
 * Value = 0FA0h; For VSHUNT = –40 mV, Value = F060h; and LSB = 10uV. */
#define DD_INA_219_V_SHUNT_LSB_VOLT            ( 0.00001F ) /**< Shunt voltage LSB @unit Volt/LSB */
#define DD_INA_219_V_SHUNT_LSB_MILLI_VOLT      ( 0.01F )    /**< Shunt voltage LSB @unit mVolt/LSB */


/*************************************************************
*                      ENUMERATORS                           *
*************************************************************/

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



/*************************************************************
*      STRUCTURES                                            *
*************************************************************/

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
    U16     voltage_raw_u16;    /**< @details Raw most recent bus voltage ADC reading @unit LSB */
} DD_INA_219_BUS_VOL_DATA_TYPE;


/**
 * @brief   INA-219 Input Interface Data Structure
 * @details Contains all data to configure the device
 * @ingroup DriverStructures
 */
typedef struct DD_INA_219_DATA_IN_TYPE_TAG
{
    DD_INA_219_SHUNT_VOL_RANGE shunt_voltage_range_e;            /**< @details Current full-scale current sense (input) voltage range */
    DD_INA_219_SADC_RES_AVE    shunt_adc_resolution_averaging_e; /**< @details Shunt voltage ADC resolution/averaging setting */
    DD_INA_219_BUS_VOL_RANGE   bus_voltage_range_e;              /**< @details Current bus voltage (input voltage) range */
    DD_INA_219_BADC_RES_AVE    bus_adc_resolution_averaging_e;   /**< @details Bus voltage ADC resolution/averaging setting */
    F32                        max_current_mA_f32;               /**< @details Maximum expected current (used for calibration) @unit mA */
} DD_INA_219_DATA_IN_TYPE;

/**
 * @brief   INA-219 Output Interface Data Structure
 * @details Contains all data that are provided by the device
 * @ingroup DriverStructures
 */
typedef struct DD_INA_219_DATA_OUT_TYPE_TAG
{

    DD_INA_219_BUS_VOL_DATA_TYPE bus_voltage_data_s;               /**< @details Raw most recent bus voltage ADC reading and status information */
    S16                          shunt_voltage_raw_s16;            /**< @details Raw ADC reading representing the voltage across the shunt resistor *@unit LSB */
    U16                          power_raw_u16;                    /**< @details Records power in watts by multiplying the value of the current with the
                                                                                 value of the bus voltage according to the Equation 5 (datasheet pg. 13) @unit LSB*/
    S16                          current_raw_s16;                  /**< @details Value is calculated by multiplying the value in the Shunt Voltage register
                                                                                 with the value in the Calibration register according to the Equation 4 (datasheet pg. 12) @unit LSB*/
    F32                          shunt_voltage_mV_f32;             /**< @details Voltage across the shunt resistor @unit mV */
    F32                          bus_voltage_V_f32;                /**< @details Bus voltage @unit V */
    F32                          bus_voltage_mV_f32;               /**< @details Bus voltage @unit mV */
    F32                          power_mW_f32;                     /**< @details Power @unit mW */
    F32                          current_mA_f32;                   /**< @details Current @unit mA */
    F32                          current_lsb_A_f32;                /**< @details Current LSB after calibration @unit Amp/LSB */
    F32                          power_lsb_W_f32;                  /**< @details Power LSB after calibration @unit Watt/LSB */
    U16                          calibration_register_u16;         /**< @details Contains the calibration register value @unit 1/Volt */
} DD_INA_219_DATA_OUT_TYPE;


/*************************************************************/
/*      CLASS DEFINITION                                     */
/*************************************************************/

class DD_INA_219_C {

  private:
    DD_INA_219_DATA_OUT_TYPE data_out_s;                  /**< @details Global device output data structure */
    U8                       i2c_addr_u8;                 /**< @details I2C device address */
    F32                      shut_resistor_value_ohm_f32; /**< @details Shunt resistor value @unit ohm */

    /**
     * @details This configures the INA-219 device
     * @param[in] shunt_voltage_range_e Shut voltage range
     * @param[in] shunt_voltage_res_ave_e Shunt voltage resolution and average settings
     * @param[in] bus_voltage_range_e Bus voltage range
     * @param[in] bus_voltage_res_ave_e Bus voltage resolution and average settings
     * @return TRUE in case no error during I2C transmission occurs
     */
    BOOLEAN configure( DD_INA_219_SHUNT_VOL_RANGE shunt_voltage_range_e,
                       DD_INA_219_SADC_RES_AVE    shunt_voltage_res_ave_e,
                       DD_INA_219_BUS_VOL_RANGE   bus_voltage_range_e,
                       DD_INA_219_BADC_RES_AVE    bus_voltage_res_ave_e );

    /**
     * @details This function writes the calibration register
     * @return TRUE in case no error during I2C transmission occurs
     */
    BOOLEAN calibrate( F32 max_current_mA_f32 );

    /**
     * @details This function reads the shunt voltage register
     * @param[in] p_value_s16 Pointer to variable that stores the register content
     * @return TRUE in case no error during I2C transmission occurs
     */
    BOOLEAN read_shunt_voltage_raw( S16* const p_value_s16 );

    /**
     * @details This function reads the bus voltage register
     * @param[in] p_value_s16 Pointer to variable that stores the register content
     * @return TRUE in case no error during I2C transmission occurs
     */
    BOOLEAN read_bus_voltage_raw( DD_INA_219_BUS_VOL_DATA_TYPE* const p_bus_data_s );

    /**
     * @details This function reads the power register
     * @param[in] p_value_u16 Pointer to variable that stores the register content
     * @return TRUE in case no error during I2C transmission occurs
     */
    BOOLEAN read_power_raw( U16* const p_value_u16 );

    /**
     * @details This function reads the current register
     * @param[in] p_value_s16 Pointer to variable that stores the register content
     * @return TRUE in case no error during I2C transmission occurs
     */
    BOOLEAN read_current_raw( S16* const p_value_s16 );


  public:

    /**
     * @details Default constructor
     * @param[in] i2c_addr_u8 I2C device address
     */
    DD_INA_219_C( U8 i2c_addr_u8, F32 shut_resistor_value_ohm_f32 );

    /**
     * @details Default destructor
     */
    ~DD_INA_219_C();

    /**
     * @details This function initialized the INA-219 device
     * @param[in] Pointer device input structure
     * @return Pointer to global device data structure
     */
    DD_INA_219_DATA_OUT_TYPE* init( DD_INA_219_DATA_IN_TYPE* p_data_in_s );

    /**
     * @details This is the drivers main function that shall be called
     * cyclicly and will provide all data through DD_INA_219_DATA_OUT_TYPE
     */
    void main( void );
};

#endif /* DD_INA_219_H_ */
