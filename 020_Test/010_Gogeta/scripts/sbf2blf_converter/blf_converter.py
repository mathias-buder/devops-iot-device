"""
Use this to convert .can/.asc files to .log files.
Usage: simpleLogConvert.py sourceLog.asc targetLog.log
"""

import sys
import os
import can.io.logger
import numpy as np
import struct as st
import ctypes

DLG_ICM_20600_GENERAL_ID                = 0x14 # 20
DLG_ICM_20600_ACCEL_ID                  = 0x15 # 21
DLG_ICM_20600_GYRO_ID                   = 0x16 # 22
DLG_ICM_20600_TEMPERATURE_ID            = 0x17 # 23
DLG_ICM_20600_SELF_TEST_ID              = 0x18 # 24
DLG_ICM_20600_FACTORY_ACCEL_TRIM_ID     = 0x19 # 25
DLG_ICM_20600_FACTORY_GYRO_TRIM_ID      = 0x1A # 26
DLG_ICM_20600_FACTORY_ACCL_TRIM_DEV_ID  = 0x1B # 27
DLG_ICM_20600_FACTORY_GYRO_TRIM_DEV_ID  = 0x1C # 28

DLG_I2C_ERROR_ID                        = 0x32 # 50




writer = can.io.logger.Logger( "test_1.blf" )
writer.channel = 1

DD_ICM_20600_accel_x = 0
DD_ICM_20600_accel_y = 0
DD_ICM_20600_accel_z = 0


# path = 'E:\\'

# files = []
# r=root, d=directories, f = files
# for r, d, f in os.walk( path ):
#    for file in f:
#        if '.sbl' in file:
#            files.append( os.path.join( r, file ) )

# for f in files:
#    print( f )


def convert(x):
    return ctypes.c_short( int( x ) << 6 ).value >> 6


def convert_to_10bit(value):
    _value = int( abs( value ) ) & 0x3FF

    if value < 0:
        _value = ~_value
        _value = _value + 1

    return _value


def twos_complement(val, nbits):
    """Compute the 2's complement of int value val"""
    if val < 0:
        val = (1 << nbits) + val
    else:
        if (val & (1 << (nbits - 1))) != 0:
            # If sign bit is set.
            # compute negative value.
            val = val - (1 << nbits)
    return val


def twos(val_str, bytes):
    val = int( val_str )
    b = val.to_bytes( bytes, byteorder=sys.byteorder, signed=False )
    return int.from_bytes( b, byteorder=sys.byteorder, signed=True )


# filename = "C:/Users/buderm/Desktop/test.sbf"
filename = "E:/test.sbf"
#               DLG_LOG_ICM_20600_DATA  DLG_LOG_I2C_DATA (f 3B H)
struct_fmt = '< 14f 10B 6h H            4B h 2x'
struct_len = st.calcsize( struct_fmt )
struct_unpack = st.Struct( struct_fmt ).unpack_from

