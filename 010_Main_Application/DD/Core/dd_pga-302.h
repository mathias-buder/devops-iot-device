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

#define DD_PGA_302_P_GAIN_SELECT                 0x47
#define DD_PGA_302_T_GAIN_SELECT                 0x48
#define DD_PGA_302_TEMP_CTRL                     0x4C
#define DD_PGA_302_OFFSET_CANCEL                 0x4E
#define DD_PGA_302_PADC_DATA1                    0x20
#define DD_PGA_302_PADC_DATA2                    0x21
#define DD_PGA_302_TADC_DATA1                    0x24
#define DD_PGA_302_TADC_DATA2                    0x25
#define DD_PGA_302_DAC_REG0_1                    0x30
#define DD_PGA_302_DAC_REG0_2                    0x31
#define DD_PGA_302_OP_STAGE_CTRL                 0x3B
#define DD_PGA_302_PSMON1                        0x58
#define DD_PGA_302_AFEDIAG                       0x5A
#define DD_PGA_302_MICRO_IF_CTRL                 0x0C


/* P_GAIN_SELECT: Pressure channel configuration */

/* P_GAIN: Pressure gain select */
#define DD_PGA_302_P_GAIN_MASK                   0xF8 /* (byte)~0b00000111 */
#define DD_PGA_302_P_GAIN_SEL_1_33               0x00
#define DD_PGA_302_P_GAIN_SEL_2                  0x01
#define DD_PGA_302_P_GAIN_SEL_4                  0x02
#define DD_PGA_302_P_GAIN_SEL_10                 0x03
#define DD_PGA_302_P_GAIN_SEL_20                 0x04
#define DD_PGA_302_P_GAIN_SEL_40                 0x05
#define DD_PGA_302_P_GAIN_SEL_100                0x06
#define DD_PGA_302_P_GAIN_SEL_200                0x07

/* PSEM: Pressure input mode select */
#define DD_PGA_302_PSEM_MASK                     0xF7 /* (byte)~0b00001000 */
#define DD_PGA_302_PSEM_SEL_SINGLE_ENDED         0x00
#define DD_PGA_302_PSEM_SEL_DIFFERENTIAL         0x01

/* P_MUX_CTRL: Pressure input mux select */
#define DD_PGA_302_P_MUX_CTRL_MASK               0xCF /* (byte)~0b01100000 */
#define DD_PGA_302_P_MUX_CTRL_SEL_VINPP_VINPN    0x00
#define DD_PGA_302_P_MUX_CTRL_SEL_VINPP_1_25V    0x10
#define DD_PGA_302_P_MUX_CTRL_SEL_1_25V_VINPN    0x20

/* P_INV: Pressure gain output inverse select */
#define DD_PGA_302_P_INV_MASK                    0x7F /* (byte)~0b10000000 */
#define DD_PGA_302_P_INV_DISABLE                 0x00
#define DD_PGA_302_P_INV_ENABLE                  0x80


/*************************************************************/
/*      GLOBAL VARIABLES                                     */
/*************************************************************/


/*************************************************************/
/*      PROTOTYPES                                           */
/*************************************************************/
BOOLEAN dd_pga_302_init( void );
void    dd_pga_302_main( void );

#endif /* DD_PGA_302_H */
