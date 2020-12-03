/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        @file ve_grid.c
        @details Some detailed description

        (c) SEWELA 2020

*********************************************************************/

/*********************************************************************/
/*      INCLUDES                                                     */
/*********************************************************************/
#include "ve_grid.h"


/*********************************************************************/
/*      GLOBAL VARIABLES                                             */
/*********************************************************************/


//VE_GRID_VIRBATOR_TYPE ve_grid_vibrator_config_vs[VE_GRID_VIBRATOR_SIZE] = {
//                            /*  PWM channel assignment      X-Position [mm]     Y-Position [mm]     Duty Cycle [%] */
//#ifndef JTAG_DEBUG_SUPPORT_ENABLE
//        /* VIBRATOR 1 */    {   DD_MCPWM_CHANNEL_1,         0.0F,               0.0F,               0.0F    },
//#endif
//        /* VIBRATOR 2 */    {   DD_MCPWM_CHANNEL_2,         0.0F,               0.0F,               0.0F    },
//        /* VIBRATOR 3 */    {   DD_MCPWM_CHANNEL_3,         0.0F,               0.0F,               0.0F    },
//        /* VIBRATOR 4 */    {   DD_MCPWM_CHANNEL_4,         0.0F,               0.0F,               0.0F    },
//        /* VIBRATOR 5 */    {   DD_MCPWM_CHANNEL_5,         0.0F,               0.0F,               0.0F    },
//        /* VIBRATOR 6 */    {   DD_MCPWM_CHANNEL_6,         0.0F,               0.0F,               0.0F    },
//#ifndef JTAG_DEBUG_SUPPORT_ENABLE
//        /* VIBRATOR 7 */    {   DD_MCPWM_CHANNEL_7,         0.0F,               0.0F,               0.0F    },
//        /* VIBRATOR 8 */    {   DD_MCPWM_CHANNEL_8,         0.0F,               0.0F,               0.0F    },
//        /* VIBRATOR 9 */    {   DD_MCPWM_CHANNEL_9,         0.0F,               0.0F,               0.0F    },
//#endif
//        /* VIBRATOR 10 */   {   DD_MCPWM_CHANNEL_10,        0.0F,               0.0F,               0.0F    }
//};


/*********************************************************************/
/*   FUNCTION DEFINITIONS                                            */
/*********************************************************************/
VE_GRID_C::VE_GRID_C( void )
{
}

VE_GRID_C::~VE_GRID_C()
{
}

VE_GRID_DATA_OUT_TYPE* VE_GRID_C::init( void )
{
    /* Default unit configuration */
    VE_GRID_CONFIG_TYPE default_cfg_s = {
        /* vibrator_vs */
        {                 /*  x [mm]   y [mm]   */
         /* VINRATOR 1  */ {  1.0F,    10.0F },
         /* VINRATOR 2  */ {  2.0F,    9.0F  },
         /* VINRATOR 3  */ {  3.0F,    8.0F  },
         /* VINRATOR 4  */ {  4.0F,    7.0F  },
         /* VINRATOR 5  */ {  5.0F,    6.0F  },
         /* VINRATOR 6  */ {  6.0F,    5.0F  },
         /* VINRATOR 7  */ {  7.0F,    4.0F  },
         /* VINRATOR 8  */ {  8.0F,    3.0F  },
         /* VINRATOR 9  */ {  9.0F,    2.0F  },
         /* VINRATOR 10 */ {  10.0F,   1.0F  }
        }
    };

    return init( default_cfg_s );
}

VE_GRID_DATA_OUT_TYPE* VE_GRID_C::init( VE_GRID_CONFIG_TYPE& r_config_s )
{
    U8 idx_u8;

    for ( idx_u8 = 0U; idx_u8 < VE_GRID_VIBRATOR_SIZE; ++idx_u8 )
    {
        this->vibrator_vs[idx_u8].position_s = r_config_s.vibrator_pos_vs[idx_u8];
    }

    return &this->data_out_s;
}


void VE_GRID_C::main( VE_GRID_DATA_IN_TYPE& r_data_in_s )
{

}


