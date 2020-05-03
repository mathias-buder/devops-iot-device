import sys
import os
import re
import can.io.logger
import numpy as np
import struct as st
import ctypes


# CAN Message IDs
# ICM-20600
DLG_ICM_20600_GENERAL_ID                = 0x14 # 20
DLG_ICM_20600_ACCEL_ID                  = 0x15 # 21
DLG_ICM_20600_GYRO_ID                   = 0x16 # 22
DLG_ICM_20600_TEMPERATURE_ID            = 0x17 # 23
DLG_ICM_20600_SELF_TEST_ID              = 0x18 # 24
DLG_ICM_20600_FACTORY_ACCEL_TRIM_ID     = 0x19 # 25
DLG_ICM_20600_FACTORY_GYRO_TRIM_ID      = 0x1A # 26
DLG_ICM_20600_FACTORY_ACCL_TRIM_DEV_ID  = 0x1B # 27
DLG_ICM_20600_FACTORY_GYRO_TRIM_DEV_ID  = 0x1C # 28

# I2C ERROR
DLG_I2C_ERROR_ID                        = 0x32 # 50

DLG_LOG_General                         = 0x3C # 60


def atoi(text):
    return int(text) if text.isdigit() else text

def natural_keys(text):
    '''
    alist.sort(key=natural_keys) sorts in human order
    http://nedbatchelder.com/blog/200712/human_sorting.html
    (See Toothy's implementation in the comments)
    '''
    return [ atoi(c) for c in re.split(r'(\d+)', text) ]


# filename = "E:/ABC123_1.sbf"

# %% Extract all .sbf files
search_path = sys.argv[1]
# search_path = "E:/"

print("Searching in " + search_path + " for .sbf files")

files = []
# r=root, d=directories, f = files
for r, d, f in os.walk(search_path):
    for file in f:
        if '.sbf' in file:
            files.append(os.path.join(r, file))

print("Found " + str(len(files)) + " .sfb files")

# Sort files
files.sort(key=natural_keys, reverse=False)

#for f in files:
#   print(f)

path, base_name = os.path.split(files[0])
blf_file_name = path + os.sep + base_name.split('_')[0] + ".blf"

print("Opening file " + blf_file_name)

# .blf file name
writer = can.io.logger.Logger(blf_file_name)

# CAN channel
writer.channel = 1


# %% Define .sbf file layout
#               time_stamp_f32, DLG_LOG_ICM_20600_DATA   DLG_LOG_I2C_DATA (f 3B H), global_msg_cnt_u8
struct_fmt = '< 14f 10B 6h H                             4B h B x'
struct_len = st.calcsize( struct_fmt )
struct_unpack = st.Struct( struct_fmt ).unpack_from


for sfb_file in files:

    print("Writing content of: ", sfb_file)

    with open( sfb_file, "rb" ) as file:
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
            error_code_s16,

            global_msg_cnt_u8
            ] = struct_unpack( data )

            msg_icm_20600_accel_data = st.pack( '3h', accel_raw_data_x_s16,
                                                    accel_raw_data_y_s16,
                                                    accel_raw_data_z_s16 )

            msg_icm_20600_accel = can.Message( arbitration_id=DLG_ICM_20600_ACCEL_ID,
                                            is_extended_id=False,
                                            timestamp=global_time_f32,
                                            data=msg_icm_20600_accel_data )

            msg_icm_20600_gyro_data = st.pack( '3h', gyro_raw_data_x_s16,
                                                    gyro_raw_data_y_s16,
                                                    gyro_raw_data_z_s16 )

            msg_icm_20600_gyro = can.Message( arbitration_id=DLG_ICM_20600_GYRO_ID,
                                            is_extended_id=False,
                                            timestamp=global_time_f32,
                                            data=msg_icm_20600_gyro_data )

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

            msg_icm_20600_factory_accl_trim_data = st.pack( '3h', np.int16( factory_trim_xa_f32 ),
                                                                np.int16( factory_trim_ya_f32 ),
                                                                np.int16( factory_trim_za_f32 ) )

            msg_icm_20600_factory_accl_trim = can.Message( arbitration_id=DLG_ICM_20600_FACTORY_ACCEL_TRIM_ID,
                                                        is_extended_id=False,
                                                        timestamp=global_time_f32,
                                                        data=msg_icm_20600_factory_accl_trim_data )

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

            # Scale (according to DLG.dbc) and pack date
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


            msg_i2c_error_data = st.pack( '4B h', np.uint8( error_state_u8 ),
                                                np.uint8( access_type_u8 ),
                                                np.uint8( device_addr_u8 ),
                                                np.uint8( register_addr_u8 ),
                                                np.int16( error_code_s16 ) )

            msg_i2c_error = can.Message( arbitration_id=DLG_I2C_ERROR_ID,
                                        is_extended_id=False,
                                        timestamp=global_time_f32,
                                        data=msg_i2c_error_data )

            msg_dlg_log_general_data = st.pack( 'B', np.uint8( global_msg_cnt_u8 ) )

            msg_dlg_log_general = can.Message( arbitration_id=DLG_LOG_General,
                                               is_extended_id=False,
                                               timestamp=global_time_f32,
                                               data=msg_dlg_log_general_data )


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
            writer.on_message_received( msg_dlg_log_general )

            # I2C Messaegs
            writer.on_message_received( msg_i2c_error )

            # print( "Time: " + str( global_time_f32 ) )
writer.stop()

print("Done")
