/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        @file dd_macpwm_if.h
        @details Some detailed description

        (c) SEWELA 2020

*********************************************************************/

#ifndef DD_MCPWM_IF_H
#define DD_MCPWM_IF_H

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include "esp_log.h"

#include "dd_public_types.h"

/*************************************************************/
/*      GLOBAL VARIABLES                                     */
/*************************************************************/

/*************************************************************/
/*      PROTOTYPES                                           */
/*************************************************************/
extern BOOLEAN        dd_mcpwm_if_init( void );
extern void           dd_mcpwm_if_main( void );
extern DD_MCPWM_DATA* dd_mcpwm_get_database( void );
extern BOOLEAN        dd_mcpwm_set_duty_cycle( DD_MCPWM_CHANNEL channel_e, F32 duty_cycle_f32 );
extern F32            dd_mcpwm_get_duty_cycle( DD_MCPWM_CHANNEL channel_e );
extern BOOLEAN        dd_mcpwm_set_mode( DD_MCPWM_CHANNEL channel_e, DD_MCPWM_MODE mode_e );
extern DD_MCPWM_MODE  dd_mcpwm_get_mode( DD_MCPWM_CHANNEL channel_e );

#endif /* DD_MCPWM_IF_H */
