/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        (c) SEWELA 2020

        @file dd_pga-302.h
        @details Some detailed description

*********************************************************************/

#ifndef DD_PGA_302_H
#define DD_PGA_302_H

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include "dd_types.h"

#include "../Config/dd_pga-302_cfg.h"
#include "../Config/dd_types_cfg.h"


/*************************************************************/
/*      GLOBAL DEFINES                                       */
/*************************************************************/

#define DD_PGA302_P_GAIN_SELECT                 0x47
#define DD_PGA302_T_GAIN_SELECT                 0x48
#define DD_PGA302_TEMP_CTRL                     0x4C
#define DD_PGA302_OFFSET_CANCE_L                0x4E
#define DD_PGA302_PADC_DATA1                    0x20
#define DD_PGA302_PADC_DATA2                    0x21
#define DD_PGA302_TADC_DATA1                    0x24
#define DD_PGA302_TADC_DATA2                    0x25
#define DD_PGA302_DAC_REG0_1                    0x30
#define DD_PGA302_DAC_REG0_2                    0x31
#define DD_PGA302_OP_STAGE_CTR_L                0x3B
#define DD_PGA302_PSMON1                        0x58
#define DD_PGA302_AFEDIAG                       0x5A
#define DD_PGA302_MICRO_IF_CTL                  0x0C


/*************************************************************/
/*      GLOBAL VARIABLES                                     */
/*************************************************************/


/*************************************************************/
/*      PROTOTYPES                                           */
/*************************************************************/
BOOLEAN dd_pga_302_init( void );
void    dd_pga_302_main( void );

#endif /* DD_PGA_302_H */
