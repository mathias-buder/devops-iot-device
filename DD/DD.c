/*********************************************************************
    MODULE SPECIFICATION:

        $URL$
        $Revision$
        $Author$
        $Date$

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        (c) SEWELA 2019

*********************************************************************/

#include "DD.h"

#define DEVICE_ADDR   0x68
#define REGISTER_ADDR 0x41

U8 p_data_u8[] = { 0x01, 0x02, 0x03, 0x01, 0x02, 0x03, 0x01, 0x02, 0x03, 0x01, 0x02, 0x03          };

U8 data_u8 = 0x12;

U8 register_data_u8;

void dd_init(void)
{
    dd_i2c_init();
}


void dd_main(void)
{



    while(1)
    {
        for (int i = 1; i >= 0; i--)
        {
            printf("Restarting in %d seconds...\n", i);

            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }

        // dd_i2c_write_single(0x68, 0x43, 0x01);

        // dd_i2c_write_burst( 0x10, 0x20, p_data_u8, sizeof(p_data_u8) );

        dd_i2c_write_burst( DEVICE_ADDR, 0x20, &data_u8, sizeof(data_u8) );

        dd_i2c_read_burst(DEVICE_ADDR, REGISTER_ADDR, &register_data_u8, sizeof(register_data_u8) );

        // dd_i2c_read_bit(DEVICE_ADDR, REGISTER_ADDR, )

        printf("Device 0x%x, Register: 0x%x, Value %x\n", DEVICE_ADDR, REGISTER_ADDR, register_data_u8);
    }


}
