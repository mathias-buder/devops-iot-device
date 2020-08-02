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
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "esp_log.h"

#include "dd_pga-302.h"
#include "dd_i2c.h"
#include "dd_database.h"

/*********************************************************************/
/*      GLOBAL VARIABLES                                             */
/*********************************************************************/

/*********************************************************************/
/*      LOCAL VARIABLES                                              */
/*********************************************************************/

/*********************************************************************/
/*      PRIVATE FUNCTION DECLARATIONS                                */
/*********************************************************************/
PRIVATE BOOLEAN dd_pga_302_set_p_gain( const DD_PGA_302_P_GAIN gain_e );
PRIVATE BOOLEAN dd_pga_302_set_p_mode( const DD_PGA_302_P_INPUT_MODE mode_e );
PRIVATE BOOLEAN dd_pga_302_set_p_mux( const DD_PGA_302_P_INPUT_MUX mux_e );
PRIVATE BOOLEAN dd_pga_302_set_p_inverse( const BOOLEAN enable_b );


/*********************************************************************/
/*   FUNCTION DEFINITIONS                                            */
/*********************************************************************/
BOOLEAN dd_pga_302_init( void )
{
    ESP_LOGI( DD_PGA_302_LOG_MSG_TAG, "Initializing ..." );

    if(    ( FALSE == dd_pga_302_set_p_gain ( dd_pga_303_p_gain_cfg_e )      )
        || ( FALSE == dd_pga_302_set_p_mode( dd_pga_303_p_mode_cfg_e )       )
        || ( FALSE == dd_pga_302_set_p_mux( dd_pga_303_p_mux_cfg_e )         )
        || ( FALSE == dd_pga_302_set_p_inverse( dd_pga_303_p_inverse_cfg_b ) ) )
    {
        ESP_LOGE( DD_PGA_302_LOG_MSG_TAG, "Initializing failed" );
        return FALSE;
    }

    ESP_LOGI( DD_PGA_302_LOG_MSG_TAG, "Done" );
    return TRUE;
}


void dd_pga_302_main( void )
{

}

PRIVATE BOOLEAN dd_pga_302_set_p_gain( const DD_PGA_302_P_GAIN gain_e )
{
    U8 gain_u8;

    switch ( gain_e )
    {
        case DD_PGA_302_P_GAIN_1_33:
               gain_u8 = DD_PGA_302_P_GAIN_SEL_1_33;
            break;

        case DD_PGA_302_P_GAIN_2:
               gain_u8 = DD_PGA_302_P_GAIN_SEL_2;
            break;

        case DD_PGA_302_P_GAIN_4:
               gain_u8 = DD_PGA_302_P_GAIN_SEL_4;
            break;

        case DD_PGA_302_P_GAIN_10:
               gain_u8 = DD_PGA_302_P_GAIN_SEL_10;
            break;

        case DD_PGA_302_P_GAIN_20:
               gain_u8 = DD_PGA_302_P_GAIN_SEL_20;
            break;

        case DD_PGA_302_P_GAIN_40:
               gain_u8 = DD_PGA_302_P_GAIN_SEL_40;
            break;

        case DD_PGA_302_P_GAIN_100:
               gain_u8 = DD_PGA_302_P_GAIN_SEL_100;
            break;

        case DD_PGA_302_P_GAIN_200:
               gain_u8 = DD_PGA_302_P_GAIN_SEL_200;
            break;

        default:
        {
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

    }

    if ( FALSE == dd_i2c_read_modify_write_mask( DD_PGA_302_I2C_ADDR_DI_OFFSET_2, DD_PGA_302_P_GAIN_SELECT, DD_PGA_302_P_GAIN_MASK, gain_u8 ) )
    {
        ESP_LOGE( DD_PGA_302_LOG_MSG_TAG, "dd_pga_302_set_p_gain I2C error" );
        return FALSE;
    }

    return TRUE;
}




PRIVATE BOOLEAN dd_pga_302_set_p_mode( const DD_PGA_302_P_INPUT_MODE mode_e )
{
    U8 mode_u8;

    switch ( mode_e )
    {
        case DD_PGA_302_P_INPUT_MODE_SINGLE_ENDED:
            mode_u8 = DD_PGA_302_PSEM_SEL_SINGLE_ENDED;
            break;

        case DD_PGA_302_P_INPUT_MODE_DIFFERENTIAL:
            mode_u8 = DD_PGA_302_PSEM_SEL_DIFFERENTIAL;
            break;

        default:
        {
            assert( mode_e == DD_PGA_302_P_INPUT_MODE_SINGLE_ENDED );
            assert( mode_e == DD_PGA_302_P_INPUT_MODE_DIFFERENTIAL );

            return FALSE;
        }

    }

    if ( FALSE == dd_i2c_read_modify_write_mask( DD_PGA_302_I2C_ADDR_DI_OFFSET_2, DD_PGA_302_P_GAIN_SELECT, DD_PGA_302_PSEM_MASK, mode_u8 ) )
    {
        ESP_LOGE( DD_PGA_302_LOG_MSG_TAG, "dd_pga_302_set_p_mode I2C error" );
        return FALSE;
    }

    return TRUE;
}

PRIVATE BOOLEAN dd_pga_302_set_p_mux( const DD_PGA_302_P_INPUT_MUX mux_e )
{
    U8 mux_u8;

    switch ( mux_e )
    {
        case DD_PGA_302_P_INPUT_MUX_VINPP_VINPN:
            mux_u8 = DD_PGA_302_P_MUX_CTRL_SEL_VINPP_VINPN;
            break;

        case DD_PGA_302_P_INPUT_MUX_VINPP_1_25V:
            mux_u8 = DD_PGA_302_P_MUX_CTRL_SEL_VINPP_1_25V;
            break;

        case DD_PGA_302_P_INPUT_MUX_1_25V_VINPN:
            mux_u8 = DD_PGA_302_P_MUX_CTRL_SEL_1_25V_VINPN;
            break;

        default:
        {
            assert( mux_u8 == DD_PGA_302_P_INPUT_MUX_VINPP_VINPN );
            assert( mux_u8 == DD_PGA_302_P_INPUT_MUX_VINPP_1_25V );
            assert( mux_u8 == DD_PGA_302_P_INPUT_MUX_1_25V_VINPN );

            return FALSE;
        }

    }

    if ( FALSE == dd_i2c_read_modify_write_mask( DD_PGA_302_I2C_ADDR_DI_OFFSET_2, DD_PGA_302_P_GAIN_SELECT, DD_PGA_302_P_MUX_CTRL_MASK, mux_u8 ) )
    {
        ESP_LOGE( DD_PGA_302_LOG_MSG_TAG, "dd_pga_302_set_p_mux I2C error" );
        return FALSE;
    }

    return TRUE;
}


PRIVATE BOOLEAN dd_pga_302_set_p_inverse( const BOOLEAN enable_b )
{
    return ( dd_i2c_read_modify_write_mask( DD_PGA_302_I2C_ADDR_DI_OFFSET_2,
                                            DD_PGA_302_P_GAIN_SELECT,
                                            DD_PGA_302_P_INV_MASK,
                                            ( ( TRUE == enable_b ) ? DD_PGA_302_P_INV_ENABLE : DD_PGA_302_P_INV_DISABLE ) ) );
}