results = []
with open( filename, "rb" ) as file:
    while True:
        data = file.read( struct_len )
        if not data: break
        [
         # DLG_LOG_ICM_20600_DATA
         global_time_f32,
         temperature_deg_f32,
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
         accel_raw_data_x_s16,
         accel_raw_data_y_s16,
         accel_raw_data_z_s16,
         gyro_raw_data_x_s16,
         gyro_raw_data_y_s16,
         gyro_raw_data_z_s16,
         temperature_raw_u16,

         # DLG_LOG_I2C_DATA
         error_state_u8,
         access_type_u8,
         device_addr_u8,
         register_addr_u8,
         error_code_s16
        ] = struct_unpack( data )

        # Scale (according to DLG.dbc) and pack date
        msg_icm_20600_accel_data = st.pack( '3h', accel_raw_data_x_s16,
                                                  accel_raw_data_y_s16,
                                                  accel_raw_data_z_s16 )

        msg_icm_20600_accel = can.Message( arbitration_id=DLG_ICM_20600_ACCEL_ID,
                                           is_extended_id=False,
                                           timestamp=global_time_f32,
                                           data=msg_icm_20600_accel_data )

        # Scale (according to DLG.dbc) and pack date
        msg_icm_20600_gyro_data = st.pack( '3h', gyro_raw_data_x_s16,
                                                 gyro_raw_data_y_s16,
                                                 gyro_raw_data_z_s16 )

        msg_icm_20600_gyro = can.Message( arbitration_id=DLG_ICM_20600_GYRO_ID,
                                          is_extended_id=False,
                                          timestamp=global_time_f32,
                                          data=msg_icm_20600_gyro_data )

        # Scale (according to DLG.dbc) and pack date
        msg_icm_20600_general_data = st.pack( '3B', np.uint8( chip_id_u8 ),
                                              np.uint8( dev_state_u8 ),
                                              np.uint8( ((self_test_passed_u8 & 0x01) << 1)
                                                        | (is_calibrated_u8 & 0x01) ) )

        msg_icm_20600_general = can.Message( arbitration_id=DLG_ICM_20600_GENERAL_ID,
                                             is_extended_id=False,
                                             timestamp=global_time_f32,
                                             data=msg_icm_20600_general_data )

        # Scale (according to DLG.dbc) and pack date
        temperature_deg_f32 = temperature_deg_f32 * 100

        msg_icm_20600_temperature_date = st.pack( 'Hh', np.uint16(temperature_raw_u16),
                                                        np.int16(temperature_deg_f32 ) )

        msg_icm_20600_temperature = can.Message( arbitration_id=DLG_ICM_20600_TEMPERATURE_ID,
                                                 is_extended_id=False,
                                                 timestamp=global_time_f32,
                                                 data=msg_icm_20600_temperature_date )

        # Scale (according to DLG.dbc) and pack date
        msg_icm_20600_self_test_data = st.pack( '6B', np.uint8( self_test_xa_u8 ),
                                                np.uint8( self_test_ya_u8 ),
                                                np.uint8( self_test_za_u8 ),
                                                np.uint8( self_test_xg_u8 ),
                                                np.uint8( self_test_yg_u8 ),
                                                np.uint8( self_test_zg_u8 ) )

        msg_icm_20600_self_test = can.Message( arbitration_id=DLG_ICM_20600_SELF_TEST_ID,
                                               is_extended_id=False,
                                               timestamp=global_time_f32,
                                               data=msg_icm_20600_self_test_data )

        # Scale (according to DLG.dbc) and pack date
        msg_icm_20600_factory_accl_trim_data = st.pack( '3h', np.int16( factory_trim_xa_f32 ),
                                                              np.int16( factory_trim_ya_f32 ),
                                                              np.int16( factory_trim_za_f32 ) )

        msg_icm_20600_factory_accl_trim = can.Message( arbitration_id=DLG_ICM_20600_FACTORY_ACCEL_TRIM_ID,
                                                       is_extended_id=False,
                                                       timestamp=global_time_f32,
                                                       data=msg_icm_20600_factory_accl_trim_data )

        # Scale (according to DLG.dbc) and pack date
        msg_icm_20600_factory_gyro_trim_data = st.pack( '3h', np.int16( factory_trim_xg_f32 ),
                                                        np.int16( factory_trim_yg_f32 ),
                                                        np.int16( factory_trim_zg_f32 ) )

        msg_icm_20600_factory_gyro_trim = can.Message( arbitration_id=DLG_ICM_20600_FACTORY_GYRO_TRIM_ID,
                                                       is_extended_id=False,
                                                       timestamp=global_time_f32,
                                                       data=msg_icm_20600_factory_gyro_trim_data )

        # Scale (according to DLG.dbc) and pack date
        factory_trim_dev_xa_f32 = factory_trim_dev_xa_f32 * 1000
        factory_trim_dev_ya_f32 = factory_trim_dev_ya_f32 * 1000
        factory_trim_dev_za_f32 = factory_trim_dev_za_f32 * 1000

        msg_icm_20600_factory_accl_trim_dev_data = st.pack( '3h', np.int16( factory_trim_dev_xa_f32 ),
                                                            np.int16( factory_trim_dev_ya_f32 ),
                                                            np.int16( factory_trim_dev_za_f32 ) )

        msg_icm_20600_accl_factory_trim_dev = can.Message( arbitration_id=DLG_ICM_20600_FACTORY_ACCL_TRIM_DEV_ID,
                                                           is_extended_id=False,
                                                           timestamp=global_time_f32,
                                                           data=msg_icm_20600_factory_accl_trim_dev_data )

        factory_trim_dev_xg_f32 = factory_trim_dev_xg_f32 * 1000
        factory_trim_dev_yg_f32 = factory_trim_dev_yg_f32 * 1000
        factory_trim_dev_zg_f32 = factory_trim_dev_zg_f32 * 1000

        msg_icm_20600_factory_gyro_trim_dev_data = st.pack( '3h', np.int16( factory_trim_dev_xg_f32 ),
                                                                  np.int16( factory_trim_dev_yg_f32 ),
                                                                  np.int16( factory_trim_dev_zg_f32 ) )

        msg_icm_20600_gyro_factory_trim_dev = can.Message( arbitration_id=DLG_ICM_20600_FACTORY_GYRO_TRIM_DEV_ID,
                                                           is_extended_id=False,
                                                           timestamp=global_time_f32,
                                                           data=msg_icm_20600_factory_gyro_trim_dev_data )


        msg_i2c_error_date = st.pack( '4B h', np.uint8( error_state_u8 ),
                                              np.uint8( access_type_u8 ),
                                              np.uint8( device_addr_u8 ),
                                              np.uint8( register_addr_u8 ),
                                              np.int16( error_code_s16 ) )

        msg_i2c_error = can.Message( arbitration_id=DLG_I2C_ERROR_ID,
                                     is_extended_id=False,
                                     timestamp=global_time_f32,
                                      data=msg_i2c_error_date )





        print( 'Temp:' + str( temperature_deg_f32 ) + " @ time: " + str( global_time_f32 ) )

        # ICM_20600 Messages
        writer.on_message_received( msg_icm_20600_accel )
        writer.on_message_received( msg_icm_20600_gyro )
        writer.on_message_received( msg_icm_20600_general )
        writer.on_message_received( msg_icm_20600_temperature )
        writer.on_message_received( msg_icm_20600_self_test )
        writer.on_message_received( msg_icm_20600_factory_accl_trim )
        writer.on_message_received( msg_icm_20600_factory_gyro_trim )
        writer.on_message_received( msg_icm_20600_accl_factory_trim_dev )
        writer.on_message_received( msg_icm_20600_gyro_factory_trim_dev )

        # I2C Messaegs
        writer.on_message_received( msg_i2c_error )

writer.stop()
