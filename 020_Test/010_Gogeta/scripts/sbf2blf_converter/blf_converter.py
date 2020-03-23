"""
Use this to convert .can/.asc files to .log files.
Usage: simpleLogConvert.py sourceLog.asc targetLog.log
"""

import sys
import os
import can.io.logger
import numpy as np
import struct as st
import ctypes as t
from random import seed
import random

DLG_ICM_20600_ACCEL_ID = 0x14
DLG_ICM_20600_GYRO_ID = 0x15
DLG_ICM_20600_TEMPERATURE_ID = 0x16
DLG_ICM_20600_SELF_TEST_ID = 0x17
DLG_ICM_20600_FACTORY_TRIM_ID = 0x18
DLG_ICM_20600_FACTORY_TRIM_DEV_ID = 0x19
DLG_ICM_20600_GENERAL_ID = 0x1A

time_stamp_interval = 0.1  # 100 ms
n_samples = 10000

writer = can.io.logger.Logger( "test_1.blf" )
writer.channel = 1

DD_ICM_20600_accel_x = 0
DD_ICM_20600_accel_y = 0
DD_ICM_20600_accel_z = 0

path = 'E:\\'

files = []
# r=root, d=directories, f = files
for r, d, f in os.walk( path ):
    for file in f:
        if '.sbl' in file:
            files.append( os.path.join( r, file ) )

for f in files:
    print( f )

filename = "E:/test.sbf"
struct_fmt = '<17f 10B 7H L'
struct_len = st.calcsize( struct_fmt )
struct_unpack = st.Struct( struct_fmt ).unpack_from

results = []
with open( filename, "rb" ) as file:
    while True:
        data = file.read( struct_len )
        if not data: break
        [temperature_deg_f32,
         factory_trim_xa_f32,
         factory_trim_ya_f32,
         factory_trim_za_f32,
         factory_trim_xg_f32,
         factory_trim_yg_f32,
         factory_trim_zg_f32,
         factory_trim_dev_xa_f32,
         factory_trim_dev_ya_f32,
         factory_trim_dev_za_f32,
         factory_trim_dev_xg_f32,
         factory_trim_dev_yg_f32,
         factory_trim_dev_zg_f32,
         quaternion_q1_f32,
         quaternion_q2_f32,
         quaternion_q3_f32,
         quaternion_q4_f32,
         chip_id_u8,
         dev_state_u8,
         is_calibrated_u8,
         self_test_passed_u8,
         self_test_xa_u8,
         self_test_ya_u8,
         self_test_za_u8,
         self_test_xg_u8,
         self_test_yg_u8,
         self_test_zg_u8,
         accel_raw_data_x_u16,
         accel_raw_data_y_u16,
         accel_raw_data_z_u16,
         gyro_raw_data_x_u16,
         gyro_raw_data_y_u16,
         gyro_raw_data_z_u16,
         temperature_raw_u16,
         global_time_u32] = struct_unpack( data )


        # Scale (according to DLG.dbc) and pack date
        msg_icm_20600_accel_data = st.pack( 'BBBBBB', np.uint8( accel_raw_data_x_u16 ),
                                                     np.uint8( accel_raw_data_x_u16 >> 8 ),
                                                     np.uint8( accel_raw_data_y_u16 ),
                                                     np.uint8( accel_raw_data_y_u16 >> 8 ),
                                                     np.uint8( accel_raw_data_z_u16 ),
                                                     np.uint8( accel_raw_data_z_u16 >> 8 ) )

        msg_icm_20600_accel = can.Message( arbitration_id=DLG_ICM_20600_ACCEL_ID,
                                           is_extended_id=False,
                                           timestamp=global_time_u32,
                                           data=msg_icm_20600_accel_data )

        # Scale (according to DLG.dbc) and pack date
        msg_icm_20600_gyro_data = st.pack( 'BBBBBB', np.uint8( gyro_raw_data_x_u16 ),
                                                     np.uint8( gyro_raw_data_x_u16 >> 8 ),
                                                     np.uint8( gyro_raw_data_y_u16 ),
                                                     np.uint8( gyro_raw_data_y_u16 >> 8 ),
                                                     np.uint8( gyro_raw_data_z_u16 ),
                                                     np.uint8( gyro_raw_data_z_u16 >> 8 ) )

        msg_icm_20600_gyro = can.Message( arbitration_id=DLG_ICM_20600_GYRO_ID,
                                          is_extended_id=False,
                                          timestamp=global_time_u32,
                                          data=msg_icm_20600_gyro_data )

        # Scale (according to DLG.dbc) and pack date
        msg_icm_20600_general_data = st.pack( 'BBB', np.uint8( chip_id_u8 ),
                                                     np.uint8( dev_state_u8 ) ,
                                                     np.uint8( (   ( self_test_passed_u8 & 0x01 ) << 1)
                                                                 | ( is_calibrated_u8 & 0x01 ) ) )

        msg_icm_20600_general = can.Message( arbitration_id=DLG_ICM_20600_GENERAL_ID,
                                             is_extended_id=False,
                                             timestamp=global_time_u32,
                                             data=msg_icm_20600_general_data )

        # Scale (according to DLG.dbc) and pack date
        temperature_deg_f32 = (temperature_deg_f32 + 10) * 10
        msg_icm_20600_temperature_date = st.pack( 'BBBB', np.uint8( temperature_raw_u16 ),
                                                  np.uint8( temperature_raw_u16 >> 8 ),
                                                  np.uint8( temperature_deg_f32 ),
                                                  ( int( temperature_deg_f32 ) >> 8 ) & 0x03 )

        msg_icm_20600_temperature = can.Message( arbitration_id=DLG_ICM_20600_TEMPERATURE_ID,
                                                 is_extended_id=False,
                                                 timestamp=global_time_u32,
                                                 data=msg_icm_20600_temperature_date )

        print( 'Temp:' + str(temperature_deg_f32) + " @ time: " + str(global_time_u32) )

        writer.on_message_received( msg_icm_20600_accel )
        writer.on_message_received( msg_icm_20600_gyro )
        writer.on_message_received( msg_icm_20600_general )
        writer.on_message_received( msg_icm_20600_temperature )



writer.stop()
