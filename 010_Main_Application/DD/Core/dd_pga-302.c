/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        (c) SEWELA 2020

        @file dd_pga-302.c
        @details Some detailed description

*********************************************************************/

/*********************************************************************/
/*      INCLUDES                                                     */
/*********************************************************************/
#include "dd_pga-302.h"

#include <stdio.h>

/*********************************************************************/
/*      GLOBAL VARIABLES                                             */
/*********************************************************************/

/*********************************************************************/
/*      LOCAL VARIABLES                                              */
/*********************************************************************/

/*********************************************************************/
/*      PRIVATE FUNCTION DECLARATIONS                                */
/*********************************************************************/



/*********************************************************************/
/*   FUNCTION DEFINITIONS                                            */
/*********************************************************************/
BOOLEAN dd_pga_302_init( void )
{
    return TRUE;
}


void dd_pga_302_main( void )
{

}

PRIVATE BOOLEAN dd_pga_302_set_p_gain( const DD_PGA_302_P_GAIN gain_e )
{
    U8 gain_u8 = 0xFF;

    switch ( gain_e )
    {
    case DD_PGA_302_P_GAIN_1_33:
     //   gain_u8 = DD_MAX_30102_LED1_PULSE_AMP;
        break;


    default:
        assert( gain_e == DD_PGA_302_P_GAIN_1_33 );
        assert( gain_e == DD_PGA_302_P_GAIN_2    );
        assert( gain_e == DD_PGA_302_P_GAIN_4    );
        assert( gain_e == DD_PGA_302_P_GAIN_10   );
        assert( gain_e == DD_PGA_302_P_GAIN_20   );
        assert( gain_e == DD_PGA_302_P_GAIN_40   );
        assert( gain_e == DD_PGA_302_P_GAIN_100  );
        assert( gain_e == DD_PGA_302_P_GAIN_200  );
        return FALSE;
    }

    if (    ( 0xFF  == gain_u8 )
         || ( FALSE == dd_i2c_write_single( DD_MAX_30105_I2C_ADDR, type_reg_addr_u8, amplitude_u8 ) ) )
    {
        ESP_LOGE( DD_MAX_30105_LOG_MSG_TAG, "dd_max_30102_set_amplitude %s error", ( 0xFF  == type_reg_addr_u8 ) ? "wrong argument" : "I2C" );
        return FALSE;
    }

    return TRUE;
}
